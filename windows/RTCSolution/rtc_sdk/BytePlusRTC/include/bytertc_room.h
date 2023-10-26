/*
 * Copyright (c) 2020 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Room Interface
 */

#pragma once

#include "rtc/bytertc_video_effect_interface.h"  // NOLINT
#include "rtc/bytertc_defines.h"
#include "bytertc_room_event_handler.h"
#include "rtc/bytertc_transcoder_interface.h"
#include "rtc/bytertc_publicstream_interface.h"
#include "rtc/bytertc_range_audio_interface.h"
#include "rtc/bytertc_spatial_audio_interface.h"
#include "rtc/bytertc_panoramic_video_interface.h"
namespace bytertc {

/** 
 * @type api
 * @brief  Room interface
 */
class IRTCRoom {
public:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IRTCRoom() {
    }

    /** 
     * @type api
     * @region  multi-room
     * @brief  Leave and destroy the room instance created by calling createRTCRoom{@link #IRTCVideo#createRTCRoom}.
     */
    virtual void destroy() = 0;

    /** 
     * @type api
     * @region Room management
     * @brief Set the visibility of the user in the room. The local user is visible to others by default before calling this API.
     *        An RTC room can accommodate a maximum of 50 visible users, and 30 media streams can be published simultaneously. For more information, see [Room Capacity](https://docs.byteplus.com/en/byteplus-rtc/docs/257549).
     * @param [in] enable Visibility of the user in the room. <br>
     *        + true: The user can publish media streams. The other users in the room get informed of the behaviors of the user, such as joining room, starting video capture, and leaving room.
     *        + false: The user cannot publish media streams. The other users in the room do not get informed of the behaviors of the user, such as joining room, starting video capture, or leaving room.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Failure. See ReturnStatus{@link #ReturnStatus}.
     * @notes   <br>
     *        + You can call this API whether the user is in a room or not.
     *        + You will receive onUserVisibilityChanged{@link #IRTCRoomEventHandler#onUserVisibilityChanged} after calling this API. (Available since v3.54)
     *   &#x0020;  • If you call this API before joining room, and the set value is different from the default value, you will receive the callback when you join the room.
     *   &#x0020;  • If you call this API after joining room, and the current visibility is different from the previous value, you will receive the callback.
     *   &#x0020;  • When reconnecting after losing internet connection, if the visibility changes, you will receive the callback. 
     *        + When you call this API while you are in the room, the other users in the room will be informed via the following callback.
     *   &#x0020;  • When you switch from `false` to `true`, they will receive onUserJoined{@link #IRTCRoomEventHandler#onUserJoined}.
     *   &#x0020;  • When you switch from `true` to `false`, they will receive onUserLeave{@link #IRTCRoomEventHandler#onUserLeave}.
     *   &#x0020;  • The invisible user will receive the warning code, `kWarningCodePublishStreamForbiden`, when trying to publish media streams.
     */
    virtual int setUserVisibility(bool enable) = 0;

    /** 
     * @type api
     * @region multiroom
     * @brief Listens for event callbacks related to the IRTCRoom{@link #IRTCRoom} instance by setting the event handler of this instance.
     * @param  [in] room_event_handler Refer to IRTCRoomEventHandler{@link #IRTCRoomEventHandler}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     */
    virtual int setRTCRoomEventHandler(IRTCRoomEventHandler* room_event_handler) = 0;

    /** 
     * @type api
     * @region Multi-room
     * @brief Join the room. <br>
     *        After creating a room by calling createRTCRoom{@link #IRTCVideo#createRTCRoom}, call this API to join the room and make audio & video calls with other users in the room. <br>
     * @param  [in] token Dynamic key for authenticating the logged-in user. <br>
     *         You need to bring Token to enter the room. When testing, you can use the console to generate temporary tokens. The official launch requires the use of the key SDK to generate and issue tokens at your server level. See [Authentication with Token](70121) for token validity and generation method. <br>
     *        + Apps with different App IDs cannot communicate with each other. <br>
     *        + Make sure that the App ID used to generate the Token is the same as the App ID used to create the engine, otherwise it will cause the join room to fail.
     * @param  [in] user_info User information. See UserInfo{@link #UserInfo}. <br>
     * @param  [in] config Room parameter configuration, set the room mode and whether to automatically publish or subscribe to the flow. See RTCRoomConfig{@link #RTCRoomConfig} for the specific configuration mode. <br>
     * @return   <br>
     *         + 0: Success <br>
     *         + -1: Room_id/user_info.uid contains invalid parameters. <br>
     *         + -2: Already in the room. After the interface call is successful, as long as the return value of 0 is received and leaveRoom:{@link #IRTCRoom#leaveRoom} is not called successfully, this return value is triggered when the room entry interface is called again, regardless of whether the filled room ID and user ID are duplicated. <br>
     *         The reason for the failure will be communicated via the onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} callback.<br>
     * @notes   <br>
     *        + In the same room with the same App ID, the user ID of each user must be unique. If two users have the same user ID, the user who entered the room later will kick the user who entered the room out of the room, and the user who entered the room will receive the onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} callback notification. For the error type. See kErrorCodeDuplicateLogin in ErrorCode{@link #ErrorCode}. <br>
     *        + Local users will receive onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} callback notification after calling this method to join the room successfully. If the local user is also a visible user, the remote user will receive an onUserJoined{@link #IRTCRoomEventHandler#onUserJoined} callback notification when joining the room.<br>
     *        + By default, the user is visible in an RTC room. Joining fails when the number of users in an RTC room reaches the upper limit. To avoid this, call setUserVisibility{@link #IRTCRoom#setUserVisibility} to change the visibility of the audience users to `false` by considering the capacity for the invisible users is much larger than that for visible users. An RTC room can accommodate a maximum of 50 visible users, and 30 media streams can be published simultaneously. For more information, see [Capability of Users and Streams](https://docs.byteplus.com/en/byteplus-rtc/docs/257549). <br>
     *        + After the user successfully joins the room, the SDK may lose connection to the server in case of poor local network conditions. At this point, onConnectionStateChanged{@link #IRTCVideoEventHandler#onConnectionStateChanged} callback will be triggered and the SDK will automatically retry until it successfully reconnects to the server. After a successful reconnection, you will receive a local onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} callback notification.
     */
    virtual int joinRoom(const char* token, const UserInfo& user_info, const RTCRoomConfig& config) = 0;

    /** 
     * @type api
     * @region  room management
     * @brief  Leave RTC room.   <br>
     *        The user calls this method to leave the RTC room, end the call process, and release all call-related resources. <br>
     *        It is an asynchronous operation, and the call returns without actually exiting the room. When actually exiting the room, you will receive onLeaveRoom{@link #IRTCRoomEventHandler#onLeaveRoom}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *        + When visible users leave the room, others in the room will receive onUserLeave{@link #IRTCRoomEventHandler#onUserLeave}. <br>
     *        + If the engine is destroyed immediately after this method is called, you will not receive onLeaveRoom{@link #IRTCRoomEventHandler#onLeaveRoom}. <br>
     */
    virtual int leaveRoom() = 0;

    /** 
     * @type api
     * @brief Update Token. 
     *        You must call this API to update token to ensure the RTC call to continue when you receive onTokenWillExpire{@link #IRTCRoomEventHandler#onTokenWillExpire}, onPublishPrivilegeTokenWillExpire{@link #IRTCRoomEventHandler#onPublishPrivilegeTokenWillExpire}, or onSubscribePrivilegeTokenWillExpire{@link #IRTCRoomEventHandler#onSubscribePrivilegeTokenWillExpire}.
     * @param token  Valid token.
     *        If the Token is invalid, you will receive onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} with the error code of `-1010`.
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes <br>
     *      + In versions before 3.50, the publish and subscribe privileges contained in the Token are reserved parameters with no actual control permissions. In version 3.50 and later, the publish/subscribe privileges will be effective for whitelisted users. Please contact the technical support team to include you in the whitelist if you need this function.
     *      + Do not call both updateToken{@link #IRTCRoom#updateToken} and joinRoom{@link #IRTCRoom#joinRoom} to update the Token. If you fail to join the room or have been removed from the room due to an expired or invalid Token, call joinRoom{@link #IRTCRoom#joinRoom} with a valid token to rejoin the room.
     */
    virtual int updateToken(const char* token) = 0;

    /** 
     * @type api
     * @region Multi-room
     * @brief Send a text message (P2P) to the specified user in the room
     * @param  [in] uid User ID of the message receiver
     * @param  [in] message <br>
     *        Text message content sent. <br>
     *         Message does not exceed 64 KB.
     * @param [in] config Message type, see MessageConfig{@link #MessageConfig}.
     * @return The number of the message sent this time is incremented from 1.
     * @notes   <br>
     *        + Before sending an in-room text message, you must call joinRoom{@link #IRTCRoom#joinRoom} to join the room. <br>
     *        + After calling this function, you will receive an onUserMessageSendResult{@link #IRTCRoomEventHandler#onUserMessageSendResult} callback to notify the message sender that the sending was successful or failed; <br>
     *        + If the text message is sent successfully, the user specified by uid will receive onUserMessageReceived{@link #IRTCRoomEventHandler#onUserMessageReceived} callback.
     */
    virtual int64_t sendUserMessage(const char* uid, const char* message, MessageConfig config = kMessageConfigReliableOrdered) = 0;

    /** 
     * @type api
     * @region multi-room
     * @brief Sends a binary message (P2P) to the specified user in the room
     * @param  [in] uid User ID of the message receiver
     * @param  [in] length <br>
     *        The length of the binary string.
     * @param  [in] message <br>
     *         The content of the binary message. The message does not exceed 46KB.
     *         The number of the message sent this time is incremented from
     * @param [in] config Message type, see MessageConfig{@link #MessageConfig}.
     * @return Serial number of the message, starting form 1.
     * @notes   + Before sending in-room binary messages, you must call joinRoom{@link #IRTCRoom#joinRoom} to join the room. <br>
     *        + After calling this function, you will receive an onUserMessageSendResult{@link #IRTCRoomEventHandler#onUserMessageSendResult} callback to notify the sender of the success or failure of the message; <br>
     *        + If the binary message is sent successfully, the user specified by uid will receive the onUserBinaryMessageReceived{@link #IRTCRoomEventHandler#onUserBinaryMessageReceived} callback.
     */
    virtual int64_t sendUserBinaryMessage(const char* uid, int length, const uint8_t* message, MessageConfig config = kMessageConfigReliableOrdered) = 0;

    /** 
     * @type api
     * @region Multi-room
     * @brief Send broadcast messages to all other users in the room.
     * @param  [in] message <br>
     *        The broadcast message sent by the user <br>
     *        Message does not exceed 64 KB.
     * @notes   <br>
     *        + Before sending in-room binary messages, you must call joinRoom{@link #IRTCRoom#joinRoom} to join the room. <br>
     *        + After calling this function, you get an onRoomMessageSendResult{@link #IRTCRoomEventHandler#onRoomMessageSendResult} callback. <br>
     *        + Other users in the same room receive an onRoomMessageReceived{@link #IRTCRoomEventHandler#onRoomMessageReceived} callback.
     */
    virtual int64_t sendRoomMessage(const char* message) = 0;
    /** 
     * @type api
     * @region Multi-room
     * @brief Send broadcast messages to all other users in the room.
     * @param  [in] size <br>
     *        The length of the binary message sent
     * @param  [in] message <br>
     *        The binary broadcast message sent by the user <br>
     *        The message does not exceed 46KB.
     * @return The number of the message sent this time is incremented from 1.
     * @notes   <br>
     *        + Before sending in-room binary messages, you must call joinRoom{@link #IRTCRoom#joinRoom} to join the room. <br>
     *        + After calling this function, you get an onRoomMessageSendResult{@link #IRTCRoomEventHandler#onRoomMessageSendResult} callback. <br>
     *        + Other users in the same room receive an onRoomBinaryMessageReceived{@link #IRTCRoomEventHandler#onRoomBinaryMessageReceived} callback.
     */
    virtual int64_t sendRoomBinaryMessage(int size, const uint8_t* message) = 0;

    /** 
     * @type api
     * @region Room Management
     * @brief Publishes media streams captured by camera/microphone in the current room.
     * @param [in] type Media stream type, used for specifying whether to publish audio stream or video stream. See MediaStreamType{@link #MediaStreamType}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *        + You don't need to call this API if you set it to Auto-publish when calling joinRoom{@link #IRTCRoom#joinRoom}.   <br>
     *        + An invisible user cannot publish media streams. Call setUserVisibility{@link #IRTCRoom#setUserVisibility} to change your visibility in the room.  <br>
     *        + Call publishScreen{@link #IRTCRoom#publishScreen} to start screen sharing. Not available on Linux.  <br>
     *        + Call startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms} to forward the published streams to the other rooms.  <br>
     *        + After you call this API, the other users in the room will receive onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}. Those who successfully received your streams will receive onFirstRemoteAudioFrame{@link #IRTCVideoEventHandler#onFirstRemoteAudioFrame}/onFirstRemoteVideoFrameDecoded{@link #IRTCVideoEventHandler#onFirstRemoteVideoFrameDecoded} at the same time.  <br>
     *        + Call unpublishStream{@link #IRTCRoom#unpublishStream} to stop publishing streams.
     */
    virtual int publishStream(MediaStreamType type) = 0;

    /** 
     * @type api
     * @region Room Management
     * @brief Stops publishing media streams captured by camera/microphone in the current room.
     * @param [in] type Media stream type, used for specifying whether to stop publishing audio stream or video stream. See MediaStreamType{@link #MediaStreamType}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *         + After calling publishStream{@link #IRTCRoom#publishStream}, call this API to stop publishing streams. <br>
     *         + After calling this API, the other users in the room will receive onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream}.
     */
    virtual int unpublishStream(MediaStreamType type) = 0;

    /** 
     * @type api
     * @region Screen Sharing
     * @brief Manually publishes local screen-sharing streams in the current room.<br>
     *        If you need to share your screen in multiple rooms, you can use the same uid to join multiple rooms and call this API in each room. Also, you can publish different types of screen-sharing streams in different rooms.
     * @param [in] type Media stream type, used for specifying whether to publish audio stream or video stream. See MediaStreamType{@link #MediaStreamType}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *        + You need to call this API to publish screen even if you set it to Auto-publish when calling joinRoom{@link #IRTCRoom#joinRoom}.   <br>
     *        + An invisible user cannot publish media streams. Call setUserVisibility{@link #IRTCRoom#setUserVisibility} to change your visibility in the room. <br>
     *        + After you called this API, the other users in the room will receive onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen}. Those who successfully received your streams will receive onFirstRemoteAudioFrame{@link #IRTCVideoEventHandler#onFirstRemoteAudioFrame}/onFirstRemoteVideoFrameDecoded{@link #IRTCVideoEventHandler#onFirstRemoteVideoFrameDecoded} at the same time.  <br>
     *        + After calling this API, you'll receive onScreenVideoFrameSendStateChanged{@link #IRTCVideoEventHandler#onScreenVideoFrameSendStateChanged}. <br>
     *        + Call unpublishScreen{@link #IRTCRoom#unpublishScreen} to stop publishing screen sharing streams. <br>
     *        + On Linux, you can only publish video streams.<br>
     *        + Refer to [Sharing Screen in PC](https://docs.byteplus.com/byteplus-rtc/docs/70144) for more information.
     */
    virtual int publishScreen(MediaStreamType type) = 0;

    /** 
     * @type api
     * @region Screen Sharing
     * @brief Stops publishing local screen sharing streams in the current room.
     * @param [in] type Media stream type, used for specifying whether to stop publishing audio stream or video stream. See MediaStreamType{@link #MediaStreamType}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *         + After calling publishScreen{@link #IRTCRoom#publishScreen}, call this API to stop publishing streams. <br>
     *         + After calling this API, the other users in the room will receive onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen}.
     */
    virtual int unpublishScreen(MediaStreamType type) = 0;

    /** 
     * @deprecated since 3.45 and will be deleted in 3.51, use subscribeStream{@link #IRTCRoom#subscribeStream}, unsubscribeStream{@link #IRTCRoom#unsubscribeStream}, subscribeScreen{@link #IRTCRoom#subscribeScreen} and unsubscribeScreen{@link #IRTCRoom#unsubscribeScreen} instead.
     * @type api
     * @region Multi-room
     * @brief Subscribe to media streams in the room.   <br>
     *        Call this API to subscribe to media streams in the room regardless of subscription mode, either Auto or Manual. <br>
     *        Call this API to update the properties and configurations of the stream you have already subscribed to. <br>
     * @param  [in] user_id User ID of the publisher of the subscribed media stream <br>
     * @param  [in] stream_type Mainstream or screen-sharing stream. Refer to StreamIndex{@link #StreamIndex} for details.
     * @param  [in] media_type Media stream type to be subscribed to. Refer to  SubscribeMediaType{@link #SubscribeMediaType} for details. <br>
     * @param  [in] video_config Subscription configuration of video streams. Refer to SubscribeVideoConfig{@link #SubscribeVideoConfig} for details. <br>
     * @notes   <br>
     *         + If the subscription fails, you will receive onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged}. Refer to ErrorCode{@link #ErrorCode} for the specific reason of failure.
     *         + You can update the subscription configuration by calling the API even after calling pauseAllSubscribedStream{@link #IRTCRoom#pauseAllSubscribedStream} to pause streaming. Call resumeAllSubscribedStream{@link #IRTCRoom#resumeAllSubscribedStream} to resume streaming and apply the changes. <br>
     */
    BYTERTC_DEPRECATED virtual int subscribeUserStream(const char* user_id, StreamIndex stream_type, SubscribeMediaType media_type, const SubscribeVideoConfig& video_config) = 0;
    /** 
     * @type api
     * @region Video Management
     * @brief Sets your expected configuration of the remote video stream that you want to subscribe to or have subscribed to.
     * @param [in] user_id ID of the remote video stream publisher you expect to configure subscription parameters for.
     * @param [in] remote_video_config The parameters you expect to configure for the remote video stream, see RemoteVideoConfig{@link #RemoteVideoConfig}.
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes <br>
     *        + This API only works after the publisher calls enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode} to enable publishing multiple video streams, in which case the subscriber will receive the stream from the publisher that is closest to the set configuration;  otherwise the subscriber will only receive one video stream with a resolution of 640px × 360px and a frame rate of 15fps.  <br>
     *        + If you don't call this API after the publisher enables the function of publishing multiple streams, you will receive by default the video stream with the largest resolution set by the publisher.  <br>
     *        + You should call this API in the room. If you want to call it before entering the room, you should set the `remote_video_config` in the `room_config` when calling joinRoom{@link #IRTCRoom#joinRoom}.  <br>
     *        + SDK will automatically select the stream to be published or subscribed based on the settings of both sides.
     */
    virtual int setRemoteVideoConfig(const char *user_id,const RemoteVideoConfig &remote_video_config) = 0;

    /** 
     * @type api
     * @region Room Management
     * @brief Subscribes to specific remote media streams captured by camera/microphone.  Or update the subscribe options of the subscribed user.
     * @param [in] user_id The ID of the remote user who published the target audio/video stream.
     * @param [in] type Media stream type, used for specifying whether to subscribe to the audio stream or the video stream. See MediaStreamType{@link #MediaStreamType}.
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes  <br>
     *        + Calling this API to update the subscribe configuration when the user has subscribed the remote user either by calling this API or by auto-subscribe.  <br>
     *        + You must first get the remote stream information through onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}} before calling this API to subscribe to streams accordingly.  <br>
     *        + After calling this API, you will be informed of the calling result with onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed}.  <br>
     *        + Once the local user subscribes to the stream of a remote user, the subscription to the remote user will sustain until the local user leaves the room or unsubscribe from it by calling unsubscribeStream{@link #IRTCRoom#unsubscribeStream}.<br>
     *        + Any other exceptions will be included in onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged}, see ErrorCode{@link #ErrorCode} for the reasons.
     */
    virtual int subscribeStream(const char *user_id,MediaStreamType type) = 0;
    /** 
     * @type api
     * @region Room Management
     * @brief Subscribes to all remote media streams captured by camera/microphone. Or update the subscribe options of all subscribed user.
     * @param [in] type Media stream type, used for specifying whether to subscribe to the audio stream or the video stream. See MediaStreamType{@link #MediaStreamType}.
     * @return API call result:  <br>
     *        + 0: Success  <br>
     *        + !0: Failure
     * @notes  <br>
     *        + If the subscription options conflict with the previous ones, they are subject to the configurations in the last call.<br>
     *        + With the Audio selection enabled, if the number of media streams exceeds the limit, we recommend you call subscribeStream{@link #IRTCRoom#subscribeStream} to subscribe each target media stream other than calling this API.<br>
     *        + After calling this API, you will be informed of the calling result with onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed}.  <br>
     *        + Once the local user subscribes to the stream of a remote user, the subscription to the remote user will sustain until the local user leaves the room or unsubscribe from it by calling unsubscribeStream{@link #IRTCRoom#unsubscribeStream}.<br>
     *        + Any other exceptions will be included in onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged}, see ErrorCode{@link #ErrorCode} for the reasons.
     */
    virtual int subscribeAllStreams(MediaStreamType type) = 0;

    /** 
     * @type api
     * @region Room Management
     * @brief Unsubscribes from specific remote media streams captured by camera/microphone.  <br>
     *        You can call this API in both automatic subscription mode and manual subscription mode.
     * @param [in] user_id The ID of the remote user who published the target audio/video stream.
     * @param [in] type Media stream type, used for specifying whether to unsubscribe from the audio stream or the video stream. See MediaStreamType{@link #MediaStreamType}.
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes  <br>
     *        + After calling this API, you will be informed of the calling result with onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed}.  <br>
     *        + Any other exceptions will be included in onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged}, see ErrorCode{@link #ErrorCode} for the reasons.
     */
    virtual int unsubscribeStream(const char *user_id,MediaStreamType type) = 0;
    /** 
     * @type api
     * @region Room Management
     * @brief Unsubscribes from all remote media streams captured by camera/microphone.  <br>
     *        You can call this API to unsubscribe from streams that are subscribed to either automatically or manually.
     * @param [in] type Media stream type, used for specifying whether to unsubscribe from the audio stream or the video stream. See MediaStreamType{@link #MediaStreamType}.
     * @return API call result:  <br>
     *        + 0: Success  <br>
     *        + !0: Failure
     * @notes  <br>
     *        + After calling this API, you will be informed of the calling result with onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed}.  <br>
     *        + Any other exceptions will be included in onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged}, see ErrorCode{@link #ErrorCode} for the reasons.
     */
    virtual int unsubscribeAllStreams(MediaStreamType type) = 0;
    /** 
     * @type api
     * @region Room Management
     * @brief Subscribes to specific screen sharing media stream.   Or update the subscribe options of the subscribed user.
     * @param [in] user_id The ID of the remote user who published the target screen audio/video stream.
     * @param [in] type Media stream type, used for specifying whether to subscribe to the audio stream or the video stream. See MediaStreamType{@link #MediaStreamType}.
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes  <br>
     *        + Calling this API to update the subscribe configuration when the user has subscribed the remote user either by calling this API or by auto-subscribe.  <br>
     *        + You must first get the remote stream information through onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen}} before calling this API to subscribe to streams accordingly.  <br>
     *        + After calling this API, you will be informed of the calling result with onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed}.  <br>
     *        + Once the local user subscribes to the stream of a remote user, the subscription to the remote user will sustain until the local user leaves the room or unsubscribe from it by calling unsubscribeScreen{@link #IRTCRoom#unsubscribeScreen}.<br>
     *        + Any other exceptions will be included in onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged}, see ErrorCode{@link #ErrorCode} for the reasons.
     */
    virtual int subscribeScreen(const char *user_id,MediaStreamType type) = 0;

    /** 
     * @type api
     * @region Room Management
     * @brief Unsubscribes from specific screen sharing media stream.  <br>
     *        You can call this API in both automatic subscription mode and manual subscription mode.
     * @param [in] user_id The ID of the remote user who published the target screen audio/video stream.
     * @param [in] type Media stream type, used for specifying whether to unsubscribe from the audio stream or the video stream. See MediaStreamType{@link #MediaStreamType}.
     * @return API call result:
     *        + 0: Success.
     *        + <0: Failure. See ReturnStatus{@link #ReturnStatus} for specific reasons.
     * @notes  <br>
     *        + After calling this API, you will be informed of the calling result with onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed}.  <br>
     *        + Any other exceptions will be included in onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged}, see ErrorCode{@link #ErrorCode} for the reasons.
     */
    virtual int unsubscribeScreen(const char *user_id,MediaStreamType type) = 0;

    /**
     * @hidden for internal use only.
     */
    virtual int enableSubscribeLocalStream(bool enable) = 0;

    /** 
     * @type api
     * @region Multi-room
     * @brief Pause receiving remote media streams.
     * @param  [in] media_type Media stream type subscribed to. Refer to PauseResumeControlMediaType{@link #PauseResumeControlMediaType}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes   <br>
     *         + Calling this API does not change the capture state and the transmission state of the remote clients. <br>
     *         + Calling this API does not cancel the subscription or change any subscription configuration. <br>
     *         + To resume, call resumeAllSubscribedStream{@link #IRTCRoom#resumeAllSubscribedStream}. <br>
     *         + In a multi-room scenario, this API only pauses the reception of streams published in the current room.
     */
    virtual int pauseAllSubscribedStream(PauseResumeControlMediaType media_type) = 0;

    /** 
     * @type api
     * @region  multi-room
     * @brief  Resume receiving remote media streams
     * @param  [in] media_type Media stream type subscribed to. Refer to PauseResumeControlMediaType{@link #PauseResumeControlMediaType}
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *         + Calling this API does not change the capture state and the transmission state of the remote clients.
     *         + Calling this API does not change any subscription configuration.
     */
    virtual int resumeAllSubscribedStream(PauseResumeControlMediaType media_type) = 0;


    /** 
     * @type api
     * @region Multi-room
     * @brief Synchronizes published audio and video.  <br>
     *        When the same user simultaneously uses separate devices to capture and publish audio and video, there is a possibility that the streams are out of sync due to the network disparity. In this case, you can call this API on the video publisher side and the SDK will automatically line the video stream up according to the timestamp of the audio stream, ensuring that the audio the receiver hears corresponds to the video the receiver watches.
     * @param [in] audio_user_id The ID of audio publisher. You can stop the current A/V synchronization by setting this parameter to null.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes  <br>
     *        + You can call this API anytime before or after entering the room.  <br>
     *        + The source user IDs of the audio and video stream to be synchronized must be in the same RTC room.  <br>
     *        + When the A/V synchronization state changes, you will receive onAVSyncStateChange{@link #IRTCRoomEventHandler#onAVSyncStateChange}.  <br>
     *        + More than one pair of audio and video can be synchronized simultaneously in the same RTC room, but you should note that one single audio source cannot be synchronized with multiple video sources at the same time.  <br>
     *        + If you want to change the audio source, call this API again with a new `audio_user_id`. If you want to change the video source, you need to stop the current synchronization first, then call this API on the new video publisher side.
     */
    virtual int setMultiDeviceAVSync(const char* audio_user_id) = 0;

    /**
     * @hidden for internal use only.
     */
    virtual int updateCloudRendering(const char* cloudrender_json_string) = 0;


    /**
     * @hidden for internal use only.
     */
    virtual int setCustomUserRole(const char* role) = 0;

    /** 
     * @type api
     * @region Multi-room
     * @brief Start relaying media stream across rooms. <br>
     *        After calling joinRoom{@link #IRTCRoom#joinRoom}, you can call this method to publish the media stream to multiple rooms that applies to scenarios such as anOnline talent contest and so on.
     * @param [in] configuration Information of the rooms where you want to relay the media stream to. Refer to ForwardStreamConfiguration{@link #ForwardStreamConfiguration} for more information.
     * @return  <br>
     *        0: Success
     *        <0: Failure  <br>
     * @notes <br>
     *        + Call this method will trigger onForwardStreamStateChanged{@link #IRTCRoomEventHandler#onForwardStreamStateChanged}.
     *        + After calling this method, listen the events from each room during the relaying by registering onForwardStreamEvent{@link #IRTCRoomEventHandler#onForwardStreamEvent}.
     *        + Once the relaying begins, the other users in the room will receive callback of onUserJoined{@link #IRTCRoomEventHandler#onUserJoined} and onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}/onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen}.
     *        + Call updateForwardStreamToRooms{@link #IRTCRoom#updateForwardStreamToRooms} to add or remove the target room(s) after calling this method.
     *        + Call stopForwardStreamToRooms{@link #IRTCRoom#stopForwardStreamToRooms} to stop relaying to all rooms after calling this method.
     *        + Call pauseForwardStreamToAllRooms{@link #IRTCRoom#pauseForwardStreamToAllRooms} to pause relaying to all rooms after calling this method.
     */
    virtual int startForwardStreamToRooms(const ForwardStreamConfiguration& configuration) = 0;

    /** 
     * @type api
     * @region Multi-room
     * @brief Update information of the rooms where you want to relay the media stream to after calling startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms}. <br>
     *        Adding and removing rooms by calling this method will trigger onForwardStreamStateChanged{@link #IRTCRoomEventHandler#onForwardStreamStateChanged} on the local.
     * @param [in] configuration Information of the rooms where you want to relay the media stream to. Refer to ForwardStreamConfiguration{@link #ForwardStreamConfiguration} for more information.
     * @return  <br>
     *         + 0: Success <br>
     *         + <0: Failure
     * @notes <br>
     *        + Users in the room which is added by calling this method will receive onUserJoined{@link #IRTCRoomEventHandler#onUserJoined} and onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}/onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen}.
     *        + Users in the room which is removed by calling this method will receive onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream}/onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen} and onUserLeave{@link #IRTCRoomEventHandler#onUserLeave}.
     */
    virtual int updateForwardStreamToRooms(const ForwardStreamConfiguration& configuration) = 0;

    /** 
     * @type api
     * @region Multi-room
     * @brief Call to this method to stop relaying media stream to all rooms after calling startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms}. <br>
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes <br>
     *        + Call this method will trigger onForwardStreamStateChanged{@link #IRTCRoomEventHandler#onForwardStreamStateChanged}.
     *        + The other users in the room will receive callback of onUserJoined{@link #IRTCRoomEventHandler#onUserJoined} and onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}/onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen} when you stop relaying.
     *        + To stop relaying media stream to specific rooms, call updateForwardStreamToRooms{@link #IRTCRoom#updateForwardStreamToRooms} instead.
     *        + To resume the relaying in a short time, call pauseForwardStreamToAllRooms{@link #IRTCRoom#pauseForwardStreamToAllRooms} instead and then call resumeForwardStreamToAllRooms{@link #IRTCRoom#resumeForwardStreamToAllRooms} to recsume after that.
     */
    virtual int stopForwardStreamToRooms() = 0;

    /** 
     * @type api
     * @region Multi-room
     * @brief Call this method to pause relaying media stream to all rooms after calling startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms}. <br>
     *        After that, call resumeForwardStreamToAllRooms{@link #IRTCRoom#resumeForwardStreamToAllRooms} to resume.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes The other users in the room will receive callback of onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream}/onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen} and onUserLeave{@link #IRTCRoomEventHandler#onUserLeave} when you pause relaying.
     */
    virtual int pauseForwardStreamToAllRooms() = 0;

    /** 
     * @type api
     * @region Multi-room
     * @brief Call this method to resume relaying to all rooms from the pause by calling pauseForwardStreamToAllRooms{@link #IRTCRoom#pauseForwardStreamToAllRooms}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes The other users in the room will receive callback of onUserJoined{@link #IRTCRoomEventHandler#onUserJoined} and onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}/onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen} when you resume relaying.
     */
    virtual int resumeForwardStreamToAllRooms() = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Range Audio
     * @brief Gets range audio instance.
     * @return API call result:  <br>
     *        + IRangeAudio: Success. You will get an IRangeAudio{@link #IRangeAudio} returned from the SDK.  <br>
     *        + nullptr: Failure. The current SDK does not offer range audio function.
     * @notes The first time this API is called must be between you create a room and you actually enter the room. Refer to [Range Voice](https://docs.byteplus.com/byteplus-rtc/docs/114727) for more information.
     */
    virtual IRangeAudio* getRangeAudio() = 0;

    /** 
     * @hidden(Linux)
     * @type api
     * @region Spatial Audio
     * @brief Gets spatial audio instance.   <br>
     * @return API call result:  <br>
     *        + ISpatialAudio: Success. You will get an ISpatialAudio{@link #ISpatialAudio} returned from the SDK.  <br>
     *        + nullptr: Failure. The current SDK does not offer spatial audio function.
     * @notes   <br>
     *        + The first time this API is called must be between you create a room and you actually enter the room.  Refer to [Spatial Audio](https://docs.byteplus.com/byteplus-rtc/docs/93903) for more information.<br>
     *        + Only using real dual-channel playback device can you actually enjoy spatial audio effect.  <br>
     *        + Low-end device may have audio stalling issues due to its poor performance, so it is not recommended to enable spatial audio function on such kind of devices. <br>
     *        + SDK currently supports up to 30 users publishing audio with spatial audio enabled at the same time in the same room.
     */
    virtual ISpatialAudio* getSpatialAudio() = 0;
    /** 
     * @type api
     * @region  multi-room
     * @brief Adjusts the audio playback volume from all the remote users in a room.
     * @param [in] volume Ratio(%) of playback volume to original volume, in the range [0, 400], with overflow protection.  <br>
     *               To ensure the audio quality, we recommend setting the volume to `100`.  <br>
     *               + 0: mute <br>
     *               + 100: original volume. Default value. <br>
     *               + 400: Up to 4 times the original volume (with overflow protection) <br>
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details
     * @notes Suppose a remote user A is always within the range of the target user whose playback volume will be adjusted,<br>
     *        + If you use both this method and setRemoteAudioPlaybackVolume{@link #IRTCVideo#setRemoteAudioPlaybackVolume}, the volume that the local user hears from user A is the volume set by the method called later.<br>
     *        + If you use both this method and setPlaybackVolume{@link #IRTCVideo#setPlaybackVolume}, the volume that the local user hears from user A is the overlay of both settings.
     */
    virtual int setRemoteRoomAudioPlaybackVolume(int volume) = 0;
    /** 
     * @hidden for internal use only on Windows and Android
     * @type api
     * @brief Gets panoramic video engine.
     * @return API call result:  <br>
     *        + IPanoramicVideo: Success. You will get an IPanoramicVideo{@link #IPanoramicVideo} returned from the SDK.  <br>
     *        + null: Failure. The current SDK does not offer range audio function.
     * @notes <br>
     *        + The API call sequence on the subscriber side is createRTCVideo{@link #IRTCVideo#createRTCVideo} > createRTCRoom{@link #IRTCVideo#createRTCRoom} > this API > joinRoom{@link #RTCRoom#joinRoom}.<br>
     *        + After calling this API, you can call updateQuaternionf{@link #IPanoramicVideo#updateQuaternionf} to update the position of the head represented as a quaternion.<br>
     *        + Refer to setRemoteVideoSink{@link #IRTCVideo#setRemoteVideoSink} for details on how to render the panoramic video with an external renderer.
     */
    virtual IPanoramicVideo* getPanoramicVideo() = 0;

    /** 
     * @valid since 3.52.
     * @type api
     * @region Room Management
     * @brief Set the priority of the local audio stream to be published.
     * @param [in] audio_selection_priority The priority of the local audio stream which defaults to be subscribable only up to the result of the Audio Selection. Refer to AudioSelectionPriority{@link #AudioSelectionPriority}.
     * @notes <br>
     *       + You must enable Audio Selection in the RTC console before using this API. You can call this API whether the user has joined a room. Refer to [Audio Selection](https://docs.byteplus.com/byteplus-rtc/docs/113547).
     *       + The setting is independent in each room that the user joins.
     */
    virtual int setAudioSelectionConfig(AudioSelectionPriority audio_selection_priority) = 0;

    /** 
     * @valid since 3.52.
     * @type api
     * @region Room Management
     * @brief Sets extra information about the room the local user joins.
     * @param [in] key Key of the extra information, less than 10 bytes in length.<br>
     *        A maximum of 5 keys can exist in the same room, beyond which the first key will be replaced.
     * @param [in] value Content of the extra information, less than 128 bytes in length.
     * @return API call result: <br>
     *        + 0: Success with a taskId returned.  <br>
     *        + <0: Failure. See SetRoomExtraInfoResult{@link #SetRoomExtraInfoResult} for the reasons.
     * @notes  <br>
     *        + Call joinRoom{@link #IRTCRoom#joinRoom} first before you call this API to set extra information. <br>
     *        + After calling this API, you will receive onSetRoomExtraInfoResult{@link #IRTCRoomEventHandler#onSetRoomExtraInfoResult} callback informing you the result of the setting. <br>
     *        + After the extra information is successfully set, other users in the same room will receive the information through onRoomExtraInfoUpdate{@link #IRTCRoomEventHandler#onRoomExtraInfoUpdate} callback.
     *        + Users who join the room later will be notified of all extra information in the room set prior to entering.
     */
    virtual int64_t setRoomExtraInfo(const char*key,const char*value) = 0;

    /** 
     * @hidden currently not available
     * @type api
     * @region Subtitle translation service
     * @brief Recognizes or translates the speech of all speakers in the room and converts the results into captions. <br>
     *        After calling this method, you will receive related information about subtitles through the onSubtitleMessageReceived{@link #IRTCRoomEventHandler#onSubtitleMessageReceived} callback.  <br>
     *        After calling this method, you will receive the onSubtitleStateChanged{@link #IRTCRoomEventHandler#onSubtitleStateChanged} to inform you of whether subtitles are on. 
     * @param [in] subtitle_config Subtitle configurations. Refer to SubtitleConfig{@link #SubtitleConfig} for details. 
     * @return  <br>
     *        +  0: Success.  <br>
     *        + !0: Failure.  
     * @notes <br>
     *        + Call this method after joining the room.  <br>
     *        + You can set your source language to Chinese by calling `joinRoom`  and importing a json formatted string `"source_language": "zh"` through the parameter of extraInfo, to English by importing `"source_language": "en"` , and to Japanese by importing `"source_language": "ja"` . If you don't set the source language, SDK will set the language of your system as the source language. If the language of your system is not Chinese, English or Japanese, SDK will set Chinese as the source language. 
     */
    virtual int startSubtitle(const SubtitleConfig& subtitle_config) = 0;
    /** 
     * @hidden currently not available
     * @type api
     * @region Subtitle translation service
     * @brief Turns off subtitles. 
     *        After calling this method, you will receive the onSubtitleStateChanged{@link #IRTCRoomEventHandler#onSubtitleStateChanged} to inform you of whether subtitles are off. 
     * @return  <br>
     *        +  0: Success.  <br>
     *        + !0: Failure.  
     */
    virtual int stopSubtitle() = 0;
    /** 
     * @valid since 3.53
     * @type api
     * @brief Get room ID.
     * @return Room ID.
     */
    virtual const char* getRoomId() = 0;
};

} // namespace bytertc
