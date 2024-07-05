#pragma once

#include <memory>

#include "cJSON.h"

namespace ocs {
namespace core {

template <typename T> struct cJSONBuilder {
    using Ptr = T;

    //! Make cJSON object.
    static T make_json() {
        return { cJSON_CreateObject(), cJSON_Delete };
    }

    //! Make cJSON array.
    static T make_json_array() {
        return { cJSON_CreateArray(), cJSON_Delete };
    }

    //! Make cJSON number.
    static T make_json_number(double value) {
        return { cJSON_CreateNumber(value), cJSON_Delete };
    }

    //! Make cJSON string.
    static T make_json_string(const char* str) {
        return { cJSON_CreateString(str), cJSON_Delete };
    }

    //! Make cJSON string reference.
    static T make_json_string_ref(const char* str) {
        return { cJSON_CreateStringReference(str), cJSON_Delete };
    }

    //! Make cJSON bool.
    static T make_json_bool(bool value) {
        return { cJSON_CreateBool(value), cJSON_Delete };
    }

    //! Make cJSON null.
    static T make_json_null() {
        return { cJSON_CreateNull(), cJSON_Delete };
    }

    //! Parse cJSON.
    static T parse_json(const char* str) {
        return { cJSON_Parse(str), cJSON_Delete };
    }
};

using cJSONUniqueBuilder = cJSONBuilder<std::unique_ptr<cJSON, decltype(&cJSON_Delete)>>;
using cJSONSharedBuilder = cJSONBuilder<std::shared_ptr<cJSON>>;

} // namespace core
} // namespace ocs
