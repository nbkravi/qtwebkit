/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "WebRenderObject.h"

#include "WebPage.h"
#include <WebCore/Frame.h>
#include <WebCore/FrameLoaderClient.h>
#include <WebCore/RenderText.h>
#include <WebCore/RenderView.h>
#include <WebCore/RenderWidget.h>

using namespace WebCore;

namespace WebKit {

PassRefPtr<WebRenderObject> WebRenderObject::create(WebPage* page)
{
    Frame* mainFrame = page->mainFrame();
    if (!mainFrame)
        return 0;

    if (!mainFrame->loader()->client()->hasHTMLView())
        return 0;

    RenderView* contentRenderer = mainFrame->contentRenderer();
    if (!contentRenderer)
        return 0;

    return adoptRef(new WebRenderObject(contentRenderer));
}

WebRenderObject::WebRenderObject(RenderObject* renderer)
{
    m_name = renderer->renderName();

    // FIXME: broken with transforms
    m_absolutePosition = flooredIntPoint(renderer->localToAbsolute(FloatPoint()));

    if (renderer->isBox())
        m_frameRect = toRenderBox(renderer)->frameRect();
    else if (renderer->isText()) {
        m_frameRect = toRenderText(renderer)->linesBoundingBox();
        m_frameRect.setX(toRenderText(renderer)->firstRunX());
        m_frameRect.setY(toRenderText(renderer)->firstRunY());
    } else if (renderer->isRenderInline())
        m_frameRect = toRenderBoxModelObject(renderer)->borderBoundingBox();

    m_children = MutableArray::create();
    for (RenderObject* coreChild = renderer->firstChild(); coreChild; coreChild = coreChild->nextSibling()) {
        RefPtr<WebRenderObject> child = adoptRef(new WebRenderObject(coreChild));
        m_children->append(child.get());
    }

    if (!renderer->isWidget())
        return;

    Widget* widget = toRenderWidget(renderer)->widget();
    if (!widget || !widget->isFrameView())
        return;

    FrameView* frameView = static_cast<FrameView*>(widget);
    if (RenderView* coreContentRenderer = frameView->frame()->contentRenderer()) {
        RefPtr<WebRenderObject> contentRenderer = adoptRef(new WebRenderObject(coreContentRenderer));
        m_children->append(contentRenderer.get());
    }
}

} // namespace WebKit
