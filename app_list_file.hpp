#pragma once

#include <cereal/cereal.hpp>
#include <boost/filesystem.hpp>
#include "ola/common/utility/ola_protocol.hpp"
#include <string>
#include <unordered_map>
#include <chrono>

namespace ola{
namespace client{
namespace utility{

class AppListFile{
    using AppMapT = std::unordered_map<std::string, ola::utility::BuildEntry>;
    
    AppMapT app_map_;
public:
    static constexpr uint32_t version = 1;
    using BuildEntry = ola::utility::BuildEntry;

    void store(const boost::filesystem::path &_path);
    void load(const boost::filesystem::path &_path);

    void insert(const std::string &_app_id, const BuildEntry &_build_entry){
        app_map_[_app_id] = _build_entry;
    }

    void erase(const std::string &_app_id){
        app_map_.erase(_app_id);
    }

    template <class F>
    void visit(F _f)const{
        for(const auto& kv: app_map_){
            _f(kv.first, kv.second);
        }
    }
    void clear(){
        app_map_.clear();
    }
    template <class Archive>
    void serialize(Archive& _a, std::uint32_t const _version)
    {
        solid_assert(version == _version);
        _a(app_map_);
    }
};

}//namespace utility
}//namespace client
}//namespace ola

CEREAL_CLASS_VERSION(ola::client::utility::AppListFile, ola::client::utility::AppListFile::version);