/*
 * Copyright (c) 2020 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Room Event Handler Interface
*/

#pragma once

#ifndef BYTE_RTC_ROOM_EVENT_HANDLER_INTERFACE_H__
#define BYTE_RTC_ROOM_EVENT_HANDLER_INTERFACE_H__

#include "rtc/bytertc_defines.h"

namespace bytertc {
/** 
 * @type callback
 * @brief  Audio & video room event callback interface
 */
class IRTCRoomEventHandler {
public:

    /**
     * @hidden constructor/destructor
     */
    virtual ~IRTCRoomEventHandler() {
    }

    /** 
     * @type callback
     * @region Multi-room
     * @brief Callback on room state changes. Via this callback you get notified of room relating warnings,  errors and events. For example, the user joins the room, the user is removed from the room, and so on.
     * @param [in] room_id  Room ID.
     * @param [in] uid  User ID.
     * @param [in] state Room state code. <br>
     *              + 0: Join room success. <br>
     *              + !0: Failed to join a room, abnormal exit, room-related warnings or errors. See ErrorCode{@link #ErrorCode} and WarningCode{@link #WarningCode} for specific indications.
     * @param [in] extra_info Extra information.
     *                 `join_type` indicates the type of room the user joins. `0` means the user joins the room for the first time, and `1` means the user rejoins the room. <br>
     *                 `elapsed` indicates the time interval from calling joinRoom{@link #IRTCRoom#joinRoom} to successfully joining room, in ms.
     */
    virtual void onRoomStateChanged(
            const char* room_id, const char* uid, int state, const char* extra_info) {
        (void)room_id;
        (void)state;
        (void)extra_info;
    }

    /** 
     * @type callback
     * @region Room management
     * @brief Callback on stream state changes. You will receive this callback when you receive stream relating warnings and errors.
     * @param [in] room_id  Room ID.
     * @param [in] uid  User ID.
     * @param [in] state Room state code. See ErrorCode{@link #ErrorCode} and WarningCode{@link #WarningCode} for specific indications.
     * @param [in] extra_info Extra information. Currently unavailable.
     */
    virtual void onStreamStateChanged(
            const char* room_id, const char* uid, int state, const char* extra_info) {
        (void)room_id;
        (void)uid;
        (void)state;
        (void)extra_info;
    }

    /** 
      * @type callback
      * @region Room management
      * @brief After leaving the room successfully, receives the callback.   <br>
      *        When the user calls the leaveRoom{@link #IRTCRoom#leaveRoom}, the SDK will stop all publishing subscription streams and release all call-related media resources. After that, the user receives this callback . <br>
      * @param [in] stats Reserved parameter.
      * @notes   <br>
      *        + If calling leaveRoom{@link #IRTCRoom#leaveRoom} and then destroyRTCVideo{@link #destroyRTCVideo} immediately after, the user will not receive this callback. <br>
      *        + If the app needs to use the media resources of the device for purposes other than RTC, you should init the resources after receiving this callback. Receiving this callback ensures that the resources are not occupied by RTC SDK.
      */
    virtual void onLeaveRoom(const RtcRoomStats& stats) {
        (void)stats;
    }

    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} and onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} instead.
     * @type callback
     * @region Room Management
     * @brief SDK Warning callback occurred. When a recoverable error is encountered inside the <br>
     *        SDK, the user is notified via this callback while attempting automatic recovery. This callback event is only used as a notification. <br>
     * @param  [in] warn Code. See WarningCode{@link #WarningCode}. <br>
     */
    BYTERTC_DEPRECATED virtual void onRoomWarning(int warn) {
        (void)warn;
    }

    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} and onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} instead.
     * @type callback
     * @region Room management
     * @brief SDK Error callback occurred. <br>
     *        SDK internal encountered an unrecoverable error, will notify the App through this callback, requires the App to operate according to the error code or prompt the user. <br>
     * @param  [in] err Error code. See ErrorCode{@link #ErrorCode}. <br>
     */
    virtual void onRoomError(int err) {
        (void)err;
    }
    /** 
     * @type callback
     * @brief Callback triggered 30s before the joining room privilege of the Token expires. <br>
     *        After receiving this callback, you must call updateToken{@link #IRTCRoom#updateToken} to update the joining room privilege Token.
     * @notes After a user's joining room privilege expires:<br>
     *        + When attempting to join a room, the user will receive onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} with the error code "-1000" indicating that the Token is invalid.<br>
     *        + If the user is already in the room, he/she will be removed from the room and receive onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} with the error code "-1009" indicating that the Token is expired. Remote users in the room will receive onUserLeave{@link #IRTCRoomEventHandler#onUserLeave} with the reason "2" indicating that the Token is invalid.
     */
    virtual void onTokenWillExpire() {

    }
    /** 
     * @type callback
     * @brief Callback triggered 30s before the publishing privilege of the Token expires. <br>
     *        After receiving this callback, you must call updateToken{@link #IRTCRoom#updateToken} to update the publishing privilege Token.
     * @notes After a user's publishing privilege expires:<br>
     *        + When attempting to publish a stream, the user will receive onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} with the error code "-1002" indicating no permission to publish streams.<br>
     *        + The published streams of the user will be removed, and he/she will receive onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} with the error code "-1002" indicating no permission to publish streams. Remote users in the room will receive onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream}/onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen} with the reason "6" indicating that the publishing privilege of the remote user has expired.
     */
    virtual void onPublishPrivilegeTokenWillExpire() {

    }
    /** 
     * @type callback
     * @brief Callback triggered 30s before the subscribing privilege of the Token expires. <br>
     *        After receiving this callback, you must call updateToken{@link #IRTCRoom#updateToken} to update the subscribing privilege Token.
     * @notes After a user's subscribing privilege expires, the user will fail to subscribe to new streams, or the subscribed streams will be removed, and he/she will receive onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} with error code "-1003" indicating no permission to subscribe to streams.
     */
    virtual void onSubscribePrivilegeTokenWillExpire() {

    }

    /** 
     * @type callback
     * @region  data statistics
     * @brief  In-room call statistics callback.    <br>
     *        After the user enters the room and starts the call, he receives this callback every 2s.
     * @param  [in] stats Current RtcEngine statistics. See RtcRoomStats{@link #RtcRoomStats}
     */
    virtual void onRoomStats(const RtcRoomStats& stats) {
        (void)stats;
    }
    /** 
     * @hidden(Linux)
     * @type callback
     * @region Statistics
     * @brief Local stream data statistics and network quality callback.   <br>
     *        After the local user publishes the flow successfully, the SDK will periodically (2s) notify the user through this callback event of the quality statistics of the published flow during this reference period. <br>
     *        Statistics are passed to the user through the callback parameters of the LocalStreamStats{@link #LocalStreamStats} type, including the sent audio & video bit rate, sent frame rate, encoded frame rate, network quality, etc. <br>
     * @param  [in] stats Current RtcEngine statistics. See LocalStreamStats{@link #LocalStreamStats}
     */
    virtual void onLocalStreamStats(const LocalStreamStats& stats) {
        (void)stats;
    }
    /** 
     * @type callback
     * @region Statistics
     * @brief Remote audio/video stream statistics and network quality callbacks for local subscriptions.   <br>
     *        After the local user subscribes to the stream successfully, the SDK will periodically (2s) notify the user through this callback event of the quality statistics of the subscribed stream during this reference period, including: sending audio & video bit rate, sending frame rate, encoded frame rate, network quality, etc.
     * @param  [in] stats Current RtcEngine statistics. See RemoteStreamStats{@link #RemoteStreamStats}
     */
    virtual void onRemoteStreamStats(const RemoteStreamStats& stats) {
        (void)stats;
    }

    /** 
      * @type callback
      * @region Room management
      * @brief You will receive this callback in following cases: <br>
      *        + The remote user calls setUserVisibility{@link #IRTCRoom#setUserVisibility} turns visible and joins your room. <br>
      *        + The remote visible user is disconnected and then reconnected to your room. <br>
      *        + The invisible remote user in your room calls setUserVisibility{@link #IRTCRoom#setUserVisibility} and turns visible. <br>
      *        + You join the room when there are visible users in the room.
      * @param [in] user_info See UserInfo{@link #UserInfo}. <br>
      * @param [in] elapsed Reserved parameter.
      */
    virtual void onUserJoined(const UserInfo& user_info, int elapsed) {
        (void)user_info;
        (void)elapsed;
    }

    /** 
     * @type callback
     * @brief This callback is triggered when a remote user is disconnected or turns invisible.
     * @param uid ID of the user who leaves the room, or switches to invisible. <br>
     * @param reason Reason to leave the room. Refer to UserOfflineReason{@link #UserOfflineReason}.<br>
     *               + 0: The remote client calls leaveRoom{@link #IRTCRoom#leaveRoom} to leave the room. <br>
     *               + 1: The remote client is disconnected because of poor network connection or expired Token. <br>
     *               + 2: The remote client calls setUserVisibility{@link #IRTCRoom#setUserVisibility} to turn invisible. <br>
     *               + 3: The remote user has been removed from the room by the administrator via a OpenAPI call.
     */
    virtual void onUserLeave(const char* uid, UserOfflineReason reason) {
        (void)uid;
        (void)reason;
    }
    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream} and onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen} instead.
     * @type callback
     * @region Room Management
     * @brief Local users will receive this callback when a remote user in the room stops publishing audio & video streams. Properties of the
     * @param  [in] stream Stream. See MediaStreamInfo{@link #MediaStreamInfo}.
     * @param  [in] reason The reason for the removal of the remote stream. See StreamRemoveReason{@link #StreamRemoveReason}.
     */
    BYTERTC_DEPRECATED virtual void onStreamRemove(const MediaStreamInfo& stream, StreamRemoveReason reason) {
        (void)stream;
    }

    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream} and onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen} instead.
     * @type callback
     * @region Room Management
     * @brief When users in the room post a new audio & video stream, other users in the room will receive this callback. Includes streams that are removed and republished.   <br>
     * @param  [in] stream Stream properties. See MediaStreamInfo{@link #MediaStreamInfo}. <br>
     */
    BYTERTC_DEPRECATED virtual void onStreamAdd(const MediaStreamInfo& stream) {
        (void)stream;
    }
    /** 
     * @type callback
     * @region Room management
     * @brief Callback on new media streams captured by camera/microphone in the room.
     * @param [in] uid The ID of the remote user who published the stream.
     * @param [in] type Media stream type. See MediaStreamType{@link #MediaStreamType}.
     * @notes You will receive this callback after a remote user successfully published media streams captured by camera/microphone in the room with publishStream{@link #IRTCRoom#publishStream}. Then you can choose whether to call subscribeStream{@link #IRTCRoom#subscribeStream} to subscribe to the streams or not.
     */
    virtual void onUserPublishStream(const char* uid, MediaStreamType type) {
        (void)uid;
        (void)type;
    }
    /** 
     * @type callback
     * @region Room management
     * @brief Callback on removal of remote media stream captured by camera/microphone.
     * @param [in] uid The ID of the remote user who removed the stream.
     * @param [in] type Media stream type. See MediaStreamType{@link #MediaStreamType}.
     * @param [in] reason The reason for the removal, see StreamRemoveReason{@link #StreamRemoveReason}.
     * @notes After receiving this callback, you can choose whether to call unsubscribeStream{@link #IRTCRoom#unsubscribeStream} to unsubscribe from the streams or not.
     */
    virtual void onUserUnpublishStream(const char* uid, MediaStreamType type,StreamRemoveReason reason) {
        (void)uid;
        (void)type;
        (void)reason;
    }
    /** 
     * @type callback
     * @region Room management
     * @brief Callback on new screen sharing media streams from remote users in the room.
     * @param [in] uid The ID of the remote user who published the stream.
     * @param [in] type Media stream type. See MediaStreamType{@link #MediaStreamType}.
     * @notes You will receive this callback after a remote user successfully published screen sharing streams in the room with publishScreen{@link #IRTCRoom#publishScreen}. Then you can choose whether to call subscribeScreen{@link #IRTCRoom#subscribeScreen} to subscribe to the streams or not.
     */
    virtual void onUserPublishScreen(const char* uid, MediaStreamType type) {
        (void)uid;
        (void)type;
    }
    /** 
     * @type callback
     * @region Room management
     * @brief Callback on removal of screen sharing media streams from remote users in the room.
     * @param [in] uid The ID of the remote user who removed the stream.
     * @param [in] type Media stream type. See MediaStreamType{@link #MediaStreamType}.
     * @param [in] reason The reason for the removal, see StreamRemoveReason{@link #StreamRemoveReason}.
     * @notes After receiving this callback, you can choose whether to call unsubscribeScreen{@link #IRTCRoom#unsubscribeScreen} to unsubscribe from the streams or not.
     */
    virtual void onUserUnpublishScreen(const char* uid, MediaStreamType type,StreamRemoveReason reason) {
        (void)uid;
        (void)type;
        (void)reason;
    }

    /** 
     * @type callback
     * @region Room management
     * @brief Callback on subscription status of media streams
     * @param [in] state_code Subscription status of media streams, see SubscribeState{@link #SubscribeState}.
     * @param [in] user_id The ID of the user who published the stream.
     * @param [in] info Configurations of stream subscription, see SubscribeConfig{@link #SubscribeConfig}.
     * @notes  <br>
     *        Local users will receive this callback:  <br>
     *        + After calling subscribeStream{@link #IRTCRoom#subscribeStream}/unsubscribeStream{@link #IRTCRoom#unsubscribeStream} to change the subscription status of remote media streams captured by camera/microphone.  <br>
     *        + After calling subscribeScreen{@link #IRTCRoom#subscribeScreen}/unsubscribeScreen{@link #IRTCRoom#unsubscribeScreen} to change the subscription status of remote screen sharing streams.
     */
    virtual void onStreamSubscribed(SubscribeState state_code, const char* user_id, const SubscribeConfig& info) {
        (void)state_code;
        (void)user_id;
        (void)info;
    }

    /** 
     * @hidden for internal use only
     * @type callback
     * @region Room management
     * @brief Callback the event when the release flow is successful.
     * @param [in] user_id The user ID of the published stream.
     * @param [in] is_screen Whether the stream is a screen stream. <br>
     *              + Ture: Screen stream; <br>
     *              + False: Non-screen stream. <br>
     */
    virtual void onStreamPublishSuccess(const char* user_id, bool is_screen) {
        (void)is_screen;
    }

    /** 
     * @type callback
     * @region Real-time messaging
     * @brief Receives a callback for broadcast messages in the room.
     * @param  [in] uid <br>
     *         User ID of the message sender
     * @param  [in] message <br>
     *         Received message content
     * @notes Other users in the same room will receive this callback when they call sendRoomMessage{@link #IRTCRoom#sendRoomMessage} to send a broadcast message.
     */
    virtual void onRoomMessageReceived(const char* uid, const char* message) {
        (void)uid;
        (void)message;
    }

    /** 
     * @type callback
     * @region Real-time messaging
     * @brief Receives a callback to a binary broadcast message in the room.
     * @param  [in] uid <br>
     *         User ID of the message sender
     * @param  [in] size <br>
     *         Length of the received binary message
     * @param  [in] message <br>
     *        Binary message content received
     * @notes Other users in the same room call sendRoomBinaryMessage{@link #IRTCRoom#sendRoomBinaryMessage} Receive this callback when sending a binary broadcast message.
     */
    virtual void onRoomBinaryMessageReceived(const char* uid, int size, const uint8_t* message) {
        (void)uid;
        (void)size;
        (void)message;
    }
    /** 
     * @type callback
     * @region Real-time messaging
     * @brief Receive this callback when you receive a text message (P2P) from another user in the room.
     * @param  [in] uid <br>
     *         User ID of the message sender.
     * @param  [in] message <br>
     *         The content of the received text message.
     * @notes   <br>
     *         + You must call the sendUserMessage{@link #IRTCRoom#sendUserMessage} method before you can receive the callback.
     */
    virtual void onUserMessageReceived(const char* uid, const char* message) {
        (void)uid;
        (void)message;
    }

    /** 
     * @type callback
     * @region Real-time Message Communication
     * @brief A single user receives a callback (P2P) of binary messages from other uid-owned users in the same room.
     * @param [in] uid User ID of the message sender.
     * @param [in] size Message length.
     * @param [in] message The content of the received binary message.
     */
    virtual void onUserBinaryMessageReceived(const char* uid, int size, const uint8_t* message) {
        (void)uid;
        (void)size;
        (void)message;
    }

    /** 
     * @type callback
     * @region Real-time messaging
     * @brief After sending a text or binary message (P2P) to a single user in the room, the message sender receives a callback with the result of the message.
     * @param  [in] msgid <br>
     *             The ID of this message.
     * @param  [in] error <br>
     *             Text or binary message sending results. See UserMessageSendResult{@link #UserMessageSendResult}
     * @notes   <br>
     *         + You must first call the sendUserMessage{@link #IRTCRoom#sendUserMessage} or sendUserBinaryMessage{@link #IRTCRoom#sendUserBinaryMessage} interface to receive this callback.
     */
    virtual void onUserMessageSendResult(int64_t msgid, int error) {
        (void)msgid;
        (void)error;
    }

    /** 
     * @type callback
     * @region Real-time messaging
     * @brief When the sendRoomMessage{@link #IRTCRoom#sendRoomMessage} function is called to send a message, the sending result of this message (feedback) is called back.
     * @param  [in] msgid <br>
     *         ID of this message
     * @param  [in] error <br>
     *         Message sending result <br>
     *         See RoomMessageSendResult{@link #RoomMessageSendResult}
     */
    virtual void onRoomMessageSendResult(int64_t msgid, int error) {
        (void)msgid;
        (void)error;
    }

    /**
     * @hidden for internal use only
     */
    virtual void onRoomModeChanged(RtcRoomMode mode) {
        (void)mode;
    }

#ifndef ByteRTC_AUDIO_ONLY
    /**
     * @hidden for internal use only
     * @brief callback when the maximum screen share fps is changed
     * @param [in] fps maximum screen share fps
     */
    virtual void onMaximumScreenShareFpsUpdated(int fps) {
        (void)fps;
    }

    /** 
     * @hidden for internal use only
     * @brief Callback when the maximum screen shared pixels (resolution) is chang
     * @param [in] screenPixels The recommended maximum video width × height value to maintain the frame rate.
     */
    virtual void onMaximumScreenSharePixelsUpdated(int screenPixels) {
        (void)screenPixels;
    }

    /** 
     * @type callback
     * @region Video management
     * @brief All the users in the room will get informed via this callback when a user is banned or the ban of the user has been lifted by calling BanUserStream/UnbanUserStream on the server side.
     * @param [in] uid User ID of the video stream that was disabled/unbanned
     * @param [in] banned Video stream sending status <br>
     *         + true: Video stream sending was disabled <br>
     *         + false: Video stream sending was unbanned
     * @notes   <br>
     *         + When the specified user in the room is disabled/unbanned Video stream sending, all users in the room will receive the callback. <br>
     *        + If the banned user leaves or disconnects and then rejoins the room, the user is still banned from publishing audio stream, and all users in the room will be informed via the callback. <br>
     *         + After the specified user is banned, other users in the room will check out and enter the room again, and will receive the callback again. <br>
     *         + If the conference mode is enabled in the console, only the user whose stream is banned will receive this callback.
     */
    virtual void onVideoStreamBanned(const char* uid, bool banned) {
        (void)uid;
        (void)banned;
    }

    /** 
     * @type callback
     * @region Multi-Room
     * @brief Stream publisher will receive this callback when the A/V synchronization state changes after setMultiDeviceAVSync{@link #IRTCRoom#setMultiDeviceAVSync} is called.
     * @param state A/V synchronization state, see AVSyncState{@link #AVSyncState}.
     */
    virtual void onAVSyncStateChange(AVSyncState state) {
        (void)state;
    }

#endif  // ByteRTC_AUDIO_ONLY
    /** 
     * @type callback
     * @region Audio event callback
     * @brief All the users in the room will get informed via this callback when a user is banned or the ban of the user has been lifted by calling BanUserStream/UnbanUserStream on the server side.
     * @param  [in] uid Disabled/unbanned audio stream user ID
     * @param  [in] banned Audio stream sending status <br>
     *         + true: audio stream sending is disabled <br>
     *         + false: audio stream sending is unbanned
     * @notes   <br>
     *         + Specified users in the room are prohibited/all users in the room when audio stream sending is unbanned Will receive the callback. <br>
     *        + If the banned user leaves or disconnects and then rejoins the room, the user is still banned from publishing audio stream, and all users in the room will be informed via the callback. <br>
     *         + After the specified user is banned, other users in the room will check out and enter the room again, and will receive the callback again. <br>
     *         + The same room is created again after dissolution, and the state in the room is empty.  <br>
     *         + If the conference mode is enabled in the console, only the user whose stream is banned will receive this callback.
     */
    virtual void onAudioStreamBanned(const char* uid, bool banned) {
        (void)uid;
        (void)banned;
    }

    /** 
     * @type callback
     * @region Multi-room
     * @brief Callback returning the state and errors during relaying the media stream to each of the rooms
     * @param [in] infos Array of the state and errors of each target room. Refer to ForwardStreamStateInfo{@link #ForwardStreamStateInfo} for more information.
     * @param [in] info_count The number of the target rooms
     */
    virtual void onForwardStreamStateChanged(ForwardStreamStateInfo* infos, int info_count) {
    }

    /** 
     * @type callback
     * @region Multi-room
     * @brief Callback returning the events during relaying the media stream to each room
     * @param [in] infos Array of the event of each target room. Refer to ForwardStreamEventInfo{@link #ForwardStreamEventInfo} for more information.
     * @param [in] info_count The number of the target rooms
     */
    virtual void onForwardStreamEvent(ForwardStreamEventInfo* infos, int info_count) {
    }

    /** 
     * @type callback
     * @brief Report the network quality of the users every 2s after the local user joins the room and starts publishing or subscribing to a stream.
     * @param [in] localQuality Local network quality. Refer to NetworkQualityStats{@link #NetworkQualityStats} for details.
     * @param [in] remoteQualities Network quality of the subscribed users. Refer to NetworkQualityStats{@link #NetworkQualityStats} for details.
     * @param [in] remoteQualityNum Array length of `remoteQualities`
     * @notes See [In-call Stats](106866) for more information.
     */
    virtual void onNetworkQuality(const NetworkQualityStats& localQuality, const NetworkQualityStats* remoteQualities, int remoteQualityNum) {
    }

    /** 
     * @valid since 3.52.
     * @type callback
     * @region Room Management
     * @brief Callback on the result of calling setRoomExtraInfo{@link #IRTCRoom#setRoomExtraInfo} to set extra information about the room.
     * @param [in] taskId The task ID of the API call.
     * @param [in] errCode See SetRoomExtraInfoResult{@link #SetRoomExtraInfoResult} for the setting results and reasons.
     */
    virtual void onSetRoomExtraInfoResult(int64_t taskId, SetRoomExtraInfoResult errCode) {
        (void)taskId;
        (void)errCode;
    }

    /** 
     * @valid since 3.52.
     * @type callback
     * @region Room Management
     * @brief Callback used to receive the extra information set by the other users in the same room with setRoomExtraInfo{@link #IRTCRoom#setRoomExtraInfo}.
     * @param [in] key Key of the extra information.
     * @param [in] value Content of the extra information.
     * @param [in] lastUpdateUserId The ID of the last user who updated this information.
     * @param [in] lastUpdateTimeMs The Unix time in ms when this information was last updated. 
     */
    virtual void onRoomExtraInfoUpdate(const char*key,const char* value,const char* lastUpdateUserId,int64_t lastUpdateTimeMs) {
        (void)key;
        (void)value;
        (void)lastUpdateUserId;
        (void)lastUpdateTimeMs;
    }

    /** 
     * @hidden currently not available 
     * @type callback
     * @region Subtitle translation service
     * @brief  Callback on subtitle states. <br>
     *         After you call startSubtitle{@link #IRTCRoom#startSubtitle} and stopSubtitle{@link #IRTCRoom#stopSubtitle}, you will receive this callback which informs you of the states and error codes of the subtitling task, as well as detailed information on the third party services' errors. 
     * @param [in] state The states of subtitles. Refer to SubtitleState{@link #SubtitleState} for details. 
     * @param [in] errorCode  Error codes of the subtitling task. Refer to SubtitleErrorCode{@link #SubtitleErrorCode}.
     * @param [in] errorMessage Detailed information on the third party services' errors. 
     */
    virtual void onSubtitleStateChanged(SubtitleState state, SubtitleErrorCode error_code, const char* error_message) {
    }

    /** 
     * @hidden currently not available 
     * @type callback
     * @region Subtitle translation service
     * @brief  Callback on subtitle messages.  <br>
     *         After calling startSubtitle{@link #IRTCRoom#startSubtitle}, you will receive this callback which informs you of the related information on subtitles. 
     * @param [in] subtitles  Subtitle messages. Refer to SubtitleMessage{@link #SubtitleMessage} for details. 
     * @param [in] cnt  The number of subtitle messages.
     */
    virtual void onSubtitleMessageReceived(const SubtitleMessage* subtitles, int cnt) {
    }
};

} // namespace bytertc

#endif // BYTE_RTC_ROOM_EVENT_HANDLER_INTERFACE_H__
