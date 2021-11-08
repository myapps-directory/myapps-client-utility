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