/*
 * Copyright (c) 2020 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Interface Lite
 */

#pragma once

#include "bytertc_room.h"
#include "rtc/bytertc_video_device_manager.h"
#include "rtc/bytertc_audio_frame.h"
#include "rtc/byte_rtc_asr_engine_event_handler.h"
#include "rtc/bytertc_audio_mixing_manager.h"
#include "rtc/bytertc_audio_effect_player.h"
#include "rtc/bytertc_media_player.h"
#include "rtc/bytertc_video_processor_interface.h"
#include "rtc/bytertc_camera_control_interface.h"
#include "bytertc_video_event_handler.h"
#include "rtc/bytertc_sing_scoring_interface.h"
#include "rtc/bytertc_ktv_manager_interface.h"

namespace bytertc {

/** 
 * @type api
 * @brief Engine API
 */
class IRTCVideo {
public:

    /** 
     * @type api
     * @region Volume management
     * @brief Adjust the audio capture volume
     * @param  [in] index Index of the stream, whose volume needs to be adjusted. Refer to StreamIndex{@link #StreamIndex} for more details.
     * @param  [in] volume Ratio of capture volume to original volume. Ranging: [0,400]. Unit: %
     *                     This changes the volume property of the audio data other than the hardware volume.<br>
     *        + 0: Mute <br>
     *        + 100: Original volume. To ensure the audio quality, we recommend [0, 100].
     *        + 400: Four times the original volume with signal-clipping protection.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  Call this API to set the volume of the audio capture before or during the audio capture.
     */
    virtual int setCaptureVolume(StreamIndex index, int volume) = 0;
    /** 
     * @type api
     * @region volume management
     * @brief Adjust the playback volume of the mixed remote audios.  You can call this API before or during the playback.
     * @param [in] volume Ratio(%) of playback volume to original volume, in the range [0, 400], with overflow protection.  <br>
     *                    This changes the volume property of the audio data other than the hardware volume.<br>
     *        To ensure the audio quality, we recommend setting the volume to `100`.  <br>
     *        + 0: mute <br>
     *        + 100: original volume <br>
     *        + 400: Four times the original volume with signal-clipping protection.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes Suppose a remote user A is always within the range of the target user whose playback volume will be adjusted, if you use both this method and setRemoteAudioPlaybackVolume{@link #IRTCVideo#setRemoteAudioPlaybackVolume}/setRemoteRoomAudioPlaybackVolume{@link #IRTCRoom#setRemoteRoomAudioPlaybackVolume}, the volume that the local user hears from user A is the overlay of both settings.
     */
    virtual int setPlaybackVolume(const int volume) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Audio management
     * @brief Turn on/off the earphone monitor function
     * @param  [in] mode Whether to turn on the earphone monitor function. See EarMonitorMode{@link #EarMonitorMode}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes You can use ear monitoring feature when the earpiece is directly connected to the device by 3.5mm interface, USB interface, or BlueTooth. You cannot use ear monitoring feature when the earpiece is connected to a monitor via the HDMI or USB-C interface, and then connected to the device, or connected to an OTG external sound card, and then connected to the device.
     */
    virtual int setEarMonitorMode(EarMonitorMode mode) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Audio Management
     * @brief Set the volume of the earphone monitor
     * @param  [in] volume The volume of the earphone monitor, the value range: [0,100], the unit:%
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes Before setting the volume of the earphone monitor, you must first call setEarMonitorMode{@link #IRTCVideo#setEarMonitorMode} to turn on the earphone monitor function.
     */
    virtual int setEarMonitorVolume(const int volume) = 0;

    /** 
     * @hidden(macOS,Windows,Linux,Android)
     * @type api
     * @region  bluetooth mode settings
     * @brief  set bluetooh mode in media scenario
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes bluetooth mode can be setted only in media scenario.
     */
    virtual int setBluetoothMode(BluetoothMode mode) = 0;

    /** 
     * @type api
     * @region Audio management
     * @brief Start internal audio capture. The default is off.   <br>
     *        Internal audio capture refers to: capturing audio using the built-in module.
     *        The local client will be informed via onAudioDeviceStateChanged{@link #IRTCVideoEventHandler#onAudioDeviceStateChanged} after starting audio capture by calling this API. <br>
     *        The remote clients in the room will be informed of the state change via onUserStartAudioCapture{@link #IRTCVideoEventHandler#onUserStartAudioCapture} after the visible user starts audio capture by calling this API..
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + To enable a microphone without the user's permission will trigger onWarning{@link #IRTCVideoEventHandler#onWarning}. <br>
     *        + Call stopAudioCapture{@link #IRTCVideo#stopAudioCapture} to stop the internal audio capture. Otherwise, the internal audio capture will sustain until you destroy the engine instance. <br>
     *       +  To mute and unmute microphones, we recommend using publishStream{@link #IRTCRoom#publishStream} and unpublishStream{@link #IRTCRoom#unpublishStream}, other than stopAudioCapture{@link #IRTCVideo#stopAudioCapture} and this API. Because starting and stopping capture devices often need some time waiting for the response of the device, that may lead to a short silence during the communication.
     *        + Once you create the engine instance, you can start internal audio capture regardless of the audio publishing state. The audio stream will start publishing only after the audio capture starts. <br>
     *        + To switch from custom to internal audio capture, stop publishing before disabling the custom audio capture module and then call this API to enable the internal audio capture.<br>
     */
    virtual int startAudioCapture() = 0;

    /** 
     * @type api
     * @region Audio management
     * @brief Stop internal audio capture. The default is off.   <br>
     *        Internal audio capture refers to: capturing audio using the built-in module.
     *         The local client will be informed via  onAudioDeviceStateChanged{@link #IRTCVideoEventHandler#onAudioDeviceStateChanged}  after stopping audio capture by calling this API.<br>. <br>
     *        The remote clients in the room will be informed of the state change via onUserStopAudioCapture{@link #IRTCVideoEventHandler#onUserStopAudioCapture} after the visible client stops audio capture by calling this API.<br>.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + Call startAudioCapture{@link #startAudioCapture} to enable the internal audio capture. <br>
     *        + Without calling this API the internal audio capture will sustain until you destroy the engine instance.<br>
     */
    virtual int stopAudioCapture() = 0;

    /** 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region Audio management
     * @brief Sets the audio scenarios.
     *        You can choose the appropriate audio scenario according to your application.
     *        After selecting the audio scenario, SDK will automatically select the call/media volume, according to the client-side audio device and status.
     * @param [in] scenario  Audio scenarios. See AudioScenarioType{@link #AudioScenarioType}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *         + It is recommended to call this API before joining the room and calling other audio related interfaces. If this API is called afterwards, audio lag may be introduced.
     *         + Call volume is more suitable for calls, meetings and other scenarios that demand information accuracy. Call volume will activate the system hardware signal processor, making the sound clearer. The volume cannot be reduced to 0. <br>
     *         + Media volume is more suitable for entertainment scenarios, which require musical expression. The volume can be reduced to 0.
     */
    virtual int setAudioScenario(AudioScenarioType scenario) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Bel Sound Effect Management
     * @brief Set the sound change effect type
     * @param  [in] voice_changer The sound change effect type. See VoiceChangerType{@link #VoiceChangerType}.
     * @return API call result: <br>
     *        + 0: Success <br>
     *        + !0: Failure
     * @notes   <br>
     *         + You can call it before and after entering the room. <br>
     *         + Effective for both internal and external audio source. <br>
     *         + Only valid for mono-channel audio. <br>
     *         + Mutually exclusive with setVoiceReverbType{@link #setVoiceReverbType}, and the effects set later will override the effects set first.<br>
     *         + Before using the feature, you must implement [Audio SDK](https://go.byteplus.com/audio-sdk). Please contact the technical supporters.
     */
    virtual int setVoiceChangerType(VoiceChangerType voice_changer) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Bel Sound Effect Management
     * @brief Set the reverb effect type
     * @param  [in] voice_reverb The reverb effect type. See VoiceReverbType{@link #VoiceReverbType}.
     * @return API call result: <br>
     *        + 0: Success <br>
     *        + !0: Failure
     * @notes   <br>
     *         + You can call it before and after entering the room. <br>
     *         + Effective for both internal and external audio source.  <br>
     *         + Only valid for mono-channel audio.  <br>
     *         + Mutually exclusive with setVoiceChangerType{@link #setVoiceChangerType}, and the effects set later will override the effects set first.<br>
     *         + To use this feature, contact us for technical support.
     */
    virtual int setVoiceReverbType(VoiceReverbType voice_reverb) = 0;
    /** 
     * @hidden(Linux)
     * @type api
     * @brief Set the equalization effect for the local captured audio. The audio includes both internal captured audio and external captured voice, but not the mixing audio file.
     * @param config See VoiceEqualizationConfig{@link #VoiceEqualizationConfig}.
     * @return   <br>
     *         + 0: Success. <br>
     *         + < 0: Failure.
     * @notes According to the Nyquist acquisition rate, the audio acquisition rate must be greater than twice the set center frequency. Otherwise, the setting will not be effective.
     */
    virtual int setLocalVoiceEqualization(VoiceEqualizationConfig config) = 0;
    /** 
     * @type api
     * @hidden(Linux)
     * @brief Set the reverb effect for the local captured audio. The audio includes both internal captured audio and external captured voice, but not the mixing audio file.
     * @param param See VoiceReverbConfig{@link #VoiceReverbConfig}.
     * @return   <br>
     *         + 0: Success. <br>
     *         + < 0: Failure.
     * @notes Call enableLocalVoiceReverb{@link #IRTCVideo#enableLocalVoiceReverb} to enable the reverb effect.
     */
    virtual int setLocalVoiceReverbParam(VoiceReverbConfig param) = 0;
    /** 
     * @type api
     * @hidden(Linux)
     * @brief Enable the reverb effect for the local captured voice.
     * @param enable
     * @return   <br>
     *         + 0: Success. <br>
     *         + < 0: Failure.
     * @notes Call setLocalVoiceReverbParam{@link #IRTCVideo#setLocalVoiceReverbParam} to set the reverb effect.
     */
    virtual int enableLocalVoiceReverb(bool enable) = 0;

    /** 
     * @type api
     * @region audio management
     * @brief Sets the sound quality. Call this API to change the sound quality if the audio settings in the current RoomProfileType{@link #RoomProfileType} can not meet your requirements.
     * @param  [in] audio_profile Sound quality. See AudioProfileType{@link #AudioProfileType}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *         + You can call this API before and after entering the room. <br>
     *         + Support dynamic switching of sound quality during a call.
     */
    virtual int setAudioProfile(AudioProfileType audio_profile) = 0;

    /** 
     * @type api
     * @region audio management
     * @brief Set the Active Noise Cancellation(ANC) mode during audio and video communications.
     * @param [in] ans_mode ANC modes. See AnsMode{@link #AnsMode}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  You can call this API before or after entering a room. When you repeatedly call it, only the last call takes effect.
     * @notes  When you repeatedly call this API, only the last call takes effect.
     */
    virtual int setAnsMode(AnsMode ans_mode) = 0;

    /** 
     * @type api
     * @valid since 3.51
     * @region Audio device Management
     * @brief  Turns on/ off AGC(Analog Automatic Gain Control). <br>
     *         After AGC is enabled, SDK can automatically adjust microphone pickup volume to keep the output volume at a steady level.
     * @param [in] enable whether to turn on AGC. <br>
     *        + true: AGC is turned on.  <br>
     *        + false: AGC is turned off. 
     * @return  <br>
     *         + 0: Success. <br>
     *         + -1: Failure.
     * @notes  <br>
     *         You can call this method before and after joining the room. To turn on AGC before joining the room, you need to contact the technical support to get a private parameter to set RoomProfileType{@link #RoomProfileType}. <br>
     *         To enable AGC after joining the room, you must set RoomProfileType{@link #RoomProfileType} to `kRoomProfileTypeMeeting`,`kRoomProfileTypeMeetingRoom` or `kRoomProfileTypeClassroom` .  <br>
     *         It is not recommended to call setAudioCaptureDeviceVolume{@link #IAudioDeviceManager#setAudioCaptureDeviceVolume} to adjust microphone pickup volume with AGC on.
     */
    virtual int enableAGC(bool enable) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Custom Audio Capture and Rendering
     * @brief Switch the audio capture type.
     * @param [in] type Audio input source type. See AudioSourceType{@link #AudioSourceType} <br>
     *            Use internal audio capture by default. The audio capture type and the audio render type may be different from each other.
     * @return   Method call result:   <br>
     *         + >0: Success. <br>
     *         + -1: Failure.
     * @notes   <br>
     *       + You can call this API before or after joining the room.<br>
     *       + If you call this API to switch from internal audio capture to custom capture, the internal audio capture is automatically disabled. You must call pushExternalAudioFrame{@link #IRTCVideo#pushExternalAudioFrame} to push custom captured audio data to RTC SDK for transmission. <br>
     *       + If you call this API to switch from custom capture to internal capture, you must then call startAudioCapture{@link #IRTCVideo#startAudioCapture} to enable internal capture. <br>
     */
    virtual int setAudioSourceType (AudioSourceType type) = 0;
    /** 
     * @hidden(Linux)
     * @type api
     * @region Custom Audio Capture and Rendering
     * @brief Switch the audio render type.
     * @param [in] type Audio output source type. See AudioRenderType{@link #AudioRenderType}. <br>
     *             Use internal audio render by default. The audio capture type and the audio render type may be different from each other.
     * @return  Method call result:   <br>
     *         + >0: Success. <br>
     *         + -1: Failure.
     * @notes   <br>
     *       + You can call this API before or after joining the room. <br>
     *       + After calling this API to enable custom audio rendering, call pullExternalAudioFrame{@link #IRTCVideo#pullExternalAudioFrame} for audio data.
     */
    virtual int setAudioRenderType (AudioRenderType type) = 0;
    /** 
     * @hidden(Linux)
     * @type api
     * @region Custom Audio Capture and Rendering
     * @brief Push custom captured audio data to the RTC SDK.
     * @param [in] audio_frame The audio data of 10 ms. See IAudioFrame{@link #IAudioFrame}。
     * @return <br>
     *         + 0: Success <br>
     *         + < 0: Failure <br>
     * @notes   <br>
     *        + Before calling this API, you must call setAudioSourceType{@link #IRTCVideo#setAudioSourceType} to enable custom audio capture. <br>
     *        + You must push custom captured audio data every 10 ms. <br>
     */
    virtual int pushExternalAudioFrame(IAudioFrame* audio_frame) = 0;
    /** 
     * @hidden(Linux)
     * @region  custom audio acquisition rendering
     * @brief  Pull remote audio data. You can use the data for audio processing or custom rendering.
     * @param [out] audio_frame The obtained audio data of 10 ms. See IAudioFrame{@link #IAudioFrame}。
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes   <br>
     *        + Before pulling the audio data, call setAudioRenderType{@link #IRTCVideo#setAudioRenderType} to enable custom rendering. <br>
     *        + You should pull audio data every 10 milliseconds since the duration of a RTC SDK audio frame is 10 milliseconds. Samples x call frequency = audio_frame's sample rate. Assume that the sampling rate is set to 48000, call this API every 10 ms, so that 480 sampling points should be pulled each time. <br>
     *        + This function runs in the user calling thread and is a synchronous function. <br>
     */
    virtual int pullExternalAudioFrame(IAudioFrame* audio_frame) = 0;

    /** 
     * @type api
     * @region Video management
     * @brief Enable internal video capture immediately. The default is off.   <br>
     *        Internal video capture refers to: use the RTC SDK built-in video capture module to capture.<br>
     *        The local client will be informed via onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} after starting video capture by calling this API.
     *        The remote clients in the room will be informed of the state change via onUserStartVideoCapture{@link #IRTCVideoEventHandler#onUserStartVideoCapture} after the visible client starts video capture by calling this API.<br>
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + Call stopVideoCapture{@link #IRTCVideo#stopVideoCapture} to stop the internal video capture. Otherwise, the internal video capture will sustain until you destroy the engine instance.<br>
     *        + Once you create the engine instance, you can start internal video capture regardless of the video publishing state. The video stream will start publishing only after the video capture starts. <br>
     *        + To switch from custom to internal video capture, stop publishing before disabling the custom video capture module and then call this API to enable the internal video capture.<br>
     */
    virtual int startVideoCapture() = 0;

    /** 
     * @type api
     * @region Video management
     * @brief Disable internal video capture immediately. The default is off.   <br>
     *        Internal video capture refers to: use the RTC SDK built-in video capture module to capture.<br>
     *        The local client will be informed via onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} after stopping video capture by calling this API.<br>
     *        The remote clients in the room will be informed of the state change via onUserStopVideoCapture{@link #IRTCVideoEventHandler#onUserStopVideoCapture} after the visible client stops video capture by calling this API.<br>
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + Call startVideoCapture{@link #IRTCVideo#startVideoCapture} to enable the internal video capture. <br>
     *        + Without calling this API the internal video capture will sustain until you destroy the engine instance.
     */
    virtual int stopVideoCapture() = 0;

   /** 
    * @type api
    * @region  Video Management
    * @brief  Set the video capture parameters for internal capture of the RTC SDK. <br>
    *         If your project uses the SDK internal capture module, you can specify the video capture parameters including preference, resolution and frame rate through this interface.
    * @param video_capture_config  Video capture parameters. See: VideoCaptureConfig{@link #VideoCaptureConfig}.
    * @return   <br>
    *         + 0: Success; <br>
    *         + < 0: Failure; <br>
    * @notes   <br>
    *  + This interface can be called after the engine is created and takes effect immediately after being called. It is recommended to call this interface before calling startVideoCapture{@link #IRTCVideo#startVideoCapture}.
    *  + It is recommended that different Engines on the same device use the same video capture parameters.
    *  + If you used the internal module to start video capture before calling this interface, the capture parameters default to Auto.
    */
    virtual int setVideoCaptureConfig(const VideoCaptureConfig& video_capture_config) = 0;

    /** 
     * @hidden(Android,iOS)
     * @type api
     * @brief Set the rotation of the local video images.
     *        Call this API to rotate the videos when the camera is fixed upside down or tilted.
     * @param rotation It defaults to `VIDEO_ROTATION_0(0)`, which means not to rotate. Refer to VideoRotation{@link #VideoRotation}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes 
     * + This API affects the external-sourced videos. The final rotation would be the original rotation angles adding up with the rotation set by calling this API.
     * + For Windows SDK, this API would not rotate the background added by calling enableVirtualBackground{@link #IVideoEffect#enableVirtualBackground}.
     * + The rotation would not be applied to the Stream pushed to CDN.
     */
    virtual int setVideoCaptureRotation(VideoRotation rotation) = 0;

    /** 
     * @type api
     * @region Video Management
     * @brief Enables/Disables the mode of publishing multiple video streams with different encoding configuration.
     * @param [in] enabled Whether to enable the mode of publishing multiple video streams:  <br>
     *        + true: Yes  <br>
     *        + false: No(Default setting)
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + You can call this API  <br>
     *            - Before entering the room, or  <br>
     *            - After entering the room but before publishing streams.  <br>
     *        + When the simulcast mode is turned on, it cannot be turned off dynamically, nor can the parameters for video encoding be updated.  <br>
     *        + After setting this API to "True", you can call [setVideoEncoderConfig](#IRTCVideo-setvideoencoderconfig-1) to set encoding configuration for each stream before you publish stream.  <br>
     *        + If this function is off, or if this function is on but you don't set the configuration of any stream, only one stream will be sent with a resolution of 640px × 360px and a frame rate of 15fps.
     */
    virtual int enableSimulcastMode(bool enabled) = 0;

    /**
     * {en}
     * @type api
     * @region Video Management
     * @brief <span id="IRTCVideo-setvideoencoderconfig-1"></span> Video publisher call this API to set the parameters of the maximum resolution video stream that is expected to be published, including resolution, frame rate, bitrate, and fallback strategy in poor network conditions.
     *        You can only set configuration for one stream with this API. If you want to set configuration for multiple streams, Call [setVideoEncoderConfig](#IRTCVideo-setvideoencoderconfig-2).
     * @param [in] max_solution The maximum video encoding parameter. See VideoEncoderConfig{@link #VideoEncoderConfig}.
     * @return  API call result: <br>
     *        + 0: Success <br>
     *        + ! 0: Failure <br>
     * @notes  <br>
     *        + You can use enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode} simultaneously to publish streams with different resolutions. Specifically, if you want to publish multiple streams with different resolutions, you need to call this method and enable the simulcast mode with enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode} before publishing your streams. The SDK will intelligently adjust the number of streams to be published (up to 4) and their parameters based on the settings of the subscribing end. The resolution set by calling this method will be the maximum resolution among the streams. For specific rules, please refer to [Simulcasting](https://docs.byteplus.com/en/byteplus-rtc/docs/70139).<br>
     *        + Without calling this API, SDK will only publish one stream for you with a resolution of 640px × 360px and a frame rate of 15fps.  <br>
     *        + In custom capturing scenario, you must call this API to set encoding configurations to ensure the integrity of the picture received by the remote users.<br>
     *        + This API is applicable to the video stream captured by the camera, see setScreenVideoEncoderConfig{@link #IRTCVideo#setScreenVideoEncoderConfig} for setting parameters for screen sharing video stream.
     */
    virtual int setVideoEncoderConfig(const VideoEncoderConfig& max_solution) = 0;
    /** 
     * @hidden for internal use only on Windows and Android
     * @type api
     * @brief <span id="RTCVideo-setvideoencoderconfig-3"></span> Video publishers call this API to set the parameters of the panoramic video, including resolution of the Fov, HD field, LD background, sizes of the Tiles, and the other regular configurations.
     *        You can only set configuration for one stream with this API. If you want to set configuration for multiple streams, Call [setVideoEncoderConfig](#RTCVideo-setvideoencoderconfig-2).
     * @param encoder_config The maximum video encoding parameter. Refer to VideoEncoderConfig{@link #VideoEncoderConfig} for more details..  <br>
     *                      Panoramic video of 8K or 4K is supported.
     * @param parameters JSON string of encoder configuration of the panoramic video <br>
     *                  + 8K: HD: 7680x3840, LD: 2560x1280, Tile: 640x640<br>
     *                  + 4K: HD: 3840x1920, LD: 1280x640, Tile: 320x320<br>
     * {<br>
     *  "rtc.fov_config":{<br>
     *      "mode":0,   //Only `0` is available for now. `0` for Equirectangular Projection(ERP).<br>
     *      "hd_width":3840,    //Width of the HD field.<br>
     *      "hd_height":1920,   //Height of the HD field<br>
     *      "ld_width":1280,    //Width of the background<br>
     *      "ld_height":640,    //Height of the background<br>
     *      "tile_width":320,   //Width of a tile<br>
     *      "tile_height":320,  //Height of a tile<br>
     *      "framerate":30, // Frame rate in fps<br>
     *      "max_kbps":40000} // Expected encoding bitrate in kbps<br>
     *}
     * @return  API call result: <br>
     *        + 0: Success <br>
     *        + ! 0: Failure <br>
     * @notes  <br>
     *        + Call this API to set encoding configurations for the panoramic video and designate an external video source. The format of the video can be YUV or Texture.<br>
     *        + Receivers get the video frames and decoding configurations via onFrame{@link #IVideoSink#onFrame} and pass them to the external renderer.
     */
    virtual int setVideoEncoderConfig(const VideoEncoderConfig& encoder_config, const char* parameters) = 0;

    /** 
     * @type api
     * @region Video Management
     * @brief <span id="IRTCVideo-setvideoencoderconfig-2"></span> Video publisher call this API to set the configurations of each stream published, including resolution, frame rate, bitrate, and fallback strategy in poor network conditions.
     * @param [in] channel_solutions List of configurations for multiple streams. You can set parameters for up to 3 streams, SDK will always take the value of the first 3 streams when parameters for more streams are set.
     *        The resolution you set is the maximum resolution of each stream, and must be arranged in descending order. The frame rates must be arranged in descending order. See VideoEncoderConfig{@link #VideoEncoderConfig}.
     * @param [in] solution_num Numbers of streams
     * @return  API call result: <br>
     *        + 0: Success <br>
     *        + ! 0: Failure <br>
     * @notes  <br>
     *        + If you call this API after enabling the mode of publishing multiple streams with enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode}, SDK will publish streams as you set, otherwise the SDK by default publishes the first stream that you have set. <br>
     *        + If you want to publish multiple streams with different resolutions, you need to call this method and enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode}.<br>
     *        + When the simulcast mode is turned on, it cannot be turned off dynamically, nor can the parameters for video encoding be updated.  <br>
     *        + Without calling this API to set parameters for multiple video streams, the SDK only publishes one video stream with a resolution of 640px × 360px and a frame rate of 15fps.  <br>
     *        + After setting parameters for multiple video streams, SDK will automatically match the streams to be published based on the desired subscription parameters set by subscribers, see [Simulcasting](https://docs.byteplus.com/en/byteplus-rtc/docs/70139) for details.  <br>
     *        + This API is applicable to the video stream captured by the camera, see setScreenVideoEncoderConfig{@link #IRTCVideo#setScreenVideoEncoderConfig} for setting parameters for screen sharing video stream.
     */
    virtual int setVideoEncoderConfig(const VideoEncoderConfig* channel_solutions, int solution_num) = 0;

    /** 
     * @type api
     * @region Screen Sharing
     * @brief  Set the configuration for shared-screen streams, including the resolution, frame rate, bitrate, and fallback strategies under challenging network conditions.
     * @param [in] screen_solution The configuration for shared-screen streams. See ScreenVideoEncoderConfig{@link #ScreenVideoEncoderConfig}.
     * @return  <br>
     *        + 0: Success. <br>
     *        + ! 0: Failure.
     * @notes We recommend that you set the encoding configuration before video capture. Otherwise, the video will be captured based on the default configuration(1080p@15fps).
     */
    virtual int setScreenVideoEncoderConfig(const ScreenVideoEncoderConfig& screen_solution) = 0;

    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use [setVideoEncoderConfig](#IRTCVideo-setvideoencoderconfig-2) instead.
     * @type api
     * @region  Video management
     * @brief  Sets the video encoder configuration of each individual stream for simulcasting, including resolution, frame rate, bitrate, scale mode, fallback strategy under poor network conditions, etc.
     * @param  [in] index Stream type. See StreamIndex{@link #StreamIndex}.
     * @param  [in] solutions    Video parameters of each individual stream for simulcasting. See VideoSolution{@link #VideoSolution}.
     *                           The maximum resolution is 4096px × 4096px. If the resolution exceeds the limit or the video cannot be encoded, the simulcasting will fail.
     * @param  [in] solution_num The length of `solutions`. <br>
     *                           The length of `solutions` is no more than 4. The resolutions must be in descending sort. <br>
     * @return   <br>
     *         + 0: Success <br>
     *         +! 0: Failure <br>
     * @notes   <br>
     *        + When using the internal capture, the resolution and frame rate will be adapted to the maximum resolution and frame rate for encoding. <br>
     *        + The video streams are encoded by default with resolution of 640px × 360px and frame rate of 15fps. <br>
     *        When you call this API, it will take effect immediately, which may cause the camera to restart. <br>
     *        + The screen stream takes the first set of parameters.
     */
    BYTERTC_DEPRECATED virtual int setVideoEncoderConfig(StreamIndex index, const VideoSolution* solutions, int solution_num) = 0;

    /** 
     * @type api
     * @region Video Management
     * @brief Sets the view to be used for local video rendering and the rendering mode.  <br>
     * @param [in] index Video stream type. See StreamIndex{@link #StreamIndex}.
     * @param [in] canvas View information and rendering mode. See VideoCanvas{@link #VideoCanvas}.
     * @return  <br>
     *        + 0: Success <br>
     *        + -1: Failure <br>
     * @notes  <br>
     *       + You should bind your stream to a view before joining the room. This setting will remain in effect after you leave the room. <br>
     *       + If you need to unbind the local video stream from the current view, you can call this API and set the videoCanvas to `null`.
     */
    virtual int setLocalVideoCanvas(StreamIndex index, const VideoCanvas& canvas) = 0;

    /** 
     * @type api
     * @region Video management
     * @brief Update the render mode and background color of local video rendering
     * @param [in] index See StreamIndex{@link #StreamIndex}.
     * @param [in] renderMode See RenderMode{@link #RenderMode}.
     * @param [in] backgroundColor See VideoCanvas{@link #VideoCanvas}.background_color.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes Calling this API during local video rendering will be effective immediately.
     */
    virtual int updateLocalVideoCanvas(StreamIndex index, const enum RenderMode renderMode, const uint32_t backgroundColor) = 0;

    /** 
     * @type api
     * @region Video Management
     * @brief Set the view and mode for rendering the specific video stream. <br>
     *        If you need to unbind the canvas for the video stream, set the canvas to `Null`.
     * @param  [in] stream_key See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param  [in] canvas canvas and rendering mode. See VideoCanvas{@link #VideoCanvas}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes When the local user leaves the room, the setting will be invalid. The remote user leaving the room does not affect the setting.
     */
    virtual int setRemoteVideoCanvas(RemoteStreamKey stream_key, const VideoCanvas& canvas) = 0;

    /** 
     * @deprecated since 3.56 on iOS and Android, and will be deleted in 3.62. Use [updateRemoteStreamVideoCanvas](70095#updateremotestreamvideocanvas-2) instead.
     * @type api
     * @region Video management
     * @brief Modifies remote video frame rendering settings, including render mode and background color.
     * @param [in] stream_key See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param [in] render_mode See RenderMode{@link #RenderMode}.
     * @param [in] background_color See VideoCanvas{@link #VideoCanvas}.background_color.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes Calling this API during remote video rendering will be effective immediately.
     */
    virtual int updateRemoteStreamVideoCanvas(RemoteStreamKey stream_key, const enum RenderMode render_mode, const uint32_t background_color) = 0;

    /** 
     * @hidden(Windows, macOS, Linux)
     * @valid since 3.56
     * @type api
     * @region Video management
     * @brief Modifies remote video frame rendering settings, including render mode, background color, and rotation angle, while using the internal rendering of the SDK.
     * @param stream_key Information about the remote stream. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param remote_video_render_config Video rendering settings. See RemoteVideoRenderConfig{@link #RemoteVideoRenderConfig}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Failure. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes
     *        + After setting the rendering configuration for the remote video frame with setRemoteVideoCanvas{@link #IRTCVideo#setRemoteVideoCanvas}, you can call this API to update settings including render mode, background color, and rotation angle.<br>
     *        + Calling this API during remote video rendering will be effective immediately.
     */
    virtual int updateRemoteStreamVideoCanvas(RemoteStreamKey stream_key, const RemoteVideoRenderConfig& remote_video_render_config) = 0;

    /** 
     * @type api
     * @region Custom Video Capturing & Rendering
     * @brief Binds the local video stream to a custom renderer.
     * @param [in] index Video stream type. See StreamIndex{@link #StreamIndex}.
     * @param [in] video_sink Custom video renderer. See IVideoSink{@link #IVideoSink}.
     * @param [in] required_format Video frame encoding format that applys to custom rendering. See PixelFormat{@link #PixelFormat}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *        + RTC SDK uses its own renderer (internal renderer) for video rendering by default.  <br>
     *        + Joining or leaving the room will not affect the binding state. <br>
     *        + If you need to unbind the video stream from the custom renderer, you must set video_sink to `null`.  <br>
     *        + You should call this API before joining the room, and after receiving onFirstLocalVideoFrameCaptured{@link #IRTCVideoEventHandler#onFirstLocalVideoFrameCaptured} which reports that the first local video frame has been successfully captured.
     */
    virtual int setLocalVideoSink(StreamIndex index, IVideoSink* video_sink, IVideoSink::PixelFormat required_format) = 0;

    /** 
     * @type api
     * @region Custom Video Capturing & Rendering
     * @brief Binds the remote video stream to a custom renderer.
     * @param [in] stream_key Remote stream information which specifys the source and type of the video stream to be rendered. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param [in] video_sink Custom video renderer. See IVideoSink{@link #IVideoSink}.
     * @param [in] required_format Encoding format which applys to the custom renderer. See PixelFormat{@link #PixelFormat}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + RTC SDK uses its own renderer (internal renderer) for video rendering by default.  <br>
     *        + Joining or leaving the room will not affect the binding state. <br>
     *         + This API can be called before and after entering the room. To call before entering the room, you need to get the remote stream information before joining the room; if you cannot get the remote stream information in advance, you can call the API after joining the room and getting the remote stream information via onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}.
     *         + If you need to unbind, you must set videoSink to null.
     */
    virtual int setRemoteVideoSink(RemoteStreamKey stream_key, IVideoSink* video_sink, IVideoSink::PixelFormat required_format) = 0;

    /** 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region Video management
     * @brief Toggle the front/postcondition camera used for internal video capture  <br>
     *        After calling this interface, onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} callback will be triggered locally.
     * @param   [in] camera_id See CameraID{@link #CameraID}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + Default uses front-facing camera.
     *        + If you are using the camera for video capture, the toggle operation will take effect immediately; if the camera is not activated, the set camera will be turned on when the internal capture is turned on later.
     *        + If you have multiple cameras locally and want to select a specific working camera, you can control it through IVideoDeviceManager{@link #IVideoDeviceManager}. <br>
     */
    virtual int switchCamera(CameraID camera_id) = 0;

    /** 
     * @type api
     * @region Screen sharing
     * @brief Push screen video frame.
     * @param  [in] frame Set the screen video frame, see: IVideoFrame{@link #IVideoFrame}. <br>
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes   <br>
     *        + Only video frames in YUV420P format are supported for the time being. <br>
     *        + This function runs in the user calling thread. Before destroying the IRTCVideo instance, please stop calling this function to push screen shared data <br>
     */
    virtual int pushScreenVideoFrame(IVideoFrame* frame) = 0;

    /** 
     * @hidden(Linux,iOS,Android)
     * @type api
     * @region screen sharing
     * @brief When the resolution of the external shared-screen stream changes, you can call this API to set the original pixel and framerate to help RTC(in the automatic mode) recommend these configurations.
     * @param [in] original_capture_width The original width of shared-screen streams.
     * @param [in] original_capture_height The original height of shared-screen streams.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *        + Before calling this API，you are advised to call setScreenVideoEncoderConfig{@link #IRTCVideo#setScreenVideoEncoderConfig} to set the encoding configurations: set the encoding mode to the automatic mode, the width and height to 0, the maximum bitrate to -1, and the minimum bitrate to 0.
     *        + After calling this API，you will receive onExternalScreenFrameUpdate{@link #IRTCVideoEventHandler#onExternalScreenFrameUpdate} to re-capture the stream based on the recommended pixel and framerate by RTC.
     */
    virtual int setOriginalScreenVideoInfo(int original_capture_width, int original_capture_height) = 0;

    /** 
     * @hidden(Android,iOS)
     * @type api
     * @region Screen sharing
     * @brief Update the capture area when capturing screen video streams through the capture module provided by the RTC SDK. Only used when the capture source is the monitor screen.
     * @param [in] region_rect Region to be shared. See Rectangle{@link #Rectangle} <br>
     *                         This parameter describes the region to be shared after calling this API, and the relative relationship between the `source_info` setting area in startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes Before calling this interface, internal screen stream capture must have been turned on by calling startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture}.
     */
    virtual int updateScreenCaptureRegion(const Rectangle& region_rect) = 0;

    /** 
     * @hidden(Android,iOS)
     * @type api
     * @region  screen sharing
     * @brief  Capture screen video stream for sharing. Screen video stream includes: content displayed on the screen, in the application window, or in the virtual screen. <br>
     *        Displaying contents in virtual screens is only available on Windows.
     * @param  [in] source_info Screen capture source information. See ScreenCaptureSourceInfo{@link #ScreenCaptureSourceInfo}. <br>
     *                          Call getScreenCaptureSourceList{@link #IRTCVideo#getScreenCaptureSourceList} to get all the screen sources that can be shared.
     * @param  [in] capture_params Screen capture parameters. See ScreenCaptureParameters{@link #ScreenCaptureParameters}.
     * @return   <br>
     *         + 0: Success; <br>
     *         + -1: Failure; <br>
     * @notes   <br>
     *        + The call of this API takes effects only when you are using RTC SDK to record screen. You will get a warning by onVideoDeviceWarning{@link #IRTCVideoEventHandler#onVideoDeviceWarning} after calling this API when the video source is set to an external recorder.<br>
     *        + This API only starts screen capturing but does not publish the captured video. Call publishScreen{@link #IRTCRoom#publishScreen} to publish the captured video. <br>
     *        + To turn off screen video capture, call stopScreenVideoCapture{@link #IRTCVideo#stopScreenVideoCapture}. <br>
     *        + Local users will receive onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} on the state of screen capturing such as start, pause, resume, and error. <br>
     *        + After successfully calling this API, local users will receive onFirstLocalVideoFrameCaptured{@link #IRTCVideoEventHandler#onFirstLocalVideoFrameCaptured}. <br>
     *        + Before calling this API, you can call setScreenVideoEncoderConfig{@link #IRTCVideo#setScreenVideoEncoderConfig} to set the frame rate and encoding resolution of the screen video stream. <br>
     *        + After receiving onFirstLocalVideoFrameCaptured{@link #IRTCVideoEventHandler#onFirstLocalVideoFrameCaptured}, you can set the local screen sharing view by calling setLocalVideoCanvas{@link #IRTCVideo#setLocalVideoCanvas} or setLocalVideoSink {@link #setLocalVideoSink}. <br>
     *        + You can also register an observer by calling setLocalVideoSink{@link #IRTCVideo#setLocalVideoSink} and receive the captured screen video frame through onFrame{@link #IVideoSink#onFrame}.<br>
     *        + For Windows SDK, after you start capturing screen video stream for sharing，you can call updateScreenCaptureHighlightConfig{@link #IRTCVideo#updateScreenCaptureHighlightConfig} to update border highlighting settings, updateScreenCaptureMouseCursor{@link #IRTCVideo#updateScreenCaptureMouseCursor} to update the processing settings for the mouse, and updateScreenCaptureFilterConfig{@link #IRTCVideo#updateScreenCaptureFilterConfig} to set the window that needs to be filtered.<br>
     */
    virtual int startScreenVideoCapture(const ScreenCaptureSourceInfo& source_info, const ScreenCaptureParameters& capture_params) = 0;

    /** 
     * @hidden(Android,iOS)
     * @type api
     * @region screen sharing
     * @brief Stop screen video streaming.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + This API can only stop the screen capture by the RTC SDK. If the video source has been set to external recorder, the call of this API will fail. You need to stop it in the external recorder. <br>
     *        + The call of this API takes effects only when you are using RTC SDK to record screen. You will get a warning by onVideoDeviceWarning{@link #IRTCVideoEventHandler#onVideoDeviceWarning} after calling this API when the video source is set to an external recorder.<br>
     *        + To turn on screen video stream capture, calling startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture} <br>
     *        + Local users will receive the onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} callback. <br>
     *        + Calling this interface does not affect screen video stream publishing.
     */
    virtual int stopScreenVideoCapture() = 0;

    /** 
     * @hidden(Android,iOS)
     * @type api
     * @region Screen sharing
     * @brief Update border highlighting settings when capturing screen video streams through the capture module provided by the RTC SDK. The default display table box.
     * @param  [in] highlight_config Border highlighting settings. See HighlightConfig{@link #HighlightConfig}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *        + Before calling this interface, you must have turned on internal screen flow collection by calling startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture}. <br>
     *        + For Linux with X11 protocal, the API is valid; for Linux with Wayland, the API is not valid.
     */
    virtual int updateScreenCaptureHighlightConfig(const HighlightConfig& highlight_config) = 0;

    /** 
     * @hidden(Android,iOS)
     * @type api
     * @region Screen sharing
     * @brief Update the processing settings for the mouse when capturing screen video streams through the capture module provided by the RTC SDK. Default acquisition mouse.
     * @param  [in] capture_mouse_cursor See MouseCursorCaptureState{@link #MouseCursorCaptureState}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  Before calling this interface, internal screen stream capture must have been turned on by calling startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture}.
     */
    virtual int updateScreenCaptureMouseCursor(MouseCursorCaptureState capture_mouse_cursor) = 0;

    /** 
     * @hidden(Linux,iOS,Android)
     * @type api
     * @region Screen sharing
     * @brief When capturing screen video streams through the capture module provided by the RTC SDK, set the window that needs to be filtered.
     * @param [in] filter_config Window filtering settings. See ScreenFilterConfig{@link #ScreenFilterConfig} <br>
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + Before calling this interface, internal screen stream capture must have been turned on by calling startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture}. <br>
     *        + This function only works when the screen source category is a screen rather than an application form. See ScreenCaptureSourceType{@link #ScreenCaptureSourceType}. <br>
     */
    virtual int updateScreenCaptureFilterConfig(const ScreenFilterConfig& filter_config) = 0;

    /** 
     * @hidden(Android,iOS)
     * @brief Get the item list for screen sharing.
     * @return List of the screen-sharing objects, including application window and screens. See IScreenCaptureSourceList{@link #IScreenCaptureSourceList}. After deciding which item to be shared, the user can share the object with startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture}.
     * @notes <br>
     *        + For Linux with X11 protocal, the API is valid; for Linux with Wayland, the API is not valid. For Linux with Wayland, when you call startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture}, the system pops a windows for the user to choose the object to share. <br>
     *        + Call release{@link #IScreenCaptureSourceList#release} to release the resources.
     */
    virtual IScreenCaptureSourceList* getScreenCaptureSourceList() = 0;

    /** 
     * @hidden(Android,iOS)
     * @brief Get the preview thumbnail of the screen-sharing object
     * @region  screen share
     * @param [in] type Type of the screen capture object. Refer to ScreenCaptureSourceType{@link #ScreenCaptureSourceType} for more details.
     * @param [in] source_id ID of the screen-shared object. Refer to view_t{@link #view_t} for more details.
     * @param [in] max_width Maximum width of the preview thumbnail. RTC will scale the thumbnail to fit the given size while maintaining the original aspect ratio. If the aspect ratio of the given size does not match the sharing object, the thumbnail will have blank borders.
     * @param [in] max_height Maximum height of the preview thumbnail. Refer to the note for `max_width`.
     * @return Preview thumbnail of the screen-sharing object Refer to IVideoFrame{@link #IVideoFrame} for more details.
     * @notes For Linux with X11 protocal, the API is valid; for Linux with Wayland, the API is not valid.
     */
    virtual IVideoFrame* getThumbnail(
            ScreenCaptureSourceType type, view_t source_id, int max_width, int max_height) = 0;
    /** 
     * @hidden(Android,iOS)
     * @brief Get application window preview thumbnail for screen sharing.
     * @param source_id ID of the screen-sharing object. You can get the ID by calling getScreenCaptureSourceList{@link #IRTCVideo#getScreenCaptureSourceList}. Refer to view_t{@link #view_t} for more details.
     * @param max_width Maximum width of the App icon. The width is always equal to the height. SDK will set the height and width to the smaller value if the given values are unequal. RTC will return nullptr if you set the value with a number out of the valid range, [32, 256]. The default size is 100 x 100.
     * @param max_height Maximum height of the app icon. Refer to the note for  `max_width`
     * @return Refer to IVideoFrame{@link #IVideoFrame} for more details. You can get the icon when the item to be shared is an application and the application is assigned with an icon. If not, the return value will be nullptr.
     * @notes For Linux with X11 protocal, the API is valid; for Linux with Wayland, the API is not valid.
     */
    virtual IVideoFrame* getWindowAppIcon(view_t source_id, int max_width = 100, int max_height = 100) = 0;

    /** 
     * @type api
     * @region Video management
     * @brief Set the video source, including the screen recordings.
     *        The internal video capture is the default, which refers to capturing video using the built-in module.
     * @param  [in] stream_index Stream index. Refer to StreamIndex{@link #StreamIndex} for more details.
     * @param  [in] type Video source type. Refer to VideoSourceType{@link #VideoSourceType} for more details.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *         + You can call this API whether the user is in a room or not. <br>
     *         + Calling this API to switch to the custom video source will stop the enabled internal video capture.
     *         + To switch to internal video capture, call this API to stop custom capture and then call startVideoCapture{@link #IRTCVideo#startVideoCapture} to enable internal video capture.
     *         + To push custom encoded video frames to the SDK, call this API to switch `VideoSourceType` to `VideoSourceTypeEncodedWithAutoSimulcast` or `VideoSourceTypeEncodedWithoutAutoSimulcast`.
     */
    virtual int setVideoSourceType(StreamIndex stream_index, VideoSourceType type) = 0;

    /** 
     * @type api
     * @region  Video Management
     * @brief Pushes external video frames.
     * @param [in] frame Set the video frame. See IVideoFrame{@link #IVideoFrame}.
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes  <br>
     *        + Support for I420, NV12, RGBA, BGRA, and ARGB.<br>
     *        + This function runs in the user calling thread. <br>
     *        + Before pushing external video frames, you must call setVideoSourceType{@link #setVideoSourceType} to turn on external video source capture.
     */
    virtual int pushExternalVideoFrame(IVideoFrame* frame) = 0;

    /** 
     * @hidden(Windows,Linux,macOS)
     * @deprecated since 3.45 and will be deleted in 3.51, use setAudioRoute{@link #IRTCVideo#setAudioRoute} instead on Android and iOS.
     * @type api
     * @region Audio Facility Management
     * @brief Set the audio playback device, and use speakers by default.   <br>
     * @param [in] device Audio playback device. See AudioPlaybackDevice{@link #AudioPlaybackDevice} <br>
     * @return Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     * @notes   <br>
     *        + This interface is only available for mobile devices. <br>
     *        + This method only supports setting audio & video playback devices as earphones or speakers. When the app connects to a wired or Bluetooth audio playback device, the SDK automatically switches to a wired or Bluetooth audio playback device. Active setting as a wired or Bluetooth audio playback device will return Failure. <br>
     *        + If you connect a wired or Bluetooth audio playback device, setting the audio playback device as a speaker or earpiece will successfully call, but it will not immediately switch to the speaker or earpiece. After the wired or Bluetooth audio playback device is removed, it will automatically switch to the earpiece or speaker according to the settings.  <br>
     *        + This method can be called before and during a call. <br>
     */
    BYTERTC_DEPRECATED virtual int setAudioPlaybackDevice(AudioPlaybackDevice device) = 0;

    /** 
     * @hidden(Windows,Linux,macOS)
     * @type api
     * @region Audio Facility Management
     * @brief Set the audio playback device. The audio route set by setDefaultAudioRoute{@link #IRTCVideo#setDefaultAudioRoute} by default.
     *         When the audio playback device changes, you will receive an onAudioRouteChanged{@link #IRTCVideoEventHandler#onAudioRouteChanged} Callback.
     * @param  [in] device Audio playback device. See AudioRoute{@link #AudioRoute}. You can only set to earpiece or speakerphone.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + You can implement most scenarios by calling setDefaultAudioRoute{@link #IRTCVideo#setDefaultAudioRoute} and the default audio route switching strategy of the RTC SDK. For details about the strategy, see [Set the Audio Route](https://docs.byteplus.com/byteplus-rtc/docs/117836). You should use this API only in a few exceptional scenarios like manually switching audio route with external audio device connected.
     *       + This interface is only supported in the `AUDIO_SCENARIO_COMMUNICATION` audio scenario. Call setAudioScenario{@link #IRTCVideo#setAudioScenario} to switch between different audio scenarios.
     */
    virtual int setAudioRoute(AudioRoute route) = 0;

    /** 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region Audio Facility Management
     * @brief Set the speaker or earpiece as the default audio playback device.   <br>
     * @param  [in] route Audio playback device. Refer to AudioRoute{@link #AudioRoute} <br>
     * @return <br>
     *         + 0: Success. The setting takes effect immediately. However, the audio route will not switch to the default device until all the audio peripheral devices are disconnected. <br>
     *         + < 0: failure. It fails when the device designated is neither a speaker nor an earpiece.
     * @notes For the audio route switching strategy of the RTC SDK, see [Set the Audio Route](https://docs.byteplus.com/byteplus-rtc/docs/117836).
     */
    virtual int setDefaultAudioRoute(AudioRoute route) = 0;

    /** 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region Audio Facility Management
     * @brief get current using audio playback device.   <br>
     *         When the audio playback device changes, you will receive an onAudioRouteChanged{@link #IRTCVideoEventHandler#onAudioRouteChanged} Callback.
     * @return  device current using Audio playback device. See AudioRoute{@link #AudioRoute}
     * @notes   <br>
     *        + 1. This interface is only available for mobile devices. <br>
     *        + 2. This method can be called before and during a call. <br>
     */
    virtual AudioRoute getAudioRoute() = 0;
    /** 
     * @type api
     * @region Room Management
     * @brief Create a room instance.<br>
     *        This API only returns a room instance. You still need to call joinRoom{@link #IRTCRoom#joinRoom} to actually create/join the room.<br>
     *        Each call of this API creates one IRTCRoom{@link #IRTCRoom} instance. Call this API as many times as the number of rooms you need, and then call joinRoom{@link #IRTCRoom#joinRoom} of each IRTCRoom instance to join multiple rooms at the same time.<br>
     *        In multi-room mode, a user can subscribe to media streams in the joined rooms at the same time.
     * @param [in] room_id The string matches the regular expression: `[a-zA-Z0-9_@\-\.]{1,128}`.
     * @return IRTCRoom{@link #IRTCRoom} instance.
     * @notes <br>
     *       + If the room that you wish to join already exists, you still need to call this API first to create the IRTCRoom instance, and then call joinRoom{@link #IRTCRoom#joinRoom}.
     *       + Do not create multiple rooms with the same roomId, otherwise the newly created room instance will replace the old one.
     *       + To forward streams to the other rooms, call startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms} instead of enabling Multi-room mode.
     */
    virtual IRTCRoom* createRTCRoom(const char* room_id) = 0;

    /** 
     * @type api
     * @region Audio & Video Fallback
     * @brief Sets the fallback option for published audio & video streams.   <br>
     *        You can call this API to set whether to automatically lower the resolution you set of the published streams under limited network conditions.
     * @param [in] option Fallback option, see PublishFallbackOption{@link #PublishFallbackOption}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *         + This API only works after you call enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode} to enable the mode of publishing multiple streams. <br>
     *         + You must call this API before entering the room.  <br>
     *         + After you allow video stream to fallback, your stream subscribers will receive onSimulcastSubscribeFallback{@link #IRTCVideoEventHandler#onSimulcastSubscribeFallback} when the resolution of your published stream are lowered or restored.  <br>
     *         + You can alternatively set fallback options with distrubutions from server side, which is of higher priority.
     */
    virtual int setPublishFallbackOption(PublishFallbackOption option) = 0;
    /** 
     * @type api
     * @region Audio & Video Fallback
     * @brief Sets the fallback option for subscribed RTC streams.   <br>
     *        You can call this API to set whether to lower the resolution of currently subscribed stream under limited network conditions.
     * @param [in] option Fallback option, see SubscribeFallbackOption{@link #SubscribeFallbackOption} for more details.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     *  @notes  <br>
     *         + You must call this API before enterting the room.  <br>
     *         + After you enables the fallback, you will receive onSimulcastSubscribeFallback{@link #IRTCVideoEventHandler#onSimulcastSubscribeFallback} and onRemoteVideoSizeChanged{@link #IRTCVideoEventHandler#onRemoteVideoSizeChanged} when the resolution of your subscribed stream is lowered or restored.  <br>
     *         + You can alternatively set fallback options with distrubutions from server side, which is of higher priority.
     */
    virtual int setSubscribeFallbackOption(SubscribeFallbackOption option) = 0;

    /** 
     * @type api
     * @region  audio & video fallback
     * @brief  Set user priority
     * @param  [in] room_id Room ID
     * @param  [in] user_id Remote user's ID
     * @param  [in] priority Remote user's requirement priority. See enumeration type RemoteUserPriority{@link #RemoteUserPriority}
     * @return 0:  Success. <br>
     *         + < 0: failure <br>
     * @notes   <br>
     *         + 1. This method is used with setSubscribeFallbackOption{@link #IRTCVideo#setSubscribeFallbackOption}. <br>
     *         + 2. If the subscription flow fallback option is turned on, weak connections or insufficient performance will give priority to ensuring the quality of the flow received by high-priority users. <br>
     *         + 3. This method can be used before and after entering the room, and the priority of the remote user can be modified. <br>
     */
    virtual int setRemoteUserPriority(const char* room_id, const char* user_id, RemoteUserPriority priority) = 0;

    /** 
     * @type api
     * @region Engine management
     * @brief  Sets the business ID   <br>
     *         You can use businessId to distinguish different business scenarios. You can customize your businessId to serve as a sub AppId, which can share and refine the function of the AppId, but it does not need authentication.
     * @param  [in] business_id <br>
     *         Your customized businessId
     *         BusinessId is a tag, and you can customize its granularity.
     * @return   <br>
     *         + 0: Success <br>
     *         + < 0: Failure. See BusinessCheckCode{@link #BusinessCheckCode} for specific reasons. <br>
     *         + -6001: The user is already in the room. <br>
     *         + -6002: The input is invalid. Legal characters include all lowercase letters, uppercase letters, numbers, and four other symbols, including '.', '-','_', and '@'.
     * @notes   <br>
     *         + You must call this API before the joinRoom{@link #IRTCRoom#joinRoom} API, otherwise it will be invalid.
     */
    virtual int setBusinessId(const char* business_id) = 0;

    /** 
     * @type api
     * @hidden(Windows,macOS,Linux)
     * @brief Set the orientation of the video capture. By default, the App direction is used as the orientation reference.<br>
     *        During rendering, the receiving client rotates the video in the same way as the sending client did.<br>
     * @param [in] rotation_mode Rotation reference can be the orientation of the App or gravity. Refer to VideoRotationMode{@link #VideoRotationMode} for details.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + The orientation setting is effective for internal video capture only. That is, the orientation setting is not effective to the custom video source or the screen-sharing stream.
     *        + If the video capture is on, the setting will be effective once you call this API. If the video capture is off, the setting will be effective on when capture starts.
     */
    virtual int setVideoRotationMode(VideoRotationMode rotation_mode) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Video management
     * @brief Sets the mirror mode for the captured video stream.
     * @param [in] mirror_type Mirror type. See MirrorType{@link #MirrorType}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + Switching video streams does not affect the settings of the mirror type. <br>
     *        + This API is not applicable to screen-sharing streams. <br>
     *        + When using an external renderer, you can set `mirrorType` to `0` and `3`, but you cannot set it to `1`. <br>
     *        + Before you call this API, the initial states of each video stream are as follows: <br>
     *        <table>
     *           <tr><th></th><th>Front-facing camera</th><th>Back-facing camera</th><th>Custom capturing</th><th>Built-in camera</th></tr>
     *           <tr><td>Mobile device</td><td>The preview is mirrored. The published video stream is not mirrored.</td><td>The preview and the published video stream are not mirrored.</td><td>The preview and the published video stream are not mirrored.</td><td>/</td></tr>
     *           <tr><td>PC</td><td>/</td><td>/</td><td>The preview and the published video stream are not mirrored.</td><td>The preview is mirrored. The published video stream is not mirrored.</td></tr>
     *        </table>
     */
    virtual int setLocalVideoMirrorType(MirrorType mirror_type) = 0;

    /** 
     * @type api
     * @region Audio & Video Processing
     * @brief Gets video effect interfaces.
     * @return Video effect interfaces. See IVideoEffect{@link #IVideoEffect}.
     */
    virtual IVideoEffect* getVideoEffectInterface() = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Audio & Video Processing
     * @brief Enables/Disables basic beauty effects.
     * @param [in] enable Whether to enable basic beauty effects. <br>
     *        + true: Enables basic beauty effects. <br>
     *        + false: (Default) Disables basic beauty effects. <br>
     * @return  <br>
     *        + 0: Success.
     *        + –1000: The Effect SDK is not integrated.
     *        + –1001: This API is not available for your current RTC SDK.
     *        + –1002: This API is not available for your current Effect SDK. You can upgrade your Effect SDK to v4.4.2+.
     *        + –1003: Contact our technical support team for further instructions.
     *        + –1004: Downloading related resources. The beauty effects will take effect after downloading.
     *        + <0: Failure. Effect SDK internal error. For specific error code, see [Error Code Table](https://docs.byteplus.com/effects/docs/error-code-table).
     * @notes <br>
     *        + You cannot use the basic beauty effects and the advanced effect features at the same time. See [how to use advanced effect features](https://docs.byteplus.com/byteplus-rtc/docs/114717) for more information.
     *        + You need to integrate Effect SDK before calling this API. Effect SDK v4.4.2+ is recommended.
     *        + Call setBeautyIntensity{@link #IRTCVideo#setBeautyIntensity} to set the beauty effect intensity. If you do not set the intensity before calling this API, the default intensity will be enabled. The default values for the intensity of each beauty mode are as follows: 0.7 for brightning, 0.8 for smoothing, 0.5 for sharpening, and 0.7 for clarity.
     *        + This API is not applicable to screen capturing.
     */
    virtual int enableEffectBeauty(bool enable) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Audio & Video Processing
     * @brief Sets the beauty effect intensity.
     * @param [in] beauty_mode Basic beauty effect. See EffectBeautyMode{@link #EffectBeautyMode}.
     * @param [in] intensity Beauty effect intensity in range of [0,1]. When you set it to 0, the beauty effect will be turned off.
     *                       The default values for the intensity of each beauty mode are as follows: 0.7 for brightning, 0.8 for smoothing, 0.5 for sharpening, and 0.7 for clarity.
     * @return  <br>
     *        + 0: Success.
     *        + –1000: The Effect SDK is not integrated.
     *        + –1001: This API is not available for your current RTC SDK.
     *        + <0: Failure. Effect SDK internal error. For specific error code, see [error codes](https://docs.byteplus.com/effects/docs/error-code-table).
     * @notes <br>
     *        + If you call this API before calling enableEffectBeauty{@link #IRTCVideo#enableEffectBeauty}, the default settings of beauty effect intensity will adjust accordingly. <br>
     *        + If you destroy the engine, the beauty effect settings will be invalid.
     */
    virtual int setBeautyIntensity(EffectBeautyMode beauty_mode, float intensity) = 0;

    /** 
     * @hidden not available
     * @type api
     * @region Audio & Video Processing
     * @brief Sets the super resolution mode for remote video stream.
     * @param [in] stream_key Remote stream information that specifies the source and type of the video stream. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param [in] mode Super resolution mode. See VideoSuperResolutionMode{@link #VideoSuperResolutionMode}.
     * @return. <br>
     *        + 0: kReturnStatusSuccess. It does not indicate the actual status of the super resolution mode, you should refer to onRemoteVideoSuperResolutionModeChanged{@link #IRTCVideoEventHandler#onRemoteVideoSuperResolutionModeChanged} callback.
     *        + -1: kReturnStatusNativeInValid. Native library is not loaded.
     *        + -2: kReturnStatusParameterErr. Invalid parameter.
     *        + -9: kReturnStatusScreenNotSupport. Failure. Screen stream is not supported.
     *        See ReturnStatus{@link #ReturnStatus} for more return value indications.
     * @notes <br>
     *        + Call this API after joining room.
     *        + The original resolution of the remote video stream should not exceed 640 × 360 pixels.
     *        + You can only turn on super-resolution mode for one stream.
     */
    virtual int setRemoteVideoSuperResolution(RemoteStreamKey stream_key, VideoSuperResolutionMode mode) = 0;

    /** 
     * @hidden not available
     * @type api
     * @region Audio & Video Processing
     * @brief Sets the video noise reduction mode.
     * @param [in] mode Video noise reduction mode which helps enhance video quality but will increase CPU utilization.
     * Refer to VideoDenoiseMode{@link #VideoDenoiseMode} for more details.
     * @return <br>
     *        + 0: Success. Please refer to onVideoDenoiseModeChanged{@link #IRTCVideoEventHandler#onVideoDenoiseModeChanged} callback for the actual state of video noise reduction mode.
     *        + < 0：Failure.
     */
    virtual int setVideoDenoiser(VideoDenoiseMode mode) = 0;

    /** 
     * @hidden(Windows,Linux,macOS)
     * @type api
     * @region Audio & Video Processing
     * @brief Sets the orientation of the video frame before custom video processing and encoding. The default value is `Adaptive`.
     *        You should set the orientation to `Portrait` when using video effects or custom processing.
     *        You should set the orientation to `Portrait` or `Landscape` when pushing a single stream to the CDN.
     * @param orientation Orientation of the video frame. See VideoOrientation{@link #VideoOrientation}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + The orientation setting is only applicable to internal captured video sources. For custom captured video sources, setting the video frame orientation may result in errors, such as swapping width and height. Screen sources do not support video frame orientation setting.  <br>
     *        + We recommend setting the orientation before joining room. The updates of encoding configurations and the orientation are asynchronous, therefore can cause a brief malfunction in preview if you change the orientation after joining room.
     */
    virtual int setVideoOrientation(VideoOrientation orientation) = 0;

    /** 
     * @hidden(macOS, Windows,Linux)
     * @type api
     * @region  video management
     * @brief  Get camera control interface
     * @return  Camera control interface pointer, see ICameraControl{@link #ICameraControl}.
     */
    virtual ICameraControl* getCameraControl() = 0;

    /** 
     * @type api
     * @region Encryption
     * @brief Sets the way to use built-in encryption when transmitting.
     * @param encrypt_type Built-in encryption algorithm. See EncryptType{@link #EncryptType}
     * @param  [in] encrypt_type Encryption type. See EncryptType{@link #EncryptType}
     * @param  [in] key Encryption key, the length is limited to 36 bits, and the excess will be Truncate
     * @param  [in] key_size The length of the parameter key
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + Use this method when using the built-in encryption on transfer; if you need to use custom encryption on transfer. See onEncryptData {@link #IEncryptHandler#onEncryptData}. Built-in encryption and custom encryption are mutually exclusive, and the transmission is determined to be encrypted according to the last method called. <br>
     *        + This method must be called before entering the room, and can be called repeatedly, taking the last called parameter as the effective parameter. <br>
     */
    virtual int setEncryptInfo(EncryptType encrypt_type, const char* key, int key_size) = 0;

    /** 
     * @type api
     * @region Encryption
     * @brief Sets custom encryption and decryption methods.
     * @param  [in] handler Custom encryption handler, you need to implement the handler's encryption and decryption method. See IEncryptHandler{@link #IEncryptHandler}. <br>
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + This method is mutually exclusive with setEncryptInfo{@link #IRTCVideo#setEncryptInfo}, you can only select custom encryption method or default encryption method. The encryption method that takes effect depends on the last method called. <br>
     *        + This method must be called before entering the room, and can be called repeatedly, taking the last called parameter as the effective parameter. <br>
     *        + Whether encrypted or decrypted, the length of the modified data needs to be controlled under 180%. That is, if the input data is 100 bytes, the processed data must be less than 180 bytes. If the encryption or decryption result exceeds the limit, the audio & video frame may be discarded. <br>
     *        + Data encryption/decryption is performed serially, so depending on the implementation, it may affect the final rendering efficiency. Whether to use this method needs to be carefully evaluated by users.
     */
    virtual int setCustomizeEncryptHandler(IEncryptHandler* handler) = 0;

    /** 
     * @type api
     * @region Audio Data Callback
     * @brief Enable audio frames callback and set the format for the specified type of audio frames.
     * @param [in] method Audio data callback method. See AudioFrameCallbackMethod{@link #AudioFrameCallbackMethod}. <br>
     *               If `method` is set as `kRecord`, `kPlayback`, `kMixed`, `kRecordScreen`, set `format` to the accurate value listed in the audio parameters format.
     *               If `method` is set as `kRemoteUser`, set `format` to `auto`.
     * @param [in] format Audio parameters format. See AudioFormat{@link #AudioFormat}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes After calling this API and registerAudioFrameObserver{@link #IRTCVideo#registerAudioFrameObserver}, IAudioFrameObserver{@link #IAudioFrameObserver} will receive the corresponding audio data callback. However, these two APIs are independent of each other and the calling order is not restricted. <br>
     */
    virtual int enableAudioFrameCallback(AudioFrameCallbackMethod method, AudioFormat format) = 0;

    /** 
     * @type api
     * @region Audio Data Callback
     * @brief Disables audio data callback.
     * @param [in] method Audio data callback method. See AudioFrameCallbackMethod{@link #AudioFrameCallbackMethod}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes Call this API after calling enableAudioFrameCallback{@link #IRTCVideo#enableAudioFrameCallback}.
     */
    virtual int disableAudioFrameCallback(AudioFrameCallbackMethod method) = 0;

    /** 
     * @type api
     * @region Audio Data Callback
     * @brief Register an audio frame observer.  <br>
     * @param [in] observer Audio data callback observer. See IAudioFrameObserver{@link #IAudioFrameObserver}. Use `null` to cancel the registration.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes After calling this API and enableAudioFrameCallback{@link #IRTCVideo#enableAudioFrameCallback}, IAudioFrameObserver{@link #IAudioFrameObserver} receives the corresponding audio data callback. You can retrieve the audio data and perform processing on it without affecting the audio that RTC SDK uses to encode or render.
     */
    virtual int registerAudioFrameObserver(IAudioFrameObserver* observer) = 0;

    /** 
     * @type api
     * @brief Register a custom audio preprocessor. <br>
     *        After that, you can call enableAudioProcessor{@link #IRTCVideo#enableAudioProcessor} to process the audio streams that either captured locally or received from the remote side. RTC SDK then encodes or renders the processed data.
     * @param [in] processor Custom audio processor. See IAudioFrameProcessor{@link #IAudioFrameProcessor}。<br>
     *        SDK only holds weak references to the processor, you should guarantee its Life Time. To cancel registration, set the parameter to nullptr.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes 
     * + When this interface is repeatedly called, only the last call takes effect.
     * + Refer to [Custom Audio Processing](https://docs.byteplus.com/en/byteplus-rtc/docs/80635) for more information.
     */
    virtual int registerAudioProcessor(IAudioFrameProcessor* processor) = 0;

    /** 
     * @type api
     * @brief Enable audio frames callback for custom processing and set the format for the specified type of audio frames.
     * @param  [in] method The types of audio frames. See AudioProcessorMethod{@link #AudioProcessorMethod}. Set this parameter to process multiple types of audio. <br>
     *        With different values, you will receive the corresponding callback: <br>
     *        + For locally captured audio, you will receive onProcessRecordAudioFrame{@link #IAudioFrameProcessor#onProcessRecordAudioFrame}. <br>
     *        + For mixed remote audio, you will receive onProcessPlayBackAudioFrame{@link #IAudioFrameProcessor#onProcessPlayBackAudioFrame}.  <br>
     *        + For audio from remote users, you will receive onProcessRemoteUserAudioFrame{@link #IAudioFrameProcessor#onProcessRemoteUserAudioFrame}.  <br>
     *        + For shared-screen audio, you will receive onProcessScreenAudioFrame{@link #IAudioFrameProcessor#onProcessScreenAudioFrame}. (Only on Windows)
     * @param  [in] format The format of audio frames. See AudioFormat{@link #AudioFormat}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + Before calling this API, call registerAudioProcessor{@link #IRTCVideo#registerAudioProcessor} to register a processor. <br>
     *        + To disable custom audio processing, call disableAudioProcessor{@link #IRTCVideo#disableAudioProcessor}.
     */
    virtual int enableAudioProcessor(AudioProcessorMethod method, AudioFormat format) = 0;

    /** 
     * @type api
     * @brief Disable custom audio processing.
     * @param [in] method Audio Frame type. See AudioProcessorMethod{@link #AudioProcessorMethod}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     */
    virtual int disableAudioProcessor(AudioProcessorMethod method) = 0;

    /** 
     * @hidden
     * @deprecated since 3.50 and will be deleted in 3.55, use setLocalVideoSink{@link #IRTCVideo#setLocalVideoSink} instead.
     * @type api
     * @region Video Data Callback
     * @brief Register a video data callback observer
     * @param [in] observer Video data callback observer, see IVideoFrameObserver{@link #IVideoFrameObserver}. Cancel the registration by setting it to `null`.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes We recommended you call this API before calling joinRoom{@link #IRTCRoom#joinRoom}.
     */
    BYTERTC_DEPRECATED virtual int registerVideoFrameObserver(IVideoFrameObserver* observer) = 0;

    /** 
     * @type api
     * @region  Video Processing
     * @brief Set up a custom video preprocessor. <br>
     *        Using this video preprocessor, you can call processVideoFrame{@link #IVideoProcessor#processVideoFrame} to preprocess the video frames collected by the RTC SDK, and use the processed video frames for RTC audio & video communication.
     * @param [in] processor Custom video processor. See IVideoProcessor{@link #IVideoProcessor}. If null is passed in, the video frames captured by the RTC SDK are not preprocessed. <br>
     *        SDK only holds weak references to the processor, you should guarantee its Life Time.
     * @param [in] config Customize the settings applicable to the video pre-processor. See VideoPreprocessorConfig{@link #VideoPreprocessorConfig}. <br>
     *             Currently, the 'required_pixel_format 'in'config' only supports: 'kVideoPixelFormatI420' and'kVideoPixelFormatUnknown ': <br>
     *             + When set to'kVideoPixelFormatUnknown', the RTC SDK gives the format of the video frame for processing by the processor, that is, the format of the acquisition.
     *               You can get the actual captured video frame format and pixel type by VideoFrameType{@link #VideoFrameType} and VideoPixelFormat{@link #VideoPixelFormat}. <br>
     *             + When set to'kVideoPixelFormatI420 ', the RTC SDK converts the captured video frames into the corresponding format for pre-processing. This method call fails when <br>
     *             + Is set to another value.
     * @return <br>
     *         + 0: Success. <br>
     *         +! 0: failure <br>
     * @notes <br>
     *        + For Windows platforms, the preprocessed video frame format returned only supports'kVideoPixelFormatI420 '<br>
     *        + For Windows platforms, setting the required_pixel_format in'config' to'kVideoPixelFormatI420 'can bring some performance optimization by avoiding format conversion. <br>
     *        + When this interface is repeatedly called, only the last call takes effect. The effects do not stack.
     */
    virtual int registerLocalVideoProcessor(IVideoProcessor* processor, VideoPreprocessorConfig config) = 0;
    /** 
     * @type api
     * @valid since 3.51
     * @brief Set the step size for each digital zooming control to the local videos.
     * @param type Required. Identifying which type the `size` is referring to. Refer to ZoomConfigType{@link #ZoomConfigType}. 
     * @param size Required. Reserved to three decimal places. It defaults to `0`. 
     *                  The meaning and range vary from different `type`s. If the scale or moving distance exceeds the range, the limit is taken as the result.
     *                  + `kZoomConfigTypeFocusOffset`: Increasement or decrease to the scaling factor. Range: [0, 7]. For example, when it is set to 0.5 and setVideoDigitalZoomControl{@link #IRTCVideo#setVideoDigitalZoomControl} is called to zoom in, the scale will increase `0.5`. The scale ranges [1，8] and defaults to `1`, which means an original size.
     *                  + `kZoomConfigTypeMoveOffset`：Ratio of the distance to the border of video images. It ranges [0, 0.5] and defaults to `0`, which means no offset. When you call setVideoDigitalZoomControl{@link #IRTCVideo#setVideoDigitalZoomControl} and choose `CAMERA_MOVE_LEFT`, the moving distance is size x original width. While for the `CAMERA_MOVE_UP`, the moving distance is size x original height. Suppose that a video spans 1080 px and the `size` is set to `0.5` so that the distance would be 0.5 x 1080 px = 540 px.
     * @return  <br>
     *        + 0: Success. <br>
     *        + ! 0: Failure.
     * @notes
     *        + Only one size can be set for a single call. You must call this API to pass values respectively if you intend to set multiple `size`s.
     *        + As the default `size` is `0`, you must call this API before performing any digital zoom control by calling setVideoDigitalZoomControl{@link #IRTCVideo#setVideoDigitalZoomControl} or startVideoDigitalZoomControl{@link #IRTCVideo#startVideoDigitalZoomControl}.
     */
    virtual int setVideoDigitalZoomConfig(ZoomConfigType type, float size) = 0;

    /** 
     * @type api
     * @valid since 3.51
     * @brief Digital zoom or move the local video image once. This action affects both the video preview locally and the stream published.
     * @param direction Action of the digital zoom control. Refer to ZoomDirectionType{@link #ZoomDirectionType}.
     * @return  <br>
     *        + 0: Success. <br>
     *        + ! 0: Failure.
     * @notes
     *        + As the default offset is `0`, you must call setVideoDigitalZoomConfig{@link #IRTCVideo#setVideoDigitalZoomConfig} before this API.
     *        + You can only move video images after they are magnified via this API or startVideoDigitalZoomControl{@link #IRTCVideo#startVideoDigitalZoomControl}.
     *        + When you request an out-of-range scale or movement, SDK will execute it with the limits. For example, when the image has been moved to the border, the image cannot be zoomed out, or has been magnified to 8x.
     *        + Call startVideoDigitalZoomControl{@link #IRTCVideo#startVideoDigitalZoomControl} to have a continuous and repeatedly digital zoom control.
     */
    virtual int setVideoDigitalZoomControl(ZoomDirectionType direction) = 0;

    /** 
     * @type api
     * @valid since 3.51
     * @brief Continuous and repeatedly digital zoom control. This action effect both the video preview locally and the stream published.
     * @param direction Action of the digital zoom control. Refer to ZoomDirectionType{@link #ZoomDirectionType}.
     * @return  <br>
     *        + 0: Success. <br>
     *        + ! 0: Failure.
     * @notes 
     *        + As the default offset is `0`, you must call setVideoDigitalZoomConfig{@link #IRTCVideo#setVideoDigitalZoomConfig} before this API.
     *        + You can only move video images after they are magnified via this API or setVideoDigitalZoomControl{@link #IRTCVideo#setVideoDigitalZoomControl}.
     *        + The control process stops when the scale reaches the limit, or the images have been moved to the border. if the next action exceeds the scale or movement range, SDK will execute it with the limits. 
     *        + Call stopVideoDigitalZoomControl{@link #IRTCVideo#stopVideoDigitalZoomControl} to stop the ongoing zoom control.
     *        + Call setVideoDigitalZoomControl{@link #IRTCVideo#setVideoDigitalZoomControl} to have a one-time digital zoom control.
     */
    virtual int startVideoDigitalZoomControl(ZoomDirectionType direction) = 0;

    /** 
     * @type api
     * @valid since 3.51
     * @brief Stop the ongoing digital zoom control instantly. 
     * @return  <br>
     *        + 0: Success. <br>
     *        + ! 0: Failure.
     * @notes Refer to startVideoDigitalZoomControl{@link #IRTCVideo#startVideoDigitalZoomControl} for starting digital zooming.
     */
    virtual int stopVideoDigitalZoomControl() = 0;

    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use registerAudioFrameObserver{@link #IRTCVideo#registerAudioFrameObserver} instead.
     * @type api
     * @region audio data callback
     * @brief Register an audio data callback observer.
     * @param [in] observer Audio data callback observer. See IRemoteAudioFrameObserver{@link #IRemoteAudioFrameObserver}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes Register the callback to receive PCM data from a single remote user.
     */
    BYTERTC_DEPRECATED virtual int registerRemoteAudioFrameObserver(IRemoteAudioFrameObserver* observer) = 0;

    /** 
     * @deprecated since 3.50 and will be deleted in 3.55, use [sendSEIMessage](#IRTCVideo-sendseimessage-2) instead.
     * @type api
     * @region Message
     * @brief <span id="IRTCVideo-sendseimessage-1"></span> Sends SEI data.  <br>
     *        In a video call scenario, SEI is sent with the video frame, while in a voice call scenario, SDK will automatically publish a black frame with a resolution of 16px × 16px to carry SEI data.
     * @param stream_index Specifys the type of media stream that carries SEI data. See StreamIndex{@link #StreamIndex}.  <br>
     *        In a voice call, you should set this parameter to `kStreamIndexMain`, otherwise the SEI data is discarded and cannot be sent to the remote user.
     * @param message SEI data
     * @param length SEI data length, no more than 4KB.
     * @param repeat_count Number of times a message is sent repeatedly. The value range is [0,30]. <br>
     *                    After calling this API, the SEI data will be added to a consecutive `repeat_count+1` number of video frames starting from the current frame.
     * @return  <br>
     *         + `>=0`: The number of SEIs to be added to the video frame <br>
     *         + `<0`: Failure
     * @notes  <br>
     *         + In a voice call scenario, this API can be called to send SEI data only in internal capture mode and with a frequency of 15/repeat_count FPS.  <br>
     *         + Each video frame carrys only the SEI data received within 2s before and after. In a voice call scenario, if no SEI data is sent within 1min after calling this API, SDK will automatically cancel publishing black frames.  <br>
     *         + After the message is sent successfully, the remote user who subscribed your video stream will receive onSEIMessageReceived{@link #IRTCVideoEventHandler#onSEIMessageReceived}.
     *         + When you switch from a voice call to a video call, SEI data will automatically start to be sent with normally captured video frames instead of black frames.
     */
    BYTERTC_DEPRECATED virtual int sendSEIMessage(StreamIndex stream_index, const uint8_t* message, int length, int repeat_count) = 0;

    /** 
     * @type api
     * @region Message
     * @brief <span id="IRTCVideo-sendseimessage-2"></span> Sends SEI data.
     *        In a video call scenario, SEI is sent with the video frame, while in a voice call scenario, SDK will automatically publish a black frame with a resolution of 16 × 16 pixels to carry SEI data.
     * @param [in] stream_index Specifys the type of media stream that carries SEI data. See StreamIndex{@link #StreamIndex}.  <br>
     *        In a voice call, you should set this parameter to `kStreamIndexMain`, otherwise the SEI data is discarded and cannot be sent to the remote user.
     * @param [in] message SEI data.
     * @param [in] length SEI data length. No more than 4 KB SEI data per frame is recommended.
     * @param [in] repeat_count Number of times a message is sent repeatedly. The value range is [0, max{29, %{video frame rate}-1}]. Recommended range: [2,4].<br>
     *                    After calling this API, the SEI data will be added to a consecutive `repeat_count+1` number of video frames starting from the current frame.
     * @param [in] mode SEI sending mode. See SEICountPerFrame{@link #SEICountPerFrame}.
     * @return  <br>
     *         + >= 0: The number of SEIs to be added to the video frame <br>
     *         + < 0: Failure
     * @notes  <br>
     *         + We recommend the number of SEI messages per second should not exceed the current video frame rate. In a voice call, the blank-frame rate is 15 fps.
     *         + In a voice call, this API can be called to send SEI data only in internal capture mode.
     *         + In a video call, the custom captured video frame can also be used for sending SEI data if the original video frame contains no SEI data, otherwise calling this method will not take effect.
     *         + Each video frame carrys only the SEI data received within 2s before and after. In a voice call scenario, if no SEI data is sent within 1min after calling this API, SDK will automatically cancel publishing black frames.
     *         + After the message is sent successfully, the remote user who subscribed your video stream will receive onSEIMessageReceived{@link #IRTCVideoEventHandler#onSEIMessageReceived}.
     *         + When you switch from a voice call to a video call, SEI data will automatically start to be sent with normally captured video frames instead of black frames.
     */
    virtual int sendSEIMessage(StreamIndex stream_index, const uint8_t* message, int length, int repeat_count, SEICountPerFrame mode) = 0;

    /** 
     * @valid since 3.56
     * @hidden
     * @type api
     * @region Message
     * @brief <span id="IRTCVideo-sendseimessage-2"></span> Public stream sends SEI data.
     * @param stream_index Specifys the type of media stream that carries SEI data. See StreamIndex{@link #StreamIndex}.  <br>
     * @param channel_id SEI message channel id. The value range is [0 - 255]. With this parameter, you can set different ChannelIDs for different recipients. In this way, different recipients can choose the SEI information  based on the ChannelID received in the callback.
     * @param message SEI data.
     * @param length SEI data length. No more than 4 KB SEI data per frame is recommended.
     * @param repeat_count Number of times a message is sent repeatedly. The value range is [0, max{29, %{video frame rate}-1}]. Recommended range: [2,4].<br>
     *                    After calling this API, the SEI data will be added to a consecutive `repeat_count+1` number of video frames starting from the current frame.
     * @param mode SEI sending mode. See SEICountPerFrame{@link #SEICountPerFrame}.
     * @return  <br>
     *         + < 0：Failure
     *         + = 0: You are unable to send SEI as the current send queue is full. <br>
     *         + > 0: Success, and the value represents the amount of sent SEI. 
     * @notes  <br>
     *         + We recommend the number of SEI messages per second should not exceed the current video frame rate.
     *         + In a video call, the custom captured video frame can also be used for sending SEI data if the original video frame contains no SEI data, otherwise calling this method will not take effect.
     *         + Each video frame carrys only the SEI data received within 2s before and after. In a voice call scenario, if no SEI data is sent within 1min after calling this API, SDK will automatically cancel publishing black frames.
     *         + After the message is sent successfully, the remote user who subscribed your video stream will receive onPublicStreamSEIMessageReceivedWithChannel{@link #IRTCVideoEventHandler#onPublicStreamSEIMessageReceivedWithChannel}.
     *         + When the call fails, neither the local nor the remote side will receive a callback.
     */
    virtual int sendPublicStreamSEIMessage(StreamIndex stream_index, int channel_id, const uint8_t* message, int length,
                                           int repeat_count, SEICountPerFrame mode) = 0;

    /** 
     * @type api
     * @region Video Facility Management
     * @brief Create a video Facility Management instance
     * @return Video Facility Management instance. See IVideoDeviceManager{@link #IVideoDeviceManager}
     */
    virtual IVideoDeviceManager* getVideoDeviceManager() = 0;

    /** 
     * @hidden(Android,iOS)
     * @type api
     * @region audio management
     * @brief Device audio management interface creation
     * @return Audio Facility Management interface
     */
    virtual IAudioDeviceManager* getAudioDeviceManager() = 0;

    /** 
     * @type api
     * @region Local recording
     * @brief This method records the audio & video data during the call to a local file.
     * @param [in] type Stream attribute, specify whether to record mainstream or screen stream. See StreamIndex{@link #StreamIndex}
     * @param [in] config Local recording parameter configuration. See RecordingConfig{@link #RecordingConfig}
     * @param [in] recording_type Local recording media type. See RecordingType{@link #RecordingType}.
     * @return   <br>
     *         + 0: normal
     *         + -1: Parameter setting exception
     *         + -2: The current version of the SDK does not support this Feature, please contact technical support staff
     * @notes   <br>
     *         + After calling this method, you will receive an onRecordingStateUpdate{@link #IRTCVideoEventHandler#onRecordingStateUpdate} callback. <br>
     *         + If the recording is normal, the system will notify the recording progress through onRecordingProgressUpdate{@link #IRTCVideoEventHandler#onRecordingProgressUpdate} Callback every second.
     */
    virtual int startFileRecording(StreamIndex type, RecordingConfig config, RecordingType recording_type) = 0;

    /** 
     * @type api
     * @region local recording
     * @brief Stop local recording
     * @param  [in] type Stream attribute, specify to stop mainstream or screen stream recording. See StreamIndex{@link #StreamIndex}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *         + Call startFileRecording{@link #IRTCVideo#startFileRecording} After starting local recording, you must call this method to stop recording. <br>
     *         + After calling this method, you will receive an onRecordingStateUpdate{@link #IRTCVideoEventHandler#onRecordingStateUpdate} callback prompting you to record the result.
     */
    virtual int stopFileRecording(StreamIndex type) = 0;

    /** 
     * @type api
     * @brief Starts recording audio communication, and generate the local file.<br>
     *        If you call this API before or after joining the room without internal audio capture, then the recording task can still begin but the data will not be recorded in the local files. Only when you call startAudioCapture{@link #IRTCVideo#startAudioCapture} to enable internal audio capture, the data will be recorded in the local files. 
     * @param [in] config See AudioRecordingConfig{@link #AudioRecordingConfig}.
     * @return  <br>
     *        + 0: `kReturnStatusSuccess`: Success <br>
     *        + -2: `kReturnStatusParameterErr`: Invalid parameters <br>
     *        + -3: `kReturnStatusWrongState`: Not valid in this SDK. Please contact the technical support.
     * @notes <br>
     *        + All audio effects are valid in the file. Mixed audio file is not included in the file. <br>
     *        + Call stopAudioRecording{@link #IRTCVideo#stopAudioRecording} to stop recording. <br>
     *        + You can call this API before and after joining the room. If this API is called before you join the room, you need to call stopAudioRecording{@link #IRTCVideo#stopAudioRecording} to stop recording. If this API is called after you join the room, the recording task ends automatically. If you join multiple rooms, audio from all rooms are recorded in one file.  <br>
     *        + After calling the API, you'll receive onAudioRecordingStateUpdate{@link #IRTCVideoEventHandler#onAudioRecordingStateUpdate}. <br>
     */
    virtual int startAudioRecording(AudioRecordingConfig& config) = 0;

    /** 
     * @type api
     * @brief Stop audio recording.
     * @return <br>
     *         + 0: Success <br>
     *         + <0: Failure
     * @notes Call startAudioRecording{@link #IRTCVideo#startAudioRecording} to start the recording task.
     */
    virtual int stopAudioRecording() = 0;

    /** 
     * @type api
     * @brief Enable the audio process mode for external sound card.
     * @param [in] enable <br>
     *        + true: enable <br>
     *        + false: disable (by default)
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *         + When you use external sound card for audio capture, enable this mode for better audio quality. <br>
     *         + When using the mode, you can only use earphones. If you need to use internal or external speaker, disable this mode.
     */
    virtual int enableExternalSoundCard(bool enable) = 0;

    /** 
     * @type api
     * @region Engine management
     * @brief Set runtime parameters
     * @param  [in] json_string String after json serialization
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes Call this API before joinRoom{@link #IRTCRoom#joinRoom} and startAudioCapture{@link #IRTCVideo#startAudioCapture}.
     */
    virtual int setRuntimeParameters(const char * json_string) = 0;

    /** 
     * @hidden currently not available
     * @type api
     * @region speech recognition service
     * @brief Enables automatic speech recognition service. This method converts the recognized user's voice into text and calls it back to the user through onMessage{@link #IRTCASREngineEventHandler#onMessage}.
     * @param  [in] asr_config Check information. See RTCASRConfig{@link #RTCASRConfig}
     * @param  [in] handler Speech recognition service usage status callback. See IRTCASREngineEventHandler{@link #IRTCASREngineEventHandler}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     */
    virtual int startASR(const RTCASRConfig& asr_config, IRTCASREngineEventHandler* handler) = 0;

    /** 
     * @hidden currently not available
     * @type api
     * @region speech recognition service
     * @brief Turn off speech recognition service
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     */
    virtual int stopASR() = 0;
    /** 
     * @type api
     * @region Engine management
     * @brief Report the user feedback to RTC.
     * @param [in] type List of preset problems. See ProblemFeedbackOption{@link #ProblemFeedbackOption}
     * @param [in] info Specific description of other problems other than the preset problem, and room's information. See ProblemFeedbackInfo{@link #ProblemFeedbackInfo}
     * @return  <br>
     *          + 0: Report successfully <br>
     *          + -1: Failed to report, not yet joined the room <br>
     *          + -2: Failed to report, data analysis error <br>
     *          + -3: Failed to report, missing fields <br>
     * @notes If the user is in the room when reporting, the report leads to the room / rooms where the user is currently located;
     *        If the user is not in the room when reporting, the report leads to the previously exited Room.
     */
    virtual int feedback(uint64_t type, const ProblemFeedbackInfo* info) = 0;
    /** 
     * @type api
     * @deprecated since 353.1, will be deleted in 359, use getAudioEffectPlayer{@link #IRTCVideo#getAudioEffectPlayer} or getMediaPlayer{@link #IRTCVideo#getMediaPlayer} instead
     * @region  mixing
     * @brief  Mixing management interface creation
     * @return  Mixing management instance. See IAudioMixingManager{@link #IAudioMixingManager}
     */
    virtual IAudioMixingManager* getAudioMixingManager() = 0;
    /** 
     * @type api
     * @valid since 3.53
     * @brief Create an instance for audio effect player.
     * @return See IAudioEffectPlayer{@link #IAudioEffectPlayer}.
     */
    virtual IAudioEffectPlayer* getAudioEffectPlayer() = 0;
    /** 
     * @type api
     * @valid since 3.53
     * @brief Create a media player instance.
     * @param player_id Media player id. The range is `[0, 3]`. You can create up to 4 instances at the same time. If it exceeds the range, nullptr will be returned.
     * @return Media player instance. See IMediaPlayer{@link #IMediaPlayer}.
     */
    virtual IMediaPlayer* getMediaPlayer(int player_id) = 0;
    
    /** 
     * @type api
     * @region Real-time messaging
     * @brief Log in to call sendUserMessageOutsideRoom{@link #IRTCVideo#sendUserMessageOutsideRoom} and sendServerMessage{@link #IRTCVideo#sendServerMessage} to send P2P messages or send messages to a server without joining the RTC room. <br>
     *        To log out, call logout{@link #IRTCVideo#logout}.
     * @param  [in] token <br>
     *        Token is required during login for authentication. <br>
     *        This Token is different from that required by calling joinRoom. You can assign any value even null to `roomId` to generate a login token. During developing and testing, you can use temporary tokens generated on the console. Deploy the token generating application on your server.
     * @param  [in] uid <br>
     *        User ID <br>
     *        User ID is unique within one appid.
     * @return <br>
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes  You will receive onLoginResult{@link #IRTCVideoEventHandler#onLoginResult} after calling this API and log in successfully. But remote users will not receive notification about that.
     */
    virtual int login(const char* token, const char* uid) = 0;

    /** 
     * @type After api
     * @region  real-time messaging
     * @brief  Calls this interface to log out, it is impossible to call methods related to out-of-room messages and end-to-server messages or receive related callbacks.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + Before calling this interface to log out, you must call login{@link #IRTCVideo#login} to log in. <br>
     *        + After local users call this method to log out, they will receive the result of the onLogout{@link #IRTCVideoEventHandler#onLogout} callback notification, and remote users will not receive the notification.
     */
    virtual int logout() = 0;

    /** 
     * @type api
     * @region Real-time messaging
     * @brief Update the Token <br>
     *        Token used by the user to log in has a certain valid period. When the Token expires, you need to call this method to update the login Token information. <br>
     *         When calling the login{@link #IRTCVideo#login} method to log in, if an expired token is used, the login will fail and you will receive an onLoginResult{@link #IRTCVideoEventHandler#onLoginResult} callback notification with an error code of kLoginErrorCodeInvalidToken. You need to reacquire the token and call this method to update the token.
     * @param  [in] token <br>
     *        Updated dynamic key
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + If the token is invalid and the login fails, call this method. After updating the token, the SDK will automatically log back in, and the user does not need to call the login{@link #IRTCVideo#login} method. <br>
     *        + Token expires, if you have successfully logged in, it will not be affected. An expired Token error will be notified to the user the next time you log in with an expired Token, or when you log in again due to a disconnection due to poor local network conditions.
     */
    virtual int updateLoginToken(const char* token) = 0;

    /** 
     * @type api
     * @region Real-time messaging
     * @brief Set application server parameters   <br>
     *        Client side calls sendServerMessage{@link #IRTCVideo#sendServerMessage} or sendServerBinaryMessage{@link #IRTCVideo#sendServerBinaryMessage} Before sending a message to the application server, a valid signature and application server address must be set.
     * @param [in] signature Dynamic signature. The App server may use the signature to verify the source of messages.<br>
     *        You need to define the signature yourself. It can be any non-empty string. It is recommended to encode information such as UID into the signature.<br>
     *        The signature will be sent to the address set through the "url" parameter in the form of a POST request.
     * @param [in] url The address of the application server
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + The user must call login{@link #IRTCVideo#login} to log in before calling this interface. <br>
     *        + After calling this interface, the SDK will use onServerParamsSetResult{@link #IRTCVideoEventHandler#onServerParamsSetResult} to return the corresponding result.
     */
    virtual int setServerParams(const char* signature, const char* url) = 0;

    /** 
     * @type api
     * @region Real-time messaging
     * @brief Query the login status of the opposite or local user
     * @param  [in] peer_user_id <br>
     *        User ID to be queried
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + login{@link #IRTCVideo#login} must be called before this interface can be called. <br>
     *        + After calling this interface, the SDK notifies the query result using the onGetPeerOnlineStatus{@link #IRTCVideoEventHandler#onGetPeerOnlineStatus} callback. <br>
     *        + Before sending an out-of-room message, the user can know whether the peer user is logged in through this interface to decide whether to send a message. You can also check your login status through this interface.
     */
    virtual int getPeerOnlineStatus(const char* peer_user_id) = 0;

    /** 
     * @type api
     * @region Real-time messaging
     * @brief Send a text message (P2P) to a specified user outside the room
     * @param  [in] uid User ID of the message receiver
     * @param  [in] message <br>
     *        Text message content sent <br>
     *        Message does not exceed 64 KB.
     * @param [in] config Message type, see MessageConfig{@link #MessageConfig}.
     * @return   <br>
     *         + > 0: Sent successfully, return the number of the sent message, increment from 1 <br>
     *         + -1: Sent failed, IRTCVideo instance not created <br>
     *         + -2: Sent failed, uid is empty
     * @notes   <br>
     *        + You must call login{@link #IRTCVideo#login} to complete the login before you can send a message in a foreign text of the room. <br>
     *        + After the user calls this interface to send a text message, he will receive an onUserMessageSendResultOutsideRoom{@link #IRTCVideoEventHandler#onUserMessageSendResultOutsideRoom} callback to know whether the message was successfully sent. <br>
     *        + If the text message is sent successfully, the user specified by uid receives the message via the onUserMessageReceivedOutsideRoom{@link #IRTCVideoEventHandler#onUserMessageReceivedOutsideRoom} callback.
     */
    virtual int64_t sendUserMessageOutsideRoom(const char* uid, const char* message, MessageConfig config = kMessageConfigReliableOrdered) = 0;

    /** 
     * @type api
     * @region Real-time messaging
     * @brief Send binary messages (P2P) to a specified user outside the room
     * @param [in] uid <br>
     *        ID of the user receiving the message
     * @param [in] length <br>
     *        Length of the binary string
     * @param [in] message <br>
     *        Content of the binary message sent <br>
     *        Message does not exceed 46KB.
     * @param [in] config Message type, see MessageConfig{@link #MessageConfig}.
     * @return   <br>
     *         + > 0: sent successfully, return the number of the sent message, increment from 1 <br>
     *         + -1: Sent failed, IRTCVideo instance not created <br>
     *         + -2: Sent failed, uid is empty
     * @notes   <br>
     *        + login{@link #IRTCVideo#login} must be called before sending out-of-room binary messages. <br>
     *        + After the user calls this interface to send a binary message, he will receive an onUserMessageSendResultOutsideRoom{@link #IRTCVideoEventHandler#onUserMessageSendResultOutsideRoom} callback to notify whether the message was sent successfully. <br>
     *        + If the binary message is sent successfully, the user specified by uid will receive the message through the onUserBinaryMessageReceivedOutsideRoom{@link #IRTCVideoEventHandler#onUserBinaryMessageReceivedOutsideRoom} callback.
     */
    virtual int64_t sendUserBinaryMessageOutsideRoom(const char* uid, int length, const uint8_t* message, MessageConfig config = kMessageConfigReliableOrdered) = 0;
    /** 
     * @type api
     * @region Real-time messaging
     * @brief The client side sends a text message to the application server (P2Server)
     * @param  [in] message <br>
     *        The content of the text message sent <br>
     *        The message does not exceed 64 KB.
     * @return   <br>
     *         + > 0: Sent successfully, return the number of the sent message, increment from 1 <br>
     *         + -1: Sent failed, IRTCVideo instance not created
     * @notes   <br>
     *        + Before sending a text message to the application server, you must first call login{@link #IRTCVideo#login} to complete the login, and then call setServerParams{@link #IRTCVideo#setServerParams} Set up the application server. <br>
     *        + After calling this interface, you will receive an onServerMessageSendResult{@link #IRTCVideoEventHandler#onServerMessageSendResult} callback to inform the message sender whether the message was sent successfully. <br>
     *        + If the text message is sent successfully, the application server that previously called setServerParams{@link #IRTCVideo#setServerParams} will receive the message.
     */
    virtual int64_t sendServerMessage(const char* message) = 0;
    /** 
     * @type api
     * @region Real-time messaging
     * @brief Client side sends binary messages to the application server (P2Server)
     * @param  [in] length <br>
     *        Length of binary string
     * @param  [in] message <br>
     *        Binary message content sent <br>
     *        Message does not exceed 46KB.
     * @return   <br>
     *         + > 0: Sent successfully, return the number of the sent message, increment from 1 <br>
     *         + -1: Sent failed, IRTCVideo instance not created
     * @notes   <br>
     *        + Before sending a binary message to the application server, you must first call login{@link #IRTCVideo#login} to complete the login, and then call setServerParams{@link #IRTCVideo#setServerParams} Set up the application server. <br>
     *        + After calling this interface, you will receive an onServerMessageSendResult{@link #IRTCVideoEventHandler#onServerMessageSendResult} callback to inform the message sender that the sending succeeded or failed. <br>
     *        + If the binary message is sent successfully, the application server that previously called setServerParams{@link #IRTCVideo#setServerParams} will receive the message.
     */
    virtual int64_t sendServerBinaryMessage(int length, const uint8_t* message) = 0;

    /** 
     * @type api
     * @region Pre-call network detection
     * @brief Pre-call network detection
     * @param  [in] is_test_uplink Whether to detect uplink bandwidth
     * @param  [in] expected_uplink_bitrate Expected uplink bandwidth, unit: kbps<br>Range: {0, [100-10000]}, `0`: Auto, that RTC will set the highest bite rate.
     * @param  [in] is_test_downlink Whether to detect downlink bandwidth
     * @param  [in] expected_downlink_biterate Expected downlink bandwidth, unit: kbps<br>Range: {0, [100-10000]}, `0`: Auto, that RTC will set the highest bite rate.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + After calling this interface, you will receive onNetworkDetectionResult{@link #IRTCVideoEventHandler#onNetworkDetectionResult} within 3s and every 2s thereafter notifying the probe result; <br>
     *        + If the probe stops, you will receive onNetworkDetectionStopped{@link #IRTCVideoEventHandler#onNetworkDetectionStopped} notify that probing has stopped.
     */
    virtual int startNetworkDetection(bool is_test_uplink, int expected_uplink_bitrate, bool is_test_downlink, int expected_downlink_biterate) = 0;

    /** 
     * @type api
     * @region Pre-call network probe
     * @brief Stop pre-call network probe
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + After calling this interface, you will receive onNetworkDetectionStopped{@link #IRTCVideoEventHandler#onNetworkDetectionStopped} notifying that the the probing has stopped.
     */
    virtual int stopNetworkDetection() = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Screen sharing
     * @brief Set the screen audio acquisition method (internal acquisition/custom acquisition)
     * @param  [in] source_type Screen audio input source type. See AudioSourceType{@link #AudioSourceType}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *       + The default acquisition method is internal to the RTC SDK Collect. <br>
     *       + You should call this method before publishScreen{@link #IRTCRoom#publishScreen}. Otherwise, you will receive an error from onWarning{@link #IRTCVideoEventHandler#onWarning}: 'kWarningCodeSetScreenAudioSourceTypeFailed' <br>
     *       + If it is set to internal collection, you must call startScreenAudioCapture{@link #IRTCVideo#startScreenAudioCapture} to start collection; <br>
     *       + If it is set to custom collection, you must call pushScreenAudioFrame{@link #IRTCVideo#pushScreenAudioFrame} to customize the collected screen audio frame Push to the RTC SDK. <br>
     *       + Whether it is an internal capture or a custom capture, you must call publishScreen{@link #IRTCRoom#publishScreen} to push the captured screen audio to the remote end.
     */
    virtual int setScreenAudioSourceType(AudioSourceType source_type) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Screen sharing
     * @brief Set the mixing mode of the screen audio stream and the audio stream collected by the microphone when the screen is shared
     * @param  [in] index The mixing mode. See StreamIndex{@link #StreamIndex} <br>
     *         + 'kStreamIndexMain': Mix the audio stream collected by the screen audio stream and the microphone <br>
     *         + 'KStreamIndexScreen ': By default, it divides  the screen audio stream and the audio stream collected by the microphone into two audio streams
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes You should call this method before publishScreen{@link #IRTCRoom#publishScreen}. Otherwise, you will receive an error from onWarning{@link #IRTCVideoEventHandler#onWarning} ': kWarningCodeSetScreenAudioStreamIndexFailed'
     */
    virtual int setScreenAudioStreamIndex(StreamIndex index) = 0;

    /** 
     * @hidden(iOS,Android,Linux)
     * @type api
     * @region Screen sharing
     * @brief Set the audio channel of the screen-sharing audio stream
     * @param  [in] channel The number of Audio channels. See AudioChannel{@link #AudioChannel}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes When you call setScreenAudioStreamIndex{@link #IRTCVideo#setScreenAudioStreamIndex} to mix the microphone audio stream and the screen-sharing audio stream, the audio channel is set by setAudioProfile{@link #IRTCVideo#setAudioProfile} rather than this API.
     */
    virtual int setScreenAudioChannel(AudioChannel channel) = 0;

    /** 
     * @hidden(Android,iOS,macOS)
     * @type api
     * @region screen sharing
     * @brief When sharing the screen, start using RTC SDK internal collection method to collect screen audio
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *         + The call of this API takes effects only when you are using RTC SDK to record screen. You will get a warning by onAudioDeviceWarning{@link #IRTCVideoEventHandler#onAudioDeviceWarning} after calling this API when the video source is set to an external recorder.<br>
     *         + After collection, you also need to call publishScreen{@link #IRTCRoom#publishScreen} to collect the screen audio Push to the far end. <br>
     *         + To turn off screen audio internal capture, call stopScreenAudioCapture{@link #IRTCVideo#stopScreenAudioCapture}.
     */
    virtual int startScreenAudioCapture() = 0;

    /** 
     * @hidden(Windows,Linux,Android,iOS)
     * @type api
     * @region screen sharing
     * @brief When sharing the screen, start using RTC SDK internal collection method to collect screen audio
     * @param [in] device_id ID of the virtual device
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *        + The call of this API takes effects only when you are using RTC SDK to record screen. You will get a warning by onAudioDeviceWarning{@link #IRTCVideoEventHandler#onAudioDeviceWarning} after calling this API when the video source is set to an external recorder.<br>
     *        + After collection, you also need to call publishScreen{@link #IRTCRoom#publishScreen} to collect the screen audio Push to the far end. <br>
     *        + To turn off screen audio internal capture, call stopScreenAudioCapture{@link #IRTCVideo#stopScreenAudioCapture}.
     */
    virtual int startScreenAudioCapture(const char device_id[MAX_DEVICE_ID_LENGTH]) = 0;

    /** 
     * @hidden(Android,iOS)
     * @type api
     * @region Screen sharing
     * @brief Stop RTC SDK's device audio recorder.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *         + The call of this API takes effects only when you are using RTC SDK to record screen. You will get a warning by onAudioDeviceWarning{@link #IRTCVideoEventHandler#onAudioDeviceWarning} after calling this API when the video source is set to an external recorder.<br>
     *        + This API can only stop the screen capture by the RTC SDK. If the video source has been set to external recorder, the call of this API will fail with a warning message. You need to stop it in the external recorder. <br>
     *        + To start the device audio recording, call startScreenAudioCapture{@link #IRTCVideo#startScreenAudioCapture}.
     */
    virtual int stopScreenAudioCapture() = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Screen Sharing
     * @brief Using a custom capture method, when capturing screen audio during screen sharing, push the audio frame to the RTC SDK for encoding and other processing.
     * @param  [in] frame Audio data frame. See IAudioFrame{@link #IAudioFrame}
     * @return  Method call result   <br>
     *         + 0: Setup succeeded <br>
     *         + < 0: Setup failed <br>
     * @notes   <br>
     *         + Before calling this interface to push custom collected audio data during screen sharing, you must call setScreenAudioSourceType{@link #IRTCVideo#setScreenAudioSourceType} Enable custom capture of screen audio. <br>
     *         + You should call this method every 10 milliseconds to push a custom captured audio frame. A push audio frame should contain frame.sample _rate/100 audio sample points. For example, if the sampling rate is 48000Hz, 480 sampling points should be pushed each time. <br>
     *         + The audio sampling format is S16. The data format in the audio buffer must be PCM data, and its capacity size should be samples × frame.channel × 2. <br>
     *         + After calling this interface to push the custom captured audio frame to the RTC SDK, you must call publishScreen{@link #IRTCRoom#publishScreen} to push the captured screen audio to the remote end. Audio frame information pushed to the RTC SDK is lost before calling publishScreen{@link #IRTCRoom#publishScreen}.
     */
    virtual int pushScreenAudioFrame(IAudioFrame* frame) = 0;

    /** 
     * @type api
     * @brief On the listener side, set all subscribed audio streams precisely timely aligned.
     * @param stream_key The remote audio stream used as the benchmark during time alignment. See RemoteStreamKey{@link #RemoteStreamKey}. <br>
     *                  You are recommended to use the audio stream from the lead singer. <br>
     *                  You must call this API after receiving onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}.
     * @param mode Whether to enable the alignment. Disabled by default. See AudioAlignmentMode{@link #AudioAlignmentMode}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + You must use the function when all participants set RoomProfileType{@link #RoomProfileType} to `kRoomProfileTypeChorus` when joining the room. <br>
     *        + All remote participants must call startAudioMixing{@link #IAudioMixingManager#startAudioMixing} to play background music and set `sync_progress_to_record_frame` of AudioMixingConfig{@link #AudioMixingConfig} to `true`. <br>
     *        + If the subscribed audio stream is delayed too much, it may not be precisely aligned. <br>
     *        + The chorus participants must not enable the alignment. If you wish to change the role from listener to participant, you should disable the alignment.
     */
    virtual int setAudioAlignmentProperty(const RemoteStreamKey& stream_key, AudioAlignmentMode mode) = 0;

    /** 
     * @hidden(macOS,Windows,Android,Linux)
     * @type api
     * @region screen sharing
     * @brief set extension configuration
     * @param group_id App and extension should belong to the same app group. Group ID needs to be passed in here
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes This function must be called immediately after the sharedenginewithappid function. If it is not called or the call time is late, local users will receive a callback to rtcEngine:onMediaDeviceStateChanged:deviceType:device_state:device_error:{@link #ByteRTCVideoDelegate#rtcEngine:onMediaDeviceStateChanged:deviceType:device_state:device_error:}. <br>
     *        Parameter device_state value is ByteRTCMediaDeviceStateStopped, device_error value is `ByteRTCMediaDeviceErrorNotFindGroupId`.
     */
    virtual int setExtensionConfig(const char* group_id) = 0;

    /** 
     * @hidden(macOS,Windows,Android,Linux)
     * @type api
     * @region  screen sharing
     * @brief send screen sharing extension program message
     * @param message:  Message content sent to extension program
     * @param size : size of message
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes This function must be called after the startScreenCapture function
     */
    virtual int sendScreenCaptureExtensionMessage(const char* message, size_t size) = 0;

    /** 
     * @hidden(macOS,Windows,Android,Linux)
     * @type api
     * @region  screen sharing
     * @brief  start local screen shared data collection
     * @param type screen shared data collection type
     * @param bundleId the bundle ID of the broadcast upload extension passed in, which is used to display the extension preferentially in settings
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes when the extension is started abnormally from the IOS control center, you don't need to call this function <br>
     *       local users will receive a callback to rtcEngine:onMediaDeviceStateChanged:deviceType:device_state:device_error:{@link #ByteRTCVideoDelegate#rtcEngine:onMediaDeviceStateChanged:deviceType:device_state:device_error:}. <br>
     *       Parameter  device_state value is ByteRTCMediaDeviceStateStarted, device_error value is ByteRTCMediaDeviceErrorOK
     */
    virtual int startScreenCapture(ScreenMediaType type, const char* bundle_id) = 0;

    /** 
     * @hidden(macOS,Windows,iOS,Linux)
     * @type api
     * @region  Screen sharing
     * @brief  Start to capture screen audio/video through the incoming Type and Context.
     * @param [in] type
     *        specifying the media type of screen capture，see ScreenMediaType{@link #ScreenMediaType}
     * @param  [in] context
     *        Android platform incoming Intent object, obtained by the business party after applying for system recording permission.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *        + After collection, you also need to call publishScreen {@link #IRtcRoom#publishScreen} to collect the screen audio/vieo Push to the far end. <br>
     *        + Enable internal screen audio/video capture, Android-specific interface.
     */
    virtual int startScreenCapture(ScreenMediaType type, void* context) = 0;

    /** 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region  screen sharing
     * @brief  stop local screen shared data collection
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     */
    virtual int stopScreenCapture() = 0;

    /** 
     * @deprecated since 3.52, will be deleted in 3.58, using startPushMixedStreamToCDN{@link #IRTCVideo#startPushMixedStreamToCDN} instead.
     * @hidden(Linux) not available
     * @type api
     * @region Multi-room
     * @brief Create a new task of pushing media streams to CDN and sets the relevant configurations.  <br>
     *        When pushing more than one live streams in the same task, SDK will first mix those streams into one single stream and then push it to CDN.
     * @param [in] task_id Task ID. The length should not exceed 126 bytes.
     *        You may want to push more than one mixed stream to CDN from the same room. When you do that, use different ID for corresponding tasks; if you will start only one task, use an empty string.
     * @param [in] param Configurations to be set when pushing streams to CDN. See ITranscoderParam{@link #ITranscoderParam}.
     * @param [in] observer Register this observer to receive callbacks from the SDK. See ITranscoderObserver{@link #ITranscoderObserver}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *       + Before calling this API，you need to enable Push to CDN on the [console](https://console.byteplus.com/rtc/workplaceRTC).  
     *       + After calling this API, you will be informed of the result and errors during the pushing process via the onStreamMixingEvent{@link #ITranscoderObserver#onStreamMixingEvent} callback.
     *       + Call stopLiveTranscoding{@link #IRTCVideo#stopLiveTranscoding} to stop pushing streams to CDN.
     */
    BYTERTC_DEPRECATED virtual int startLiveTranscoding(const char* task_id, ITranscoderParam* param, ITranscoderObserver* observer) = 0;

    /** 
     * @deprecated since 3.52, will be deleted in 3.58, using stopPushStreamToCDN{@link #IRTCVideo#stopPushStreamToCDN} instead.
     * @hidden(Linux) not available
     * @type api
     * @region Multi-room
     * @brief Stops pushing media streams to CDN. You will be informed of the change via the onStreamMixingEvent{@link #ITranscoderObserver#onStreamMixingEvent} callback. <br>
     *        For starting pushing media streams to CDN, see startLiveTranscoding{@link #IRTCVideo#startLiveTranscoding}.
     * @param [in] task_id Task ID. Specifys which pushing task you want to stop.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     */
    BYTERTC_DEPRECATED virtual int stopLiveTranscoding(const char* task_id) = 0;

    /** 
     * @deprecated since 3.52, will be deleted in 3.58, using updatePushMixedStreamToCDN{@link #IRTCVideo#updatePushMixedStreamToCDN} instead.
     * @hidden(Linux) not available
     * @type api
     * @region Multi-room
     * @brief Update parameters needed when pushing media streams to CDN.  You will be informed of the change via the onStreamMixingEvent{@link #ITranscoderObserver#onStreamMixingEvent} callback. <br>
     *        After calling startLiveTranscoding{@link #IRTCVideo#startLiveTranscoding} to enable the function of pushing streams to CDN, you can call this API to update the relevant configurations.
     * @param [in] task_id Task ID. Specifys of which pushing task you want to update the parameters.
     * @param [in] param Configurations that you want to update. See ITranscoderParam{@link #ITranscoderParam} for specific indications. You can update any property for the task unless it is specified as unavailable for updates.
     *                   If you left some properties blank, you can expect these properties to be set to their default values.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     */
    BYTERTC_DEPRECATED virtual int updateLiveTranscoding(const char* task_id, ITranscoderParam* param) = 0;

    /** 
     * @valid since 3.52
     * @hidden(Linux) not available
     * @type api
     * @region Multi-room
     * @brief Create a new task of pushing mixed media streams to CDN and sets the relevant configurations.  <br>
     *        When pushing more than one live streams in the same task, SDK will first mix those streams into one single stream and then push it to CDN.
     * @param [in] task_id Task ID. The length should not exceed 126 bytes.<br>
     *        You may want to push more than one mixed stream to CDN from the same room. When you do that, use different ID for corresponding tasks; if you will start only one task, use an empty string.
     * @param [in] config Configurations to be set when pushing streams to CDN. See IMixedStreamConfig{@link #IMixedStreamConfig}.
     * @param [in] observer Register this observer to receive callbacks from the SDK. See IMixedStreamObserver{@link #IMixedStreamObserver}.
     * @return <br>
     *        + 0: Success<br>
     *        + !0: Failure<br>
     * @notes   <br>
     *       + Before calling this API，you need to enable Push to CDN on the [console](https://console.byteplus.com/rtc/workplaceRTC).     
     *       + After calling this API, you will be informed of the result and errors during the pushing process via the onMixingEvent{@link #IMixedStreamObserver#onMixingEvent} callback.
     *       + If you have subscribed to the push-to-CDN server callback in the [console](https://console.byteplus.com/rtc/cloudRTC?tab=callback), calling this API will receive a [TranscodeStarted](https://docs.byteplus.com/en/byteplus-rtc/docs/75125#transcodestarted) server callback notification. When calling this API repeatedly, subsequent calls to this API will trigger both [TranscodeStarted](https://docs.byteplus.com/en/byteplus-rtc/docs/75125#transcodestarted) and [TranscodeUpdated](https://docs.byteplus.com/en/byteplus-rtc/docs/75125#transcodeupdated) callbacks.
     *       + Call stopPushStreamToCDN{@link #IRTCVideo#stopPushStreamToCDN} to stop pushing streams to CDN.
     */
    virtual int startPushMixedStreamToCDN(const char* task_id, IMixedStreamConfig* config, IMixedStreamObserver* observer) = 0;
    /** 
     * @valid since 3.52
     * @hidden(Linux) not available
     * @type api
     * @region Multi-room
     * @brief Update parameters needed when pushing mixed media streams to CDN.  You will be informed of the change via the onMixingEvent{@link #IMixedStreamObserver#onMixingEvent} callback. <br>
     *        After calling startPushMixedStreamToCDN{@link #IRTCVideo#startPushMixedStreamToCDN} to enable the function of pushing streams to CDN, you can call this API to update the relevant configurations.
     * @param [in] task_id Task ID. Specifys of which pushing task you want to update the parameters.
     * @param [in] config Configurations that you want to update. See IMixedStreamConfig{@link #IMixedStreamConfig} for specific indications. You can update any property for the task unless it is specified as unavailable for updates.<br>
     *        If you left some properties blank, you can expect these properties to be set to their default values.
     * @return <br>
     *        + 0: Success<br>
     *        + !0: Failure<br>
     */
    virtual int updatePushMixedStreamToCDN(const char* task_id, IMixedStreamConfig* config) = 0;
    /** 
     * @hidden(Linux)
     * @type api
     * @region Push to CDN
     * @brief Create a new task of pushing a single media stream to CDN.
     * @param task_id Task ID. <br>
     *        You may want to start more than one task to push streams to CDN. When you do that, use different IDs for corresponding tasks; if you will start only one task, use an empty string.
     * @param param Configurations for pushing a single stream to CDN. See PushSingleStreamParam{@link #PushSingleStreamParam}.
     * @param observer Register this observer to receive callbacks from the SDK. See IPushSingleStreamToCDNObserver{@link #IPushSingleStreamToCDNObserver}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *       + Before calling this API，you need to enable Push to CDN on the [console](https://console.byteplus.com/rtc/workplaceRTC).     
     *       + After calling this API, you will be informed of the result and errors during the pushing process with onStreamPushEvent{@link #IPushSingleStreamToCDNObserver#onStreamPushEvent}.
     *       + Call stopPushStreamToCDN{@link #IRTCVideo#stopPushStreamToCDN} to stop the task.
     *       + Since this API does not perform encoding and decoding, the video stream pushed to RTMP will change according to the resolution, encoding method, and turning off the camera of the end of pushing streams.
     */
    virtual int startPushSingleStreamToCDN(const char* task_id, PushSingleStreamParam& param, IPushSingleStreamToCDNObserver* observer) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Push to CDN
     * @brief Stops the task to push a single media stream to CDN. <br>
     *        This API can be used to stop both pushing single and mixed stream to CDN. You need to distinguish the tasks to be stopped by different "taskId".
     * @param task_id Task ID. Specifys the task you want to stop.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + To start pushing single stream to CDN, see startPushSingleStreamToCDN{@link #IRTCVideo#startPushSingleStreamToCDN}.
     *        + To start pushing mixed stream to CDN, see startPushMixedStreamToCDN{@link #IRTCVideo#startPushMixedStreamToCDN}.
     */
    virtual int stopPushStreamToCDN(const char* task_id) = 0;

    /** 
      * @hidden internal use only
      * @type api
      * @brief Start aligning RTC data by cache. Received RTC data from different sources will be cached, and aligned based on the included timestamps. This feature compromizes the real-time nature of RTC data consumption.
      * @param config See ChorusCacheSyncConfig{@link #ChorusCacheSyncConfig}.
      * @param observer Event and data observer. See IChorusCacheSyncObserver{@link #IChorusCacheSyncObserver}.
      * @return See ReturnStatus{@link #ReturnStatus}.
      * @notes To disable the feature, call stopChorusCacheSync{@link #IRTCVideo#stopChorusCacheSync}.
      */
    virtual int startChorusCacheSync(ChorusCacheSyncConfig * config, IChorusCacheSyncObserver* observer) = 0;
    /** 
      * @hidden internal use only
      * @type api
      * @brief Stop aligning RTC data by cache.
      * @return See ReturnStatus{@link #ReturnStatus}.
      */
    virtual int stopChorusCacheSync() = 0;
  
    /** 
     * @hidden currently not available
     * @type api
     * @brief Start publishing a public media stream.<br>
     *        A public stream refers to a media stream that does not belong to any room or any user. Users within the same `appID` can call startPlayPublicStream{@link #IRTCVideo#startPlayPublicStream} to subscribe to the public stream regardless the user has joined which room or has not joined any room.
     * @param public_stream_id ID of the public stream
     * @param param Properties of the public stream. Refer to IPublicStreamParam{@link #IPublicStreamParam} for more details.<br>
     *              A public stream can include a bundle of media streams and appears as the designated layout.
     * @return <br>
     *        + 0: Success. And you will be informed by onPushPublicStreamResult{@link #IRTCVideoEventHandler#onPushPublicStreamResult}. <br>
     *        + !0: Failure because of invalid parameter or empty parameters.<br>
     * @notes <br>
     *        + Call updatePublicStreamParam{@link #IRTCVideo#updatePublicStreamParam} to update the properties of the public stream which is published by the same user. Calling this API with the same stream ID repeatedly by the same user can not update the existing public stream.<br>
     *        + If Users with different userID call this API with the same stream ID, the public stream will be updated with the parameters passed in the latest call.<br>
     *        + To publish multiple public streams, call this API with different stream ID respectively.<br>
     *        + To stop publishing the public stream, call stopPushPublicStream{@link #IRTCVideo#stopPushPublicStream}.
     *        + Please contact ts to enable this function before using it.
     */
    virtual int startPushPublicStream(const char* public_stream_id, IPublicStreamParam* param) = 0;
    /** 
     * @hidden currently not available.
     * @type api
     * @brief Stop the public stream published by the current user.<br>
     *        Refer to startPushPublicStream{@link #IRTCVideo#startPushPublicStream} for details about starting publishing a public stream.
     * @param public_stream_id ID of the public stream<br>
     *                  The public stream must be published by the current user.
     * @return <br>
     *        + 0: Success<br>
     *        + !0: Failure<br>
     */
    virtual int stopPushPublicStream(const char* public_stream_id) = 0;

    /** 
     * @hidden currently not available.
     * @type api
     * @brief Update the properties of the public stream published by the current user.<br>
     *        Refer to startPushPublicStream{@link #IRTCVideo#startPushPublicStream} for details about starting publishing a public stream.
     * @param public_stream_id ID of the public stream<br>
     * @param param Properties of the public stream. Refer to IPublicStreamParam{@link #IPublicStreamParam} for more details.<br>
     *              The stream to be updated must be published by the current user.
     * @return <br>
     *        + 0: Success<br>
     *        + !0: Failure<br>
     * @notes Make sure the public stream has started successfully via onPushPublicStreamResult{@link #IRTCVideoEventHandler#onPushPublicStreamResult} before calling this API.
     */
    virtual int updatePublicStreamParam(const char* public_stream_id, IPublicStreamParam* param) = 0;


    /** 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region  screen sharing
     * @brief  update screen acquisition data type
     * @param type screen acquisition data type
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes This API must be called after the startScreenCapture function. Local users will receive a callback to rtcEngine:onMediaDeviceStateChanged:deviceType:device_state:device_error:{@link #ByteRTCVideoDelegate#rtcEngine:onMediaDeviceStateChanged:deviceType:device_state:device_error:}. <br>
     *        Parameter device_state value is ByteRTCMediaDeviceStateStarted or ByteRTCMediaDeviceStateStopped, device_error value is ByteRTCMediaDeviceErrorOK.
     */
    virtual int updateScreenCapture(ScreenMediaType type) = 0;

     /** 
      * @type api
      * @region Audio Management
      * @brief Enable audio information prompts. After that, you will receive onLocalAudioPropertiesReport{@link #IRTCVideoEventHandler#onLocalAudioPropertiesReport}, onRemoteAudioPropertiesReport{@link #IRTCVideoEventHandler#onRemoteAudioPropertiesReport}, and onActiveSpeaker{@link #IRTCVideoEventHandler#onActiveSpeaker}.
      * @param config See AudioPropertiesConfig{@link #AudioPropertiesConfig}
      * @return  <br>
      *        + 0: Success.
      *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
      */
    virtual int enableAudioPropertiesReport(const AudioPropertiesConfig& config) = 0;

    /** 
     * @type api
     * @region  multi-room
     * @brief Adjusts the audio playback volume of the specified remote user.
     * @param [in] room_id ID of the room from which the remote audio source is published.
     * @param [in] user_id  The remote user ID of the audio source
     * @param [in] volume Ratio(%) of playback volume to original volume, in the range [0, 400], with overflow protection.  <br>
     *                    This changes the volume property of the audio data other than the hardware volume.<br>
     *               To ensure the audio quality, we recommend setting the volume to `100`.  <br>
     *               + 0: mute <br>
     *               + 100: original volume. Default value. <br>
     *               + 400: Up to 4 times the original volume (with overflow protection)
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes Suppose a remote user A is always within the range of the target user whose playback volume will be adjusted,<br>
     *        + If you use both this method and setRemoteRoomAudioPlaybackVolume{@link #IRTCRoom#setRemoteRoomAudioPlaybackVolume}, the volume that the local user hears from user A is the volume set by the method called later.<br>
     *        + If you use both this method and setPlaybackVolume{@link #IRTCVideo#setPlaybackVolume}, the volume that the local user hears from user A is the overlay of both settings.
     */
    virtual int setRemoteAudioPlaybackVolume(const char* room_id,const char* user_id, int volume) = 0;

    /** 
     * @type api
     * @region Audio Management
     * @brief Enables/disables the loudness equalization function.  <br>
     *        If you call this API with the parameter set to True, the loudness of user's voice will be adjusted to -16lufs. If then you also call setAudioMixingLoudness{@link #IAudioMixingManager#setAudioMixingLoudness} and import the original loudness of the audio data used in audio mixing, the loudness will be adjusted to -20lufs when the audio data starts to play.
     * @param [in] enable Whether to enable loudness equalization function:  <br>
     *        + true: Yes <br>
     *        + false: No
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes You must call this API before starting to play the audio file with startAudioMixing{@link #IAudioMixingManager#startAudioMixing}.
     */
    virtual int enableVocalInstrumentBalance(bool enable) = 0;

    /** 
     * @type api
     * @region Audio Management
     * @brief Enables/disables the playback ducking function. This function is usually used in scenarios where short videos or music will be played simultaneously during RTC calls. <br>
     *        With the function on, if remote voice is detected, the local media volume will be lowered to ensure the clarity of the remote voice. If remote voice disappears, the local media volume restores.
     * @param enable Whether to enable playback ducking:  <br>
     *        + true: Yes  <br>
     *        + false: No
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     */
    virtual int enablePlaybackDucking(bool enable) = 0;

    /** 
     * @type api
     * @region Video Data Callback
     * @brief Register a local video frame observer.   <br>
     *        This method applys to both internal capturing and custom capturing.  <br>
     *        After calling this API, SDK triggers onLocalEncodedVideoFrame{@link #ILocalEncodedVideoFrameObserver#onLocalEncodedVideoFrame} whenever a video frame is captured.
     * @param [in] observer Local video frame observer. See ILocalEncodedVideoFrameObserver{@link #ILocalEncodedVideoFrameObserver}. You can cancel the registration by setting it to `nullptr`.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes You can call this API before or after entering the RTC room. Calling this API before entering the room ensures that video frames are monitored and callbacks are triggered as early as possible.
     */
    virtual int registerLocalEncodedVideoFrameObserver(ILocalEncodedVideoFrameObserver* observer) = 0;

    /** 
     * @type api
     * @region video management
     * @brief Video data callback after registering remote encoding.   <br>
     *         After registration, when the SDK detects a remote encoded video frame, it will trigger the onRemoteEncodedVideoFrame{@link #IRemoteEncodedVideoFrameObserver#onRemoteEncodedVideoFrame} callback
     * @param [in] observer Remote encoded video data monitor. See IRemoteEncodedVideoFrameObserver{@link #IRemoteEncodedVideoFrameObserver}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + See [Custom Video Encoding and Decoding](https://docs.byteplus.com/byteplus-rtc/docs/82921#custom-video-decoding) for more details about custom video decoding. <br>
     *        + This method applys to manual subscription mode and can be called either before or after entering the Room. It is recommended to call it before entering the room. <br>
     *        + The engine needs to be unregistered before it is destroyed. Call this method to set the parameter to nullptr.
     */
    virtual int registerRemoteEncodedVideoFrameObserver(IRemoteEncodedVideoFrameObserver* observer) = 0;

    /** 
     * @type api
     * @region Video management
     * @brief Register custom encoded frame push event callback
     * @param  [in] encoder_handler Custom encoded frame callback class. See IExternalVideoEncoderEventHandler{@link #IExternalVideoEncoderEventHandler}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + This method needs to be called before entering the room. <br>
     *        + The engine needs to be unregistered before it is destroyed. Call this method to set the parameter to nullptr.
     */
    virtual int setExternalVideoEncoderEventHandler(IExternalVideoEncoderEventHandler* encoder_handler) = 0;

    /** 
     * @type api
     * @region  video management
     * @brief Push custom encoded video stream
     * @param [in] index The attributes of the encoded stream that need to be pushed. See StreamIndex{@link #StreamIndex}
     * @param [in] video_index The corresponding encoded stream subscript, starting from 0, if you call setVideoEncoderConfig{@link #IRTCVideo#setVideoEncoderConfig} to set multiple streams, the number here must be consistent with it
     * @param [in] video_stream Encoded stream video frame information. See IEncodedVideoFrame{@link #IEncodedVideoFrame}.
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes  <br>
     *         + Currently, only video frames in H264 and ByteVC1 formats are supported, and the video stream protocol must be in an Annex B format. <br>
     *         + This function runs within the user calling thread <br>
     *         + Before pushing a custom encoded video frame, you must call setVideoSourceType{@link #IRTCVideo#setVideoSourceType} to switch the video input source to the custom encoded video source.
     */
    virtual int pushExternalEncodedVideoFrame(StreamIndex index, int video_index, IEncodedVideoFrame* video_stream) = 0;

    /** 
     * @type api
     * @region Video Management
     * @brief Before subscribing to the remote video stream, set the remote video data decoding method
     * @param  [in] key The remote stream information, that is, which video stream is decoded. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param  [in] config Video decoding method. See VideoDecoderConfig{@link #VideoDecoderConfig}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *         + When you want to custom decode a remote stream, you need to call registerRemoteEncodedVideoFrameObserver{@link #IRTCVideo#registerRemoteEncodedVideoFrameObserver} to register the remote video stream monitor, and then call the interface to set the decoding method to custom decoding. The monitored video data will be called back through onRemoteEncodedVideoFrame{@link #IRemoteEncodedVideoFrameObserver#onRemoteEncodedVideoFrame}.
     *         + Since version 3.56, for automatic subscription, you can set the `RoomId` and `UserId` of `key` as `nullptr`. In this case, the decoding settings set by calling the API applies to all remote main streams or screen sharing streams based on the `StreamIndex` value of `key`.
     */
    virtual int setVideoDecoderConfig(RemoteStreamKey key, VideoDecoderConfig config) = 0;

    /** 
     * @type api
     * @region Video Management
     * @brief After subscribing to the remote video stream, request the key frame to the remote
     * @param  [in] stream_info The remote stream information. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *         + This method is only suitable for manual subscription mode and is used after successful subscription to the remote flow. <br>
     *         + This method is suitable for calling setVideoDecoderConfig{@link #IRTCVideo#setVideoDecoderConfig} to turn on the custom decoding function, and the custom decoding fails
     */
    virtual int requestRemoteVideoKeyFrame(const RemoteStreamKey& stream_info) = 0;

    /** 
     * @type api
     * @region Audio management
     * @brief Send audio stream synchronization information. The message is sent to the remote end through the audio stream and synchronized with the audio stream. After the interface is successfully called, the remote user will receive a onStreamSyncInfoReceived{@link #IRTCVideoEventHandler#onStreamSyncInfoReceived} callback.
     * @param  [in] data Message content.
     * @param  [in] length Message length. Message length must be [1,16] bytes.
     * @param  [in] config For configuration of media stream information synchronization. See StreamSycnInfoConfig{@link #StreamSycnInfoConfig}.
     * @return   <br>
     *         + > = 0: Message sent successfully. Returns the number of successful sends. <br>
     *         + -1: Message sending failed. Message length greater than 16 bytes. <br>
     *         + -2: Message sending failed. The content of the incoming message is empty. <br>
     *         + -3: Message sending failed. This screen stream was not published when the message was synchronized through the screen stream. <br>
     *         + -4: Message sending failed. This audio stream is not yet published when you synchronize messages with an audio stream captured by a microphone or custom device, as described in ErrorCode{@link #ErrorCode}. <br>
     * @notes
     * + Regarding the frequency, we recommend no more than 50 calls per second.
     * + When using `kRoomProfileTypeInteractivePodcast` as room profile, the data will be delivered. For other coom profiles, the data may be lost when the local user is muted.
     */
    virtual int sendStreamSyncInfo(const uint8_t* data, int32_t length, const StreamSycnInfoConfig& config) = 0;

    /** 
     * @type api
     * @region Audio Mixing
     * @brief Change local voice to a different key, mostly used in Karaoke scenarios.  <br>
     *        You can adjust the pitch of local voice such as ascending or descending with this method.
     * @param [in] pitch The value that is higher or lower than the original local voice within a range from -12 to 12. The default value is 0, i.e. No adjustment is made.  <br>
     *        The difference in pitch between two adjacent values within the value range is a semitone, with positive values indicating an ascending tone and negative values indicating a descending tone, and the larger the absolute value set, the more the pitch is raised or lowered.  <br>
     *        Out of the value range, the setting fails and triggers onWarning{@link #IRTCVideoEventHandler#onWarning} callback, indicating `WARNING_CODE_SET_SCREEN_STREAM_INVALID_VOICE_PITCH` for invalid value setting with WarningCode{@link #WarningCode}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     */
    virtual int setLocalVoicePitch(int pitch) = 0;

    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use setPlaybackVolume{@link #IRTCVideo#setPlaybackVolume} instead.
     * @type api
     * @region Media stream management
     * @brief Play/Stop the local audio stream.
     * @param  [in] mute_state Playinging status to identify whether to play the local audio stream, see: MuteState {@link #MuteState}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes This method controls the local audio stream but does not affect the local audio playback device.
     */
    BYTERTC_DEPRECATED virtual int muteAudioPlayback(MuteState mute_state) = 0;

    /** 
     * @hidden currently not available.
     * @type api
     * @brief Subscribe the public stream<br>
     *        A user can call this method to subscribe a public stream whether he/she has joined the room or not.
     * @param [in] public_stream_id ID of the public stream. If the stream has not been published then, the local client will receive the public stream once it starts publishing.
     * @return <br>
     *        + 0: Success. You will also be informed by onPlayPublicStreamResult{@link #IRTCVideoEventHandler#onPlayPublicStreamResult}.  <br>
     *        + !0: Failure because of invalid parameter or empty parameters.<br>
     * @notes  <br>
     *        + One client can play up to 5 public streams simultaneously. Please call stopPlayPublicStream{@link #IRTCVideo#stopPlayPublicStream} timely to avoid exceeding the maximum number of subscribed public streams.
     *        + We recommend to bind a view for the public stream before calling this API to subscribe a public stream. <br>
     *              - Internal renderer: By calling setPublicStreamVideoCanvas{@link #IRTCVideo# setPublicStreamVideoCanvas} or <br>
     *              - Custom renderer: By calling setPublicStreamVideoSink{@link #IRTCVideo#setPublicStreamVideoSink}<br>
     *        + After calling this API, you will be informed once the first frame has been decoded successfully by onFirstPublicStreamVideoFrameDecoded{@link #IRTCVideoEventHandler#onFirstPublicStreamVideoFrameDecoded} and onFirstPublicStreamAudioFrame{@link #IRTCVideoEventHandler#onFirstPublicStreamAudioFrame}.<br>
     *        + If the public stream contains SEI information, you will be informed by onPublicStreamSEIMessageReceived{@link #IRTCVideoEventHandler#onPublicStreamSEIMessageReceived}.<br>
     */
    virtual int startPlayPublicStream(const char* public_stream_id) = 0;
    /** 
     * @hidden currently not available
     * @type api
     * @brief Cancel subscribing the public stream.<br>
     *        Call this method to cancel subscribing to the public stream by calling startPlayPublicStream{@link #IRTCVideo#startPlayPublicStream}.
     * @param [in] public_stream_id ID of the public stream
     * @return
     *        + 0: Success<br>
     *        + !0: Failure<br>
     */

    virtual int stopPlayPublicStream(const char* public_stream_id) = 0;
    /** 
     * @hidden currently not available
     * @type api
     * @brief Assign a internal render view to the public stream
     * @param [in] public_stream_id ID of the public stream
     * @param [in] canvas Internal render view. Refer to VideoCanvas{@link #VideoCanvas} for more details.
     * @return
     *        + 0: Success<br>
     *        + !0: Failure<br>
     */
    virtual int setPublicStreamVideoCanvas(const char* public_stream_id, const VideoCanvas& canvas) = 0;
    /** 
     * @hidden currently not available
     * @type api
     * @brief Assign a custom renderer to the public stream
     * @param [in] public_stream_id ID of the public stream
     * @param [in] video_sink Custom renderer. Set to be `null` when you want to release the renderer. Refer to IVideoSink{@link #IVideoSink} for more details.
     * @param [in] format Format of the video frames required by the external video renderer. Refer to PixelFormat{@link #PixelFormat} for more details.
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     */
    virtual int setPublicStreamVideoSink(const char* public_stream_id, IVideoSink* video_sink,
                                          IVideoSink::PixelFormat format) = 0;
    
    /** 
     * @type api
     * @valid since 3.51
     * @brief Set the audio playback volume of the public stream.
     * @param [in] public_stream_id ID of the public stream.
     * @param [in] volume Ratio(%) of the audio playback volume to the original volume, in the range `[0, 400]`, with overflow protection. The default volume is 100. <br>
     *               To ensure the audio quality, the recommended range is `[0,  100]`.  <br>
     * @return   <br>
     *         + 0: Success. <br>
     *         + -2: Wrong parameter.
     */
    virtual int setPublicStreamAudioPlaybackVolume(const char* public_stream_id, int volume) = 0;
    
    /** 
     * @type api
     * @region Audio & Video Processing
     * @brief Adds watermark to designated video stream.
     * @param [in] stream_index The index of the target stream. See StreamIndex{@link #StreamIndex}.
     * @param [in] image_path The absolute path of the watermark image. The path should be less than 512 bytes.
     *        The watermark image should be in PNG or JPG format.
     * @param [in] config Watermark configurations. See RTCWatermarkConfig{@link #RTCWatermarkConfig}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + Call clearVideoWatermark{@link #IRTCVideo#clearVideoWatermark} to remove the watermark from the designated video stream. <br>
     *        + You can only add one watermark to one video stream. The newly added watermark replaces the previous one. You can call this API multiple times to add watermarks to different streams. <br>
     *        + If you mirror the preview, or the preview and the published stream, the watermark will also be mirrored locally, but the published watermark will not be mirrored. <br>
     *        + When you enable simulcast mode, the watermark will be added to all video streams, and it will be scaled down to smaller encoding configurations accordingly. <br>
     */
    virtual int setVideoWatermark(StreamIndex stream_index, const char* image_path, RTCWatermarkConfig config) = 0;

    /** 
     * @type api
     * @region Audio & Video Processing
     * @brief  Removes video watermark from designated video stream.
     * @param  [in] stream_index The index of the target stream. See StreamIndex{@link #StreamIndex}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     */
    virtual int clearVideoWatermark(StreamIndex stream_index) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @brief Take a snapshot of the local video.
     * @param [in] stream_index See StreamIndex{@link #StreamIndex}.
     * @param [in] callback See ISnapshotResultCallback{@link #ISnapshotResultCallback}.
     * @return The index of the local snapshot task, starting from `1`.
     * @notes <br>
     *        + The snapshot is taken with all video effects on, like rotation, and mirroring. <br>
     *        + You can take the snapshot either using SDK internal video capture or customized capture.
     */
    virtual long takeLocalSnapshot(const StreamIndex stream_index, ISnapshotResultCallback* callback) = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @brief Take a snapshot of the remote video.
     * @param [in] stream_key See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param [in] callback See ISnapshotResultCallback{@link #ISnapshotResultCallback}.
     * @return The index of the remote snapshot task, starting from `1`.
     */
    virtual long takeRemoteSnapshot(const RemoteStreamKey stream_key, ISnapshotResultCallback* callback) = 0;

    /** 
     * @type api
     * @region Audio & Video Transport
     * @brief Set an alternative image when the local internal video capture is not enabled.
     *        When you call `stopVideoCapture`, an alternative image will be pushed. You can set the path to null or open the camera to stop publishing the image.
     *        You can repeatedly call this API to update the image.
     * @param [in] file_path Set the path of the static image.  <br>
     *        You can use the absolute path (file://xxx). The maximum size for the path is 512 bytes.  <br>
     *        You can upload a .JPG, .JPEG, .PNG, or .BMP file.  <br>
     *        When the aspect ratio of the image is inconsistent with the video encoder configuration, the image will be proportionally resized, with the remaining pixels rendered black. The framerate and the bitrate are consistent with the video encoder configuration.
     * @return  <br>
     *        + 0: Success.  <br>
     *        + -1: Failure.
     * @notes  <br>
     *        + The API is only effective when publishing an internally captured video.  <br>
     *        + You cannot locally preview the image.  <br>
     *        + You can call this API before and after joining an RTC room. In the multi-room mode, the image can be only displayed in the room you publish the stream.  <br>
     *        + You cannot apply effects like filters and mirroring to the image, while you can watermark the image.  <br>
     *        + The image is not effective for a screen-sharing stream.  <br>
     *        + When you enable the simulcast mode, the image will be added to all video streams, and it will be proportionally scaled down to smaller encoding configurations.<br>
     */
    virtual int setDummyCaptureImagePath(const char* file_path) = 0;

    /** 
     * @type api
     * @region  cloud proxy
     * @brief  Start cloud proxy
     * @param  [in] configuration cloud proxy informarion list. See CloudProxyConfiguration{@link #CloudProxyConfiguration}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *         + Call this API before joining the room.<br>
     *         + Start pre-call network detection after starting cloud proxy. <br>
     *         + After starting cloud proxy and connects the cloud proxy server successfully, receives onCloudProxyConnected{@link #IRTCVideoEventHandler#onCloudProxyConnected}. <br>
     *         + To stop cloud proxy, call stopCloudProxy{@link #IRTCVideo#stopCloudProxy}.
     */
    virtual int startCloudProxy(const CloudProxyConfiguration& configuration) = 0;

    /** 
     * @type api
     * @region  cloud proxy
     * @brief  Stop cloud proxy
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes To start cloud proxy, call startCloudProxy{@link #IRTCVideo#startCloudProxy}.
     */
    virtual int stopCloudProxy() = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Network Management
     * @brief Starts a call test.  <br>
     *        Before entering the room, you can call this API to test whether your local audio/video equipment as well as the upstream and downstream networks are working correctly.  <br>
     *        Once the test starts, SDK will record your sound or video. If you receive the playback within the delay range you set, the test is considered normal.
     * @param [in] echo_test_config Test configurations, see EchoTestConfig{@link #EchoTestConfig}.
     * @param [in] play_delay_time Delayed audio/video playback time specifying how long you expect to receive the playback after starting the. The range of the value is [2,10] in seconds and the default value is 2.
     * @return API call result:  <br>
     *        + 0: Success  <br>
     *        + -1: Failure, testing in progress  <br>
     *        + -2: Failure, you are in the room  <br>
     *        + -3: Failure, neither video nor audio is captured  <br>
     *        + -4: Failure, Parameter exception  <br>
     *        + -5: Failure, the roomID is already used
     * @notes  <br>
     *        + Once you start the test, you can either call stopEchoTest{@link #IRTCVideo#stopEchoTest} or wait until the test stops automatically after 60s, to start the next test or enter the room.  <br>
     *        + All APIs related to device control and stream control called before this API are invalidated during the test and are restored after the test.  <br>
     *        + All APIs related to device control, stream control, and room entry called during the test do not take effect, and you will receive onWarning{@link #IRTCVideoEventHandler#onWarning} with the warning code `kWarningCodeInEchoTestMode`.
     *        + You will receive the test result from onEchoTestResult{@link #IRTCVideoEventHandler#onEchoTestResult}.
     */
    virtual int startEchoTest(EchoTestConfig echo_test_config, unsigned int play_delay_time) = 0;
    /** 
     * @hidden(Linux)
     * @type api
     * @region Network Management
     * @brief Stop the current call test.  <br>
     *        After calling startEchoTest{@link #IRTCVideo#startEchoTest}, you must call this API to stop the test.
     * @return API call result:  <br>
     *        + 0: Success  <br>
     *        + -1: Failure, no test is in progress
     * @notes After stopping the test with this API, all the system devices and streams are restored to the state they were in before the test.
     */
    virtual int stopEchoTest() = 0;
    /** 
     * @hidden(Linux)
     * @type api
     * @brief Create a karaoke scoring management interface.
     * @return Karaoke scoring management interface, see ISingScoringManager{@link #ISingScoringManager}.
     */
    virtual ISingScoringManager* getSingScoringManager() = 0;
    /** 
     * @type api
     * @hidden(Linux)
     * @brief Obtain the synchronization network time information.
     * @return See NetworkTimeInfo{@link #NetworkTimeInfo}.
     * @notes <br>
     *        + When you call this API for the first time, you starts synchornizing the network time information and receive the return value `0`. After the synchonization finishes, you will receive onNetworkTimeSynchronized{@link #IRTCVideoEventHandler#onNetworkTimeSynchronized}. After that, calling this API will get you the correct network time. <br>
     *        + Under chorus scenario, participants shall start audio mixing at the same network time.
     */
    virtual NetworkTimeInfo getNetworkTimeInfo() = 0;
    /** 
     * @hidden internal use
     * @valid since 3.52
     * @type api
     * @brief invoke experimental API. <br>
     *        You may receive onInvokeExperimentalAPI{@link #IRTCVideoEventHandler#onInvokeExperimentalAPI}.
     * @notes [in] param JSON string like:
     * ```{
     *   "api_name":"startPublish",
     *   "params":{
     *     "streamID":"",
     *     "observer":"",
     *     "uri":"",
     *     "option":""
     *   }
     * }```
     * @return  <br>
     *        + 0: Success.  <br>
     *        + < 0: Failure.
     */
    virtual int invokeExperimentalAPI(const char* param) = 0;


    /** 
     * @hidden currently not available
     * @type api
     * @brief Creates the KTV manager interfaces.
     * @return KTV manager interfaces. See IKTVManager{@link #IKTVManager}.
     */
    virtual IKTVManager* getKTVManager() = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Audio Management
    * @brief Start echo detection before joining a room.
    * @param [in] test_audio_file_path Absolute path of the music file for the detection. It is expected to encode with UTF-8. The following files are supported: mp3, aac, m4a, 3gp, wav. <br>
    *          We recommend to assign a music file whose duration is between 10 to 20 seconds.<br>
    *        Do not pass a Silent file.
    * @return Method call result: <br>
    *        + 0: Success. <br>
    *        + -1: Failure due to the onging process of the previous detection. Call stopHardwareEchoDetection{@link #IRTCVideo#stopHardwareEchoDetection} to stop it before calling this API again.
    *        + -2: Failure due to an invalid file path or file format.
    * @notes <br>
    *        + You can use this feature only when RoomProfileType{@link #RoomProfileType} is set to `kRoomProfileTypeMeeting` or `kRoomProfileTypeMeetingRoom`.<br>
    *        + Before calling this API, ask the user for the permissions to access the local audio devices. <br>
    *        + Before calling this api, make sure the audio devices are activate and keep the capture volume and the playback volume within a reasonable range.<br>
    *        + The detection result is passed as the argument of onHardwareEchoDetectionResult{@link #IRTCVideoEventHandler#onHardwareEchoDetectionResult}. <br>
    *        + During the detection, the SDK is not able to response to the other testing APIs, such as startEchoTest{@link #IRTCVideo#startEchoTest}、startAudioDeviceRecordTest{@link #IAudioDeviceManager#startAudioDeviceRecordTest} or startAudioPlaybackDeviceTest{@link #IAudioDeviceManager#startAudioPlaybackDeviceTest}. <br>
    *        + Call stopHardwareEchoDetection{@link #IRTCVideo#stopHardwareEchoDetection} to stop the detection and release the audio devices.
    */
    virtual int startHardwareEchoDetection(const char* test_audio_file_path) = 0;
    /** 
     * @hidden(Linux)
     * @type api
     * @brief Stop the echo detection before joining a room.
     * @return   Method call result:   <br>
     *        + 0: Success. <br>
     *        + -1: Failure.
     * @notes <br>
     *        + Refer to startHardwareEchoDetection{@link #IRTCVideo#startHardwareEchoDetection} for information on how to start a echo detection. <br>
     *        + We recommend calling this API to stop the detection once getting the detection result from onHardwareEchoDetectionResult{@link #IRTCVideoEventHandler#onHardwareEchoDetectionResult}. <br>
     *        + You must stop the echo detection to release the audio devices before the user joins a room. Otherwise, the detection may interfere with the call.
     */
    virtual int stopHardwareEchoDetection() = 0;


    /** 
     * @hidden(macOS, Windows, Linux)
     * @type api
     * @brief Enable cellular network assisted communication to improve call quality.
     * @param [in] config See MediaTypeEnhancementConfig{@link #MediaTypeEnhancementConfig}.
     * @return Method call result:   <br>
     *        + 0: Success. <br>
     *        + -1: Failure, internal error. <br>
     *        + -2: Failure, invalid parameters. <br>
     * @notes The function is off by default.
     */
    virtual int setCellularEnhancement(const MediaTypeEnhancementConfig& config) = 0;

    /** 
     * @type api
     * @region proxy
     * @brief Sets local proxy.
     * @param [in] configurations Local proxy configurations. Refer to LocalProxyConfiguration{@link #LocalProxyConfiguration}. <br>
     *         You can set both Http tunnel and Socks5 as your local proxies, or only set one of them based on your needs. If you set both Http tunnel and Socks5 as your local proxies, then media traffic and signaling are routed through Socks5 proxy and Http requests through Http tunnel proxy. If you set either Http tunnel or Socks5 as your local proxy, then media traffic, signaling and Http requests are all routed through the proxy you chose. <br>
     *         If you want to remove the existing local proxy configurations, you can call this API with the parameter set to null. 
     * @param [in] configuration_num The number of local proxy configurations. 
     * @notes <br>
     *       + You must call this API before joining the room.  <br>
     *       + After calling this API, you will receive onLocalProxyStateChanged{@link #IRTCVideoEventHandler#onLocalProxyStateChanged} callback that informs you of the states of local proxy connection. 
     */
    virtual int setLocalProxy(const LocalProxyConfiguration* configurations, int configuration_num) = 0;
};

/** 
* @type api
* @region Engine Management
* @brief Creates an engine instance.   <br>
*        This is the very first API that you must call if you want to use all the RTC capabilities.  <br>
*        If there is no engine instance in current process, calling this API will create one. If an engine instance has been created, calling this API again will have the created engine instance returned.
* @param [in] app_id A unique identifier for each App, randomly generated by the RTC console. Only instances created with the same app_id are able to communicate with each other.
* @param [in] event_handler Handler sent from SDK to App. See IRTCVideoEventHandler{@link #IRTCVideoEventHandler}.
* @param [in] parameters Reserved parameters. Please contact technical support fellow if needed.
* @return  <br>
*        + IRTCVideo: A successfully created engine instance.  <br>
*        + Null:  app_id is null or empty. event_handler is null.
*/
BYTERTC_API bytertc::IRTCVideo* createRTCVideo(const char* app_id,
     bytertc::IRTCVideoEventHandler *event_handler, const char* parameters);

/** 
 * @hidden internal use only
 * @type api
 * @region Engine Management
 * @brief Creates an engine instance for multi-engine senario.   <br>
 *        This is the very first API that you must call if you want to use all the RTC capabilities.  <br>
 *        If there is no engine instance in current process, calling this API will create one. If an engine instance has been created, calling this API again will have a new created engine instance returned.
 * @param context Android application context
 * @param appId A unique identifier for each App, randomly generated by the RTC console. Engine instances created with different AppIds cannot communicate with each other.
 * @param handler Handler sent from SDK to App. See IRTCVideoEventHandler{@link #IRTCVideoEventHandler} <br>
 * @param eglContext If you need SDK to support custom texture hardcoding, you must import the eglContext in a JObject. <br>
 * @param parameters Reserved parameters. Please contact technical support fellow if needed.
 * @return  <br>
 *         + RTCVideo: A successfully created engine instance.  <br>
 *         + Null: Failed to load the .so file. No instance is returned.
 * @notes The lifecycle of the handler must be longer than that of the RTCVideo, i.e. the handler must be created before calling createRTCVideo{@link #RTCVideo#createRTCVideo} and destroyed after calling destroyRTCVideo{@link #RTCVideo#destroyRTCVideo}.
 */
BYTERTC_API bytertc::IRTCVideo* createRTCVideoMulti(const char* app_id,
                                                    bytertc::IRTCVideoEventHandler* event_handler, const char* parameters);
/** 
 * @type api
 * @region Engine Management
 * @brief Destroy the engine instance created by createRTCVideo{@link #createRTCVideo}, and release all related resources.<br>
 * @notes  <br>
 *         + Call this API after all business scenarios related to the engine instance are destroyed. In a multi-thread scenario, you must not call IRTCVideo{@link #IRTCVideo} related APIs after calling this interface, or the SDK may crash. When the API is called, RTC SDK destroys all memory associated with the engine instance and stops any interaction with the media server.  <br>
 *         + Calling this API will start the SDK exit logic. The engine thread is held until the exit logic is complete. The engine thread is retained until the exit logic is complete. Therefore, do not call this API directly in the callback thread, or it will cause a deadlock. This function takes a long time to execute, so it's not recommended to call this API in the main thread, or the main thread may be blocked.
 */
BYTERTC_API void destroyRTCVideo();

/** 
 * @hidden internal use only
 * @type api
 * @region Engine management
 * @brief Destroy the engine instance created by createRTCVideo{@link #RTCVideo#createRTCVideoMulti}, and release all related resources. <br>
 * @notes  <br>
 *       + Call this API after all business scenarios related to the engine instance are destroyed.  <br>
 *       + When the API is called, RTC SDK destroys all memory associated with the engine instance and stops any interaction with the media server.  <br>
 *       + Calling this API will start the SDK exit logic. The engine thread is held until the exit logic is complete. The engine thread is retained until the exit logic is complete. Therefore, do not call this API directly in the callback thread, or it will cause a deadlock. This function takes a long time to execute, so it's not recommended to call this API in the main thread, or the main thread may be blocked.
 */
BYTERTC_API void destroyRTCVideoMulti(IRTCVideo* instance_multi);
/** 
 * @type api
 * @region  error code
 * @brief Gets the description text of different error codes and warning codes in the SDK.<br>
 * @param [in] code Needs to get the description of the error code
 * @return The description of the error code
 * @notes This interface is a general function and does not need to rely on the engine object when calling.
 */
BYTERTC_API const char* getErrorDescription(int code);
/** 
 * @type api
 * @region  engine management
 * @brief  Get the current SDK version information.
 * @return Current SDK version information.
 */
BYTERTC_API const char* getSDKVersion();

/** 
 * @type api
 * @region Engine Management
 * @brief Configures the local log parameters of RTC SDK, including the logging level, directory, and the limits for total log file size.
 * @param [in] log_config Local log parameters. See LogConfig{@link #LogConfig}.
 * @return <br>
 *        + 0: Success.
 *        + –1: Failure. This API must be called before creating engine.
 *        + –2: Failure. Invalid parameters.
 * @notes This API must be called before createRTCVideo{@link #createRTCVideo}.
 */
BYTERTC_API int setLogConfig(const LogConfig& log_config);

}  // namespace bytertc
