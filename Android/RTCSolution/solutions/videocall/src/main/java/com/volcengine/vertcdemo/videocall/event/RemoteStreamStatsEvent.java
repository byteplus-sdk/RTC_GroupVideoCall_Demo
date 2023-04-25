// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.event;

import com.ss.bytertc.engine.type.RemoteStreamStats;
/**
 * Remote audio and video stream reporting events for UI display
 */
public class RemoteStreamStatsEvent {

    public final RemoteStreamStats remoteStreamStats;

    public RemoteStreamStatsEvent(RemoteStreamStats remoteStreamStats) {
        this.remoteStreamStats = remoteStreamStats;
    }
}
