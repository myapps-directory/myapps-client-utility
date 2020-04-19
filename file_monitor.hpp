#pragma once
#include "solid/system/pimpl.hpp"
#include "solid/system/common.hpp"
#include <boost/filesystem.hpp>
#include <functional>
#include <chrono>

namespace ola {
namespace client {
namespace utility {

class FileMonitor: solid::NonCopyable{
    struct Implementation;
    solid::PimplT<Implementation>   pimpl_;
public:
    using OnChangeFunctionT = std::function<void(const boost::filesystem::path &, const boost::filesystem::path &, const std::chrono::system_clock::time_point&)>;

    FileMonitor();
    ~FileMonitor();

    void start();
    void stop();
    void add(const boost::filesystem::path &_file_path, OnChangeFunctionT &&);
};

} //namespace utility
} //namespace client
} //namespace ola