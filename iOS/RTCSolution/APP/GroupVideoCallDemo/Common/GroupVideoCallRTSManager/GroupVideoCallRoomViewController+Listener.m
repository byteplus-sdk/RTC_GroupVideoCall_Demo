// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import "GroupVideoCallRoomViewController+Listener.h"
#import "GroupVideoCallRTSManager.h"
#import "GroupVideoCallRTCManager.h"

@implementation GroupVideoCallRoomViewController (Listener)

- (void)addRTSListener {
    __weak __typeof(self) wself = self;
    //User Join
    [GroupVideoCallRTSManager onCloseRoomWithBlock:^(NSString * _Nonnull roomId) {
        [wself hangUp];
        [[ToastComponent shareToastComponent] showWithMessage:LocalizedString(@"minutes_meeting") delay:0.8];
    }];
        
}

@end
