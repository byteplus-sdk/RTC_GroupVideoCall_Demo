// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import <UIKit/UIKit.h>
#import "GroupVideoCallRoomUserModel.h"
@class GroupVideoCallRoomNavView;

typedef NS_ENUM(NSInteger, RoomNavStatus) {
    RoomNavStatusSwitchCamera,
    RoomNavStatusHangeup
};

NS_ASSUME_NONNULL_BEGIN

@protocol GroupVideoCallRoomNavViewDelegate <NSObject>

- (void)GroupVideoCallRoomNavView:(GroupVideoCallRoomNavView *)GroupVideoCallRoomNavView didSelectStatus:(RoomNavStatus)status;

@end

@interface GroupVideoCallRoomNavView : UIView

@property (nonatomic, strong) GroupVideoCallRoomUserModel *localVideoSession;

@property (nonatomic, weak) id<GroupVideoCallRoomNavViewDelegate> delegate;

@property (nonatomic, assign) NSInteger meetingTime;

@end

NS_ASSUME_NONNULL_END
