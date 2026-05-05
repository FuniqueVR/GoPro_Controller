/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once
#include <string>
#include "hv/WebSocketClient.h"

/**
 * Basically holds the Websocket instance
 */
struct ServerConnection {
    /**
     * Websocket IP
     */
    std::string ip;
    /**
     * The libhv websocket client
     */
    std::shared_ptr<hv::WebSocketClient> client;
    /**
     * Current connection state
     */
    bool connected = false;
    /**
     * last message received from the server
     */
    std::string last_message;
};