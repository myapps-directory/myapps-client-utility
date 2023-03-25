#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <ole2.h>
#include <winioctl.h>

#include "myapps/client/utility/locale.hpp"
#include "myapps/client/utility/auth_file.hpp"
#include <boost/filesystem.hpp>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace std;
namespace fs = boost::filesystem;
namespace {

std::string get_file_guid(const fs::path& _path)
{
    std::string retval;
    HANDLE h = CreateFileA(_path.generic_string().c_str(), 0,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
        OPEN_EXISTING, 0, NULL);
    if (h != INVALID_HANDLE_VALUE) {
        FILE_OBJECTID_BUFFER buf;
        DWORD                cbOut;
        if (DeviceIoControl(h, FSCTL_CREATE_OR_GET_OBJECT_ID,
                NULL, 0, &buf, sizeof(buf),
                &cbOut, NULL)) {
            GUID guid;
            CopyMemory(&guid, &buf.ObjectId, sizeof(GUID));
            WCHAR szGuid[39];
            StringFromGUID2(guid, szGuid, 39);
            retval = myapps::client::utility::narrow(szGuid);
        }
    }
    return retval;
}
} // namespace
int test_file_guid(int argc, char* argv[])
{
    boost::system::error_code err;
    fs::remove("test.auth", err);

    cout << "File guid = " << get_file_guid("test.auth") << endl;

    myapps::client::utility::auth_write("test.auth", "localhost", "gigel", "token");

    cout << "File guid = " << get_file_guid("test.auth")<<endl;
    chrono::system_clock::time_point tp;
    string                           endpoint;
    string                           name;
    string                           token;
    myapps::client::utility::auth_update("test.auth", tp, endpoint, name, token);
    cout << "File guid = " << get_file_guid("test.auth")<<endl;

    fs::rename("test.auth", "test2.auth", err);
    cout << "File guid = " << get_file_guid("test.auth") << endl;
    cout << "File guid = " << get_file_guid("test2.auth") << endl;

    return 0;
}