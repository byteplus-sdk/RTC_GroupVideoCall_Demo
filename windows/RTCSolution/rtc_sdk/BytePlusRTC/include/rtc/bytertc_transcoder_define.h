/*
 * Copyright 2021 @bytedance
 *  Created on: june 20, 2021
 */

#pragma once
#include "bytertc_video_frame.h"
#include "bytertc_audio_frame.h"
#include "bytertc_transcoder_base_interface.h"

namespace bytertc {
/** 
 * @type keytype
 * @brief Events during pushing streams to CDN
 */
enum StreamMixingEvent {
    /**
     * @hidden for internal use only
     */
    kStreamMixingEventBase = 0,
    /** 
     * @brief Request to start pushing streams to CDN
     */
    kStreamMixingEventStart = 1,
    /** 
     * @brief ask to push streams to CDN started
     */
    kStreamMixingEventStartSuccess = 2,
    /** 
     * @brief Failed to start the task to push streams to CDN
     */
    kStreamMixingEventStartFailed = 3,
    /** 
     * @brief Request to update the configuration for the task to push streams to CDN
     */
    kStreamMixingEventUpdate = 4,
    /** 
     * @brief Successfully update the configuration for the task to push streams to CDN
     */
    kStreamMixingEventUpdateSuccess = 5,
    /** 
     * @brief Failed to update the configuration for the task to push streams to CDN
     */
    kStreamMixingEventUpdateFailed = 6,
    /** 
     * @brief Request to stop the task to push streams to CDN
     */
    kStreamMixingEventStop = 7,
    /** 
     * @brief The task to push streams to CDN stopped
     */
    kStreamMixingEventStopSuccess = 8,
    /** 
     * @brief Failed to stop the task to push streams to CDN
     */
    kStreamMixingEventStopFailed = 9,
    /** 
     * @brief Timeout for the request to update the configuration for the task to push streams to CDN.
     */
    kStreamMixingEventChangeMixType = 10,
    /** 
     * @brief Got the first frame of the mixed audio stream by client.
     */
    kStreamMixingEventFirstAudioFrameByClientMix = 11,
    /** 
     * @brief Got the first frame of the mixed video stream by client.
     */
    kStreamMixingEventFirstVideoFrameByClientMix = 12,
    /** 
     * @brief Timeout for the request to update the configuration for the task to push streams to CDN
     */
    kStreamMixingEventUpdateTimeout = 13,
    /** 
     * @brief Timeout for the request to start the task to push streams to CDN
     */
    kStreamMixingEventStartTimeout = 14,
    /** 
     * @brief Error in the parameters of the request for the task to push streams to CDN
     */
    kStreamMixingEventRequestParamError = 15,
    /** 
     * @brief Mixing image.
     */
    kStreamMixingEventMixImageEvent = 16,
    /** 
     * @hidden internal use only
     * @hidden currently not available
     * @brief Simplex chorus event
     */
    kStreamMixingEventMixSingleWayChorusEvent = 17,
};


/** 
 * @type errorcode
 * @brief Errors occurring during pushing a single stream to CDN
 */
enum SingleStreamPushEvent {
    /**
     * @hidden for internal use only
     */
    kSingleStreamPushEventBase = 0,
    /** 
    * @brief Starting pushing a single stream to CDN.
    */
    kSingleStreamPushEventStart = 1,
    /** 
    * @brief Successfully pushed a single stream to CDN.
    */
    kSingleStreamPushEventSuccess = 2,
    /** 
    * @brief Failed to push a single stream to CDN.
    */
    kSingleStreamPushEventFailed = 3,
    /** 
     * @brief Stop pushing a single stream to CDN.
     */
    kSingleStreamPushEventStop = 4,
    /** 
    * @brief Request timed out. Please check network status and retry.
    */
    kSingleStreamPushEventTimeout = 5,
    /** 
     * @brief Request failed due to invalid parameter.
     */
    kSingleStreamPushEventParamError = 6,
};

/** 
 * @type errorcode
 * @brief Errors occurring during pushing streams to CDN
 */
enum StreamMixingErrorCode {
    /** 
     * @brief Successfully pushed streams to target CDN.
     */
    kStreamMixingErrorCodeOK = 0,
    /** 
     * @brief Undefined error
     */
    kStreamMixingErrorCodeBase= 1090,
    /** 
     * @brief Invalid parameters detected by Client SDK.
     */
    kStreamMixingErrorCodeInvalidParam = 1091,
    /** 
     * @brief Program runs with an error, the state machine is in abnormal condition.
     */
    kStreamMixingErrorCodeInvalidState = 1092,
    /** 
     * @brief Invalid operation
     */
    kStreamMixingErrorCodeInvalidOperator = 1093,
    /** 
     * @brief Request timed out. Please check network status and retry.
     */
    kStreamMixingErrorCodeTimeout = 1094,
    /** 
     * @brief Invalid parameters detected by the server
     */
    kStreamMixingErrorCodeInvalidParamByServer = 1095,
    /** 
     * @brief Subscription to the stream has expired.
     */
    kStreamMixingErrorCodeSubTimeoutByServer = 1096,
    /** 
     * @brief Internal server error.
     */
    kStreamMixingErrorCodeInvalidStateByServer = 1097,
    /** 
     * @brief The server failed to push streams to CDN.
     */
    kStreamMixingErrorCodeAuthenticationByCDN  = 1098,
    /** 
     * @brief Signaling connection timeout error. Please check network status and retry.
     */
    kStreamMixingErrorCodeTimeoutBySignaling = 1099,
    /** 
     * @brief Failed to mix image.
     */
    kStreamMixingErrorCodeMixImageFail = 1100,
    /** 
     * @brief Unknown error from server.
     */
    kStreamMixingErrorCodeUnKnownByServer = 1101,
    /** 
     * @hidden internal use only
     * @brief The cache is not synchronized.
     */
    kStreamMixingErrorCodeStreamSyncWorse = 1102,
    /** 
     * @hidden for internal use only
     * @brief The ‘regions’ field in transcoding message is changed.
     */
    kStreamMixingErrorCodeUpdateRegionChanged = 1103,
    /**
     * @hidden for internal use only
     */
    kStreamMixingErrorCodeMax = 1199,
};

/** 
 * @type keytype
 * @brief Stream mixing type.(New)
 */
enum MixedStreamType {
    /** 
     * @brief Mix media streams on the server side.
     */
    kMixedStreamTypeByServer = 0,
    /** 
     * @brief Intelligent stream mixing. The SDK will intelligently decide that a stream mixing task would be done on the client or the server.
     *        Before using the option, please contact the technical support. Otherwise, the option does not take effect.
     */
    kMixedStreamTypeByClient = 1,
};


/** 
 * @type keytype
 * @brief Advanced Audio Coding (AAC) profile.(New)
 */
enum MixedStreamAudioProfile {
    /** 
     * @brief (Default) AAC Low-Complexity profile (AAC-LC).
     */
    kMixedStreamAudioProfileLC = 0,
    /** 
     * @brief HE-AAC v1 profile (AAC LC with SBR).
     */
    kMixedStreamAudioProfileHEv1 = 1,
    /** 
     * @brief HE-AAC v2 profile (AAC LC with SBR and Parametric Stereo).
     */
    kMixedStreamAudioProfileHEv2 = 2,
};

/**
 * @hidden
 * @brief notes missing
 */
enum MixedStreamSyncStrategy {
    kMixedStreamSyncStrategyNoSync = 0,
    kMixedStreamSyncStrategyAudioPreciseSync = 1,
    kMixedStreamSyncStrategySimplexModeSync = 2,
};

/** 
 * @type keytype
 * @brief The audio codec.(New)
 */
enum MixedStreamAudioCodecType {
    /** 
     * @brief AAC format.
     */
    kMixedStreamAudioCodecTypeAAC = 0,
};

/** 
 * @type keytype
 * @brief Content of SEI sent while pushing mixed stream to CDN.
 */
enum MixedStreamSEIContentMode {
    /** 
     * @brief The video stream contains all the SEI information. Default value.
     */
    kMixedStreamSEIContentModeDefault = 0,
    /** 
     * @brief The SEI data transmitted with non-key frames contains volume information only.<br>
     *        This parameter only takes effect after you set `MixedStreamServerControlConfig.enable_volume_indication` to "True".
     */
    kMixedStreamSEIContentModeEnableVolumeIndication = 1,
};

/** 
 * @type keytype
 * @brief The video codec.(New)
 */
enum MixedStreamVideoCodecType {
    /** 
     * @brief (Default) H.264 format.
     */
    kMixedStreamVideoCodecTypeH264 = 0,
    /** 
     * @brief ByteVC1 format.
     */
    kMixedStreamVideoCodecTypeByteVC1 = 1,
};

/** 
 * @type keytype
 * @brief The render mode.(New)
 */
enum MixedStreamRenderMode {
    /** 
     * @brief (Default) Fill and Crop.
     *        The video frame is scaled with fixed aspect ratio, until it completely fills the canvas. The region of the video exceeding the canvas will be cropped.
     */
    kMixedStreamRenderModeHidden = 1,
    /** 
     * @brief Fit.
     *        The video frame is scaled with fixed aspect ratio, until it fits just within the canvas. Other part of the canvas is filled with the background color.
     */
    kMixedStreamRenderModeFit = 2,
    /** 
     * @brief Fill the canvas.
     *        The video frame is scaled to fill the canvas. During the process, the aspect ratio may change.
     */
    kMixedStreamRenderModeAdaptive = 3,
};

/** 
 * @type keytype
 * @brief Types of stream after being mixed.(New)
 */
enum MixedStreamMediaType {
    /** 
     * @brief Audio and video
     */
    kMixedStreamMediaTypeAudioAndVideo = 0,
    /** 
     * @brief Audio only
     */
    kMixedStreamMediaTypeAudioOnly = 1,
    /** 
     * @brief Video only
     */
    kMixedStreamMediaTypeVideoOnly = 2,
};

/** 
 * @type keytype
 * @brief Stream mixing region type.(New)
 */
enum MixedStreamLayoutRegionType {
    /** 
     * @brief The region type is a video stream.
     */
    kMixedStreamLayoutRegionTypeVideoStream = 0,
    /** 
     * @brief The region type is an image.
     */
    kMixedStreamLayoutRegionTypeImage = 1,
};

/** 
 * @type keytype
 * @brief The video format for client stream mixing callback.(New)
 *        If the format you set is not adaptable to the system, the format will be set as the default value.
 */
enum MixedStreamClientMixVideoFormat {
    /** 
     * @brief YUV I420 format. The default format for Android and Windows. Supported system: Android, Windows.
     */
    kMixedStreamClientMixVideoFormatI420 = 0,
    /** 
     * @brief OpenGL GL_TEXTURE_2D format. Supported system: Android.
     */
    kMixedStreamClientMixVideoFormatTexture2D = 1,
    /** 
     * @brief CVPixelBuffer BGRA format. The default format for iOS. support system: iOS.
     */
    kMixedStreamClientMixVideoFormatCVPixelBufferBGRA = 2,
    /**
     * {en}
     * @brief YUV NV12 format. The default format for macOS. Supported system: macOS.
     */
    kMixedStreamClientMixVideoFormatNV12 = 3,
};
/** 
 * @type keytype
 * @brief Stream type
 */
enum MixedStreamVideoType {
    /** 
     * @brief Mainstream, including: <br>
     *       + Video/audio captured by the the camera/microphone using internal capturing; <br>
     *       + Video/audio captured by custom method.
     */
    kMixedStreamVideoTypeMain = 0,
    /** 
     * @brief Screen-sharing stream.
     */
    kMixedStreamVideoTypeScreen = 1,
};


/** 
 *  @type keytype
 *  @brief Audio mix stream configurations.(New)
 *         With invalid or empty input, the configurations will be set as the default values.
 */
typedef struct MixedStreamAudioConfig {
    /** 
     * @brief The sample rate (Hz). Supported sample rates: 32,00 Hz, 44,100 Hz, 48,000 Hz. Defaults to 48,000 Hz. It's recommended to be set.
     */
    int32_t sample_rate = 48000;
    /** 
     * @brief The number of channels. Supported channels: 1 (single channel), 2 (dual channel).  Defaults to 2. It's recommended to be set.
     */
    int32_t channels = 2;
    /** 
     * @brief The bitrate (Kbps) in range of [32, 192]. Defaults to 64 Kbps. It's recommended to be set.
     */
    int32_t bitrate = 64;
    /** 
     * @brief AAC profile. See MixedStreamAudioProfile{@link #MixedStreamAudioProfile}. Defaults to `0`. It's recommended to be set.
     */
    MixedStreamAudioProfile audio_profile = MixedStreamAudioProfile::kMixedStreamAudioProfileLC;
    /** 
     * @brief AAC profile. See MixedStreamAudioCodecType{@link #MixedStreamAudioCodecType}. Defaults to `0`. It's recommended to be set.
     */
    MixedStreamAudioCodecType audio_codec = MixedStreamAudioCodecType::kMixedStreamAudioCodecTypeAAC;
} MixedStreamAudioConfig;

/** 
 * @type keytype
 * @brief Video mix stream configurations.(New)
 *        With invalid or empty input, the configurations will be set as the default values.
 */
typedef struct MixedStreamVideoConfig {
    /** 
     * @brief The width (pixels) to be set. The range is [2, 1920], and must be an even number. The default value is 360 pixels. It's recommended to be set.
     *        If an odd number is set, the width will be adjusted to the next larger even number.
     */
    int32_t width = 360;
    /** 
     * @brief The height (pixels) to be set. The range is [2, 1920], and must be an even number. The default value is 640 pixels. It's recommended to be set.
     *        If an odd number is set, the height will be adjusted to the next larger even number.
     */
    int32_t height = 640;
    /** 
     * @brief The frame rate (FPS) in range of [1, 60]. The default value is 15 FPS. It's recommended to be set.
     */
    int32_t fps = 15;
    /** 
     * @brief The time interval between I-frames (second) in range of [1, 5]. The default value is 2 seconds. It's recommended to be set.
     *        This parameter cannot be updated while pushing stream to the CDN.
     */
    int32_t gop = 2;
    /** 
     * @brief The bitrate (Kbps) in range of [1, 10000]. The default value is self-adaptive. It's recommended to be set.
     */
    int32_t bitrate = 500;
    /** 
     * @brief The video codec. See MixedStreamVideoCodecType{@link #MixedStreamVideoCodecType}. The default value is `0`. It's recommended to be set.
     *        This parameter cannot be updated while pushing stream to the CDN.
     */
    MixedStreamVideoCodecType video_codec = MixedStreamVideoCodecType::kMixedStreamVideoCodecTypeH264;
    /** 
      * @brief Whether to push streams with B frame, only support by server mix:   <br>
      *         + True: Yes <br>
      *         + False: No
      */
    bool enable_bframe = false;
} MixedStreamVideoConfig;

/** 
 * @type keytype
 * @brief Client mixing parameters.(New)
 */
typedef struct MixedStreamClientMixConfig {
    /** 
     * @brief Whether to use audio mixing. Default is true.
     */
    bool use_audio_mixer = true;
    /** 
     * @brief The video format to be set. See MixedStreamClientMixVideoFormat{@link #MixedStreamClientMixVideoFormat}.
     */
    MixedStreamClientMixVideoFormat video_format;
} MixedStreamClientMixConfig;

/** 
 * @hidden for internal use only
 * @type keytype
 * @brief Parameters of simplex mode and synchronization when pushing to CDN.(New)
 */
typedef struct MixedStreamSyncControlConfig {
    /** 
     * @brief The max length of the cached stream in milliseconds. 2000 by default.
     * @notes Set the value based on the stall of the media streams. Higher the value, bigger the memory usage. The recommended value is `2000`.
     */
    int32_t max_cache_time_ms = 2000;
    /** 
     * @brief Whether to enable simplex mode during pushing to CDN. False(duplex mode) by default.
     *        When simplex mode is enabled, RTC SDK synchronizes and transcodes the media streams from the local user and several remote users, and pushing the mixed stream to CDN. `syncBaseUser` does not play the streams from the remote users. You must set the following parameters.
     *        When duplex mode is enabled, RTC SDK transcodes the media streams from the local user and several remote users without synchronization, and pushing the mixed stream to CDN. The following parameters are not effective.
     */
    MixedStreamSyncStrategy sync_strategy = MixedStreamSyncStrategy::kMixedStreamSyncStrategyNoSync;
    /** 
     * @brief User ID of the base stream during syncing. Null by default.
     */
    const char* base_user_id = nullptr;
    /** 
     * @brief Whether to use RTC SDK to push to CDN. True by default.
     *        If `False`, you can get the media frames by onCacheSyncVideoFrames{@link #ITranscoderObserver#onCacheSyncVideoFrames} and manually push them to CDN.
     */
    bool video_need_sdk_mix = true;
} MixedStreamSyncControlConfig;

/** 
 * @type keytype
 * @brief Image parameters for stream mixing.(New)
 */
typedef struct MixedStreamLayoutRegionImageWaterMarkConfig {
    /** 
     * @brief Width of the original image in px.
     */
    int image_width = 0;
    /** 
     * @brief Height of the original image in px.
     */
    int image_height = 0;
} MixedStreamLayoutRegionImageWaterMarkConfig;

/** 
 * @type keytype
 * @brief Spatial audio config when pushing to CDN.(New)
 */
typedef struct MixedStreamSpatialAudioConfig {
    /** 
     * @brief Whether to enable the spatial audio effect when pushing to CDN.
     * @notes when you enable the feature, set the `spatial_position` of each MixedStreamLayoutRegionConfig{@link #MixedStreamLayoutRegionConfig} for spatial audio effect.
     */
    bool enable_spatial_render;
    /** 
     * @brief The spatial position of the audience. See Position{@link #Position}. <br>
     *        The audience is the users who receive the audio stream from CDN.
     */
    Position audience_spatial_position;
    /** 
     * @brief The orientation of the audience. See HumanOrientation{@link #HumanOrientation}. <br>
     *        The audience is the users who receive the audio stream from CDN.
     */
    HumanOrientation audience_spatial_orientation;
} MixedStreamSpatialAudioConfig;

/** 
 * @type keytype
 * @brief Configurations to be set while mixing streams on the server side.
 */
typedef struct MixedStreamServerControlConfig {
    /** 
     * @brief Sets whether to enable the function of separately sending sound indication SEI:
     *        + True: Enable.
     *        + False: Disable. Default setting.
     *        After setting this parameter to "true", you can choose whether to send sound indication SEI only through `MixedStreamServerControlConfig.sei_content_mode`.
     */
    bool enable_volume_indication = false;
    /** 
     * @brief Sound indication interval in s. The range is [0.3,+∞). The default value is 2.<br>
     *        If a decimal with two or more decimal places is passed in, it will be rounded to the value of the first decimal place. For example, if you pass in 0.36, it will be automatically adjusted to 0.4.
     */
    float volume_indication_interval = 2.0f;
    /** 
     * @brief Effective volume within the range of [0, 255]. The default value is 0.<br>
     *        If the value is set beyond the range, it will be automatically adjusted to the default value 0.
     */
    int talk_volume = 0;
    /** 
     * @brief Sets whether volume value is contained in the sound indication SEI:
     *        + True: Yes.
     *        + False: No. Default setting.
     */
    bool is_add_volume_value = false;
    /** 
     * @brief Sets SEI content. See MixedStreamSEIContentMode{@link #MixedStreamSEIContentMode}.
     */
    MixedStreamSEIContentMode sei_content_mode = kMixedStreamSEIContentModeDefault;
     /** 
      * @type keytype
      * @brief SEI payload type.
      * @notes The default value is `100`, and the value supported is `5` and `100`.
      *        During the process of pushing streams to CDN, you cannot change the parameter. 
      */
      int sei_payload_type = 100;
      /** 
       * @type keytype
       * @brief SEI payload UUID.
       * @notes When PayloadType is `5`, you must set PayloadUUID, or you will receive a callback indicating parameter error.  <br>
       *        When PayloadType is not `5`, it is not required to set PayloadUUID. If filled, it will be ignored by the backend. <br>
       *        The length of PayloadUUID should be 32 bits, or you will receive a callback indicating parameter error.   <br>
       *        Each character of the parameter should be within the range of [0, 9] [a, f] [A, F].  <br>
       *        The PayloadUUID should not contain `-`. If the automatically generated UUID contains `-`, you should delete it. <br>
       *        During the process of pushing streams to CDN, you cannot change the parameter.  
       */
      const char* sei_payload_uuid = nullptr;
} MixedStreamServerControlConfig;

/** 
 * @type keytype
 * @brief Layout information for one of the video streams to be mixed.(New)
 *        After starting to push streams to CDN and mixing multiple video streams, you can set the layout information for each of them.
 */
typedef struct MixedStreamLayoutRegionConfig {
    /** 
     * @brief The user ID of the user who publishes the video stream. It's recommended to be set.
     */
    const char* region_id = nullptr;
   /** 
     * @brief The room ID of the media stream. It's recommended to be set. <br>
     *        If the media stream is the stream forwarded by startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms}, you must set the roomID to the room ID of the target room.
     */
    const char* room_id = nullptr;
    /** 
     * @brief The normalized horizontal coordinate value of the top left end vertex of the user's view, ranging within [0.0, 1.0). The default value is 0.0. It's recommended to be set.
     */
    float location_x = 0.0f;
    /** 
     * @brief The normalized vertical coordinate value of the top left end vertex of the user's view, ranging within [0.0, 1.0). The default value is 0.0. It's recommended to be set.
     */
    float location_y = 0.0f;
    /** 
     * @brief The normalized width of the user's view, ranging within [0.0, 1.0]. The default value is 1.0. It's recommended to be set.
     */
    float width_proportion = 1.0f;
    /** 
     * @brief The normalized height of the user's view, ranging within [0.0, 1.0]. The default value is 1.0. It's recommended to be set.
     */
    float height_proportion = 1.0f;
    /** 
     * @brief The opacity in range of (0.0, 1.0]. The lower value, the more transparent. The default value is 1.0.
     */
    float alpha = 1.0f;
    /** 
     * @brief The proportion of the radius to the width of the canvas. `0.0` by default.
     * @notes After you set the value, `width_px`, `height_px`, and `corner_radius_px` are calculated based on `width`, `height`, `corner_radius`, and the width of the canvas. If `corner_radius_px < min(width_px/2, height_px/2)` is met, the value of `corner_radius` is set valid; if not, `corner_radius_px` is set to `min(width_px/2, height_px/2)`, and `corner_radius` is set to the proportion of `corner_radius_px` to the width of the canvas.
     */
    float corner_radius = 0;

    /** 
     * @brief The layer on which the video is rendered. The range is [0, 100]. 0 for the bottom layer, and 100 for the top layer. The default value is 0.
     */
    int32_t z_order = 0;
    /** 
     * @brief Whether the source user of the stream is a local user:   <br>
     *         + True: Yes <br>
     *         + False: No
     */
    bool is_local_user = false;
    /** 
     * @brief Whether the stream comes from screen sharing:   <br>
     */
    MixedStreamVideoType stream_type = MixedStreamVideoType::kMixedStreamVideoTypeMain;
    /** 
     * @brief The stream mixing content type. The default value is `kMediaTypeAudioAndVideo`. See MixedStreamMediaType{@link #MixedStreamMediaType}.
     */
    MixedStreamMediaType media_type = MixedStreamMediaType::kMixedStreamMediaTypeAudioAndVideo;
    /** 
     * @brief The render mode. See MixedStreamRenderMode{@link #MixedStreamRenderMode}. The default value is 1. It's recommended to be set.
     */
    MixedStreamRenderMode render_mode = MixedStreamRenderMode::kMixedStreamRenderModeHidden;
    /** 
     * @type keytype
     * @brief  Stream mixing region type. See MixedStreamLayoutRegionType{@link #MixedStreamLayoutRegionType}.
     */
    MixedStreamLayoutRegionType region_content_type = MixedStreamLayoutRegionType::  kMixedStreamLayoutRegionTypeVideoStream;
    /** 
     * @type keytype
     * @brief The RGBA data of the mixing image. Put in null when mixing video streams.
     */
    uint8_t* image_water_mark = nullptr;
    /** 
     * @type keytype
     * @brief  Image parameters for stream mixing. See MixedStreamLayoutRegionImageWaterMarkConfig{@link #MixedStreamLayoutRegionImageWaterMarkConfig}. Put in null when mixing video streams.
     */
    MixedStreamLayoutRegionImageWaterMarkConfig image_water_mark_param;
    /** 
     * @type keytype
     * @brief spatial position. See Position{@link #Position}.
     */
    Position spatial_position;
    
    bool apply_spatial_audio = true;
    
} MixedStreamLayoutRegionConfig;

/** 
 * @deprecated since 3.52, use MixedStreamType{@link #MixedStreamType} instead.
 * @type keytype
 * @brief Stream mixing type
 */
enum StreamMixingType {
    /** 
     * @brief Mix media streams on the server side.
     */
    kStreamMixingTypeByServer = 0,
    /** 
     * @brief Intelligent stream mixing. The SDK will intelligently decide that a stream mixing task would be done on the client or the server.
     */
    kStreamMixingTypeByClient = 1,
};


/** 
 * @deprecated since 3.52, use MixedStreamAudioProfile{@link #MixedStreamAudioProfile} instead.
 * @type keytype
 * @brief Advanced Audio Coding (AAC) profile.
 */
enum TranscoderAudioCodecProfile {
    /** 
     * @brief (Default) AAC Low-Complexity profile (AAC-LC).
     */
    kByteAACProfileLC = 0,
    /** 
     * @brief HE-AAC v1 profile (AAC LC with SBR).
     */
    kByteAACProfileHEv1 = 1,
    /** 
     * @brief HE-AAC v2 profile (AAC LC with SBR and Parametric Stereo).
     */
    kByteAACProfileHEv2 = 2,
};

/** 
 * @deprecated since 3.52, use MixedStreamAudioCodecType{@link #MixedStreamAudioCodecType} instead.
 * @type keytype
 * @brief The audio codec.
 */
enum TranscoderAudioCodecType {
    /** 
     * @brief AAC format.
     */
    kTranscodeAudioCodecAAC = 0,
};

/** 
 * @deprecated since 3.52, use MixedStreamVideoCodecType{@link #MixedStreamVideoCodecType} instead.
 * @type keytype
 * @brief The video codec.
 */
enum TranscoderVideoCodecType {
    /** 
     * @brief (Default) H.264 format.
     */
    kTranscodeVideoCodecH264 = 0,
    /** 
     * @brief ByteVC1 format.
     */
    kTranscodeVideoCodecH265 = 1,
};

/** 
 * @deprecated since 3.52, use MixedStreamRenderMode{@link #MixedStreamRenderMode} instead.
 * @type keytype
 * @brief The render mode.
 */
enum TranscoderRenderMode {
    /** 
     * @deprecated since 3.45 and will be deleted in 3.51.
     */
    kRenderUnknown = 0,
    /** 
     * @brief (Default) Fill and Crop.
     *        The video frame is scaled with fixed aspect ratio, until it completely fills the canvas. The region of the video exceeding the canvas will be cropped.
     */
    kRenderHidden = 1,
    /** 
     * @brief Fit.
     *        The video frame is scaled with fixed aspect ratio, until it fits just within the canvas. Other part of the canvas is filled with the background color.
     */
    kRenderFit = 2,
    /** 
     * @brief Fill the canvas.
     *        The video frame is scaled to fill the canvas. During the process, the aspect ratio may change.
     */
    kRenderAdaptive = 3,
};

/** 
 * @deprecated since 3.52, use MixedStreamLayoutRegionType{@link #MixedStreamLayoutRegionType} instead.
 * @type keytype
 * @brief Stream mixing region type
 */
enum TranscoderLayoutRegionType {
    /** 
     * @brief The region type is a video stream.
     */
    kLayoutRegionTypeVideoStream = 0,
    /** 
     * @brief The region type is an image.
     */
    kLayoutRegionTypeImage = 1,
};

/** 
 * @deprecated since 3.52, use MixedStreamLayoutRegionImageWaterMarkConfig{@link #MixedStreamLayoutRegionImageWaterMarkConfig} instead.
 * @type keytype
 * @brief  Image parameters for stream mixing
 */
typedef struct TranscoderLayoutRegionDataParam {
    /** 
     * @brief Width of the original image in px.
     */
    int image_width = 0;
    /** 
     * @brief Height of the original image in px.
     */
    int image_height = 0;
}TranscoderLayoutRegionDataParam;

/** 
 * @deprecated since 3.52, use MixedStreamClientMixVideoFormat{@link #MixedStreamClientMixVideoFormat} instead.
 * @type keytype
 * @brief The video format for client stream mixing callback. If the format you set is not adaptable to the system, the format will be set as the default value.
 */
enum TranscoderClientMixVideoFormat {
    /** 
     * @brief YUV I420 format. The default format for Android and Windows. Supported system: Android, Windows.
     */
    kClientMixVideoFormatI420 = 0,
    /** 
     * @brief OpenGL GL_TEXTURE_2D format. Supported system: Android.
     */
    kClientMixVideoFormatTexture2D = 1,
    /** 
     * @brief CVPixelBuffer BGRA format. The default format for iOS. support system: iOS.
     */
    kClientMixVideoFormatCVPixelBufferBGRA = 2,
    /** 
     * @brief YUV NV12 format. The default format for macOS. Supported system: macOS.
     */
    kClientMixVideoFormatNV12 = 3,
};
/** 
 * @type keytype
 * @brief  Data frame type
 */
enum DataFrameType {
    /** 
     * @brief SEI  video frame
     */
    kDataFrameTypeSei = 0,
};

/**
 * @hidden for internal use only
 */
typedef size_t status_t;
/**
 * @hidden for internal use only
 */
class IVideoFrame;
/**
 * @hidden for internal use only
 */
class IAudioFrame;

/** 
 * @type keytype
 * @brief  Data frame
 */
typedef struct IDataFrame {
    /** 
     * @brief Data frame type. See DataFrameType{@link #DataFrameType}
     */
    DataFrameType frame_type = kDataFrameTypeSei;
    /** 
     * @brief Data frame data
     */
    uint8_t* u8_data = nullptr;
    /** 
     * @brief Data frame data size
     */
    uint32_t u32_data_size = 0;
    /** 
     * @brief Data frame timestamp in microseconds
     */
    uint64_t u64_ts_us = 0;
} IDataFrame;

/** 
 * @deprecated since 3.52, use MixedStreamLayoutRegionConfig{@link #MixedStreamLayoutRegionConfig} instead.
 * @type keytype
 * @brief Layout information for one of the video streams to be mixed.
 *        After starting to push streams to CDN and mixing multiple video streams, you can set the layout information for each of them.
 */
typedef struct TranscoderLayoutRegion {
    /** 
     * @brief The user ID of the user who publishes the video stream. Required.
     */
    const char* region_id = nullptr;
   /** 
     * @brief The room ID of the media stream. Required. <br>
     *        If the media stream is the stream forwarded by startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms}, you must set the roomID to the room ID of the target room.
     */
    const char* room_id = nullptr;
    /** 
     * @brief The normalized horizontal coordinate value of the top left end vertex of the user's view, ranging within [0.0, 1.0). The default value is 0.0.
     */
    float x = 0.0f;
    /** 
     * @brief The normalized vertical coordinate value of the top left end vertex of the user's view, ranging within [0.0, 1.0). The default value is 0.0.
     */
    float y = 0.0f;
    /** 
     * @brief The normalized width of the user's view, ranging within [0.0, 1.0]. The default value is 1.0.
     */
    float width = 1.0f;
    /** 
     * @brief The normalized height of the user's view, ranging within [0.0, 1.0]. The default value is 1.0.
     */
    float height = 1.0f;
    /** 
     * @brief (Only server-side stream mixing can set this parameter.) The opacity in range of (0.0, 1.0]. The lower value, the more transparent. The default value is 1.0.
     */
    float alpha = 1.0f;
    /** 
     * @brief (Only server-side stream mixing can set this parameter.) The proportion of the radius to the width of the canvas. The default value is 0.0.
     * @notes After you set the value, `width_px`, `height_px`, and `corner_radius_px` are calculated based on `width`, `height`, `corner_radius`, and the width of the canvas. If `corner_radius_px < min(width_px/2, height_px/2)` is met, the value of `corner_radius` is set valid; if not, `corner_radius_px` is set to `min(width_px/2, height_px/2)`, and `corner_radius` is set to the proportion of `corner_radius_px` to the width of the canvas.
     */
    float corner_radius = 0;

    /** 
     * @brief The layer on which the video is rendered. The range is [0, 100]. 0 for the bottom layer, and 100 for the top layer. The default value is 0.
     */
    int32_t i32_z_order = 0;
    /** 
     * @brief Whether the source user of the stream is a local user:   <br>
     *         + true: Yes <br>
     *         + false: No
     */
    bool local_user = false;
    /** 
     * @brief (Only server-side stream mixing can set this parameter to true.) Whether the stream comes from screen sharing:   <br>
     *         + true: Yes <br>
     *         + false: No
     */
    bool screen_stream = false;
    /** 
     * @brief (Only server-side stream mixing can set this parameter.) The stream mixing content type. The default value is `kHasAudioAndVideo`. See TranscoderContentControlType{@link #TranscoderContentControlType}.
     */
    TranscoderContentControlType content_control = TranscoderContentControlType::kHasAudioAndVideo;
    /** 
     * @brief The render mode. See TranscoderRenderMode{@link #TranscoderRenderMode}. The default value is 1.
     */
    TranscoderRenderMode render_mode = kRenderHidden;
    /** 
     * @type keytype
     * @brief  Stream mixing region type. See TranscoderLayoutRegionType{@link #TranscoderLayoutRegionType}.
     */
    TranscoderLayoutRegionType type = kLayoutRegionTypeVideoStream;
    /** 
     * @type keytype
     * @brief The RGBA data of the mixing image. Put in null when mixing video streams.
     */
    uint8_t* data = nullptr;
    /** 
     * @type keytype
     * @brief  Image parameters for stream mixing. See TranscoderLayoutRegionDataParam{@link #TranscoderLayoutRegionDataParam}. Put in null when mixing video streams.
     */
    TranscoderLayoutRegionDataParam data_param;
    /** 
     * @type keytype
     * @brief spatial position. See Position{@link #Position}.
     */
    Position spatial_position;

    bool apply_spatial_audio = true;

} TranscoderLayoutRegion;

/** 
 * @deprecated since 3.52, use MixedStreamAudioConfig{@link #MixedStreamAudioConfig} instead.
 *  @type keytype
 *  @brief Audio transcoding configurations. With invalid or empty input, the configurations will be set as the default values.
 */
typedef struct TranscoderAudioParam {
    /** 
     * @brief The sample rate (Hz). Supported sample rates: 32,00 Hz, 44,100 Hz, 48,000 Hz. Defaults to 48,000 Hz.
     */
    int32_t i32_sample_rate = 48000;
    /** 
     * @brief The number of channels. Supported channels: 1 (single channel), 2 (dual channel).  Defaults to 2.
     */
    int32_t i32_channel_num = 2;
    /** 
     * @brief The bitrate (Kbps) in range of [32, 192]. Defaults to 64 Kbps.
     */
    int32_t i32_bitrate_kbps = 64;
    /** 
     * @brief AAC profile. See TranscoderAudioCodecProfile{@link #TranscoderAudioCodecProfile}. Defaults to `0`.
     */
    TranscoderAudioCodecProfile audio_codec_profile = kByteAACProfileLC;
    /** 
     * @brief AAC profile. See TranscoderAudioCodecType{@link #TranscoderAudioCodecType}. Defaults to `0`.
     */
    TranscoderAudioCodecType audio_codec_type = kTranscodeAudioCodecAAC;
} TranscoderAudioParam;

/** 
 * @deprecated since 3.52, use MixedStreamVideoConfig{@link #MixedStreamVideoConfig} instead.
 * @type keytype
 * @brief Video transcoding configurations. With invalid or empty input, the configurations will be set as the default values.
 */
typedef struct TranscoderVideoParam {
    /** 
     * @brief The width (pixels) to be set. The range is [2, 1920], and must be an even number. The default value is 360 pixels.
     *        If an odd number is set, the width will be adjusted to the next larger even number.
     */
    int32_t i32_width = 360;
    /** 
     * @brief The height (pixels) to be set. The range is [2, 1920], and must be an even number. The default value is 640 pixels.
     *        If an odd number is set, the height will be adjusted to the next larger even number.
     */
    int32_t i32_height = 640;
    /** 
     * @brief The frame rate (FPS) in range of [1, 60]. The default value is 15 FPS.
     */
    int32_t i32_fps = 15;
    /** 
     * @brief The time interval between I-frames (second) in range of [1, 5]. The default value is 2 seconds.
     *        This parameter cannot be updated while pushing stream to the CDN.
     */
    int32_t i32_gop = 2;
    /** 
     * @brief The bitrate (Kbps) in range of [1, 10000]. The default value is self-adaptive.
     */
    int32_t i32_bitrate_kbps = 500;
    /** 
     * @brief The video codec. See TranscoderVideoCodecType{@link #TranscoderVideoCodecType}. The default value is `0`.
     *        This parameter cannot be updated while pushing stream to the CDN.
     */
    TranscoderVideoCodecType video_codec_type = kTranscodeVideoCodecH264;
    /** 
      * @brief Whether to push streams with B frame, only support by server mix:   <br>
      *         + true: Yes <br>
      *         + false: No
      */
    bool bFrame = false;
} TranscoderVideoParam;

/** 
 * @deprecated since 3.52, use MixedStreamSyncControlConfig instead.
 * @hidden internal use only
 * @valid since 3.50
 * @type keytype
 * @brief Parameters of simplex mode and synchronization when pushing to CDN.
 */
typedef struct TranscoderSyncControlParam {
    /** 
     * @brief Whether to enable simplex mode during pushing to CDN. False(duplex mode) by default.
     *        When simplex mode is enabled, RTC SDK synchronizes and transcodes the media streams from the local user and several remote users, and pushing the mixed stream to CDN. `syncBaseUser` does not play the streams from the remote users. You must set the following parameters.
     *        When duplex mode is enabled, RTC SDK transcodes the media streams from the local user and several remote users without synchronization, and pushing the mixed stream to CDN. The following parameters are not effective.
     */
     bool sync_stream = false;
    /** 
     * @hidden
     */
     MixedStreamSyncStrategy sync_strategy = MixedStreamSyncStrategy::kMixedStreamSyncStrategyNoSync;
    /** 
     * @brief User ID of the base stream during syncing. Null by default.
     */
    const char* sync_base_user = nullptr;
    /** 
     * @brief The max length of the cached stream in milliseconds. 0 by default.
     * @notes Set the value based on the stall of the media streams. Higher the value, bigger the memory usage. The recommended value is `2000`.
     */
    int32_t sync_queue_length_ms = 0;
    /** 
     * @brief Whether to use RTC SDK to push to CDN. True by default.
     *        If `False`, you can get the media frames by onCacheSyncVideoFrames{@link #ITranscoderObserver#onCacheSyncVideoFrames} and manually push them to CDN.
     */
    bool sync_client_video_need_mix = true;
    /** 
     * @brief Whether only send cache sync pts and not start sync and callback data. False by default.
     *        In the simplex chorus scene of two people, the lead singer should set to False, and the sub-singer should set to True.
     *        In the duplex chorus scene of two people, all the singers should set to False.
     */
    bool sync_only_send_pts = false;
} TranscoderSyncControlParam;

/** 
 * @deprecated since 3.52, use MixedStreamClientMixConfig{@link #MixedStreamClientMixConfig} instead.
 * @type keytype
 * @brief Configurations for mixing and pushing to CDN at client side.
 */
typedef struct TranscoderClientMixParam {
    /** 
     * @brief Whether to use original video frames. False by default.
     */
    bool client_mix_use_original_frame = false;
    /** 
     * @brief Whether to use audio mixing. True by default.
     */
    bool client_mix_use_audio_mixer = true;
    /** 
     * @brief The video format to be set. See TranscoderClientMixVideoFormat{@link #TranscoderClientMixVideoFormat}.
     */
    TranscoderClientMixVideoFormat client_mix_video_format_;
} TranscoderClientMixParam;

/** 
 * @deprecated since 3.52, use MixedStreamSpatialAudioConfig{@link #MixedStreamSpatialAudioConfig} instead.
 * @type keytype
 * @brief Spatial audio config when pushing to CDN.
 */
typedef struct TranscoderSpatialConfig {
    /** 
     * @brief Whether to enable the spatial audio effect when pushing to CDN.
     * @notes when you enable the feature, set the `spatial_position` of each TranscoderLayoutRegion{@link #TranscoderLayoutRegion} for spatial audio effect.
     */
    bool enable_spatial_render;
    /** 
     * @brief The spatial position of the audience. See Position{@link #Position}. <br>
     *        The audience is the users who receive the audio stream from CDN.
     */
    Position audience_spatial_position;
    /** 
     * @brief The orientation of the audience. See HumanOrientation{@link #HumanOrientation}. <br>
     *        The audience is the users who receive the audio stream from CDN.
     */
    HumanOrientation audience_spatial_orientation;
} TranscoderSpatialConfig;

/** 
 * @deprecated since 3.52, use IMixedStreamConfig{@link #IMixedStreamConfig} instead.
 * @type keytype
 * @brief Configurations to be set when pushing media streams to CDN.
 */
class ITranscoderParam : public ITranscoderParamBase {
public:

    /** 
     * @type api
     * @region Push to CDN
     * @brief Gets the stream mixing type.
     * @return Stream mixing type. See StreamMixingType{@link #StreamMixingType}
     * @notes This parameter cannot be updated while pushing stream to the CDN.
     */
    virtual StreamMixingType expectedMixingType() = 0;
    /** 
     * @type api
     * @brief Gets the user ID for live transcoding.
     * @return The user ID for live transcoding.
     */
    virtual const char* userID() = 0;
    /** 
     * @type api
     * @brief Gets the URL for live transcoding.
     * @return The CDN url.
     */
    virtual const char* uri() = 0;
    /** 
     * @type api
     * @region Forward to live broadcast
     * @brief Get SEI information.
     * @return SEI Information.
     */
    virtual const char* appData() = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Gets the audio configurations.
     * @return Audio configurations. See TranscoderAudioParam{@link #TranscoderAudioParam}.
     */
    virtual TranscoderAudioParam audioParam() = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Gets the video configurations.
     * @return Video configurations. See TranscoderVideoParam{@link #TranscoderVideoParam}.
     */
    virtual TranscoderVideoParam videoParam() = 0;
    /** 
     * @hidden(Windows,macOS,Linux)
     * @type api
     * @brief Get the spatial audio configurations of pushing to CDN.
     * @return See TranscoderSpatialConfig{@link #TranscoderSpatialConfig}.
     */
    virtual TranscoderSpatialConfig spatialConfig() = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Gets the video layout information of the mixed stream.
     * @param [in] index The number of the view of which you want to get information.
     * @return Layout information. See TranscoderLayoutRegion{@link #TranscoderLayoutRegion}.
     */
    virtual TranscoderLayoutRegion layoutRegionByIndex(int32_t index) = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @region Forward to live broadcast
     * @brief Get dynamically extend customizable parameters
     * @return dynamically extend customizable parameters
     */
    virtual const char* advancedConfig() = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @region Forward to live broadcast
     * @brief Get Business Transparent Authentication Information
     * @return Business Transparent Authentication Information
     */
    virtual const char* authInfo() = 0;
    /** 
     * @valid since 3.50
     * @type api
     * @brief Get client mixing parameters.
     * @return Client mixing parameters. See TranscoderClientMixParam{@link #TranscoderClientMixParam}.
     */
    virtual TranscoderClientMixParam clientMixParam()  = 0;
    /** 
     * @hidden for internal use only
     * @valid since 3.50
     * @type api
     * @brief Get the parameters of simplex mode and synchronization when pushing to CDN.
     * @return See TranscoderSyncControlParam{@link #TranscoderSyncControlParam}.
     */
    virtual TranscoderSyncControlParam syncControlparam() = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Sets the expected stream mixing type
     * @param [in] expected_mix_type Stream mixing type. See StreamMixingType{@link #StreamMixingType}
     */
    virtual void setExpectedMixingType(StreamMixingType expected_mix_type) = 0;
    /** 
     * @type api
     * @brief Sets the user ID for live transcoding. The sum length of `room_id` and `user_id` should not exceed 126 bytes.
     *        This parameter cannot be updated while pushing stream to the CDN.
     * @param [in] user_id The user ID for live transcoding.
     */
    virtual void setUserID(const char* user_id) = 0;
    /** 
     * @type api
     * @brief Sets the URL for live transcoding. Only supports live transcoding via RTMP. The URL should match the regular expression `/^rtmps?:\/\//`.
     *        This parameter cannot be updated while pushing stream to the CDN.
     * @param [in] uri The URL to be set.
     */
    virtual void setUri(const char* uri) = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Sets audio configurations. See TranscoderAudioParam{@link #TranscoderAudioParam}.
     *        This parameters cannot be updated while pushing stream to the CDN.
     */
    virtual void setAudioParam(const TranscoderAudioParam&) = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Sets video configurations. See TranscoderVideoParam{@link #TranscoderVideoParam}.
     */
    virtual void setVideoParam(const TranscoderVideoParam&) = 0;
    /** 
     * @hidden(Windows,macOS,Linux)
     * @type api
     * @brief Sets spatial audio configurations. See TranscoderSpatialConfig{@link #TranscoderSpatialConfig}.
     */
    virtual void setSpatialConfig(const TranscoderSpatialConfig&) = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Sets the layout configurations.
     * @param [in] regions[] User layout information list. It's a list of TranscoderLayoutRegion{@link #TranscoderLayoutRegion} that you construct for each stream.
     *                       With invalid or empty input, the configurations will be set as the default values.
     * @param [in] regions_size Amount of views.
     * @param [in] bg_color Background-color of the mixed stream in hexadecimal values such as #FFFFFF and #000000. The default value is #000000.
     *                      With invalid or empty input, the configurations will be set as the default values.
     * @param [in] app_data Additional data that you want to import，up to 4KB in length.
     */
    virtual void setLayoutParam(
            TranscoderLayoutRegion regions[], int32_t regions_size, const char* bg_color, const char* app_data) = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @region  Forward to live broadcast
     * @brief Sets dynamically extend customizable parameters
     * @param [in] advanced_config  dynamically extend customizable parameters
     */
    virtual void setAdvancedConfig(const char* advanced_config) = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @region Forward to live broadcast
     * @brief Sets Business Transparent Authentication Information
     * @param [in] auth_info  Business Transparent Authentication Information
     */
    virtual void setAuthInfo(const char* auth_info) = 0;
    /** 
     * @hidden internal use only
     * @valid since 3.50
     * @type api
     * @brief Sets client mixing parameters.
     * @param [in] param See TranscoderClientMixParam{@link #TranscoderClientMixParam}.
     * @return See TranscoderClientMixParam{@link #TranscoderClientMixParam}.
     */
    virtual void setClientMixParam(TranscoderClientMixParam param) = 0;
    /** 
     * @hidden for internal use only
     * @valid since 3.50
     * @type api
     * @brief Sets the parameters of simplex mode and synchronization when pushing to CDN.
     * @param [in] param See TranscoderSyncControlParam{@link #TranscoderSyncControlParam}.
     * @return See TranscoderSyncControlParam{@link #TranscoderSyncControlParam}.
     */
    virtual void setSyncControlparam(TranscoderSyncControlParam param) = 0;

    /** 
     * @type api
     * @region Push to CDN
     * @brief Convert the json format string into the ITranscoderParam structure
     * @param [in] json_str Json format string
     * @return Converted ITranscoderParam structure
     */
    virtual ITranscoderParam* inflatten(const char* json_str) = 0;

    /** 
     * @type api
     * @region Push to CDN
     * @brief Get default transcoding param
     * @return Default ITranscoderParam structure
     */
    virtual ITranscoderParam* defaultTranscoding() = 0;
    /**
     * @hidden only provide for IMixedStreamConfig
     */
    virtual MixedStreamServerControlConfig getServerControlConfig() = 0;
    /**
     * @hidden only provide for IMixedStreamConfig
     */
    virtual void setServerControlConfig(MixedStreamServerControlConfig& config) = 0;
    /**
     * @hidden constructor/destructor
     */
    virtual ~ITranscoderParam() = default;
};

/** 
 * @type keytype
 * @brief Configurations to be set while pushing media streams to CDN.(New)
 */
class IMixedStreamConfig : public ITranscoderParamBase {
public:

    /** 
     * @type api
     * @region Push to CDN
     * @brief Gets the stream mixing type.
     * @return Stream mixing type. See MixedStreamType{@link #MixedStreamType}
     */
    virtual MixedStreamType getExpectedMixingType() = 0;
    /** 
     * @type api
     * @brief Gets the user ID for mixed stream.
     * @return The user ID for live mixed stream.
     */
    virtual const char* getUserID() = 0;
    /** 
     * @type api
     * @brief Gets the URL for live mixed stream.
     * @return The CDN url.
     */
    virtual const char* getPushURL() = 0;
    /** 
     * @type api
     * @region Forward to live broadcast
     * @brief Get user config extra information.
     * @return extraI Information.
     */
    virtual const char* getUserConfigExtraInfo() = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Gets the audio configurations.
     * @return Audio configurations. See MixedStreamAudioConfig{@link #MixedStreamAudioConfig}.
     */
    virtual MixedStreamAudioConfig getAudioConfig() = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Gets the video configurations.
     * @return Video configurations. See MixedStreamVideoConfig{@link #MixedStreamVideoConfig}.
     */
    virtual MixedStreamVideoConfig getVideoConfig() = 0;
    /** 
     * @hidden(Windows,macOS,Linux)
     * @type api
     * @brief Get the spatial audio configurations of pushing to CDN.
     * @return See MixedStreamSpatialAudioConfig{@link #MixedStreamSpatialAudioConfig}.
     */
    virtual MixedStreamSpatialAudioConfig getSpatialAudioConfig() = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Gets the video layout information of the mixed stream.
     * @param [in] index The number of the view of which you want to get information.
     * @return Layout information. See MixedStreamLayoutRegionConfig{@link #MixedStreamLayoutRegionConfig}.
     */
    virtual MixedStreamLayoutRegionConfig getLayoutByIndex(int32_t index) = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @region Forward to live broadcast
     * @brief Get dynamically extend customizable parameters
     * @return dynamically extend customizable parameters
     */
    virtual const char* getAdvancedConfig() = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @region Forward to live broadcast
     * @brief Get Business Transparent Authentication Information
     * @return Business Transparent Authentication Information
     */
    virtual const char* getAuthInfo() = 0;
    /** 
     * @type api
     * @brief Get client mixing parameters.
     * @return Client mixing parameters. See MixedStreamClientMixConfig{@link #MixedStreamClientMixConfig}.
     */
    virtual MixedStreamClientMixConfig getClientMixConfig()  = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @brief Get transcoding sync control parameters.
     * @return Transcoding sync control parameters. See MixedStreamSyncControlConfig{@link #MixedStreamSyncControlConfig}.
     */
    virtual MixedStreamSyncControlConfig getSyncControlConfig() = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Sets the expected stream mixing type. It's recommended to be set.
     * @param [in] expected_mix_type Stream mixing type. See MixedStreamType{@link #MixedStreamType}
     */
    virtual void setExpectedMixingType(MixedStreamType expected_mix_type) = 0;
    /** 
     * @type api
     * @brief Sets the user ID for live transcoding. The sum length of `room_id` and `user_id` should not exceed 126 bytes. It's recommended to be set.
     *        This parameter cannot be updated while pushing stream to the CDN.
     * @param [in] user_id The user ID for live transcoding.
     */
    virtual void setUserID(const char* user_id) = 0;
    /** 
     * @type api
     * @brief Sets the URL for live transcoding. Only supports live transcoding via RTMP. The URL should match the regular expression `/^rtmps?:\/\//`. It's recommended to be set.
     *        This parameter cannot be updated while pushing stream to the CDN.
     * @param [in] url The URL to be set.
     */
    virtual void setPushURL(const char* push_url) = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Sets audio configurations. See MixedStreamAudioConfig{@link #MixedStreamAudioConfig}. It's recommended to be set.
     *        This parameters cannot be updated while pushing stream to the CDN.
     */
    virtual void setAudioConfig(const MixedStreamAudioConfig&) = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Sets video configurations. See MixedStreamVideoConfig{@link #MixedStreamVideoConfig}. It's recommended to be set.
     */
    virtual void setVideoConfig(const MixedStreamVideoConfig&) = 0;
    /** 
     * @hidden(Windows,macOS,Linux)
     * @type api
     * @brief Sets spatial audio configurations. See MixedStreamSpatialAudioConfig{@link #MixedStreamSpatialAudioConfig}.
     */
    virtual void setSpatialAudioConfig(const MixedStreamSpatialAudioConfig&) = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Sets the layout configurations.
     * @param [in] regions[] User layout information list. It's a list of MixedStreamLayoutRegionConfig{@link #MixedStreamLayoutRegionConfig} that you construct for each stream.
     *                       With invalid or empty input, the configurations will be set as the default values. It's recommended to be set.
     * @param [in] regions_size Amount of views. It's recommended to be set.
     * @param [in] background_color Background-color of the mixed stream in hexadecimal values such as #FFFFFF and #000000. The default value is #000000. It's recommended to be set.
     *                      With invalid or empty input, the configurations will be set as the default values.
     * @param [in] user_extra_info Additional data that you want to import.
     */
    virtual void setLayoutConfig(MixedStreamLayoutRegionConfig regions[],
                                int32_t regions_size,
                                const char* background_color,
                                const char* user_extra_info) = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @region Forward to live broadcast
     * @brief Sets dynamically extend customizable parameters
     * @param [in] advanced_config  dynamically extend customizable parameters
     */
    virtual void setAdvancedConfig(const char* advanced_config) = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @region Forward to live broadcast
     * @brief Sets Business Transparent Authentication Information
     * @param [in] auth_info  Business Transparent Authentication Information
     */
    virtual void setAuthInfo(const char* auth_info) = 0;
    /** 
     * @type api
     * @brief Sets client mixing parameters. See MixedStreamClientMixConfig{@link #MixedStreamClientMixConfig}.
     */
    virtual void setClientMixConfig(MixedStreamClientMixConfig&) = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @brief Sets transcoding sync control parameters. See MixedStreamSyncControlConfig{@link #MixedStreamSyncControlConfig}.
     */
    virtual void setSyncControlConfig(MixedStreamSyncControlConfig&) = 0;
    /** 
     * @type api
     * @brief Gets the configurations while mixing streams on the server side. See MixedStreamServerControlConfig{@link #MixedStreamServerControlConfig}.
     */
    virtual MixedStreamServerControlConfig getServerControlConfig() = 0;
    /** 
     * @type api
     * @brief Sets the configurations while mixing streams on the server side. See MixedStreamServerControlConfig{@link #MixedStreamServerControlConfig}.
     */
    virtual void setServerControlConfig(MixedStreamServerControlConfig& config) = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Get default transcoding config
     * @return Default IMixedStreamConfig structure
     */
    virtual IMixedStreamConfig* defaultMixedStreamConfig() = 0;
    /**
     * @hidden convertToTranscoderParam
     */
    virtual ITranscoderParam* convertToTranscoderParam() = 0;
    /**
     * @hidden constructor/destructor
     */
    virtual ~IMixedStreamConfig() = default;
};

/** 
 * @type keytype
 * @brief Configurations for pushing a single media stream to CDN.
 */
typedef struct PushSingleStreamParam {
    /** 
     * @brief The room ID of the media stream
     */
    const char* room_id;
    /** 
     * @brief The user ID of the media stream
     */
    const char* user_id;
    /** 
     * @brief The URL for live transcoding. Only supports live transcoding via RTMP. The URL should match the regular expression `/^rtmps?:\/\//`.
     *        This parameter cannot be updated while pushing stream to the CDN.
     */
    const char* uri;
    /** 
     * @brief Whether the media stream is a screen-sharing stream.
     */
    const bool is_screen_stream;
}PushSingleStreamParam;

/** 
* @hidden internal use only
* @type keytype
*/
enum ChorusCacheSyncMode {
    /** 
     * @brief Under chorus scenarios, the lead singer uses the mode to send the media data with timestamp attached.
     */
    kChorusCacheSyncModeProducer= 0,
    /** 
     * @brief Under chorus scenarios, the sub singer uses the mode.
     *        In this mode, the sub singer receives the media data from the lead singer with timestamp attached. The sub singer sends the media data with the lead singer's timestamp attached.
     */
    kChorusCacheSyncModeRetransmitter= 1,
    /** 
     * @brief Under chorus scenarios, the audience uses the mode.
     *        In this mode, the audience receives the lead singer's timestamp. The audience aligns and renders the media data from lead and sub singers, and gets good chorus experience.
     */
    kChorusCacheSyncModeConsumer = 2
};
/** 
* @hidden internal use only
* @type keytype
*/
enum ChorusCacheSyncEvent {
     /** 
     * @brief Success
     */
    kChorusCacheSyncEventStartSuccess = 0,
     /** 
     * @brief Failed
     */
    kChorusCacheSyncEventStartFailed = 1,
};
/** 
* @hidden internal use only
* @type errorcode
*/
enum ChorusCacheSyncError {
    /** 
     * @brief OK
     */
    kChorusCacheSyncErrorOK = 0,
   /** 
    * @type Failure. Check if the config of pushing mixed streams to CDN is as follows:
    *        + `IMixedStreamConfig.MixedStreamSyncControlConfig.enable_sync = true`；
    *        + `IMixedStreamConfig.MixedStreamSyncControlConfig.base_user_id = {uid of producer}`。
    */   
    kChorusCacheSyncErrorWrongState = 1,
    /** 
    * @type Already running. Do not enable the feature repeatedly.
    */
    kChorusCacheSyncErrorAlreadyRunning = 2
};
/** 
* @hidden internal use only
* @type keytype
*/
struct ChorusCacheSyncConfig {
     /** 
      * @brief Maximum duration of media data cached in ms.
      *        `2000` by default. The range is `[500, 2500]`. 
      *        Use higher value for better syncing effect. If the time difference between media streams in cache synchronization exceeds this value, frame loss will occur.
      */
    int32_t max_cache_time_ms = 2000;
    /** 
      * @brief The frequency of receiving onSyncedVideoFrames{@link #IChorusCacheSyncObserver#onSyncedVideoFrames}.
      *        `15` by default. The value is smaller than that of the original video fps usually; if the value is bagger than that, you may receive duplicated video frame.
      */
    int32_t video_fps = 15;
    /** 
      * @brief Mode. See ChorusCacheSyncMode{@link #ChorusCacheSyncMode}. `retransmitter` by default.
      */
    ChorusCacheSyncMode mode = kChorusCacheSyncModeRetransmitter;
};
}  // namespace bytertc
