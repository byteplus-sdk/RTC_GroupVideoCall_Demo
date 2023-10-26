// 
// Copyright (c) 2023 BytePlus Pte. Ltd.
// SPDX-License-Identifier: MIT
// 

#import "GroupVideoCallAvatarPageView.h"

@interface GroupVideoCallAvatarPageView () <UIScrollViewDelegate>

@property (nonatomic, assign) NSUInteger currentPageIndex;
@property (nonatomic, assign) NSUInteger currentPageCount;

@property (nonatomic, assign) NSUInteger mainViewIndex;

@property (nonatomic, strong) UIScrollView *pageView;

@property (nonatomic, strong) NSMutableArray<UIView *> *avatarViewArray;

@end

@implementation GroupVideoCallAvatarPageView

#pragma mark - Public Function

- (NSUInteger)avatarViewCount {
    return self.avatarViewArray.count;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        CGSize size = frame.size;
        _pageView = [[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, size.width, size.height)];
        _pageView.backgroundColor = [UIColor clearColor];
        _pageView.contentSize = size;
        _pageView.pagingEnabled = YES;

        [self addSubview:_pageView];

        _avatarViewArray = [NSMutableArray array];
        _currentPageCount = 1;
    }
    return self;
}

- (void)addAvatarView:(UIView *)avatarView {
    [self.avatarViewArray addObject:avatarView];

    [self pageViewAddAvatarView:avatarView];
}

- (void)addAvatarView:(UIView *)avatarView atIndex:(NSUInteger)index {
    if (index >= self.avatarViewCount) {
        [self addAvatarView:avatarView];
    } else {
        [self.avatarViewArray insertObject:avatarView atIndex:index];

        [self pageViewAddAvatarView:avatarView];
    }
}

- (void)removeAvatarView:(UIView *)avatarView {
    [avatarView removeFromSuperview];
    [self.avatarViewArray removeObject:avatarView];

    [self updatePageInfo];

    [self layoutPageView];
}

- (void)removeAvatarViewAtIndex:(NSUInteger)index {
    UIView *avatarViewWillRemoved = [self avatarViewAtIndex:index];

    [self removeAvatarView:avatarViewWillRemoved];
}

- (nullable UIView *)avatarViewAtIndex:(NSUInteger)index {
    if (index >= self.avatarViewCount) {
        return nil;
    }

    return self.avatarViewArray[index];
}

- (void)bringViewToMainAvatarViewOfIndex:(NSUInteger)index {
    _mainViewIndex = index;

    if ([self isOneByOne]) {
        [self layoutViewWithOneByOneMode];
    }
}

#pragma mark - Private Function

- (void)pageViewAddAvatarView:(UIView *)avatarView {
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onClickAvatarView:)];
    avatarView.userInteractionEnabled = YES;
    [avatarView addGestureRecognizer:tap];

    [self.pageView addSubview:avatarView];

    [self updatePageInfo];

    [self layoutPageView];
}

- (void)onClickAvatarView:(UITapGestureRecognizer *)sender {
    UIView *avatarView = sender.view;
    NSUInteger index = [self.avatarViewArray indexOfObject:avatarView];

    if (index != NSNotFound) {
        if (self.avatarPageViewDelegate && [self.avatarPageViewDelegate respondsToSelector:@selector(onClickAvatarView:index:)]) {
            [self.avatarPageViewDelegate onClickAvatarView:avatarView index:index];
        }
    }
}

- (BOOL)isOneByOne {
    return self.avatarViewCount <= 2;
}

- (void)updatePageInfo {
    self.currentPageCount = ceil((double)self.avatarViewCount / 4.0);

    CGSize pageSize = self.pageView.frame.size;
    self.pageView.contentSize = CGSizeMake(pageSize.width * self.currentPageCount, pageSize.height);

    if (self.currentPageIndex >= self.currentPageCount) {
        [self scrollToLastPage];
    }

    if ([self isOneByOne]) {
        // When you are alone, you will occupy the main window; when there are two people, the other party will occupy the main window
        self.mainViewIndex = self.avatarViewCount - 1;
    }
}

- (void)onTurnPage {
    CGSize pageSize = self.pageView.frame.size;

    NSUInteger currentIndex = (NSUInteger)(self.pageView.contentOffset.x / pageSize.width);

    NSUInteger previousIndex = self.currentPageIndex;

    if (currentIndex == previousIndex) {
        return;
    }

    if (self.avatarPageViewDelegate && [self.avatarPageViewDelegate respondsToSelector:@selector(onScrollToPageIndex:)]) {
        [self.avatarPageViewDelegate onScrollToPageIndex:currentIndex];
    }

    for (int i = (int)previousIndex * 4; (i < (previousIndex + 1) * 4) && (i < [self avatarViewCount]); i++) {
        UIView *avatarView = [self avatarViewAtIndex:i];

        if (self.avatarPageViewDelegate && [self.avatarPageViewDelegate respondsToSelector:@selector(onHideAvatarView:index:)]) {
            [self.avatarPageViewDelegate onHideAvatarView:avatarView index:i];
        }
    }

    for (int i = (int)currentIndex * 4; (i < (currentIndex + 1) * 4) && (i < [self avatarViewCount]); i++) {
        UIView *avatarView = [self avatarViewAtIndex:i];

        if (self.avatarPageViewDelegate && [self.avatarPageViewDelegate respondsToSelector:@selector(onShowAvatarView:index:)]) {
            [self.avatarPageViewDelegate onShowAvatarView:avatarView index:i];
        }
    }

    self.currentPageIndex = currentIndex;
}

- (void)scrollToLastPage {
    CGSize pageSize = self.pageView.frame.size;

    [self.pageView setContentOffset:CGPointMake(pageSize.width * (self.currentPageCount - 1), 0) animated:YES];
}

- (void)layoutViewWithOneByOneMode {
    UIView *firstView = [self avatarViewAtIndex:0];
    UIView *secondView = [self avatarViewAtIndex:1];

    UIView *mainView = (self.mainViewIndex == 0) ? firstView : secondView;
    UIView *smallView = (self.mainViewIndex == 0) ? secondView : firstView;

    if (mainView) {
        CGSize pageSize = self.pageView.frame.size;
        mainView.frame = CGRectMake(0, 0, pageSize.width, pageSize.height);
    }

    if (smallView) {
        CGFloat smallViewWith = 90.f;
        CGFloat smallViewHeight = 129.f;
        CGFloat margin = 8.f;
        CGSize pageSize = self.pageView.frame.size;

        smallView.frame = CGRectMake(pageSize.width - smallViewWith - margin, margin, smallViewWith, smallViewHeight);

        [self.pageView bringSubviewToFront:smallView];
    }
}

- (void)layoutViewWithMoreMode {
    CGSize pageSize = self.pageView.frame.size;

    for (UIView *avatarView in self.avatarViewArray) {
        NSUInteger index = [self.avatarViewArray indexOfObject:avatarView];

        NSUInteger pageIndex = index / 4;
        NSUInteger currentPageAvatarIndex = index % 4;
        NSUInteger columnIndex = currentPageAvatarIndex % 2;
        NSUInteger rowIndex = currentPageAvatarIndex / 2;

        CGFloat avatarWidth = pageSize.width * 0.5;
        CGFloat avatarHeight = pageSize.height * 0.5;

        avatarView.frame = CGRectMake(pageSize.width * pageIndex + columnIndex * avatarWidth, rowIndex * avatarHeight, avatarWidth, avatarHeight);
    }
}

- (void)layoutPageView {
    if ([self isOneByOne]) {
        [self layoutViewWithOneByOneMode];
    } else {
        [self layoutViewWithMoreMode];
    }
}

#pragma mark - UIScrollViewDelegate

- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)scrollView {
    [self onTurnPage];
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView {
    [self onTurnPage];
}

@end
