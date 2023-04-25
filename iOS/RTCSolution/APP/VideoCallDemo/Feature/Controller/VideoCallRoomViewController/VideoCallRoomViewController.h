// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import <UIKit/UIKit.h>
#import "VideoCallRoomUserModel.h"

@interface VideoCallRoomViewController : UIViewController

- (instancetype)initWithVideoSession:(VideoCallRoomUserModel *)loginModel
                            rtcToken:(NSString *)rtcToken
                            duration:(NSInteger)duration;

- (void)hangUp;

@end
