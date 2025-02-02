// myapps/client/utility/arrvec.hpp

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

#include <cassert>
#include <vector>

namespace myapps {
namespace client {
namespace utility {

template <size_t ArrSz, typename T>
class ArrVec {
    using VecT   = std::vector<T>;
    size_t size_ = 0;
    T      arr_[ArrSz];
    VecT   vec_;

public:
    size_t size() const { return size_; }
    bool   empty() const { return size_ == 0; }
    T&     operator[](const size_t _i)
    {
        assert(_i < size_);
        if (_i < ArrSz) {
            return arr_[_i];
        } else {
            return vec_[_i - ArrSz];
        }
    }
    const T& operator[](const size_t _i) const
    {
        assert(_i < size_);
        if (_i < ArrSz) {
            return arr_[_i];
        } else {
            return vec_[_i - ArrSz];
        }
    }

    template <typename... Args>
    void emplace_back(Args&&... _args)
    {
        if (size_ < ArrSz) {
            arr_[size_] = T(std::forward<Args>(_args)...);
        } else {
            vec_.emplace_back(std::forward<Args>(_args)...);
        }
        ++size_;
    }

    T&       back() { return (*this)[size_ - 1]; }
    const T& back() const { return (*this)[size_ - 1]; }

    void clear()
    {
        size_ = 0;
        vec_.clear();
    }
};

} // namespace utility
} // namespace client
} // namespace myapps