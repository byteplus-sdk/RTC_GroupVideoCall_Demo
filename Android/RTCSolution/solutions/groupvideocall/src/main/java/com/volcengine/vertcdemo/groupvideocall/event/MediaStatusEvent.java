// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.event;

import com.volcengine.vertcdemo.groupvideocall.core.Constants;
/**
 * Media state switching event, used for UI display
 */
public class MediaStatusEvent {

    public String uid;
    public final @Constants.MediaType int mediaType;
    public final @Constants.MediaStatus int status;

    public MediaStatusEvent(String uid,
            @Constants.MediaType int mediaType, @Constants.MediaStatus int status) {
        this.uid = uid;
        this.status = status;
        this.mediaType = mediaType;
    }
}
