//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import <UIKit/UIKit.h>

@interface GroupVideoCallSettingsSwitchCell : UITableViewCell

@property (nonatomic, strong, readonly) UILabel *settingsLabel;

- (void)setSwitchOn:(BOOL)on;
- (void)switchValueChangeCallback:(void (^)(BOOL on))callback;
- (void)setSwitchAccessibilityIdentifier:(NSString *)accessId;

@end
