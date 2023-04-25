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
import com.volcengine.vertcdemo.videocall.databinding.LayoutPortraitScreenBinding;
/**
 * The room page displays screen sharing views in portrait mode
 *
 * Function:
 * 1. Bind user data {@link #bind(VideoCallUserInfo)}
 * 2. Set the zoom event {@link #setExpandAction(IAction)}
 */
public class PortraitLayout extends FrameLayout {

    private LayoutPortraitScreenBinding mViewBinding;
    // Currently displayed user information
    private VideoCallUserInfo mUserInfo;
    // Click event of zoom button
    private IAction<VideoCallUserInfo> mExpandAction;

    public PortraitLayout(@NonNull Context context) {
        super(context);
        initView();
    }

    public PortraitLayout(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public PortraitLayout(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
    }

    private void initView() {
        View view = View.inflate(getContext(), R.layout.layout_portrait_screen, this);
        mViewBinding = LayoutPortraitScreenBinding.bind(view);

        mViewBinding.fullScreenUserRenderZoom.setOnClickListener((v) -> {
            if (mExpandAction != null) {
                mExpandAction.act(mUserInfo);
            }
        });
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
     * Set zoom event callback
     *
     * @param expandAction callback
     */
    public void setExpandAction(IAction<VideoCallUserInfo> expandAction) {
        mExpandAction = expandAction;
    }
}