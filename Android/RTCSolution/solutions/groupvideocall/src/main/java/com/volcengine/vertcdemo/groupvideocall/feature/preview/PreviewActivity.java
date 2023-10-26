// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.feature.preview;

import static com.volcengine.vertcdemo.core.net.rts.RTSInfo.KEY_RTS;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputFilter;
import android.text.TextUtils;
import android.util.Log;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import androidx.annotation.Keep;

import com.ss.bytertc.engine.RTCVideo;
import com.vertcdemo.joinrtsparams.bean.JoinRTSRequest;
import com.vertcdemo.joinrtsparams.common.JoinRTSManager;
import com.volcengine.vertcdemo.common.IAction;
import com.volcengine.vertcdemo.common.LengthFilterWithCallback;
import com.volcengine.vertcdemo.common.SolutionBaseActivity;
import com.volcengine.vertcdemo.common.SolutionToast;
import com.volcengine.vertcdemo.common.TextWatcherAdapter;
import com.volcengine.vertcdemo.core.SolutionDataManager;
import com.volcengine.vertcdemo.core.eventbus.SolutionDemoEventManager;
import com.volcengine.vertcdemo.core.eventbus.AppTokenExpiredEvent;
import com.volcengine.vertcdemo.core.net.ErrorTool;
import com.volcengine.vertcdemo.core.net.IRequestCallback;
import com.volcengine.vertcdemo.core.net.ServerResponse;
import com.volcengine.vertcdemo.core.net.rts.RTSBaseClient;
import com.volcengine.vertcdemo.core.net.rts.RTSInfo;
import com.volcengine.vertcdemo.groupvideocall.bean.JoinRoomResponse;
import com.volcengine.vertcdemo.groupvideocall.core.VideoCallRTCManager;
import com.volcengine.vertcdemo.groupvideocall.databinding.ActivityPreviewBinding;
import com.volcengine.vertcdemo.groupvideocall.event.AudioRouterEvent;
import com.volcengine.vertcdemo.groupvideocall.event.MediaStatusEvent;
import com.volcengine.vertcdemo.groupvideocall.event.RefreshPreviewEvent;
import com.volcengine.vertcdemo.groupvideocall.feature.main.RoomMainActivity;
import com.volcengine.vertcdemo.utils.AppUtil;
import com.volcengine.vertcdemo.utils.Utils;
import com.volcengine.vertcdemo.groupvideocall.R;
import com.volcengine.vertcdemo.groupvideocall.core.Constants;

import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.regex.Pattern;
/**
 * preview page
 *
 * Function:
 * 1. Control the switch status of camera, microphone and speaker
 * 2. Preview your own video
 * 3. Enter the room
 */
public class PreviewActivity extends SolutionBaseActivity {

    private static final String TAG = "PreviewActivity";

    public static final String ROOM_INPUT_REGEX = "^[0-9]+$";
    // The rts related data needed for this scene
    private RTSInfo mRTSInfo;
    // room id
    private String mRoomId;

    private ActivityPreviewBinding mViewBinding;
    // Whether the length of the room id is too long
    private boolean mRoomIdOverflow = false;
    // The room id length prompt automatically disappears the delayed task
    private final Runnable mRoomIdDismissRunnable = () -> mViewBinding.previewInputError.setVisibility(View.GONE);

    private final TextWatcherAdapter mTextWatcher = new TextWatcherAdapter() {
        @Override
        public void afterTextChanged(Editable s) {
            updateJoinButtonStatus();
        }
    };

    private final IRequestCallback<JoinRoomResponse> mJoinCallback = new IRequestCallback<JoinRoomResponse>() {
        @Override
        public void onSuccess(JoinRoomResponse data) {
            Intent intent = new Intent(PreviewActivity.this, RoomMainActivity.class);
            intent.putExtra(Constants.EXTRA_ROOM_ID, mRoomId);
            intent.putExtra(Constants.EXTRA_TOKEN, data.rtcToken);
            intent.putExtra(Constants.EXTRA_LAST_TS, data.durationS * 1000);
            startActivity(intent);
        }

        @Override
        public void onError(int errorCode, String message) {
            SolutionToast.show(ErrorTool.getErrorMessageByErrorCode(errorCode, message));
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        mViewBinding = ActivityPreviewBinding.inflate(getLayoutInflater());
        setContentView(mViewBinding.getRoot());
        
        initRTSInfo();

        SolutionToast.show(R.string.minutes_meeting_title);

        mViewBinding.previewBackBtn.setOnClickListener((v) -> finish());

        mViewBinding.previewInputEt.removeTextChangedListener(mTextWatcher);
        mViewBinding.previewInputEt.addTextChangedListener(mTextWatcher);

        InputFilter roomIDFilter = new LengthFilterWithCallback(18, (overflow) -> {
            if (overflow) {
                mViewBinding.previewInputError.setVisibility(View.VISIBLE);
                mViewBinding.previewInputError.setText(R.string.room_number_error_content_limit);
                mViewBinding.previewInputError.removeCallbacks(mRoomIdDismissRunnable);
                mViewBinding.previewInputError.postDelayed(mRoomIdDismissRunnable, 2500);
            } else {
                mViewBinding.previewInputError.setVisibility(View.GONE);
                mViewBinding.previewInputError.removeCallbacks(mRoomIdDismissRunnable);
            }
            mRoomIdOverflow = overflow;
        });
        InputFilter[] meetingIDFilters = new InputFilter[]{roomIDFilter};
        mViewBinding.previewInputEt.setFilters(meetingIDFilters);
        
        mViewBinding.previewEnterBtn.setOnClickListener((v) -> enterRoom());
        mViewBinding.previewEnterBtn.setEnabled(false);

        String appVersion = getString(R.string.app_version_vxxx, AppUtil.getAppVersionName());
        String SDKVersion = getString(R.string.sdk_version_vxxx, RTCVideo.getSDKVersion());
        String version = String.format("%s / %s", appVersion, SDKVersion);
        mViewBinding.previewVersion.setText(version);

        initRTC();

        SolutionDemoEventManager.register(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        setLocalVideoCanvas();
        
        if (VideoCallRTCManager.ins().isCameraOn()) {
            VideoCallRTCManager.ins().startVideoCapture(true);
        }
        if (VideoCallRTCManager.ins().isMicOn()) {
            VideoCallRTCManager.ins().startPublishAudio(true);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        SolutionDemoEventManager.unregister(this);
        VideoCallRTCManager.ins().clearUserView();
        VideoCallRTCManager.ins().destroyEngine();
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
    /**
     * Get RTS information from Intent
     */
    private void initRTSInfo() {
        Intent intent = getIntent();
        if (intent == null) {
            return;
        }
        mRTSInfo = intent.getParcelableExtra(RTSInfo.KEY_RTS);
        if (mRTSInfo == null || !mRTSInfo.isValid()) {
            finish();
        }
    }
    /**
     * Initialize RTC according to the RTS information obtained from the server
     */
    private void initRTC() {
        VideoCallRTCManager.ins().initEngine(mRTSInfo);
        RTSBaseClient rtsClient = VideoCallRTCManager.ins().getRTSClient();
        if (rtsClient == null) {
            finish();
            return;
        }
        rtsClient.login(mRTSInfo.rtsToken, (resultCode, message) -> {
            if (resultCode == RTSBaseClient.LoginCallBack.SUCCESS) {
                initView();
            } else {
                SolutionToast.show("Login Rtm Fail Error:" + resultCode + ",Message:" + message);
            }
        });
    }

    private void initView() {
        mViewBinding.previewCameraBtn.setImageResource(VideoCallRTCManager.ins().isCameraOn()
                ? R.drawable.camera_enable_icon : R.drawable.camera_disable_icon);
        mViewBinding.previewMicBtn.setImageResource(VideoCallRTCManager.ins().isMicOn()
                ? R.drawable.microphone_enable_icon : R.drawable.microphone_disable_icon);
        mViewBinding.previewSpeakerPhoneBtn.setImageResource(VideoCallRTCManager.ins().isSpeakerphone()
                ? R.drawable.speakerphone_icon : R.drawable.earpiece_icon);

        mViewBinding.previewCameraBtn.setOnClickListener((v) -> VideoCallRTCManager.ins()
                .startVideoCapture(!VideoCallRTCManager.ins().isCameraOn()));
        mViewBinding.previewMicBtn.setOnClickListener((v) -> VideoCallRTCManager.ins()
                .startPublishAudio(!VideoCallRTCManager.ins().isMicOn()));
        mViewBinding.previewSpeakerPhoneBtn.setOnClickListener((v) -> VideoCallRTCManager.ins()
                .useSpeakerphone(!VideoCallRTCManager.ins().isSpeakerphone()));

        setLocalVideoCanvas();
    }
    /**
     * According to the room name entered by the user, control the state of the button to join the room
     */
    private void updateJoinButtonStatus() {
        int roomIDLength = mViewBinding.previewInputEt.getText().length();
        boolean meetingIdRegexMatch = false;
        if (Pattern.matches(ROOM_INPUT_REGEX, mViewBinding.previewInputEt.getText().toString())) {
            if (mRoomIdOverflow) {
                mViewBinding.previewInputError.setVisibility(View.VISIBLE);
                mViewBinding.previewInputError.setText(R.string.room_number_error_content_limit);
                mViewBinding.previewInputError.removeCallbacks(mRoomIdDismissRunnable);
                mViewBinding.previewInputError.postDelayed(mRoomIdDismissRunnable, 2500);
            } else {
                mViewBinding.previewInputError.setVisibility(View.INVISIBLE);
                meetingIdRegexMatch = true;
            }
        } else {
            if (roomIDLength > 0) {
                mViewBinding.previewInputError.setVisibility(View.VISIBLE);
                mViewBinding.previewInputError.setText(R.string.room_number_error_content_limit);
            } else {
                mViewBinding.previewInputError.setVisibility(View.INVISIBLE);
            }
        }
        boolean joinBtnEnable = roomIDLength > 0 && roomIDLength <= 18
                && meetingIdRegexMatch;
        mViewBinding.previewEnterBtn.setEnabled(joinBtnEnable);
    }
    /**
     * Join the room page
     */
    private void enterRoom() {
        String roomId = mViewBinding.previewInputEt.getText().toString().trim();
        if (TextUtils.isEmpty(roomId)) {
            return;
        }
        // todo 隔离临时方案
        roomId = "call_" + roomId;
        mRoomId = roomId;
        VideoCallRTCManager.ins().getRTSClient().requestJoinRoom(roomId, mJoinCallback);
    }
    /**
     * Set the local video rendering
     */
    private void setLocalVideoCanvas() {
        if (VideoCallRTCManager.ins().isCameraOn()) {
            TextureView textureView = VideoCallRTCManager.ins().getRenderView(SolutionDataManager.ins().getUserId());
            FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            Utils.attachViewToViewGroup(mViewBinding.previewRenderContainer, textureView, params);
            VideoCallRTCManager.ins().setLocalVideoCanvas(false, textureView);
            mViewBinding.previewRenderDisable.setVisibility(View.GONE);
        } else {
            mViewBinding.previewRenderContainer.removeAllViews();
            mViewBinding.previewRenderDisable.setVisibility(View.VISIBLE);
        }
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onMediaStatusEvent(MediaStatusEvent event) {
        if (!TextUtils.equals(SolutionDataManager.ins().getUserId(), event.uid)) {
            return;
        }
        boolean on = event.status == Constants.MEDIA_STATUS_ON;
        if (event.mediaType == Constants.MEDIA_TYPE_AUDIO) {
            mViewBinding.previewMicBtn.setImageResource(on ? R.drawable.microphone_enable_icon
                    : R.drawable.microphone_disable_icon);
        } else if (event.mediaType == Constants.MEDIA_TYPE_VIDEO) {
            mViewBinding.previewCameraBtn.setImageResource(on ? R.drawable.camera_enable_icon :
                    R.drawable.camera_disable_icon);
            setLocalVideoCanvas();
        }
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onAudioRouterEvent(AudioRouterEvent event) {
        mViewBinding.previewSpeakerPhoneBtn.setImageResource(event.isSpeakerPhone ? R.drawable.speakerphone_icon
                : R.drawable.earpiece_icon);
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onRefreshPreviewEvent(RefreshPreviewEvent event) {
        // Restore all media to default state
        VideoCallRTCManager.ins().startVideoCapture(true);
        VideoCallRTCManager.ins().startPublishAudio(true);
        VideoCallRTCManager.ins().switchCamera(true);
        VideoCallRTCManager.ins().useSpeakerphone(true);
        VideoCallRTCManager.ins().setMirrorType(true);

        setLocalVideoCanvas();
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onTokenExpiredEvent(AppTokenExpiredEvent event) {
        finish();
    }
    /**
     * The entry method of the scene
     *
     * The shell project calls this method through reflection, and this page will be opened after success,
     * otherwise a prompt will pop up
     *
     * @param activity caller page
     * @param doneAction callback after finishing joinRTM
     */
    @Keep
    @SuppressWarnings("unused")
    public static void prepareSolutionParams(Activity activity, IAction<Object> doneAction) {
        Log.d(TAG, "prepareSolutionParams() invoked");
        IRequestCallback<ServerResponse<RTSInfo>> callback = new IRequestCallback<ServerResponse<RTSInfo>>() {
            @Override
            public void onSuccess(ServerResponse<RTSInfo> response) {
                RTSInfo data = response == null ? null : response.getData();
                if (data == null || !data.isValid()) {
                    onError(-1, "");
                    return;
                }
                Intent intent = new Intent(Intent.ACTION_MAIN);
                intent.setClass(AppUtil.getApplicationContext(), PreviewActivity.class);
                intent.putExtra(KEY_RTS, data);
                activity.startActivity(intent);
                if (doneAction != null) {
                    doneAction.act(null);
                }
            }

            @Override
            public void onError(int errorCode, String message) {
                if (doneAction != null) {
                    doneAction.act(null);
                }
            }
        };
        JoinRTSRequest request = new JoinRTSRequest(Constants.SOLUTION_NAME_ABBR, SolutionDataManager.ins().getToken());
        JoinRTSManager.setAppInfoAndJoinRTM(request, callback);
    }
}