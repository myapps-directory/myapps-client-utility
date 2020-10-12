#pragma once

#include "ola/client/utility/version_impl.hpp"

namespace ola {
namespace client {
namespace utility {

const char* version_vcs_commit();
const char* version_vcs_branch();
const char* version_full();

} //namespace utility
} //namespace client
} //namespace ola
