#include "ocs_status/code_to_str.h"

namespace ocs {
namespace status {

const char* code_to_str(StatusCode status) {
    switch (status) {
    case StatusCode::OK:
        return "OK";
    case StatusCode::Error:
        return "Error";

    default:
        break;
    }

    return "<none>";
}

} // namespace status
} // namespace ocs
