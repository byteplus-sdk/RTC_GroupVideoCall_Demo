//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import "GroupVideoCallStatsView.h"
#import "GroupVideoCallRoomAudioStatsTableViewCell.h"
#import "GroupVideoCallRoomStatsView.h"
#import "GroupVideoCallRoomVideoStatsTableViewCell.h"

@interface GroupVideoCallStatsView () <UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, copy) NSArray<GroupVideoCallRoomParamInfoModel *> *videoStatsInfoArray;
@property (nonatomic, copy) NSArray<GroupVideoCallRoomParamInfoModel *> *audioStatsInfoArray;

@property (nonatomic, strong) GroupVideoCallRoomStatsView *statsView;

@property (nonatomic, strong) UIButton *statsCloseButton;

@end

@implementation GroupVideoCallStatsView

- (instancetype)init {
    self = [super init];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        self.hidden = YES;

        [self addSubview:self.statsView];
        [self.statsView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.width.equalTo(self);
            make.height.mas_equalTo(414.f);
            make.bottom.equalTo(self).offset(414);
        }];

        [self addSubview:self.statsCloseButton];
        [self.statsCloseButton mas_makeConstraints:^(MASConstraintMaker *make) {
            make.edges.equalTo(self);
        }];
    }
    return self;
}

#pragma mark - Publish Action

- (void)setVideoStats:(NSArray<GroupVideoCallRoomParamInfoModel *> *)videoStats {
    self.videoStatsInfoArray = videoStats;

    [self.statsView.statsTableView reloadData];
}

- (void)setAudioStats:(NSArray<GroupVideoCallRoomParamInfoModel *> *)audioStats {
    self.audioStatsInfoArray = audioStats;

    [self.statsView.statsTableView reloadData];
}

- (void)showStatsView {
    [UIView animateWithDuration:0.25 animations:^{
        [self.statsView mas_updateConstraints:^(MASConstraintMaker *make) {
            make.bottom.equalTo(self).offset(0);
        }];
        [self.statsView.superview layoutIfNeeded];
    }];

    self.hidden = NO;
    self.statsCloseButton.hidden = NO;
    self.statsView.hidden = NO;
    [self bringSubviewToFront:self.statsView];
}

#pragma mark - Private Action

- (void)hiddeStatsView:(id)sender {
    self.hidden = YES;
    self.statsCloseButton.hidden = YES;
    self.statsView.hidden = YES;
    [self.statsView mas_updateConstraints:^(MASConstraintMaker *make) {
        make.bottom.equalTo(self).offset(414);
    }];
}
#pragma mark - UITableViewDelegate

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return 172.f;
}

#pragma mark - UITableViewDataSource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    NSUInteger index = self.statsView.currentSelectedIndex;
    if (index == 0) {
        return self.videoStatsInfoArray.count;
    } else {
        return self.audioStatsInfoArray.count;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    NSUInteger index = self.statsView.currentSelectedIndex;
    if (index == 0) {
        GroupVideoCallRoomVideoStatsTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"VideoStatsCell"];
        if (!cell) {
            cell = [[GroupVideoCallRoomVideoStatsTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"VideoStatsCell"];
        }
        [cell updateUIWithModel:[self.videoStatsInfoArray objectAtIndex:indexPath.row]];
        return cell;
    } else {
        GroupVideoCallRoomAudioStatsTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"AudioStatsCell"];
        if (!cell) {
            cell = [[GroupVideoCallRoomAudioStatsTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"AudioStatsCell"];
        }
        [cell updateUIWithModel:[self.audioStatsInfoArray objectAtIndex:indexPath.row]];
        return cell;
    }
}

#pragma mark - Getter

- (GroupVideoCallRoomStatsView *)statsView {
    if (!_statsView) {
        _statsView = [[GroupVideoCallRoomStatsView alloc] init];
        _statsView.statsTableView.delegate = self;
        _statsView.statsTableView.dataSource = self;
    }
    return _statsView;
}

- (UIButton *)statsCloseButton {
    if (!_statsCloseButton) {
        _statsCloseButton = [[UIButton alloc] init];
        [_statsCloseButton addTarget:self action:@selector(hiddeStatsView:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _statsCloseButton;
}

@end
