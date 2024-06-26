//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import "GroupVideoCallRTSManager.h"
#import "GroupVideoCallRTCManager.h"
#import "JoinRTSParams.h"
#import "ToolKit.h"

@implementation GroupVideoCallRTSManager

#pragma mark - Get meeting data

+ (void)joinRoom:(GroupVideoCallRoomUserModel *)loginModel
           block:(void (^)(NSString *token,
                           NSInteger duration,
                           RTSACKModel *model))block {
    NSMutableDictionary *dic = [[NSMutableDictionary alloc] init];
    [dic setValue:loginModel.uid forKey:@"user_id"];
    [dic setValue:loginModel.roomId forKey:@"room_id"];

    NSDictionary *dicData = [JoinRTSParams addTokenToParams:[dic copy]];
    [[GroupVideoCallRTCManager shareRtc] emitWithAck:@"videocallJoinRoom"
                                                with:dicData
                                               block:^(RTSACKModel *_Nonnull ackModel) {
                                                   NSString *token = @"";
                                                   NSInteger duration = 0;
                                                   if ([ackModel.response isKindOfClass:[NSDictionary class]]) {
                                                       token = ackModel.response[@"rtc_token"];
                                                       duration = [[NSString stringWithFormat:@"%@", ackModel.response[@"duration"]] integerValue];
                                                   }
                                                   dispatch_async(dispatch_get_main_queue(), ^{
                                                       if (block) {
                                                           block(token, duration, ackModel);
                                                       }
                                                   });
                                               }];
}

+ (void)leaveRoom {
    NSDictionary *dic = [JoinRTSParams addTokenToParams:nil];
    [[GroupVideoCallRTCManager shareRtc] emitWithAck:@"videocallLeaveRoom"
                                                with:dic
                                               block:nil];
}

+ (void)reconnectWithBlock:(void (^)(NSString *RTCToken,
                                     RTSACKModel *model))block {
    NSDictionary *dic = [JoinRTSParams addTokenToParams:nil];
    [[GroupVideoCallRTCManager shareRtc] emitWithAck:@"videocallReconnect"
                                                with:dic
                                               block:^(RTSACKModel *_Nonnull ackModel) {
                                                   NSString *token = @"";
                                                   if ([ackModel.response isKindOfClass:[NSDictionary class]]) {
                                                       token = ackModel.response[@"rtc_token"];
                                                   }
                                                   dispatch_async(dispatch_get_main_queue(), ^{
                                                       if (block) {
                                                           block(token, ackModel);
                                                       }
                                                   });
                                               }];
}

#pragma mark - Notification message

+ (void)onCloseRoomWithBlock:(void (^)(NSString *roomId))block {
    [[GroupVideoCallRTCManager shareRtc] onSceneListener:@"videocallOnCloseRoom" block:^(RTSNoticeModel *_Nonnull noticeModel) {
        NSString *roomId = @"";
        if ([noticeModel.data isKindOfClass:[NSDictionary class]]) {
            roomId = noticeModel.data[@"room_id"];
        }
        if (block) {
            block(roomId);
        }
    }];
}

@end
