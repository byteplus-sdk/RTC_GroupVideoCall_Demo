/*
 * Copyright 2021 @bytedance
 *  Created on: june 20, 2021
 */

#pragma once
#include "bytertc_video_frame.h"
#include "bytertc_audio_frame.h"
#include "bytertc_transcoder_base_interface.h"

#define TRANSCODE_ERR_OK 0
#define TRANSCODE_ERR_BASE 1090
#define TRANSCODE_ERR_INVALID_PARAM 1091
#define TRANSCODE_ERR_INVALID_STATE 1092
#define TRANSCODE_ERR_INVALID_OPERATOR 1093
#define TRANSCODE_ERR_TIMEOUT 1094
#define TRANSCODE_ERR_INVALID_PARAM_BY_SERVER 1095
#define TRANSCODE_ERR_SUB_TIMEOUT_BY_SERVER 1096
#define TRANSCODE_ERR_INVALID_STATE_BY_SERVER 1097
#define TRANSCODE_ERR_AUTHENTICATION_BY_CDN 1098
#define TRANSCODE_ERR_TIMEOUT_BY_SIGNALING 1099
#define TRANSCODE_ERR_PUSH_PUBLIC_STREAM_FAIL 1100
#define TRANSCODE_ERR_MIX_IMAGE_FAIL 1101
#define TRANSCODE_ERR_UNKNOW_ERROR_BY_SERVER 1102
#define TRANSCODE_ERR_UPDATE_REGION_CHANGED 1103
#define TRANSCODE_ERR_MAX 1199

namespace bytertc {
/** 
 * @type keytype
 * @brief Events during pushing streams to CDN
 */
enum StreamMixingEvent {
    /**
     * @hidden for internal use only
     */
    kStreamMixingBase = 0,
    /** 
     * @brief Request to start pushing streams to CDN
     */
    kStreamMixingStart = 1,
    /** 
     * @brief ask to push streams to CDN started
     */
    kStreamMixingStartSuccess = 2,
    /** 
     * @brief Failed to start the task to push streams to CDN
     */
    kStreamMixingStartFailed = 3,
    /** 
     * @brief Request to update the configuration for the task to push streams to CDN
     */
    kStreamMixingUpdate = 4,
    /** 
     * @brief Successfully update the configuration for the task to push streams to CDN
     */
    kStreamMixingUpdateSuccess = 5,
    /** 
     * @brief Failed to update the configuration for the task to push streams to CDN
     */
    kStreamMixingUpdateFailed = 6,
    /** 
     * @brief Request to stop the task to push streams to CDN
     */
    kStreamMixingStop = 7,
    /** 
     * @brief The task to push streams to CDN stopped
     */
    kStreamMixingStopSuccess = 8,
    /** 
     * @brief Failed to stop the task to push streams to CDN
     */
    kStreamMixingStopFailed = 9,
    /** 
     * @brief Timeout for the request to update the configuration for the task to push streams to CDN.
     */
    kStreamMixingChangeMixType = 10,
    /** 
     * @brief Got the first frame of the mixed audio stream by client.
     */
    kStreamMixingFirstAudioFrameByClientMix = 11,
    /** 
     * @brief Got the first frame of the mixed video stream by client.
     */
    kStreamMixingFirstVideoFrameByClientMix = 12,
    /** 
     * @brief Timeout for the request to update the configuration for the task to push streams to CDN
     */
    kStreamMixingUpdateTimeout = 13,
    /** 
     * @brief Timeout for the request to start the task to push streams to CDN
     */
    kStreamMixingStartTimeout = 14,
    /** 
     * @brief Error in the parameters of the request for the task to push streams to CDN
     */
    kStreamMixingRequestParamError = 15,
    /** 
     * @brief Mixing image.
     */
    kStreamMixingMixImageEvent = 16,
    /** 
     * @hidden internal use only
     * @hidden currently not available
     * @brief Simplex chorus event
     */
    kStreamMixingMixSingleWayChorusEvent = 17,
};


/** 
 * @type errorcode
 * @brief Errors occurring during pushing a single stream to CDN
 */
enum SingleStreamPushEvent {
    /**
     * @hidden for internal use only
     */
    kSingleStreamPushBase = 0,
    /** 
    * @brief Starting pushing a single stream to CDN.
    */
    kSingleStreamPushStart = 1,
    /** 
    * @brief Successfully pushed a single stream to CDN.
    */
    kSingleStreamPushSuccess = 2,
    /** 
    * @brief Failed to push a single stream to CDN.
    */
    kSingleStreamPushFailed = 3,
    /** 
     * @brief Stop pushing a single stream to CDN.
     */
    kSingleStreamPushStop = 4,
    /** 
    * @brief Request timed out. Please check network status and retry.
    */
    kSingleStreamPushTimeout = 5,
    /** 
     * @brief Request failed due to invalid parameter.
     */
    kSingleStreamPushParamError = 6,
};

/** 
 * @type errorcode
 * @brief Errors occuring during pushing streams to CDN
 */
enum StreamMixingErrorCode {
    /** 
     * @brief Successfully pushed streams to target CDN.
     */
    kStreamMixingErrorOK = 0,
    /** 
     * @brief Undefined error
     */
    kStreamMixingErrorBase= 1090,
    /** 
     * @brief Invalid parameters detected by Client SDK.
     */
    kStreamMixingErrorInvalidParam = 1091,
    /** 
     * @brief Program runs with an error, the state machine is in abnormal condition.
     */
    kStreamMixingErrorInvalidState = 1092,
    /** 
     * @brief Invalid operation
     */
    kStreamMixingErrorInvalidOperator = 1093,
    /** 
     * @brief Request timed out. Please check network status and retry.
     */
    kStreamMixingErrorTimeout = 1094,
    /** 
     * @brief Invalid parameters detected by the server
     */
    kStreamMixingErrorInvalidParamByServer = 1095,
    /** 
     * @brief Subscription to the stream has expired.
     */
    kStreamMixingErrorSubTimeoutByServer = 1096,
    /** 
     * @brief Internal server error.
     */
    kStreamMixingErrorInvalidStateByServer = 1097,
    /** 
     * @brief The server failed to push streams to CDN.
     */
    kStreamMixingErrorAuthenticationByCDN  = 1098,
    /** 
     * @brief Signaling connection timeout error. Please check network status and retry.
     */
    kStreamMixingErrorTimeoutBySignaling = 1099,
    /** 
     * @brief Failed to mix image.
     */
    kStreamMixingErrorMixImageFail = 1100,
    /** 
     * @brief Unknown error from server.
     */
    kStreamMixingErrorUnKnownByServer = 1101,
    /** 
     * @hidden internal use only
     * @brief The cache is not synchronized.
     */
    kStreamMixingErrorCacheSyncWorse = 1102,
    /**
     * @hidden for internal use only
     */
    kStreamMixingErrorMax = 1199,
};

/** 
 * @type keytype
 * @brief Stream mixing type
 */
enum StreamMixingType {
    /** 
     * @brief Server-side stream mixing
     */
    kStreamMixingTypeByServer = 0,
    /** 
     * @brief Intelligent stream mixing. The SDK will intelligently decide that a stream mixing task would be done on the client or the server.
     */
    kStreamMixingTypeByClient = 1,
};


/** 
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
     * {en}
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
    int32_t i32_z_order = 0;
    /** 
     * @brief Whether the source user of the stream is a local user:   <br>
     *         + true: Yes <br>
     *         + false: No
     */
    bool local_user = false;
    /** 
     * @brief Whether the stream comes from screen sharing:   <br>
     *         + true: Yes <br>
     *         + false: No
     */
    bool screen_stream = false;
    /** 
     * @brief The stream mixing content type. The default value is `kHasAudioAndVideo`. See TranscoderContentControlType{@link #TranscoderContentControlType}.
     */
    TranscoderContentControlType content_control = kHasAudioAndVideo;
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


} TranscoderLayoutRegion;

/** 
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
 * @type keytype
 * @brief Configrations for mixing and pushing to CDN at client side.
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
     * @hidden internal use only
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
     * @brief Sets audio configurations.
     *        These parameters cannot be updated while pushing stream to the CDN.
     * @param [in] TranscoderAudioParam Audio configurations. See TranscoderAudioParam{@link #TranscoderAudioParam}
     */
    virtual void setAudioParam(const TranscoderAudioParam&) = 0;
    /** 
     * @type api
     * @region Push to CDN
     * @brief Sets video configurations.
     * @param [in] TranscoderVideoParam Video configurations. See TranscoderVideoParam{@link #TranscoderVideoParam}
     */
    virtual void setVideoParam(const TranscoderVideoParam&) = 0;
    /** 
     * @hidden(Windows,macOS,Linux)
     * @type api
     * @brief Sets spatial audio configurations
     * @param [in] TranscoderSpatialConfig See TranscoderSpatialConfig{@link #TranscoderSpatialConfig}.
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
     * @brief  Set dynamically extend customizable parameters
     * @param [in] advancedConfig  dynamically extend customizable parameters
     */
    virtual void setAdvancedConfig(const char* advancedConfig) = 0;
    /** 
     * @hidden for internal use only
     * @type api
     * @region Forward to live broadcast
     * @brief Set Business Transparent Authentication Information
     * @param [in] authInfo  Business Transparent Authentication Information
     */
    virtual void setAuthInfo(const char* authInfo) = 0;
    /** 
     * @hidden internal use only
     * @valid since 3.50
     * @type api
     * @brief Set client mixing parameters.
     * @param [in] param See TranscoderClientMixParam{@link #TranscoderClientMixParam}.
     * @return See TranscoderClientMixParam{@link #TranscoderClientMixParam}.
     */
    virtual void setClientMixParam(TranscoderClientMixParam param) = 0;
    /** 
     * @hidden for internal use only
     * @valid since 3.50
     * @type api
     * @brief Set the parameters of simplex mode and synchronization when pushing to CDN.
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
     * @hidden constructor/destructor
     */
    virtual ~ITranscoderParam() = default;
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
     * @brief the target address of the CDN
     */
    const char* uri;
    /** 
     * @brief Whether the media stream is a screen-sharing stream.
     */
    const bool is_screen_stream;
}PushSingleStreamParam;
}  // namespace bytertc
