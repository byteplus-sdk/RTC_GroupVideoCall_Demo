// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.volcengine.vertcdemo.videocall.R;
import com.volcengine.vertcdemo.videocall.core.VideoCallRTCManager;
import com.volcengine.vertcdemo.videocall.databinding.LayoutTitleBinding;

import java.util.Locale;
/**
 * Functional area at the top of the room home page
 *
 * Included features:
 * 1. Switch the front and rear cameras (directly call the interface of RTC manager internally)
 * 2. Display room name {@link #setRoomId(String)}
 * 3. Room duration timer {@link #startCountDown(long)}
 * 4. Leave the room (call back the event to the caller via {@link #setITitleCallback(ITitleCallback)})
 */
public class TitleView extends ConstraintLayout {

    private long mStartTs;
    // Title button click event
    private ITitleCallback mITitleCallback;
    
    private LayoutTitleBinding mViewBinding;

    public TitleView(@NonNull Context context) {
        super(context);
        initView();
    }

    public TitleView(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public TitleView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
    }

    private void initView() {
        View view = View.inflate(getContext(), R.layout.layout_title, this);
        mViewBinding = LayoutTitleBinding.bind(view);

        mViewBinding.titleZoom.setOnClickListener((v) -> {
            if (mITitleCallback != null) {
                mITitleCallback.onZoomClick();
            }
        });
        mViewBinding.titleSwitchCamera.setOnClickListener((v) ->
                VideoCallRTCManager.ins().switchCamera(!VideoCallRTCManager.ins().isFrontCamera()));

        mViewBinding.titleHangUp.setOnClickListener((v) -> {
            if (mITitleCallback != null) {
                mITitleCallback.onHangUpClick();
            }
        });
    }

    public void setITitleCallback(ITitleCallback callback) {
        this.mITitleCallback = callback;
    }
    /**
     * Set room id
     * @param roomId room id
     */
    public void setRoomId(@Nullable String roomId) {
        if (roomId == null) {
            roomId = "";
        }
        // todo 隔离临时方案
        mViewBinding.titleRoomId.setText(String.format("ID: %s", roomId.replace("call_", "")));
    }
    /**
     * Start countdown
     *
     * @param lastTimeMs duration, unit ms
     */
    public void startCountDown(long lastTimeMs) {
        mStartTs = System.currentTimeMillis() - lastTimeMs;
        updateDuration();
    }
    /**
     * Update countdown
     */
    private void updateDuration() {
        long lastTs = System.currentTimeMillis() - mStartTs;
        mViewBinding.titleDuration.setText(formatTs(lastTs));

        mViewBinding.titleDuration.postDelayed(this::updateDuration, 500);
    }
    /**
     * format time
     *
     * @param time timestamp, unit ms
     * @return mm:ss
     */
    private String formatTs(long time) {
        long minute = time / 60_000;
        String mStr;
        if (minute >= 10) {
            mStr = String.format(Locale.US, "%d", minute);
        } else if (minute > 0) {
            mStr = String.format(Locale.US, "0%d", minute);
        } else {
            mStr = "00";
        }

        long second = (time / 1000) % 60;
        String sStr;
        if (second >= 10) {
            sStr = String.format(Locale.US, "%d", second);
        } else if (second > 0) {
            sStr = String.format(Locale.US, "0%d", second);
        } else {
            sStr = "00";
        }

        return String.format(Locale.US, "%s:%s", mStr, sStr);
    }

    public interface ITitleCallback {

        void onZoomClick();

        void onHangUpClick();
    }
}
