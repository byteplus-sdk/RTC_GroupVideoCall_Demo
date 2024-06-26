//
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
@class BytedPlayerProtocol;

typedef NS_ENUM(NSInteger, PullScalingMode) {
    PullScalingModeNone,
    PullScalingModeAspectFit,
    PullScalingModeAspectFill,
    PullScalingModeFill
};

NS_ASSUME_NONNULL_BEGIN

@protocol BytedPlayerDelegate <NSObject>

- (void)protocol:(BytedPlayerProtocol *)protocol
    setPlayerWithURL:(NSString *)urlString
           superView:(UIView *)superView
            SEIBlcok:(void (^)(NSDictionary *SEIDic))SEIBlcok;

- (void)protocol:(BytedPlayerProtocol *)protocol updatePlayScaleMode:(PullScalingMode)scalingMode;

- (void)protocolDidPlay:(BytedPlayerProtocol *)protocol;

- (void)protocolDidStop:(BytedPlayerProtocol *)protocol;

- (void)protocolDestroy:(BytedPlayerProtocol *)protocol;

- (void)protocol:(BytedPlayerProtocol *)protocol replacePlayWithUrl:(NSString *)url;

- (BOOL)protocolIsSupportSEI;

- (void)protocolStartWithConfiguration;

@end

@interface BytedPlayerProtocol : NSObject

/**
 * @brief Start configuration Player
 */
- (void)startWithConfiguration;

/**
 * @brief Set playback address, parent view
 * @param urlString stream URL
 * @param superView parent class view
 * @param SEIBlcok SEI callback
 */
- (void)setPlayerWithURL:(NSString *)urlString
               superView:(UIView *)superView
                SEIBlcok:(void (^)(NSDictionary *SEIDic))SEIBlcok;

/**
 * @brief update playback scale mode
 * @param scalingMode playback scale mode
 */
- (void)updatePlayScaleMode:(PullScalingMode)scalingMode;

/**
 * @brief start playing
 */
- (void)play;

/**
 * @brief stop playback
 */
- (void)stop;

/**
 * @brief Update the new playback address
 * @param url new play URL
 */
- (void)replacePlayWithUrl:(NSString *)url;

/**
 * @brief Whether the player supports SEI function
 * @return BOOL YES supports SEI, NO does not support SEI
 */
- (BOOL)isSupportSEI;

/**
 * @brief Release player
 */
- (void)destroy;

@end

NS_ASSUME_NONNULL_END
