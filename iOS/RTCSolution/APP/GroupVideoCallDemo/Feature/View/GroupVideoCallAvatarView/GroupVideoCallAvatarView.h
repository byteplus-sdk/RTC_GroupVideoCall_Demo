// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import <UIKit/UIKit.h>

typedef NS_ENUM(NSInteger, GroupVideoCallAvatarViewVideoStatus) {
    GroupVideoCallAvatarViewVideoStatusOff,
    GroupVideoCallAvatarViewVideoStatusOn
};

typedef NS_ENUM(NSInteger, GroupVideoCallAvatarViewMicStatus) {
    GroupVideoCallAvatarViewMicStatusOff,
    GroupVideoCallAvatarViewMicStatusOn,
    GroupVideoCallAvatarViewMicStatusSpeaking
};

NS_ASSUME_NONNULL_BEGIN

@interface GroupVideoCallAvatarView : UIView

@property (nonatomic, strong) UIView *videoContainerView;

- (void)setVideoStatus:(GroupVideoCallAvatarViewVideoStatus)status;

- (void)setMicStatus:(GroupVideoCallAvatarViewMicStatus)status;

- (void)setName:(NSString *)name;

@end

NS_ASSUME_NONNULL_END
