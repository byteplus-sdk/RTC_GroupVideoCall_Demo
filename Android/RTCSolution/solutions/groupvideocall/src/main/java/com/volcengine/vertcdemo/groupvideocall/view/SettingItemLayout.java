// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT

package com.volcengine.vertcdemo.groupvideocall.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.volcengine.vertcdemo.groupvideocall.R;
import com.volcengine.vertcdemo.groupvideocall.databinding.ItemSettingLayoutBinding;
/**
 * Setting dialog box single setting view
 *
 * Function:
 * 1. Set the string value
 * 2. Set the switch type value
 */
public class SettingItemLayout extends FrameLayout {

    private ItemSettingLayoutBinding mViewBinding;

    public SettingItemLayout(@NonNull Context context) {
        super(context);
        initView();
    }

    public SettingItemLayout(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        initView();
    }

    public SettingItemLayout(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initView();
    }

    private void initView() {
        View view = View.inflate(getContext(), R.layout.item_setting_layout, this);
        mViewBinding = ItemSettingLayoutBinding.bind(view);
    }
    /**
     * Set the string type value
     *
     * @param key setting name
     * @param value setting value
     */
    public void setData(String key, String value) {
        mViewBinding.settingKey.setText(key);
        mViewBinding.settingTextValue.setText(value);

        mViewBinding.settingTextValueLayout.setVisibility(VISIBLE);
        mViewBinding.settingSwitchValue.setVisibility(GONE);
    }
    /**
     * Set switch type value
     *
     * @param key setting name
     * @param value setting value
     */
    public void setData(String key, boolean value) {
        mViewBinding.settingKey.setText(key);
        mViewBinding.settingSwitchValue.setChecked(value);

        mViewBinding.settingTextValueLayout.setVisibility(GONE);
        mViewBinding.settingSwitchValue.setVisibility(VISIBLE);
    }
    /**
     * Set the switch state change callback
     *
     * @param listener callback
     */
    public void setOnCheckListener(CompoundButton.OnCheckedChangeListener listener) {
        mViewBinding.settingSwitchValue.setOnCheckedChangeListener(listener);
    }
}
