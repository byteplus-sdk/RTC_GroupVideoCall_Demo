// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.core;

import android.util.Pair;

import androidx.annotation.IntDef;

import com.ss.bytertc.engine.type.AudioProfileType;
import com.volcengine.vertcdemo.utils.AppUtil;
import com.volcengine.vertcdemo.groupvideocall.R;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.util.LinkedHashMap;
/**
 * Define the constants used
 *
 * Include:
 * 1. Scenario abbreviation (used to request data from the server, and the front and back ends are unified)
 * 2. Intent transfer data key
 * 3. Volume detection related value
 * 4. Resolution copywriting and value mapping
 * 5. Sound quality copywriting and value mapping
 * 6. Default resolution and sound quality definition
 * 7. Definition of media-related constants
 */
public class Constants {
    // solution name
    public static final String SOLUTION_NAME_ABBR = "videocall";

    public static final String EXTRA_ROOM_ID = "room_id";
    public static final String EXTRA_TOKEN = "token";
    public static final String EXTRA_LAST_TS = "last_ts";
    // During volume detection, consider the volume threshold of the user speaking
    public static final int VOLUME_SPEAKING_THRESHOLD = 10;
    // volume detection interval
    public static final int VOLUME_SPEAKING_INTERVAL = 2000;
    // Mapping of resolution text and specific value
    public static final LinkedHashMap<String, Pair<Integer, Integer>> RESOLUTION_MAP
            = new LinkedHashMap<String, Pair<Integer, Integer>>() {
        {
            put("720*1280", new Pair<>(720, 1280));
            put("540*960", new Pair<>(540, 960));
            put("360*640", new Pair<>(360, 640));
            put("180*320", new Pair<>(180, 320));
        }
    };
    // audio quality copy and RTC range mapping
    public static final LinkedHashMap<String, AudioProfileType> QUALITY_MAP
            = new LinkedHashMap<String, AudioProfileType>() {
        {
            put(AppUtil.getApplicationContext().getString(R.string.clarity),
                    AudioProfileType.AUDIO_PROFILE_FLUENT);
            put(AppUtil.getApplicationContext().getString(R.string.high_definition),
                    AudioProfileType.AUDIO_PROFILE_STANDARD);
            put(AppUtil.getApplicationContext().getString(R.string.extreme),
                    AudioProfileType.AUDIO_PROFILE_HD);
        }
    };
    // Default resolution definition
    public static final String DEFAULT_RESOLUTION = "720*1280";
    // Default sound quality definition
    public static final String DEFAULT_QUALITY = AppUtil.getApplicationContext()
            .getString(R.string.high_definition);
    // media state definition
    public static final int MEDIA_STATUS_ON = 1;
    public static final int MEDIA_STATUS_OFF = 0;

    @IntDef({MEDIA_STATUS_ON, MEDIA_STATUS_OFF})
    @Retention(RetentionPolicy.SOURCE)
    public @interface MediaStatus {
    }
    // media type definition
    public static final int MEDIA_TYPE_VIDEO = 0;
    public static final int MEDIA_TYPE_AUDIO = 1;

    @IntDef({MEDIA_TYPE_VIDEO, MEDIA_TYPE_AUDIO})
    @Retention(RetentionPolicy.SOURCE)
    public @interface MediaType {

    }
}
