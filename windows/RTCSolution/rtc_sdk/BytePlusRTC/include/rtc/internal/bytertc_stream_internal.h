/*
 * Copyright 2020 @bytedance
 *
 *  Created on: Mar 16, 2020
 */

#pragma once

#include "../bytertc_engine_interface.h"
#include <stdint.h>
#include <stdarg.h>

namespace bytertc {

/** 
 * @brief ByteRTC SDK start push stream outside room
 * @notes Generally used in the SDK for Linux
 */
BYTERTC_API int startPushStream(void* engine, const char* token, const char* stream_id);

/** 
 * @brief ByteRTC SDK stop push stream outside room
 * @notes Generally used in the SDK for Linux
 */
BYTERTC_API void stopPushStream(void* engine);

class IEngineInternalStreamEventHandler {
public:
    virtual ~IEngineInternalStreamEventHandler() = default;
    virtual void onPushStreamResult(const char* stream_id, int error_code) {}
};

/** 
 * @brief Set the push stream event handler for the current engine
 * @param [in] engine RTC engine for current app
 * @param [in] handler Internal stream event callback handle for the current app
 * @notes You must call this method before start push stream
 */
BYTERTC_API void setEngineInternalStreamEventHandler(void *engine, IEngineInternalStreamEventHandler* handler);

}  // namespace bytertc
