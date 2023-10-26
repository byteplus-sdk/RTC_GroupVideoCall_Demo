// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import <UIKit/UIKit.h>
#import "GroupVideoCallAvatarView.h"

NS_ASSUME_NONNULL_BEGIN


@interface GroupVideoCallNameView : UIView

- (void)setMicStatus:(GroupVideoCallAvatarViewMicStatus)status;

- (void)setName:(NSString *)name;

@end

NS_ASSUME_NONNULL_END
