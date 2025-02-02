// myapps/client/utility/file_monitor.hpp

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
#include "solid/system/common.hpp"
#include "solid/system/pimpl.hpp"
#include <boost/filesystem.hpp>
#include <chrono>
#include <functional>

namespace myapps {
namespace client {
namespace utility {

class FileMonitor : solid::NonCopyable {
    struct Implementation;
    solid::PimplT<Implementation> pimpl_;

public:
    using OnChangeFunctionT = std::function<void(
        const boost::filesystem::path&, const boost::filesystem::path&,
        const std::chrono::system_clock::time_point&)>;

    FileMonitor();
    ~FileMonitor();

    void start();
    void stop();
    void add(const boost::filesystem::path& _file_path, OnChangeFunctionT&&);
};

} // namespace utility
} // namespace client
} // namespace myapps