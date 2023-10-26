// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import "GroupVideoCallRoomViewController.h"
#import "GroupVideoCallSettingViewController.h"
#import "GroupVideoCallRoomNavView.h"
#import "GroupVideoCallRoomBottomView.h"
#import "GroupVideoCallRoomViewController+Listener.h"
#import "UIViewController+Orientation.h"
#import "GroupVideoCallRTCManager.h"
#import "GroupVideoCallRTSManager.h"
#import "GroupVideoCallAvatarPageView.h"
#import "GroupVideoCallAvatarView.h"
#import "GroupVideoCallFullScreenView.h"
#import "GroupVideoCallStatsView.h"

@interface GroupVideoCallRoomViewController () <UINavigationControllerDelegate, GroupVideoCallRTCManagerDelegate, GroupVideoCallRoomNavViewDelegate, GroupVideoCallRoomBottomViewDelegate, GroupVideoCallAvatarPageViewDelegate>

@property (nonatomic, strong) GroupVideoCallRoomNavView *navView;
@property (nonatomic, strong) GroupVideoCallRoomBottomView *bottomView;
@property (nonatomic, strong) GroupVideoCallAvatarPageView *roomMainView;
@property (nonatomic, strong) GroupVideoCallFullScreenView *fullScreenView;
@property (nonatomic, strong) GroupVideoCallStatsView *statsView;
@property (nonatomic, strong) GroupVideoCallRoomUserModel *localUserModel;
@property (nonatomic, strong) NSMutableArray <GroupVideoCallRoomUserModel *>*userArray;
@property (nonatomic, assign) NSInteger currentFullScreenUserIndex;
@property (nonatomic, assign) NSInteger duration;

@end

@implementation GroupVideoCallRoomViewController

- (instancetype)initWithVideoSession:(GroupVideoCallRoomUserModel *)loginModel
                            rtcToken:(NSString *)rtcToken
                            duration:(NSInteger)duration {
    self = [super init];
    if (self) {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationBecomeActive) name:UIApplicationWillEnterForegroundNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationEnterBackground) name: UIApplicationDidEnterBackgroundNotification object:nil];
        [UIApplication sharedApplication].idleTimerDisabled = YES;
        self.localUserModel = loginModel;
        self.duration = duration;
        self.currentFullScreenUserIndex = -1;
        
        [GroupVideoCallRTCManager shareRtc].delegate = self;
        [[GroupVideoCallRTCManager shareRtc] joinRTCRoomWithModel:self.localUserModel
                                                    rtcToken:rtcToken];
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor colorFromHexString:@"#1D2129"];
    // Add device direction monitor
    [self addOrientationNotice];
    // Add RTS listener
    [self addRTSListener];
    // Create UI layout
    [self createUIComponent];
    [self updateNavTime:self.duration];
    [self startLocalCameraPreview];
    
    __weak __typeof(self) wself = self;
    self.fullScreenView.clickOrientationBlock = ^(BOOL isLandscape) {
        if (!isLandscape) {
            [wself setDeviceInterfaceOrientation:UIDeviceOrientationLandscapeLeft];
        } else {
            [wself setDeviceInterfaceOrientation:UIDeviceOrientationPortrait];
        }
    };
    [self.bottomView updateButtonStatus:RoomBottomStatusMic close:!self.localUserModel.isEnableAudio];
    [self.bottomView updateButtonStatus:RoomBottomStatusCamera close:!self.localUserModel.isEnableVideo];
    [self.bottomView updateButtonStatus:RoomBottomStatusAudio close:!self.localUserModel.isSpeakers];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    [self.navigationController setNavigationBarHidden:YES animated:NO];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
    
    [self setAllowAutoRotate:ScreenOrientationPortrait];
}

#pragma mark - NSNotification

- (void)applicationBecomeActive {
    /**
     * @brief When the APP returns to the active state, if the camera is turned on, the camera acquisition needs to be resumed.
     */
    if (self.localUserModel.isEnableVideo) {
        [[GroupVideoCallRTCManager shareRtc] switchVideoCapture:YES];
    }
}

- (void)applicationEnterBackground {
    [[GroupVideoCallRTCManager shareRtc] switchVideoCapture:NO];
}

#pragma mark - UIViewController+Orientation

- (void)orientationDidChang:(BOOL)isLandscape {
    CGFloat navHeight = 44 + [DeviceInforTool getStatusBarHight];
    CGFloat bottomBottom = 0;
    if (isLandscape) {
        navHeight = 0;
        bottomBottom = (128/2 + [DeviceInforTool getVirtualHomeHeight] * 2);
    }
    
    [self.navView mas_updateConstraints:^(MASConstraintMaker *make) {
        make.height.mas_equalTo(navHeight);
    }];
    [self.bottomView mas_updateConstraints:^(MASConstraintMaker *make) {
        make.bottom.equalTo(self.view).offset(bottomBottom);
    }];
}

#pragma mark - GroupVideoCallRTCManagerDelegate

- (void)groupVideoCallRTCManager:(GroupVideoCallRTCManager *)manager onRoomStateChanged:(RTCJoinModel *)joinModel {
    if (joinModel.joinType != 0 && joinModel.errorCode == 0) {
        [self groupVideoCallReconnect];
    }
}

- (void)rtcManager:(GroupVideoCallRTCManager * _Nonnull)rtcManager onUserJoined:(NSString *_Nullable)uid userName:(NSString *_Nullable)name {
    // The remote user joins the room
    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        [self addRemoteUserViewWithUid:uid name:name];
    });
}

- (void)rtcManager:(GroupVideoCallRTCManager * _Nonnull)rtcManager onUserLeaved:(NSString *_Nullable)uid {
    // The remote user leaves the room
    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        [self removeRemoteUserViewWithUid:uid isOnlyRemoveScreenShare:NO];
    });
}

- (void)rtcManager:(GroupVideoCallRTCManager * _Nonnull)rtcManager didScreenStreamAdded:(NSString *_Nullable)screenStreamsUid {
    // Callback on new screen sharing media streams from remote users in the room.
    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        [self addRemoteScreenViewWithUid:screenStreamsUid];
    });
}

- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager didScreenStreamRemoved:(NSString *)screenStreamsUid {
    // Callback on removal of screen sharing media streams from remote users in the room.
    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        [self removeRemoteUserViewWithUid:screenStreamsUid
                  isOnlyRemoveScreenShare:YES];
    });
}

- (void)rtcManager:(GroupVideoCallRTCManager *_Nullable)rtcManager didUpdateVideoStatsInfo:(NSDictionary <NSString *, GroupVideoCallRoomParamInfoModel *>*_Nullable)statsInfo {
    // Received video stream information change
    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        NSMutableArray *videoStatsInfoArray = [NSMutableArray array];
        for (GroupVideoCallRoomUserModel *userModel in self.userArray) {
            if (userModel.isScreen) {
                continue;
            }
            
            GroupVideoCallRoomParamInfoModel *videoStatsInfo = [statsInfo objectForKey:userModel.uid];
            if (!videoStatsInfo) {
                continue;
            }
            
            videoStatsInfo.name = userModel.name;
            [videoStatsInfoArray addObject:videoStatsInfo];
        }
        
        [self.statsView setVideoStats:videoStatsInfoArray];
    });
}

- (void)rtcManager:(GroupVideoCallRTCManager *_Nullable)rtcManager didUpdateAudioStatsInfo:(NSDictionary <NSString *, GroupVideoCallRoomParamInfoModel *>*_Nullable)statsInfo {
    // Received audio stream information change
    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        NSMutableArray *audioStatsInfoArray = [NSMutableArray array];
        for (GroupVideoCallRoomUserModel *userModel in self.userArray) {
            if (userModel.isScreen) {
                continue;
            }
            
            GroupVideoCallRoomParamInfoModel *audioStatsInfo = [statsInfo objectForKey:userModel.uid];
            if (!audioStatsInfo) {
                continue;
            }
            
            audioStatsInfo.name = userModel.name;
            [audioStatsInfoArray addObject:audioStatsInfo];
        }
        
        [self.statsView setAudioStats:audioStatsInfoArray];
    });
}

- (void)rtcManager:(GroupVideoCallRTCManager *)rtcManager reportAllAudioVolume:(NSDictionary<NSString *, NSNumber *> *)volumeInfo {
    // Receive volume change
    for (GroupVideoCallRoomUserModel *userModel in self.userArray) {
        NSUInteger index = [self.userArray indexOfObject:userModel];
        GroupVideoCallAvatarView *avatarView = (GroupVideoCallAvatarView *)[self.roomMainView avatarViewAtIndex:index];
        
        GroupVideoCallAvatarViewMicStatus micStatus = GroupVideoCallAvatarViewMicStatusOff;
        
        if (userModel.isEnableAudio) {
            NSString *uid = userModel.uid;
            NSNumber *value = [volumeInfo objectForKey:uid];
            if (value.integerValue > 0) {
                micStatus = GroupVideoCallAvatarViewMicStatusSpeaking;
            } else {
                micStatus = GroupVideoCallAvatarViewMicStatusOn;
            }
        } else {
            micStatus = GroupVideoCallAvatarViewMicStatusOff;
        }
        [avatarView setMicStatus:micStatus];
    }
}

- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager onUserMuteAudio:(NSString * _Nonnull)uid isMute:(BOOL)isMute {
    // Callback received when the remote user closes the audio stream
    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        [self setMuteAudioWithUid:uid ismute:isMute];
    });
}

- (void)rtcManager:(GroupVideoCallRTCManager *_Nonnull)rtcManager onUserMuteVideo:(NSString * _Nonnull)uid isMute:(BOOL)isMute {
    // Callback received when the remote user closes the video stream
    dispatch_queue_async_safe(dispatch_get_main_queue(), ^{
        [self setMuteVideoWithUid:uid ismute:isMute];
    });
}

- (void)rtcManager:(GroupVideoCallRTCManager *)rtcManager onAudioRouteChanged:(BOOL)isHeadset {
    // Received remote user audio settings change
    if (isHeadset) {
        [self.bottomView updateButtonStatus:RoomBottomStatusAudio enable:NO];
    } else {
        [self.bottomView updateButtonStatus:RoomBottomStatusAudio enable:YES];
        [self.bottomView updateButtonStatus:RoomBottomStatusAudio close:NO];
    }
}

#pragma mark - GroupVideoCallRoomBottomViewDelegate

- (void)GroupVideoCallRoomBottomView:(GroupVideoCallRoomBottomView *)GroupVideoCallRoomBottomView itemButton:(GroupVideoCallRoomItemButton *)itemButton didSelectStatus:(RoomBottomStatus)status {
    if (status == RoomBottomStatusMic) {
        [self clickRoomBottomStatusMic:itemButton];
    } else if (status == RoomBottomStatusCamera) {
        [self clickRoomBottomStatusCamera:itemButton];
    } else if (status == RoomBottomStatusAudio) {
        BOOL isSpeakers = (itemButton.status == ButtonStatusActive);
        if (isSpeakers) {
            itemButton.status = ButtonStatusNone;
            itemButton.desTitle = LocalizedString(@"speaker");
        } else {
            itemButton.status = ButtonStatusActive;
            itemButton.desTitle = LocalizedString(@"earpiece");
        }
        ByteRTCAudioRoute route = isSpeakers ? ByteRTCAudioRouteSpeakerphone : ByteRTCAudioRouteEarpiece;
        [[GroupVideoCallRTCManager shareRtc] setDeviceAudioRoute:route];
        self.localUserModel.isSpeakers = isSpeakers;
    } else if (status == RoomBottomStatusParameter) {
        [self.statsView showStatsView];
    } else if (status == RoomBottomStatusSet) {
        GroupVideoCallSettingViewController *settingsVC = [[GroupVideoCallSettingViewController alloc] init];
        [self.navigationController pushViewController:settingsVC animated:YES];
    } else {
        
    }
}

#pragma mark - GroupVideoCallRoomNavViewDelegate

- (void)GroupVideoCallRoomNavView:(GroupVideoCallRoomNavView *)GroupVideoCallRoomNavView didSelectStatus:(RoomNavStatus)status {
    if (status == RoomNavStatusHangeup) {
        [self showEndView];
    } else if (status == RoomNavStatusSwitchCamera) {
        if (self.localUserModel.isEnableVideo) {
            [[GroupVideoCallRTCManager shareRtc] switchCamera];
        }
    }
}

#pragma mark - GroupVideoCallAvatarPageViewDelegate

- (void)onShowAvatarView:(UIView *)avatarView index:(NSUInteger)index {
    
}

- (void)onHideAvatarView:(UIView *)avatarView index:(NSUInteger)index {
    
}

- (void)onClickAvatarView:(UIView *)avatarView index:(NSUInteger)index {
    if (self.userArray.count <= 2) {
        /**
         * @brief In 1v1, click the small window to become the main window
         */
        if (index != self.roomMainView.mainViewIndex) {
            [self.roomMainView bringViewToMainAvatarViewOfIndex:index];
        }
    } else {
        GroupVideoCallRoomUserModel *userModel = self.userArray[index];
        if (!userModel.isScreen) {
            return;
        }
        
        [self showFullScreenViewWithIndex:index];
    }
}

- (void)onScrollToPageIndex:(NSUInteger)pageIndex {
    
}

#pragma mark - Network request Action

- (void)groupVideoCallReconnect {
    __weak __typeof(self) wself = self;
    [GroupVideoCallRTSManager reconnectWithBlock:^(NSString * _Nonnull RTCToken, RTSACKModel * _Nonnull model) {
        if (model.code == RTSStatusCodeUserIsInactive ||
                   model.code == RTSStatusCodeRoomDisbanded ||
                   model.code == RTSStatusCodeUserNotFound) {
            [wself hangUp];
            [[ToastComponent shareToastComponent] showWithMessage:model.message];
        } else {

        }
    }];
}

#pragma mark - Publish Action

- (void)hangUp {
    /**
     * @brief leave business room
     */
    [GroupVideoCallRTSManager leaveRoom];
    
    /**
     * @brief leave the RTC room
     */
    [[GroupVideoCallRTCManager shareRtc] leaveRTCRoom];
    [self dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark - Private Action

- (void)setMuteVideoWithUid:(NSString *)uid ismute:(BOOL)isMute {
    for (GroupVideoCallRoomUserModel *userModel in self.userArray) {
        if ([userModel.uid isEqualToString:uid] && !userModel.isScreen) {
            NSUInteger index = [self.userArray indexOfObject:userModel];
        
            GroupVideoCallAvatarView *avatarView = (GroupVideoCallAvatarView *)[self.roomMainView avatarViewAtIndex:index];
            [avatarView setVideoStatus:isMute ? GroupVideoCallAvatarViewVideoStatusOff : GroupVideoCallAvatarViewVideoStatusOn];
        }
    }
}

- (void)setMuteAudioWithUid:(NSString *)uid ismute:(BOOL)isMute {
    for (GroupVideoCallRoomUserModel *userModel in self.userArray) {
        if ([userModel.uid isEqualToString:uid]) {
            NSUInteger index = [self.userArray indexOfObject:userModel];
            userModel.isEnableAudio = !isMute;
            GroupVideoCallAvatarView *avatarView = (GroupVideoCallAvatarView *)[self.roomMainView avatarViewAtIndex:index];
            [avatarView setMicStatus:isMute ? GroupVideoCallAvatarViewMicStatusOff : GroupVideoCallAvatarViewMicStatusOn];
        }
    }
}

- (void)showFullScreenViewWithIndex:(NSUInteger)index {
    if (index >= self.userArray.count) {
        return;
    }
    
    self.currentFullScreenUserIndex = index;
    
    GroupVideoCallRoomUserModel *userModel = self.userArray[index];
    NSString *uid = userModel.uid;
    
    [self setAllowAutoRotate:ScreenOrientationLandscapeAndPortrait];
    __weak __typeof(self) wself = self;
    [self.fullScreenView show:uid
                     userName:userModel.name
                       roomId:self.localUserModel.roomId
                        block:^(BOOL isRemove) {
        if (userModel.isScreen) {
            if (!isRemove) {
                /**
                 * @brief If you remove the screen flow, you don't need to add it again. If it is a manual click, you need to re-add the screen flow.
                 */
                [wself addRemoteScreenViewWithUid:uid];
            }
        } else {
            [wself restoreUserStreamDisplay];
        }
        [wself setAllowAutoRotate:ScreenOrientationPortrait];
        wself.currentFullScreenUserIndex = -1;
    }];
}

- (void)restoreUserStreamDisplay {
    UIView *smallView = [self.roomMainView avatarViewAtIndex:self.currentFullScreenUserIndex];
    if (!smallView) {
        return;
    }
    
    GroupVideoCallRoomUserModel *userModel = self.userArray[self.currentFullScreenUserIndex];
    
    ByteRTCVideoCanvas *videoCanvas = [ByteRTCVideoCanvas new];
    videoCanvas.view = smallView;
    videoCanvas.renderMode = ByteRTCRenderModeHidden;
    
    ByteRTCRemoteStreamKey *streamKey = [[ByteRTCRemoteStreamKey alloc] init];
    streamKey.userId = userModel.uid;
    streamKey.roomId = self.localUserModel.roomId;
    streamKey.streamIndex = ByteRTCStreamIndexMain;
    
    [[GroupVideoCallRTCManager shareRtc] setupRemoteVideoStreamKey:streamKey canvas:videoCanvas];
}

- (void)updateNavTime:(NSInteger)time {
    if (time < 0) {
        time = 0;
    }
    self.navView.meetingTime = time;
    self.navView.localVideoSession = self.localUserModel;
}

- (void)createUIComponent {
    [self.view addSubview:self.navView];
    [self.view addSubview:self.bottomView];
    
    CGFloat navHeight = 44 + [DeviceInforTool getStatusBarHight];
    CGFloat bottomHeight = 128/2 + [DeviceInforTool getVirtualHomeHeight];
    
    [self.navView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.left.right.equalTo(self.view);
        make.height.mas_equalTo(navHeight);
    }];
    
    [self.bottomView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.equalTo(self.view);
        make.bottom.equalTo(self.view).offset(0);
        make.height.mas_equalTo(bottomHeight);
    }];
    
    self.roomMainView = [[GroupVideoCallAvatarPageView alloc] initWithFrame:CGRectMake(0, navHeight, self.view.frame.size.width, self.view.frame.size.height - navHeight - bottomHeight)];
    self.roomMainView.avatarPageViewDelegate = self;
    [self.view addSubview:self.roomMainView];
    
    [self.view addSubview:self.fullScreenView];
    [self.fullScreenView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.equalTo(self.view);
        make.top.equalTo(self.navView.mas_bottom);
        make.bottom.equalTo(self.bottomView.mas_top);
    }];
    
    [self.view addSubview:self.statsView];
    [self.statsView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(self.view);
    }];
}

- (void)showEndView {
    AlertActionModel *alertCancelModel = [[AlertActionModel alloc] init];
    alertCancelModel.title = LocalizedString(@"ok");
    __weak typeof(self) weakSelf = self;
    alertCancelModel.alertModelClickBlock = ^(UIAlertAction * _Nonnull action) {
        [weakSelf hangUp];
    };
    AlertActionModel *alertModel = [[AlertActionModel alloc] init];
    alertModel.title = LocalizedString(@"cancel");
    [[AlertActionManager shareAlertActionManager] showWithMessage:LocalizedString(@"exit_room") actions:@[alertModel, alertCancelModel]];
}

- (void)clickRoomBottomStatusMic:(GroupVideoCallRoomItemButton *)itemButton {
    [SystemAuthority authorizationStatusWithType:AuthorizationTypeAudio block:^(BOOL isAuthorize) {
        if (isAuthorize) {
            BOOL isEnableAudio = (itemButton.status == ButtonStatusActive) ? YES : NO;
            itemButton.status = isEnableAudio ? ButtonStatusNone : ButtonStatusActive;
            self.localUserModel.isEnableAudio = isEnableAudio;
            [[GroupVideoCallRTCManager shareRtc] publishAudioStream:isEnableAudio];
            [SystemAuthority autoJumpWithAuthorizationStatusWithType:AuthorizationTypeAudio];
            
            [self setMuteAudioWithUid:[LocalUserComponent userModel].uid ismute:!self.localUserModel.isEnableAudio];
        } else {
            [[ToastComponent shareToastComponent] showWithMessage:LocalizedString(@"microphone_permission_disabled")];
        }
    }];
}

- (void)clickRoomBottomStatusCamera:(GroupVideoCallRoomItemButton *)itemButton {
    [SystemAuthority authorizationStatusWithType:AuthorizationTypeCamera block:^(BOOL isAuthorize) {
        if (isAuthorize) {
            itemButton.status = (itemButton.status == ButtonStatusActive) ? ButtonStatusNone : ButtonStatusActive;
            self.localUserModel.isEnableVideo = !self.localUserModel.isEnableVideo;
            [[GroupVideoCallRTCManager shareRtc] switchVideoCapture:self.localUserModel.isEnableVideo];
            [SystemAuthority autoJumpWithAuthorizationStatusWithType:AuthorizationTypeCamera];
            
            [self setMuteVideoWithUid:[LocalUserComponent userModel].uid ismute:!self.localUserModel.isEnableVideo];
        } else {
            [[ToastComponent shareToastComponent] showWithMessage:LocalizedString(@"camera_permission_disabled")];
        }
    }];
}

- (void)startLocalCameraPreview {
    NSString *name = [NSString stringWithFormat:LocalizedString(@"%@ (me)"), [LocalUserComponent userModel].name];
    [self.userArray addObject:self.localUserModel];
    
    GroupVideoCallAvatarView *avatarView = [GroupVideoCallAvatarView new];
    [avatarView setName:name];
    [self.roomMainView addAvatarView:avatarView];
    
    ByteRTCVideoCanvas *canvas = [[ByteRTCVideoCanvas alloc] init];
    canvas.view = avatarView.videoContainerView;
    canvas.renderMode = ByteRTCRenderModeHidden;
    canvas.view.backgroundColor = [UIColor clearColor];
    [[GroupVideoCallRTCManager shareRtc] setupLocalVideo:canvas];
}

- (void)addRemoteUserViewWithUid:(NSString *)uid name:(NSString *)name {
    GroupVideoCallRoomUserModel *userModel = [GroupVideoCallRoomUserModel new];
    userModel.uid = uid;
    userModel.name = name;
    userModel.isScreen = NO;
    [self.userArray addObject:userModel];
    
    GroupVideoCallAvatarView *avatarView = [GroupVideoCallAvatarView new];
    [avatarView setName:name];
    [self.roomMainView addAvatarView:avatarView];

    ByteRTCVideoCanvas *videoCanvas = [ByteRTCVideoCanvas new];
    videoCanvas.view = avatarView.videoContainerView;
    videoCanvas.renderMode = ByteRTCRenderModeHidden;
    
    ByteRTCRemoteStreamKey *streamKey = [[ByteRTCRemoteStreamKey alloc] init];
    streamKey.userId = uid;
    streamKey.roomId = self.localUserModel.roomId;
    streamKey.streamIndex = ByteRTCStreamIndexMain;
    
    [[GroupVideoCallRTCManager shareRtc] setupRemoteVideoStreamKey:streamKey
                                                       canvas:videoCanvas];
}

- (void)removeRemoteUserViewWithUid:(NSString *)uid
            isOnlyRemoveScreenShare:(BOOL)isScreenShare {
    if (isScreenShare) {
        [self.fullScreenView dismiss:YES];
    }
    NSMutableArray *userModelMovedArray = [NSMutableArray array];
    for (GroupVideoCallRoomUserModel *userModel in self.userArray) {
        if ([userModel.uid isEqualToString:uid]) {
            if (isScreenShare && !userModel.isScreen) {
                continue;
            }
            NSUInteger index = [self.userArray indexOfObject:userModel];

            [userModelMovedArray addObject:userModel];
            [self.roomMainView removeAvatarViewAtIndex:index];
        }
    }
    [self.userArray removeObjectsInArray:userModelMovedArray];
}

- (void)addRemoteScreenViewWithUid:(NSString *)uid {
    GroupVideoCallRoomUserModel *screenModel = nil;
    GroupVideoCallRoomUserModel *screenUserModel = nil;
    for (GroupVideoCallRoomUserModel *model in self.userArray) {
        if ([model.uid isEqualToString:uid]) {
            if (model.isScreen) {
                screenModel = model;
            } else {
                screenUserModel = model;
            }
            break;
        }
    }
    if (!screenModel) {
        NSString *tagMessage = [NSString stringWithFormat:LocalizedString(@"%@'s_screen_sharing"), screenUserModel.name];
        screenModel = [GroupVideoCallRoomUserModel new];
        screenModel.uid = uid;
        screenModel.name = tagMessage;
        screenModel.isScreen = YES;
        screenModel.isEnableAudio = screenUserModel.isEnableAudio;
        screenModel.isEnableVideo = YES;
        [self.userArray insertObject:screenModel atIndex:0];
        
        GroupVideoCallAvatarView *avatarView = [GroupVideoCallAvatarView new];
        [avatarView setName:tagMessage];
        [self.roomMainView addAvatarView:avatarView atIndex:0];
    }
    
    GroupVideoCallAvatarView *avatarView = (GroupVideoCallAvatarView *)[self.roomMainView avatarViewAtIndex:0];
    UIView *screenRenderView = [[GroupVideoCallRTCManager shareRtc] getScreenStreamViewWithUid:uid];
    screenRenderView.hidden = NO;
    [avatarView.videoContainerView addSubview:screenRenderView];
    [screenRenderView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(avatarView.videoContainerView);
    }];
}

#pragma mark - Getter

- (GroupVideoCallRoomBottomView *)bottomView {
    if (!_bottomView) {
        _bottomView = [[GroupVideoCallRoomBottomView alloc] init];
        _bottomView.delegate = self;
    }
    return _bottomView;
}

- (GroupVideoCallRoomNavView *)navView {
    if (!_navView) {
        _navView = [[GroupVideoCallRoomNavView alloc] init];
        _navView.delegate = self;
    }
    return _navView;
}

- (NSMutableArray<GroupVideoCallRoomUserModel *> *)userArray {
    if (!_userArray) {
        _userArray = [NSMutableArray array];
    }
    return _userArray;
}

- (GroupVideoCallFullScreenView *)fullScreenView {
    if (!_fullScreenView) {
        _fullScreenView = [[GroupVideoCallFullScreenView alloc] init];
    }
    return _fullScreenView;
}

- (GroupVideoCallStatsView *)statsView {
    if (!_statsView) {
        _statsView = [[GroupVideoCallStatsView alloc] init];
    }
    return _statsView;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [UIApplication sharedApplication].idleTimerDisabled = NO;
}

@end
