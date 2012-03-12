/*
 * Copyright (C) 2009, 2010, 2011, 2012 Research In Motion Limited. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef WebPage_p_h
#define WebPage_p_h

#include "ChromeClient.h"
#if USE(ACCELERATED_COMPOSITING)
#include "LayerRenderer.h"
#endif
#include "PageClientBlackBerry.h"
#include "PlatformMouseEvent.h"
#include "ScriptSourceCode.h"
#include "Timer.h"
#include "ViewportArguments.h"
#include "WebPage.h"
#include "WebSettings.h"

#include <BlackBerryPlatformMessage.h>

namespace WebCore {
class DOMWrapperWorld;
class Document;
class Frame;
class GeolocationControllerClientBlackBerry;
class JavaScriptDebuggerBlackBerry;
class KURL;
class Node;
class Page;
class PluginView;
class RenderLayer;
class RenderObject;
class ScrollView;
class TransformationMatrix;
template<typename T> class Timer;
}

namespace BlackBerry {
namespace WebKit {

class BackingStore;
class BackingStoreClient;
class BackingStoreTile;
class DumpRenderTreeClient;
class InPageSearchManager;
class InputHandler;
class SelectionHandler;
class TouchEventHandler;
class WebPageClient;

#if USE(ACCELERATED_COMPOSITING)
class FrameLayers;
class WebPageCompositor;
#endif

// In the client code, there is screen size and viewport.
// In WebPagePrivate, the screen size is called the transformedViewportSize,
// the viewport position is called the transformedScrollPosition,
// and the viewport size is called the transformedActualVisibleSize.
class WebPagePrivate : public PageClientBlackBerry, public WebSettingsDelegate, public Platform::GuardedPointerBase {
public:
    enum ViewMode { Mobile, Desktop, FixedDesktop };
    enum LoadState { None /* on instantiation of page */, Provisional, Committed, Finished, Failed };

    WebPagePrivate(WebPage*, WebPageClient*, const WebCore::IntRect&);
    virtual ~WebPagePrivate();

    static WebCore::Page* core(const WebPage*);

    void init(const WebString& pageGroupName);
    bool handleMouseEvent(WebCore::PlatformMouseEvent&);
    bool handleWheelEvent(WebCore::PlatformWheelEvent&);

    void load(const char* url, const char* networkToken, const char* method, Platform::NetworkRequest::CachePolicy, const char* data, size_t dataLength, const char* const* headers, size_t headersLength, bool isInitial, bool mustHandleInternally = false, bool forceDownload = false, const char* overrideContentType = "");
    void loadString(const char* string, const char* baseURL, const char* mimeType, const char* failingURL = 0);
    bool executeJavaScript(const char* script, JavaScriptDataType& returnType, WebString& returnValue);
    bool executeJavaScriptInIsolatedWorld(const WebCore::ScriptSourceCode&, JavaScriptDataType& returnType, WebString& returnValue);

    void stopCurrentLoad();
    void prepareToDestroy();

    LoadState loadState() const { return m_loadState; }
    bool isLoading() const { return m_loadState == WebPagePrivate::Provisional || m_loadState == WebPagePrivate::Committed; }

    // Called from within WebKit via FrameLoaderClientBlackBerry.
    void setLoadState(LoadState);

    // Clamp the scale.
    double clampedScale(double scale) const;

    // Determine if we should zoom, clamping the scale parameter if required.
    bool shouldZoomAboutPoint(double scale, const WebCore::FloatPoint& anchor, bool enforeScaleClamping, double* clampedScale);

    // Scale the page to the given scale and anchor about the point which is specified in untransformed content coordinates.
    bool zoomAboutPoint(double scale, const WebCore::FloatPoint& anchor, bool enforceScaleClamping = true, bool forceRendering = false, bool isRestoringZoomLevel = false);
    bool scheduleZoomAboutPoint(double scale, const WebCore::FloatPoint& anchor, bool enforceScaleClamping = true, bool forceRendering = false);
    void unscheduleZoomAboutPoint();
    WebCore::IntPoint calculateReflowedScrollPosition(const WebCore::FloatPoint& anchorOffset, double inverseScale);
    void setTextReflowAnchorPoint(const Platform::IntPoint& focalPoint);

    void schedulePinchZoomAboutPoint(double scale, const WebCore::IntPoint& anchor);

    // Perform actual zoom for block zoom.
    void zoomBlock();

    // Called by the backing store as well as the method below.
    void requestLayoutIfNeeded() const;
    void setNeedsLayout();

    WebCore::IntPoint scrollPosition() const;
    WebCore::IntPoint maximumScrollPosition() const;
    void setScrollPosition(const WebCore::IntPoint&);
    bool scrollBy(int deltaX, int deltaY, bool scrollMainFrame = true);

    void enqueueRenderingOfClippedContentOfScrollableNodeAfterInRegionScrolling(WebCore::Node*);
    std::vector<Platform::ScrollViewBase> inRegionScrollableAreasForPoint(const Platform::IntPoint&);
    void notifyInRegionScrollStatusChanged(bool status);
    void setScrollOriginPoint(const Platform::IntPoint&);
    void setHasInRegionScrollableAreas(bool);

    // The actual visible size as reported by the client, but in WebKit coordinates.
    WebCore::IntSize actualVisibleSize() const;

    // The viewport size is the same as the client's window size, but in webkit coordinates.
    WebCore::IntSize viewportSize() const;

    // Modifies the zoomToFit algorithm logic to construct a scale such that the viewportSize above is equal to this size.
    bool hasVirtualViewport() const;
    bool isUserScalable() const { return m_userScalable; }
    void setUserScalable(bool userScalable) { m_userScalable = userScalable; }

    // Sets default layout size without doing layout or marking as needing layout.
    void setDefaultLayoutSize(const WebCore::IntSize&);

    // Updates WebCore when the viewportSize() or actualVisibleSize() change.
    void updateViewportSize(bool setFixedReportedSize = true, bool sendResizeEvent = true);

    WebCore::FloatPoint centerOfVisibleContentsRect() const;
    WebCore::IntRect visibleContentsRect() const;
    WebCore::IntSize contentsSize() const;
    WebCore::IntSize absoluteVisibleOverflowSize() const;

    // Virtual functions inherited from PageClientBlackBerry.
    virtual void setCursor(WebCore::PlatformCursorHandle);
    virtual Platform::NetworkStreamFactory* networkStreamFactory();
    virtual Platform::Graphics::Window* platformWindow() const;
    virtual void setPreventsScreenDimming(bool preventDimming);
    virtual void showVirtualKeyboard(bool showKeyboard);
    virtual void ensureContentVisible(bool centerInView = true);
    virtual void zoomToContentRect(const WebCore::IntRect&);
    virtual void registerPlugin(WebCore::PluginView*, bool);
    virtual void notifyPageOnLoad();
    virtual bool shouldPluginEnterFullScreen(WebCore::PluginView*, const char*);
    virtual void didPluginEnterFullScreen(WebCore::PluginView*, const char*);
    virtual void didPluginExitFullScreen(WebCore::PluginView*, const char*);
    virtual void onPluginStartBackgroundPlay(WebCore::PluginView*, const char*);
    virtual void onPluginStopBackgroundPlay(WebCore::PluginView*, const char*);
    virtual bool lockOrientation(bool landscape);
    virtual void unlockOrientation();
    virtual int orientation() const;
    virtual double currentZoomFactor() const;
    virtual int showAlertDialog(WebPageClient::AlertType atype);
    virtual bool isActive() const;

    // Called from within WebKit via ChromeClientBlackBerry.
    void enterFullscreenForNode(WebCore::Node*);
    void exitFullscreenForNode(WebCore::Node*);
    void contentsSizeChanged(const WebCore::IntSize&);
    void overflowExceedsContentsSize() { m_overflowExceedsContentsSize = true; }
    void layoutFinished();
    void setNeedTouchEvents(bool);

    // Called according to our heuristic or from setLoadState depending on whether we have a virtual viewport.
    void zoomToInitialScaleOnLoad();

    // Various scale factors.
    double currentScale() const { return m_transformationMatrix->m11(); }
    double zoomToFitScale() const;
    double initialScale() const;
    void setInitialScale(double scale) { m_initialScale = scale; }
    double minimumScale() const
    {
        return (m_minimumScale > zoomToFitScale() && m_minimumScale <= maximumScale()) ? m_minimumScale : zoomToFitScale();
    }

    void setMinimumScale(double scale) { m_minimumScale = scale; }
    double maximumScale() const;
    void setMaximumScale(double scale) { m_maximumScale = scale; }
    void resetScales();

    // Note: to make this reflow width transform invariant just use
    // transformedActualVisibleSize() here instead!
    int reflowWidth() const { return actualVisibleSize().width(); }

    // These methods give the real geometry of the device given the currently set transform.
    WebCore::IntPoint transformedScrollPosition() const;
    WebCore::IntPoint transformedMaximumScrollPosition() const;
    WebCore::IntSize transformedActualVisibleSize() const;
    WebCore::IntSize transformedViewportSize() const;
    WebCore::IntRect transformedVisibleContentsRect() const;
    WebCore::IntSize transformedContentsSize() const;

    // Generic conversions of points, rects, relative to and from contents and viewport.
    WebCore::IntPoint mapFromContentsToViewport(const WebCore::IntPoint&) const;
    WebCore::IntPoint mapFromViewportToContents(const WebCore::IntPoint&) const;
    WebCore::IntRect mapFromContentsToViewport(const WebCore::IntRect&) const;
    WebCore::IntRect mapFromViewportToContents(const WebCore::IntRect&) const;

    // Generic conversions of points, rects, relative to and from transformed contents and transformed viewport.
    WebCore::IntPoint mapFromTransformedContentsToTransformedViewport(const WebCore::IntPoint&) const;
    WebCore::IntPoint mapFromTransformedViewportToTransformedContents(const WebCore::IntPoint&) const;
    WebCore::IntRect mapFromTransformedContentsToTransformedViewport(const WebCore::IntRect&) const;
    WebCore::IntRect mapFromTransformedViewportToTransformedContents(const WebCore::IntRect&) const;

    // Generic conversions of points, rects, and sizes to and from transformed coordinates.
    WebCore::IntPoint mapToTransformed(const WebCore::IntPoint&) const;
    WebCore::FloatPoint mapToTransformedFloatPoint(const WebCore::FloatPoint&) const;
    WebCore::IntPoint mapFromTransformed(const WebCore::IntPoint&) const;
    WebCore::FloatPoint mapFromTransformedFloatPoint(const WebCore::FloatPoint&) const;
    WebCore::FloatRect mapFromTransformedFloatRect(const WebCore::FloatRect&) const;
    WebCore::IntSize mapToTransformed(const WebCore::IntSize&) const;
    WebCore::IntSize mapFromTransformed(const WebCore::IntSize&) const;
    WebCore::IntRect mapToTransformed(const WebCore::IntRect&) const;
    void clipToTransformedContentsRect(WebCore::IntRect&) const;
    WebCore::IntRect mapFromTransformed(const WebCore::IntRect&) const;
    bool transformedPointEqualsUntransformedPoint(const WebCore::IntPoint& transformedPoint, const WebCore::IntPoint& untransformedPoint);

    // Notification methods that deliver changes to the real geometry of the device as specified above.
    void notifyTransformChanged();
    void notifyTransformedContentsSizeChanged();
    void notifyTransformedScrollChanged();

    void assignFocus(Platform::FocusDirection);
    Platform::IntRect focusNodeRect();
    WebCore::IntRect getRecursiveVisibleWindowRect(WebCore::ScrollView*, bool noClipOfMainFrame = false);

    WebCore::IntPoint frameOffset(const WebCore::Frame*) const;

    WebCore::Node* bestNodeForZoomUnderPoint(const WebCore::IntPoint&);
    WebCore::Node* bestChildNodeForClickRect(WebCore::Node* parentNode, const WebCore::IntRect& clickRect);
    WebCore::Node* nodeForZoomUnderPoint(const WebCore::IntPoint&);
    WebCore::Node* adjustedBlockZoomNodeForZoomLimits(WebCore::Node*);
    WebCore::IntRect rectForNode(WebCore::Node*);
    WebCore::IntRect blockZoomRectForNode(WebCore::Node*);
    WebCore::IntRect adjustRectOffsetForFrameOffset(const WebCore::IntRect&, const WebCore::Node*);
    bool compareNodesForBlockZoom(WebCore::Node* n1, WebCore::Node* n2);
    double newScaleForBlockZoomRect(const WebCore::IntRect&, double oldScale, double margin);
    double maxBlockZoomScale() const;

    // Plugin Methods.
    void notifyPluginRectChanged(int id, const WebCore::IntRect& rectChanged);

    // Context Methods.
    ActiveNodeContext activeNodeContext(TargetDetectionStrategy);
    PassRefPtr<WebCore::Node> contextNode(TargetDetectionStrategy);

#if ENABLE(VIEWPORT_REFLOW)
    void toggleTextReflowIfEnabledForBlockZoomOnly(bool shouldEnableTextReflow = false);
#endif

    void selectionChanged(WebCore::Frame*);

    void updateDelegatedOverlays(bool dispatched = false);

    void updateCursor();

    void onInputLocaleChanged(bool isRTL);

    ViewMode viewMode() const { return m_viewMode; }
    bool setViewMode(ViewMode); // Returns true if the change requires re-layout.

    void setShouldUseFixedDesktopMode(bool b) { m_shouldUseFixedDesktopMode = b; }

    bool useFixedLayout() const;
    WebCore::IntSize fixedLayoutSize(bool snapToIncrement = false) const;

    // ZoomToFitOnLoad can lead to a large recursion depth in FrameView::layout() as we attempt
    // to determine the zoom scale factor so as to have the content of the page fit within the
    // area of the frame. From observation, we can bail out after a recursion depth of 10 and
    // still have reasonable results.
    bool didLayoutExceedMaximumIterations() const { return m_nestedLayoutFinishedCount > 10; }

    void clearFocusNode();
    WebCore::Frame* focusedOrMainFrame() const;
    WebCore::Frame* mainFrame() const { return m_mainFrame; }

    bool scrollNodeRecursively(WebCore::Node* originalNode, const WebCore::IntSize& delta);
    bool scrollRenderer(WebCore::RenderObject* renderer, const WebCore::IntSize& delta);
    void adjustScrollDelta(const WebCore::IntPoint& maxOffset, const WebCore::IntPoint& currentOffset, WebCore::IntSize& delta) const;

    bool canScrollRenderBox(WebCore::RenderBox*);
    bool canScrollInnerFrame(WebCore::Frame*) const;

#if ENABLE(EVENT_MODE_METATAGS)
    void didReceiveCursorEventMode(WebCore::CursorEventMode);
    void didReceiveTouchEventMode(WebCore::TouchEventMode);
#endif

    void dispatchViewportPropertiesDidChange(const WebCore::ViewportArguments&);
    WebCore::IntSize recomputeVirtualViewportFromViewportArguments();

    void resetBlockZoom();

    void zoomAboutPointTimerFired(WebCore::Timer<WebPagePrivate>*);
    bool shouldSendResizeEvent();
    void scrollEventTimerFired(WebCore::Timer<WebPagePrivate>*);
    void resizeEventTimerFired(WebCore::Timer<WebPagePrivate>*);

    // If this url should be handled as a pattern, returns the pattern
    // otherwise, returns an empty string.
    String findPatternStringForUrl(const WebCore::KURL&) const;

    void suspendBackingStore();
    void resumeBackingStore();

    void setShouldResetTilesWhenShown(bool flag) { m_shouldResetTilesWhenShown = flag; }
    bool shouldResetTilesWhenShown() const { return m_shouldResetTilesWhenShown; }

    void setScreenOrientation(int);

    // Scroll and/or zoom so that the WebPage fits the new actual
    // visible size.
    void setViewportSize(const WebCore::IntSize& transformedActualVisibleSize, bool ensureFocusElementVisible);
    void screenRotated(); // Helper method for setViewportSize().

    void scheduleDeferrableTimer(WebCore::Timer<WebPagePrivate>*, double timeOut);
    void unscheduleAllDeferrableTimers();
    void willDeferLoading();
    void didResumeLoading();

    // Returns true if the escape key handler should zoom.
    bool shouldZoomOnEscape() const;

    WebCore::TransformationMatrix* transformationMatrix() const
    {
        return m_transformationMatrix;
    }

#if USE(ACCELERATED_COMPOSITING)
    // WebKit thread.
    bool needsOneShotDrawingSynchronization();
    void rootLayerCommitTimerFired(WebCore::Timer<WebPagePrivate>*);
    bool commitRootLayerIfNeeded();
    WebCore::LayerRenderingResults lastCompositingResults() const;

    // WebKit thread, plumbed through from ChromeClientBlackBerry.
    void setRootLayerWebKitThread(WebCore::Frame*, WebCore::LayerWebKitThread*);
    void setNeedsOneShotDrawingSynchronization();
    void scheduleRootLayerCommit();

    // Thread safe.
    void resetCompositingSurface();
    void drawLayersOnCommit(); // Including backing store blit.
    bool drawSubLayers(const WebCore::IntRect& dstRect, const WebCore::FloatRect& contents);
    bool drawSubLayers(); // Draw them at last known position.

    // Compositing thread.
    void setRootLayerCompositingThread(WebCore::LayerCompositingThread*);
    void commitRootLayer(const WebCore::IntRect&, const WebCore::IntSize&);
    void setIsAcceleratedCompositingActive(bool);
    bool isAcceleratedCompositingActive() const { return m_isAcceleratedCompositingActive; }
    void destroyCompositor();
    void syncDestroyCompositorOnCompositingThread();
    void destroyLayerResources();
    void suspendRootLayerCommit();
    void resumeRootLayerCommit();
    void blitVisibleContents();
#endif

    bool dispatchTouchEventToFullScreenPlugin(WebCore::PluginView*, const Platform::TouchEvent&);
    bool dispatchTouchPointAsMouseEventToFullScreenPlugin(WebCore::PluginView*, const Platform::TouchPoint&);
    bool dispatchMouseEventToFullScreenPlugin(WebCore::PluginView*, const Platform::MouseEvent&);

    BackingStoreClient* backingStoreClientForFrame(const WebCore::Frame*) const;
    void addBackingStoreClientForFrame(const WebCore::Frame*, BackingStoreClient*);
    void removeBackingStoreClientForFrame(const WebCore::Frame*);

    // Clean up any document related data we might be holding.
    void clearDocumentData(const WebCore::Document*);

    void frameUnloaded(const WebCore::Frame*);

    static WebCore::RenderLayer* enclosingPositionedAncestorOrSelfIfPositioned(WebCore::RenderLayer*);
    static WebCore::RenderLayer* enclosingFixedPositionedAncestorOrSelfIfFixedPositioned(WebCore::RenderLayer*);

    static WebCore::IntSize defaultMaxLayoutSize();

    WebPage* m_webPage;
    WebPageClient* m_client;
    WebCore::Page* m_page;
    WebCore::Frame* m_mainFrame;
    RefPtr<WebCore::Node> m_currentContextNode;
    WebSettings* m_webSettings;

#if ENABLE(JAVASCRIPT_DEBUGGER)
    OwnPtr<WebCore::JavaScriptDebuggerBlackBerry> m_scriptDebugger;
#endif

    bool m_visible;
    bool m_shouldResetTilesWhenShown;
    bool m_userScalable;
    bool m_userPerformedManualZoom;
    bool m_userPerformedManualScroll;
    bool m_contentsSizeChanged;
    bool m_overflowExceedsContentsSize;
    bool m_resetVirtualViewportOnCommitted;
    bool m_shouldUseFixedDesktopMode;
    bool m_needTouchEvents;
    int m_preventIdleDimmingCount;

#if ENABLE(TOUCH_EVENTS)
    bool m_preventDefaultOnTouchStart;
#endif
    unsigned m_nestedLayoutFinishedCount;
    WebCore::IntSize m_previousContentsSize;
    int m_actualVisibleWidth;
    int m_actualVisibleHeight;
    int m_virtualViewportWidth;
    int m_virtualViewportHeight;
    WebCore::IntSize m_defaultLayoutSize;
    WebCore::ViewportArguments m_viewportArguments; // We keep this around since we may need to re-evaluate the arguments on rotation.
    bool m_didRestoreFromPageCache;
    ViewMode m_viewMode;
    LoadState m_loadState;
    WebCore::TransformationMatrix* m_transformationMatrix;
    BackingStore* m_backingStore;
    BackingStoreClient* m_backingStoreClient;
    InPageSearchManager* m_inPageSearchManager;
    InputHandler* m_inputHandler;
    SelectionHandler* m_selectionHandler;
    TouchEventHandler* m_touchEventHandler;

#if ENABLE(EVENT_MODE_METATAGS)
    WebCore::CursorEventMode m_cursorEventMode;
    WebCore::TouchEventMode m_touchEventMode;
#endif

    Platform::BlackBerryCursor m_currentCursor;

    DumpRenderTreeClient* m_dumpRenderTree;

    double m_initialScale;
    double m_minimumScale;
    double m_maximumScale;

    // Block zoom animation data.
    WebCore::FloatPoint m_finalBlockPoint;
    WebCore::FloatPoint m_finalBlockPointReflowOffset;
    double m_blockZoomFinalScale;
    RefPtr<WebCore::Node> m_currentPinchZoomNode;
    WebCore::FloatPoint m_anchorInNodeRectRatio;
    RefPtr<WebCore::Node> m_currentBlockZoomNode;
    RefPtr<WebCore::Node> m_currentBlockZoomAdjustedNode;
    bool m_shouldReflowBlock;

    // Delayed zoomAboutPoint.
    OwnPtr<WebCore::Timer<WebPagePrivate> > m_delayedZoomTimer;
    struct {
        double scale;
        WebCore::FloatPoint anchor;
        bool enforceScaleClamping;
        bool forceRendering;
    } m_delayedZoomArguments;

    double m_lastUserEventTimestamp; // Used to detect user scrolling.

    WebCore::PlatformMouseEvent m_lastMouseEvent;
    bool m_pluginMouseButtonPressed; // Used to track mouse button for full screen plugins.
    bool m_pluginMayOpenNewTab;

    WebCore::GeolocationControllerClientBlackBerry* m_geolocationClient;

    HashSet<WebCore::PluginView*> m_pluginViews;

    RefPtr<WebCore::Node> m_inRegionScrollStartingNode;

#if USE(ACCELERATED_COMPOSITING)
    bool m_isAcceleratedCompositingActive;
    OwnPtr<FrameLayers> m_frameLayers; // WebKit thread only.
    OwnPtr<WebPageCompositor> m_compositor; // Compositing thread only.
    OwnPtr<WebCore::Timer<WebPagePrivate> > m_rootLayerCommitTimer;
    bool m_needsOneShotDrawingSynchronization;
    bool m_needsCommit;
    bool m_suspendRootLayerCommit;
#endif

    int m_pendingOrientation;

    RefPtr<WebCore::Node> m_fullscreenVideoNode;
    RefPtr<WebCore::PluginView> m_fullScreenPluginView;

    typedef HashMap<const WebCore::Frame*, BackingStoreClient*> BackingStoreClientForFrameMap;
    BackingStoreClientForFrameMap m_backingStoreClientForFrameMap;

    // WebSettingsDelegate methods.
    virtual void didChangeSettings(WebSettings*);

    RefPtr<WebCore::DOMWrapperWorld> m_isolatedWorld;
    bool m_hasInRegionScrollableAreas;
    bool m_updateDelegatedOverlaysDispatched;
};
}
}

#endif // WebPage_p_h
