/*
 * Copyright (c) 2020 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Event Handler Lite
*/

#pragma once

#ifndef BYTE_RTC_LITE_EVENT_HANDLER_H__
#define BYTE_RTC_LITE_EVENT_HANDLER_H__

#include "rtc/bytertc_defines.h"

namespace bytertc {
/** 
 * @type callback
 * @brief Audio & video engine event callback interface
 */
class IRTCVideoEventHandler {
public:

    /**
     * @hidden constructor/destructor
     */
    virtual ~IRTCVideoEventHandler() {
    }

    /** 
     * @type callback
     * @region Warning code
     * @brief A warning occurred during the SDK runtime. The SDK usually recovers automatically and warnings can be ignored.
     * @param [in] warn Warning identification code, see: WarningCode{@link #WarningCode}
     */
    virtual void onWarning(int warn) {
        (void)warn;
    }

    /** 
     * @type callback
     * @region Error code
     * @brief Error callback occurred. This callback is triggered when a network or media-related error occurs during the <br>
     *        SDK runtime and cannot be automatically recovered and you may need to intervene.
     * @param  [in] err Error code. See ErrorCode{@link #ErrorCode}
     */
    virtual void onError(int err) {
        (void)err;
    }

    /** 
     * @type callback
     * @region  Mix
     * @brief   Callback when audio mix file playback state changes
     * @param  [in] mix_id <br>
     *         Mix ID <br>
     *         Unique ID passed in when using IAudioMixingManager{@link #IAudioMixingManager} related interface.
     * @param  [in] state <br>
     *        Mix status <br>
     *        The mix status can be referred to: AudioMixingState{@link #AudioMixingState}.
     * @param  [in] error
     *         Error code <br>
     *         See AudioMixingError{@link #AudioMixingError}
     * @notes   <br>
     *        The timing of this callback will be triggered is summarized as follows: <br>
     *        + When the startAudioMixing{@link #IAudioMixingManager#startAudioMixing} method is successfully called, a callback with a state value of kAudioMixingStatePlaying will be triggered; otherwise the state will be triggered A callback with a value of kAudioMixingStateFailed. <br>
     *        + When startAudioMixing{@link #IAudioMixingManager#startAudioMixing} is called repeatedly with the same ID, the latter overrides the previous, and this callback notifies the previous mix has stopped with kAudioMixingStateStopped. <br>
     *        + When calling the pauseAudioMixing{@link #IAudioMixingManager#pauseAudioMixing} method to pause playback successfully, a callback with a state value of kAudioMixingStatePaused will be triggered; otherwise, a callback with a state value of kAudioMixingStateFailed will be triggered. <br>
     *        + When the resumeAudioMixing{@link #IAudioMixingManager#resumeAudioMixing} method is called to resume playing successfully, a callback with a state value of kAudioMixingStatePlaying will be triggered; otherwise, a callback with a state value of kAudioMixingStateFailed will be triggered. <br>
     *        + When calling the stopAudioMixing{@link #IAudioMixingManager#stopAudioMixing} method to pause playback successfully, a callback with a state value of kAudioMixingStateStopped will be triggered; otherwise, a callback with a state value of kAudioMixingStateFailed will be triggered. <br>
     *        + The end of playback triggers a callback with the state value kAudioMixingStateFinished.
     */
    virtual void onAudioMixingStateChanged(int mix_id, bytertc::AudioMixingState state, bytertc::AudioMixingError error) {
    }

    /** 
     * @type callback
     * @region Audio Mixing
     * @brief Callback for playback progress of mixed audio files
     * @param [in] mix_id ID of the mixing task  <br>
     * @param [in] progress The current playback progress (ms) of the mixed audio file
     * @notes After calling setAudioMixingProgressInterval{@link #IAudioMixingManager#setAudioMixingProgressInterval} to set the time interval to a value greater than 0, or calling startAudioMixing{@link #IAudioMixingManager#startAudioMixing} to set the time interval in AudioMixingConfig{@link #AudioMixingConfig} to a value greater than 0, the SDK will trigger the callback according to the set time interval.
     */
    virtual void onAudioMixingPlayingProgress(int mix_id, int64_t progress){
    }

    /** 
     * @type callback
     * @region  engine management
     * @brief  Terminal monitoring log callback. The callback is triggered when a terminal monitoring event is generated.
     * @param  [in] log_type <br>
     *         Event type. The current type is fixed to "live_webrtc_monitor_log".
     * @param  [in] log_content <br>
     *         Terminal monitoring log content.
     */
    virtual void onLogReport(const char* log_type, const char* log_content) {
        (void)log_type;
        (void)log_content;
    }

    /** 
     * @hidden(Windows,Linux,macOS)
     * @deprecated since 3.45 and will be deleted in 3.51, use onAudioRouteChanged{@link #IRTCVideoEventHandler#onAudioRouteChanged} instead.
     * @type callback
     * @region Audio event callback
     * @brief Callback triggered when the audio playback device changed on mobile.
     * @param [in] device Changed audio playback device. See AudioPlaybackDevice{@link #AudioPlaybackDevice}. <br>
     */
    virtual void onAudioPlaybackDeviceChanged(AudioPlaybackDevice device) {
        (void)device;
    };

    /** 
     * @hidden(macOS,Windows,Linux)
     * @type callback
     * @region Audio event callback
     * @brief Audio playback The event is called back when the device changes.
     * @param [in] device Changed audio playback device. See AudioRoute{@link #AudioRoute}. <br>
     */
    virtual void onAudioRouteChanged(AudioRoute route) {
        (void)route;
    };

    /** 
     * @type callback
     * @region  engine management
     * @brief SDK  connection state change callback with signaling server. Triggered when the connection state changes.
     * @param  [in] state The current connection status between the SDK and the signaling server. See ConnectionState{@link #ConnectionState}
     * @notes Refer to [Getting Connection Status](https://docs.byteplus.com/byteplus-rtc/docs/95376) for more details.
     */
    virtual void onConnectionStateChanged(bytertc::ConnectionState state) {
    }

    /** 
     * @type callback
     * @region Engine management
     * @brief SDK Current network connection type change callback. Callbacks the event when the current network connection type of the SDK changes.
     * @param  [in] type <br>
     *        SDK The current network connection type, see: NetworkType{@link #NetworkType}.
     */
    virtual void onNetworkTypeChanged(bytertc::NetworkType type) {
    }
    /** 
     * @type callback
     * @region Audio & Video Fallback
     * @brief This callback is triggered when media streams start to fall back or restore from a fallback.
     * @param [in] event Information about the fallback. See RemoteStreamSwitch{@link #RemoteStreamSwitch}.
     */
    virtual void onSimulcastSubscribeFallback(const RemoteStreamSwitch& event) {
        (void)event;
    }
    /** 
     * @type callback
     * @region audio & video fallback
     * @brief publish performance fallback is not turned on locally. When insufficient device performance is detected, this callback is received. <br>
     *        Locally turn on the release performance fallback. When the release performance fallback/recovery is caused due to device performance/network reasons, this callback is received.
     * @param [in] mode Indicates whether the release fallback function is turned on locally. See PerformanceAlarmMode{@link #PerformanceAlarmMode} <br>
     *                   + When the publisher does not turn on the release performance fallback, the mode value is kPerformanceAlarmModeNormal. <br>
     *                   + When the publisher turns on the release performance fallback, the mode value is kPerformance AlarmModeSimulcast.
     * @param [in] room_id <br>
     *                       + When the release performance fallback is not turned on, the room_id is empty <br>
     *                       + When the release performance fallback is turned on, the room_id is the room ID affected by the alarm.
     * @param [in] reason Reason for the alarm. See PerformanceAlarmReason{@link #PerformanceAlarmReason}
     * @param [in] data Performance rollback related data. See SourceWantedData{@link #SourceWantedData}.
     */
    virtual void onPerformanceAlarms(bytertc::PerformanceAlarmMode mode, const char* room_id,
            bytertc::PerformanceAlarmReason reason, const bytertc::SourceWantedData& data) {
        (void)mode;
        (void)room_id;
        (void)reason;
        (void)data;
    }

    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use onAudioDeviceStateChanged{@link #IRTCVideoEventHandler#onAudioDeviceStateChanged} and onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} instead.
     * @type callback
     * @region engine management
     * @brief Callback of media-device state.  <br>
     *        Media devices includes the following three: audio capture devices, audio playback devices, and video capture devices.
     * @param [in] device_id Device ID
     * @param [in] device_type Device type. Refer to MediaDeviceType{@link #MediaDeviceType} for more details.
     * @param [in] device_state Device state. Refer to MediaDeviceState{@link #MediaDeviceState} for more details.
     * @param [in] device_error Device error. Refer to MediaDeviceError{@link #MediaDeviceError} for more details.
     */
    virtual void onMediaDeviceStateChanged(const char* device_id,
                                           bytertc::MediaDeviceType device_type,
                                           bytertc::MediaDeviceState device_state,
                                           bytertc::MediaDeviceError device_error) {
        (void)device_id;
        (void)device_type;
        (void)device_state;
        (void)device_error;
    }
    /** 
     * @type callback
     * @region  engine management
     * @brief Callback of audio device state. Audio devices include audio capture devices and audio playback devices.
     * @param [in] device_id Device ID
     * @param [in] device_type Device type. Refer to RTCAudioDeviceType{@link #RTCAudioDeviceType} for more details.
     * @param [in] device_state Device state. Refer to MediaDeviceState{@link #MediaDeviceState} for more details.
     * @param [in] device_error Device error. Refer to MediaDeviceError{@link #MediaDeviceError} for more details.
     */
    virtual void onAudioDeviceStateChanged(const char* device_id, bytertc::RTCAudioDeviceType device_type,
            bytertc::MediaDeviceState device_state, bytertc::MediaDeviceError device_error) {
        (void)device_id;
        (void)device_type;
        (void)device_state;
        (void)device_error;
    }
    /** 
     * @type callback
     * @region Engine Management
     * @brief Callback of video device state. Video devices include cameras and screen sharing video capture devices.
     * @param  [in] device_id Device ID
     * @param  [in] device_type Device type. Refer to RTCVideoDeviceType{@link #RTCVideoDeviceType} for more details.
     * @param  [in] device_state Device state. Refer to MediaDeviceState{@link #MediaDeviceState} for more details.
     * @param  [in] device_error Device error. Refer to MediaDeviceError{@link #MediaDeviceError} for more details.
     */
    virtual void onVideoDeviceStateChanged(const char* device_id, bytertc::RTCVideoDeviceType device_type,
            bytertc::MediaDeviceState device_state, bytertc::MediaDeviceError device_error) {
        (void)device_id;
        (void)device_type;
        (void)device_state;
        (void)device_error;
    }

    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use onAudioDeviceWarning{@link #IRTCVideoEventHandler#onAudioDeviceWarning} and onVideoDeviceWarning{@link #IRTCVideoEventHandler#onVideoDeviceWarning} instead.
     * @type callback
     * @region Engine Management
     * @brief Media device warning callback. The media devices includes: audio capture devices, audio rendering devices, and video capture devices.
     * @param [in] device_id Device ID
     * @param [in] device_type Device type. See MediaDeviceType{@link #MediaDeviceType}
     * @param [in] device_warning Device error type. See MediaDeviceWarning{@link #MediaDeviceWarning}
     */
    virtual void onMediaDeviceWarning(const char* device_id, bytertc::MediaDeviceType device_type,
            bytertc::MediaDeviceWarning device_warning){
        (void)device_id;
        (void)device_type;
        (void)device_warning;
    }
    /** 
     * @type callback
     * @region Engine Management
     * @brief Audio device warning callback. The audio devices include audio capture devices and audio rendering devices.
     * @param [in] device_id Device ID
     * @param [in] device_type Device type. See RTCAudioDeviceType{@link #RTCAudioDeviceType}
     * @param [in] device_warning Device error type. See MediaDeviceWarning{@link #MediaDeviceWarning}
     */
    virtual void onAudioDeviceWarning(const char* device_id, bytertc::RTCAudioDeviceType device_type,
            bytertc::MediaDeviceWarning device_warning) {
        (void)device_id;
        (void)device_type;
        (void)device_warning;
    }
    /** 
     * @type callback
     * @region Engine Management
     * @brief Video device warning callback. The video devices include video capture devices.
     * @param [in] device_id Device ID
     * @param [in] device_type Device type. See RTCVideoDeviceType{@link #RTCVideoDeviceType}
     * @param [in] device_warning Device error type. See MediaDeviceWarning{@link #MediaDeviceWarning}
     */
    virtual void onVideoDeviceWarning(const char* device_id, bytertc::RTCVideoDeviceType device_type,
            bytertc::MediaDeviceWarning device_warning) {
        (void)device_id;
        (void)device_type;
        (void)device_warning;
    }

    /** 
     * @type callback
     * @region  engine management
     * @brief  You will periodically(2s) receive the callbacks of the current CPU and memory information.
     * @param  [in] stats The structure containing the current CPU and memory information. See SysStats{@link #SysStats}.
     */
    virtual void onSysStats(const bytertc::SysStats& stats) {
    }

    /** 
     * @type callback
     * @region Engine Management
     * @brief Callback on create room failure.
     * @param room_id  Room ID.
     * @param error_code Create room error code. See ErrorCode{@link #ErrorCode} for specific indications.
     */
    virtual void onCreateRoomStateChanged(const char* room_id, int error_code) {
        (void)room_id;
        (void)error_code;
    }

    /** 
     * @type callback
     * @region Proxy callback
     * @brief HTTP Receive the callback when the proxy connection state changes.
     * @param  [in] state The current HTTP proxy connection status. See HttpProxyState{@link #HttpProxyState}
     */
        virtual void onHttpProxyState(int state) {
    }

    /** 
     * @type callback
     * @region Proxy callback
     * @brief HTTPS Receive the callback when the proxy connection state changes.
     * @param   [out] State the current HTTPS proxy connection status. See HttpProxyState{@link #HttpProxyState}
     */
    virtual void onHttpsProxyState(int state) {
    }

    /** 
     * @type callback
     * @region Proxy callback
     * @brief Socks5 Receive the callback when the proxy state changes.
     * @param [out] state SOCKS5 proxy connection status. See Socks5ProxyState{@link #Socks5ProxyState}
     * @param [out] cmd every step of the proxy connection operating command
     * @param [out] proxy_address proxy address information
     * @param [out] local_address the local address used by the current connection
     * @param [out] remote_address the remote connection address
     */
    virtual void onSocks5ProxyState(int state,
        const char* cmd,
        const char* proxy_address,
        const char* local_address,
        const char* remote_address) {
    }

    /** 
     * @type callback
     * @region Local recording
     * @brief Get a callback of the local recording status.   <br>
     *        The callback is triggered by startFileRecording{@link #IRTCVideo#startFileRecording} or stopFileRecording{@link #IRTCVideo#stopFileRecording}.
     * @param [out] type Stream properties of the recorded stream. See StreamIndex{@link #StreamIndex}
     * @param [out] state Recording state. See RecordingState{@link #RecordingState}
     * @param [out] error_code Recording error code. See RecordingErrorCode{@link #RecordingErrorCode}
     * @param [out] info For more information about the recorded file. See RecordingInfo{@link #RecordingInfo}
     */
    virtual void onRecordingStateUpdate(
            StreamIndex type, RecordingState state, RecordingErrorCode error_code, RecordingInfo info) {
    }

    /** 
     * @type callback
     * @region Local recording
     * @brief Local recording progress callback.   <br>
     *        This callback is triggered by startFileRecording{@link #IRTCVideo#startFileRecording}. When the recording state is normal, the system will prompt the recording progress through this callback every second.
     * @param [out] type Stream properties of the recorded stream. See StreamIndex{@link #StreamIndex}
     * @param [out] process Recording progress. See RecordingProgress{@link #RecordingProgress}
     * @param [out] info More information about the recorded file. See RecordingInfo{@link #RecordingInfo}
     */
    virtual void onRecordingProgressUpdate(StreamIndex type, RecordingProgress process, RecordingInfo info) {
    }
    /** 
     *  @type callback
     *  @brief When calling startAudioRecording{@link #IRTCVideo#startAudioRecording} or stopAudioRecording{@link #IRTCVideo#stopAudioRecording} to change the recording status, receive the callback.
     *  @param [in] state See AudioRecordingState{@link #AudioRecordingState}.
     *  @param [in] error_code See AudioRecordingErrorCode{@link #AudioRecordingErrorCode}.
     */
    virtual void onAudioRecordingStateUpdate(AudioRecordingState state, AudioRecordingErrorCode error_code) {
    }

    /** 
     * @type callback
     * @region Real-time messaging
     * @brief login result callback
     * @param  [in] uid <br>
     *        Login user ID
     * @param  [in] error_code <br>
     *        Login result <br>
     *        See LoginErrorCode{@link #LoginErrorCode}.
     * @param  [in] elapsed <br>
     *         The time taken from the call to the login{@link #IRTCVideo#login} interface to return the result is <br>
     *         In ms. This callback is received after
     * @notes You will receive this callback after calling login{@link #IRTCVideo#login}.
     */
    virtual void onLoginResult(const char* uid, int error_code, int elapsed) {
        (void)uid;
        (void)error_code;
        (void)elapsed;
    }
    /** 
     * @type callback
     * @region Real-time messaging
     * @brief logout result callback
     * @notes After calling logout{@link #IRTCVideo#logout}, you will receive this callback.
     */
    virtual void onLogout() {
    }
    /** 
     * @type callback
     * @region Real-time messaging
     * @brief Set the return result of the application server parameter
     * @param  [in] error <br>
     *         + 200, set successfully <br>
     *         + != 200: Failure. Refer to UserMessageSendResult{@link #UserMessageSendResult} for details.
     * @notes Receive this callback after calling setServerParams{@link #IRTCVideo#setServerParams}.
     */
    virtual void onServerParamsSetResult(int error) {
        (void)error;
    }
    /** 
     * @type callback
     * @region Real-time messaging
     * @brief The return result of querying the login status of the peer or local user
     * @param  [in] peer_user_id User ID
     * @param  [in] status <br>
     *        The user login status of the query <br>
     *        See USER_ONLINE_STATUS{@link #USER_ONLINE_STATUS}.
     * @notes You must first call getPeerOnlineStatus{@link #IRTCVideo#getPeerOnlineStatus} to receive this callback.
     */
    virtual void onGetPeerOnlineStatus(const char* peer_user_id, int status) {
        (void)peer_user_id;
        (void)status;
    }
    /** 
     * @type callback
     * @region Real-time messaging
     * @brief Receive this callback when you receive a text message from an out-of-room user calling sendUserMessageOutsideRoom{@link #IRTCVideo#sendUserMessageOutsideRoom}
     * @param  [in] uid User ID of the message sender
     * @param  [in] message <br>
     *        Received text message content
     */
    virtual void onUserMessageReceivedOutsideRoom(const char* uid, const char* message) {
        (void)uid;
        (void)message;
    }
    /** 
     * @type callback
     * @region Real-time messaging
     * @brief Receive this callback when you receive a binary message from an out-of-room user calling sendUserBinaryMessageOutsideRoom{@link #IRTCVideo#sendUserBinaryMessageOutsideRoom}
     * @param  [in] uid <br>
     *        User ID of the message sender
     * @param  [in] size <br>
     *        Binary message length
     * @param  [in] message <br>
     *         Binary message content received
     */
    virtual void onUserBinaryMessageReceivedOutsideRoom(const char* uid, int size, const uint8_t* message) {
        (void)uid;
        (void)size;
        (void)message;
    }
    /** 
     * @type callback
     * @region Real-time messaging
     * @brief A callback that sends a message to a specified user outside the room
     * @param  [in] msgid <br>
     *        The ID of this message <br>
     *        All P2P and P2Server messages share a single ID sequence.
     * @param  [in] error <br>
     *         Message sending result <br>
     *         See UserMessageSendResult{@link #UserMessageSendResult}.
     * @notes Receive this callback when a message is sent by calling sendUserMessageOutsideRoom{@link #IRTCVideo#sendUserMessageOutsideRoom} or sendUserBinaryMessageOutsideRoom{@link #IRTCVideo#sendUserBinaryMessageOutsideRoom}.
     */
    virtual void onUserMessageSendResultOutsideRoom(int64_t msgid, int error) {
        (void)msgid;
        (void)error;
    }

    /** 
     * @type callback
     * @region Real-time messaging
     * @brief Receives the callback after sending the message to the application server successfully.
     * @param [in] msgid The ID of this message <br>
     *        All P2P and P2Server messages share a single ID sequence.
     * @param [in] error Message Sending Results. See UserMessageSendResult{@link #UserMessageSendResult}.
     * @param [in] msg The message returned in ACK when the application server receives HTTP request. The message should not exceed 64 KB.
     * @notes This callback is asynchronous. You will receive this callback when you call sendServerMessage{@link #IRTCVideo#sendServerMessage} or sendServerBinaryMessage{@link #IRTCVideo#sendServerBinaryMessage} to send a message to your application server.
     */
    virtual void onServerMessageSendResult(int64_t msgid, int error, const bytertc::ServerACKMsg& msg) {
        (void)msgid;
        (void)error;
        (void)msg;
    }

    /** 
     * @type callback
     * @region Video management
     * @brief Receive this callback when you receive a video frame with a SEI message sent via sendSEIMessage{@link #IRTCVideo#sendSEIMessage}
     * @param  [in] stream_key Contains the user name, room name and media stream of the SEI sender. See RemoteStreamKey{@link #RemoteStreamKey}
     * @param  [in] message The content of the SEI message received
     * @param  [in] length Length of received SEI message
    */
    virtual void onSEIMessageReceived(RemoteStreamKey stream_key, const uint8_t* message, int length){
        (void)stream_key;
        (void)message;
        (void)length;
    }
    /** 
     * @type callback
     * @region Message
     * @brief Callback about publishing status of the black frame video stream .  <br>
     *        In a voice call scenario, when the local user calls sendSEIMessage{@link #IRTCVideo#sendSEIMessage} to send SEI data with a black frame, the sending status of the stream is notified to the remote user through this callback.  <br>
     *        You can tell from this callback that the video frame carrying SEI data is a black frame and thus not render that video frame.
     * @param key Information about stream from the remote user, see RemoteStreamKey{@link #RemoteStreamKey}.
     * @param type State of the black frame video stream, see SEIStreamEventType{@link #SEIStreamEventType}.
     */
    virtual void onSEIStreamUpdate(const RemoteStreamKey& key, SEIStreamEventType type) {
        (void)key;
        (void)type;
    }

     /** 
      * @type callback
      * @region Audio Management
      * @brief After calling enableAudioPropertiesReport{@link #IRTCVideo#enableAudioPropertiesReport}, you will periodically receive this callback for the information about the subscribed remote audio streams. <br>
      *        The remote audio streams includes the microphone audio and screen audio collected using the RTC SDK internal mechanism/custom mechanism.
      * @param [in] audio_properties_infos See RemoteAudioPropertiesInfo{@link #RemoteAudioPropertiesInfo}.
      * @param [in] audio_properties_info_number The length of `audio_properties_infos`
      * @param [in] total_remote_volume The total volume of all the subscribed remote streams. The range is [0,255]. <br>
      *        + [0,25] Is close to silent; <br>
      *        + [25,75] Is low volume; <br>
      *        + [76,204] Is medium volume; <br>
      *        + [205,255] Is high volume. <br>
      */
    virtual void onRemoteAudioPropertiesReport(const RemoteAudioPropertiesInfo* audio_properties_infos, int audio_properties_info_number, int total_remote_volume) {
        (void)audio_properties_infos;
        (void)audio_properties_info_number;
        (void)total_remote_volume;
    }

     /** 
      * @type callback
      * @region Audio Management
      * @brief After calling enableAudioPropertiesReport{@link #IRTCVideo#enableAudioPropertiesReport}, you will periodically receive this callback for the information about local audio. <br>
      *        Local audio includes the microphone audio and the screen audio captured using RTC SDK internal mechanisms.
      * @param [in] audio_properties_infos See LocalAudioPropertiesInfo{@link #LocalAudioPropertiesInfo}.
      * @param [in] audio_properties_info_number The length of `audio_properties_infos`
      */
    virtual void onLocalAudioPropertiesReport(const LocalAudioPropertiesInfo * audio_properties_infos, int audio_properties_info_number) {
        (void)audio_properties_infos;
        (void)audio_properties_info_number;
    }

    /** 
     * @type callback
     * @region Audio Device Management
     * @brief Notification on the playing volume during the test for the local audio devices
     * @param [in] volume Playing volume during the test for the local audio devices in milliseconds. We recommend to set it to 200 ms. The range is [0,255].
     * @notes Start an audio-device test by calling startAudioPlaybackDeviceTest{@link #IAudioDeviceManager#startAudioPlaybackDeviceTest} or startAudioDeviceRecordTest{@link #IAudioDeviceManager#startAudioDeviceRecordTest} will register this callback for regular notification on playing volume. You can set the time interval between each callback by passing a proper value when calling the API above.
     */
    virtual void onAudioPlaybackDeviceTestVolume(int volume) {
        (void)volume;
    }

    /** 
     * @hidden(Android,iOS)
     * @type callback
     * @region device management
     * @brief Callback to notify you the volume of the audio device has been changed or when the device has been muted or unmuted. No need to activate the notification beforehand.
     * @param [in] device_type Includes microphones and speakers. Refer to RTCAudioDeviceType{@link #RTCAudioDeviceType} for more details.
     * @param [in] volume Volume ranging [0, 255]. When the volume turns to 0, muted turns to True. Note that on Windows, when the volume of a microphone turns to 0, muted remains unchanged.
     * @param [in] muted Whether is muted. When a speaker is muted, muted turns True but volume remains unchanged.
     */
    virtual void onAudioDeviceVolumeChanged(bytertc::RTCAudioDeviceType device_type, int volume, bool muted) {
        (void)device_type;
        (void)volume;
        (void)muted;
    }

    /** 
     * @deprecated since 3.50 and will be deleted in 3.55, use onAudioDeviceStateChanged{@link #IRTCVideoEventHandler#onAudioDeviceStateChanged} instead.
     * @type callback
     * @region Audio event callback
     * @brief When the state of the local audio changes, the callback notifies the current local audio state.
     * @param [in] state The state of the local audio device, see: LocalAudioStreamState{@link #LocalAudioStreamState}
     * @param [in] error Error code when the local audio state changes, see: LocalAudioStreamError{@link #LocalAudioStreamError}
     */
    virtual void onLocalAudioStateChanged(LocalAudioStreamState state, LocalAudioStreamError error) {
        (void)state;
        (void)error;
    }
    /** 
     * @type callback
     * @region Audio event callback
     * @brief Users in the room call startAudioCapture{@link #IRTCVideo#startAudioCapture} When audio capture is turned on, other users in the room will receive this callback.
     * @param [in] room_id ID of the room where the remote user enables audio capture
     * @param [in] user_id Remote user ID to enable audio capture
     */
    virtual void onUserStartAudioCapture(const char* room_id, const char* user_id) {
        (void)user_id;
    }

    /** 
     * @type callback
     * @region Audio event callback
     * @brief When a user in the room calls stopAudioCapture{@link #IRTCVideo#stopAudioCapture} to turn off audio capture, other users in the room will receive this callback.
     * @param [in] room_id ID of the room where the remote user disables audio capture
     * @param [in] user_id Remote user ID to turn off audio capture
     */
    virtual void onUserStopAudioCapture(const char* room_id, const char* user_id) {
        (void)user_id;
    }

    /** 
     * @type callback
     * @region Audio Management
     * @brief After calling enableAudioPropertiesReport{@link #IRTCVideo#enableAudioPropertiesReport}, you will periodically receive this callback for the active speaker information. <br>
     * @param [in] room_id  Room ID.
     * @param [in] uid The user ID of the active speaker.
     */
    virtual void onActiveSpeaker(const char* room_id, const char* uid) {
        (void)room_id;
        (void)uid;
    }

    /** 
     * @type callback
     * @region audio management
     * @brief Audio stream synchronization information callback. You can use this callback to receive audio stream synchronization information sent remotely after the remote user calls sendStreamSyncInfo{@link #IRTCVideo#sendStreamSyncInfo} to send an audio stream synchronization message. <br>
     * @param  [in] stream_key Remote stream information. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param  [in] stream_type Media stream type. See SyncInfoStreamType{@link #SyncInfoStreamType}.
     * @param  [in] data Message content.
     * @param  [in] length Message length.
     */
    virtual void onStreamSyncInfoReceived(RemoteStreamKey stream_key, SyncInfoStreamType stream_type,
                                         const uint8_t* data,
                                         int32_t length) {
        (void)stream_key;
        (void)stream_type;
        (void)data;
        (void)length;
    }

    /** 
     * @type callback
     * @region Pre-call network detection
     * @brief Pre-call network detection result.  <br>
     *        After successfully calling startNetworkDetection{@link #IRTCVideo#startNetworkDetection}, you will receive this callback for the first time in 3s and every 2s thereafter.
     * @param [in] type  Identifies the network type as uplink/downlink. <br>
     * @param [in] quality Network quality, see NetworkQuality{@link #NetworkQuality}. <br>
     * @param [in] rtt Network RTT in ms. <br>
     * @param [in] lost_rate Packet loss rate. <br>
     * @param [in] bitrate Network bandwidth in kbps. <br>
     * @param [in] jitter Network jitter in ms <br>
     */
    virtual void onNetworkDetectionResult(NetworkDetectionLinkType type, NetworkQuality quality, int rtt, double lost_rate,
                                      int bitrate, int jitter){
        (void)type;
        (void)quality;
        (void)rtt;
        (void)lost_rate;
        (void)bitrate;
        (void)jitter;
    }

    /** 
     * @type callback
     * @region Pre-call network probing
     * @brief Pre-call network probing ends <br>
     *        The following will stop detection and receive this primary callback:  <br>
     *        1. This callback is received once when the stopNetworkDetection{@link #IRTCVideo#stopNetworkDetection} interface is called to stop probing;  <br>
     *        2. Stop detection when the first frame of remote/local audio is received;  <br>
     *        3. Stop detecting when the detection exceeds 3 minutes;  <br>
     *        4. When the probe link is disconnected for a certain period of time, the probe is stopped.
     * @param [in] reason   <br>
     *        See NetworkDetectionStopReason{@link #NetworkDetectionStopReason} for reasons of stopping probing.
     */
    virtual void onNetworkDetectionStopped(NetworkDetectionStopReason reason){
        (void)reason;
    }
    /** 
     * @hidden currently not available
     * @type callback
     * @brief Callback of the result of subscribing to the public stream<br>
     *        You will be informed of the result of subscribing to the public stream by this callback after calling startPlayPublicStream{@link #IRTCVideo#startPlayPublicStream}.
     * @param [in] public_stream_id
     * @param [in] errorCode Code for the result of playing the public stream. Refer to PublicStreamErrorCode{@link #PublicStreamErrorCode} for details.
     */
    virtual void onPlayPublicStreamResult(const char* public_stream_id, PublicStreamErrorCode errorCode) {
        (void)public_stream_id;
        (void)errorCode;
    }
    /** 
     * @hidden currently not available
     * @type callback
     * @brief Callback on receiving the SEI message carried by the public video stream.
     *        After calling startPlayPublicStream{@link #IRTCVideo#startPlayPublicStream}, you will receive this callback if the public stream has an SEI message.
     * @param [in] public_stream_id The ID of the public stream.
     * @param [in] message The SEI(supplemental enhancement information) message carried by the public video stream.
     * @param [in] message_length The length of the SEI message.
     * @param [in] source_type SEI source type. See SEIMessageSourceType{@link #SEIMessageSourceType}.
     * @notes You will receive SEI from all the video streams if the SEI messages do not have conflicts.
     *        However, if the SEI  messages from different video streams have conflicts, you will receive only one of them.
     */
    virtual void onPublicStreamSEIMessageReceived(const char* public_stream_id,
        const uint8_t* message,
        int message_length,
        SEIMessageSourceType source_type) {
        (void)public_stream_id;
        (void)message;
        (void)message_length;
        (void)source_type;
    }
    /** 
     * @hidden currently not available
     * @type callback
     * @brief Callback of successfully decoding of the first video frame of the public stream<br>
     *        Refer to startPlayPublicStream{@link #IRTCVideo#startPlayPublicStream} for details about subsribing to a public stream.
     * @param [in] public_stream_id ID of the public stream
     * @param [in] info Information of the video stream. Refer to VideoFrameInfo{@link #VideoFrameInfo} for more details.
     */
    virtual void onFirstPublicStreamVideoFrameDecoded(const char* public_stream_id, const VideoFrameInfo& info) {
        (void)public_stream_id;
    }
    /** 
     * @type callback
     * @region Video event callback
     * @brief The remote clients in the room will be informed of the state change via this callback after the visible user starts video capture by calling startVideoCapture{@link #IRTCVideo#startVideoCapture}.
     * @param [in] room_id ID of the room from which the remote video stream is published.
     * @param [in] user_id The user who starts the internal video capture
     */
    virtual void onUserStartVideoCapture(const char* room_id, const char* user_id) {
        (void)user_id;
    }
    /** 
     * @type callback
     * @region Video event callback
     * @brief The remote clients in the room will be informed of the state change via  this callback after the visible user stops video capture by calling stopVideoCapture{@link #IRTCVideo#stopVideoCapture}.
     * @param [in] room_id ID of the room
     * @param [in] user_id The user who stops the internal video capture
     */
    virtual void onUserStopVideoCapture(const char* room_id, const char* user_id) {
        (void)user_id;
    }
    /** 
     * @type callback
     * @region video management
     * @brief Receive this callback when the local video size or rotation configuration changes.
     * @param [in] index See StreamIndex{@link #StreamIndex}。
     * @param [in] info Video frame information. See VideoFrameInfo{@link #VideoFrameInfo}.
     */
    virtual void onLocalVideoSizeChanged(StreamIndex index, const VideoFrameInfo& info) {
        (void)index;
        (void)info;
    }
    /** 
     * @type callback
     * @region Video Management
     * @brief Users in the room who subscribe to this video stream receive this callback when the remote video size or rotation configuration changes.
     * @param  [in] key Remote stream information. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param  [in] info Video frame information. See VideoFrameInfo{@link #VideoFrameInfo}.
     */
    virtual void onRemoteVideoSizeChanged(RemoteStreamKey key, const VideoFrameInfo& info) {
        (void)key;
        (void)info;
    }
    /** 
     * @type callback
     * @region Video Management
     * @brief RTC SDK receives this callback when the first video frame or screen video frame capture is completed locally.
     * @param [in] index Stream property. See StreamIndex{@link #StreamIndex}.
     * @param [in] info Video information. See VideoFrameInfo{@link #VideoFrameInfo}.
     * @notes For captured local video frames, you can call setLocalVideoCanvas{@link #IRTCVideo#setLocalVideoCanvas} or setLocalVideoSink{@link #IRTCVideo#setLocalVideoSink} to render locally.
     */
    virtual void onFirstLocalVideoFrameCaptured(StreamIndex index, VideoFrameInfo info) {
        (void)index;
        (void)info;
    }
    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}, onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen}, onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream} and onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen} instead.
     * @type callback
     * @region Media Stream Management
     * @brief Transmission state change of an audio stream in the room triggers this callback.
     * @param [in] user_id The user who changes his/her transmission state of the audio streams
     * @param [in] mute_state Transmission state of the audio streams. Refer to MuteState{@link #MuteState} for more details.
     */
    virtual void onUserMuteAudio(const char* room_id, const char* user_id, MuteState mute_state) {
        (void)user_id;
        (void)mute_state;
    }
    /** 
     * @type callback
     * @region Audio event callback
     * @brief Receives the callback when the first frame from a remote audio stream is received.
     * @param  [in] key Remote audio stream information. See RemoteStreamKey{@link #RemoteStreamKey}
     * @notes The callback is received when the user has just received each audio stream in the room.
     */
    virtual void onFirstRemoteAudioFrame(const RemoteStreamKey& key) {
        (void)key;
    }
    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}, onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen}, onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream} and onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen} instead.
     * @type callback
     * @region Media Stream Management
     * @brief Transmission state change of an video stream in the room triggers this callback.
     * @param  [in] uid The user who changes his/her transmission state of the video streams
     * @param  [in] mute Transmission state of the video streams. Refer to MuteState{@link #MuteState} for more details.
     */
    virtual void onUserMuteVideo(const char* room_id, const char* uid, MuteState mute) {
        (void)uid;
        (void)mute;
    }
    /** 
     * @hidden not available
     * @type callback
     * @region Audio event callback
     * @brief When the state of the audio stream from the remote user subscribes to changes, this callback will be received to understand the current state of the remote audio stream.
     * @param  [in] key Remote stream information. See RemoteStreamKey{@link #RemoteStreamKey}
     * @param  [in] state Remote audio stream state. See RemoteAudioState{@link #RemoteAudioState}
     * @param  [in] reason For remote audio stream state change. See RemoteAudioStateChangeReason{@link #RemoteAudioStateChangeReason}
     */
    virtual void onRemoteAudioStateChanged(
            const RemoteStreamKey& key, RemoteAudioState state, RemoteAudioStateChangeReason reason) {
        (void)key;
        (void)state;
        (void)reason;
    }
    /** 
     * @deprecated since 3.50 and will be deleted in 3.55. Use onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} instead.
     * @type callback
     * @region video management [in]
     * @brief Receive this event when the state of the local video stream changes.
     * @param [in] index Index audio/video properties. See StreamIndex{@link #StreamIndex}
     * @param [in] state Local video stream status. See LocalVideoStreamState{@link #LocalVideoStreamState}
     * @param [in] error Error code when local video status changes. See LocalVideoStreamError{@link #LocalVideoStreamError}
     */
    virtual void onLocalVideoStateChanged(StreamIndex index, LocalVideoStreamState state, LocalVideoStreamError error) {
        (void)index;
        (void)state;
        (void)error;
    }
    /** 
     * @hidden not available
     * @type callback
     * @region Video Management
     * @brief When the state of a remote video stream changes, users in the room who subscribe to this stream receive the event.
     * @param  [in] key Information about the remote video stream, room, user ID, stream attributes, etc. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param  [in] state Remote video stream state. See RemoteVideoState{@link #RemoteVideoState}.
     * @param  [in] reason For the remote video stream state change. See RemoteVideoStateChangeReason{@link #RemoteVideoStateChangeReason}.
     */
    virtual void onRemoteVideoStateChanged(
            RemoteStreamKey key, RemoteVideoState state, RemoteVideoStateChangeReason reason) {
        (void)key;
        (void)state;
        (void)reason;
    }

    /** 
     * @type callback
     * @hidden for internal use only on Android, iOS, macOS, and Windows
     * @region Audio & Video Processing
     * @brief When the super resolution mode of a remote video stream changes, users in the room who subscribe to this stream will receive this callback.
     * @param [in] stream_key Remote stream information that includes the room ID, user ID, and stream type. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param [in] mode Super resolution mode. See VideoSuperResolutionMode{@link #VideoSuperResolutionMode}.
     * @param [in] reason Remote video stream super resolution mode change reason. See VideoSuperResolutionModeChangedReason{@link #VideoSuperResolutionModeChangedReason}
     */
    virtual void onRemoteVideoSuperResolutionModeChanged(
            RemoteStreamKey stream_key, VideoSuperResolutionMode mode, VideoSuperResolutionModeChangedReason reason) {
        (void)stream_key;
        (void)mode;
        (void)reason;
    }

    /** 
     * @type callback
     * @region Room management
     * @brief Audio first frame sending status change callback
     * @param [in] room_id ID of the room where the audio is published
     * @param [in] user Local user information. See RtcUser{@link #RtcUser}
     * @param [in] state First frame sending status. See FirstFrameSendState{@link #FirstFrameSendState}
     */
    virtual void onAudioFrameSendStateChanged(const char* room_id, const RtcUser& user, FirstFrameSendState state) {
        (void)user;
        (void)state;
    }

    /** 
     * @type callback
     * @region Room management
     * @brief Screen Audio first frame sending status change callback
     * @param [in] room_id ID of the room where the audio is published
     * @param [in] user Local user information. See RtcUser{@link #RtcUser}
     * @param [in] state First frame sending status. See FirstFrameSendState{@link #FirstFrameSendState}
     */
    virtual void onScreenAudioFrameSendStateChanged(const char* room_id, const RtcUser& user, FirstFrameSendState state) {
        (void)user;
        (void)state;
    }

    /** 
     * @type callback
     * @region Room management
     * @brief Video first frame sending status change callback
     * @param [in] room_id ID of the room where the video is published
     * @param [in] user Local user information. See RtcUser{@link #RtcUser}
     * @param [in] state First frame sending status. See FirstFrameSendState{@link #FirstFrameSendState}
     */
    virtual void onVideoFrameSendStateChanged(const char* room_id, const RtcUser& user, FirstFrameSendState state) {
        (void)user;
        (void)state;
    }
    /** 
     * @type callback
     * @region Video Management
     * @brief  Receive this callback after the first frame of remote video stream is locally rendered by SDK.
     * @param  [in] key Remote stream information. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param  [in] info Video frame information. See VideoFrameInfo{@link #VideoFrameInfo}.
     */
    virtual void onFirstRemoteVideoFrameRendered(const RemoteStreamKey key, const VideoFrameInfo& info) {
        (void)key;
        (void)info;
    }
    /** 
     * @type callback
     * @region Video Management
     * @brief Receive this callback after the first frame of remote video stream is received and decoded by SDK.
     * @param [in] key Remote stream information. See RemoteStreamKey {@link #RemoteStreamKey}.
     * @param [in] info Video frame information. See VideoFrameInfo {@link #VideoFrameInfo}.
     */
    virtual void onFirstRemoteVideoFrameDecoded(const RemoteStreamKey key, const VideoFrameInfo& info) {
        (void)key;
        (void)info;
    }
    /** 
     * @type callback
     * @region room management
     * @brief Screen sharing streaming video first frame sending status change callback
     * @param [in] room_id ID of the room from which the screen stream is published.
     * @param [in] user Local user information. See RtcUser{@link #RtcUser}
     * @param [in] state First frame sending status. See FirstFrameSendState{@link #FirstFrameSendState}
     */
    virtual void onScreenVideoFrameSendStateChanged(const char* room_id, const RtcUser& user, FirstFrameSendState state) {
        (void)user;
        (void)state;
    }
    /** 
     * @type callback
     * @region  room management
     * @brief  Audio first frame playback state change callback
     * @param [in] room_id ID of the room from which the stream is published
     * @param [in] user Remote user information. See RtcUser{@link #RtcUser}
     * @param [in] state First frame playback status. See FirstFramePlayState{@link #FirstFramePlayState}
     */
    virtual void onAudioFramePlayStateChanged(const char* room_id, const RtcUser& user, FirstFramePlayState state) {
        (void)user;
        (void)state;
    }
    /** 
     * @type callback
     * @region  room management
     * @brief  Screen Audio first frame playback state change callback
     * @param [in] room_id ID of the room from which the stream is published
     * @param [in] user Remote user information. See RtcUser{@link #RtcUser}
     * @param [in] state First frame playback status. See FirstFramePlayState{@link #FirstFramePlayState}
     */
    virtual void onScreenAudioFramePlayStateChanged(const char* room_id, const RtcUser& user, FirstFramePlayState state) {
        (void)user;
        (void)state;
    }
    /** 
     * @type callback
     * @region  room management
     * @brief  Video first frame playback state change callback
     * @param [in] room_id ID of the room from which the stream is published
     * @param [in] user Remote user information. See RtcUser{@link #RtcUser}
     * @param [in] state First frame playback state. See FirstFramePlayState{@link #FirstFramePlayState}
     */
    virtual void onVideoFramePlayStateChanged(const char* room_id, const RtcUser& user, FirstFramePlayState state) {
        (void)user;
        (void)state;
    }
    /** 
     * @type callback
     * @region room management
     * @brief Screen sharing streaming video first frame playback state change callback
     * @param [in] room_id ID of the room from which the stream is published
     * @param [in] user Remote user information. See RtcUser{@link #RtcUser}
     * @param [in] state First frame playback state. See FirstFramePlayState{@link #FirstFramePlayState}
     */
    virtual void onScreenVideoFramePlayStateChanged(const char* room_id, const RtcUser& user, FirstFramePlayState state) {
        (void)user;
        (void)state;
    }

    /** 
     * @type callback
     * @region Audio event callback
     * @brief Receive the callback when publishing local audio stream.
     * @param  [in] index Audio stream properties. See StreamIndex{@link #StreamIndex}
     */
    virtual void onFirstLocalAudioFrame(StreamIndex index) {
        (void)index;
    }
    /** 
     * @hidden currently not available
     * @type callback
     * @brief Callback of the result of publishing the public stream<br>
     *        You will be informed of the result of publishing the public stream by this callback after calling startPushPublicStream{@link #IRTCVideo#startPushPublicStream}.
     * @param [in] room_id Room ID from which the public stream is published
     * @param [in] public_streamid ID of the public stream
     * @param [in] errorCode Code for the result of publishing the public stream. Refer to PublicStreamErrorCode{@link #PublicStreamErrorCode} for details.
     */
    virtual void onPushPublicStreamResult(const char* room_id, const char* public_streamid, PublicStreamErrorCode errorCode) {
        (void)room_id;
        (void)public_streamid;
        (void)errorCode;
    }

    /** 
     * @hidden currently not available
     * @type callback
     * @brief Callback of successfully decoding of the first audio frame of the public stream<br>
     *        Refer to startPlayPublicStream{@link #IRTCVideo#startPlayPublicStream} for details about subsribing to a public stream.
     * @param [in] public_stream_id ID of the public stream
     */
    virtual void onFirstPublicStreamAudioFrame(const char* public_stream_id) {
        (void)public_stream_id;
    }
    /** 
     * @type callback
     * @brief Receives the callback when you call startCloudProxy{@link #IRTCVideo#startCloudProxy} to start cloud proxy, and the SDK connects the proxy server successfully.
     * @param [in] interval The interval in ms between starting cloud proxy and connects the cloud proxy server successfully.
     */
    virtual void onCloudProxyConnected(int interval) {
        (void)interval;
    }
    /** 
     * @hidden(Linux)
     * @type callback
     * @region Network Management
     * @brief Callback about the call test result.
     * @param [in] result Test result, see EchoTestResult{@link #EchoTestResult}.
     * @notes The timing when this callback will be triggered is as follows:  <br>
     *        + A device-related error occured during the test；  <br>
     *        + After a successful test；  <br>
     *        + After stopping the test, provided that the audio/video playback was not received during the test due to non-device reasons.
     */
    virtual void onEchoTestResult(EchoTestResult result) {
        (void)result;
    };

    /** 
     * @hidden for internal use only
     * @type callabck
     * @brief audio dump status change callback
     * @param [in] status audio dump status, see AudioDumpStatus{@link #AudioDumpStatus}.
     * @notes This callback is used to internally troubleshoot sound quality related exceptions, which developers need not pay attention to.
     */
    virtual void onAudioDumpStateChanged(AudioDumpStatus status) {
        (void)status;
    }
    /** 
     * @hidden(Linux)
     * @type callback
     * @brief After calling getNetworkTimeInfo{@link #IRTCVideo#getNetworkTimeInfo} for the first time, the SDK starts network time synchronization internally. This callback will be triggered when the synchronization is completed.
     */
    virtual void onNetworkTimeSynchronized() {
    }
    /** 
     * @type callabck
     * @brief License expiration time reminder
     * @param [in] days Expiration time in days
     */
    virtual void onLicenseWillExpire(int days) {
        (void)days;
    }

    /** 
     * @hidden(Linux,Android,iOS)
     * @type callback
     * @brief After calling setOriginalScreenVideoInfo{@link #IRTCVideo#setOriginalScreenVideoInfo} to set the original width and height of the external shared-screen stream and setting the encoding mode to the automatic mode, you will receive this callback to re-capture the stream based on the recommended pixel and framerate by RTC.
     * @param [in] frameUpdateInfo The recommended pixel and framerate by RTC. See ByteRTCFrameUpdateInfo{@link #ByteRTCFrameUpdateInfo}.
     */
    virtual void onExternalScreenFrameUpdate(FrameUpdateInfo frameUpdateInfo) {
        (void)frameUpdateInfo;
    }
    /** 
     * @type callabck
     * @brief callback for experimental api
     * @param param result of callback in JSON string
     */
    virtual void onInvokeExperimentalAPI(const char* param) {
        (void)param;
    }
};

} // namespace bytertc

#endif // BYTE_RTC_LITE_EVENT_HANDLER_H__
