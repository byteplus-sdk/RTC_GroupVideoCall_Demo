// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.event;

import com.ss.bytertc.engine.type.LocalStreamStats;
/**
 * Local audio and video streams report events for UI display
 */
public class LocalStreamStatsEvent {

    public final LocalStreamStats localStreamStats;

    public LocalStreamStatsEvent(LocalStreamStats localStreamStats) {
        this.localStreamStats = localStreamStats;
    }
}
