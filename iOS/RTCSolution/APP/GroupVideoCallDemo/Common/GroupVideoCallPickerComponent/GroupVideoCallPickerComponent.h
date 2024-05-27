//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import <Foundation/Foundation.h>

@interface GroupVideoCallPickerComponent : NSObject

@property (nonatomic, copy) NSString *titleStr;

@property (nonatomic, copy) void (^clickDismissBlock)(BOOL isCancel, id selectItem, NSInteger row);

- (instancetype)initWithHeight:(CGFloat)height superView:(UIView *)superView;

- (void)show:(NSArray *)dataLists selectItem:(id)item;

@end
