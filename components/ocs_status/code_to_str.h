#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace status {

//! Format human readable status description.
const char* code_to_str(StatusCode status);

} // namespace status
} // namespace ocs
