// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.event;

import com.google.gson.annotations.SerializedName;
import com.volcengine.vertcdemo.core.net.rts.RTSBizInform;
/**
 * Room end event, used to close the room page
 */
public class RoomFinishEvent implements RTSBizInform {

    @SerializedName("room_id")
    public String roomId;

    @Override
    public String toString() {
        return "RoomFinishEvent{" +
                "roomId='" + roomId + '\'' +
                '}';
    }
}
