// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import "GroupVideoCallMockDataComponent.h"
#import "GroupVideoCallSettingsRightLabelCell.h"
#import "GroupVideoCallSettingsSwitchCell.h"
#import "GroupVideoCallSettingViewController.h"
#import "GroupVideoCallPickerComponent.h"
#import "GroupVideoCallRTCManager.h"

static NSString *const kGroupVideoCallSettingsRightLabelCellIdentifier = @"kGroupVideoCallSettingsRightLabelCellIdentifier";
static NSString *const kGroupVideoCallSettingsSwitchCellIdentifier = @"kGroupVideoCallSettingsSwitchCellIdentifier";

typedef NS_ENUM(NSInteger, SettingsGroupType) {
    // Resolution configuration
    SettingsGroupTypeResolution,
    //  Conversation quality
    SettingsGroupTypeAudioProfile,
    //  Mirror
    SettingsGroupTypeMirror,
};

@interface GroupVideoCallSettingViewController () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) UITableView *settingsTableView;
@property (nonatomic, copy) NSArray *groupTypes;
@property (nonatomic, strong) GroupVideoCallPickerComponent *resolutionPicker;
@property (nonatomic, strong) GroupVideoCallPickerComponent *audioProfilePicker;
@property (nonatomic, assign) BOOL isOpenMirror;

@end

@implementation GroupVideoCallSettingViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor clearColor];
    self.navView.backgroundColor = [UIColor colorFromHexString:@"#1D2129"];
    self.groupTypes = @[LocalizedString(@"resolution"),
                        LocalizedString(@"call_quality"),
                        LocalizedString(@"local_mirror")];

    [self createUIComponent];
    [self.settingsTableView reloadData];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

    self.navTitle = LocalizedString(@"set_up");
}

- (void)createUIComponent {
    [self.view addSubview:self.settingsTableView];
    [self.settingsTableView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.navView.mas_bottom);
        make.left.right.bottom.equalTo(self.view);
    }];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    UITableViewCell *cell = [UITableViewCell new];
    GroupVideoCallMockDataComponent *mockData = [GroupVideoCallMockDataComponent shared];
    switch (indexPath.row) {
        case SettingsGroupTypeResolution: {
            cell = [tableView dequeueReusableCellWithIdentifier:kGroupVideoCallSettingsRightLabelCellIdentifier forIndexPath:indexPath];
            GroupVideoCallSettingsRightLabelCell *labelCell = (GroupVideoCallSettingsRightLabelCell *)cell;
            [labelCell settingsLabel].text = self.groupTypes[indexPath.row];
            [labelCell settingsRightLabel].text = mockData.currentResolutionDic[@"title"];
        } break;
        case SettingsGroupTypeAudioProfile: {
            cell = (GroupVideoCallSettingsRightLabelCell *)[tableView dequeueReusableCellWithIdentifier:kGroupVideoCallSettingsRightLabelCellIdentifier forIndexPath:indexPath];
            GroupVideoCallSettingsRightLabelCell *labelCell = (GroupVideoCallSettingsRightLabelCell *)cell;
            [labelCell settingsLabel].text = self.groupTypes[indexPath.row];
            [labelCell settingsRightLabel].text = mockData.currentaudioProfileDic[@"title"];
        } break;
            
        case SettingsGroupTypeMirror: {
            cell = (GroupVideoCallSettingsSwitchCell *)[tableView dequeueReusableCellWithIdentifier:kGroupVideoCallSettingsSwitchCellIdentifier forIndexPath:indexPath];
            GroupVideoCallSettingsSwitchCell *mirrorCell = (GroupVideoCallSettingsSwitchCell *)cell;
            [mirrorCell settingsLabel].text = self.groupTypes[indexPath.row];
            [mirrorCell setSwitchOn:mockData.isOpenMirror];
            __weak __typeof(self) wself = self;
            [mirrorCell switchValueChangeCallback:^(BOOL on) {
                mockData.isOpenMirror = on;
                [wself updateAudioAndVideoSettings];
            }];
        } break;
        default:
            break;
    }
    if (cell) {
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    GroupVideoCallMockDataComponent *mockData = [GroupVideoCallMockDataComponent shared];
    [tableView deselectRowAtIndexPath:indexPath animated:NO];
    switch (indexPath.row) {
        case SettingsGroupTypeResolution: {
            [self.resolutionPicker show:mockData.resLists
                             selectItem:mockData.currentResolutionDic];
            __weak __typeof(self) wself = self;
            self.resolutionPicker.clickDismissBlock = ^(BOOL isCancel, id selectItem, NSInteger row) {
                if (!isCancel) {
                    [GroupVideoCallMockDataComponent shared].currentResolutionDic = [GroupVideoCallMockDataComponent shared].resLists[row];
                    [wself.settingsTableView reloadData];
                    [wself updateAudioAndVideoSettings];
                }
                wself.resolutionPicker = nil;
            };
        } break;
        case SettingsGroupTypeAudioProfile: {
            [self.audioProfilePicker show:mockData.audioProfileLists
                               selectItem:mockData.currentaudioProfileDic];
            __weak __typeof(self) wself = self;
            self.audioProfilePicker.clickDismissBlock = ^(BOOL isCancel, id selectItem, NSInteger row) {
                if (!isCancel) {
                    [GroupVideoCallMockDataComponent shared].currentaudioProfileDic = [GroupVideoCallMockDataComponent shared].audioProfileLists[row];
                    [wself.settingsTableView reloadData];
                    [wself updateAudioAndVideoSettings];
                }
                wself.audioProfilePicker = nil;
            };
        } break;
        
        default:
            break;
    }
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 119/2;
}

#pragma mark - Private Action

- (void)updateAudioAndVideoSettings {
    [[GroupVideoCallRTCManager shareRtc] updateAudioAndVideoSettings];
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return self.groupTypes.count;
}

#pragma mark - Getter

- (UITableView *)settingsTableView {
    if (!_settingsTableView) {
        _settingsTableView = [[UITableView alloc] init];
        _settingsTableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        _settingsTableView.delegate = self;
        _settingsTableView.dataSource = self;
        [_settingsTableView registerClass:GroupVideoCallSettingsRightLabelCell.class forCellReuseIdentifier:kGroupVideoCallSettingsRightLabelCellIdentifier];
        [_settingsTableView registerClass:GroupVideoCallSettingsSwitchCell.class forCellReuseIdentifier:kGroupVideoCallSettingsSwitchCellIdentifier];
        _settingsTableView.backgroundColor = [UIColor colorFromHexString:@"#1D2129"];
    }
    return _settingsTableView;
}

- (GroupVideoCallPickerComponent *)resolutionPicker {
    if (!_resolutionPicker) {
        _resolutionPicker = [[GroupVideoCallPickerComponent alloc] initWithHeight:566/2 superView:self.view];
        _resolutionPicker.titleStr = LocalizedString(@"resolution");
    }
    return _resolutionPicker;
}

- (GroupVideoCallPickerComponent *)audioProfilePicker {
    if (!_audioProfilePicker) {
        _audioProfilePicker = [[GroupVideoCallPickerComponent alloc] initWithHeight:566/2 superView:self.view];
        _audioProfilePicker.titleStr = LocalizedString(@"call_quality");
    }
    return _audioProfilePicker;
}

#pragma mark - Tool

@end
