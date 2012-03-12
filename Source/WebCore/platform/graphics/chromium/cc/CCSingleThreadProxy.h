/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CCSingleThreadProxy_h
#define CCSingleThreadProxy_h

#include "cc/CCAnimationEvents.h"
#include "cc/CCCompletionEvent.h"
#include "cc/CCLayerTreeHostImpl.h"
#include "cc/CCProxy.h"
#include <limits>
#include <wtf/OwnPtr.h>

namespace WebCore {

class CCLayerTreeHost;

class CCSingleThreadProxy : public CCProxy, CCLayerTreeHostImplClient {
public:
    static PassOwnPtr<CCProxy> create(CCLayerTreeHost*);
    virtual ~CCSingleThreadProxy();

    // CCProxy implementation
    virtual bool compositeAndReadback(void *pixels, const IntRect&);
    virtual void startPageScaleAnimation(const IntSize& targetPosition, bool useAnchor, float scale, double duration);
    virtual GraphicsContext3D* context();
    virtual void finishAllRendering();
    virtual bool isStarted() const;
    virtual bool initializeContext();
    virtual bool initializeLayerRenderer();
    virtual bool recreateContext();
    virtual int compositorIdentifier() const { return m_compositorIdentifier; }
    virtual const LayerRendererCapabilities& layerRendererCapabilities() const;
    virtual void loseContext();
    virtual void setNeedsAnimate();
    virtual void setNeedsCommit();
    virtual void setNeedsRedraw();
    virtual void setVisible(bool);
    virtual void start();
    virtual void stop();
    virtual size_t maxPartialTextureUpdates() const { return std::numeric_limits<size_t>::max(); }

    // CCLayerTreeHostImplClient implementation
    virtual void didLoseContextOnImplThread() { }
    virtual void onSwapBuffersCompleteOnImplThread() { ASSERT_NOT_REACHED(); }
    virtual void setNeedsRedrawOnImplThread() { m_layerTreeHost->setNeedsCommit(); }
    virtual void setNeedsCommitOnImplThread() { m_layerTreeHost->setNeedsCommit(); }
    virtual void postAnimationEventsToMainThreadOnImplThread(PassOwnPtr<CCAnimationEventsVector>, double wallClockTime);

    // Called by the legacy path where RenderWidget does the scheduling.
    void compositeImmediately();

private:
    explicit CCSingleThreadProxy(CCLayerTreeHost*);

    bool commitIfNeeded();
    void doCommit();
    bool doComposite();
    void didSwapFrame();

    // Accessed on main thread only.
    CCLayerTreeHost* m_layerTreeHost;
    bool m_contextLost;
    int m_compositorIdentifier;

    // Holds on to the context between initializeContext() and initializeLayerRenderer() calls. Shouldn't
    // be used for anything else.
    RefPtr<GraphicsContext3D> m_contextBeforeInitialization;

    // Used on the CCThread, but checked on main thread during initialization/shutdown.
    OwnPtr<CCLayerTreeHostImpl> m_layerTreeHostImpl;
    bool m_layerRendererInitialized;
    LayerRendererCapabilities m_layerRendererCapabilitiesForMainThread;

    bool m_nextFrameIsNewlyCommittedFrame;
};

// For use in the single-threaded case. In debug builds, it pretends that the
// code is running on the thread to satisfy assertion checks.
class DebugScopedSetImplThread {
public:
    DebugScopedSetImplThread()
    {
#if !ASSERT_DISABLED
        CCProxy::setCurrentThreadIsImplThread(true);
#endif
    }
    ~DebugScopedSetImplThread()
    {
#if !ASSERT_DISABLED
        CCProxy::setCurrentThreadIsImplThread(false);
#endif
    }
};

// For use in the single-threaded case. In debug builds, it pretends that the
// code is running on the main thread to satisfy assertion checks.
class DebugScopedSetMainThread {
public:
    DebugScopedSetMainThread()
    {
#if !ASSERT_DISABLED
        CCProxy::setCurrentThreadIsImplThread(false);
#endif
    }
    ~DebugScopedSetMainThread()
    {
#if !ASSERT_DISABLED
        CCProxy::setCurrentThreadIsImplThread(true);
#endif
    }
};

} // namespace WebCore

#endif
