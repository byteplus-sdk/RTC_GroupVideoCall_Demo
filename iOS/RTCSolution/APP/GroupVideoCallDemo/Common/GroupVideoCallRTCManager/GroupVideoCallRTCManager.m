//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import "GroupVideoCallRTCManager.h"
#import "AlertActionManager.h"
#import "GroupVideoCallMockDataComponent.h"

@interface GroupVideoCallRTCManager () <ByteRTCVideoDelegate>

// RTC / RTS room object
@property (nonatomic, strong, nullable) ByteRTCRoom *rtcRoom;
@property (nonatomic, strong) NSMutableDictionary<NSString *, GroupVideoCallRoomParamInfoModel *> *videoStatsInfoDic;
@property (nonatomic, strong) NSMutableDictionary<NSString *, GroupVideoCallRoomParamInfoModel *> *audioStatsInfoDic;
@property (nonatomic, strong) NSMutableDictionary<NSString *, UIView *> *streamViewDic;
@property (nonatomic, assign) NSInteger currnetCameraID;
@property (nonatomic, assign) ByteRTCAudioRoute audioRoute;
@property (nonatomic, assign) ByteRTCAudioRoute curAudioRoute;
@property (nonatomic, strong) UIView *localVideoView;

@end

@implementation GroupVideoCallRTCManager

+ (GroupVideoCallRTCManager *_Nullable)shareRtc {
    static GroupVideoCallRTCManager *manager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        manager = [[GroupVideoCallRTCManager alloc] init];
    });
    return manager;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        // Default front camera, speaker mode
        self.currnetCameraID = ByteRTCCameraIDFront;
        self.audioRoute = ByteRTCAudioRouteSpeakerphone;
    }
    return self;
}

#pragma mark - Publish Action

- (void)configeRTCEngine {
    [super configeRTCEngine];
}

- (void)joinRTCRoomWithModel:(GroupVideoCallRoomUserModel *)localUserModel
                    rtcToken:(NSString *)rtcToken {
    // Set the audio scene type
    [self.rtcEngineKit setAudioScenario:ByteRTCAudioScenarioCommunication];
    // Turn on/off local video capture
    [self switchVideoCapture:localUserModel.isEnableVideo];
    // Turn on/off local audio captue
    [self.rtcEngineKit startAudioCapture];
    // Set the audio routing mode, YES speaker/NO earpiece
    ByteRTCAudioRoute audioRoute = localUserModel.isSpeakers ? ByteRTCAudioRouteSpeakerphone : ByteRTCAudioRouteEarpiece;
    [self setDeviceAudioRoute:audioRoute];
    // Turn on/off speaker volume keying
    ByteRTCAudioPropertiesConfig *audioPropertiesConfig = [[ByteRTCAudioPropertiesConfig alloc] init];
    audioPropertiesConfig.interval = 1000;
    [self.rtcEngineKit enableAudioPropertiesReport:audioPropertiesConfig];
    // Join the room, start connecting the microphone, you need to apply for AppId and Token
    ByteRTCUserInfo *userInfo = [[ByteRTCUserInfo alloc] init];
    userInfo.userId = localUserModel.uid;
    NSDictionary *extraInfo = @{
        @"user_name": localUserModel.name,
        @"user_id": localUserModel.uid
    };
    userInfo.extraInfo = [extraInfo yy_modelToJSONString];
    ByteRTCRoomConfig *config = [[ByteRTCRoomConfig alloc] init];
    config.profile = ByteRTCRoomProfileCommunication;
    config.isAutoPublish = YES;
    config.isAutoSubscribeAudio = YES;
    config.isAutoSubscribeVideo = YES;
    self.rtcRoom = [self.rtcEngineKit createRTCRoom:localUserModel.roomId];
    self.rtcRoom.delegate = self;
    [self.rtcRoom joinRoom:rtcToken userInfo:userInfo roomConfig:config];
    // Turn on/off local audio capture
    [self publishAudioStream:localUserModel.isEnableAudio];
}

- (void)updateAudioAndVideoSettings {
    // Modify resolution
    GroupVideoCallMockDataComponent *mockData = [GroupVideoCallMockDataComponent shared];
    NSNumber *res = mockData.currentResolutionDic[@"value"];
    ByteRTCVideoEncoderConfig *config = [[ByteRTCVideoEncoderConfig alloc] init];
    config.width = res.CGSizeValue.width;
    config.height = res.CGSizeValue.height;
    config.frameRate = 15;
    config.maxBitrate = -1;
    config.encoderPreference = ByteRTCVideoEncoderPreferenceDisabled;
    [self.rtcEngineKit setMaxVideoEncoderConfig:config];
    // Set mirror, cancel the mirroring of the rear camera
    if (mockData.isOpenMirror && self.currnetCameraID == ByteRTCCameraIDFront) {
        [self.rtcEngineKit setLocalVideoMirrorType:ByteRTCMirrorTypeRenderAndEncoder];
    } else {
        [self.rtcEngineKit setLocalVideoMirrorType:ByteRTCMirrorTypeNone];
    }
    // Set audio profile
    NSNumber *audioProfile = mockData.currentaudioProfileDic[@"value"];
    [self.rtcEngineKit setAudioProfile:audioProfile.integerValue];
}

- (void)switchCamera {
    // Switch front/rear camera
    GroupVideoCallMockDataComponent *mockData = [GroupVideoCallMockDataComponent shared];

    ByteRTCCameraID cameraID = self.currnetCameraID;
    if (cameraID == ByteRTCCameraIDFront) {
        cameraID = ByteRTCCameraIDBack;
    } else {
        cameraID = ByteRTCCameraIDFront;
    }

    if (cameraID == ByteRTCCameraIDFront && mockData.isOpenMirror) {
        [self.rtcEngineKit setLocalVideoMirrorType:ByteRTCMirrorTypeRenderAndEncoder];
    } else {
        [self.rtcEngineKit setLocalVideoMirrorType:ByteRTCMirrorTypeNone];
    }

    [self.rtcEngineKit switchCamera:cameraID];
    self.currnetCameraID = cameraID;
}

- (void)setDeviceAudioRoute:(ByteRTCAudioRoute)route {
    // Turn on/off the loudspeaker
    if (self.audioRoute == ByteRTCAudioRouteEarpiece ||
        self.audioRoute == ByteRTCAudioRouteSpeakerphone) {
        if (self.audioRoute == route) {
            return;
        }
        self.audioRoute = route;

        [self.rtcEngineKit setDefaultAudioRoute:route];
    }
}

- (void)publishAudioStream:(BOOL)isPublish {
    // Turn on/off local audio capture
    if (isPublish) {
        [self.rtcRoom publishStream:ByteRTCMediaStreamTypeAudio];
    } else {
        [self.rtcRoom unpublishStream:ByteRTCMediaStreamTypeAudio];
    }
}

- (void)switchVideoCapture:(BOOL)isStart {
    // Turn on/off local video capture
    if (isStart) {
        [self.rtcEngineKit startVideoCapture];
        [self startPreview:self.localVideoView];
    } else {
        [self.rtcEngineKit stopVideoCapture];
        [self startPreview:nil];
    }
}

- (void)leaveRTCRoom {
    // Leave RTC room
    if (self.currnetCameraID != ByteRTCCameraIDFront) {
        self.currnetCameraID = ByteRTCCameraIDFront;
        [self.rtcEngineKit switchCamera:ByteRTCCameraIDFront];
    }
    [self.streamViewDic removeAllObjects];
    [self.rtcEngineKit stopAudioCapture];
    [self.rtcRoom leaveRoom];
    [self.rtcRoom destroy];
    self.rtcRoom = nil;
}

- (void)setupRemoteVideoStreamKey:(ByteRTCRemoteStreamKey *)streamKey
                           canvas:(ByteRTCVideoCanvas *)canvas {
    // Setup remote video stream
    if (!canvas || !streamKey) {
        return;
    }
    [self.rtcEngineKit setRemoteVideoCanvas:streamKey withCanvas:canvas];
}

- (void)setupLocalVideo:(ByteRTCVideoCanvas *)canvas {
    // Setup local video stream
    self.localVideoView = canvas.view;
    [self.rtcEngineKit setLocalVideoCanvas:ByteRTCStreamIndexMain withCanvas:canvas];
}

- (void)bingScreenCanvasViewToUid:(NSString *)uid {
    dispatch_queue_async_safe(dispatch_get_main_queue(), (^{
                                  UIView *remoteRoomView = [self getScreenStreamViewWithUid:uid];
                                  if (!remoteRoomView) {
                                      remoteRoomView = [[UIView alloc] init];
                                      remoteRoomView.hidden = YES;
                                      ByteRTCVideoCanvas *canvas = [[ByteRTCVideoCanvas alloc] init];
                                      canvas.renderMode = ByteRTCRenderModeFit;
                                      canvas.view.backgroundColor = [UIColor clearColor];
                                      canvas.view = remoteRoomView;

                                      ByteRTCRemoteStreamKey *streamKey = [[ByteRTCRemoteStreamKey alloc] init];
                                      streamKey.userId = uid;
                                      streamKey.roomId = self.rtcRoom.getRoomId;
                                      streamKey.streamIndex = ByteRTCStreamIndexScreen;

                                      [self.rtcEngineKit setRemoteVideoCanvas:streamKey
                                                                   withCanvas:canvas];

                                      NSString *groupKey = [NSString stringWithFormat:@"screen_%@", uid];
                                      [self.streamViewDic setValue:remoteRoomView forKey:groupKey];
                                  }
                              }));
}

- (UIView *)getScreenStreamViewWithUid:(NSString *)uid {
    if (IsEmptyStr(uid)) {
        return nil;
    }
    NSString *typeStr = @"screen";
    NSString *key = [NSString stringWithFormat:@"%@_%@", typeStr, uid];
    UIView *view = self.streamViewDic[key];
    return view;
}

#pragma mark - ByteRTCRoomDelegate

- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onRoomStateChanged:(NSString *)roomId
               withUid:(NSString *)uid
                 state:(NSInteger)state
             extraInfo:(NSString *)extraInfo {
    [super rtcRoom:rtcRoom onRoomStateChanged:roomId withUid:uid state:state extraInfo:extraInfo];

    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        RTCJoinModel *joinModel = [RTCJoinModel modelArrayWithClass:extraInfo state:state roomId:roomId];
        if ([self.delegate respondsToSelector:@selector(groupVideoCallRTCManager:onRoomStateChanged:)]) {
            [self.delegate groupVideoCallRTCManager:self onRoomStateChanged:joinModel];
        }
    });
}

- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onUserJoined:(ByteRTCUserInfo *)userInfo elapsed:(NSInteger)elapsed {
    NSString *name = userInfo.userId;
    if (userInfo.extraInfo) {
        NSDictionary *extraInfo =
            [NSJSONSerialization JSONObjectWithData:[userInfo.extraInfo dataUsingEncoding:NSUTF8StringEncoding]
                                            options:NSJSONReadingMutableContainers
                                              error:nil];
        if (extraInfo) {
            id value = extraInfo[@"user_name"];
            if ([value isKindOfClass:[NSString class]]) {
                name = value;
            }
        }
    }

    if ([self.delegate respondsToSelector:@selector(rtcManager:onUserJoined:userName:)]) {
        [self.delegate rtcManager:self onUserJoined:userInfo.userId userName:name];
    }
}
- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onUserLeave:(NSString *)uid reason:(ByteRTCUserOfflineReason)reason {
    if ([self.delegate respondsToSelector:@selector(rtcManager:onUserLeaved:)]) {
        [self.delegate rtcManager:self onUserLeaved:uid];
    }
}

- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onUserPublishScreen:(NSString *)userId type:(ByteRTCMediaStreamType)type {
    [self bingScreenCanvasViewToUid:userId];
    if ([self.delegate respondsToSelector:@selector(rtcManager:didScreenStreamAdded:)]) {
        [self.delegate rtcManager:self didScreenStreamAdded:userId];
    }
}

- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onUserUnpublishScreen:(NSString *)userId type:(ByteRTCMediaStreamType)type reason:(ByteRTCStreamRemoveReason)reason {
    if ([self.delegate respondsToSelector:@selector(rtcManager:didScreenStreamRemoved:)]) {
        [self.delegate rtcManager:self didScreenStreamRemoved:userId];
    }
}

- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onLocalStreamStats:(ByteRTCLocalStreamStats *)stats {
    GroupVideoCallRoomParamInfoModel *videoStatsInfo = [GroupVideoCallRoomParamInfoModel new];
    videoStatsInfo.uid = [LocalUserComponent userModel].uid;
    videoStatsInfo.width = stats.videoStats.encodedFrameWidth;
    videoStatsInfo.height = stats.videoStats.encodedFrameHeight;
    videoStatsInfo.bitRate = stats.videoStats.sentKBitrate;
    videoStatsInfo.fps = stats.videoStats.sentFrameRate;
    videoStatsInfo.delay = stats.videoStats.rtt;
    videoStatsInfo.lost = stats.videoStats.videoLossRate;

    GroupVideoCallRoomParamInfoModel *audioStatsInfo = [GroupVideoCallRoomParamInfoModel new];
    audioStatsInfo.uid = [LocalUserComponent userModel].uid;
    audioStatsInfo.bitRate = stats.audioStats.sentKBitrate;
    audioStatsInfo.delay = stats.audioStats.rtt;
    audioStatsInfo.lost = stats.audioStats.audioLossRate;

    ByteRTCNetworkQuality quality = MAX(stats.txQuality, stats.rxQuality);
    GroupVideoCallRoomParamNetQuality netQuality;
    switch (quality) {
        case ByteRTCNetworkQualityUnknown:
            netQuality = GroupVideoCallRoomParamNetQualityNormal;
            break;
        case ByteRTCNetworkQualityExcellent:
        case ByteRTCNetworkQualityGood:
            netQuality = GroupVideoCallRoomParamNetQualityGood;
            break;
        case ByteRTCNetworkQualityPoor:
            netQuality = GroupVideoCallRoomParamNetQualityNormal;
            break;
        case ByteRTCNetworkQualityBad:
        case ByteRTCNetworkQualityVeryBad:
            netQuality = GroupVideoCallRoomParamNetQualityBad;
            break;
        default:
            netQuality = GroupVideoCallRoomParamNetQualityNormal;
            break;
    }
    videoStatsInfo.netQuality = netQuality;
    audioStatsInfo.netQuality = netQuality;

    [self updateGroupVideoCallRoomVideoStatsInfo:videoStatsInfo audioStatsInfo:audioStatsInfo];
}

- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onRemoteStreamStats:(ByteRTCRemoteStreamStats *)stats {
    GroupVideoCallRoomParamInfoModel *videoStatsInfo = [GroupVideoCallRoomParamInfoModel new];
    videoStatsInfo.uid = stats.uid;
    videoStatsInfo.width = stats.videoStats.width;
    videoStatsInfo.height = stats.videoStats.height;
    videoStatsInfo.bitRate = stats.videoStats.receivedKBitrate;
    videoStatsInfo.fps = stats.videoStats.receivedFrameRate;
    videoStatsInfo.delay = stats.videoStats.rtt;
    videoStatsInfo.lost = stats.videoStats.videoLossRate;

    GroupVideoCallRoomParamInfoModel *audioStatsInfo = [GroupVideoCallRoomParamInfoModel new];
    audioStatsInfo.uid = stats.uid;
    audioStatsInfo.bitRate = stats.audioStats.receivedKBitrate;
    audioStatsInfo.delay = stats.audioStats.rtt;
    audioStatsInfo.lost = stats.audioStats.audioLossRate;

    ByteRTCNetworkQuality quality = MAX(stats.txQuality, stats.txQuality);
    GroupVideoCallRoomParamNetQuality netQuality;
    switch (quality) {
        case ByteRTCNetworkQualityUnknown:
            netQuality = GroupVideoCallRoomParamNetQualityNormal;
            break;
        case ByteRTCNetworkQualityExcellent:
        case ByteRTCNetworkQualityGood:
            netQuality = GroupVideoCallRoomParamNetQualityGood;
            break;
        case ByteRTCNetworkQualityPoor:
            netQuality = GroupVideoCallRoomParamNetQualityNormal;
            break;
        case ByteRTCNetworkQualityBad:
        case ByteRTCNetworkQualityVeryBad:
            netQuality = GroupVideoCallRoomParamNetQualityBad;
            break;
        default:
            netQuality = GroupVideoCallRoomParamNetQualityNormal;
            break;
    }
    videoStatsInfo.netQuality = netQuality;
    audioStatsInfo.netQuality = netQuality;

    [self updateGroupVideoCallRoomVideoStatsInfo:videoStatsInfo audioStatsInfo:audioStatsInfo];
}

#pragma mark - ByteRTCVideoDelegate

- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onUserPublishStream:(NSString *)userId type:(ByteRTCMediaStreamType)type {
    if (type == ByteRTCMediaStreamTypeAudio ||
        type == ByteRTCMediaStreamTypeBoth) {
        // Determine the remote user to open and close the microphone
        if ([self.delegate respondsToSelector:@selector(rtcManager:onUserMuteAudio:isMute:)]) {
            [self.delegate rtcManager:self onUserMuteAudio:userId
                               isMute:NO];
        }
    }
}

- (void)rtcRoom:(ByteRTCRoom *)rtcRoom onUserUnpublishStream:(NSString *)userId type:(ByteRTCMediaStreamType)type reason:(ByteRTCStreamRemoveReason)reason {
    if (type == ByteRTCMediaStreamTypeAudio ||
        type == ByteRTCMediaStreamTypeBoth) {
        // Determine the remote user to open and close the microphone
        if ([self.delegate respondsToSelector:@selector(rtcManager:onUserMuteAudio:isMute:)]) {
            [self.delegate rtcManager:self onUserMuteAudio:userId
                               isMute:YES];
        }
    }
}

- (void)rtcEngine:(ByteRTCVideo *)engine onUserStartVideoCapture:(NSString *)roomId uid:(NSString *)uid {
    // Determine the remote user to start the camera
    if ([self.delegate respondsToSelector:@selector(rtcManager:onUserMuteVideo:isMute:)]) {
        [self.delegate rtcManager:self onUserMuteVideo:uid isMute:NO];
    }
}

- (void)rtcEngine:(ByteRTCVideo *)engine onUserStopVideoCapture:(NSString *)roomId uid:(NSString *)uid {
    // Determine the remote user to stop the camera
    if ([self.delegate respondsToSelector:@selector(rtcManager:onUserMuteVideo:isMute:)]) {
        [self.delegate rtcManager:self onUserMuteVideo:uid isMute:YES];
    }
}

- (void)rtcEngine:(ByteRTCVideo *)engine onAudioRouteChanged:(ByteRTCAudioRoute)device {
    NSInteger isHeadset = -1;
    if (device == ByteRTCAudioRouteDefault ||
        device == ByteRTCAudioRouteHeadset ||
        device == ByteRTCAudioRouteHeadsetBluetooth ||
        device == ByteRTCAudioRouteHeadsetUSB) {
        // Plug in headphones
        isHeadset = 1;
        self.curAudioRoute = self.audioRoute;
        self.audioRoute = device;
    } else {
        if (self.audioRoute == ByteRTCAudioRouteDefault ||
            self.audioRoute == ByteRTCAudioRouteHeadset ||
            self.audioRoute == ByteRTCAudioRouteHeadsetBluetooth ||
            self.audioRoute == ByteRTCAudioRouteHeadsetUSB) {
            // Unplug the headset
            isHeadset = 2;
            // When the headset is pulled out, the SDK will automatically switch to the speaker mode
            self.audioRoute = self.curAudioRoute;
        } else {
            // Normal settings for receiver/loud speaker
        }
    }
    if (isHeadset > 0) {
        dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
            if ([self.delegate respondsToSelector:@selector(rtcManager:onAudioRouteChanged:isSpeakerphone:)]) {
                BOOL isHeadsetBool = (isHeadset == 1) ? YES : NO;
                BOOL isSpeakerphone = (self.audioRoute == ByteRTCAudioRouteSpeakerphone) ? YES : NO;
                [self.delegate rtcManager:self onAudioRouteChanged:isHeadsetBool isSpeakerphone:isSpeakerphone];
            }
        });
    }
}

- (void)rtcEngine:(ByteRTCVideo *)engine reportSysStats:(const ByteRTCSysStats *)stats {
}

- (void)rtcEngine:(ByteRTCVideo *_Nonnull)engine onLocalAudioPropertiesReport:(NSArray<ByteRTCLocalAudioPropertiesInfo *> *_Nonnull)audioPropertiesInfos {
    NSInteger minVolume = 10;
    NSMutableDictionary *parDic = [[NSMutableDictionary alloc] init];
    for (int i = 0; i < audioPropertiesInfos.count; i++) {
        ByteRTCLocalAudioPropertiesInfo *model = audioPropertiesInfos[i];
        if (model.audioPropertiesInfo.linearVolume > minVolume) {
            [parDic setValue:@(model.audioPropertiesInfo.linearVolume) forKey:[LocalUserComponent userModel].uid];
        }
    }

    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        if ([self.delegate respondsToSelector:@selector(rtcManager:reportAllAudioVolume:)]) {
            [self.delegate rtcManager:self reportAllAudioVolume:[parDic copy]];
        }
    });
}

- (void)rtcEngine:(ByteRTCVideo *)engine onRemoteAudioPropertiesReport:(NSArray<ByteRTCRemoteAudioPropertiesInfo *> *)audioPropertiesInfos totalRemoteVolume:(NSInteger)totalRemoteVolume {
    NSInteger minVolume = 10;
    NSMutableDictionary *parDic = [[NSMutableDictionary alloc] init];
    for (int i = 0; i < audioPropertiesInfos.count; i++) {
        ByteRTCRemoteAudioPropertiesInfo *model = audioPropertiesInfos[i];
        if (model.audioPropertiesInfo.linearVolume > minVolume) {
            [parDic setValue:@(model.audioPropertiesInfo.linearVolume) forKey:model.streamKey.userId];
        }
    }

    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        if ([self.delegate respondsToSelector:@selector(rtcManager:reportAllAudioVolume:)]) {
            [self.delegate rtcManager:self reportAllAudioVolume:[parDic copy]];
        }
    });
}

#pragma mark - Private Action

- (void)updateGroupVideoCallRoomVideoStatsInfo:(GroupVideoCallRoomParamInfoModel *)videoStatsInfo audioStatsInfo:(GroupVideoCallRoomParamInfoModel *)audioStatsInfo {
    [self.videoStatsInfoDic setObject:videoStatsInfo forKey:videoStatsInfo.uid];
    if ([self.delegate respondsToSelector:@selector(rtcManager:didUpdateVideoStatsInfo:)]) {
        [self.delegate rtcManager:self didUpdateVideoStatsInfo:self.videoStatsInfoDic];
    }

    [self.audioStatsInfoDic setObject:audioStatsInfo forKey:audioStatsInfo.uid];
    if ([self.delegate respondsToSelector:@selector(rtcManager:didUpdateAudioStatsInfo:)]) {
        [self.delegate rtcManager:self didUpdateAudioStatsInfo:self.audioStatsInfoDic];
    }
}

- (void)startPreview:(UIView *)view {
    if (view) {
        self.localVideoView = view;
    }
    ByteRTCVideoCanvas *canvas = [[ByteRTCVideoCanvas alloc] init];
    canvas.view = view;
    canvas.renderMode = ByteRTCRenderModeHidden;
    canvas.view.backgroundColor = [UIColor clearColor];
    // Set local video display information
    [self.rtcEngineKit setLocalVideoCanvas:ByteRTCStreamIndexMain withCanvas:canvas];
}

#pragma mark - Getter

- (NSMutableDictionary<NSString *, UIView *> *)streamViewDic {
    if (!_streamViewDic) {
        _streamViewDic = [[NSMutableDictionary alloc] init];
    }
    return _streamViewDic;
}

- (NSMutableDictionary<NSString *, GroupVideoCallRoomParamInfoModel *> *)videoStatsInfoDic {
    if (!_videoStatsInfoDic) {
        _videoStatsInfoDic = [NSMutableDictionary dictionary];
    }
    return _videoStatsInfoDic;
}

- (NSMutableDictionary<NSString *, GroupVideoCallRoomParamInfoModel *> *)audioStatsInfoDic {
    if (!_audioStatsInfoDic) {
        _audioStatsInfoDic = [NSMutableDictionary dictionary];
    }
    return _audioStatsInfoDic;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end
