// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.bean;

import com.google.gson.annotations.SerializedName;
import com.volcengine.vertcdemo.core.net.rts.RTSBizResponse;
/**
 * Reconnect the business server room interface to return the model
 */
public class ReconnectResponse implements RTSBizResponse {
    // Token to join the rtc room
    @SerializedName("rtc_token")
    public String rtcToken;

    @Override
    public String toString() {
        return "ReconnectResponse{" +
                "rtcToken='" + rtcToken + '\'' +
                '}';
    }
}
