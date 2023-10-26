// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.event;

import com.ss.bytertc.engine.data.StreamIndex;
/**
 * Volume event, used to update the UI
 */
public class AudioPropertiesReportEvent {

    public StreamIndex streamIndex;
    public String uid;
    public boolean isSpeaking;

    public AudioPropertiesReportEvent(StreamIndex streamIndex, String uid, boolean isSpeaking) {
        this.streamIndex = streamIndex;
        this.uid = uid;
        this.isSpeaking = isSpeaking;
    }
}
