/*
 * Copyright (C) 2011 Samsung Electronics
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

// Use this file to assert that various WebKit API enum values continue
// matching WebCore defined enum values.

#include "config.h"

#include "ContextMenuItem.h"
#include "Page.h"
#include "VisibleSelection.h"
#include "ewk_contextmenu.h"
#include "ewk_frame.h"
#include "ewk_view.h"
#include <wtf/Assertions.h>

#if ENABLE(TOUCH_EVENTS)
#include "PlatformTouchEvent.h"
#include "PlatformTouchPoint.h"
#endif

#define COMPILE_ASSERT_MATCHING_ENUM(webkit_name, webcore_name) \
    COMPILE_ASSERT(static_cast<int>(webkit_name) == static_cast<int>(WebCore::webcore_name), mismatching_enums)

#if ENABLE(PAGE_VISIBILITY_API)
COMPILE_ASSERT_MATCHING_ENUM(EWK_PAGE_VISIBILITY_STATE_VISIBLE, PageVisibilityStateVisible);
COMPILE_ASSERT_MATCHING_ENUM(EWK_PAGE_VISIBILITY_STATE_HIDDEN, PageVisibilityStateHidden);
COMPILE_ASSERT_MATCHING_ENUM(EWK_PAGE_VISIBILITY_STATE_PRERENDER, PageVisibilityStatePrerender);
#endif

COMPILE_ASSERT_MATCHING_ENUM(EWK_TEXT_SELECTION_NONE, VisibleSelection::NoSelection);
COMPILE_ASSERT_MATCHING_ENUM(EWK_TEXT_SELECTION_CARET, VisibleSelection::CaretSelection);
COMPILE_ASSERT_MATCHING_ENUM(EWK_TEXT_SELECTION_RANGE, VisibleSelection::RangeSelection);

#if ENABLE(TOUCH_EVENTS)
COMPILE_ASSERT_MATCHING_ENUM(EWK_TOUCH_POINT_RELEASED, PlatformTouchPoint::TouchReleased);
COMPILE_ASSERT_MATCHING_ENUM(EWK_TOUCH_POINT_PRESSED, PlatformTouchPoint::TouchPressed);
COMPILE_ASSERT_MATCHING_ENUM(EWK_TOUCH_POINT_MOVED, PlatformTouchPoint::TouchMoved);
COMPILE_ASSERT_MATCHING_ENUM(EWK_TOUCH_POINT_STATIONARY, PlatformTouchPoint::TouchStationary);
COMPILE_ASSERT_MATCHING_ENUM(EWK_TOUCH_POINT_CANCELLED, PlatformTouchPoint::TouchCancelled);
COMPILE_ASSERT_MATCHING_ENUM(EWK_TOUCH_POINT_END, PlatformTouchPoint::TouchStateEnd);
#endif

COMPILE_ASSERT_MATCHING_ENUM(EWK_VIEW_MODE_INVALID, Page::ViewModeInvalid);
COMPILE_ASSERT_MATCHING_ENUM(EWK_VIEW_MODE_WINDOWED, Page::ViewModeWindowed);
COMPILE_ASSERT_MATCHING_ENUM(EWK_VIEW_MODE_FLOATING, Page::ViewModeFloating);
COMPILE_ASSERT_MATCHING_ENUM(EWK_VIEW_MODE_FULLSCREEN, Page::ViewModeFullscreen);
COMPILE_ASSERT_MATCHING_ENUM(EWK_VIEW_MODE_MAXIMIZED, Page::ViewModeMaximized);
COMPILE_ASSERT_MATCHING_ENUM(EWK_VIEW_MODE_MINIMIZED, Page::ViewModeMinimized);

#if ENABLE(CONTEXT_MENUS)
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_NO_ACTION, ContextMenuItemTagNoAction);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_OPEN_LINK_IN_NEW_WINDOW, ContextMenuItemTagOpenLinkInNewWindow);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_DOWNLOAD_LINK_TO_DISK, ContextMenuItemTagDownloadLinkToDisk);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_COPY_LINK_TO_CLIPBOARD, ContextMenuItemTagCopyLinkToClipboard);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_OPEN_IMAGE_IN_NEW_WINDOW, ContextMenuItemTagOpenImageInNewWindow);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_DOWNLOAD_IMAGE_TO_DISK, ContextMenuItemTagDownloadImageToDisk);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_COPY_IMAGE_TO_CLIPBOARD, ContextMenuItemTagCopyImageToClipboard);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_COPY_IMAGE_URL_TO_CLIPBOARD, ContextMenuItemTagCopyImageUrlToClipboard);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_OPEN_FRAME_IN_NEW_WINDOW, ContextMenuItemTagOpenFrameInNewWindow);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_COPY, ContextMenuItemTagCopy);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_GO_BACK, ContextMenuItemTagGoBack);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_GO_FORWARD, ContextMenuItemTagGoForward);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_STOP, ContextMenuItemTagStop);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_RELOAD, ContextMenuItemTagReload);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_CUT, ContextMenuItemTagCut);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_PASTE, ContextMenuItemTagPaste);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_SELECT_ALL, ContextMenuItemTagSelectAll);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_SPELLING_GUESS, ContextMenuItemTagSpellingGuess);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_NO_GUESSES_FOUND, ContextMenuItemTagNoGuessesFound);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_IGNORE_SPELLING, ContextMenuItemTagIgnoreSpelling);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_LEARN_SPELLING, ContextMenuItemTagLearnSpelling);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_OTHER, ContextMenuItemTagOther);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_SEARCH_IN_SPOTLIGHT, ContextMenuItemTagSearchInSpotlight);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_SEARCH_WEB, ContextMenuItemTagSearchWeb);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_LOOK_UP_IN_DICTIONARY, ContextMenuItemTagLookUpInDictionary);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_OPEN_WITH_DEFAULT_APPLICATION, ContextMenuItemTagOpenWithDefaultApplication);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_PDFACTUAL_SIZE, ContextMenuItemPDFActualSize);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_PDFZOOM_IN, ContextMenuItemPDFZoomIn);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_PDFZOOM_OUT, ContextMenuItemPDFZoomOut);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_PDFAUTO_SIZE, ContextMenuItemPDFAutoSize);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_PDFSINGLE_PAGE, ContextMenuItemPDFSinglePage);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_PDFFACING_PAGES, ContextMenuItemPDFFacingPages);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_PDFCONTINUOUS, ContextMenuItemPDFContinuous);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_PDFNEXT_PAGE, ContextMenuItemPDFNextPage);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_PDFPREVIOUS_PAGE, ContextMenuItemPDFPreviousPage);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_OPEN_LINK, ContextMenuItemTagOpenLink);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_IGNORE_GRAMMAR, ContextMenuItemTagIgnoreGrammar);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_SPELLING_MENU, ContextMenuItemTagSpellingMenu);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_SHOW_SPELLING_PANEL, ContextMenuItemTagShowSpellingPanel);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_CHECK_SPELLING, ContextMenuItemTagCheckSpelling);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_CHECK_SPELLING_WHILE_TYPING, ContextMenuItemTagCheckSpellingWhileTyping);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_CHECK_GRAMMAR_WITH_SPELLING, ContextMenuItemTagCheckGrammarWithSpelling);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_FONT_MENU, ContextMenuItemTagFontMenu);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_SHOW_FONTS, ContextMenuItemTagShowFonts);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_BOLD, ContextMenuItemTagBold);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_ITALIC, ContextMenuItemTagItalic);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_UNDERLINE, ContextMenuItemTagUnderline);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_OUTLINE, ContextMenuItemTagOutline);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_STYLES, ContextMenuItemTagStyles);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_SHOW_COLORS, ContextMenuItemTagShowColors);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_SPEECH_MENU, ContextMenuItemTagSpeechMenu);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_START_SPEAKING, ContextMenuItemTagStartSpeaking);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_STOP_SPEAKING, ContextMenuItemTagStopSpeaking);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_WRITING_DIRECTION_MENU, ContextMenuItemTagWritingDirectionMenu);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_DEFAULT_DIRECTION, ContextMenuItemTagDefaultDirection);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_LEFT_TO_RIGHT, ContextMenuItemTagLeftToRight);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_RIGHT_TO_LEFT, ContextMenuItemTagRightToLeft);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_PDFSINGLE_PAGE_SCROLLING, ContextMenuItemTagPDFSinglePageScrolling);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_PDFFACING_PAGES_SCROLLING, ContextMenuItemTagPDFFacingPagesScrolling);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_INSPECT_ELEMENT, ContextMenuItemTagInspectElement);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_TEXT_DIRECTION_MENU, ContextMenuItemTagTextDirectionMenu);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_TEXT_DIRECTION_DEFAULT, ContextMenuItemTagTextDirectionDefault);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_TEXT_DIRECTION_LEFT_TO_RIGHT, ContextMenuItemTagTextDirectionLeftToRight);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_TEXT_DIRECTION_RIGHT_TO_LEFT, ContextMenuItemTagTextDirectionRightToLeft);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_OPEN_MEDIA_IN_NEW_WINDOW, ContextMenuItemTagOpenMediaInNewWindow);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_COPY_MEDIA_LINK_TO_CLIPBOARD, ContextMenuItemTagCopyMediaLinkToClipboard);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_TOGGLE_MEDIA_CONTROLS, ContextMenuItemTagToggleMediaControls);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_TOGGLE_MEDIA_LOOP, ContextMenuItemTagToggleMediaLoop);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_ENTER_VIDEO_FULLSCREEN, ContextMenuItemTagEnterVideoFullscreen);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_MEDIA_PLAY_PAUSE, ContextMenuItemTagMediaPlayPause);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_TAG_MEDIA_MUTE, ContextMenuItemTagMediaMute);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_BASE_CUSTOM_TAG, ContextMenuItemBaseCustomTag);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_CUSTOM_TAG_NO_ACTION, ContextMenuItemCustomTagNoAction);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_LAST_CUSTOM_TAG, ContextMenuItemLastCustomTag);
COMPILE_ASSERT_MATCHING_ENUM(EWK_CONTEXT_MENU_ITEM_BASE_APPLICATION_TAG, ContextMenuItemBaseApplicationTag);
#endif
