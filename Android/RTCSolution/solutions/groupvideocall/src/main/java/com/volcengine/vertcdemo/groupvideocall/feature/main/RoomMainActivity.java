// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.feature.main;

import static android.view.View.GONE;
import static com.volcengine.vertcdemo.groupvideocall.core.Constants.MEDIA_STATUS_ON;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.volcengine.vertcdemo.common.SolutionBaseActivity;
import com.volcengine.vertcdemo.common.SolutionToast;
import com.volcengine.vertcdemo.common.IAction;
import com.volcengine.vertcdemo.common.SolutionCommonDialog;
import com.volcengine.vertcdemo.core.SolutionDataManager;
import com.volcengine.vertcdemo.core.eventbus.SolutionDemoEventManager;
import com.volcengine.vertcdemo.core.eventbus.AppTokenExpiredEvent;
import com.volcengine.vertcdemo.core.net.ErrorTool;
import com.volcengine.vertcdemo.core.net.IRequestCallback;
import com.volcengine.vertcdemo.groupvideocall.bean.VideoCallUserInfo;
import com.volcengine.vertcdemo.groupvideocall.core.VideoCallRTCManager;
import com.volcengine.vertcdemo.groupvideocall.databinding.ActivityRoomMainBinding;
import com.volcengine.vertcdemo.groupvideocall.event.MediaStatusEvent;
import com.volcengine.vertcdemo.groupvideocall.R;
import com.volcengine.vertcdemo.groupvideocall.bean.LeaveRoomResponse;
import com.volcengine.vertcdemo.groupvideocall.bean.ReconnectResponse;
import com.volcengine.vertcdemo.groupvideocall.core.Constants;
import com.volcengine.vertcdemo.groupvideocall.core.VideoCallDataManager;
import com.volcengine.vertcdemo.groupvideocall.event.AudioPropertiesReportEvent;
import com.volcengine.vertcdemo.groupvideocall.event.FullScreenFinishEvent;
import com.volcengine.vertcdemo.groupvideocall.event.RefreshPreviewEvent;
import com.volcengine.vertcdemo.groupvideocall.event.RefreshUserLayoutEvent;
import com.volcengine.vertcdemo.groupvideocall.event.RoomFinishEvent;
import com.volcengine.vertcdemo.core.eventbus.SDKReconnectToRoomEvent;
import com.volcengine.vertcdemo.groupvideocall.event.ScreenShareEvent;
import com.volcengine.vertcdemo.groupvideocall.view.TitleView;

import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
/**
 * Room home page
 *
 * Function:
 * 1. Display room information
 * 2. Display video information of users in the room
 * 3. Media control function
 */
public class RoomMainActivity extends SolutionBaseActivity {

    private static final String TAG = "RoomMainActivity";
    // session duration
    private long mLastTs;
    // room id
    private String mRoomId;
    // Join the rtc room token
    private String mToken;

    private ActivityRoomMainBinding mViewBinding;
    /**
     * Request the business server to reconnect to the callback of the current room interface,
     * which is used by the server to control the number of people in the room
     */
    private final IRequestCallback<ReconnectResponse> mReconnectCallback
            = new IRequestCallback<ReconnectResponse>() {
        @Override
        public void onSuccess(ReconnectResponse data) {

        }

        @Override
        public void onError(int errorCode, String message) {
            SolutionToast.show(ErrorTool.getErrorMessageByErrorCode(errorCode, message));
            finish();
        }
    };
    /**
     * Request a callback from the business server to leave the current room interface,
     * which is used by the server to control the number of people in the room
     */
    private final IRequestCallback<LeaveRoomResponse> mLeaveCallback
            = new IRequestCallback<LeaveRoomResponse>() {
        @Override
        public void onSuccess(LeaveRoomResponse data) {

        }

        @Override
        public void onError(int errorCode, String message) {

        }
    };
    /**
     * The click event of the functional area in the upper part of the room
     */
    private final TitleView.ITitleCallback mITitleCallback = new TitleView.ITitleCallback() {
        // zoom out event, temporarily useless
        @Override
        public void onZoomClick() {

        }
        // Click the hangup event to display the confirm exit room dialog box
        @Override
        public void onHangUpClick() {
            attemptLeaveRoom();
        }
    };
    // User view click event in user list
    private final IAction<VideoCallUserInfo> mUserViewClick = userInfo -> {
        if (userInfo.isScreenShare) {
            mViewBinding.portraitScreenLayout.setVisibility(View.VISIBLE);
            mViewBinding.portraitScreenLayout.bind(userInfo);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mViewBinding = ActivityRoomMainBinding.inflate(getLayoutInflater());
        setContentView(mViewBinding.getRoot());

        Intent intent = getIntent();
        mLastTs = intent.getLongExtra(Constants.EXTRA_LAST_TS, 0);
        mRoomId = intent.getStringExtra(Constants.EXTRA_ROOM_ID);
        mToken = intent.getStringExtra(Constants.EXTRA_TOKEN);
        
        mViewBinding.userListPagerLayout.setOnUserViewClick(mUserViewClick);
        
        mViewBinding.portraitScreenLayout.setExpandAction((userInfo) ->
                startActivity(new Intent(this, FullScreenActivity.class)));
        mViewBinding.portraitScreenLayout.setOnClickListener((v) -> {
            mViewBinding.portraitScreenLayout.bind(null);
            mViewBinding.portraitScreenLayout.setVisibility(GONE);

            VideoCallUserInfo userInfo = VideoCallDataManager.ins().getScreenShareUser();
            if (userInfo != null) {
                mViewBinding.userListPagerLayout.updateUserVideoStatus(userInfo.userId, true, true);
            }
        });
        mViewBinding.portraitScreenLayout.setVisibility(GONE);

        mViewBinding.titleView.setITitleCallback(mITitleCallback);
        mViewBinding.titleView.setRoomId(mRoomId);
        mViewBinding.titleView.startCountDown(mLastTs);

        SolutionDemoEventManager.register(this);
        // join the RTC room
        VideoCallRTCManager.ins().joinRoom(mToken, mRoomId, SolutionDataManager.ins().getUserId(),
                SolutionDataManager.ins().getUserName());
    }
    /**
     * Pop-up confirm to leave the room dialog
     */
    private void attemptLeaveRoom() {
        SolutionCommonDialog dialog = new SolutionCommonDialog(this);
        dialog.setMessage(getString(R.string.exit_room));
        dialog.setNegativeListener((v) -> dialog.dismiss());
        dialog.setPositiveListener((v) -> {
            dialog.dismiss();
            finish();
        });
        dialog.show();
    }

    @Override
    protected boolean onMicrophonePermissionClose() {
        Log.d(TAG, "onMicrophonePermissionClose");
        finish();
        return true;
    }

    @Override
    protected boolean onCameraPermissionClose() {
        Log.d(TAG, "onCameraPermissionClose");
        finish();
        return true;
    }

    @Override
    public void onBackPressed() {
        attemptLeaveRoom();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        SolutionDemoEventManager.unregister(this);

        VideoCallDataManager.ins().removeAllUser();
        VideoCallRTCManager.ins().getRTSClient().requestLeaveRoom(mRoomId, mLeaveCallback);
        VideoCallRTCManager.ins().leaveRoom();
        SolutionDemoEventManager.post(new RefreshPreviewEvent());
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onRefreshUserLayoutEvent(RefreshUserLayoutEvent event) {
        // Update user list view
        mViewBinding.userListPagerLayout.setUserList(event.userInfoList);
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onAudioPropertiesReportEvent(AudioPropertiesReportEvent event) {
        // Update user speaking status UI
        mViewBinding.userListPagerLayout.updateUserSpeakingStatus(event.uid, event.isSpeaking);
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onMediaStatusEvent(MediaStatusEvent event) {
        // Update user media status UI
        if (event.mediaType == Constants.MEDIA_TYPE_VIDEO) {
            mViewBinding.userListPagerLayout.updateUserVideoStatus(event.uid, false, event.status == MEDIA_STATUS_ON);
        } else if (event.mediaType == Constants.MEDIA_TYPE_AUDIO) {
            mViewBinding.userListPagerLayout.updateUserAudioStatus(event.uid, event.status == MEDIA_STATUS_ON);
        }
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onSDKReconnectToRoomEvent(SDKReconnectToRoomEvent event) {
        VideoCallRTCManager.ins().getRTSClient().requestReconnect(mRoomId, mReconnectCallback);
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onRoomFinishEvent(RoomFinishEvent event) {
        // The length of the room is limited. After the maximum time is reached, the server sends a notification and the client checks out
        SolutionToast.show(R.string.minutes_meeting);
        finish();
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onScreenShareEvent(ScreenShareEvent event) {
        if (!event.isStart) {
            // At the end of screen sharing, try to bind new user data
            VideoCallUserInfo userInfo = VideoCallDataManager.ins().getScreenShareUser();
            mViewBinding.portraitScreenLayout.bind(userInfo);
            mViewBinding.portraitScreenLayout.setVisibility(userInfo == null ? GONE : View.VISIBLE);
        }
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onFullScreenFinishEvent(FullScreenFinishEvent event) {
        VideoCallUserInfo userInfo = VideoCallDataManager.ins().getScreenShareUser();
        mViewBinding.portraitScreenLayout.bind(userInfo);
        mViewBinding.portraitScreenLayout.setVisibility(userInfo == null ? GONE : View.VISIBLE);
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onTokenExpiredEvent(AppTokenExpiredEvent event) {
        finish();
    }
}