//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import "BaseRTCManager.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NetworkReachabilityManager : NSObject

+ (instancetype)sharedManager;

- (void)startMonitoring;

- (void)stopMonitoring;

- (NSString *)getNetType;

- (BOOL)isDisconnect;

@end

NS_ASSUME_NONNULL_END
