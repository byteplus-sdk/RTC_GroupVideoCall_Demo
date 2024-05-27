//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import "BaseRTCManager.h"
#import "GroupVideoCallRTCManager.h"
#import "GroupVideoCallRoomParamInfoModel.h"
#import "GroupVideoCallRoomUserModel.h"
#import "GroupVideoCallRoomViewController.h"

NS_ASSUME_NONNULL_BEGIN

@class GroupVideoCallRTCManager;
@protocol GroupVideoCallRTCManagerDelegate <NSObject>

/**
 * @brief Callback on room state changes. Via this callback you get notified of room relating warnings, errors and events. For example, the user joins the room, the user is removed from the room, and so on.
 * @param manager GameRTCManager model
 * @param userID UserID
 */
- (void)groupVideoCallRTCManager:(GroupVideoCallRTCManager *)manager
              onRoomStateChanged:(RTCJoinModel *)joinModel;

/**
 * @brief video message status change
 * @param rtcManager GroupVideoCallRTCManager object
 * @param statsInfo status information, key is user id, value is status information content.
 */
- (void)rtcManager:(GroupVideoCallRTCManager *_Nullable)rtcManager didUpdateVideoStatsInfo:(NSDictionary<NSString *, GroupVideoCallRoomParamInfoModel *> *_Nullable)statsInfo;

/**
 * @brief audio message status change
 * @param rtcManager GroupVideoCallRTCManager object
 * @param statsInfo status information, key is user id, value is status information content.
 */
- (void)rtcManager:(GroupVideoCallRTCManager *_Nullable)rtcManager didUpdateAudioStatsInfo:(NSDictionary<NSString *, GroupVideoCallRoomParamInfoModel *> *_Nullable)statsInfo;

/**
 * @brief The remote user joins the room callback
 * @param rtcManager GroupVideoCallRTCManager object
 * @param uid user user id
 * @param name user nickname
 */
- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager onUserJoined:(NSString *_Nullable)uid userName:(NSString *_Nullable)name;

/**
 * @brief The remote user leaves the room callback
 * @param rtcManager GroupVideoCallRTCManager object
 * @param uid user user id
 */
- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager onUserLeaved:(NSString *_Nullable)uid;

/**
 * @brief Add remote user screen stream callback
 * @param rtcManager GroupVideoCallRTCManager object
 * @param screenStreamsUid user user id
 */
- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager didScreenStreamAdded:(NSString *_Nullable)screenStreamsUid;

/**
 * @brief remove remote user screen stream callback
 * @param rtcManager GroupVideoCallRTCManager object
 * @param screenStreamsUid user user id
 */
- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager didScreenStreamRemoved:(NSString *_Nullable)screenStreamsUid;

/**
 * @brief User microphone volume change callback
 * @param rtcManager GroupVideoCallRTCManager object
 * @param volumeInfo volume information
 */
- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager reportAllAudioVolume:(NSDictionary<NSString *, NSNumber *> *_Nonnull)volumeInfo;

/**
 * @brief The remote user stops the audio streaming callback
 * @param rtcManager GroupVideoCallRTCManager object
 * @param uid user user id
 * @param isMute pauses the audio stream when it is true, and resumes the audio stream when it is false.
 */
- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager onUserMuteAudio:(NSString *_Nonnull)uid isMute:(BOOL)isMute;

/**
 * @brief The remote user stops the video stream callback
 * @param rtcManager GroupVideoCallRTCManager object
 * @param uid user user id
 * @param isMute pauses the video stream when it is true, and resumes the video stream when it is false.
 */
- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager onUserMuteVideo:(NSString *_Nonnull)uid isMute:(BOOL)isMute;

/**
 * @brief audio routing change callback
 * @param isHeadset is true, the current route is headphone, and when it is false, the current route is other.
 */
- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager onAudioRouteChanged:(BOOL)isHeadset isSpeakerphone:(BOOL)isSpeakerphone;

@end

@interface GroupVideoCallRTCManager : BaseRTCManager

@property (nonatomic, weak) id<GroupVideoCallRTCManagerDelegate> _Nullable delegate;

+ (GroupVideoCallRTCManager *_Nullable)shareRtc;

#pragma mark - Base Method

/**
 * @brief Join RTC room
 * @param localUserModel Current login user model
 * @param rtcToken RTC token
 */
- (void)joinRTCRoomWithModel:(GroupVideoCallRoomUserModel *)localUserModel
                    rtcToken:(NSString *)rtcToken;

/**
 * @brief Leave RTC room
 */
- (void)leaveRTCRoom;

/**
 * @brief Real-time update of video parameters
 */
- (void)updateAudioAndVideoSettings;

/**
 * @brief Switch camera
 */
- (void)switchCamera;

/**
 * @brief Switch audio routing
 * @param route ByteRTCAudioRoute object
 */
- (void)setDeviceAudioRoute:(ByteRTCAudioRoute)route;

/**
 * @brief Control the sending status of the local audio stream: send/not send
 * @param isPublish ture:Send, false：Not send
 */
- (void)publishAudioStream:(BOOL)isPublish;

/**
 * @brief Switch local video capture
 * @param isStart ture:Open video capture false：Turn off video capture
 */
- (void)switchVideoCapture:(BOOL)isStart;

/**
 * @brief render the far end camera stream
 * @param canvas ByteRTCRemoteStreamKey object
 * @param canvas ByteRTCVideoCanvas object
 */
- (void)setupRemoteVideoStreamKey:(ByteRTCRemoteStreamKey *)streamKey
                           canvas:(ByteRTCVideoCanvas *)canvas;

/**
 * @brief render local camera stream
 * @param canvas ByteRTCVideoCanvas object
 */
- (void)setupLocalVideo:(ByteRTCVideoCanvas *_Nullable)canvas;

/**
 * @brief Get the screen stream rendering UIView
 * @param uid RTC room user id
 */
- (UIView *)getScreenStreamViewWithUid:(NSString *)uid;

@end

NS_ASSUME_NONNULL_END
