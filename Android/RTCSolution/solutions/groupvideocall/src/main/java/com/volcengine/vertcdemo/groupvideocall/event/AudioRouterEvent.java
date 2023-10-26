// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.event;
/**
 * Audio route switching event, used to update UI
 */
public class AudioRouterEvent {

    public final boolean isSpeakerPhone;

    public AudioRouterEvent(boolean isSpeakerPhone) {
        this.isSpeakerPhone = isSpeakerPhone;
    }
}
