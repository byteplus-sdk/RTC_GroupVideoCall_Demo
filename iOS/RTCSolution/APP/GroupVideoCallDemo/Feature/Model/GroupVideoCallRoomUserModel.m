// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import "GroupVideoCallRoomUserModel.h"
#import "GroupVideoCallRTCManager.h"

@implementation GroupVideoCallRoomUserModel

- (instancetype)initWithUid:(NSString *)uid {
    self = [super init];
    if (self) {
        self.uid = uid;
    }
    return self;
}

@end
