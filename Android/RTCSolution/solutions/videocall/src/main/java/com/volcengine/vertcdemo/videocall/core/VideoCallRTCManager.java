// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.core;

import static com.ss.bytertc.engine.VideoCanvas.RENDER_MODE_FIT;
import static com.ss.bytertc.engine.VideoCanvas.RENDER_MODE_HIDDEN;
import static com.ss.bytertc.engine.data.EffectBeautyMode.EFFECT_SHARPEN_MODE;
import static com.ss.bytertc.engine.data.EffectBeautyMode.EFFECT_SMOOTH_MODE;
import static com.ss.bytertc.engine.data.EffectBeautyMode.EFFECT_WHITE_MODE;
import static com.ss.bytertc.engine.type.NetworkQuality.NETWORK_QUALITY_UNKNOWN;
import static com.volcengine.vertcdemo.videocall.core.Constants.VOLUME_SPEAKING_INTERVAL;
import static com.volcengine.vertcdemo.videocall.core.Constants.VOLUME_SPEAKING_THRESHOLD;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Process;
import android.text.TextUtils;
import android.util.Log;
import android.util.Pair;
import android.view.TextureView;

import androidx.annotation.NonNull;

import com.ss.bytertc.engine.RTCRoom;
import com.ss.bytertc.engine.RTCRoomConfig;
import com.ss.bytertc.engine.RTCVideo;
import com.ss.bytertc.engine.UserInfo;
import com.ss.bytertc.engine.VideoCanvas;
import com.ss.bytertc.engine.VideoEncoderConfig;
import com.ss.bytertc.engine.data.AudioPropertiesConfig;
import com.ss.bytertc.engine.data.AudioRoute;
import com.ss.bytertc.engine.data.CameraId;
import com.ss.bytertc.engine.data.LocalAudioPropertiesInfo;
import com.ss.bytertc.engine.data.MirrorType;
import com.ss.bytertc.engine.data.RemoteAudioPropertiesInfo;
import com.ss.bytertc.engine.data.RemoteStreamKey;
import com.ss.bytertc.engine.data.StreamIndex;
import com.ss.bytertc.engine.type.AudioProfileType;
import com.ss.bytertc.engine.type.AudioScenarioType;
import com.ss.bytertc.engine.type.ChannelProfile;
import com.ss.bytertc.engine.type.LocalStreamStats;
import com.ss.bytertc.engine.type.MediaStreamType;
import com.ss.bytertc.engine.type.NetworkQualityStats;
import com.ss.bytertc.engine.type.RemoteStreamStats;
import com.ss.bytertc.engine.type.StreamRemoveReason;
import com.volcengine.vertcdemo.common.GsonUtils;
import com.volcengine.vertcdemo.common.SolutionToast;
import com.volcengine.vertcdemo.utils.AppUtil;
import com.volcengine.vertcdemo.core.SolutionDataManager;
import com.volcengine.vertcdemo.core.eventbus.SolutionDemoEventManager;
import com.volcengine.vertcdemo.core.net.rts.RTCRoomEventHandlerWithRTS;
import com.volcengine.vertcdemo.core.net.rts.RTCVideoEventHandlerWithRTS;
import com.volcengine.vertcdemo.core.net.rts.RTSInfo;
import com.volcengine.vertcdemo.videocall.R;
import com.volcengine.vertcdemo.videocall.bean.VideoCallUserInfo;
import com.volcengine.vertcdemo.videocall.event.AudioPropertiesReportEvent;
import com.volcengine.vertcdemo.videocall.event.AudioRouterEvent;
import com.volcengine.vertcdemo.videocall.event.LocalStreamStatsEvent;
import com.volcengine.vertcdemo.videocall.event.MediaStatusEvent;
import com.volcengine.vertcdemo.core.eventbus.SDKReconnectToRoomEvent;
import com.volcengine.vertcdemo.videocall.event.RemoteStreamStatsEvent;

import java.util.HashMap;
import java.util.Map;
/**
 * RTC object management class
 *
 * Use the singleton form, call the RTC interface, and update the VideoCallDataManager data in the call
 * Internal record switch status
 *
 * Function:
 * 1. Switch and media status
 * 2. Get the current media status
 * 3. Receive various RTC callbacks, such as: user check-in and check-out, media status change, media status data callback, network status callback, volume callback
 * 4. Manage user video rendering view
 * 5. Join and leave the room
 * 6. Create and destroy engine
 */
public class VideoCallRTCManager {

    private static final String TAG = "VideoCallRTCManager";

    private static VideoCallRTCManager sInstance;
    // rts instance
    private VideoCallRTSClient mRTSClient;
    // rtc engine instance
    private RTCVideo mRTCVideo;
    // rtc room instance
    private RTCRoom mRTCRoom;
    // camera video stream rendering
    public Map<String, TextureView> mUserIdViewMap = new HashMap<>();
    // screen video stream rendering
    public Map<String, TextureView> mScreenUserIdViewMap = new HashMap<>();
    // camera status
    private boolean mIsCameraOn = true;
    // microphone status
    private boolean mIsMicOn = true;
    // whether to publish the audio stream
    private boolean mIsAudioMute = false;
    // whether to use speaker
    private boolean mIsSpeakerphone = true;
    // Whether to enable video mirroring
    private boolean mVideoMirror = true;
    // whether to use the front camera
    private boolean mIsFrontCamera = true;
    // the roomId of the current room needs to be passed in when rendering the video of the remote user
    private String mRoomId;
    // current audio route
    private AudioRoute mCurrentAudioRoute = null;
    // video resolution
    private String mResolution = Constants.DEFAULT_RESOLUTION;
    // audio quality
    private String mAudioQuality = Constants.DEFAULT_QUALITY;

    private final HashMap<String, Integer> mQualityCache = new HashMap<>();
    // Cache the remote user's microphone status to reduce the number of Events sent
    private final HashMap<String, Boolean> mRemoteSpeakingCache = new HashMap<>();
    // RTC engine callback object
    private final RTCVideoEventHandlerWithRTS mIRTCEngineEventHandler = new RTCVideoEventHandlerWithRTS() {

        private boolean mLocalSpeaking = false;
        /**
         * Local audio includes microphone audio and screen audio captured using the internal mechanism of the RTC SDK.
         * @param audioPropertiesInfos Local audio information, see LocalAudioPropertiesInfo for details.
         */
        @Override
        public void onLocalAudioPropertiesReport(LocalAudioPropertiesInfo[] audioPropertiesInfos) {
            super.onLocalAudioPropertiesReport(audioPropertiesInfos);
            Log.d(TAG, String.format("onLocalAudioPropertiesReport: %s", GsonUtils.gson().toJson(audioPropertiesInfos)));

            if (audioPropertiesInfos == null || audioPropertiesInfos.length == 0) {
                return;
            }

            LocalAudioPropertiesInfo info = audioPropertiesInfos[0];
            boolean isLocalSpeaking = info.audioPropertiesInfo.linearVolume > VOLUME_SPEAKING_THRESHOLD;

            if (isLocalSpeaking != mLocalSpeaking) {
                SolutionDemoEventManager.post(new AudioPropertiesReportEvent(
                        info.streamIndex,
                        SolutionDataManager.ins().getUserId(),
                        isLocalSpeaking));
                mLocalSpeaking = isLocalSpeaking;
            }
        }
        /**
         * The audio of the remote user includes the microphone audio and screen audio captured
         * by the internal mechanism/custom mechanism of the RTC SDK.
         *
         * @param audioPropertiesInfos Remote audio information, including audio stream properties,
         *                            room ID, user ID, see RemoteAudioPropertiesInfo for details.
         * @param totalRemoteVolume The total volume of all remote streams subscribed to.
         */
        @Override
        public void onRemoteAudioPropertiesReport(RemoteAudioPropertiesInfo[] audioPropertiesInfos,
                                                  int totalRemoteVolume) {
            super.onRemoteAudioPropertiesReport(audioPropertiesInfos, totalRemoteVolume);
            Log.d(TAG, String.format("onRemoteAudioPropertiesReport: %s", GsonUtils.gson().toJson(audioPropertiesInfos)));
            for (RemoteAudioPropertiesInfo audioPropertiesInfo : audioPropertiesInfos) {
                final boolean isSpeaking = audioPropertiesInfo.audioPropertiesInfo.linearVolume > VOLUME_SPEAKING_THRESHOLD;
                final String userId = audioPropertiesInfo.streamKey.getUserId();
                final boolean oldValue = mRemoteSpeakingCache.get(userId) == Boolean.TRUE;
                if (oldValue != isSpeaking) { // 变更时，再进行通知
                    SolutionDemoEventManager.post(new AudioPropertiesReportEvent(
                            audioPropertiesInfo.streamKey.getStreamIndex(),
                            userId,
                            isSpeaking));
                    if (isSpeaking) {
                        mRemoteSpeakingCache.put(userId, Boolean.TRUE);
                    } else {
                        mRemoteSpeakingCache.remove(userId);
                    }
                }
            }
        }
        /**
         * When a visible user in the room calls startVideoCapture to start internal video capture,
         * other users in the room will receive this callback.
         * @param roomId The room ID of the remote user who starts video capture
         * @param uid remote user ID to start video capture
         */
        @Override
        public void onUserStartVideoCapture(String roomId, String uid) {
            super.onUserStartVideoCapture(roomId, uid);
            VideoCallDataManager.ins().updateVideoStatus(uid, true);
        }
        /**
         * When a visible user in the room calls stopVideoCapture to stop internal video capture,
         * other users in the room will receive this callback.
         * @param roomId The room ID of the remote user whose video capture is disabled
         * @param uid The remote user ID for closing video capture
         */
        @Override
        public void onUserStopVideoCapture(String roomId, String uid) {
            super.onUserStopVideoCapture(roomId, uid);
            VideoCallDataManager.ins().updateVideoStatus(uid, false);
        }
        /**
         * This callback is received when the audio playback route changes.
         * @param route new audio playback route, see AudioRoute for details
         */
        @Override
        public void onAudioRouteChanged(AudioRoute route) {
            super.onAudioRouteChanged(route);
            Log.d(TAG, String.format("onAudioRouteChanged: %s", route));
            AudioRoute lastRouter = mCurrentAudioRoute;
            mCurrentAudioRoute = route;

            if (isHeadSet(lastRouter) && !isHeadSet(route)) {
                // corresponds to the case of unplugging the headset
                useSpeakerphone(true);
            }
        }
    };

    private final RTCRoomEventHandlerWithRTS mRTCRoomEventHandler = new RTCRoomEventHandlerWithRTS() {
        /**
         * Room status change callback, this callback will be received when joining a room,
         * leaving a room, or when a room-related warning or error occurs.
         * @param roomId room id
         * @param uid user id
         * @param state room status code
         * @param extraInfo extra information
         */
        @Override
        public void onRoomStateChanged(String roomId, String uid, int state, String extraInfo) {
            super.onRoomStateChanged(roomId, uid, state, extraInfo);
            Log.d(TAG, String.format("onRoomStateChanged: %s %s %d %s", roomId, uid, state, extraInfo));
            if (isFirstJoinRoomSuccess(state, extraInfo)) {
                VideoCallUserInfo userInfo = new VideoCallUserInfo();
                userInfo.userId = uid;
                userInfo.userName = SolutionDataManager.ins().getUserName();
                userInfo.isCameraOn = mIsCameraOn;
                userInfo.isMicOn = mIsMicOn && !mIsAudioMute;
                userInfo.isScreenShare = false;

                VideoCallDataManager.ins().addUser(userInfo);
            } else if (isReconnectSuccess(state, extraInfo)) {
                SolutionDemoEventManager.post(new SDKReconnectToRoomEvent(roomId));
            }
            mRoomId = roomId;
        }
        /**
         * Added a callback for the far-end camera/microphone to collect audio and video streams in the room.
         * @param uid The user ID of the remote stream publishing user.
         * @param type The type of remote media stream, see MediaStreamType.
         */
        @Override
        public void onUserPublishStream(String uid, MediaStreamType type) {
            super.onUserPublishStream(uid, type);
            Log.d(TAG, String.format("onUserPublishStream: %s %s", uid, type.toString()));
            if (type == MediaStreamType.RTC_MEDIA_STREAM_TYPE_BOTH) {
                VideoCallDataManager.ins().updateAudioStatus(uid, true);
                VideoCallDataManager.ins().updateVideoStatus(uid, true);
            } else if (type == MediaStreamType.RTC_MEDIA_STREAM_TYPE_AUDIO) {
                VideoCallDataManager.ins().updateAudioStatus(uid, true);
            } else if (type == MediaStreamType.RTC_MEDIA_STREAM_TYPE_VIDEO) {
                VideoCallDataManager.ins().updateVideoStatus(uid, true);
            }
        }
        /**
         * The callback for removing the media stream collected by the far-end camera/microphone in the room.
         * @param uid The user ID of the removed remote stream publishing user.
         * @param type The removed remote stream type, see MediaStreamType.
         * @param reason The reason for remote stream removal, see StreamRemoveReason.
         */
        @Override
        public void onUserUnpublishStream(String uid, MediaStreamType type, StreamRemoveReason reason) {
            super.onUserUnpublishStream(uid, type, reason);
            Log.d(TAG, String.format("onUserUnPublishStream: %s, %s, %s", uid, type.toString(), reason.toString()));
            if (type == MediaStreamType.RTC_MEDIA_STREAM_TYPE_BOTH) {
                VideoCallDataManager.ins().updateAudioStatus(uid, false);
                VideoCallDataManager.ins().updateVideoStatus(uid, false);
            } else if (type == MediaStreamType.RTC_MEDIA_STREAM_TYPE_AUDIO) {
                VideoCallDataManager.ins().updateAudioStatus(uid, false);
            } else if (type == MediaStreamType.RTC_MEDIA_STREAM_TYPE_VIDEO) {
                VideoCallDataManager.ins().updateVideoStatus(uid, false);
            }
        }
        /**
         * Added a callback for remote screen sharing audio and video streams in the room.
         * @param uid The user ID of the remote stream publishing user.
         * @param type The type of remote media stream, see MediaStreamType.
         */
        @Override
        public void onUserPublishScreen(String uid, MediaStreamType type) {
            super.onUserPublishScreen(uid, type);
            Log.d(TAG, String.format("onUserPublishScreen: %s %s", uid, type.toString()));

            VideoCallUserInfo vcUserInfo = new VideoCallUserInfo();
            vcUserInfo.userId = uid;
            vcUserInfo.userName = VideoCallDataManager.ins().getUserNameByUserId(uid);
            vcUserInfo.isScreenShare = true;
            vcUserInfo.isMicOn = true;
            vcUserInfo.isCameraOn = true;
            VideoCallDataManager.ins().setScreenShareUser(vcUserInfo);
        }
        /**
         * The callback for removing the audio and video stream of the remote screen sharing in the room.
         * @param uid The user ID of the removed remote stream publishing user.
         * @param type The removed remote stream type, see MediaStreamType.
         * @param reason The reason for remote stream removal, see StreamRemoveReason.
         */
        @Override
        public void onUserUnpublishScreen(String uid, MediaStreamType type, StreamRemoveReason reason) {
            super.onUserUnpublishScreen(uid, type, reason);
            Log.d(TAG, String.format("onUserUnPublishScreen: %s, %s, %s", uid, type.toString(), reason.toString()));

            VideoCallDataManager.ins().removeScreenShareUser(uid);
        }
        /**
         * After joining the room, report the user's network quality information every 2 seconds
         *
         * @param localQuality Local network quality, see NetworkQualityStats for details.
         * @param remoteQualities The network quality of subscribed users, see NetworkQualityStats for details.
         * @see NetworkQualityStats
         */
        @Override
        public void onNetworkQuality(NetworkQualityStats localQuality, NetworkQualityStats[] remoteQualities) {
            super.onNetworkQuality(localQuality, remoteQualities);
            for (NetworkQualityStats remoteQuality : remoteQualities) {
                mQualityCache.put(remoteQuality.uid, remoteQuality.rxQuality);
            }
        }
        /**
         * A callback that reflects the statistical information of the audio/video stream sent by the local device during the call and the network status.
         * @param stats audio and video streams and network status statistics. See LocalStreamStats.
         */
        @Override
        public void onLocalStreamStats(LocalStreamStats stats) {
            super.onLocalStreamStats(stats);
            Log.d(TAG, String.format("onLocalStreamStats: %s", stats));
            SolutionDemoEventManager.post(new LocalStreamStatsEvent(stats));
        }
        /**
         * A callback that reflects the statistical information of the subscribed remote audio/video stream received by the local device during the call and the network status. This callback is received every 2s.
         * @param stats audio and video streams and network status statistics. See RemoteStreamStats.
         */
        @Override
        public void onRemoteStreamStats(RemoteStreamStats stats) {
            super.onRemoteStreamStats(stats);
            Log.d(TAG, String.format("onRemoteStreamStats: %s", stats));

            final Integer value = mQualityCache.get(stats.uid);
            int quality = value == null ? NETWORK_QUALITY_UNKNOWN : value;

            stats.rxQuality = quality;
            if (stats.audioStats != null) {
                stats.audioStats.quality = quality;
            }
            SolutionDemoEventManager.post(new RemoteStreamStatsEvent(stats));
        }
        /**
         * Callback when visible users join the room, or invisible users in the room switch to visible.
         * @param userInfo user information
         * @param elapsed The elapsed time from when the host role user calls joinRoom to join the room to when other users in the room receive the event, in ms.
         */
        @Override
        public void onUserJoined(UserInfo userInfo, int elapsed) {
            super.onUserJoined(userInfo, elapsed);
            Log.d(TAG, String.format("onUserJoined: %s %d", userInfo.toString(), elapsed));

            VideoCallUserInfo videoCallUserInfo = rtcUserInfoToVCUserInfo(userInfo);
            VideoCallDataManager.ins().addUser(videoCallUserInfo);
        }
        /**
         * When the remote user leaves the room or becomes invisible, the local user will receive this event
         * @param uid Remote user ID to leave room, or switch to invisible.
         * @param reason The reason why the user left the room:
         * • 0: The remote user calls leaveRoom to actively leave the room.
         * • 1: The remote user is disconnected due to Token expiration or network reasons.
         * • 2: The remote user calls setUserVisibility to switch to the invisible state.
         * • 3: The server calls OpenAPI to kick the remote user out of the room.
         */
        @Override
        public void onUserLeave(String uid, int reason) {
            super.onUserLeave(uid, reason);
            Log.d(TAG, String.format("onUserLeave: %s %d", uid, reason));

            VideoCallDataManager.ins().removeUser(uid);
            mQualityCache.remove(uid);
            mRemoteSpeakingCache.remove(uid);
        }
    };

    public static @NonNull
    VideoCallRTCManager ins() {
        if (sInstance == null) {
            sInstance = new VideoCallRTCManager();
        }
        return sInstance;
    }

    public VideoCallRTSClient getRTSClient() {
        return mRTSClient;
    }

    public void initEngine(RTSInfo info) {
        Log.d(TAG, String.format("initEngine: %s", info));
        destroyEngine();
        // Create an engine instance
        mRTCVideo = RTCVideo.createRTCVideo(AppUtil.getApplicationContext(), info.appId,
                mIRTCEngineEventHandler, null, null);
        /*
          Set business identification parameters
          Different business scenarios can be distinguished by businessId.
          The businessId is customized by the customer, which is equivalent to a "label",
          which can share and refine the function of the logical division of the current AppId,
          but does not require authentication.
          */
        mRTCVideo.setBusinessId(info.bid);
        // Set the audio scene type
        mRTCVideo.setAudioScenario(AudioScenarioType.AUDIO_SCENARIO_COMMUNICATION);
        // Immediately starts internal video capture. The default is off.
        mRTCVideo.startVideoCapture();
        // Enable internal audio capture. The default is off.
        mRTCVideo.startAudioCapture();
        initDefaultVideoEffect();
        // Enable audio message prompts.
        mRTCVideo.enableAudioPropertiesReport(new AudioPropertiesConfig(VOLUME_SPEAKING_INTERVAL, true, false));
        mRTSClient = new VideoCallRTSClient(mRTCVideo, info);
        mIRTCEngineEventHandler.setBaseClient(mRTSClient);
        mRTCRoomEventHandler.setBaseClient(mRTSClient);

        setVideoResolution(mResolution);
        setAudioProfile(mAudioQuality);
        setMirrorType(true);
    }

    public void destroyEngine() {
        Log.d(TAG, "destroyEngine");
        if (mRTCRoom != null) {
            mRTCRoom.leaveRoom();
            mRTCRoom.destroy();
            mRTCRoom = null;
        }
        // Destroy the engine instance
        RTCVideo.destroyRTCVideo();
        mRTCVideo = null;
        if (mRTSClient != null) {
            mRTSClient.removeAllEventListener();
        }
        mRTSClient = null;
    }
    /**
     * Set the basic beauty effect
     */
    public void initDefaultVideoEffect() {
        mRTCVideo.enableEffectBeauty(true);
        // Set the whitening effect
        mRTCVideo.setBeautyIntensity(EFFECT_WHITE_MODE, 0.2F);
        // Set skin smoothing effect
        mRTCVideo.setBeautyIntensity(EFFECT_SMOOTH_MODE, 0.3F);
        // Set the sharpening effect
        mRTCVideo.setBeautyIntensity(EFFECT_SHARPEN_MODE, 0.4F);
    }
    /**
     * Join the RTC room
     *
     * @param token rtc join room token
     * @param roomId room id
     * @param userId user id
     * @param userName user name
     */
    public void joinRoom(String token, String roomId, String userId, String userName) {
        if (token == null) {
            token = "";
        }
        if (roomId == null) {
            roomId = "";
        }
        if (userId == null) {
            userId = "";
        }
        if (userName == null) {
            userName = "";
        }
        Log.d(TAG, String.format("joinRoom: %s %s %s %s", token, roomId, userId, userName));
        leaveRoom();
        if (mRTCVideo == null) {
            return;
        }
        // Join the RTC room, enable automatic publishing and subscription
        mRTCRoom = mRTCVideo.createRTCRoom(roomId);
        mRTCRoom.setRTCRoomEventHandler(mRTCRoomEventHandler);
        RTCRoomConfig roomConfig = new RTCRoomConfig(ChannelProfile.CHANNEL_PROFILE_COMMUNICATION,
                true, true, true);
        mRTCRoom.joinRoom(token, userIdNameToUserInfo(userId, userName), roomConfig);
        // According to the setting, manually set whether to publish the video
        startVideoCapture(mIsCameraOn);
        // According to the settings, manually set whether to publish audio
        startPublishAudio(!mIsAudioMute && mIsMicOn);
    }
    /**
     * Assemble userInfo of RTC
     *
     * @param userId user id
     * @param userName user nickname
     * @return userinfo
     */
    public UserInfo userIdNameToUserInfo(String userId, String userName) {
        VideoCallUserInfo userInfo = new VideoCallUserInfo();
        userInfo.userId = userId;
        userInfo.userName = userName;
        String extra = GsonUtils.gson().toJson(userInfo);
        return new UserInfo(userId, extra);
    }
    /**
     * Convert RTC UserInfo to business UserInfo
     *
     * @param userInfo RTC UserInfo object
     * @return business UserInfo
     */
    public VideoCallUserInfo rtcUserInfoToVCUserInfo(UserInfo userInfo) {
        String extraInfo = userInfo.getExtraInfo();
        if (TextUtils.isEmpty(extraInfo)) {
            return new VideoCallUserInfo(userInfo.getUid());
        }
        VideoCallUserInfo videoCallUserInfo = GsonUtils.gson().fromJson(extraInfo, VideoCallUserInfo.class);
        videoCallUserInfo.userId = userInfo.getUid();
        return videoCallUserInfo;
    }
    /**
     * leave the room
     */
    public void leaveRoom() {
        Log.d(TAG, "leaveRoom");
        mRoomId = null;
        if (mRTCRoom != null) {
            mRTCRoom.leaveRoom();
            mRTCRoom.destroy();
        }
        mRTCRoom = null;
    }
    /**
     * Whether there is camera permission
     *
     * @return whether there is permission
     */
    public boolean hasCameraPermission() {
        return hasPermission(Manifest.permission.CAMERA);
    }
    /**
     * Whether there is microphone permission
     *
     * @return whether there is permission
     */
    public boolean hasAudioPermission() {
        return hasPermission(Manifest.permission.RECORD_AUDIO);
    }
    /**
     * Whether there is a corresponding authority
     *
     * @param permission permission name
     * @return whether there is permission
     */
    public boolean hasPermission(String permission) {
        int res = AppUtil.getApplicationContext().checkPermission(
                permission, android.os.Process.myPid(), Process.myUid());
        return res == PackageManager.PERMISSION_GRANTED;
    }
    /**
     * Turn on the camera
     *
     * @param on Whether to open
     */
    public void startVideoCapture(boolean on) {
        Log.d(TAG, String.format("startVideoCapture: %b", on));
        if (!hasCameraPermission()) {
            mIsCameraOn = false;
            SolutionToast.show(R.string.camera_permission_disabled);
            mRTCVideo.stopVideoCapture();
            return;
        }
        mIsCameraOn = on;
        if (mRTCVideo != null) {
            if (on) {
                // Immediately starts internal video capture. The default is off.
                mRTCVideo.startVideoCapture();
            } else {
                // Close internal video capture immediately. The default is off.
                mRTCVideo.stopVideoCapture();
            }
        }
        if (mRTCRoom != null) {
            if (on) {
                // Publish the local media stream collected by the camera/microphone in the current room
                mRTCRoom.publishStream(MediaStreamType.RTC_MEDIA_STREAM_TYPE_VIDEO);
            } else {
                // Stop publishing the media stream collected by the local camera/microphone to the current room
                mRTCRoom.unpublishStream(MediaStreamType.RTC_MEDIA_STREAM_TYPE_VIDEO);
            }
        }

        SolutionDemoEventManager.post(new MediaStatusEvent(
                SolutionDataManager.ins().getUserId(),
                Constants.MEDIA_TYPE_VIDEO,
                mIsCameraOn ? Constants.MEDIA_STATUS_ON : Constants.MEDIA_STATUS_OFF));
    }
    /**
     * Start publishing the audio stream
     *
     * @param on Whether to publish
     */
    public void startPublishAudio(boolean on) {
        Log.d(TAG, String.format("startPublishAudio: %b", on));
        mIsAudioMute = !on;
        if (!hasAudioPermission()) {
            mIsMicOn = false;
            SolutionToast.show(R.string.microphone_permission_disabled);
            mRTCVideo.stopAudioCapture();
            return;
        }
        if (mRTCVideo != null) {
            if (on) {
                // Enable internal audio capture. The default is off.
                mRTCVideo.startAudioCapture();
            }
        }
        if (mRTCRoom != null) {
            if (on) {
                // Publish the local media stream collected by the camera/microphone in the current room
                mRTCRoom.publishStream(MediaStreamType.RTC_MEDIA_STREAM_TYPE_AUDIO);
            } else {
                // Stop publishing the media stream collected by the local camera/microphone to the current room
                mRTCRoom.unpublishStream(MediaStreamType.RTC_MEDIA_STREAM_TYPE_AUDIO);
            }
        }

        SolutionDemoEventManager.post(new MediaStatusEvent(
                SolutionDataManager.ins().getUserId(),
                Constants.MEDIA_TYPE_AUDIO,
                mIsMicOn && !mIsAudioMute ? Constants.MEDIA_STATUS_ON : Constants.MEDIA_STATUS_OFF));
    }
    /**
     * Whether the current audio route is a headset
     * @param audioRoute audio routing
     * @return true indicates any type of headset
     */
    public boolean isHeadSet(AudioRoute audioRoute) {
        return audioRoute == AudioRoute.AUDIO_ROUTE_HEADSET
                || audioRoute == AudioRoute.AUDIO_ROUTE_HEADSET_USB
                || audioRoute == AudioRoute.AUDIO_ROUTE_HEADSET_BLUETOOTH;
    }
    /**
     * Whether to use speakers
     *
     * @param use use speaker
     */
    public void useSpeakerphone(boolean use) {
        Log.d(TAG, String.format("useSpeakerphone: %b", use));
        if (isHeadSet(mCurrentAudioRoute)) {
            return;
        }
        mIsSpeakerphone = use;
        if (mRTCVideo != null) {
            // Set the current audio playback route. By default, the audio route set in setDefaultAudioRoute is used.
            mRTCVideo.setAudioRoute(use ? AudioRoute.AUDIO_ROUTE_SPEAKERPHONE
                    : AudioRoute.AUDIO_ROUTE_EARPIECE);
        }

        SolutionDemoEventManager.post(new AudioRouterEvent(use));
    }
    /**
     * Set video resolution
     * @param str Video resolution string. The string is an enumeration value
     */
    public void setVideoResolution(String str) {
        if (!Constants.RESOLUTION_MAP.containsKey(str)) {
            str = Constants.DEFAULT_RESOLUTION;
        }
        mResolution = str;
        Pair<Integer, Integer> resolution = Constants.RESOLUTION_MAP.get(str);
        if (resolution != null) {
            setVideoResolution(resolution.first, resolution.second);
        }
    }
    /**
     * Set audio quality
     * @param audioProfile audio profile
     */
    public void setAudioProfile(String audioProfile) {
        if (!Constants.QUALITY_MAP.containsKey(audioProfile)) {
            audioProfile = Constants.DEFAULT_QUALITY;
        }
        mAudioQuality = audioProfile;
        AudioProfileType audioProfileType = Constants.QUALITY_MAP.get(audioProfile);
        if (audioProfileType != null) {
            setAudioProfile(audioProfileType);
        }
    }
    /**
     * Set video resolution
     * @param width width
     * @param height height
     */
    public void setVideoResolution(int width, int height) {
        Log.d(TAG, String.format("setResolution: %d  %d", width, height));
        if (mRTCVideo != null) {
            VideoEncoderConfig videoEncoderConfig = new VideoEncoderConfig();
            videoEncoderConfig.width = width;
            videoEncoderConfig.height = height;
            // stream bitrate adaptive
            videoEncoderConfig.maxBitrate = -1;
            videoEncoderConfig.frameRate = 15;
            // The video publisher sets the maximum resolution video stream parameters expected to be published,
            // including resolution, frame rate, bit rate, scaling mode, fallback strategy when the network is not good, etc.
            mRTCVideo.setVideoEncoderConfig(videoEncoderConfig);
        }
    }
    /**
     * Set audio quality
     * @param audioProfileType audio quality
     */
    public void setAudioProfile(AudioProfileType audioProfileType) {
        Log.d(TAG, String.format("setAudioProfile: %s", audioProfileType));
        if (mRTCVideo != null) {
            // Set the sound quality level. When the audio parameters in the selected
            // ChannelProfile cannot meet the needs of your scene, call this interface to switch the audio quality gear.
            mRTCVideo.setAudioProfile(audioProfileType);
        }
    }
    /**
     * Set mirror effect
     * @param mirror Whether to enable mirroring
     */
    public void setMirrorType(boolean mirror) {
        Log.d(TAG, String.format("setMirrorType: %b", mirror));
        mVideoMirror = mirror;
        if (mRTCVideo != null) {
            // Enable mirroring for the captured video stream,
            // mirroring is only set if the front end and mirroring are enabled, otherwise no mirroring
            mRTCVideo.setLocalVideoMirrorType(mirror && mIsFrontCamera
                    ? MirrorType.MIRROR_TYPE_RENDER_AND_ENCODER
                    : MirrorType.MIRROR_TYPE_NONE);
        }
    }
    /**
     * flip camera
     * @param isFrontCamera is the front camera
     */
    public void switchCamera(boolean isFrontCamera) {
        Log.d(TAG, String.format("switchCamera: %b", isFrontCamera));
        mIsFrontCamera = isFrontCamera;
        if (mRTCVideo != null) {
            // Switch the front/back camera used when capturing the video internally
            mRTCVideo.switchCamera(isFrontCamera
                    ? CameraId.CAMERA_ID_FRONT
                    : CameraId.CAMERA_ID_BACK);
        }
        // After changing the camera, you need to reset the mirror effect
        setMirrorType(mVideoMirror);
    }
    /**
     * Set the local user video rendering view
     * @param isScreen is screen stream
     * @param textureView rendering view
     */
    public void setLocalVideoCanvas(boolean isScreen, TextureView textureView) {
        Log.d(TAG, String.format("setLocalVideoCanvas: %b", isScreen));
        if (mRTCVideo != null) {
            StreamIndex index = isScreen ? StreamIndex.STREAM_INDEX_SCREEN : StreamIndex.STREAM_INDEX_MAIN;
            VideoCanvas canvas = new VideoCanvas();
            canvas.renderView = textureView;
            canvas.renderMode = RENDER_MODE_HIDDEN;
            // Set the view used for local video rendering and set the rendering mode.
            mRTCVideo.setLocalVideoCanvas(index, canvas);
        }
    }
    /**
     * Set remote user video rendering views
     * @param uid user id
     * @param isScreen is screen stream
     * @param textureView rendering view
     */
    public void setRemoteVideCanvas(String uid, boolean isScreen, TextureView textureView) {
        Log.d(TAG, String.format("setRemoteVideCanvas: %s  %b", uid, isScreen));
        if (mRTCVideo != null) {
            StreamIndex index = isScreen ? StreamIndex.STREAM_INDEX_SCREEN : StreamIndex.STREAM_INDEX_MAIN;
            VideoCanvas canvas = new VideoCanvas(textureView, isScreen ? RENDER_MODE_FIT : RENDER_MODE_HIDDEN);
            RemoteStreamKey remoteStreamKey = new RemoteStreamKey(mRoomId, uid, index);
            canvas.renderView = textureView;
            // When rendering the video stream from the specified remote user, set the view and rendering mode used.
            mRTCVideo.setRemoteVideoCanvas(remoteStreamKey, canvas);
        }
    }

    public boolean isCameraOn() {
        return mIsCameraOn;
    }

    public boolean isMicOn() {
        return mIsMicOn && !mIsAudioMute;
    }

    public boolean isSpeakerphone() {
        return mIsSpeakerphone;
    }

    public boolean isVideoMirror() {
        return mVideoMirror;
    }

    public boolean isFrontCamera() {
        return mIsFrontCamera;
    }

    public String getResolution() {
        return mResolution;
    }

    public String getAudioQuality() {
        return mAudioQuality;
    }
    /**
     * Get the views for user camera stream rendering
     *
     * @param uid user id
     * @return render view
     */
    public TextureView getRenderView(String uid) {
        if (TextUtils.isEmpty(uid)) {
            return null;
        }
        TextureView textureView = mUserIdViewMap.get(uid);
        if (textureView != null) {
            return textureView;
        }
        textureView = new TextureView(AppUtil.getApplicationContext());
        mUserIdViewMap.put(uid, textureView);
        return textureView;
    }
    /**
     * Get the views for rendering the video stream on the user's screen
     *
     * @param uid user id
     * @return render view
     */
    public TextureView getScreenRenderView(String uid) {
        if (TextUtils.isEmpty(uid)) {
            return null;
        }
        TextureView textureView = mScreenUserIdViewMap.get(uid);
        if (textureView != null) {
            return textureView;
        }
        textureView = new TextureView(AppUtil.getApplicationContext());
        mScreenUserIdViewMap.put(uid, textureView);
        return textureView;
    }
    /**
     * Clear all user-rendered views
     */
    public void clearUserView() {
        mUserIdViewMap.clear();
        mScreenUserIdViewMap.clear();
    }
}
