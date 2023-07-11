/*
 * Copyright (c) 2020 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Video Frame
 */

#pragma once

#ifndef BYTE_RTC_VIDEO_FRAME_H__
#define BYTE_RTC_VIDEO_FRAME_H__

#include <stdint.h>
#include <stddef.h>
#include <memory>
#include "bytertc_media_defines.h"
#ifdef BYTERTC_ANDROID
#include <jni.h>
#endif

namespace bytertc {

/** 
 * @type keytype
 * @brief Video compression picture type
 */
enum VideoPictureType {
    /** 
     * @brief Unknown type
     */
    kVideoPictureTypeUnknown = 0,
    /** 
     * @brief I-frames, key frames which are coded without reference to any other frame.
     */
    kVideoPictureTypeI,
    /** 
     * @brief P-frames, forward-predicted frames which are coded by a forward predictive coding method.
     */
    kVideoPictureTypeP,
    /** 
     * @brief  B-frames, bi-directionally predicted frames which are coded by both forward and backward predictive coding method.
     */
    kVideoPictureTypeB
};

/** 
 * @type keytype
 * @brief Video frame rotation
 */
enum VideoRotation {
    /** 
     * @brief Not to rotate.
    */
    kVideoRotation0 = 0,
    /** 
     * @brief Rotate 90 degrees clockwise. 
    */
    kVideoRotation90 = 90,
    /** 
     * @brief Rotate 180 degrees clockwise. 
    */
    kVideoRotation180 = 180,
    /** 
     * @brief Rotate 270 degrees clockwise. 
    */
    kVideoRotation270 = 270
};

/** 
 * @type keytype
 * @brief  Video frame scale mode
 */
enum VideoStreamScaleMode {
    /** 
     * @brief Auto mode, default to FitWithCropping.
     */
    kVideoStreamScaleModeAuto = 0,
    /** 
     * @brief Stretch the video frame until the video frame and the window have the same resolution. The video frame's aspect ratio can be changed as it is automatically stretched to fill the window, but the whole image is visible.
     */
    kVideoStreamScaleModeStretch = 1,
    /** 
     * @brief  Fit the window with cropping <br>
     *         Scale the video frame uniformly until the window is filled. If the video frame's aspect ratio is different from that of the window, the extra part of the video frame will be cropped. <br>
     *         After the scaling process is completed, the width or height of the video frame will be consistent with that of the window, and the other dimension will be greater than or equal to that of the window.
     */
    kVideoStreamScaleModeFitWithCropping = 2,
    /** 
     * @brief  Fit the window with filling <br>
     *         Scale the video frame uniformly until its width or height reaches the boundary of the window. If the video frame's aspect ratio is different from that of the window, the area that is not filled will be black. <br>
     *         After the scaling process is completed, the width or height of the video frame will be consistent with that of the window, and the other dimension will be less than or equal to that of the window.
     */
    kVideoStreamScaleModeFitWithFilling = 3,
};


/** 
 * @type keytype
 * @brief  Video encoding mode
 */
enum VideoCodecMode {
    /** 
     * @brief Automatic selection
     */
    kCodecModeAuto = 0,
    /** 
     * @brief Hardcoding
     */
    kCodecModeHardware,
    /** 
     * @brief Softcoding
     */
    kCodecModeSoftware,
};

/** 
 * @type keytype
 * @brief  Encoder preference.
 */
enum VideoEncodePreference {
    /** 
     * @brief No preference. The frame rate and the resolution will not be adjusted.
     */
    kVideoEncodePreferenceDisabled = 0,
    /** 
     * @brief (Default) Frame rate first.
     */
    kVideoEncodePreferenceFramerate,
    /** 
     * @brief Resolution first.
     */
    kVideoEncodePreferenceQuality,
    /** 
     * @brief Balancing resolution and frame rate.
     */
    kVideoEncodePreferenceBalance,
};

/** 
 * @type keytype
 * @brief  camera.
 */
enum CameraID {
    /** 
     *@brief Front-facing camera for mobile, build-in camera for PC
     */
    kCameraIDFront = 0,
    /** 
     *@brief Postconditioning camera for mobile, PC is undefined for camera 1
     */
    kCameraIDBack = 1,
    /** 
     *@brief External camera
     */
    kCameraIDExternal = 2,
    /** 
     *@brief Invalid value
     */
    kCameraIDInvalid = 3
};


#define SEND_KBPS_AUTO_CALCULATE -1
#define SEND_KBPS_DISABLE_VIDEO_SEND 0

/** 
 * @hidden used in streaming only
 * @deprecated since 3.45 along with setVideoEncoderConfig(StreamIndex index, const VideoSolution* solutions, int solution_num) = 0;
 * @type keytype
 * @brief Video stream parameters
 */
struct VideoSolution {
    /** 
     * @brief Width (pixels)
     */
    int width;
    /** 
     * @brief Height (pixels)
     */
    int height;
    /** 
     * @brief Video frame rate
     */
    int fps;
    /** 
     * @brief Set maximum bitrate in kbps <br>
     *        We recommend the default setting: `SEND_KBPS_AUTO_CALCULATE(-1)`
     *        `SEND_KBPS_DISABLE_VIDEO_SEND(0)` for no limitation for the sending bitrate
     */
    int max_send_kbps = SEND_KBPS_AUTO_CALCULATE;
    int min_send_kbps = 0;
    /** 
     * @brief ncoder preference. See VideoEncodePreference{@link #VideoEncodePreference}.
     */
    VideoEncodePreference encode_preference = VideoEncodePreference::kVideoEncodePreferenceFramerate;
};

/** 
 * @hidden for internal use only on Windows and Android
 * @type keytype
 * @brief Information of video frames within the FoV (Field of View). <br>
 *        Tile is the unit of a video within Fov.<br>
 *        A video within Fov includes HD view and LD background each of which consists of multiple Tiles.<br>
 *        The information of the video frames within the Fov is set by calling `setVideoEncoderConfig(const VideoEncoderConfig& encoderConfig, const char* parameters)` on the sender side.
 */
struct FovVideoTileInfo {
    /** 
     * @brief Width of the HD view.
     */
    uint32_t hd_width = 0;
    /** 
     * @brief Height of the HD view
     */
    uint32_t hd_height = 0;
    /** 
     * @brief Width of the LD background
     */
    uint32_t ld_width = 0;
    /** 
     * @brief Height of the LD background
     */
    uint32_t ld_height = 0;
    /** 
     * @brief Width of a Tile
     */
    uint32_t tile_width = 0;
    /** 
     * @brief Height of a Tile
     */
    uint32_t tile_height = 0;
    /** 
     * @brief Number of Tile rows in the HD view
     */
    uint32_t hd_row = 0;
    /** 
     * @brief Number of Tile columns in the HD view
     */
    uint32_t hd_column = 0;
    /** 
     * @brief Number of Tile rows in the LD background
     */
    uint32_t ld_row = 0;
    /** 
     * @brief Number of Tile columns in the LD background
     */
    uint32_t ld_column = 0;
    /** 
     * @brief Number of tile rows within the FoV
     */
    uint32_t dest_row = 0;
    /** 
     * @brief Number of tile columns within the FoV
     */
    uint32_t dest_column = 0;
    /** 
     * @brief Position map of the Tiles
     */
    uint8_t* tile_map = nullptr;
    /** 
     * @brief Number of the Tiles
     */
    uint32_t tile_size = 0;
};
/** 
 * @type keytype
 * @brief  Video encoding configuration. Refer to [Setting Video Encoder Configuration](https://docs.byteplus.com/byteplus-rtc/docs/70122) to learn more.
 */
struct VideoEncoderConfig {
    /** 
     * @brief Width of the video frame in px
     */
    int width;
    /** 
     * @brief Height of the video frame in px
     */
    int height;
    /** 
     * @brief Video frame rate in fps
     */
    int frameRate;
    /** 
     * @brief Maximum bit rate in kbps. Optional for internal capturing while mandatory for custom capturing.  <br>
     *        The default value is -1 in internal capturing mode, SDK will automatically calculate the applicable bit rate based on the input resolution and frame rate.  <br>
     *        No stream will be encoded and published if you set this parameter to 0.
     */
    int maxBitrate = SEND_KBPS_AUTO_CALCULATE;
    /** 
     * @brief Minimum video encoding bitrate in kbps. The encoding bitrate will not be lower than the `minBitrate`.<br>
     *        It defaults to `0`. <br>
     *        It ranges within [0, maxBitrate). When `maxBitrate` < `minBitrate`, the bitrate is self-adpapted.<br>
     *         In the following circumstance, the assignment to this variable has no effect:<br>
     *        + When `maxBitrate` = `0`, the video encoding is disabled.<br>
     *        + When `maxBitrate` < `0`, the bitrate is self-adapted.
     */
    int minBitrate = 0;
    /** 
     * @brief Encoding preference. The default value is kVideoEncodePreferenceFramerate. See VideoEncodePreference{@link #VideoEncodePreference}.
     */
    VideoEncodePreference encoderPreference = VideoEncodePreference::kVideoEncodePreferenceFramerate;
};

/** 
 * @deprecated since 3.45 along with onUserUnPublishStream and onUserUnPublishScreen, and will be deleted in 3.51.
 * @type keytype
 * @brief Video attribute
 */
struct VideoSolutionDescription {
    /** 
     * @brief Width in pixels <br>
     *        The default value is `1920`<br>
     *        `0` for remaining the width of the source.
     */
    int width;
    /** 
     * @brief High in pixels <br>
     *        The default value is `1080`<br>
     *        `0` for remaining the height of the source.
     */
    int height;
    /** 
     * @brief Video frame rate in fps. The default value is 15 fps.
     */
    int fps;
    /** 
     * @brief Maximum transmission rate in Kbps<br>
     *        The default value is `-1`, Adaptive mode in which RTC will set the bitrate to a value which is calculated according to the target resolution and the frame rate.
     */
    int max_send_kbps;
    int min_send_kbps;
    /** 
     * @brief Zoom mode. See VideoStreamScaleMode{@link #VideoStreamScaleMode}
     */
    VideoStreamScaleMode scale_mode = VideoStreamScaleMode::kVideoStreamScaleModeAuto;
    /** 
     * @brief The encoding type of the video. See VideoCodecType{@link #VideoCodecType}
     */
    VideoCodecType codec_name = VideoCodecType::kVideoCodecTypeUnknown;
    /** 
     * @brief The encoding mode of the video. See VideoCodecMode{@link #VideoCodecMode}
     */
    VideoCodecMode codec_mode = VideoCodecMode::kCodecModeAuto;
    /** 
     * @brief Video coding quality preference strategy. See VideoEncodePreference{@link #VideoEncodePreference}
     */
    VideoEncodePreference encode_preference = VideoEncodePreference::kVideoEncodePreferenceFramerate;

    /**
     * @hidden constructor/destructor
     */
    bool operator==(const VideoSolutionDescription& config) const {
        bool result = width == config.width && height == config.height && fps == config.fps
                             && max_send_kbps == config.max_send_kbps && min_send_kbps == config.min_send_kbps
                             && scale_mode == config.scale_mode && codec_name == config.codec_name
                             && codec_mode == config.codec_mode && encode_preference == config.encode_preference;
        return result;
    }

    /**
     * @hidden constructor/destructor
     */
    bool operator!=(const VideoSolutionDescription& config) const {
        bool result = (*this == config);
        return !result;
    }
};


/** 
 * @deprecated since 3.45 and will be deleted in 3.51.
 * @type keytype
 * @region video management
 */
typedef VideoSolutionDescription VideoProfile;

/** 
 * @type keytype
 * @brief Encoding format for video frame color
 */
enum VideoPixelFormat {
    /** 
     * @brief Unknown format
     */
    kVideoPixelFormatUnknown = 0,
    /** 
     * @brief YUV I420
     */
    kVideoPixelFormatI420,
    /** 
     * @brief YUV NV12
     */
    kVideoPixelFormatNV12,
    /** 
     * @brief YUV NV21
     */
    kVideoPixelFormatNV21,
    /** 
     * @brief RGB 24bit
     */
    kVideoPixelFormatRGB24,
    /** 
     * @brief RGBA
     */
    kVideoPixelFormatRGBA,
    /** 
     * @brief ARGB
     */
    kVideoPixelFormatARGB,
    /** 
     * @brief BGRA
     */
    kVideoPixelFormatBGRA,
    /** 
     * @brief Texture2D
     */
    kVideoPixelFormatTexture2D = 0x0DE1,
    /** 
     * @brief TextureOES
     */
    kVideoPixelFormatTextureOES = 0x8D65,
};

/** 
 * @type keytype
 * @brief Video content type
 */
enum VideoContentType {
    /** 
     * @brief Normal video
     */
    kVideoContentTypeNormalFrame = 0,
    /** 
     * @brief Black frame video
     */
    kVideoContentTypeBlackFrame,
};

/** 
 * @type keytype
 * @region  video management
 * @brief  Video YUV format color space
 */
enum ColorSpace {
    /** 
     * Unknown color space, default kColorSpaceYCbCrBT601LimitedRange color space
     */
    kColorSpaceUnknown = 0,
    /** 
     * @brief BT.601 Digital Coding Standard, Color Space [16-235]
     */
    kColorSpaceYCbCrBT601LimitedRange,
    /** 
     *  @brief BT.601 Digital Coding Standard, Color Space [0-255]
     */
    kColorSpaceYCbCrBT601FullRange,
    /** 
     * @brief BT.7091 Digital Coding Standard, Color Space [16-235]
     */
    kColorSpaceYCbCrBT709LimitedRange,
    /** 
     * @brief BT.7091 Digital Coding Standard, Color Space [0-255]
     */
    kColorSpaceYCbCrBT709FullRange,
};

/** 
 * @type keytype
 * @brief Video frame format
 */
enum VideoFrameType {
    /** 
     * @brief Original format, stored as memory
     */
    kVideoFrameTypeRawMemory,
    /** 
     * @brief CVPixelBufferRef (for iOS and macOS)
     */
    kVideoFrameTypeCVPixelBuffer,
    /** 
     * @brief Open gl texture
     */
    kVideoFrameTypeGLTexture,
    /** 
     * @brief Cuda
     */
    kVideoFrameTypeCuda,
    /** 
     * @brief Direct3d 11
     */
    kVideoFrameTypeD3D11,
    /** 
     * @brief Direct3d 9
     */
    kVideoFrameTypeD3D9,
    /** 
     * @brief VideoFrame used in Java
     */
    kVideoFrameTypeJavaFrame,
    /** 
     * @brief Vaapi
     */
    kVideoFrameTypeVAAPI,
};

/** 
 * @type keytype
 * @brief  Custom Memory Release
 */
struct ManagedMemory {
    /** 
     * @deprecated since 3.45 and will be deleted in 3.51.
     * @brief Memory data data type, the default is cpu memory. See VideoFrameType:{@link #VideoFrameType}
     * @notes Not available any more and will be removed soon.
     */
    VideoFrameType type = kVideoFrameTypeRawMemory;
    /** 
     * @brief Memory data address
     */
    uint8_t* data = nullptr;
    /** 
     * @brief Size of memory space
     */
    int size = 0;
    /** 
     * @brief User-defined data
     */
    void* user_opaque = nullptr;
    /** 
     * @brief Custom memory free method pointer, if the pointer is not empty, the method will be called to free memory space. The parameters passed in by the
     *         Function are data, size, user_opaque 3 fields in this structure.
     */
    int (*memory_deleter)(uint8_t* data, int size, void* user_opaque) = nullptr;
};

/** 
 * @type keytype
 * @region Video Management
 * @brief Video frame construction object
 */
typedef struct VideoFrameBuilder {
#define ByteRTCNumDataPointers 4
    /** 
     * @brief Memory data type, the default value is cpu memory. See VideoFrameType{@link #VideoFrameType}
     */
    VideoFrameType frame_type = kVideoFrameTypeRawMemory;
    /** 
     * @brief Video frame pixel format. See VideoPixelFormat{@link #VideoPixelFormat}
     */
    VideoPixelFormat pixel_fmt = kVideoPixelFormatUnknown;
    /** 
     * @brief Video frame color space, see ColorSpace{@link #ColorSpace}
     */
    ColorSpace color_space = kColorSpaceUnknown;
    /** 
     * @brief Array of video frame data plane pointer
     */
    uint8_t* data[ByteRTCNumDataPointers] = { 0 };
    /** 
     * @brief Line length of video frame data plane
     */
    int linesize[ByteRTCNumDataPointers] = { 0 };
    /** 
     * @brief SEI data address
     */
    uint8_t* extra_data = nullptr;
    /** 
     * @brief SEI data size
     */
    int extra_data_size = 0;
    /** 
     * @brief Address of supplementary data
     */
    uint8_t* supplementary_info = nullptr;
    /** 
     * @brief Size of supplementary data
     */
    int supplementary_info_size = 0;
    /** 
     * @brief Video frame memory space size
     */
    int size = 0;
    /** 
     * @brief Video frame width
     */
    int width = 0;
    /** 
     * @brief Video frame height
     */
    int height = 0;
    /** 
     * @brief Video rotation angle, see VideoRotation{@link #VideoRotation}.<br>
     *        If the video is rendered on the receiver side using internal rendering of the SDK, the engine processes the rotation angle according to the rotation set here before rendering.<br>
     *        If the video receiver uses custom rendering, the rotation set here does not take effect. The rotation information can be get from IVideoFrame::rotation().
     */
    VideoRotation rotation = kVideoRotation0;
    /** 
     * @hidden for internal use only
     * @brief Mirror parameter
     */
    bool flip = false;
    /** 
     * @brief Hardware acceleration buffer pointer
     */
    void* hwaccel_buffer = nullptr;
    /** 
     * @brief Private data pointer
     */
    void* user_opaque = nullptr;
    /** 
     * @brief Video frame timestamp
     */
    int64_t timestamp_us = 0;
    /** 
     * @brief Hardware accelerate context(AKA Opengl Context, Vulkan Context)
     */
    void* hwaccel_context = nullptr;
#ifdef __ANDROID__
    /** 
     * @brief Hardware accelerate context's java object(Only for Android, AKA Opengl Context)
     */
    jobject android_hwaccel_context = nullptr;
#endif
    /** 
     * @brief Texture matrix (only for texture type frame)
     */
    float tex_matrix[16] = { };
    /** 
     * @brief Texture ID (only for texture type frame)
     */
    uint32_t texture_id = 0;

    /** 
     * @brief User-defined pointer used for release the memory space, if the pointer is not empty.
     */
    int (*memory_deleter)(struct VideoFrameBuilder* builder) = nullptr;
} VideoFrameBuilder;

/** 
 * @type keytype
 * @brief Sets the video frame
 */
class IVideoFrame {
public:
    /** 
     * @brief Gets video frame type, see VideoFrameType{@link #VideoFrameType}
     */
    virtual VideoFrameType frameType() const = 0;
    /** 
     * @brief Gets video frame format, see VideoPixelFormat{@link #VideoPixelFormat}
     */
    virtual VideoPixelFormat pixelFormat() const = 0;
    /** 
     * @brief Gets video content type, see VideoContentType{@link #VideoContentType}.
     */
    virtual VideoContentType videoContentType() const = 0;

    /** 
     * @brief Gets video frame timestamp in microseconds
     */
    virtual int64_t timestampUs() const = 0;
    /** 
     * @brief Gets video frame width in px
     */
    virtual int width() const = 0;
    /** 
     * @brief Gets video frame height in px
     */
    virtual int height() const = 0;
    /** 
     * @brief Gets the video frame rotation angle, see VideoRotation{@link #VideoRotation}
     */
    virtual VideoRotation rotation() const = 0;
    /** 
     * @hidden for internal use only
     * @brief Gets mirror information
     * @return Is there a need to mirror the video:  <br>
     *        + True: Yes  <br>
     *        + False: No
     */
    virtual bool flip() const = 0;
    /** 
     * @brief Gets video frame color space, see ColorSpace{@link #ColorSpace}
     */
    virtual ColorSpace colorSpace() const = 0;
    /** 
     * @brief Video frame color plane number
     * @notes YUV formats are categorized into planar format and packed format.  <br>
     *        In a planar format, the Y, U, and V components are stored separately as three planes, while in a packed format, the Y, U, and V components are stored in a single array.
     */
    virtual int numberOfPlanes() const = 0;
    /** 
     * @brief Gets plane data pointer
     * @param [in] plane_index Plane data index
     */
    virtual uint8_t* getPlaneData(int plane_index) = 0;
    /** 
     * @brief Gets the length of the data line in the plane
     * @param [in] plane_index Plane data index
     */
    virtual int getPlaneStride(int plane_index) = 0;
    /** 
     * @brief Gets extended data pointer
     * @param [in] size Size of extended data in bytes
     */
    virtual uint8_t* getExtraDataInfo(int& size) const = 0;  // NOLINT
    /** 
     * @brief Gets supplementary data pointer
     * @param [in] size Size of supplementary data in bytes
     */
    virtual uint8_t* getSupplementaryInfo(int& size) const = 0;  // NOLINT
    /** 
     * @brief Gets local buffer pointer
     */
    virtual void* getHwaccelBuffer() = 0;
    /** 
     * @brief Get hardware accelerate context(AKA Opengl Context, Vulkan Context)
     */
    virtual void* getHwaccelContext() = 0;
#ifdef __ANDROID__
    /** 
     * @brief Get hardware accelerate context's java object(Only for Android, AKA Opengl Context)
     * @return return JavaLocalRef, need delete manually by use DeleteLocalRef(env, jobject)
     */
    virtual jobject getAndroidHwaccelContext() = 0;
#endif
    /** 
     * @brief Get Texture matrix (only for texture type frame)
     */
    virtual void getTexMatrix(float matrix[16]) = 0;
    /** 
     * @brief Get Texture ID (only for texture type frame)
     */
    virtual uint32_t getTextureId() = 0;
    /** 
     * @brief Makes shallow copies of video frame and return pointer
     */
    virtual IVideoFrame* shallowCopy() = 0;
    /** 
     * @brief Releases video frame
     * @notes After calling pushExternalVideoFrame{@link #IRTCVideo#pushExternalVideoFrame} to push the video frame, you must not call this API to release the resource.
     */
    virtual void release() = 0;
    /** 
     * @brief Converts video frames to i420 format
     */
    virtual void toI420() = 0;
    /** 
     * @brief Get cameraId of the frame, see CameraID{@link #CameraID}
     */
    virtual CameraID getCameraId() const = 0;
    /** 
     * @hidden for internal use only on Windows and Android
     * @type api
     * @brief Get Tile information from the panoramic video frames to enable the FoV (Field of View).
     * @return Video frames in the FoV(filed of view) accroding to the head pose. Refer to FovVideoTileInfo{@link #FovVideoTileInfo} for more details.
     */
    virtual FovVideoTileInfo getFovTile() = 0;
/**
 * @hidden constructor/destructor
 */
protected:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IVideoFrame() = default;
};

/** 
 * @type api
 * @region  video management
 * @brief  Create IVideoFrame
 * @param  [in] builder Video frame build instance. See VideoFrameBuilder{@link #VideoFrameBuilder}
 * @return IVideoFrame{@link #IVideoFrame} instance
 */
BYTERTC_API IVideoFrame* buildVideoFrame(const VideoFrameBuilder& builder);

/** 
 * @type keytype
 * @region  video management
 * @brief  Video frame parameters
 */
typedef struct EncodedVideoFrameBuilder {
    /** 
     * @brief Video frame encoding format. See VideoCodecType{@link #VideoCodecType}
     */
    VideoCodecType codec_type = kVideoCodecTypeUnknown;
    /** 
     * @brief Video frame encoding type. See VideoPictureType{@link #VideoPictureType}
     */
    VideoPictureType picture_type = kVideoPictureTypeUnknown;
    /** 
     * @brief Video frame rotation angle. See VideoRotation{@link #VideoRotation}
     */
    VideoRotation rotation = kVideoRotation0;
    /** 
     * @brief Video frame data pointer
     * @notes IEncodedVideoFrame  takes ownership of the data
     */
    uint8_t* data = nullptr;
    /** 
     * @brief Video frame data size
     */
    int size = 0;
    /** 
     * @brief Video frame width in px
     */
    int width = 0;
    /** 
     * @brief Video frame height in px
     */
    int height = 0;
    /** 
     * @brief Video capture timestamp in microseconds
     */
    int64_t timestamp_us = 0;
    /** 
     * @brief Video encoding timestamp in microseconds
     */
    int64_t timestamp_dts_us = 0;
    /** 
     * @brief The user-defined video frame releases the callback function pointer. If the pointer is not empty, the method will be called to free memory space
     */
    int (*memory_deleter)(uint8_t* data, int size, void* user_opaque) = nullptr;
} EncodedVideoFrameBuilder;
/** 
 * @type keytype
 * @brief Information about video frames
 */
class IEncodedVideoFrame {
public:
    /** 
     * @type api
     * @brief Gets the video encoding type
     * @return Video encoding type. See VideoCodecType{@link #VideoCodecType}
     */
    virtual VideoCodecType codecType() const = 0;
    /** 
     * @type api
     * @brief Gets video capture timestamp
     * @return Video capture timestamp in microseconds
     */
    virtual int64_t timestampUs() const = 0;
    /** 
     * @type api
     * @brief Gets video encoding timestamp
     * @return Video encoding timestamp in microseconds
     */
    virtual int64_t timestampDtsUs() const = 0;
    /** 
     * @type api
     * @brief Get videos frame width
     * @return Video frame width in px
     */
    virtual int width() const = 0;
    /** 
     * @type api
     * @brief Gets video frame height
     * @return Video frame height in px
     */
    virtual int height() const = 0;
    /** 
     * @type api
     * @brief Gets video compression picture type.
     * @return Video compression picture type.See VideoPictureType{@link #VideoPictureType}
     */
    virtual VideoPictureType pictureType() const = 0;
    /** 
     * @type api
     * @brief Gets video frame rotation angle
     * @return Video frame rotation angle. See VideoRotation{@link #VideoRotation}
     */
    virtual VideoRotation rotation() const = 0;
    /** 
     * @type api
     * @brief Get the pointer to the video frame
     * @return The pointer to the video frame
     */
    virtual uint8_t* data() const = 0;
    /** 
     * @type api
     * @brief Gets video frame data size
     * @return Video frame data size
     */
    virtual int dataSize() const = 0;
    /** 
     * @type api
     * @brief Makes shallow copies of video frame and return pointer
     */
    virtual IEncodedVideoFrame* shallowCopy() const = 0;
    /** 
     * @type api
     * @brief Releases video frame
     */
    virtual void release() = 0;
    /**
     * @hidden constructor/destructor
     */
protected:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IEncodedVideoFrame() = default;
};

/** 
 * @type api
 * @region  video management
 * @brief  Create IEncodedVideoFrame
 * @param  [in] builder Encoded video frame build instance. See EncodedVideoFrameBuilder{@link #EncodedVideoFrameBuilder}
 * @return IEncodedVideoFrame{@link #IEncodedVideoFrame} instance
 */
BYTERTC_API IEncodedVideoFrame* buildEncodedVideoFrame(const EncodedVideoFrameBuilder& builder);


/** 
 * @hidden(Linux,Android,iOS)
 * @type keytype
 * @brief  The recommended pixel and framerate by RTC.
 */
struct FrameUpdateInfo {
    /** 
     * @brief Pixel (width * height).
     */
    int pixel;
    /** 
     * @brief The frame rate.
     */
    int framerate;
};


/** 
 * @type keytype
 * @brief The encoding modes for shared-screen streams. The default mode is the high-resolution mode. If you exclude specific windows by setting ScreenFilterConfig{@link #ScreenFilterConfig}, the frame rate of the shared-screen stream will be slower than 30fps。
 */
enum ScreenVideoEncodePreference {
    /** 
     * @hidden(Linux,Android,iOS)
     * @brief The automatic mode. The encoding mode is dynamically determined by RTC based on the content.
     */
    kScreenVideoEncodePreferenceAuto = 0,
    /** 
     * @brief The high frame rate mode. Ensure the highest framerate possible under challenging network conditions. This mode is designed to share audiovisual content, including games and videos.
     */
    kScreenVideoEncodePreferenceFramerate,
    /** 
     * @brief The high-resolution mode. Ensure the highest resolution possible under challenging network conditions. This mode is designed to share micro-detailed content, including slides, documents, images, illustrations, or graphics.
     */
    kScreenVideoEncodePreferenceQuality,

};

/** 
 * @type keytype
 * @brief The encoding configuration for shared-screen streams. See [Setting Video Encoder Configuration](https://docs.byteplus.com/byteplus-rtc/docs/70122).
 */
struct ScreenVideoEncoderConfig {
    /** 
     * @brief Width(in px).
     */
    int width;
    /** 
     * @brief Height(in px).
     */
    int height;
    /** 
     * @brief The frame rate(in fps).
     */
    int frameRate;
    /** 
     * @brief The maximum bitrate(in kbps). Optional for internal capture while mandatory for custom capture.
     *        If you set this value to -1, RTC will automatically recommend the bitrate based on the input resolution and frame rate.
     *        If you set this value to 0, streams will not be encoded and published.
     *        On Version 3.44 or later, the default value for internal capture is -1. On versions earlier than 3.44, you must set the maximum bit rate because there is no default value.
     */
    int maxBitrate = SEND_KBPS_AUTO_CALCULATE;
    /** 
     * @brief The minimum bitrate(in kbps).Optional for internal capture while mandatory for custom capture.
     *        The minimum bitrate must be set lower than the maximum bitrate. Otherwise, streams will not be encoded and published.
     */
    int minBitrate = 0;
    /** 
     * @brief The encoding modes for shared-screen streams.See ScreenVideoEncoderPreference{@link #ScreenVideoEncoderPreference}.
     */
    ScreenVideoEncodePreference encoderPreference = ScreenVideoEncodePreference::kScreenVideoEncodePreferenceQuality;
};


}  // namespace bytertc

#endif // BYTE_RTC_VIDEO_FRAME_H__
