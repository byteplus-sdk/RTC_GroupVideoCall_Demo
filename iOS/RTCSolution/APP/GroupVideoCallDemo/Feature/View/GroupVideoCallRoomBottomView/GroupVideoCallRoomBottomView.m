// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import "GroupVideoCallRoomBottomView.h"

@interface GroupVideoCallRoomBottomView ()

@property (nonatomic, strong) UIView *contentView;
@property (nonatomic, strong) NSMutableArray *buttonLists;
@property (nonatomic, assign) BOOL isAction;
@end

@implementation GroupVideoCallRoomBottomView

- (instancetype)init {
    self = [super init];
    if (self) {
        self.isAction = NO;
        self.clipsToBounds = NO;
        self.backgroundColor = [UIColor colorFromHexString:@"#1D2129"];
        
        [self addSubview:self.contentView];
        [self.contentView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.edges.equalTo(self);
        }];
        
        [self addSubviewAndConstraints];
    }
    return self;
}

- (void)buttonAction:(GroupVideoCallRoomItemButton *)sender {
    if (self.isAction) {
        return;
    }
    self.isAction = YES;
    [self performSelector:@selector(buttonIsAction) withObject:nil afterDelay:0.25];
    RoomBottomStatus status = sender.tag - 3000;
    if ([self.delegate respondsToSelector:@selector(GroupVideoCallRoomBottomView:itemButton:didSelectStatus:)]) {
        [self.delegate GroupVideoCallRoomBottomView:self itemButton:sender didSelectStatus:status];
    }
}

- (void)buttonIsAction {
    self.isAction = NO;
}

- (void)addSubviewAndConstraints {
    NSInteger groupNum = 5;
    for (int i = 0; i < groupNum; i++) {
        GroupVideoCallRoomItemButton *button = [[GroupVideoCallRoomItemButton alloc] init];
        button.tag = 3000 + i;
        UIImage *image = [self getImageWithStatus:i];
        [button bingImage:image status:ButtonStatusNone];
        [button bingImage:[self getSelectImageWithStatus:i] status:ButtonStatusActive];
        button.desTitle = [self getdesTitleWithStatus:i];
        [button addTarget:self action:@selector(buttonAction:) forControlEvents:UIControlEventTouchUpInside];
        [button setImageEdgeInsets:UIEdgeInsetsMake(10, 0, 30, 0)];
        button.imageView.contentMode = UIViewContentModeScaleAspectFit;
        button.hidden = image ? NO : YES;
        [self.contentView addSubview:button];
        [self.buttonLists addObject:button];
    }
    
    [self.contentView.subviews mas_distributeViewsAlongAxis:MASAxisTypeHorizontal withFixedItemLength:150/2 leadSpacing:0 tailSpacing:0];
    [self.contentView.subviews mas_updateConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.contentView);
        make.bottom.mas_equalTo(-[DeviceInforTool getVirtualHomeHeight]);
    }];
}

- (void)updateButtonStatus:(RoomBottomStatus)status close:(BOOL)isClose {
    GroupVideoCallRoomItemButton *selectButton = [self getItemButtonToStatus:status];
    if (selectButton) {
        selectButton.status = isClose ? ButtonStatusActive : ButtonStatusNone;
        if (status == RoomBottomStatusAudio) {
            selectButton.desTitle = isClose ? LocalizedString(@"earpiece") : LocalizedString(@"speaker");
        }
    }
}

- (void)updateButtonStatus:(RoomBottomStatus)status enable:(BOOL)isEnable {
    GroupVideoCallRoomItemButton *selectButton = [self getItemButtonToStatus:status];
    if (selectButton) {
        selectButton.userInteractionEnabled = isEnable;
        selectButton.alpha = isEnable ? 1 : 0.5;
    }
}

- (ButtonStatus)getButtonStatus:(RoomBottomStatus)status {
    NSString *title = [self getdesTitleWithStatus:status];
    GroupVideoCallRoomItemButton *selectButton = nil;
    for (GroupVideoCallRoomItemButton *button in self.buttonLists) {
        if ([button.desTitle isEqualToString:title]) {
            selectButton = button;
            break;
        }
    }
    if (selectButton) {
        return selectButton.status;
    } else {
        return ButtonStatusNone;
    }
}

#pragma mark - Private Action

- (NSString *)getdesTitleWithStatus:(RoomBottomStatus)status {
    NSString *name = @"";
    switch (status) {
        case RoomBottomStatusMic:
            name = LocalizedString(@"microphone");
            break;
        case RoomBottomStatusCamera:
            name = LocalizedString(@"camera");
            break;
        case RoomBottomStatusAudio:
            name = LocalizedString(@"speaker");
            break;
        case RoomBottomStatusParameter:
            name = LocalizedString(@"real-time_data");
            break;
        case RoomBottomStatusSet:
            name = LocalizedString(@"set_up");
            break;
        default:
            break;
    }
    return name;
}

- (UIImage *)getImageWithStatus:(RoomBottomStatus)status {
    NSString *name = @"";
    switch (status) {
        case RoomBottomStatusMic:
            name = @"room_mic";
            break;
        case RoomBottomStatusCamera:
            name = @"room_video";
            break;
        case RoomBottomStatusAudio:
            name = @"room_audio";
            break;
        case RoomBottomStatusParameter:
            name = @"rom_parameter";
            break;
        case RoomBottomStatusSet:
            name = @"room_set";
            break;
        default:
            break;
    }
    return [UIImage imageNamed:name bundleName:HomeBundleName];
}

- (UIImage *)getSelectImageWithStatus:(RoomBottomStatus)status {
    NSString *name = @"";
    switch (status) {
        case RoomBottomStatusMic:
            name = @"room_mic_s";
            break;
        case RoomBottomStatusCamera:
            name = @"room_video_s";
            break;
        case RoomBottomStatusAudio:
            name = @"room_earpiece";
            break;
        default:
            break;
    }
    return [UIImage imageNamed:name bundleName:HomeBundleName];
}

- (GroupVideoCallRoomItemButton *)getItemButtonToStatus:(RoomBottomStatus)status {
    NSString *title = [self getdesTitleWithStatus:status];
    GroupVideoCallRoomItemButton *selectButton = nil;
    for (GroupVideoCallRoomItemButton *button in self.buttonLists) {
        if (status == RoomBottomStatusAudio) {
            if ([button.desTitle isEqualToString:LocalizedString(@"earpiece")] ||
                [button.desTitle isEqualToString:LocalizedString(@"speaker")]) {
                selectButton = button;
                break;
            }
        } else {
            if ([button.desTitle isEqualToString:title]) {
                selectButton = button;
                break;
            }
        }
    }
    return selectButton;
}

#pragma mark - Getter

- (UIView *)contentView {
    if (!_contentView) {
        _contentView = [[UIView alloc] init];
        _contentView.backgroundColor = [UIColor colorFromHexString:@"#1D2129"];
    }
    return _contentView;
}

- (NSMutableArray *)buttonLists {
    if (!_buttonLists) {
        _buttonLists = [[NSMutableArray alloc] init];
    }
    return _buttonLists;
}

@end
