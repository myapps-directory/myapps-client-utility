#pragma once
#ifdef WIN32
#include <Windows.h>
#endif
#include <string>

namespace myapps {
namespace client {
namespace utility {
#ifdef WIN32
inline std::string narrow(const std::wstring& wstr)
{
    std::string ret;

    auto retval = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    if (retval <= 0) {
        return std::string{};
    }
    ret.resize(retval + 1);
    retval = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), &ret[0], retval, nullptr, nullptr);
    if (retval <= 0) {
        return std::string{};
    }
    ret.resize(retval);
    return ret;
}

inline std::wstring widen(const std::string& str)
{
    std::wstring ret;
    
    auto retval = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
    if (retval <= 0) {
        return std::wstring{};
    }
    ret.resize(retval + 1);
    retval = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), &ret[0], retval);
    if (retval <= 0) {
        return std::wstring{};
    }
    ret.resize(retval);
    return ret;
}
#endif
} // namespace utility
} // namespace client
} // namespace myapps