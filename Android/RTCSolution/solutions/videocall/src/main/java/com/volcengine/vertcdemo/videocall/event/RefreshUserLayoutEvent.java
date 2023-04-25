// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.event;

import com.volcengine.vertcdemo.videocall.bean.VideoCallUserInfo;

import java.util.List;
/**
 * Refresh the user list event in the room
 */
public class RefreshUserLayoutEvent {

    public final List<VideoCallUserInfo> userInfoList;

    public RefreshUserLayoutEvent(List<VideoCallUserInfo> userInfoList) {
        this.userInfoList = userInfoList;
    }
}
