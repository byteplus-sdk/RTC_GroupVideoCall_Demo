// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import <UIKit/UIKit.h>
#import "GroupVideoCallRoomUserModel.h"

@interface GroupVideoCallRoomViewController : UIViewController

- (instancetype)initWithVideoSession:(GroupVideoCallRoomUserModel *)loginModel
                            rtcToken:(NSString *)rtcToken
                            duration:(NSInteger)duration;

- (void)hangUp;

@end
