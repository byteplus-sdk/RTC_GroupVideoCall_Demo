//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface GroupVideoCallMockDataComponent : NSObject

+ (instancetype)shared;

@property (nonatomic, copy) NSDictionary *currentResolutionDic;

@property (nonatomic, copy) NSDictionary *currentaudioProfileDic;

@property (nonatomic, assign) BOOL isOpenMirror;

@property (nonatomic, copy) NSArray *resLists;

@property (nonatomic, copy) NSArray *audioProfileLists;

@end

NS_ASSUME_NONNULL_END
