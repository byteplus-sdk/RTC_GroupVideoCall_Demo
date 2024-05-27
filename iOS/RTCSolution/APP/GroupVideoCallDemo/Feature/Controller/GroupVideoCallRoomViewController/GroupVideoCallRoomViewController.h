//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import "GroupVideoCallRoomUserModel.h"
#import <UIKit/UIKit.h>

@interface GroupVideoCallRoomViewController : UIViewController

- (instancetype)initWithVideoSession:(GroupVideoCallRoomUserModel *)loginModel
                            rtcToken:(NSString *)rtcToken
                            duration:(NSInteger)duration;

- (void)hangUp;

@end
