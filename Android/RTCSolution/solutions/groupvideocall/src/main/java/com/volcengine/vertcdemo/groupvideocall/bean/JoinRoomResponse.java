// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.bean;

import com.google.gson.annotations.SerializedName;
import com.volcengine.vertcdemo.core.net.rts.RTSBizResponse;
/**
 * Join the business server room interface to return the model
 */
public class JoinRoomResponse implements RTSBizResponse {
    // room duration, unit:second
    @SerializedName("duration")
    public long durationS;
    // Token to join the rtc room
    @SerializedName("rtc_token")
    public String rtcToken;

    @Override
    public String toString() {
        return "JoinRoomResponse{" +
                "durationS=" + durationS +
                ", rtcToken='" + rtcToken + '\'' +
                '}';
    }
}
