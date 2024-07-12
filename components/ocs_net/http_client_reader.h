/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "esp_http_client.h"

#include "ocs_core/cond.h"
#include "ocs_core/noncopyable.h"
#include "ocs_net/http_client_builder.h"

namespace ocs {
namespace net {

class HTTPClientReader : public core::NonCopyable<> {
public:
    struct Params {
        //! Domain or IP as string.
        std::string host;

        //! HTTP Path, if not set, default is `/`.
        std::string path;

        //! Underlying buffer size for reading the response.
        unsigned bufsize { 128 };
    };

    //! Initialize.
    explicit HTTPClientReader(const Params& params);

    //! Return the underlying HTTP client handle.
    esp_http_client_handle_t client() const;

    //! Wait for the response.
    //!
    //! @return
    //!  True if the response is received withing the @p wait interval.
    bool wait(TickType_t wait = portMAX_DELAY);

    //! Read the response.
    //!
    //! @return
    //!  Number of bytes read.
    unsigned read(char* buf, unsigned size);

private:
    static esp_err_t handle_event_(esp_http_client_event_t* event);

    void handle_event_on_data_(esp_http_client_event_t* event);
    void handle_event_on_finish_();

    const Params params_;

    esp_http_client_config_t config_;
    HTTPClientSharedPtr client_;

    core::StaticMutex mu_;
    core::Cond cond_;

    bool data_received_ { false };
    std::unique_ptr<char[]> buf_;
};

} // namespace net
} // namespace ocs
