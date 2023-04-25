// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.core;

import android.text.TextUtils;

import androidx.annotation.NonNull;

import com.google.gson.JsonObject;
import com.ss.bytertc.engine.RTCVideo;
import com.volcengine.vertcdemo.common.AppExecutors;
import com.volcengine.vertcdemo.common.AbsBroadcast;
import com.volcengine.vertcdemo.common.AppExecutors;
import com.volcengine.vertcdemo.core.SolutionDataManager;
import com.volcengine.vertcdemo.core.eventbus.SolutionDemoEventManager;
import com.volcengine.vertcdemo.core.net.IRequestCallback;
import com.volcengine.vertcdemo.core.net.rts.RTSBaseClient;
import com.volcengine.vertcdemo.core.net.rts.RTSBizInform;
import com.volcengine.vertcdemo.core.net.rts.RTSBizResponse;
import com.volcengine.vertcdemo.core.net.rts.RTSInfo;
import com.volcengine.vertcdemo.videocall.bean.JoinRoomResponse;
import com.volcengine.vertcdemo.videocall.bean.LeaveRoomResponse;
import com.volcengine.vertcdemo.videocall.bean.ReconnectResponse;
import com.volcengine.vertcdemo.videocall.event.RoomFinishEvent;

import java.util.UUID;
/**
 * The class that the client actively requests and receives broadcasts from the business server
 *
 * Use RTS function to send and receive messages
 *
 * Function:
 * 1. Request to the business server to join the room
 * 2. Request to the business server to leave the room
 * 3. Request to the business server to reconnect to the current room
 * 4. Receive room to maximum time broadcast
 */
public class VideoCallRTSClient extends RTSBaseClient {
    // The active request and broadcast command agreed by the front and back ends
    private static final String CMD_VIDEO_CALL_JOIN = "videocallJoinRoom";
    private static final String CMD_VIDEO_CALL_LEAVE = "videocallLeaveRoom";
    private static final String CMD_VIDEO_CALL_RECONNECT = "videocallReconnect";

    private static final String ON_ROOM_FINISH = "videocallOnCloseRoom";

    public VideoCallRTSClient(@NonNull RTCVideo rtcVideo, @NonNull RTSInfo rtmInfo) {
        super(rtcVideo, rtmInfo);
        initEventListener();
    }
    // generate default request parameters
    private JsonObject getCommonParams(String cmd) {
        JsonObject params = new JsonObject();
        params.addProperty("app_id", mRTSInfo.appId);
        params.addProperty("room_id", "");
        params.addProperty("user_id", SolutionDataManager.ins().getUserId());
        params.addProperty("event_name", cmd);
        params.addProperty("request_id", UUID.randomUUID().toString());
        params.addProperty("device_id", SolutionDataManager.ins().getDeviceId());
        params.addProperty("login_token", SolutionDataManager.ins().getToken());
        return params;
    }

    private void initEventListener() {
        // Room to max time close event
        putEventListener(new AbsBroadcast<>(ON_ROOM_FINISH, RoomFinishEvent.class,
                SolutionDemoEventManager::post));
    }

    private void putEventListener(AbsBroadcast<? extends RTSBizInform> absBroadcast) {
        mEventListeners.put(absBroadcast.getEvent(), absBroadcast);
    }

    public void removeAllEventListener() {
        mEventListeners.remove(ON_ROOM_FINISH);
    }

    private <T extends RTSBizResponse> void sendServerMessageOnNetwork(String roomId,
                                                                       JsonObject content,
                                                                       Class<T> resultClass,
                                                                       IRequestCallback<T> callback) {
        String cmd = content.get("event_name").getAsString();
        if (TextUtils.isEmpty(cmd)) {
            return;
        }
        AppExecutors.networkIO().execute(() ->
                sendServerMessage(cmd, roomId, content, resultClass, callback));
    }
    /**
     * request to join a room
     * @param roomId room id
     * @param callback Callback after the request is sent
     */
    public void requestJoinRoom(String roomId, IRequestCallback<JoinRoomResponse> callback) {
        JsonObject content = getCommonParams(CMD_VIDEO_CALL_JOIN);
        content.addProperty("room_id", roomId);
        sendServerMessageOnNetwork(roomId, content, JoinRoomResponse.class, callback);
    }
    /**
     * Request to reconnect to the room
     * @param roomId room id
     * @param callback Callback after the request is sent
     */
    public void requestReconnect(String roomId, IRequestCallback<ReconnectResponse> callback) {
        JsonObject content = getCommonParams(CMD_VIDEO_CALL_RECONNECT);
        content.addProperty("room_id", roomId);
        sendServerMessageOnNetwork(roomId, content, ReconnectResponse.class, callback);
    }
    /**
     * Request to leave the room
     * @param roomId room id
     * @param callback Callback after the request is sent
     */
    public void requestLeaveRoom(String roomId, IRequestCallback<LeaveRoomResponse> callback) {
        JsonObject content = getCommonParams(CMD_VIDEO_CALL_LEAVE);
        content.addProperty("room_id", roomId);
        sendServerMessageOnNetwork(roomId, content, LeaveRoomResponse.class, callback);
    }
}
