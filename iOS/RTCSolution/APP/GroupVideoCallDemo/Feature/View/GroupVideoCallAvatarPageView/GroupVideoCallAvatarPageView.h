// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol GroupVideoCallAvatarPageViewDelegate <NSObject>
- (void)onShowAvatarView:(UIView *)avatarView index:(NSUInteger)index;
- (void)onHideAvatarView:(UIView *)avatarView index:(NSUInteger)index;
- (void)onClickAvatarView:(UIView *)avatarView index:(NSUInteger)index;
- (void)onScrollToPageIndex:(NSUInteger)pageIndex;

@end


@interface GroupVideoCallAvatarPageView : UIView

@property (nonatomic, weak) id <GroupVideoCallAvatarPageViewDelegate> avatarPageViewDelegate;

@property (nonatomic, readonly) NSUInteger currentPageIndex;

@property (nonatomic, readonly) NSUInteger currentPageCount;

@property (nonatomic, readonly) NSUInteger avatarViewCount;

@property (nonatomic, readonly) NSUInteger mainViewIndex;

/**
 * @brief Don't initialize via init, use initWithFrame:
 */
- (instancetype)init NS_UNAVAILABLE;

/**
 * @brief Don't initialize via initWithCoder, use initWithFrame:
 */
- (instancetype)initWithCoder:(NSCoder *)coder NS_UNAVAILABLE;

/**
 * @brief When initializing, specify the correct frame
 */
- (instancetype)initWithFrame:(CGRect)frame NS_DESIGNATED_INITIALIZER;

/**
 * @brief Add video window
 */
- (void)addAvatarView:(UIView *)avatarView;

/**
 * @brief Add video window according to index
 */
- (void)addAvatarView:(UIView *)avatarView atIndex:(NSUInteger)index;

/**
 * @brief Delete video window
 */
- (void)removeAvatarView:(UIView *)avatarView;

/**
 * @brief Delete video window according to index
 */
- (void)removeAvatarViewAtIndex:(NSUInteger)index;

/**
 * @brief Get the video window corresponding to the index
 */
- (nullable UIView *)avatarViewAtIndex:(NSUInteger)index;

/**
 * @brief Set the video window corresponding to the index as the main window (only effective when there are only two video windows)
 */
- (void)bringViewToMainAvatarViewOfIndex:(NSUInteger)index;


@end

NS_ASSUME_NONNULL_END
