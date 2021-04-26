#include "solid/system/common.hpp"
#include "solid/system/error.hpp"
#include "solid/system/log.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/deque.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/set.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>

#include "ola/client/utility/app_list_file.hpp"
#include "ola/client/utility/auth_file.hpp"
#include "ola/common/utility/encode.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>

using namespace std;

namespace fs = boost::filesystem;

namespace ola {
namespace client {
namespace utility {

namespace {

const solid::LoggerT logger("ola::client::utility::file");

// trim from start (in place)
static inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                [](int ch) { return !std::isspace(ch); })
                .base(),
        s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}

#if defined(SOLID_ON_WINDOWS)
void write_with_retry(const boost::filesystem::path& _path,
    const string&                                    _data)
{
    while (true) {
        // Open the existing file.
        auto hFile = CreateFile(TEXT(_path.generic_string().c_str()),
            GENERIC_WRITE, // open for writing
            0, // do not share
            NULL, // no security
            CREATE_ALWAYS, // existing file only
            FILE_ATTRIBUTE_NORMAL, // normal file
            NULL); // no attr. template

        if (hFile != INVALID_HANDLE_VALUE) {
            OVERLAPPED overlapped;
            overlapped.Offset     = 0;
            overlapped.OffsetHigh = 0;
            overlapped.hEvent     = 0;

            if (LockFileEx(hFile, LOCKFILE_EXCLUSIVE_LOCK, 0, _data.size(), 0,
                    &overlapped)) {
                DWORD writen = 0;
                WriteFile(hFile, _data.data(), _data.size(), &writen, nullptr);
                UnlockFile(hFile, 0, 0, _data.size(), 0);
            }
            CloseHandle(hFile);
            return;
        } else {
            const auto err = GetLastError();
            const auto msg = solid::last_system_error().message();
            solid_log(logger, Error,
                "CreateFile failed (" << _path.generic_string()
                                      << "): " << msg);
            if (err == ERROR_SHARING_VIOLATION) {
                this_thread::sleep_for(chrono::milliseconds(10));
            } else {
                return;
            }
        }
    }
}

void read_with_retry(const boost::filesystem::path& _path, string& _data)
{
    char buf[4096];

    while (true) {
        // Open the existing file.
        auto hFile = CreateFile(TEXT(_path.generic_string().c_str()),
            GENERIC_READ, // open for reading
            FILE_SHARE_READ,
            NULL, // no security
            OPEN_EXISTING, // existing file only
            FILE_ATTRIBUTE_NORMAL, // normal file
            NULL); // no attr. template

        if (hFile != INVALID_HANDLE_VALUE) {
            OVERLAPPED overlapped;
            overlapped.Offset     = 0;
            overlapped.OffsetHigh = 0;
            overlapped.hEvent     = 0;

            if (LockFileEx(hFile, LOCKFILE_EXCLUSIVE_LOCK, 0, 4096, 0, &overlapped)) {
                DWORD read_count = 0;
                do {
                    ReadFile(hFile, buf, 4096, &read_count, nullptr);
                    if (read_count > 0) {
                        _data.append(buf, read_count);
                    }
                } while (read_count > 0);
                UnlockFile(hFile, 0, 0, 4096, 0);
            }
            CloseHandle(hFile);
            return;
        } else {
            const auto err = GetLastError();
            const auto msg = solid::last_system_error().message();
            solid_log(logger, Error,
                "CreateFile failed (" << _path.generic_string()
                                      << "): " << msg);
            if (err == ERROR_SHARING_VIOLATION) {
                this_thread::sleep_for(chrono::milliseconds(10));
            } else {
                return;
            }
        }
    }
}

#endif
} // namespace

void auth_write(const boost::filesystem::path& _path,
    const std::string& _endpoint, const std::string& _name,
    const std::string& _token)
{
#if !defined(SOLID_ON_WINDOWS)
    ofstream ofs(_path.generic_string(), std::ios::trunc);
    if (ofs) {
        ofs << _endpoint << endl;
        ofs << _name << endl;
        ofs << ola::utility::base64_encode(_token) << endl;
        ofs.flush();
    }
#else

    string out;
    {
        ostringstream oss;

        oss << _endpoint << endl;
        oss << _name << endl;
        oss << ola::utility::base64_encode(_token) << endl;

        out = oss.str();
    }

    write_with_retry(_path, out);
#endif
}

void auth_read(const boost::filesystem::path& _path, std::string& _rendpoint,
    std::string& _rname, std::string& _rtoken)
{
#if !defined(SOLID_ON_WINDOWS)
    ifstream ifs(_path.generic_string());
    if (ifs) {
        getline(ifs, _rendpoint);
        getline(ifs, _rname);
        getline(ifs, _rtoken);
        try {
            _rtoken = ola::utility::base64_decode(_rtoken);
        } catch (std::exception&) {
            _rendpoint.clear();
            _rname.clear();
            _rtoken.clear();
        }
    }
#else
    string data;
    read_with_retry(_path, data);
    if (!data.empty()) {
        istringstream iss(data);
        getline(iss, _rendpoint);
        getline(iss, _rname);
        getline(iss, _rtoken);
        trim(_rendpoint);
        trim(_rname);
        trim(_rtoken);
        try {
            _rtoken = ola::utility::base64_decode(_rtoken);
        } catch (std::exception&) {
            _rendpoint.clear();
            _rname.clear();
            _rtoken.clear();
        }
    }
#endif
}

void auth_update(const boost::filesystem::path& _path,
    std::chrono::system_clock::time_point&      _rwrite_time_point,
    std::string& _endpoint, std::string& _name,
    std::string& _token)
{
    boost::system::error_code              err;
    const chrono::system_clock::time_point empty_time_point;
    const auto                             write_time_point = chrono::system_clock::from_time_t(fs::last_write_time(_path, err));

    if (_rwrite_time_point == empty_time_point || _rwrite_time_point == write_time_point) {
        auth_write(_path, _endpoint, _name, _token);
        _rwrite_time_point = chrono::system_clock::from_time_t(fs::last_write_time(_path, err));
        auto tt            = chrono::system_clock::to_time_t(_rwrite_time_point);
        solid_log(logger, Info,
            "auth_update (" << _path.generic_string()
                            << "): " << err.message() << " "
                            << std::put_time(std::localtime(&tt), "%F %T"));
    } else {
        auto tt1 = chrono::system_clock::to_time_t(_rwrite_time_point);
        auto tt2 = chrono::system_clock::to_time_t(write_time_point);
        solid_log(logger, Warning,
            "auth_update skipped ("
                << _path.generic_string() << "): " << err.message() << " "
                << std::put_time(std::localtime(&tt1), "%F %T") << " "
                << std::put_time(std::localtime(&tt2), "%F %T"));
    }
}

void AppListFile::store(const boost::filesystem::path& _path)
{
#if !defined(SOLID_ON_WINDOWS)
#else
    ostringstream oss;
    try {
        cereal::BinaryOutputArchive a(oss);
        a(app_map_);
    } catch (...) {
        return;
    }
    write_with_retry(_path, oss.str());
#endif
}

void AppListFile::load(const boost::filesystem::path& _path)
{
#if !defined(SOLID_ON_WINDOWS)
#else
    app_map_.clear();
    string data;
    read_with_retry(_path, data);

    if (!data.empty()) {
        istringstream iss(data);
        try {
            cereal::BinaryInputArchive a(iss);
            a(app_map_);
        } catch (...) {
            return;
        }
    }
#endif
}

} // namespace utility
} // namespace client
} // namespace ola