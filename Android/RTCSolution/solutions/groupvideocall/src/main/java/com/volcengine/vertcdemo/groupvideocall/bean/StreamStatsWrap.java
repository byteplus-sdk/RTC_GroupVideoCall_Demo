// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.bean;

import com.ss.bytertc.engine.type.RemoteAudioStats;
import com.ss.bytertc.engine.type.RemoteStreamStats;
import com.ss.bytertc.engine.type.RemoteVideoStats;

public class StreamStatsWrap {
    public final String uid;
    public final int quality;

    public RemoteAudioStats audioStats;
    public RemoteVideoStats videoStats;

    StreamStatsWrap(RemoteStreamStats stats, int quality) {
        this.uid = stats.uid;
        this.quality = quality;
        this.audioStats = stats.audioStats;
        this.videoStats = stats.videoStats;
    }

    public static StreamStatsWrap from(RemoteStreamStats stats, int quality) {
        return new StreamStatsWrap(stats, quality);
    }
}
