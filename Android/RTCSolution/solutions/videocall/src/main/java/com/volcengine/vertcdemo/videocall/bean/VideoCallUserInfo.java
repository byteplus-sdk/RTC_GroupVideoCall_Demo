// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.bean;

import android.text.TextUtils;

import androidx.annotation.NonNull;

import com.google.gson.annotations.SerializedName;

import java.util.Objects;
/**
 * User data model
 */
public class VideoCallUserInfo {
    // user name
    @SerializedName("user_name")
    public String userName;
    // user id
    @SerializedName("user_id")
    public String userId;
    // Whether there is audio data
    public transient boolean isMicOn;
    // Whether there is video data
    public transient boolean isCameraOn;
    // Whether it is screen sharing
    public transient boolean isScreenShare;

    public VideoCallUserInfo() {

    }

    public VideoCallUserInfo(String uid) {
        this.userId = uid;
        this.userName = uid;
    }
    // Get the first character of the username
    public @NonNull
    String getNamePrefix() {
        if (userName == null) {
            return "";
        }
        String str = userName.trim();
        if (TextUtils.isEmpty(str)) {
            return "";
        }
        return str.substring(0, 1);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }
        VideoCallUserInfo userInfo = (VideoCallUserInfo) o;
        return Objects.equals(userId, userInfo.userId) && (isScreenShare == userInfo.isScreenShare);
    }

    @Override
    public int hashCode() {
        return Objects.hash(userId, isScreenShare);
    }

    @Override
    public String toString() {
        return "VideoCallUserInfo{" +
                "userName='" + userName + '\'' +
                ", userId='" + userId + '\'' +
                ", isMicOn=" + isMicOn +
                ", isCameraOn=" + isCameraOn +
                ", isScreenShare=" + isScreenShare +
                '}';
    }
}
