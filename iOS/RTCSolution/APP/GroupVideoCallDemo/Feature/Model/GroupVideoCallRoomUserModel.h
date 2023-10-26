// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import "BaseUserModel.h"

@interface GroupVideoCallRoomUserModel : BaseUserModel

@property (nonatomic, copy) NSString *roomId;
@property (nonatomic, assign) BOOL isScreen;
@property (nonatomic, assign) BOOL isEnableVideo;
@property (nonatomic, assign) BOOL isEnableAudio;

// speaker or earpiece
@property (nonatomic, assign) BOOL isSpeakers;

@end

