// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.view;

import android.content.Context;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.View;
import android.widget.LinearLayout;

import androidx.annotation.Nullable;

import com.volcengine.vertcdemo.core.SolutionDataManager;
import com.volcengine.vertcdemo.core.eventbus.SolutionDemoEventManager;
import com.volcengine.vertcdemo.videocall.R;
import com.volcengine.vertcdemo.videocall.core.Constants;
import com.volcengine.vertcdemo.videocall.core.VideoCallRTCManager;
import com.volcengine.vertcdemo.videocall.databinding.LayoutBottomOptionGroupBinding;
import com.volcengine.vertcdemo.videocall.event.AudioRouterEvent;
import com.volcengine.vertcdemo.videocall.event.MediaStatusEvent;

import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;
/**
 * Functional area at the bottom of the room page
 *
 * Contains five independent function buttons
 *
 * Included features:
 * 1. Open and close the microphone (call the RTC manager interface internally, and change the UI display according to the event)
 * 2. Open and close the camera (call the RTC manager interface internally, and change the UI display according to the event)
 * 3. Switch the speaker (call the RTC manager interface internally, and change the UI display according to the event)
 * 4. Open the real-time data statistics dialog box
 * 5. Open the settings dialog
 */
public class BottomOptionGroup extends LinearLayout {

    private LayoutBottomOptionGroupBinding mViewBinding;

    public BottomOptionGroup(Context context) {
        super(context);
        initView();
    }

    public BottomOptionGroup(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public BottomOptionGroup(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
    }

    private void initView() {
        View view = View.inflate(getContext(), R.layout.layout_bottom_option_group, this);
        mViewBinding = LayoutBottomOptionGroupBinding.bind(view);

        mViewBinding.groupMicrophone.setText(R.string.microphone);
        mViewBinding.groupCamera.setText(R.string.camera);
        mViewBinding.groupMediaStats.setText(R.string.real_time_data);
        mViewBinding.groupSetting.setText(R.string.set_up);

        updateMicrophone(VideoCallRTCManager.ins().isMicOn());
        updateCamera(VideoCallRTCManager.ins().isCameraOn());
        updateSpeakerPhone(VideoCallRTCManager.ins().isSpeakerphone());
        mViewBinding.groupMediaStats.setIcon(R.drawable.media_stat_icon);
        mViewBinding.groupSetting.setIcon(R.drawable.setting_icon);

        mViewBinding.groupMicrophone.setOnClickListener((v) ->
                VideoCallRTCManager.ins().startPublishAudio(!VideoCallRTCManager.ins().isMicOn()));
        mViewBinding.groupCamera.setOnClickListener((v) ->
                VideoCallRTCManager.ins().startVideoCapture(!VideoCallRTCManager.ins().isCameraOn()));
        mViewBinding.groupSpeakerPhone.setOnClickListener((v) ->
                VideoCallRTCManager.ins().useSpeakerphone(!VideoCallRTCManager.ins().isSpeakerphone()));
        mViewBinding.groupMediaStats.setOnClickListener((v) -> {
            MediaStatsDialog mediaStatsDialog = new MediaStatsDialog(getContext());
            mediaStatsDialog.show();
        });
        mViewBinding.groupSetting.setOnClickListener((v) -> {
            SettingDialog settingDialog = new SettingDialog(getContext());
            settingDialog.show();
        });
    }
    /**
     * Update UI based on microphone status
     *
     * @param isOn Whether to open
     */
    public void updateMicrophone(boolean isOn) {
        mViewBinding.groupMicrophone.setIcon(isOn ? R.drawable.microphone_enable_icon : R.drawable.microphone_disable_icon);
    }
    /**
     * Update the UI according to the camera status
     *
     * @param isOn Whether to open
     */
    public void updateCamera(boolean isOn) {
        mViewBinding.groupCamera.setIcon(isOn ? R.drawable.camera_enable_icon : R.drawable.camera_disable_icon);
    }
    /**
     * Update UI based on audio routing status
     *
     * @param isOn Whether to use the speaker
     */
    public void updateSpeakerPhone(boolean isOn) {
        mViewBinding.groupSpeakerPhone.setIcon(isOn ? R.drawable.speakerphone_icon
                : R.drawable.earpiece_icon);
        mViewBinding.groupSpeakerPhone.setText(isOn ? R.string.speaker : R.string.earpiece);
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        SolutionDemoEventManager.register(this);
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        SolutionDemoEventManager.unregister(this);
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onMediaStatusEvent(MediaStatusEvent event) {
        if (!TextUtils.equals(SolutionDataManager.ins().getUserId(), event.uid)) {
            return;
        }
        boolean on = event.status == Constants.MEDIA_STATUS_ON;
        if (event.mediaType == Constants.MEDIA_TYPE_AUDIO) {
            mViewBinding.groupMicrophone.setIcon(on ? R.drawable.microphone_enable_icon
                    : R.drawable.microphone_disable_icon);
        } else if (event.mediaType == Constants.MEDIA_TYPE_VIDEO) {
            mViewBinding.groupCamera.setIcon(on ? R.drawable.camera_enable_icon :
                    R.drawable.camera_disable_icon);
        }
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onAudioRouterEvent(AudioRouterEvent event) {
        updateSpeakerPhone(event.isSpeakerPhone);
    }
}
