// myapps/client/utility/app_list_file.hpp

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

#include "myapps/common/utility/protocol.hpp"
#include <boost/filesystem.hpp>
#include <cereal/cereal.hpp>
#include <chrono>
#include <string>
#include <unordered_map>

namespace myapps {
namespace client {
namespace utility {

class AppListFile {
    using AppMapT = std::unordered_map<std::string, myapps::utility::AppItemEntry>;

    AppMapT app_map_;

public:
    static constexpr uint32_t version = 1;
    using AppItemEntry                = myapps::utility::AppItemEntry;

    void store(const boost::filesystem::path& _path);
    void load(const boost::filesystem::path& _path);

    void insert(const std::string& _app_id, const AppItemEntry& _build_entry)
    {
        app_map_[_app_id] = _build_entry;
    }

    void erase(const std::string& _app_id) { app_map_.erase(_app_id); }

    AppItemEntry find(const std::string& _app_id) const
    {
        auto it = app_map_.find(_app_id);
        if (it != app_map_.end()) {
            return it->second;
        }
        return AppItemEntry{};
    }

    template <class F>
    void visit(F _f) const
    {
        for (const auto& kv : app_map_) {
            _f(kv.first, kv.second);
        }
    }
    void clear() { app_map_.clear(); }
    template <class Archive>
    void serialize(Archive& _a, std::uint32_t const _version)
    {
        solid_assert(version == _version);
        _a(app_map_);
    }
};

} // namespace utility
} // namespace client
} // namespace myapps

CEREAL_CLASS_VERSION(myapps::client::utility::AppListFile,
    myapps::client::utility::AppListFile::version);