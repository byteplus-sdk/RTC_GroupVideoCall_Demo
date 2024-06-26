//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import "BaseRTCManager.h"
#import "GroupVideoCallRoomUserModel.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface GroupVideoCallRTSManager : BaseRTCManager

#pragma mark - Get meeting data

/*
 * Join the meeting
 * @param loginModel Login user data
 * @param block Callback
 */
+ (void)joinRoom:(GroupVideoCallRoomUserModel *)loginModel
           block:(void (^)(NSString *token,
                           NSInteger duration,
                           RTSACKModel *model))block;

/*
 * Leave room
 */
+ (void)leaveRoom;

/*
 * Reconnect
 * @param block Callback
 */
+ (void)reconnectWithBlock:(void (^)(NSString *RTCToken,
                                     RTSACKModel *model))block;

#pragma mark - Notification message

/*
 * On close room
 * @param block Callback
 */
+ (void)onCloseRoomWithBlock:(void (^)(NSString *roomId))block;

@end

NS_ASSUME_NONNULL_END
