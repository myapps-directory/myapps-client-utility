// myapps/client/utility/auth_file.hpp

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

#include <boost/filesystem.hpp>
#include <chrono>
#include <string>

namespace myapps {
namespace client {
namespace utility {

void auth_write(const boost::filesystem::path& _path,
    const std::string& _endpoint, const std::string& _name,
    const std::string& _token);

void auth_read(const boost::filesystem::path& _path, std::string& _endpoint,
    std::string& _name, std::string& _token);

void auth_update(const boost::filesystem::path& _path,
    std::chrono::system_clock::time_point&      _rwrite_time_point,
    std::string& _endpoint, std::string& _name,
    std::string& _token);

} // namespace utility
} // namespace client
} // namespace myapps