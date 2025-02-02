// myapps/client/utility/locale.hpp

// This file is part of MyApps.directory project
// Copyright (C) 2020, 2021, 2022, 2023, 2024, 2025 Valentin Palade (vipalade @ gmail . com)

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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