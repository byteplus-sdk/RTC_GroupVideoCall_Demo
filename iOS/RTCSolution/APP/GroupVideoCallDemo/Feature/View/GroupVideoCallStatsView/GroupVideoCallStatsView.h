// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import <UIKit/UIKit.h>
#import "GroupVideoCallRoomParamInfoModel.h"

NS_ASSUME_NONNULL_BEGIN

@interface GroupVideoCallStatsView : UIView

- (void)setVideoStats:(NSArray <GroupVideoCallRoomParamInfoModel *>*)videoStats;

- (void)setAudioStats:(NSArray <GroupVideoCallRoomParamInfoModel *>*)audioStats;

- (void)showStatsView;

@end

NS_ASSUME_NONNULL_END
