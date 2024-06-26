//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JoinRTSParamsModel : NSObject

@property (nonatomic, copy) NSString *appId;

@property (nonatomic, copy) NSString *RTSToken;

@property (nonatomic, copy) NSString *serverUrl;

@property (nonatomic, copy) NSString *serverSignature;

@property (nonatomic, copy) NSString *bid;

@end

NS_ASSUME_NONNULL_END
