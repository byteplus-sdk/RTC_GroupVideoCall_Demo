//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import "GroupVideoCallRoomItemButton.h"
#import <UIKit/UIKit.h>
@class GroupVideoCallRoomBottomView;

typedef NS_ENUM(NSInteger, RoomBottomStatus) {
    RoomBottomStatusMic = 0,
    RoomBottomStatusCamera,
    RoomBottomStatusAudio,
    RoomBottomStatusParameter,
    RoomBottomStatusSet,
};

@protocol GroupVideoCallRoomBottomViewDelegate <NSObject>

- (void)GroupVideoCallRoomBottomView:(GroupVideoCallRoomBottomView *_Nonnull)GroupVideoCallRoomBottomView itemButton:(GroupVideoCallRoomItemButton *_Nullable)itemButton didSelectStatus:(RoomBottomStatus)status;

@end

NS_ASSUME_NONNULL_BEGIN

@interface GroupVideoCallRoomBottomView : UIView

@property (nonatomic, weak) id<GroupVideoCallRoomBottomViewDelegate> delegate;

- (void)updateButtonStatus:(RoomBottomStatus)status close:(BOOL)isClose;

- (ButtonStatus)getButtonStatus:(RoomBottomStatus)status;

- (void)updateButtonStatus:(RoomBottomStatus)status enable:(BOOL)isEnable;

@end

NS_ASSUME_NONNULL_END
