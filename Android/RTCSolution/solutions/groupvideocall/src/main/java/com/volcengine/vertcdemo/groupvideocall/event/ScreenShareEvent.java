// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.event;
/**
 * The user in the room starts and ends the screen sharing event
 */
public class ScreenShareEvent {

    public boolean isStart;

    public ScreenShareEvent(boolean isStart) {
        this.isStart = isStart;
    }
}
