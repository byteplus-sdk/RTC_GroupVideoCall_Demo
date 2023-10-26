/*
 * Copyright (c) 2022 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Video Defines
 */

#pragma once

#include "bytertc_rts_defines.h"
#include "bytertc_video_frame.h"

namespace bytertc {
/** 
 * @type keytype
 * @brief Rendering mode
 */
enum RenderMode {
    /** 
     * @brief (Default) Fill and Crop.
     *        The video frame is scaled with fixed aspect ratio, and completely fills the canvas. The region of the video exceeding the canvas will be cropped. <br>
     */
    kRenderModeHidden = 1,
    /** 
     * @brief Fit.
     *        The video frame is scaled with fixed aspect ratio, and is shown completely in the canvas. The region of the canvas not filled with video frame, is filled with `background`.
     */
    kRenderModeFit = 2,
    /** 
     *  @brief Fill the canvas.
     *         The video frame is scaled to fill the canvas. During the process, the aspect ratio may change.
     */
    kRenderModeFill = 3,
};

/** 
 * @type keytype
 * @brief Type of the media stream subscribed to
 */
enum SubscribeMediaType {
    /** 
     * @brief Neither audio nor video
     */
    kRTCSubscribeMediaTypeNone = 0,
    /** 
     * @brief Audio only
     */
    kRTCSubscribeMediaTypeAudioOnly,
    /** 
     * @brief Video only
     */
    kRTCSubscribeMediaTypeVideoOnly,
    /** 
     * @brief Video only
     */
    kRTCSubscribeMediaTypeVideoAndAudio
};

/** 
 * @type keytype
 * @brief Type of the stream to be paused/resumed.
 */
enum PauseResumeControlMediaType {
    /** 
     * @brief Audio only
     */
    kRTCPauseResumeControlMediaTypeAudio = 0,
    /** 
     * @brief Video only
     */
    kRTCPauseResumeControlMediaTypeVideo = 1,
    /** 
     * @brief Both video and audio
     */
    kRTCPauseResumeControlMediaTypeVideoAndAudio = 2
};

/** 
 * @type keytype
 * @brief  Time domain hierarchical definition
 */
enum SVCLayer {
    /** 
    * @brief No hierarchy specified (default)
    */
    kSVCLayerDefault = 0,
    /** 
     * @brief T0 layer
     */
    kSVCLayerBase = 1,
    /** 
     * @brief T0 + T1 layer
     */
    kSVCLayerMain = 2,
    /** 
     * @brief T0 + T1 + T2 layer
     */
    kSVCLayerHigh = 3,
};

/** 
 * @type keytype
 * @brief Information of stream switching due to a Fallback
 */
struct RemoteStreamSwitch {
    /** 
     * @brief User ID of the publisher of the subscribed media stream
     */
    const char* uid;
    /** 
     * @brief Whether it is a screen-sharing stream
     */
    bool is_screen;
    /** 
     * @brief Index corresponding to the resolution of the subscribed video stream before the stream switch.
     */
    int before_video_index;
    /** 
     * @brief Index corresponding to the resolution of the subscribed video stream after the stream switch.
     */
    int after_video_index;
    /** 
     * @brief Is there a video stream before switching.
     */
    bool before_enable;
    /** 
     * @brief Is there a video stream after switching.
     */
    bool after_enable;
    /** 
     * @brief Refer to FallbackOrRecoverReason{@link #FallbackOrRecoverReason} for the reason of the Fallback or reverting from the Fallback of the subscribed stream.
     */
    FallbackOrRecoverReason reason;
};

/** 
 * @type keytype
 * @brief Background mode
 */
enum BackgroundMode {
    /** 
     *@brief No
     */
    kBackgroundModeNone = 0,
    /** 
     *@brief Virtualization
     */
    kBackgroundModeBlur = 1,
    /** 
     *@brief Background 1
     */
    kBackgroundModeA = 2,
    /** 
     *@brief Background 2
     */
    kBackgroundModeB = 3,
};

/** 
 * @type keytype
 * @brief To split the model
 */
enum DivideMode {
    /** 
     *@brief Self-research
     */
    kDivideModeNone = 0,
    /**
     *@brief effect
     */
    kDivideModeEffect = 1,
};

/** 
 * @type keytype
 * @brief Flow property.   <br>
 */
struct MediaStreamInfo {
    /** 
     * @brief The user ID that published this stream.
     */
    const char* user_id;
    /** 
     * @hidden for internal use only
     * @brief The flag of the stream, unique in user scope.
     */
    const char* stream_name;
    /** 
     * @brief Whether this stream is a shared screen stream.
     */
    bool is_screen;
    /** 
     * @brief Whether this stream includes a video stream.
     */
    bool has_video;
    /** 
     * @brief Whether the stream includes an audio stream.
     */
    bool has_audio;
    /** 
     * @brief Properties of the video stream.   <br>
     *        When a remote user calls the setVideoEncoderConfig{@link #IRTCVideo#setVideoEncoderConfig} method to publish multiple configured video streams, this will contain attribute information for all video streams published by the user.
     *        See VideoSolutionDescription{@link #VideoSolutionDescription}.
     */
    VideoSolutionDescription* profiles;
    /** 
     * @brief Number of different configuration streams.   <br>
     *        When a remote user calls the setVideoEncoderConfig{@link #IRTCVideo#setVideoEncoderConfig} method to publish multiple configured video streams, this contains the number of video streams published by the user.
     */
    int profile_count;

    /** 
     * @brief Properties of the maximum video stream.   <br>
     *        When a remote user publishes multiple configured video streams, this will contain the maximum attribute information . user with new subscribe call can require any stream does not exceed this resolution.
     *        The final stream received, influenced by the others in room, server would diliver the closest to the resolution requested by the user。
     *
     */
    VideoSolutionDescription max_profile;
};

/** 
 * @type keytype
 * @brief  Video subscription configuration information
 */
struct SubscribeVideoConfig {
    /** 
     * @brief Subscribed video stream resolution subscript.   <br>
     *        In Simulcast mode, use a number to specify the expected quality of the video stream to be subscribed to.  In Simulcast mode, a video has a diversity of encoded qualities that ranking from 0 to 3. Call setVideoEncoderConfig{@link #IRTCVideo#setVideoEncoderConfig} to enable Simulcast mode on the publisher's clients.  <br>
     *         Ranging from -1 to 3<br>
     *         0 (Default): The best quality <br>
     *         -1: Use the previous settings. <br>
     */
    int video_index = 0;
    /** 
     * @brief Remote user priority. The default value is 0. Refer to RemoteUserPriority{@link #RemoteUserPriority} for more details.
     */
    int priority = 0;
    /** 
     * @hidden constructor/destructor
     * @brief Constructor
     */
    SubscribeVideoConfig() : video_index(0), priority(0) {
    }
    /**
     * @hidden constructor/destructor
     */
    bool operator==(const SubscribeVideoConfig& config) const {
        bool result = video_index == config.video_index && priority == config.priority;
        return result;
    }
    /**
     * @hidden constructor/destructor
     */
    bool operator!=(const SubscribeVideoConfig& config) const {
        bool result = (*this == config);
        return !result;
    }
};

/** 
 * @type keytype
 * @brief Configuration information for manual subscription flows.  
 */
struct SubscribeConfig {
    /** 
     * @brief Whether it is a screen stream (default is no).   
     */
    bool is_screen = false;
    /** 
     * @brief Whether to subscribe to videos.   <br>
     */
    bool sub_video;
    /** 
     * @brief Whether to subscribe to audio.   <br>
     */
    bool sub_audio;
    /** 
     * @brief Subscribed video stream resolution subscript.   <br>
     *        Users can publish multiple videos of different resolutions in a stream by calling the setVideoEncoderConfig{@link #IRTCVideo#setVideoEncoderConfig} method. Therefore, when subscribing to a stream, you need to specify the specific resolution of the subscription. This parameter is used to specify the subscript of the resolution to be subscribed to, and the default value is 0. <br>
     */
    int video_index;
    /** 
     * @brief For the remote user's requirement priority. See RemoteUserPriority{@link #RemoteUserPriority}, the default value is 0. <br>
     *         When the subscription flow fallback option function is turned on (see setSubscribeFallbackOption{@link #IRTCVideo#setSubscribeFallbackOption} method for details), weak connections or insufficient performance will give priority to ensuring the quality of the received high-priority user's flow. <br>
     */
    int priority = 0;
    /** 
     * @brief The time domain hierarchy of the remote user. See SVCLayer{@link #SVCLayer}, the default value is 0. <br>
     *         This only works if the stream supports the SVC feature. <br>
     */
    int svc_layer = 0;
    /** 
     * @brief Expected maximum frame rate of the subscribed stream in fps. The default value is 0, values greater than 10 are valid.  <br>
     *        If the frame rate of the stream published is lower than the value you set, or if your subscribed stream falls back under limited network conditions, the frame rate you set will drop accordingly.  <br>
     *        Only valid if the stream is coded with SVC technique.
     */
    int framerate = 0;
    /** 
     * @brief The user specifies the width(px) of the most appropriate stream corresponding to the UI by specifying
     */
    int sub_width = 0;
    /** 
     * @brief The user specifies the height(px) of the most appropriate stream corresponding to the UI by specifying
     */
    int sub_height = 0;
    /**
     * @hidden for internal use only
     */
    int sub_video_index = -1;

public:
    /** 
     * @hidden constructor/destructor
     * @brief Constructor
     */
    SubscribeConfig() : is_screen(false), sub_video(true), sub_audio(true), video_index(0), priority(0) {
    }

    /** 
     * @hidden constructor/destructor
     * @brief Constructor
     */
    SubscribeConfig(bool is_screen, bool subvideo, bool subaudio, int videoindex)
            : is_screen(is_screen), sub_video(subvideo), sub_audio(subaudio), video_index(videoindex) {
    }

    /** 
     * @hidden constructor/destructor
     * @brief Constructor
     */
    SubscribeConfig(bool is_screen, bool subvideo, bool subaudio, int videoindex, int priority)
            : is_screen(is_screen),
              sub_video(subvideo),
              sub_audio(subaudio),
              video_index(videoindex),
              priority(priority) {
    }

    /** 
     * @hidden constructor/destructor
     * @brief constructor
     */
    SubscribeConfig(bool is_screen, bool subvideo, bool subaudio, int videoindex,
     int priority,int svc_layer,int width,int height)
            : is_screen(is_screen),
              sub_video(subvideo),
              sub_audio(subaudio),
              video_index(videoindex),
              priority(priority),
              svc_layer(svc_layer),
              sub_width(width),
              sub_height(height) {
    }
    /**
     * @hidden constructor/destructor
     */
    bool operator==(const SubscribeConfig& config) const {
        // sub_width * sub_height valid
        bool common_result = is_screen == config.is_screen && sub_video == config.sub_video
                                          && sub_audio == config.sub_audio && priority == config.priority
                                          && svc_layer == config.svc_layer && framerate == config.framerate;
        bool result;
        if(sub_width * sub_height > 0 && config.sub_width * config.sub_height > 0) {
          result = common_result && sub_width == config.sub_width && sub_height == config.sub_height ;
        }  else if((sub_width * sub_height == 0) && (config.sub_width * config.sub_height == 0) ) {
          result = common_result && video_index == config.video_index;
        } else {
          result = false;
        }
        return result;
    }

    /**
     * @hidden constructor/destructor
     */
    bool operator!=(const SubscribeConfig& config) const {
        bool result = (*this == config);
        return !result;
    }
};

/** 
 * @type keytype
 * @brief Remote video type
 *      Currently C++ sdk only supports kVideoStreamTypeHigh type
 */
enum VideoStreamType {
    /** 
     * @brief High Resolution Video Streaming
     */
    kVideoStreamTypeHigh = 0,
    /** 
     * @brief Low Resolution Video
     */
    kVideoStreamTypeLow = 1,
};

/** 
 * @type keytype
 * @brief Render target type
 */
enum RenderTargetType {
    /** 
     * @brief Use this value when the target is in the range of SurfaceView/TextureView for Android, UIView for iOS, NSView for macOS, or HWND for Windows
     */
    kRenderTargetView = 0,
};

/** 
 * @type keytype
 * @brief Video rendering configuration.
 */
struct VideoCanvas {
    /** 
     * @brief Local view handle
     */
    void* view = NULL;
    /** 
     * @brief Video rendering mode. See RenderMode{@link #RenderMode}
     */
    int render_mode = kRenderModeHidden;
    /** 
     * @brief Set the background color of the canvas which is not filled with video frame. The range is `[0x00000000, 0xFFFFFFFF]`. The default is `0x00000000`. The Alpha index is ignored.
     */
    uint32_t background_color = 0;
    /** 
     * @brief See RenderTargetType{@link #RenderTargetType}.
     */
    RenderTargetType render_target_type = kRenderTargetView;
    /** 
     * @brief Video frame rotation angle. See VideoRotation{@link #VideoRotation}. The default value is 0, which means no rotation is applied.<br>
     *        This parameter only applies to remote video and does not affect local video settings.
     */
     VideoRotation render_rotation = kVideoRotation0;
    /** 
     * @hidden constructor/destructor
     * @brief Constructor
     */
    VideoCanvas() : view(NULL), render_mode(kRenderModeHidden), background_color(0), render_target_type(kRenderTargetView) {
    }
    /** 
     * @hidden constructor/destructor
     * @brief Constructor
     */
    VideoCanvas(void* v, int m, uint32_t c) : view(v), render_mode(m), background_color(c) {
    }
};

/** 
 * @type keytype
 * @brief Remote video rendering configuration.
 */
struct RemoteVideoRenderConfig {
    /** 
     * @brief Rendering mode.
     *        + 1（`RENDER_MODE_HIDDEN`）Fill and Crop. Default setting. The video frame is scaled with a fixed aspect ratio and completely fills the canvas. The region of the video exceeding the canvas will be cropped.
     *        + 2（`RENDER_MODE_FIT`）Fit. The video frame is scaled with a fixed aspect ratio and is shown completely on the canvas. The region of the canvas not filled with the video frame will be filled with `backgroundColor`.
     *        + 3（`RENDER_MODE_FILL`）Fill the canvas. The video frame is scaled to fill the canvas. During the process, the aspect ratio may change.
     */
    int render_mode = kRenderModeHidden;
    /** 
     * @brief Set the background color of the canvas which is not filled with video frame. The range is `[0x00000000, 0xFFFFFFFF]`. The default is `0x00000000`. The Alpha index is ignored.
     */
    uint32_t background_color = 0;

    /** 
     * @brief User-defined render rotation。the value must be 0, 90, 180. 270, 0 mean keeping origin canvas orientation, 90, 180, and 270 represent clockwise rotation angles of the original canvas.
     */
    VideoRotation render_rotation = kVideoRotation0;
};

/** 
 * @type keytype
 * @brief Windows window id, Windows platform down to Windows window handle
 */
typedef void* view_t;

/** 
 * @type keytype
 * @brief  A rectangle that you can move around and re-size to share just the portion of the screen defined by that rectangle.
 */
struct Rectangle {
    /** 
     * @brief The upper-left X coordinates of the rectangular.
     */
    int x = 0;
    /** 
     * @brief The upper-left Y coordinates of the rectangular.
     */
    int y = 0;
    /** 
     * @brief Width.
     */
    int width = 0;
    /** 
     * @brief Height.
     */
    int height = 0;
};

/** 
 * @type keytype
 * @brief Configure how a border is highlighted when you share your screen.
 */
struct HighlightConfig {
    /** 
     * @brief Whether to display a highlighted border. A border is highlighted by default.
     */
    bool enable_highlight = true;
    /** 
     * @brief Color(ARGB format: 0xAARRGGBB).
     */
    uint32_t border_color = 0xFF29CCA3;
    /** 
     * @brief Width(in pixel).
     */
    int border_width = 4;
};

/** 
 * @type keytype
 * @brief  Exclude a specific window when sharing your screen. No window is excluded by default.
 */
struct ScreenFilterConfig {
    /** 
     * @brief The list of windows excluded from screen capture. This parameter only takes effect when sharing your screen.
     */
    view_t* excluded_window_list = nullptr;
   /** 
     * @brief The number of windows excluded from screen capture.
     */
    int excluded_window_num = 0;
};

/** 
 * @type keytype
 * @region Screen sharing
 * @brief Whether to capture the mouse cursor when capturing screen streams by RTC.
 */
enum MouseCursorCaptureState {
    /** 
     * @brief Yes.
     */
    kMouseCursorCaptureStateOn,
    /** 
     * @brief No.
     */
    kMouseCursorCaptureStateOff,
};

/** 
 * @type keytype
 * @brief  Screen sharing parameter
 */
struct ScreenParameters {
    /** 
     * @brief Encoded frame rate in fps
     */
    int frame_rate = 15;
    /**  
     * @brief Encoding bitrate in kbps of the screen-sharing stream. `-1` for self-adapted that SDK will choose the appropriate encoding rate according to the width and height for the stream.
     */
    int kbitrate = -1;
    /**  
     * @brief Minimum video encoding bitrate in kbps. The encoding bitrate will not be lower than the `min_kbitrate`.<br>
     *        It defaults to `0`. <br>
     *        It ranges within [0, kbitrate). When `bitrate` < `min_kbitrate`, the bitrate is self-adpapted.<br>
     *        In the following circumstance, the assignment to this variable has no effect:<br>
     *        + When `min_kbitrate` = `0`, the video encoding is disabled.<br>
     *        + When `min_kbitrate` < `0`, the bitrate is self-adapted.
     */
    int min_kbitrate = 0;
};

/** 
 * @type keytype
 * @region Screen sharing
 * @brief Content hints of the internally captured screen video stream.
 */
enum ContentHint {
    /** 
     * @brief Detailed content. Recommended when you capture documents, images, etc. Default value.
     */
    kContentHintDetails = 0,
    /** 
     * @brief  Animation content. Recommended when you capture videos, games, etc.
     */
    kContentHintMotion,
};

/** 
 * @type keytype
 * @brief  Screen internal capture parameters
 */
struct ScreenCaptureParameters {
    /** 
     * @brief Collection area. See Rectangle{@link #Rectangle}.
     */
    Rectangle region_rect;
    /** 
     * @brief To collect mouse status. See MouseCursorCaptureState{@link #MouseCursorCaptureState}.
     */
    MouseCursorCaptureState capture_mouse_cursor = MouseCursorCaptureState::kMouseCursorCaptureStateOn;
    /** 
     * @brief Screen filtering settings. Provide ID of the windows to be excluded. See ScreenFilterConfig{@link #ScreenFilterConfig}.
     */
    ScreenFilterConfig filter_config;
    /** 
     * @brief Settings for highlighting borders of the sharing region. See HighlightConfig{@link #HighlightConfig}.
     */
    HighlightConfig highlight_config;
};

/** 
 * @type keytype
 * @brief  The encoding configuration for screen sharing.
 */
struct DesktopCaptureParameters {
    /** 
     * @brief The maximum width(in px).
     */
    int max_width = 1920;
    /** 
     * @brief The maximum height(in px).
     */
    int max_height = 1080;
    /** 
     * @brief The frame rate(in fps).
     */
    int frame_rate = 15;
    /** 
     * @brief The bitrate. If you set this value to -1, RTC will automatically recommend the bitrate(in kbps) based on the input resolution.
     */
    int bitrate = -1;
    /** 
     * @brief The minimum bitrate(in kbps).Optional for internal capture while mandatory for custom capture.  <br>
     *        The minimum bitrate must be set lower than the maximum bitrate. Otherwise, the streams will not be encoded and published. 
     */
    int min_bitrate = 0;
    /** 
     * @brief Whether to capture the mouse cursor.
     */
    bool capture_mouse_cursor = true;
    /** 
     * @brief The list of windows excluded from screen capture. This parameter only takes effect when you capture your screen.
     */
    view_t* excluded_window_list = nullptr;
    /** 
     * @brief The number of windows excluded from screen capture.
     */
    int excluded_window_num = 0;
    /** 
     * @brief Configure how a border is highlighted.
     */
    HighlightConfig highlight_config;
};

/** 
 * @type keytype
 * @brief Custom video renderer
 */
class IVideoSink {
public:
    /** 
     * @type keytype
     * @brief Video frame encoding format
     */
    enum PixelFormat {
        /** 
         * @brief YUV I420 format
         */
        kI420 = VideoPixelFormat::kVideoPixelFormatI420,
        /** 
         * @brief RGBA format
         */
        kRGBA = VideoPixelFormat::kVideoPixelFormatRGBA,
        /** 
         * @brief Original format
         */
		kOriginal = VideoPixelFormat::kVideoPixelFormatUnknown,
    };
    /** 
     * @type callback
     * @brief Video frame callback
     * @param [out] video_frame Video frame structure. See IVideoFrame{@link #IVideoFrame}.
     * @return Temporarily unavailable
     */
    virtual bool onFrame(IVideoFrame* video_frame) = 0;

    /** 
     * @type callback
     * @region Room Management
     * @brief Gets the time taken in custom rendering.
     * @notes Gets the time taken in custom rendering and report. You need to calculate the average rendering time by yourself.
     */
    virtual int getRenderElapse() = 0;
    /** 
     * @type callback
     * @brief Releases the renderer.
     * @notes Used to notify the user that the renderer is about to be deprecated. Resources can be released upon receipt of this notification.
     */
    virtual void release() {
    }
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IVideoSink() = default;
};

/** 
 * @hidden
 * @type keytype
 * @deprecated since 3.45 and will be deleted in 3.51, use MirrorType{@link #MirrorType} instead.
 * @brief Whether to turn on mirror mode
 */
enum MirrorMode {
    /** 
     * @brief Not open
     */
    kMirrorModeOff = 0,
    /** 
     * @brief Open
     */
    kMirrorModeOn = 1,
};

/** 
 * @type keytype
 * @brief  Mirror type
 */
enum MirrorType : int {
    /** 
     * @brief The preview and the published video stream are not mirrored.
     */
    kMirrorTypeNone = 0,
    /** 
     * @brief The preview is mirrored. The published video stream is not mirrored.
     */
    kMirrorTypeRender = 1,
    /** 
     * @brief The preview and the published video stream are mirrored.
     */
    kMirrorTypeRenderAndEncoder = 3,
};

/** 
 * @type keytype
 * @brief  Low light adjusted type
 */
enum AdjustedMode {
    /** 
     * @brief Turn off low light adaptation
     */
    kAdjustedModeDisabled = 0,
    /** 
     * @brief Turn on low light adaptation
     */
    kAdjustedModeEnabled = 1,
};

/** 
 * @type keytype
 * @brief Video source type
 */
enum VideoSourceType {
    /** 
     * @brief Custom video source
     */
    kVideoSourceTypeExternal = 0,
    /** 
     * @brief Internal video capture
     */
    kVideoSourceTypeInternal = 1,
    /** 
     * @brief Custom encoded video source.   <br>
     *        Push the encoded video stream with the largest resolution, and the SDK will automatically transcode to generate multiple lower-quality streams for Simulcast.
     */
    kVideoSourceTypeEncodedWithAutoSimulcast = 2,
    /** 
     * @brief Custom encoded video source.   <br>
     *         The SDK does not automatically generate multiple streams for Simulcast, you need to generate and push streams of different qualities.
     */
    kVideoSourceTypeEncodedWithoutAutoSimulcast = 3,
};

/** 
 * @type keytype
 * @brief Digital Zoom type
 */
enum ZoomConfigType {
    /** 
     * @brief To set the offset for zooming in and zooming out.
     */
    kZoomConfigTypeFocusOffset = 0, 
    /** 
     * @brief To set the offset for panning and tiling.
     */
    kZoomConfigTypeMoveOffset,
};

/** 
 * @type keytype
 * @brief Action of the digital zoom control
 */
enum ZoomDirectionType {
    /** 
     * @brief Move to the left.
     */
    kZoomDirectionTypeMoveLeft = 0,
    /** 
     * @brief Move to the right.
     */
    kZoomDirectionTypeMoveRight,
    /** 
     * @brief Move upwards.
     */
    kZoomDirectionTypeMoveUp,
    /** 
     * @brief Move downwards.
     */
    kZoomDirectionTypeMoveDown,
    /** 
     * @brief Zoom out.
     */
    kZoomDirectionTypeZoomOut,
    /** 
     * @brief Zoom in.
     */
    kZoomDirectionTypeZoomIn,
    /** 
     * @brief Reset digital zoom.
     */
    kZoomDirectionTypeReset,
};

/** 
 * @type keytype
 * @brief  Video frame information
 */
struct VideoFrameInfo {
    /** 
     * @brief Width (pixels)
     */
    int width = 0;
    /** 
     * @brief High (pixels)
     */
    int height = 0;
    /** 
     * @brief Video frame clockwise rotation angle. See VideoRotation{@link #VideoRotation}.
     */
    VideoRotation rotation = VideoRotation::kVideoRotation0;
};

/** 
 * @type keytype
 * @brief  Video preprocessing configuration parameters. <br>
 */
struct VideoPreprocessorConfig {
    /** 
     * @brief Video frame pixel format. See VideoPixelFormat{@link #VideoPixelFormat}
     *         Only `kVideoPixelFormatI420` and `kVideoPixelFormatUnknown` are supported.
     */
    VideoPixelFormat required_pixel_format = kVideoPixelFormatUnknown;
};

/** 
 * @type keytype
 * @brief  Custom coded stream information
 */
struct VideoRateInfo {
    /** 
     * @brief Frame rate, units fps
     */
    int fps = 0;
    /** 
     * @brief Bit rate, unit kbps
     */
    int bitrate_kbps = 0;
    /**  
     * @brief Minimum video encoding bitrate in kbps. The encoding bitrate will not be lower than the `min_bitrate_kbps`.<br>
     *        It defaults to `0`. <br>
     *        It ranges within [0, bitrate_kbps). When `bitrate_kbps` < `min_bitrate_kbps`, the bitrate is self-adpapted.<br>
     *        In the following circumstance, the assignment to this variable has no effect:<br>
     *        + When `bitrate_kbps` = `0`, the video encoding is disabled.<br>
     *        + When `bitrate_kbps` < `0`, the bitrate is self-adapted.
     */
    int min_bitrate_kbps = 0;
};

/** 
 * @type keytype
 * @brief  Video decoding method
 */
enum VideoDecoderConfig {
    /** 
     * @brief Enable SDK internal decoding, only callback the decoded data
     */
    kVideoDecoderConfigRaw,
    /** 
     * @brief Turn on custom decoding, only callback the data before decoding
     */
    kVideoDecoderConfigEncode,
    /** 
     * @brief Enable SDK internal decoding, and call back the data before and after decoding at the same time
     */
    kVideoDecoderConfigBoth,
};

/** 
 * @type keytype
 * @brief  Video capture configuration parameters. <br>
 */
struct VideoCaptureConfig {
   /** 
    * @type keytype
    * @brief Video capture preference
    */
    enum CapturePreference {
       /** 
        * @brief (Default) Video capture preference: auto <br>
        *        SDK determines the best video capture parameters referring to the camera output parameters and the encoder configuration.
        */
        kAuto = 0,
       /** 
        * @brief Video capture preference: manual <br>
        *        Set the resolution and the frame rate manually.
        */
        kManual = 1,
       /** 
        * @brief Video capture preference: encoder configuration <br>
        *        The capture parameters are the same with the parameters set in setVideoEncoderConfig{@link #IRTCVideo#setVideoEncoderConfig}.
        */
        kAutoPerformance = 2,
    };
    /** 
     * @brief Video capture preference. See [CapturePreference](#capturepreference-1).
     */
    CapturePreference capture_preference = CapturePreference::kAuto;

    /** 
     * @brief The width of video capture resolution in px.
     */
    int width = 0;
    /** 
     * @brief The height of video capture resolution in px.
     */
    int height = 0;
    /** 
     * @brief Video capture frame rate in fps.
    */
    int frame_rate = 0;
};

/** 
 * @type keytype
 * @brief Locally recorded media type
 */
enum RecordingType {
    /** 
     * @brief Audio only
     */
    kRecordingTypeAudioOnly = 0,
    /** 
     * @brief Video only
     */
    kRecordingTypeVideoOnly = 1,
    /** 
     * @brief Record audio and video simultaneously
     */
    kRecordingTypeVideoAndAudio = 2,
};

/** 
 * @type keytype
 * @brief  Current audio device type
 */
enum RTCVideoDeviceType {
    /** 
     * @brief Unknown device
     */
    kRTCVideoDeviceTypeUnknown = -1,
    /** 
     * @brief Video rendering device
     */
    kRTCVideoDeviceTypeRenderDevice = 0,
    /** 
     * @brief Video capture device
     */
    kRTCVideoDeviceTypeCaptureDevice = 1,
    /** 
     * @brief Screen recorder
     */
    kRTCVideoDeviceTypeScreenCaptureDevice = 2,
};

/** 
 * @hidden currently not available
 * @type errorcode
 * @brief State and errors for publishing or subscribing public streams
 */
enum PublicStreamErrorCode {
    /** 
     * @brief Published or subscribed successfully.
     */
    kPublicStreamErrorCodeOK = 0,
    /** 
     * @brief Invalid parameter(s). Please revise the paramter(s) and retry.
     */
    kPublicStreamErrorCodePushInvalidParam = 1191,
    /** 
     * @brief Error for the task at the server side. The server will retry upon the failure.
     */
    kPublicStreamErrorCodePushInvalidStatus = 1192,
    /** 
     * @brief Unrecoverable error of publishing the public stream. Please start the task again.
     */
    kPublicStreamErrorCodePushInternalError = 1193,
    /** 
     * @brief Failed to publish. The SDK will retry upon the failure. We recommend to keep listening to the publishing result.
     */
    kPublicStreamErrorCodePushFailed = 1195,
    /** 
     * @brief Failed to publish the public stream for time-out error. The SDK will retry 10 s after the timeout. The maximum number of retry attempts is 3.
     */
    kPublicStreamErrorCodePushTimeout = 1196,
    /** 
     * @brief Failed to play a public stream because the publisher has not started publishing.
     */
    kPublicStreamErrorCodePullNoPushStream = 1300,
};

/** 
 * @type keytype
 * @brief Video orientation
 */
enum VideoRotationMode {
    /** 
     * @brief The orientation of the App
     */
    kVideoRotationModeFollowApp = 0,
    /** 
     * @brief Gravity
     */
    kVideoRotationModeFollowGSensor = 1,
};

/** 
 * @type keytype
 * @brief Media stream type
 */
enum MediaStreamType {
    /** 
     * @brief Controls audio only
     */
    kMediaStreamTypeAudio = 1 << 0,
    /** 
     * @brief Controls video only
     */
    kMediaStreamTypeVideo = 1 << 1,
    /** 
     * @brief Controls both audio and video
     */
    kMediaStreamTypeBoth = kMediaStreamTypeAudio | kMediaStreamTypeVideo
};
/** 
 * @type keytype
 * @brief Ratio to the original video frame rate of the publisher
 */
enum FrameRateRatio {
  /** 
   * @brief 100%
   */
  kFrameRateRatioOriginal = 0,
  /** 
   * @brief 50%
   */
  kFrameRateRatioHalf = 1,
  /** 
   * @brief 25%
   */
  kFrameRateRatioQuarter = 2,
};
/** 
 * @type keytype
 * @brief Information on remote video frame
 */
struct RemoteVideoConfig {
    /** 
     * @brief Expected maximum frame rate of the subscribed stream in px. The default value is 0, which represents full-frame-rate, values greater than 0 are valid.  <br>
     *        If the frame rate of the stream published is lower than the value you set, or if your subscribed stream falls back under limited network conditions, the frame rate you set will drop accordingly.  <br>
     *        Only valid if the stream is coded with SVC technique.
     */
    int framerate = 0;
    /** 
     * @brief Width of the video frame in px
     */
    int resolution_width = 0;
    /** 
     * @brief Height of the video frame in px
     */
    int resolution_height = 0;
};
/** 
 * @type keytype
 * @brief Room configuration. Room is a abstract concept for an RTC call. Users can interactive with each other in the same room.
 */
struct RTCRoomConfig {
    /** 
     * @brief Room profile. See RoomProfileType{@link #RoomProfileType}. The default is `kRoomProfileTypeCommunication`. The setting cannot be changed after joining the room.
     */
    RoomProfileType room_profile_type = kRoomProfileTypeCommunication;
    /** 
    * @brief Whether to publish media streams automatically. The default is automatic publishing.  <br>
     *       Only one of the rooms the user joined can be set to auto-publish. If no settings are made in each room, the stream is automatically published in the first room joined by default.<br>
    *        If you call setUserVisibility{@link #IRTCRoom#setUserVisibility} to set your own visibility to false, you will not publish media streams regardless of the value of `is_auto_publish`.
    */
    bool is_auto_publish = true;
    /** 
     * @brief Whether to automatically subscribe to the audio stream. The default is automatic subscription.<br>
     *        This setting affects both the main stream and the screen-sharing stream.
     */
    bool is_auto_subscribe_audio = true;
    /** 
     * @brief Whether to automatically subscribe to the video stream. The default is automatic subscription.<br>
     *        This setting affects both the main stream and the screen-sharing stream.
     */
    bool is_auto_subscribe_video = true;
    /** 
     * @brief Expected configuration of remote video stream, see RemoteVideoConfig{@link #RemoteVideoConfig}.
     */
    RemoteVideoConfig remote_video_config;
};

/** 
 * @type keytype
 * @brief  The media type for screen capture
 */
enum ScreenMediaType {
    /** 
     * @brief Capture video only
     */
    kScreenMediaTypeVideoOnly = 0,
    /** 
     * @brief Capture audio only
     */
    kScreenMediaTypeAudioOnly = 1,
    /** 
     * @brief Capture Audio and video simultaneously.
     */
    kScreenMediaTypeVideoAndAudio = 2,
};

/** 
 * @type keytype
 * @brief Basic beauty effect.
 */
enum EffectBeautyMode {
    /** 
     * @brief Brightening.
     */ 
    kEffectBeautyModeWhite = 0,
    /** 
     * @brief Smoothing.
     */
    kEffectBeautyModeSmooth = 1,
    /** 
     * @brief Sharpening.
     */ 
    kEffectBeautyModeSharpen = 2,
    /** 
     * @valid since 3.55
     * @brief Clarity. Integrating Effects SDK v4.4.2+ is required for this sub-item.
     */ 
    kEffectBeautyModeClear = 3,
};

/**  
 * @type keytype
 * @brief  video device facing type
 */
enum VideoDeviceFacing {
    /**  
     * @brief Front-facing video camera
     */
    kVideoDeviceFacingFront = 0,
    /**  
     * @brief Back-facing video camera
     */
    kVideoDeviceFacingBack = 1,
    /**  
     * @brief video camera facing unknown
     */
    kVideoDeviceFacingUnknown = 2,
};

/** 
 * @type keytype
 * @brief Video device information
 */
struct VideoDeviceInfo {
    /** 
     * @brief Device ID
     */
    char device_id[MAX_DEVICE_ID_LENGTH];
    /** 
     * @brief Device alias
     */
    char device_name[MAX_DEVICE_ID_LENGTH];
    /** 
     * @brief ID of the device vendor
     */
    int64_t device_vid;
    /** 
     * @brief Product number of the device
     */
    int64_t device_pid;
    /** 
     * @brief Connection type of the device
     */
    DeviceTransportType transport_type;
    /**  
     * @brief  video device facing type
     */
    VideoDeviceFacing device_facing;
    /**
     * @hidden constructor/destructon
     */
    VideoDeviceInfo() {
        memset(device_id, 0, MAX_DEVICE_ID_LENGTH);
        memset(device_name, 0, MAX_DEVICE_ID_LENGTH);
        this->device_vid = 0;
        this->device_pid = 0;
        this->transport_type = DeviceTransportType::kDeviceTransportTypeUnknown;
        this->device_facing = VideoDeviceFacing::kVideoDeviceFacingFront;
    };

    /**
     * @hidden constructor/destructon
     */
    VideoDeviceInfo& operator=(const VideoDeviceInfo& src) {
        if (this != &src) {
            strncpy(device_id, src.device_id, MAX_DEVICE_ID_LENGTH - 1);
            strncpy(device_name, src.device_name, MAX_DEVICE_ID_LENGTH - 1);
            device_id[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_name[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_vid = src.device_vid;
            device_pid = src.device_pid;
            transport_type = src.transport_type;
            device_facing = src.device_facing;
        }

        return *this;
    }
};

/** 
 * @type keytype
 * @brief Orientation of the video frame.
 */
enum VideoOrientation {
    /** 
     * @brief (Default) The original orientation of the captured video frame.
     */
    kVideoOrientationAdaptive = 0,
    /** 
     * @brief Portrait mode. Adjust the captured video frame to vertical layout in RTC SDK.
     */
    kVideoOrientationPortrait = 1,
    /** 
     * @brief Landscape mode. Adjust the captured video frame to horizontal layout in RTC SDK.
     */
    kVideoOrientationLandscape = 2
};

/** 
 * @hidden not available
 * @type keytype
 * @brief The reason for the change in super resolution mode.
 */
enum VideoSuperResolutionModeChangedReason {
    /** 
     * @brief Successfully turned off the super resolution mode by calling setRemoteVideoSuperResolution{@link #IRTCVideo#setRemoteVideoSuperResolution}.
     */
    kVideoSuperResolutionModeChangedReasonAPIOff = 0,
    /** 
     * @brief Successfully turned on the super resolution mode by calling setRemoteVideoSuperResolution{@link #IRTCVideo#setRemoteVideoSuperResolution}.
     */
    kVideoSuperResolutionModeChangedReasonAPIOn = 1,
    /** 
     * @brief Failed to turn on super-resolution mode. The original resolution of the remote video stream should not exceed 640 × 360 pixels.
     */
    kVideoSuperResolutionModeChangedReasonResolutionExceed = 2,
    /** 
     * @brief Failed to turn on super-resolution mode. You can only turn on super-resolution mode for one stream.
     */
    kVideoSuperResolutionModeChangedReasonOverUse = 3,
    /** 
     * @brief Incompatible device for super resolution.
     */
    kVideoSuperResolutionModeChangedReasonDeviceNotSupport = 4,
    /** 
     * @brief The super-resolution mode is turned off because of lacking device capabilities.
     */
    kVideoSuperResolutionModeChangedReasonDynamicClose = 5,
    /** 
     * @brief The super-resolution mode is turned off for other reasons.
     */
    kVideoSuperResolutionModeChangedReasonOtherSettingDisabled = 6,
    /** 
     * @brief The super-resolution mode is turned on for other reasons.
     */
    kVideoSuperResolutionModeChangedReasonOtherSettingEnabled = 7,
    /** 
     * @brief The super-resolution mode is not compiled in the SDK.
     */
    kVideoSuperResolutionModeChangedReasonNoComponent = 8,
    /** 
     * @brief The remote stream does not exist. Reasons include invalid room ID, user ID, or the stream is not published.
     */
    kVideoSuperResolutionModeChangedReasonStreamNotExist = 9,
};

/** 
 * @hidden not available
 * @type keytype
 * @brief The reasons for the change in the video noise reduction mode.
 */
enum VideoDenoiseModeChangedReason {
    /** 
     * @brief Video noise reduction mode changed due to unknown reasons.
     */
    kVideoDenoiseModeChangedReasonNull = -1,
    /** 
     * @brief Successfully turned off the video noise reduction by calling setVideoDenoiser{@link #IRTCVideo#setVideoDenoiser}.
     */
    kVideoDenoiseModeChangedReasonApiOff = 0,
    /** 
     * @brief Successfully turned on the video noise reduction by calling setVideoDenoiser{@link #IRTCVideo#setVideoDenoiser}.
     */
    kVideoDenoiseModeChangedReasonApiOn = 1,
    /** 
     * @brief Video noise reduction disabled by configuration.
     */
    kVideoDenoiseModeChangedReasonConfigDisabled = 2,
    /** 
     * @brief Video noise reduction enabled by configuration.
     */
    kVideoDenoiseModeChangedReasonConfigEnabled = 3,
    /** 
     * @brief Video noise reduction turned off due to internal exceptions. 
     */
    kVideoDenoiseModeChangedReasonInternalException = 4,
    /** 
     * @brief Video noise reduction turned off due to hardware performance stress. 
     */
    kVideoDenoiseModeChangedReasonDynamicClose = 5,
    /** 
     * @brief Device hardware capacity is sufficient, video noise reduction turned on.
     */
    kVideoDenoiseModeChangedReasonDynamicOpen = 6,
    /** 
     * @brief Video noise reduction mode changed due to video resolution. High resolution leads to high hardware utilization. Reduce the resolution to recover video noise reduction.
     */
    kVideoDenoiseModeChangedReasonResolution = 7,
};

/** 
 * @type keytype
 * @brief Type of the screen capture object
 */
enum ScreenCaptureSourceType {
    /** 
     * @brief Type unknown
     */
    kScreenCaptureSourceTypeUnknown,

    /** 
     * @brief Application window
     */
    kScreenCaptureSourceTypeWindow,

    /** 
     * @brief Desktop
     */
    kScreenCaptureSourceTypeScreen
};

/** 
 * @type keytype
 * @brief Detailed information of the screen sharing object
 */
struct ScreenCaptureSourceInfo {
    /** 
     * @brief Type of the screen-sharing object. Refer to ScreenCaptureSourceType{@link #ScreenCaptureSourceType} for the type of shared object.
     */
    ScreenCaptureSourceType type = kScreenCaptureSourceTypeUnknown;

    /** 
     * @brief ID of the screen-sharing object
     */
    view_t source_id = nullptr;
    /** 
     * @brief Name of the screen-sharing object<br>
     *        Make sure you haved save them as `string` objects before calling release{@link #IScreenCaptureSourceList#release} to release the dynamic memory.
     */
    const char* source_name = nullptr;
    /** 
     * @brief The title of the application to be shared.<br>
     *        Only available if the sharing object is an application windows.<br>
     *        Make sure you haved save them as `string` objects before calling release{@link #IScreenCaptureSourceList#release} to release the dynamic memory.
     */
    const char* application = nullptr;
    /** 
     * @brief Process pid of the application to be shared.<br>
     *        Only available if the sharing object is an application windows.<br>
     */
    int pid = 0;
    /** 
     * @brief Tag for the screen to be shared identifying whether it is the primary screen<br>
     *        Only available when the screen-sharing object is a screen. <br>
     */
    bool primary_monitor = false;
    /** 
     * @brief Coordinates of the screen-sharing object. See Rectangle{@link #Rectangle}.<br>
     *        Valid only when the capture source is the monitor screen. <br>
      *       + When there are several screens, the origin (0, 0) is different for different platforms: <br>
      *         - For Windows, the origin (0, 0) is the top-left corner of the main screen. <br>
      *         - For Linux, the origin (0, 0) is the top-left corner of the rectangle that merely covers all screens. <br>
      *       + The region of the window is different for different platforms:
      *         - For Windows and macOS, the region includes the system title bar of the window. <br>
      *         - For Linux, the region does not includes the system title bar of the window. 
     */
    Rectangle region_rect;
};

/** 
 * @type api
 * @region  screen sharing
 * @brief  Information list of the screen-sharing objects
 * Calling release{@link #IScreenCaptureSourceList#release} will release all the objects related to this class.
 */
class IScreenCaptureSourceList {
public:
   /**
    * @hidden constructor/destructor
    */
    virtual ~IScreenCaptureSourceList() {
    }
    /** 
     * @type api
     * @region Screen sharing
     * @brief Get the length of the list
     * @return Number of the items in the list
     */
    virtual int32_t getCount() = 0;

    /** 
     * @type api
     * @region Screen share
     * @brief According to the index, get the elements in the screen share list
     * @param [in] index Index number
     * @return Screen source item. Refer to ScreenCaptureSourceInfo{@link #ScreenCaptureSourceInfo} for more details.
     */
    virtual ScreenCaptureSourceInfo getSourceInfo(int32_t index) = 0;

    /** 
     * @type api
     * @region Screen sharing
     * @brief If you are finished with the IScreenCaptureSourceList{@link #IScreenCaptureSourceList}, call this API to destruct it and release the memory.
     */
    virtual void release() = 0;
};

/** 
 * @type callback
 * @brief  Custom coded frame callback class
 * Note: Callback functions are thrown synchronously in a non-UI thread within the SDK. Therefore, you must not perform any time-consuming operations or direct UI operations within the callback function, as this may cause the app to crash.
 */
class IExternalVideoEncoderEventHandler {
public:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IExternalVideoEncoderEventHandler(){}
    /** 
     * @type callback
     * @region Video management
     * @brief Prompt custom encoded frames to start pushing callbacks.   <br>
     *         After receiving this callback, you can call pushExternalEncodedVideoFrame{@link #IRTCVideo#pushExternalEncodedVideoFrame} to push a custom encoded video frame to the SDK
     * @param  [in] index The properties of the encoded stream that can be pushed. See StreamIndex{@link #StreamIndex}
     */
    virtual void onStart(StreamIndex index) = 0;
    /** 
     * @type callback
     * @region Video management
     * @brief When you receive the callback, you need to stop pushing custom encoded video frames to the SDK
     * @param  [in] index The properties of the encoded stream that you need to stop pushing. See StreamIndex{@link #StreamIndex}
     */
    virtual void onStop(StreamIndex index) = 0;
    /** 
     * @type callback
     * @region Video management
     * @brief When the frame rate or bit rate of a custom encoded stream changes, the callback is triggered
     * @param  [in] index The Properties of the encoded stream with the changed index. See StreamIndex{@link #StreamIndex}
     * @param  [in] video_index The subscript of the corresponding encoded stream
     * @param  [in] info The encoded stream information after the change. See VideoRateInfo{@link #VideoRateInfo}
     */
    virtual void onRateUpdate(StreamIndex index, int32_t video_index, VideoRateInfo info) = 0;
    /** 
     * @type callback
     * @region Video management
     * @brief This callback is used to inform the stream publisher that a keyframe needs to be regenerated.
     * @param [in] index Properties of the remote encoded stream. See StreamIndex{@link #StreamIndex}
     * @param [in] video_index The subscript of the corresponding encoded stream
     */
    virtual void onRequestKeyFrame(StreamIndex index, int32_t video_index) = 0;
    /** 
     * @valid since 3.56
     * @type callback
     * @brief As the sender of a external encoded video stream, you will receive this callback at certain moments. 
     *        Based on the hints of this callback, you can selectively encode the video stream that is ready for transmission, in order to reduce the performance impact of video encoding on the local device. This callback is triggered based on a combination of factors, including  the performance of the local device, the local network, and whether the stream is subscribed by the remote user.
     * @param index See StreamIndex{@link #StreamIndex}.
     * @param video_index Subscript of the corresponding encoded stream.
     * @param active The active state of the corresponding encoded stream.
     * @notes To receive the callback, call setVideoSourceType{@link #IRTCVideo#setVideoSourceType} to set the input video source to custom encoded video and call setExternalVideoEncoderEventHandler{@link #IRTCVideo#setExternalVideoEncoderEventHandler} to set the callback handler.
     */
    virtual void onActiveVideoLayer(StreamIndex index, int32_t video_index, bool active) = 0;
};

/** 
 * @type callback
 * @region Video Data Callback
 * @brief Local video frame observer
 * Note: Callback functions are thrown synchronously in a non-UI thread within the SDK. Therefore, you must not perform any time-consuming operations or direct UI operations within the callback function, as this may cause the app to crash.
 */
class ILocalEncodedVideoFrameObserver {
public:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~ILocalEncodedVideoFrameObserver() {
    }
    /** 
     * @type callback
     * @region Video Data Callback
     * @brief After calling registerLocalEncodedVideoFrameObserver{@link #IRTCVideo#registerLocalEncodedVideoFrameObserver}, SDK will trigger this callback when it receives local video frames.
     * @param [in] type Type of the local video frame. See StreamIndex{@link #StreamIndex}.
     * @param [in] video_stream Information about the local video frame. See IEncodedVideoFrame{@link #IEncodedVideoFrame}.
     */
    virtual void onLocalEncodedVideoFrame(StreamIndex type, const IEncodedVideoFrame& video_stream) = 0;
};

/** 
 * @type callback
 * @region  video management
 * @brief  Remote encoded video data monitor
 * Note: Callback functions are thrown synchronously in a non-UI thread within the SDK. Therefore, you must not perform any time-consuming operations or direct UI operations within the callback function, as this may cause the app to crash.
 */
class IRemoteEncodedVideoFrameObserver {
public:
    /** 
     * @hidden constructor/destructor
     * @brief  Destructor
     */
    virtual ~IRemoteEncodedVideoFrameObserver() {
    }
    /** 
     * @type callback
     * @region  video data callback
     * @brief  Call registerRemoteEncodedVideoFrameObserver{@link #IRTCVideo#registerRemoteEncodedVideoFrameObserver}, the callback is triggered when the SDK detects the remote encoded video data
     * @param  [in] stream_info The received remote stream information. See RemoteStreamKey{@link #RemoteStreamKey}
     * @param  [in] video_stream The received remote video frame information. See IEncodedVideoFrame{@link #IEncodedVideoFrame}
     */
    virtual void onRemoteEncodedVideoFrame(const RemoteStreamKey& stream_info, const IEncodedVideoFrame& video_stream) = 0;
};

/** 
 * @type keytype
 * @brief The metadata information contained in the video frame
 */
struct VideoMetadataBuffer {
    /** 
     * @brief Received or sent metadata
     */
    char* data;
    /** 
     * @brief The size of metadata data received or sent cannot exceed 1024
     */
    int size;
    /** 
     * @brief A timestamp containing metadata video frames in microseconds
     */
    int64_t timestamp_us;
};

/** 
 * @deprecated since 3.45 and will be deleted in 3.51, use [sendSEIMessage](70098#IRTCVideo-sendseimessage-2) instead.
 * @type callback
 * @region video data callback
 * @brief Metadata observer, you can receive metadata in the media stream, or add metadata to the media stream
 * Note: Callback functions are thrown synchronously in a non-UI thread within the SDK. Therefore, you must not perform any time-consuming operations or direct UI operations within the callback function, as this may cause the app to crash.
 */
class IMetadataObserver {
public:
    /**
     * @hidden constructor/destructor
     */
    virtual ~IMetadataObserver() {
    }

    /** 
     * @type callback
     * @region Video data callback
     * @brief When the SDK is ready to send a video frame, the event is called back to determine whether metadata needs to be added to the video frame.
     * @param  [in/out] Metadata The data to be sent, copy the data into the data field, and set the size to the real size.
     * @return   <br>
     *         + true: You need to add metadata <br>
     *         + false: You don't need to add metadata <br>
     * @notes   <br>
     *         + The size of metadata cannot exceed 1024 bytes <br>
     *         + The timestampUs in metadata is the input field, which represents the timestamp of the video frame. It is used for synchronization and does not need to be modified .. <br>
     *         + There must be no time-consuming operations in the callback to avoid affecting the video card <br>
     */
    virtual bool onReadyToSendMetadata(VideoMetadataBuffer* metadata) = 0;

    /** 
     * @type callback
     * @region Video data callback
     * @brief When a video frame is received by the SDK and contains a medatada, the event is called back.
     * @param  [in] roomid The room ID to which the current frame belongs.
     * @param  [in] uid The user ID to which the current frame belongs.
     * @param  [in] metadata The metadata information contained in the video frame. See VideoMetadataBuffer{@link #VideoMetadataBuffer}.
     * @notes There must be no time-consuming operations in the callback so as not to affect the video card.
     */
    virtual void onMetadataReceived(const char* roomid, const char* uid, const VideoMetadataBuffer& metadata) = 0;
};

/** 
 * @hidden
 * @deprecated since 3.50 and will be deleted in 3.55.
 * @type callback
 * @region Video Management
 * @brief Video data callback observer
 */
class IVideoFrameObserver {
public:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IVideoFrameObserver() = default;

    /** 
     * @type callback
     * @region Video Management
     * @brief Get the successfully captured local screen video frames for custom processing or rendering.
     * @param [in] video_frame Video data. See IVideoFrame{@link #IVideoFrame}.
     */
    virtual bool onLocalScreenFrame(IVideoFrame* video_frame) = 0;

    /** 
     * @type callback
     * @region Video Management
     * @brief Get the successfully captured local camera stream for custom processing or rendering.
     * @param [in] video_frame Video data. See IVideoFrame{@link #IVideoFrame}.
     */
    virtual bool onLocalVideoFrame(IVideoFrame* video_frame) = 0;

    /** 
     * @type callback
     * @region Video Management
     * @brief Get the successfully captured remote screen video frames for custom processing or rendering.
     * @param [in] roomid The ID of the room from which the video is streamed.
     * @param [in] uid The ID of the remote user who published the stream.
     * @param [in] video_frame Video data. See IVideoFrame{@link #IVideoFrame}.
     */
    virtual bool onRemoteScreenFrame(const char* roomid, const char* uid, IVideoFrame* video_frame) = 0;

    /** 
     * @type callback
     * @region Video Management
     * @brief Get the successfully captured remote camera stream for custom processing or rendering.
     * @param [in] roomid The ID of the room from which the video is streamed.
     * @param [in] uid The ID of the remote user who published the stream.
     * @param [in] video_frame Video data. See IVideoFrame{@link #IVideoFrame}.
     * @notes The pixelFormat of the received video frame depends on your platform (macOS, Windows, Linux).
     */
    virtual bool onRemoteVideoFrame(const char* roomid, const char* uid, IVideoFrame* video_frame) = 0;

    /** 
     * @type callback
     * @region video management
     * @brief Callback carrying splicing video data
     * @param [in] roomid The ID of the room from which the video is streamed.
     * @param [in] uid The ID of the remote user who published the stream.
     * @param [in] video_frame Video data. See IVideoFrame{@link #IVideoFrame}.
     */
    virtual bool onMergeFrame(const char* roomid, const char* uid, IVideoFrame* video_frame) {
        return false;
    }
};
/** 
 * @hidden(Linux)
 * @type callback
 * @brief The callback of taking snapshots.
 * Note: Callback functions are thrown synchronously in a non-UI thread within the SDK. Therefore, you must not perform any time-consuming operations or direct UI operations within the callback function, as this may cause the app to crash.
 */
class ISnapshotResultCallback {
public:
    /**
     * @hidden constructor/destructor
     */
    virtual ~ISnapshotResultCallback() = default;
    /** 
     * @type callback
     * @brief Receives the callback after calling takeLocalSnapshot{@link #IRTCVideo#takeLocalSnapshot}.
     * @param [in] task_id The index for the snapshot, the same as the return value of takeLocalSnapshot{@link #IRTCVideo#takeLocalSnapshot}.
     * @param [in] stream_index See StreamIndex{@link #StreamIndex}.
     * @param [in] image The snapshot image. If the snapshot task fails, the value is `null`.
     * @param [in] error_code Error code: <br>
     *        + 0: Success. <br>
     *        + -1: Failure. Fails to generate the image. <br>
     *        + -2: Failure. The stream is invalid.
     *        + -3: Failure. snapshot timeout, default 1s.
     */
    virtual void onTakeLocalSnapshotResult(long task_id, StreamIndex stream_index, IVideoFrame* image, int error_code) = 0;
    /** 
     * @type callback
     * @brief Receives the callback after calling takeRemoteSnapshot{@link #IRTCVideo#takeRemoteSnapshot}.
     * @param [in] task_id The index for the remote snapshot, the same as the return value of takeRemoteSnapshot{@link #IRTCVideo#takeRemoteSnapshot}.
     * @param [in] stream_key See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param [in] image The snapshot image. If the snapshot task fails, the value is `null`.
     * @param [in] error_code Error code: <br>
     *        + 0: Success. <br>
     *        + -1: Failure. Fails to generate the image. <br>
     *        + -2: Failure. The stream is invalid.
     *        + -3: Failure. snapshot timeout, default 1s.
     */
    virtual void onTakeRemoteSnapshotResult(long task_id, RemoteStreamKey stream_key, IVideoFrame* image, int error_code) = 0;
};
/** 
 * @hidden(macOS, Windows, Linux)
 * @type keytype
 * @brief Media type for cellular assisted Enhancement
 */
struct MediaTypeEnhancementConfig {
    /** 
     * @brief Apply to signaling or not. Not by default.
     */
    bool enhance_signaling = false;
    /** 
     * @brief Apply to audio stream (Screen-sharing audio not included) or not. Not by default.
     */
    bool enhance_audio = false;
    /** 
     * @brief Apply to screen sharing audio or not. Not by default.
     */
    bool enhance_screen_audio = false;
    /** 
     * @brief Apply to video (Screen-sharing video not included) or not. Not by default.
     */
    bool enhance_video = false;
    /** 
     * @brief Apply to screen sharing video or not. Not by default.
     */
    bool enhance_screen_video = false;
};

}  // namespace bytertc
