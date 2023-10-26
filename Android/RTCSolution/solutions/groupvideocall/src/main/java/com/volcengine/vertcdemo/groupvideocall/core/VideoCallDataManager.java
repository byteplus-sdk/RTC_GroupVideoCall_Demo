// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.core;

import android.text.TextUtils;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.volcengine.vertcdemo.core.eventbus.SolutionDemoEventManager;
import com.volcengine.vertcdemo.groupvideocall.bean.VideoCallUserInfo;
import com.volcengine.vertcdemo.groupvideocall.event.MediaStatusEvent;
import com.volcengine.vertcdemo.groupvideocall.event.RefreshUserLayoutEvent;
import com.volcengine.vertcdemo.groupvideocall.event.RoomUserEvent;
import com.volcengine.vertcdemo.groupvideocall.event.ScreenShareEvent;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
/**
 * In-room user data management
 *
 * Use singleton form, modify data through RTC callback, and then notify UI update through eventbus {@link #ins()}
 *
 * Function:
 * 1. Add user {@link #addUser(VideoCallUserInfo)}
 * 2. Remove user {@link #removeUser(String)}
 * 3. Remove all users {@link #removeAllUser()}
 * 4. Get user list {@link #getUserList()}
 * 5. Update the user's audio status {@link #updateAudioStatus(String, boolean)}
 * 6. Update the user's video status {@link #updateVideoStatus(String, boolean)}
 * 7. Set screen share user information {@link #setScreenShareUser(VideoCallUserInfo)}
 * 8. Remove screen share user information {@link #removeScreenShareUser(String)}
 * 9. Get screen share user information {@link #getScreenShareUser()}
 * 10. Get user name by user id {@link #getUserNameByUserId(String)}
 */
public class VideoCallDataManager {

    private final List<VideoCallUserInfo> mUserList = new ArrayList<>();
    private VideoCallUserInfo mScreenShareUser;

    private static VideoCallDataManager sInstance;

    private VideoCallDataManager() {

    }

    public static @NonNull
    VideoCallDataManager ins() {
        if (sInstance == null) {
            sInstance = new VideoCallDataManager();
        }
        return sInstance;
    }
    /**
     * Add user information
     *
     * @param userInfo user information
     */
    public void addUser(@Nullable VideoCallUserInfo userInfo) {
        if (userInfo == null || TextUtils.isEmpty(userInfo.userId)) {
            return;
        }
        if (mUserList.contains(userInfo)) {
            SolutionDemoEventManager.post(new RefreshUserLayoutEvent(getUserList()));
            return;
        }
        mUserList.add(userInfo);

        SolutionDemoEventManager.post(new RoomUserEvent(userInfo, true));
        SolutionDemoEventManager.post(new RefreshUserLayoutEvent(getUserList()));
    }
    /**
     * Remove user information
     *
     * @param userId user id
     */
    public void removeUser(@Nullable String userId) {
        if (TextUtils.isEmpty(userId)) {
            return;
        }
        Iterator<VideoCallUserInfo> iterator = mUserList.listIterator();
        while (iterator.hasNext()) {
            VideoCallUserInfo ui = iterator.next();
            if (TextUtils.equals(ui.userId, userId)) {
                iterator.remove();

                SolutionDemoEventManager.post(new RoomUserEvent(ui, false));
                SolutionDemoEventManager.post(new RefreshUserLayoutEvent(getUserList()));
            }
        }
    }
    /**
     * Remove all user information
     */
    public void removeAllUser() {
        mUserList.clear();
        mScreenShareUser = null;
    }
    /**
     * Update the user's audio status
     *
     * @param userId user id
     * @param on audio state
     */
    public void updateAudioStatus(@Nullable String userId, boolean on) {
        if (TextUtils.isEmpty(userId)) {
            return;
        }
        for (VideoCallUserInfo ui : mUserList) {
            if (TextUtils.equals(ui.userId, userId)) {
                ui.isMicOn = on;
            }
        }

        SolutionDemoEventManager.post(new MediaStatusEvent(
                userId,
                Constants.MEDIA_TYPE_AUDIO,
                on ? Constants.MEDIA_STATUS_ON : Constants.MEDIA_STATUS_OFF));
    }
    /**
     * Update user's video status
     *
     * @param userId user id
     * @param on video status
     */
    public void updateVideoStatus(@Nullable String userId, boolean on) {
        for (VideoCallUserInfo ui : mUserList) {
            if (TextUtils.equals(ui.userId, userId)) {
                ui.isCameraOn = on;
            }
        }

        SolutionDemoEventManager.post(new MediaStatusEvent(
                userId,
                Constants.MEDIA_TYPE_VIDEO,
                on ? Constants.MEDIA_STATUS_ON : Constants.MEDIA_STATUS_OFF));
    }
    /**
     * Get the list of users in the current room. If there is a screen sharing user,
     * the user information will be the first in the user list
     *
     * @return user list, always not null
     */
    public @NonNull
    List<VideoCallUserInfo> getUserList() {
        List<VideoCallUserInfo> userInfoList = new ArrayList<>(mUserList);
        if (mScreenShareUser != null) {
            userInfoList.add(0, mScreenShareUser);
        }
        return userInfoList;
    }
    /**
     * In the current user list, find the user nickname through the user id
     *
     * @param userId user id
     * @return User nickname, if not found return an empty string
     */
    public @NonNull
    String getUserNameByUserId(@Nullable String userId) {
        if (TextUtils.isEmpty(userId)) {
            return "";
        }
        for (VideoCallUserInfo userInfo : mUserList) {
            if (TextUtils.equals(userId, userInfo.userId)) {
                return userInfo.userName;
            }
        }
        return "";
    }
    /**
     * In the current user list, find user information by user id
     *
     * @param userId user id
     * @return user information, return empty object if not found
     */
    public @Nullable
    VideoCallUserInfo getUserByUserId(@Nullable String userId) {
        if (TextUtils.isEmpty(userId)) {
            return null;
        }
        for (VideoCallUserInfo userInfo : mUserList) {
            if (TextUtils.equals(userId, userInfo.userId)) {
                return userInfo;
            }
        }
        return null;
    }
    /**
     * Set screen sharing user information
     *
     * @param userInfo user information
     */
    public void setScreenShareUser(@Nullable VideoCallUserInfo userInfo) {
        VideoCallUserInfo videoCallUserInfo = getUserByUserId(userInfo == null ? null : userInfo.userId);
        // Synchronize the previous user's camera and microphone status
        if (videoCallUserInfo != null && userInfo != null) {
            userInfo.isMicOn = videoCallUserInfo.isMicOn;
            userInfo.isCameraOn = videoCallUserInfo.isCameraOn;
        }

        mScreenShareUser = userInfo;
        if (userInfo != null) {
            SolutionDemoEventManager.post(new RefreshUserLayoutEvent(getUserList()));
            SolutionDemoEventManager.post(new ScreenShareEvent(true));
        }
    }
    /**
     * Remove screen sharing user information
     *
     * @param uid user id
     */
    public void removeScreenShareUser(@Nullable String uid) {
        if (TextUtils.isEmpty(uid) || mScreenShareUser == null) {
            return;
        }
        if (TextUtils.equals(uid, mScreenShareUser.userId)) {
            mScreenShareUser = null;
            SolutionDemoEventManager.post(new RefreshUserLayoutEvent(getUserList()));
            SolutionDemoEventManager.post(new ScreenShareEvent(false));
        }
    }
    /**
     * Get screen sharing user information
     *
     * @return user information
     */
    public @Nullable
    VideoCallUserInfo getScreenShareUser() {
        return mScreenShareUser;
    }
}
