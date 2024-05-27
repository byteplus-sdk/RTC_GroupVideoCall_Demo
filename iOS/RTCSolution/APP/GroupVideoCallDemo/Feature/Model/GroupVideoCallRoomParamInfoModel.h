//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, GroupVideoCallRoomParamNetQuality) {
    GroupVideoCallRoomParamNetQualityGood,
    GroupVideoCallRoomParamNetQualityNormal,
    GroupVideoCallRoomParamNetQualityBad
};

@interface GroupVideoCallRoomParamInfoModel : NSObject

@property (nonatomic, strong) NSString *uid;
@property (nonatomic, strong) NSString *name;
@property (nonatomic, assign) NSInteger width;
@property (nonatomic, assign) NSInteger height;
@property (nonatomic, assign) NSInteger bitRate;
@property (nonatomic, assign) NSInteger fps;
@property (nonatomic, assign) NSInteger delay;
@property (nonatomic, assign) float lost;
@property (nonatomic, assign) GroupVideoCallRoomParamNetQuality netQuality;

@end

NS_ASSUME_NONNULL_END
