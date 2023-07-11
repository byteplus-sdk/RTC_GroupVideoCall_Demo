/*
 * Copyright (c) 2020 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Video Effect Interface
*/

#pragma once

#ifndef BYTE_RTC_VIDEO_EFFECT_INTERFACE_H__
#define BYTE_RTC_VIDEO_EFFECT_INTERFACE_H__

#include <stdint.h>
#include "bytertc_defines.h"

namespace bytertc {
/** 
 * @type keytype
 * @brief Virtual background type.
 */
enum VirtualBackgroundSourceType {
    /** 
     * @brief Replace the original background with a solid color.
     */
    kVirtualBackgroundSourceColor = 0,
    /** 
     * @brief Replace the original background with the specified image.
     */
    kVirtualBackgroundSourceImage = 1,
};

/** 
 * @hidden for internal use only
 * @type keytype
 * @brief Expression detection configuration
 */
struct VideoEffectExpressionDetectConfig {
  /** 
   * @brief Whether to enable age detection.
   */
  bool detect_age = false;
  /** 
   * @brief Whether to enable gender detection.
   */
  bool detect_gender = false;
  /** 
   * @brief Whether to enable emotion detection.
   */
  bool detect_emotion = false;
  /** 
   * @brief Whether to enable attractiveness detection.
   */
  bool detect_attractiveness = false;
  /** 
   * @brief Whether to enable happiness detection.
   */
  bool detect_happiness = false;
};

/** 
 * @type keytype
 * @brief Virtual background object.
 */
struct VirtualBackgroundSource {
    /** 
     * @brief See VirtualBackgroundSourceType{@link #VirtualBackgroundSourceType}.
     */
    VirtualBackgroundSourceType source_type = kVirtualBackgroundSourceColor;
    /** 
     * @brief The solid color of the background. <br>
     *        The format is 0xAARRGGBB. <br>
     */
    uint32_t source_color = 0xFFFFFFFF;
    /** 
     * @brief The absolute path of the specified image.
     *       + You can upload a .JPG, .PNG, or .JPEG file.  <br>
     *       + The image with a resolution higher than 1080p(Full HD) will be rescaled proportionally to fit in the video.  <br>
     *       + If the image's aspect ratio matches the video's, the image will be rescaled proportionally to fit in the video.  <br>
     *       + If the image’s aspect ratio doesn't match the video's, the shortest side (either height or width) of the image will be stretched proportionally to match the video. Then the image will be cropped to fill in the video.  <br>
     *       + The transparent area in the image will be filled with black.
     */
    const char* source_path = nullptr;
};
/** 
 * @type keytype
 * @brief Face Detection Result
 */
struct FaceDetectResult {
    /** 
     * @brief Face information storage limit, up to 10 faces can be stored.
     */
    static const int max_face_num = 10;
    /** 
     * @brief Face Detection Result <br>
     *        + 0: Success <br>
     *        + !0: Failure. See [Error Code Table](https://docs.byteplus.com/en/effects/docs/error-code-table).
     */
    int detect_result = 0;
    /** 
     * @brief Number of the detected face
     */
    int face_count = 0;
    /** 
     * @brief The face recognition rectangles. The length of the array is the same as the number of detected faces. See Rectangle{@link #Rectangle}.
     */
    Rectangle rect[max_face_num];
    /** 
     * @brief Width of the original image (px)
     */
    int image_width = 0;
    /** 
     * @brief Height of the original image (px)
     */
    int image_height = 0;
    /** 
     * @brief The time stamp of the video frame using face detection.
     */
    int64_t frame_timestamp_us = 0;
};

/** 
 * @hidden for internal use only
 * @type keytype
 * @brief Expression detect information
 */
struct ExpressionDetectInfo {
   /** 
    * @brief The estimated age in range of (0, 100).
    */
    float age = 0;
    /** 
    * @brief The estimated probability of being a male in range of (0.0, 1.0).
    */
    float boy_prob = 0;
    /** 
    * @brief The estimated attractiveness in range of (0, 100).
    */
    float attractive = 0;
    /** 
    * @brief The estimated happy score in range of (0, 100).
    */
    float happy_score = 0;
    /** 
    * @brief The estimated sad score in range of (0, 100).
    */
    float sad_score = 0;
    /** 
    * @brief The estimated angry score in range of (0, 100).
    */
    float angry_score = 0;
    /** 
    * @brief The estimated surprise score in range of (0, 100).
    */
    float surprise_score = 0;
    /** 
    * @brief The estimated emotional arousal in range of (0, 100).
    */
    float arousal = 0;
    /** 
    * @brief The estimated emotional valence in range of (-100, 100).
    */
    float valence = 0;
};

/** 
 * @hidden for internal use only
 * @type keytype
 * @brief Expression detection result
 */
struct ExpressionDetectResult {
    /** 
     * @brief Face information storage limit, up to 10 faces can be stored.
     */
    static const int max_face_num = 10;
   /** 
    * @brief Expression detection result <br>
    *        + 0: Success <br>
    *        + !0: Failure <br>
    */
    int detect_result = 0;
   /** 
    * @brief The number of detected faces.
    */
    int face_count = 0;
   /** 
    * @brief Expression detection information. The length of the array is the same as the number of detected faces. See ExpressionDetectInfo{@link #ExpressionDetectInfo}.
    */
    ExpressionDetectInfo detect_info[max_face_num];
};

/** 
 * @type callback
 * @region Video Effect
 * @brief Face detection observer
 */
class IFaceDetectionObserver {
public:
    /** 
     * @type callback
     * @region Video Effect
     * @brief Callback of the result of face detection with Effect SDK. <br>
     *        After calling enableFaceDetection{@link #IVideoEffect#enableFaceDetection} and using the Effect SDK integrated in the RTC SDK, you will receive this callback.
     * @param [in] result Face detection result. See FaceDetectResult{@link #FaceDetectResult}.
    */
    virtual void onFaceDetectResult(const FaceDetectResult& result) = 0;
    /** 
     * @hidden for internal use only
     * @type callback
     * @region Video Effect
     * @brief Callback of the result of face detection with Effect SDK. <br>
     *        After calling registerFaceDetectionObserver{@link #IVideoEffect#registerFaceDetectionObserver} and setVideoEffectExpressionDetect{@link #IVideoEffect#setVideoEffectExpressionDetect}, you will receive this callback.
     * @param [in] result Expression detection result. See ExpressionDetectResult{@link #ExpressionDetectResult}.
    */
    virtual void onExpressionDetectResult(const ExpressionDetectResult& result) = 0;

};

/** 
 * @type api
 * @region Audio & Video Processing
 * @brief Advanced video effects.
 */
class IVideoEffect {
public:
    /** 
     * @type api
     * @brief Checks video effect license, sets the video effect resource model path, and initializes video effect.
     * @param [in] license_file_path The absolute path of the license file for authorization.
     * @param [in] algo_model_dir The absolute path of the Effects SDK's models file.
     * @return  <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     */
    virtual int initCVResource(const char* license_file_path, const char* algo_model_dir) = 0;

    /** 
     * @hidden for internal use only
     * @type api
     * @brief  Sets the video effects resource finder path and initializes video effects.
     * @param [in] finder ResourceFinder path
     * @param [in] deleter ResourceDeleter path
     * @return  <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     */
    virtual int setAlgoModelResourceFinder(uintptr_t finder, uintptr_t deleter) = 0;

    /** 
     * @deprecated since 3.50 and will be deleted in 3.55, use initCVResource{@link #IVideoEffect#initCVResource} instead.
     * @type api
     * @region Video Effects
     * @brief Sets the video effects algorithm model path
     * @param [in] modelPath  Model path
     */
    virtual void setAlgoModelPath(const char* modelPath) = 0;

    /** 
     * @deprecated since 3.50 and will be deleted in 3.55, use initCVResource{@link #IVideoEffect#initCVResource} instead.
     * @type api
     * @region Video Effects
     * @brief  Video effects license check
     * @param androidContext   <br>
     *        Android Context object pointer (jobject type). The nullptr
     * @param jnienv Android <br>
     *        JNIEnv pointer is passed to platforms other than Android. Incoming nullptr
     * @param licensePath  Absolute path of license file from platforms other than Android
     * @return   <br>
     *       + 0: call succeeded <br>
     *       + 1000: not integrated cvsdk <br>
     *       + 1001: cv function is not supported in this RTC version <br>
     *       + < 0: Failure Please refer to [Error Code Table](https://docs.byteplus.com/en/effects/docs/error-code-table).
     * @notes  Before you start using video effects, you must first call this method for license verification
     */
    virtual int checkLicense(void* androidContext, void* jnienv, const char* licensePath) = 0;

    /** 
     * @type api
     * @region Video Effects
     * @hidden(iOS, Android)
     * @brief Get authorization messages from the Effect SDK to obtain online licenses.
     * @param  [in] ppmsg Authorization message string address
     * @param  [in] len Authorization message string length
     * @return   <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     * @notes  <br>
     *         + You must obtain an online license for the Effect SDK before using the CV functions. <br>
     *         + After obtaining authorization messages through this API, you must refer to [Online Authorization Instructions](https://www.volcengine.com/docs/6705/102012) to implement the business logic of obtaining online licenses by yourself. After obtaining the license, you must call initCVResource{@link #IVideoEffect#initCVResource} to confirm that the license is valid. Then you can use the CV function. <br>
     *         + After obtaining the authorization message, call freeAuthMessage{@link #freeAuthMessage} to free memory.
     */
    virtual int getAuthMessage(char ** ppmsg, int * len) = 0;
    /** 
     * @type api
     * @region Video Effects
     * @hidden(iOS,Android)
     * @brief After using the authorization message string, free the memory.
     * @param  [in] pmsg The authorization message string returned by getAuthMessage.
     * @return   <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     */
    virtual int freeAuthMessage(char * pmsg) = 0;

    /** 
     * @deprecated since 3.50 and will be deleted in 3.55, use enableVideoEffect{@link #IVideoEffect#enableVideoEffect} instead.
     * @type api
     * @region Video Effects
     * @brief  Turn on and off video effects
     * @param [in] enabled  Whether to turn on effects, true: on, false: off
     * @return   <br>
     *      + 0: Succeed <br>
     *      + 1000: Effect SDK is not integrated  <br>
     *      + 1001: CV functionality is not supported in the current RTC version  <br>
     *      + <0: Other errors, see [Error Code Table](https://docs.byteplus.com/en/effects/docs/error-code-table).
     * @notes Call this API after CheckLicense{@link #checkLicense} and SetAlgoModelPath{@link #setAlgoModelPath}.
     */
    virtual int enableEffect(bool enabled) = 0;
    
    /** 
     * @hidden for internal use only
     * @region Video Effects
     * @brief Returns video effect handle. Private method
     */
    virtual void* getEffectHandle() = 0;

    /** 
     * @type api
     * @brief Enables video effects including beauty and color filters.
     * @return  <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     * @notes  <br>
     *      + You must call initCVResource{@link #IVideoEffect#initCVResource} before calling this API.
     *      + This API does not turn on video effects directly, you must call setEffectNodes{@link #IVideoEffect#setEffectNodes} or setColorFilter{@link #IVideoEffect#setColorFilter} next.
     *      + Call disableVideoEffect{@link #IVideoEffect#disableVideoEffect} to turn off video effects.
     */
    virtual int enableVideoEffect() = 0;

    /** 
     * @type api
     * @brief Disables video effects.
     * @return  <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     * @notes Call enableVideoEffect{@link #IVideoEffect#enableVideoEffect} to enable video effects.
     */
    virtual int disableVideoEffect() = 0;

    /** 
     * @type api
     * @brief Sets the video effects material package.
     * @param [in] effectNodePaths Array of effect material package paths. <br>
     *        To remove the current video effect, set it to null.
     * @param [in] nodeNum Number of effect material packages.
     * @return   <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     * @notes You must call enableVideoEffect{@link #IVideoEffect#enableVideoEffect} before calling this API.
     */
    virtual int setEffectNodes(const char** effectNodePaths, int nodeNum) = 0;

    /** 
     * @hidden for internal use only
     * @type api
     * @brief  Appends video effects material package.
     * @param [in] effectNodePaths Array of effect material package paths.
     * @param [in] nodeNum Number of effect material packages.
     * @return  <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     * @notes This API adds new video effect to the video effect you set with setEffectNodes{@link #IVideoEffect#setEffectNodes}.
     */
    virtual int appendEffectNodes(const char** effectNodePaths, int nodeNum) = 0;

    /** 
     * @hidden for internal use only
     * @type api
     * @brief  Removes the designated video effects material package.
     * @param [in] effectNodePaths Array of effect material package paths.
     * @param [in] nodeNum Number of effect material packages.
     * @return  <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     * @notes Removes the designated video effects in setEffectNodes{@link #IVideoEffect#setEffectNodes} or appendEffectNodes{@link #IVideoEffect#appendEffectNodes}.
     */
    virtual int removeEffectNodes(const char** effectNodePaths, int nodeNum) = 0;

    /** 
     * @deprecated since 3.50 and will be deleted in 3.55, use updateEffectNode{@link #IVideoEffect#updateEffectNode} instead.
     * @type api
     * @region Video Effects
     * @brief  Set the effect intensity
     * @param  [in] nodePath <br>
     *         Special effects material package path
     * @param  [in] nodeKey <br>
     *         The name of the material key to be set, please refer to [Material key correspondence instructions](http://ailab-cv-sdk.bytedance.com/docs/2036/99769/) for the value.
     * @param  [in] nodeValue <br>
     *         The intensity value that needs to be set, the value range [0,1], and the setting is invalid when it exceeds the range.
     * @return   <br>
     *       + 0: Call succeeded <br>
     *       + 1000: The Effect SDK is not integrated.  <br>
     *       + 1001: This API is not available for your Effect SDK.  <br>
     *       + < 0: Failure. For specific error codes, please refer to the [error codes](https://www.volcengine.com/docs/6705/102042).
     */
    virtual int updateNode(const char* nodePath, const char* nodeKey, float nodeValue) = 0;

    /** 
     * @type api
     * @brief Sets the intensity of video effects.
     * @param [in] effectNodePath The absolute path of the effects resource package, see [Resource Package Structure](https://docs.byteplus.com/effects/docs/resource-package-structure-v421-and-later).
     * @param [in] nodeKey The name of the material key to be set, see [Functions of Resource Keys](https://docs.byteplus.com/effects/docs/functions-of-resource-keys-v421-and-later) for the value.
     * @param [in] nodeValue The intensity value that needs to be set, the value range [0,1], and the setting is invalid when it exceeds the range.
     * @return   <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     */
    virtual int updateEffectNode(const char* effectNodePath, const char* nodeKey, float nodeValue) = 0;

    /** 
     * @type api
     * @brief Sets the color filter.
     * @param [in] resPath Filter effects package absolute path.
     * @return   <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     * @notes Call setColorFilterIntensity{@link #IVideoEffect#setColorFilterIntensity} to set the intensity of the color filter enabled. Set the intensity to 0 to turn off color filter.
     */
    virtual int setColorFilter(const char* resPath) = 0;

    /** 
     * @type api
     * @brief Sets the intensity of the color filter enabled.
     * @param [in] intensity Filter intensity. The value range [0,1] is set to be invalid when the range is exceeded.
     *                  Set the intensity to 0 to turn off color filter.
     * @return   <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     */
    virtual int setColorFilterIntensity(float intensity) = 0;
    
    /** 
     * @hidden for internal use only
     * @type api
     * @brief Private method，Sets the video effects material package.
     * @param [in] stickerPath effect material package path. <br>
     *        To remove the current video effect, set it to null.
     * @return   <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is not available for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     * @notes You must call enableVideoEffect{@link #IVideoEffect#enableVideoEffect} before calling this API.
     */
    virtual int ApplyStickerEffect(const char* stickerPath) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @brief Sets the original background to a specified image or a solid color.
     * @param [in] bg_sticker_path The absolute path of virtual background effects.
     * @param [in] source Virtual background source. See VirtualBackgroundSource{@link #VirtualBackgroundSource}.
     * @return  <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     * @notes <br>
     *      + You must call initCVResource{@link #IVideoEffect#initCVResource} before calling this API.
     *      + Call disableVirtualBackground{@link #IVideoEffect#disableVirtualBackground} to turn off the virtual background.
     */
    virtual int enableVirtualBackground(const char* bg_sticker_path, const VirtualBackgroundSource& source) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @brief Turns off the virtual background.
     * @return   <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     * @notes After calling enableVirtualBackground{@link #IVideoEffect#enableVirtualBackground} to enable the virtual background function, you can call this API to turn it off.
     */
    virtual int disableVirtualBackground() = 0;

    /** 
     * @hidden for internal use only
     * @type api
     * @brief  Sets the configuration for video effects expression detection.
     * @param [in] expressionDetectConfig Expression detection configuration. See VideoEffectExpressionConfig{@link #VideoEffectExpressionConfig}.
     * @return  <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     */
    virtual int setVideoEffectExpressionDetect(const VideoEffectExpressionDetectConfig& expressionDetectConfig) = 0;

    /** 
     * @deprecated since 3.50 and will be deleted in 3.55, use enableFaceDetection{@link #IVideoEffect#enableFaceDetection} and disableFaceDetection{@link #IVideoEffect#disableFaceDetection} instead.
     * @type api
     * @brief  Register the observer for the result of face detection.
     *        With this observer, you will receive onFaceDetectResult{@link #IFaceDetectionObserver#onFaceDetectResult} periodically.
     * @param [in] observer See IFaceDetectionObserver{@link #IFaceDetectionObserver}.
     * @param [in] interval_ms Time interval in ms. The value should be greater than 0. The actual time interval of receiving callbacks is between `interval` and `interval+the time slot of a captured video frame`.
     * @return <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     */
    virtual int registerFaceDetectionObserver(IFaceDetectionObserver* observer, int interval_ms) = 0;

    /** 
     * @type api
     * @brief Starts face detection and registers the observer for the result.
     *        With this observer, you will receive onFaceDetectResult{@link #IFaceDetectionObserver#onFaceDetectResult} periodically.
     * @param [in] observer See IFaceDetectionObserver{@link #IFaceDetectionObserver}.
     * @param [in] interval_ms The minimum time interval between two callbacks in milliseconds. The value should be greater than 0. The actual time interval is between interval_ms and interval_ms+the time slot of a captured video frame.
     * @param [in] faceModelPath The absolute path of the face detection algorithm file. Typically it is the tt_face_vXXX.model file in the ttfacemodel folder.
     * @return <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + -1004: Initializing. This function will be available when the initialization is completed.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     */
    virtual int enableFaceDetection(IFaceDetectionObserver* observer, unsigned int interval_ms, const char* faceModelPath) = 0;

    /** 
     * @type api
     * @brief Stops face detection.
     * @return <br>
     *      + 0: Success.
     *      + –1000: The Effects SDK is not integrated.
     *      + –1001: This API is unavailable for your Effects SDK.
     *      + –1002: Your Effects SDK's version is incompatible.
     *      + < 0: Other error. See [error code table](https://docs.byteplus.com/effects/docs/error-code-table) for specific instructions.
     */
    virtual int disableFaceDetection() = 0;

    /** 
     * @deprecated since 3.50 and will be deleted in 3.55, use enableVirtualBackground{@link #IVideoEffect#enableVirtualBackground} instead.
     * @type api
     * @region Video Effects
     * @brief Set the original background to a specified image or a solid color.
     *        To disable this effect, set the modelPath parameter to null.
     * @param [in] modelPath Set the path of virtual background effects.  <br>
     * @param [in] source Set the local path of background images. See VirtualBackgroundSource{@link #VirtualBackgroundSource}.
     * @return  <br>
     *        + 0：Success.  <br>
     *        + 1000：The BytePlus Effects SDK is not integrated.  <br>
     *        + 1001：This feature is not supported in the BytePlus Effects SDK.  <br>
     *        + < 0：Other errors. See [Error Code Table](https://docs.byteplus.com/en/effects/docs/error-code-table).
     * @notes  <br>
     *        Before calling this method, you should call checkLicense{@link #IVideoEffect#checkLicense}、setAlgoModelPath{@link #IVideoEffect#setAlgoModelPath}, and enableEffect {@link #IVideoEffect#enableEffect} in order.
     */
    virtual int setBackgroundSticker(const char* modelPath, const VirtualBackgroundSource& source) = 0;

};

}  // namespace bytertc

#endif // BYTE_RTC_VIDEO_EFFECT_INTERFACE_H__
