// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.feature.main;

import android.os.Bundle;
import android.text.TextUtils;

import com.volcengine.vertcdemo.common.SolutionBaseActivity;
import com.volcengine.vertcdemo.core.eventbus.SolutionDemoEventManager;
import com.volcengine.vertcdemo.core.eventbus.AppTokenExpiredEvent;
import com.volcengine.vertcdemo.videocall.bean.VideoCallUserInfo;
import com.volcengine.vertcdemo.videocall.core.VideoCallDataManager;
import com.volcengine.vertcdemo.videocall.databinding.ActivityFullScreenBinding;
import com.volcengine.vertcdemo.videocall.event.FullScreenFinishEvent;
import com.volcengine.vertcdemo.videocall.event.RoomFinishEvent;
import com.volcengine.vertcdemo.videocall.event.ScreenShareEvent;

import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
/**
 * Screen sharing full screen horizontal screen display page
 *
 * This page is independent to simplify the logic of UI changes
 */
public class FullScreenActivity extends SolutionBaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ActivityFullScreenBinding viewBinding = ActivityFullScreenBinding.inflate(getLayoutInflater());
        setContentView(viewBinding.getRoot());

        VideoCallUserInfo userInfo = VideoCallDataManager.ins().getScreenShareUser();
        if (userInfo == null || TextUtils.isEmpty(userInfo.userId)) {
            finish();
        }
        viewBinding.fullScreenLayout.bind(userInfo);
        viewBinding.fullScreenLayout.setZoomAction((ui) -> finish());

        SolutionDemoEventManager.register(this);
    }

    @Override
    protected boolean onMicrophonePermissionClose() {
        finish();
        return true;
    }

    @Override
    protected boolean onCameraPermissionClose() {
        finish();
        return true;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // When this page is closed, it needs to notify the horizontal screen that the screen sharing has ended
        SolutionDemoEventManager.post(new FullScreenFinishEvent());
        SolutionDemoEventManager.unregister(this);
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onScreenShareEvent(ScreenShareEvent event) {
        // This page needs to be closed when screen sharing ends
        if (!event.isStart) {
            finish();
        }
    }


    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onRoomFinishEvent(RoomFinishEvent event) {
        // This page needs to be closed when the room is closed
        finish();
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onTokenExpiredEvent(AppTokenExpiredEvent event) {
        finish();
    }
}