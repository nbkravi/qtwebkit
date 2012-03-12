/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "PlatformGestureCurve.h"

#include "ActivePlatformGestureAnimation.h"
#include "PlatformGestureCurveTarget.h"
#include "TouchFlingPlatformGestureCurve.h"
#include "WheelFlingPlatformGestureCurve.h"
#include "cc/CCActiveGestureAnimation.h"
#include "cc/CCGestureCurve.h"
#include <gtest/gtest.h>
#include <wtf/OwnPtr.h>

namespace {

using namespace WebCore;

class MockPlatformGestureCurveTarget : public PlatformGestureCurveTarget {
public:
    virtual void setScrollIncrement(const IntPoint& delta)
    {
        m_cumulativeDelta = m_cumulativeDelta + delta;
    }

    IntPoint cumulativeDelta() const { return  m_cumulativeDelta; }
    void resetCumulativeDelta() { m_cumulativeDelta = IntPoint(); }

private:
    IntPoint m_cumulativeDelta;
};

TEST(PlatformGestureCurve, flingCurve)
{
    MockPlatformGestureCurveTarget target;
    OwnPtr<ActivePlatformGestureAnimation> animation = ActivePlatformGestureAnimation::create(0, WheelFlingPlatformGestureCurve::create(FloatPoint(100, 0)), &target);

    // Note: the expectations below are dependent on the value of sigma hard-coded in the Rayleigh
    //       curve. If sigma changes, these test expectations will also change.
    EXPECT_TRUE(animation->animate(0));
    EXPECT_TRUE(animation->animate(0.25));
    EXPECT_TRUE(animation->animate(0.45)); // Use non-uniform tick spacing.
    EXPECT_TRUE(animation->animate(0.75));
    EXPECT_TRUE(animation->animate(1000));
    EXPECT_FALSE(animation->animate(1001));
    // Since the Rayleigh CDF maxes out at 1, we expect the cumulative scroll increments to
    // match the input velocity parameter.
    // Since we can be off by +/-0.5 on each conversion to int for setScrollIncrement,
    // pick the 'nearness' to be within the number of times animate returns true.
    EXPECT_NEAR(target.cumulativeDelta().x(), 100, 1);
    EXPECT_EQ(target.cumulativeDelta().y(), 0);

    double baseTime = 42.42;
    animation = ActivePlatformGestureAnimation::create(baseTime, WheelFlingPlatformGestureCurve::create(FloatPoint(100, 0)), &target);
    target.resetCumulativeDelta();

    EXPECT_TRUE(animation->animate(baseTime + 0.35));
    EXPECT_TRUE(animation->animate(baseTime + 1.35));
    EXPECT_FALSE(animation->animate(baseTime + 1000));
    EXPECT_NEAR(target.cumulativeDelta().x(), 100, 1);

    animation = ActivePlatformGestureAnimation::create(0, WheelFlingPlatformGestureCurve::create(FloatPoint(50, 150)), &target);
    target.resetCumulativeDelta();

    EXPECT_TRUE(animation->animate(0));
    EXPECT_TRUE(animation->animate(0.25));
    EXPECT_TRUE(animation->animate(0.45));
    EXPECT_TRUE(animation->animate(0.75));
    EXPECT_TRUE(animation->animate(1000));
    EXPECT_FALSE(animation->animate(1001));
    EXPECT_NEAR(target.cumulativeDelta().x(), 50, 1);
    EXPECT_NEAR(target.cumulativeDelta().y(), 150, 1);
}

TEST(PlatformGestureCurve, flingCurveTouch)
{
    MockPlatformGestureCurveTarget target;
    OwnPtr<ActivePlatformGestureAnimation> animation = ActivePlatformGestureAnimation::create(0, TouchFlingPlatformGestureCurve::create(FloatPoint(1000, 0)), &target);

    // Note: the expectations below are dependent on the value of sigma hard-coded in the Rayleigh
    //       curve. If sigma changes, these test expectations will also change.
    EXPECT_TRUE(animation->animate(0));
    EXPECT_TRUE(animation->animate(0.25));
    EXPECT_TRUE(animation->animate(0.45)); // Use non-uniform tick spacing.
    EXPECT_TRUE(animation->animate(0.75));
    EXPECT_TRUE(animation->animate(0.9));
    EXPECT_TRUE(animation->animate(1000));
    EXPECT_FALSE(animation->animate(1001));
    // Since we can be off by +/-0.5 on each conversion to int for setScrollIncrement,
    // pick the 'nearness' to be within the 5 * number of times animate returns true.
    EXPECT_NEAR(target.cumulativeDelta().x(), 1000, 1);
    EXPECT_EQ(target.cumulativeDelta().y(), 0);
}

} // namespace anonymous
