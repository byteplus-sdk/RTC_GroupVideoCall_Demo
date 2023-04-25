// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.event;

import com.volcengine.vertcdemo.videocall.bean.VideoCallUserInfo;
/**
 * The user in the room joins and leaves the event
 */
public class RoomUserEvent {

    public final boolean isJoin;
    public final VideoCallUserInfo userInfo;

    public RoomUserEvent(VideoCallUserInfo userInfo, boolean isJoin) {
        this.userInfo = userInfo;
        this.isJoin = isJoin;
    }
}
