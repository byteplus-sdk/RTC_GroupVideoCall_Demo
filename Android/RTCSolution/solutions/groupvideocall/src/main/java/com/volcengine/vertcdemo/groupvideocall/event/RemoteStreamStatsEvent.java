// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.event;

import com.ss.bytertc.engine.type.RemoteStreamStats;
import com.volcengine.vertcdemo.groupvideocall.bean.StreamStatsWrap;
/**
 * Remote audio and video stream reporting events for UI display
 */
public class RemoteStreamStatsEvent {

    public final StreamStatsWrap stats;

    public RemoteStreamStatsEvent(RemoteStreamStats stats, int quality) {
        this.stats = StreamStatsWrap.from(stats, quality);
    }
}
