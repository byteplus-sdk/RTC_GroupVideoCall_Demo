// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.volcengine.vertcdemo.common.IAction;
import com.volcengine.vertcdemo.utils.Utils;
import com.volcengine.vertcdemo.videocall.R;
import com.volcengine.vertcdemo.videocall.bean.VideoCallUserInfo;
import com.volcengine.vertcdemo.videocall.core.VideoCallRTCManager;
import com.volcengine.vertcdemo.videocall.databinding.LayoutFullScreenBinding;
/**
 * Landscape screen sharing page views
 *
 * Used for horizontal screen page data display
 *
 * Function:
 * 1. Bind data {@link #bind(VideoCallUserInfo)}
 * 2. Set zoom out callback {@link #setZoomAction(IAction)}
 */
public class LandScapeScreenLayout extends FrameLayout {

    private LayoutFullScreenBinding mViewBinding;

    public VideoCallUserInfo mUserInfo;
    private IAction<VideoCallUserInfo> mZoomAction;

    public LandScapeScreenLayout(@NonNull Context context) {
        super(context);
        initView();
    }

    public LandScapeScreenLayout(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public LandScapeScreenLayout(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
    }

    private void initView() {
        View view = View.inflate(getContext(), R.layout.layout_full_screen, this);
        mViewBinding = LayoutFullScreenBinding.bind(view);
        mViewBinding.fullScreenUserRenderZoom.setOnClickListener((v -> {
            if (mZoomAction != null) {
                mZoomAction.act(mUserInfo);
            }
        }));
    }
    /**
     * Bind user data
     *
     * @param userInfo user information
     */
    public void bind(@Nullable VideoCallUserInfo userInfo) {
        mViewBinding.fullScreenUserRenderContainer.removeAllViews();
        mUserInfo = userInfo;
        if (userInfo == null) {
            mViewBinding.fullScreenUserName.setText("");
            mViewBinding.fullScreenUserMic.setImageResource(R.drawable.microphone_enable_icon);
            mViewBinding.fullScreenUserRenderContainer.removeAllViews();
            return;
        }
        mViewBinding.fullScreenUserName.setText(getResources().getString(R.string.xxxs_screen_sharing, userInfo.userName));
        ViewGroup.LayoutParams layoutParams = new LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        TextureView view = VideoCallRTCManager.ins().getScreenRenderView(userInfo.userId);
        Utils.attachViewToViewGroup(mViewBinding.fullScreenUserRenderContainer, view, layoutParams);
    }
    /**
     * Set zoom out event callback
     *
     * @param zoomAction callback
     */
    public void setZoomAction(@Nullable IAction<VideoCallUserInfo> zoomAction) {
        mZoomAction = zoomAction;
    }
}
