// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.videocall.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.DrawableRes;
import androidx.annotation.Nullable;
import androidx.annotation.StringRes;

import com.volcengine.vertcdemo.videocall.R;
import com.volcengine.vertcdemo.videocall.databinding.LayoutBottomOptionViewBinding;
import com.volcengine.vertcdemo.videocall.databinding.LayoutFullScreenBinding;
/**
 * Single function view in the functional area at the bottom of the room page
 *
 * Contains an icon and text views
 *
 * Function:
 * 1. Set icon resource {@link #setIcon(int)}
 * 2. Set up copywriting resources {@link #setText(int)}
 */
public class BottomOptionView extends LinearLayout {

    private LayoutBottomOptionViewBinding mViewBinding;

    public BottomOptionView(Context context) {
        super(context);
        initView();
    }

    public BottomOptionView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public BottomOptionView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
    }

    private void initView() {
        View view = View.inflate(getContext(), R.layout.layout_bottom_option_view, this);
        mViewBinding = LayoutBottomOptionViewBinding.bind(view);
    }
    /**
     * Set icon resources
     *
     * @param id resource id
     */
    public void setIcon(@DrawableRes int id) {
        mViewBinding.optionIcon.setImageResource(id);
    }
    /**
     * Set text resources
     *
     * @param id resource id
     */
    public void setText(@StringRes int id) {
        mViewBinding.optionText.setText(id);
    }
}
