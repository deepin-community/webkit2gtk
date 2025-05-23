/*
 * Copyright (C) 2015-2023 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "RenderStyleConstants.h"

#include "CSSPrimitiveValueMappings.h"
#include "TabSize.h"
#include <wtf/text/TextStream.h>

namespace WebCore {


bool alwaysPageBreak(BreakBetween between)
{
    return between >= BreakBetween::Page;
}

CSSBoxType transformBoxToCSSBoxType(TransformBox transformBox)
{
    switch (transformBox) {
    case TransformBox::StrokeBox:
        return CSSBoxType::StrokeBox;
    case TransformBox::ContentBox:
        return CSSBoxType::ContentBox;
    case TransformBox::BorderBox:
        return CSSBoxType::BorderBox;
    case TransformBox::FillBox:
        return CSSBoxType::FillBox;
    case TransformBox::ViewBox:
        return CSSBoxType::ViewBox;
    default:
        ASSERT_NOT_REACHED();
        return CSSBoxType::BorderBox;
    }
}

TextStream& operator<<(TextStream& ts, AnimationFillMode fillMode)
{
    switch (fillMode) {
    case AnimationFillMode::None: ts << "none"; break;
    case AnimationFillMode::Forwards: ts << "forwards"; break;
    case AnimationFillMode::Backwards: ts << "backwards"; break;
    case AnimationFillMode::Both: ts << "both"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, AnimationPlayState playState)
{
    switch (playState) {
    case AnimationPlayState::Playing: ts << "playing"; break;
    case AnimationPlayState::Paused: ts << "paused"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, AspectRatioType aspectRatioType)
{
    switch (aspectRatioType) {
    case AspectRatioType::Auto: ts << "auto"; break;
    case AspectRatioType::Ratio: ts << "ratio"; break;
    case AspectRatioType::AutoAndRatio: ts << "autoandratio"; break;
    case AspectRatioType::AutoZero: ts << "autozero"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, AutoRepeatType repeatType)
{
    switch (repeatType) {
    case AutoRepeatType::None: ts << "none"; break;
    case AutoRepeatType::Fill: ts << "fill"; break;
    case AutoRepeatType::Fit: ts << "fit"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BackfaceVisibility visibility)
{
    switch (visibility) {
    case BackfaceVisibility::Visible: ts << "visible"; break;
    case BackfaceVisibility::Hidden: ts << "hidden"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BlockStepAlign blockStepAlign)
{
    switch (blockStepAlign) {
    case BlockStepAlign::Auto: ts << "auto"; break;
    case BlockStepAlign::Center: ts << "center"; break;
    case BlockStepAlign::Start: ts << "start"; break;
    case BlockStepAlign::End: ts << "end"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BlockStepInsert blockStepInsert)
{
    switch (blockStepInsert) {
    case BlockStepInsert::MarginBox: ts << "margin-box"; break;
    case BlockStepInsert::PaddingBox: ts << "padding-box"; break;
    case BlockStepInsert::ContentBox: ts << "content-box"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BlockStepRound blockStepRound)
{
    switch (blockStepRound) {
    case BlockStepRound::Up: ts << "up"; break;
    case BlockStepRound::Down: ts << "down"; break;
    case BlockStepRound::Nearest: ts << "nearest"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BorderCollapse collapse)
{
    switch (collapse) {
    case BorderCollapse::Separate: ts << "separate"; break;
    case BorderCollapse::Collapse: ts << "collapse"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BorderStyle borderStyle)
{
    switch (borderStyle) {
    case BorderStyle::None: ts << "none"; break;
    case BorderStyle::Hidden: ts << "hidden"; break;
    case BorderStyle::Inset: ts << "inset"; break;
    case BorderStyle::Groove: ts << "groove"; break;
    case BorderStyle::Outset: ts << "outset"; break;
    case BorderStyle::Ridge: ts << "ridge"; break;
    case BorderStyle::Dotted: ts << "dotted"; break;
    case BorderStyle::Dashed: ts << "dashed"; break;
    case BorderStyle::Solid: ts << "solid"; break;
    case BorderStyle::Double: ts << "double"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BoxAlignment boxAlignment)
{
    switch (boxAlignment) {
    case BoxAlignment::Stretch: ts << "stretch"; break;
    case BoxAlignment::Start: ts << "start"; break;
    case BoxAlignment::Center: ts << "center"; break;
    case BoxAlignment::End: ts << "end"; break;
    case BoxAlignment::Baseline: ts << "baseline"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BoxDecorationBreak decorationBreak)
{
    switch (decorationBreak) {
    case BoxDecorationBreak::Slice: ts << "slice"; break;
    case BoxDecorationBreak::Clone: ts << "clone"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BoxDirection boxDirection)
{
    switch (boxDirection) {
    case BoxDirection::Normal: ts << "normal"; break;
    case BoxDirection::Reverse: ts << "reverse"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BoxLines boxLines)
{
    switch (boxLines) {
    case BoxLines::Single: ts << "single"; break;
    case BoxLines::Multiple: ts << "multiple"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BoxOrient boxOrient)
{
    switch (boxOrient) {
    case BoxOrient::Horizontal: ts << "horizontal"; break;
    case BoxOrient::Vertical: ts << "vertical"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BoxPack boxPack)
{
    switch (boxPack) {
    case BoxPack::Start: ts << "start"; break;
    case BoxPack::Center: ts << "center"; break;
    case BoxPack::End: ts << "end"; break;
    case BoxPack::Justify: ts << "justify"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BoxSizing boxSizing)
{
    switch (boxSizing) {
    case BoxSizing::ContentBox: ts << "content-box"; break;
    case BoxSizing::BorderBox: ts << "border-box"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BreakBetween breakBetween)
{
    switch (breakBetween) {
    case BreakBetween::Auto: ts << "auto"; break;
    case BreakBetween::Avoid: ts << "avoid"; break;
    case BreakBetween::AvoidColumn: ts << "avoid-column"; break;
    case BreakBetween::AvoidPage: ts << "avoid-page"; break;
    case BreakBetween::Column: ts << "column"; break;
    case BreakBetween::Page: ts << "page"; break;
    case BreakBetween::LeftPage: ts << "left-page"; break;
    case BreakBetween::RightPage: ts << "right-page"; break;
    case BreakBetween::RectoPage: ts << "recto-page"; break;
    case BreakBetween::VersoPage: ts << "verso-page"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, BreakInside breakInside)
{
    switch (breakInside) {
    case BreakInside::Auto: ts << "auto"; break;
    case BreakInside::Avoid: ts << "avoid"; break;
    case BreakInside::AvoidColumn: ts << "avoidColumn"; break;
    case BreakInside::AvoidPage: ts << "avoidPage"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, CSSBoxType boxType)
{
    switch (boxType) {
    case CSSBoxType::BoxMissing: ts << "missing"; break;
    case CSSBoxType::MarginBox: ts << "margin-box"; break;
    case CSSBoxType::BorderBox: ts << "border-box"; break;
    case CSSBoxType::PaddingBox: ts << "padding-box"; break;
    case CSSBoxType::ContentBox: ts << "content-box"; break;
    case CSSBoxType::FillBox: ts << "fill-box"; break;
    case CSSBoxType::StrokeBox: ts << "stroke-box"; break;
    case CSSBoxType::ViewBox: ts << "view-box"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, CaptionSide side)
{
    switch (side) {
    case CaptionSide::Top: ts << "top"; break;
    case CaptionSide::Bottom: ts << "bottom"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, Clear clear)
{
    switch (clear) {
    case Clear::None: ts << "none"; break;
    case Clear::Left: ts << "left"; break;
    case Clear::Right: ts << "right"; break;
    case Clear::InlineStart : ts << "inline-start"; break;
    case Clear::InlineEnd : ts << "inline-end"; break;
    case Clear::Both: ts << "both"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, UsedClear clear)
{
    switch (clear) {
    case UsedClear::None: ts << "none"; break;
    case UsedClear::Left: ts << "left"; break;
    case UsedClear::Right: ts << "right"; break;
    case UsedClear::Both: ts << "both"; break;
    }
    return ts;
}

#if ENABLE(DARK_MODE_CSS)
TextStream& operator<<(TextStream& ts, ColorScheme colorScheme)
{
    switch (colorScheme) {
    case ColorScheme::Light: ts << "light"; break;
    case ColorScheme::Dark: ts << "dark"; break;
    }
    return ts;
}
#endif

TextStream& operator<<(TextStream& ts, ColumnAxis axis)
{
    switch (axis) {
    case ColumnAxis::Horizontal: ts << "horizontal"; break;
    case ColumnAxis::Vertical: ts << "vertical"; break;
    case ColumnAxis::Auto: ts << "auto"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ColumnFill fill)
{
    switch (fill) {
    case ColumnFill::Auto: ts << "auto"; break;
    case ColumnFill::Balance: ts << "balance"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ColumnProgression progression)
{
    switch (progression) {
    case ColumnProgression::Normal: ts << "normal"; break;
    case ColumnProgression::Reverse: ts << "reverse"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ColumnSpan span)
{
    switch (span) {
    case ColumnSpan::None: ts << "none"; break;
    case ColumnSpan::All: ts << "all"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ContentDistribution distribution)
{
    switch (distribution) {
    case ContentDistribution::Default: ts << "default"; break;
    case ContentDistribution::SpaceBetween: ts << "space-between"; break;
    case ContentDistribution::SpaceAround: ts << "space-around"; break;
    case ContentDistribution::SpaceEvenly: ts << "space-evenly"; break;
    case ContentDistribution::Stretch: ts << "stretch"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ContentPosition position)
{
    switch (position) {
    case ContentPosition::Normal: ts << "normal"; break;
    case ContentPosition::Baseline: ts << "baseline"; break;
    case ContentPosition::LastBaseline: ts << "last-baseline"; break;
    case ContentPosition::Center: ts << "center"; break;
    case ContentPosition::Start: ts << "start"; break;
    case ContentPosition::End: ts << "end"; break;
    case ContentPosition::FlexStart: ts << "flex-start"; break;
    case ContentPosition::FlexEnd: ts << "flex-end"; break;
    case ContentPosition::Left: ts << "left"; break;
    case ContentPosition::Right: ts << "right"; break;

    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ContentVisibility contentVisibility)
{
    switch (contentVisibility) {
    case ContentVisibility::Visible: ts << "visible"; break;
    case ContentVisibility::Auto: ts << "auto"; break;
    case ContentVisibility::Hidden: ts << "hidden"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, CursorType cursor)
{
    switch (cursor) {
    case CursorType::Auto: ts << "auto"; break;
    case CursorType::Default: ts << "default"; break;
    case CursorType::ContextMenu: ts << "contextmenu"; break;
    case CursorType::Help: ts << "help"; break;
    case CursorType::Pointer: ts << "pointer"; break;
    case CursorType::Progress: ts << "progress"; break;
    case CursorType::Wait: ts << "wait"; break;
    case CursorType::Cell: ts << "cell"; break;
    case CursorType::Crosshair: ts << "crosshair"; break;
    case CursorType::Text: ts << "text"; break;
    case CursorType::VerticalText: ts << "vertical-text"; break;
    case CursorType::Alias: ts << "alias"; break;
    case CursorType::Move: ts << "move"; break;
    case CursorType::NoDrop: ts << "nodrop"; break;
    case CursorType::NotAllowed: ts << "not-allowed"; break;
    case CursorType::Grab: ts << "grab"; break;
    case CursorType::Grabbing: ts << "grabbing"; break;
    case CursorType::EResize: ts << "e-resize"; break;
    case CursorType::NResize: ts << "n-resize"; break;
    case CursorType::NEResize: ts << "ne-resize"; break;
    case CursorType::NWResize: ts << "nw-resize"; break;
    case CursorType::SResize: ts << "sr-esize"; break;
    case CursorType::SEResize: ts << "se-resize"; break;
    case CursorType::SWResize: ts << "sw-resize"; break;
    case CursorType::WResize: ts << "w-resize"; break;
    case CursorType::EWResize: ts << "ew-resize"; break;
    case CursorType::NSResize: ts << "ns-resize"; break;
    case CursorType::NESWResize: ts << "nesw-resize"; break;
    case CursorType::NWSEResize: ts << "nwse-resize"; break;
    case CursorType::ColumnResize: ts << "column-resize"; break;
    case CursorType::RowResize: ts << "row-resize"; break;
    case CursorType::AllScroll: ts << "all-scroll"; break;
    case CursorType::ZoomIn: ts << "zoom-in"; break;
    case CursorType::ZoomOut: ts << "zoom-out"; break;
    case CursorType::Copy: ts << "copy"; break;
    case CursorType::None: ts << "none"; break;
    }
    return ts;
}

#if ENABLE(CURSOR_VISIBILITY)
TextStream& operator<<(TextStream& ts, CursorVisibility visibility)
{
    switch (visibility) {
    case CursorVisibility::Auto: ts << "auto"; break;
    case CursorVisibility::AutoHide: ts << "autohide"; break;
    }
    return ts;
}
#endif

TextStream& operator<<(TextStream& ts, DisplayType display)
{
    switch (display) {
    case DisplayType::Inline: ts << "inline"; break;
    case DisplayType::Block: ts << "block"; break;
    case DisplayType::ListItem: ts << "list-item"; break;
    case DisplayType::InlineBlock: ts << "inline-block"; break;
    case DisplayType::Table: ts << "table"; break;
    case DisplayType::InlineTable: ts << "inline-table"; break;
    case DisplayType::TableRowGroup: ts << "table-row-group"; break;
    case DisplayType::TableHeaderGroup: ts << "table-header-group"; break;
    case DisplayType::TableFooterGroup: ts << "table-footer-group"; break;
    case DisplayType::TableRow: ts << "table-row"; break;
    case DisplayType::TableColumnGroup: ts << "table-column-group"; break;
    case DisplayType::TableColumn: ts << "table-column"; break;
    case DisplayType::TableCell: ts << "table-cell"; break;
    case DisplayType::TableCaption: ts << "table-caption"; break;
    case DisplayType::Box: ts << "box"; break;
    case DisplayType::InlineBox: ts << "inline-box"; break;
    case DisplayType::Flex: ts << "flex"; break;
    case DisplayType::InlineFlex: ts << "inline-flex"; break;
    case DisplayType::Contents: ts << "contents"; break;
    case DisplayType::Grid: ts << "grid"; break;
    case DisplayType::InlineGrid: ts << "inline-grid"; break;
    case DisplayType::FlowRoot: ts << "flow-root"; break;
    case DisplayType::Ruby: ts << "ruby"; break;
    case DisplayType::RubyBlock: ts << "block ruby"; break;
    case DisplayType::RubyBase: ts << "ruby-base"; break;
    case DisplayType::RubyAnnotation: ts << "ruby-text"; break;
    case DisplayType::None: ts << "none"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, Edge edge)
{
    switch (edge) {
    case Edge::Top: ts << "top"; break;
    case Edge::Right: ts << "right"; break;
    case Edge::Bottom: ts << "bottom"; break;
    case Edge::Left: ts << "left"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, EmptyCell emptyCell)
{
    switch (emptyCell) {
    case EmptyCell::Show: ts << "show"; break;
    case EmptyCell::Hide: ts << "hide"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, EventListenerRegionType listenerType)
{
    switch (listenerType) {
    case EventListenerRegionType::Wheel: ts << "wheel"; break;
    case EventListenerRegionType::NonPassiveWheel: ts << "active wheel"; break;
    case EventListenerRegionType::MouseClick: ts << "mouse click"; break;
    case EventListenerRegionType::TouchStart: ts << "touch start"; break;
    case EventListenerRegionType::NonPassiveTouchStart: ts << "active touch start"; break;
    case EventListenerRegionType::TouchEnd: ts << "touch end"; break;
    case EventListenerRegionType::NonPassiveTouchEnd: ts << "active touch end"; break;
    case EventListenerRegionType::TouchCancel: ts << "touch cancel"; break;
    case EventListenerRegionType::NonPassiveTouchCancel: ts << "active touch cancel"; break;
    case EventListenerRegionType::TouchMove: ts << "touch move"; break;
    case EventListenerRegionType::NonPassiveTouchMove: ts << "active touch move"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, FieldSizing sizing)
{
    switch (sizing) {
    case FieldSizing::Fixed: ts << "fixed"; break;
    case FieldSizing::Content: ts << "content"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, FillAttachment attachment)
{
    switch (attachment) {
    case FillAttachment::ScrollBackground: ts << "scroll"; break;
    case FillAttachment::LocalBackground: ts << "local"; break;
    case FillAttachment::FixedBackground: ts << "fixed"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, FillBox fill)
{
    switch (fill) {
    case FillBox::BorderBox: ts << "border-box"; break;
    case FillBox::PaddingBox: ts << "padding-box"; break;
    case FillBox::ContentBox: ts << "content-box"; break;
    case FillBox::BorderArea: ts << "border-area"; break;
    case FillBox::Text: ts << "text"; break;
    case FillBox::NoClip: ts << "no-clip"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, FillRepeat repeat)
{
    switch (repeat) {
    case FillRepeat::Repeat: ts << "repeat"; break;
    case FillRepeat::NoRepeat: ts << "no-repeat"; break;
    case FillRepeat::Round: ts << "round"; break;
    case FillRepeat::Space: ts << "space"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, FillSizeType sizeType)
{
    switch (sizeType) {
    case FillSizeType::Contain: ts << "contain"; break;
    case FillSizeType::Cover: ts << "cover"; break;
    case FillSizeType::Size: ts << "size-length"; break;
    case FillSizeType::None: ts << "size-none"; break;
    }
    
    return ts;
}

TextStream& operator<<(TextStream& ts, FlexDirection flexDirection)
{
    switch (flexDirection) {
    case FlexDirection::Row: ts << "row"; break;
    case FlexDirection::RowReverse: ts << "row-reverse"; break;
    case FlexDirection::Column: ts << "column"; break;
    case FlexDirection::ColumnReverse: ts << "column-reverse"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, FlexWrap flexWrap)
{
    switch (flexWrap) {
    case FlexWrap::NoWrap: ts << "no-wrap"; break;
    case FlexWrap::Wrap: ts << "wrap"; break;
    case FlexWrap::Reverse: ts << "reverse"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, Float floating)
{
    switch (floating) {
    case Float::None: ts << "none"; break;
    case Float::Left: ts << "left"; break;
    case Float::Right: ts << "right"; break;
    case Float::InlineStart: ts << "inline-start"; break;
    case Float::InlineEnd: ts << "inline-end"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, UsedFloat floating)
{
    switch (floating) {
    case UsedFloat::None: ts << "none"; break;
    case UsedFloat::Left: ts << "left"; break;
    case UsedFloat::Right: ts << "right"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, GridAutoFlow gridAutoFlow)
{
    switch (gridAutoFlow) {
    case AutoFlowRow: ts << "row"; break;
    case AutoFlowColumn: ts << "column"; break;
    case AutoFlowRowDense: ts << "row-dense"; break;
    case AutoFlowColumnDense: ts << "column-dense"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, HangingPunctuation punctuation)
{
    switch (punctuation) {
    case HangingPunctuation::First: ts << "first"; break;
    case HangingPunctuation::Last: ts << "last"; break;
    case HangingPunctuation::AllowEnd: ts << "allow-end"; break;
    case HangingPunctuation::ForceEnd: ts << "force-end"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, Hyphens hyphens)
{
    switch (hyphens) {
    case Hyphens::None: ts << "none"; break;
    case Hyphens::Manual: ts << "manual"; break;
    case Hyphens::Auto: ts << "auto"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ImageRendering imageRendering)
{
    switch (imageRendering) {
    case ImageRendering::Auto: ts << "auto"; break;
    case ImageRendering::OptimizeSpeed: ts << "optimizeSpeed"; break;
    case ImageRendering::OptimizeQuality: ts << "optimizeQuality"; break;
    case ImageRendering::CrispEdges: ts << "crispEdges"; break;
    case ImageRendering::Pixelated: ts << "pixelated"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, InsideLink inside)
{
    switch (inside) {
    case InsideLink::NotInside: ts << "not-inside"; break;
    case InsideLink::InsideUnvisited: ts << "inside-unvisited"; break;
    case InsideLink::InsideVisited: ts << "inside-visited"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, Isolation isolation)
{
    switch (isolation) {
    case Isolation::Auto: ts << "auto"; break;
    case Isolation::Isolate: ts << "isolate"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ItemPosition position)
{
    switch (position) {
    case ItemPosition::Legacy: ts << "legacy"; break;
    case ItemPosition::Auto: ts << "auto"; break;
    case ItemPosition::Normal: ts << "normal"; break;
    case ItemPosition::Stretch: ts << "stretch"; break;
    case ItemPosition::Baseline: ts << "baseline"; break;
    case ItemPosition::LastBaseline: ts << "last-baseline"; break;
    case ItemPosition::Center: ts << "center"; break;
    case ItemPosition::Start: ts << "start"; break;
    case ItemPosition::End: ts << "end"; break;
    case ItemPosition::SelfStart: ts << "self-start"; break;
    case ItemPosition::SelfEnd: ts << "self-end"; break;
    case ItemPosition::FlexStart: ts << "flex-start"; break;
    case ItemPosition::FlexEnd: ts << "flex-end"; break;
    case ItemPosition::Left: ts << "left"; break;
    case ItemPosition::Right: ts << "right"; break;
    case ItemPosition::AnchorCenter: ts << "anchor-center"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ItemPositionType positionType)
{
    switch (positionType) {
    case ItemPositionType::NonLegacy: ts << "non-legacy"; break;
    case ItemPositionType::Legacy: ts << "legacy"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, LineAlign align)
{
    switch (align) {
    case LineAlign::None: ts << "none"; break;
    case LineAlign::Edges: ts << "edges"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, LineBreak lineBreak)
{
    switch (lineBreak) {
    case LineBreak::Auto: ts << "auto"; break;
    case LineBreak::Loose: ts << "loose"; break;
    case LineBreak::Normal: ts << "normal"; break;
    case LineBreak::Strict: ts << "strict"; break;
    case LineBreak::AfterWhiteSpace: ts << "after-whiteSpace"; break;
    case LineBreak::Anywhere: ts << "anywhere"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, LineSnap lineSnap)
{
    switch (lineSnap) {
    case LineSnap::None: ts << "none"; break;
    case LineSnap::Baseline: ts << "baseline"; break;
    case LineSnap::Contain: ts << "contain"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ListStylePosition position)
{
    switch (position) {
    case ListStylePosition::Outside: ts << "outside"; break;
    case ListStylePosition::Inside: ts << "inside"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, MarginTrimType marginTrimType)
{
    switch (marginTrimType) {
    case MarginTrimType::BlockStart: ts << "block-start"; break;
    case MarginTrimType::BlockEnd: ts << "block-end"; break;
    case MarginTrimType::InlineStart: ts << "inline-start"; break;
    case MarginTrimType::InlineEnd: ts << "inline-end"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, MarqueeBehavior marqueeBehavior)
{
    switch (marqueeBehavior) {
    case MarqueeBehavior::None: ts << "none"; break;
    case MarqueeBehavior::Scroll: ts << "scroll"; break;
    case MarqueeBehavior::Slide: ts << "slide"; break;
    case MarqueeBehavior::Alternate: ts << "alternate"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, MarqueeDirection marqueeDirection)
{
    switch (marqueeDirection) {
    case MarqueeDirection::Auto: ts << "auto"; break;
    case MarqueeDirection::Left: ts << "left"; break;
    case MarqueeDirection::Right: ts << "right"; break;
    case MarqueeDirection::Up: ts << "up"; break;
    case MarqueeDirection::Down: ts << "down"; break;
    case MarqueeDirection::Forward: ts << "forward"; break;
    case MarqueeDirection::Backward: ts << "backward"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, MaskMode maskMode)
{
    switch (maskMode) {
    case MaskMode::Alpha: ts << "alpha"; break;
    case MaskMode::Luminance: ts << "luminance"; break;
    case MaskMode::MatchSource: ts << "match-source"; break;
    }

    return ts;
}

TextStream& operator<<(TextStream& ts, NBSPMode mode)
{
    switch (mode) {
    case NBSPMode::Normal: ts << "normal"; break;
    case NBSPMode::Space: ts << "space"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ObjectFit objectFit)
{
    switch (objectFit) {
    case ObjectFit::Fill: ts << "fill"; break;
    case ObjectFit::Contain: ts << "contain"; break;
    case ObjectFit::Cover: ts << "cover"; break;
    case ObjectFit::None: ts << "none"; break;
    case ObjectFit::ScaleDown: ts << "scale-down"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, Order order)
{
    switch (order) {
    case Order::Logical: ts << "logical"; break;
    case Order::Visual: ts << "visual"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, Overflow overflow)
{
    switch (overflow) {
    case Overflow::Visible: ts << "visible"; break;
    case Overflow::Hidden: ts << "hidden"; break;
    case Overflow::Scroll: ts << "scroll"; break;
    case Overflow::Auto: ts << "auto"; break;
    case Overflow::PagedX: ts << "paged-x"; break;
    case Overflow::PagedY: ts << "paged-y"; break;
    case Overflow::Clip: ts << "clip"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, OverflowAlignment alignment)
{
    switch (alignment) {
    case OverflowAlignment::Default: ts << "default"; break;
    case OverflowAlignment::Unsafe: ts << "unsafe"; break;
    case OverflowAlignment::Safe: ts << "safe"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, OverflowWrap overflowWrap)
{
    switch (overflowWrap) {
    case OverflowWrap::Normal: ts << "normal"; break;
    case OverflowWrap::BreakWord: ts << "break-word"; break;
    case OverflowWrap::Anywhere: ts << "anywhere"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, PaintOrder paintOrder)
{
    switch (paintOrder) {
    case PaintOrder::Normal: ts << "normal"; break;
    case PaintOrder::Fill: ts << "fill"; break;
    case PaintOrder::FillMarkers: ts << "fill markers"; break;
    case PaintOrder::Stroke: ts << "stroke"; break;
    case PaintOrder::StrokeMarkers: ts << "stroke markers"; break;
    case PaintOrder::Markers: ts << "markers"; break;
    case PaintOrder::MarkersStroke: ts << "markers stroke"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, PointerEvents pointerEvents)
{
    switch (pointerEvents) {
    case PointerEvents::None: ts << "none"; break;
    case PointerEvents::Auto: ts << "auto"; break;
    case PointerEvents::Stroke: ts << "stroke"; break;
    case PointerEvents::Fill: ts << "fill"; break;
    case PointerEvents::Painted: ts << "painted"; break;
    case PointerEvents::Visible: ts << "visible"; break;
    case PointerEvents::BoundingBox: ts << "bounding-box"; break;
    case PointerEvents::VisibleStroke: ts << "visible-stroke"; break;
    case PointerEvents::VisibleFill: ts << "visible-fill"; break;
    case PointerEvents::VisiblePainted: ts << "visible-painted"; break;
    case PointerEvents::All: ts << "all"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, PositionType position)
{
    switch (position) {
    case PositionType::Static: ts << "static"; break;
    case PositionType::Relative: ts << "relative"; break;
    case PositionType::Absolute: ts << "absolute"; break;
    case PositionType::Sticky: ts << "sticky"; break;
    case PositionType::Fixed: ts << "fixed"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, PrintColorAdjust colorAdjust)
{
    switch (colorAdjust) {
    case PrintColorAdjust::Economy: ts << "economy"; break;
    case PrintColorAdjust::Exact: ts << "exact"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, PseudoId pseudoId)
{
    switch (pseudoId) {
    case PseudoId::None: ts << "none"; break;
    case PseudoId::FirstLine: ts << "first-line"; break;
    case PseudoId::FirstLetter: ts << "first-letter"; break;
    case PseudoId::GrammarError: ts << "grammar-error"; break;
    case PseudoId::Highlight: ts << "highlight"; break;
    case PseudoId::InternalWritingSuggestions: ts << "-internal-writing-suggestions"; break;
    case PseudoId::Marker: ts << "marker"; break;
    case PseudoId::Backdrop: ts << "backdrop"; break;
    case PseudoId::Before: ts << "before"; break;
    case PseudoId::After: ts << "after"; break;
    case PseudoId::Selection: ts << "selection"; break;
    case PseudoId::SpellingError: ts << "spelling-error"; break;
    case PseudoId::TargetText: ts << "target-text"; break;
    case PseudoId::ViewTransition: ts << "view-transition"; break;
    case PseudoId::ViewTransitionGroup: ts << "view-transition-group"; break;
    case PseudoId::ViewTransitionImagePair: ts << "view-transition-image-pair"; break;
    case PseudoId::ViewTransitionOld: ts << "view-transition-old"; break;
    case PseudoId::ViewTransitionNew: ts << "view-transition-new"; break;
    case PseudoId::WebKitResizer: ts << "-webkit-resizer"; break;
    case PseudoId::WebKitScrollbar: ts << "-webkit-scrollbar"; break;
    case PseudoId::WebKitScrollbarThumb: ts << "-webkit-scrollbar-thumb"; break;
    case PseudoId::WebKitScrollbarButton: ts << "-webkit-scrollbar-button"; break;
    case PseudoId::WebKitScrollbarTrack: ts << "-webkit-scrollbar-track"; break;
    case PseudoId::WebKitScrollbarTrackPiece: ts << "-webkit-scrollbar-trackpiece"; break;
    case PseudoId::WebKitScrollbarCorner: ts << "-webkit-scrollbar-corner"; break;
    default:
        ts << "other";
        break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, QuoteType quoteType)
{
    switch (quoteType) {
    case QuoteType::OpenQuote: ts << "open"; break;
    case QuoteType::CloseQuote: ts << "close"; break;
    case QuoteType::NoOpenQuote: ts << "no-open"; break;
    case QuoteType::NoCloseQuote: ts << "no-close"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ReflectionDirection direction)
{
    switch (direction) {
    case ReflectionDirection::Below: ts << "below"; break;
    case ReflectionDirection::Above: ts << "above"; break;
    case ReflectionDirection::Left: ts << "left"; break;
    case ReflectionDirection::Right: ts << "right"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, Resize resize)
{
    switch (resize) {
    case Resize::None: ts << "none"; break;
    case Resize::Both: ts << "both"; break;
    case Resize::Horizontal: ts << "horizontal"; break;
    case Resize::Vertical: ts << "vertical"; break;
    case Resize::Block: ts << "block"; break;
    case Resize::Inline: ts << "inline"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, RubyPosition position)
{
    switch (position) {
    case RubyPosition::Over: ts << "over"; break;
    case RubyPosition::Under: ts << "under"; break;
    case RubyPosition::InterCharacter: ts << "inter-character"; break;
    case RubyPosition::LegacyInterCharacter: ts << "legacy inter-character"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, RubyAlign alignment)
{
    switch (alignment) {
    case RubyAlign::Start: ts << "start"; break;
    case RubyAlign::Center: ts << "center"; break;
    case RubyAlign::SpaceBetween: ts << "space-between"; break;
    case RubyAlign::SpaceAround: ts << "space-around"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, RubyOverhang overhang)
{
    switch (overhang) {
    case RubyOverhang::Auto: ts << "auto"; break;
    case RubyOverhang::None: ts << "none"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ScrollSnapAxis axis)
{
    switch (axis) {
    case ScrollSnapAxis::XAxis: ts << "x-axis"; break;
    case ScrollSnapAxis::YAxis: ts << "y-Axis"; break;
    case ScrollSnapAxis::Block: ts << "block"; break;
    case ScrollSnapAxis::Inline: ts << "inline"; break;
    case ScrollSnapAxis::Both: ts << "both"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ScrollSnapAxisAlignType alignType)
{
    switch (alignType) {
    case ScrollSnapAxisAlignType::None: ts << "none"; break;
    case ScrollSnapAxisAlignType::Start: ts << "start"; break;
    case ScrollSnapAxisAlignType::Center: ts << "center"; break;
    case ScrollSnapAxisAlignType::End: ts << "end"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ScrollSnapStrictness strictness)
{
    switch (strictness) {
    case ScrollSnapStrictness::None: ts << "none"; break;
    case ScrollSnapStrictness::Proximity: ts << "proximity"; break;
    case ScrollSnapStrictness::Mandatory: ts << "mandatory"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ScrollSnapStop stop)
{
    switch (stop) {
    case ScrollSnapStop::Normal: ts << "normal"; break;
    case ScrollSnapStop::Always: ts << "always"; break;
    }
    return ts;
}
TextStream& operator<<(TextStream& ts, SpeakAs speakAs)
{
    switch (speakAs) {
    case SpeakAs::SpellOut: ts << "spell-out"; break;
    case SpeakAs::Digits: ts << "digits"; break;
    case SpeakAs::LiteralPunctuation: ts << "literal-punctuation"; break;
    case SpeakAs::NoPunctuation: ts << "no-punctuation"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, StyleDifference diff)
{
    switch (diff) {
    case StyleDifference::Equal: ts << "equal"; break;
    case StyleDifference::RecompositeLayer: ts << "recomposite layer"; break;
    case StyleDifference::Repaint: ts << "repaint"; break;
    case StyleDifference::RepaintIfText: ts << "repaint if text"; break;
    case StyleDifference::RepaintLayer: ts << "repaint layer"; break;
    case StyleDifference::LayoutPositionedMovementOnly: ts << "layout positioned movement only"; break;
    case StyleDifference::Overflow: ts << "overflow"; break;
    case StyleDifference::OverflowAndPositionedMovement: ts << "overflow and positioned movement"; break;
    case StyleDifference::Layout: ts << "layout"; break;
    case StyleDifference::NewStyle: ts << "new style"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TableLayoutType layoutType)
{
    switch (layoutType) {
    case TableLayoutType::Auto: ts << "Auto"; break;
    case TableLayoutType::Fixed: ts << "Fixed"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextAlignMode alignMode)
{
    switch (alignMode) {
    case TextAlignMode::Left: ts << "left"; break;
    case TextAlignMode::Right: ts << "right"; break;
    case TextAlignMode::Center: ts << "center"; break;
    case TextAlignMode::Justify: ts << "justify"; break;
    case TextAlignMode::WebKitLeft: ts << "webkit-left"; break;
    case TextAlignMode::WebKitRight: ts << "webkit-right"; break;
    case TextAlignMode::WebKitCenter: ts << "webkit-center"; break;
    case TextAlignMode::Start: ts << "start"; break;
    case TextAlignMode::End: ts << "end"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextAlignLast textAlignLast)
{
    switch (textAlignLast) {
    case TextAlignLast::Auto: ts << "auto"; break;
    case TextAlignLast::Start: ts << "start"; break;
    case TextAlignLast::End: ts << "end"; break;
    case TextAlignLast::Left: ts << "left"; break;
    case TextAlignLast::Right: ts << "right"; break;
    case TextAlignLast::Center: ts << "center"; break;
    case TextAlignLast::Justify: ts << "justify"; break;
    }

    return ts;
}

TextStream& operator<<(TextStream& ts, TextCombine textCombine)
{
    switch (textCombine) {
    case TextCombine::None: ts << "none"; break;
    case TextCombine::All: ts << "all"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextDecorationLine line)
{
    switch (line) {
    case TextDecorationLine::Underline: ts << "underline"; break;
    case TextDecorationLine::Overline: ts << "overline"; break;
    case TextDecorationLine::LineThrough: ts << "line-through"; break;
    case TextDecorationLine::Blink: ts << "blink"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextDecorationSkipInk skip)
{
    switch (skip) {
    case TextDecorationSkipInk::None: ts << "none"; break;
    case TextDecorationSkipInk::Auto: ts << "auto"; break;
    case TextDecorationSkipInk::All: ts << "all"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextDecorationStyle decorationStyle)
{
    switch (decorationStyle) {
    case TextDecorationStyle::Solid: ts << "solid"; break;
    case TextDecorationStyle::Double: ts << "double"; break;
    case TextDecorationStyle::Dotted: ts << "dotted"; break;
    case TextDecorationStyle::Dashed: ts << "dashed"; break;
    case TextDecorationStyle::Wavy: ts << "wavy"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextEmphasisFill fill)
{
    switch (fill) {
    case TextEmphasisFill::Filled: ts << "filled"; break;
    case TextEmphasisFill::Open: ts << "open"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextEmphasisMark mark)
{
    switch (mark) {
    case TextEmphasisMark::None: ts << "none"; break;
    case TextEmphasisMark::Auto: ts << "auto"; break;
    case TextEmphasisMark::Dot: ts << "dot"; break;
    case TextEmphasisMark::Circle: ts << "circle"; break;
    case TextEmphasisMark::DoubleCircle: ts << "double-circle"; break;
    case TextEmphasisMark::Triangle: ts << "triangle"; break;
    case TextEmphasisMark::Sesame: ts << "sesame"; break;
    case TextEmphasisMark::Custom: ts << "custom"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextEmphasisPosition position)
{
    switch (position) {
    case TextEmphasisPosition::Over: ts << "Over"; break;
    case TextEmphasisPosition::Under: ts << "Under"; break;
    case TextEmphasisPosition::Left: ts << "Left"; break;
    case TextEmphasisPosition::Right: ts << "Right"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextGroupAlign textGroupAlign)
{
    switch (textGroupAlign) {
    case TextGroupAlign::None: ts << "none"; break;
    case TextGroupAlign::Start: ts << "start"; break;
    case TextGroupAlign::End: ts << "end"; break;
    case TextGroupAlign::Left: ts << "left"; break;
    case TextGroupAlign::Right: ts << "right"; break;
    case TextGroupAlign::Center: ts << "center"; break;
    }

    return ts;
}

TextStream& operator<<(TextStream& ts, TextJustify justify)
{
    switch (justify) {
    case TextJustify::Auto: ts << "auto"; break;
    case TextJustify::InterCharacter: ts << "inter-character"; break;
    case TextJustify::InterWord: ts << "inter-word"; break;
    case TextJustify::None: ts << "none"; break;
    }

    return ts;
}

TextStream& operator<<(TextStream& ts, TextOverflow overflow)
{
    switch (overflow) {
    case TextOverflow::Clip: ts << "clip"; break;
    case TextOverflow::Ellipsis: ts << "ellipsis"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextSecurity textSecurity)
{
    switch (textSecurity) {
    case TextSecurity::None: ts << "none"; break;
    case TextSecurity::Disc: ts << "disc"; break;
    case TextSecurity::Circle: ts << "circle"; break;
    case TextSecurity::Square: ts << "square"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextTransform textTransform)
{
    switch (textTransform) {
    case TextTransform::Capitalize: ts << "capitalize"; break;
    case TextTransform::Uppercase: ts << "uppercase"; break;
    case TextTransform::Lowercase: ts << "lowercase"; break;
    case TextTransform::FullSizeKana: ts << "full-size-kana"; break;
    case TextTransform::FullWidth: ts << "full-width"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextUnderlinePosition position)
{
    switch (position) {
    case TextUnderlinePosition::FromFont: ts << "from-font"; break;
    case TextUnderlinePosition::Under: ts << "under"; break;
    case TextUnderlinePosition::Left: ts << "left"; break;
    case TextUnderlinePosition::Right: ts << "right"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextWrapMode wrap)
{
    switch (wrap) {
    case TextWrapMode::Wrap: ts << "wrap"; break;
    case TextWrapMode::NoWrap: ts << "nowrap"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextWrapStyle style)
{
    switch (style) {
    case TextWrapStyle::Auto: ts << "auto"; break;
    case TextWrapStyle::Balance: ts << "balance"; break;
    case TextWrapStyle::Pretty: ts << "pretty"; break;
    case TextWrapStyle::Stable: ts << "stable"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextBoxTrim textBoxTrim)
{
    switch (textBoxTrim) {
    case TextBoxTrim::None: ts << "None"; break;
    case TextBoxTrim::TrimStart: ts << "trim-start"; break;
    case TextBoxTrim::TrimEnd: ts << "trim-end"; break;
    case TextBoxTrim::TrimBoth: ts << "trim-both"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextEdgeType textEdgeType)
{
    switch (textEdgeType) {
    case TextEdgeType::Auto: ts << "auto"; break;
    case TextEdgeType::Leading: ts << "half-leading"; break;
    case TextEdgeType::Text: ts << "text-over/under baseline"; break;
    case TextEdgeType::CapHeight: ts << "cap-height baseline"; break;
    case TextEdgeType::ExHeight: ts << "x-height baseline"; break;
    case TextEdgeType::Alphabetic: ts << "alphabetic baseline"; break;
    case TextEdgeType::CJKIdeographic: ts << "ideographic-over baseline"; break;
    case TextEdgeType::CJKIdeographicInk: ts << "ideographic-ink-over/ink-under baseline"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TextZoom zoom)
{
    switch (zoom) {
    case TextZoom::Normal: ts << "normal"; break;
    case TextZoom::Reset: ts << "reset"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TransformBox box)
{
    switch (box) {
    case TransformBox::BorderBox: ts << "border-box"; break;
    case TransformBox::FillBox: ts << "fill-box"; break;
    case TransformBox::ViewBox: ts << "view-box"; break;
    case TransformBox::StrokeBox: ts << "stroke-box"; break;
    case TransformBox::ContentBox: ts << "content-box"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, TransformStyle3D transformStyle)
{
    switch (transformStyle) {
    case TransformStyle3D::Flat: ts << "flat"; break;
    case TransformStyle3D::Preserve3D: ts << "preserve-3d"; break;
#if HAVE(CORE_ANIMATION_SEPARATED_LAYERS)
    case TransformStyle3D::Separated: ts << "separated"; break;
#endif
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, UserDrag userDrag)
{
    switch (userDrag) {
    case UserDrag::Auto: ts << "auto"; break;
    case UserDrag::None: ts << "none"; break;
    case UserDrag::Element: ts << "element"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, UserModify userModify)
{
    switch (userModify) {
    case UserModify::ReadOnly: ts << "read-only"; break;
    case UserModify::ReadWrite: ts << "read-write"; break;
    case UserModify::ReadWritePlaintextOnly: ts << "read-write plaintext only"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, UserSelect userSelect)
{
    switch (userSelect) {
    case UserSelect::None: ts << "none"; break;
    case UserSelect::Text: ts << "text"; break;
    case UserSelect::All: ts << "all"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, VerticalAlign verticalAlign)
{
    switch (verticalAlign) {
    case VerticalAlign::Baseline: ts << "baseline"; break;
    case VerticalAlign::Middle: ts << "middle"; break;
    case VerticalAlign::Sub: ts << "sub"; break;
    case VerticalAlign::Super: ts << "super"; break;
    case VerticalAlign::TextTop: ts << "text-top"; break;
    case VerticalAlign::TextBottom: ts << "text-bottom"; break;
    case VerticalAlign::Top: ts << "top"; break;
    case VerticalAlign::Bottom: ts << "bottom"; break;
    case VerticalAlign::BaselineMiddle: ts << "baseline-middle"; break;
    case VerticalAlign::Length: ts << "length"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, Visibility visibility)
{
    switch (visibility) {
    case Visibility::Visible: ts << "visible"; break;
    case Visibility::Hidden: ts << "hidden"; break;
    case Visibility::Collapse: ts << "collapse"; break;
    }
    
    return ts;
}

TextStream& operator<<(TextStream& ts, WhiteSpace whiteSpace)
{
    switch (whiteSpace) {
    case WhiteSpace::Normal: ts << "normal"; break;
    case WhiteSpace::Pre: ts << "pre"; break;
    case WhiteSpace::PreWrap: ts << "pre-wrap"; break;
    case WhiteSpace::PreLine: ts << "pre-line"; break;
    case WhiteSpace::NoWrap: ts << "nowrap"; break;
    case WhiteSpace::BreakSpaces: ts << "break-spaces"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, WhiteSpaceCollapse whiteSpaceCollapse)
{
    switch (whiteSpaceCollapse) {
    case WhiteSpaceCollapse::Collapse: ts << "collapse"; break;
    case WhiteSpaceCollapse::Preserve: ts << "preserve"; break;
    case WhiteSpaceCollapse::PreserveBreaks: ts << "preserve-breaks"; break;
    case WhiteSpaceCollapse::BreakSpaces: ts << "break-spaces"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, WordBreak wordBreak)
{
    switch (wordBreak) {
    case WordBreak::Normal: ts << "normal"; break;
    case WordBreak::BreakAll: ts << "break-all"; break;
    case WordBreak::KeepAll: ts << "keep-all"; break;
    case WordBreak::BreakWord: ts << "break-word"; break;
    case WordBreak::AutoPhrase: ts << "auto-phrase"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, MathStyle mathStyle)
{
    switch (mathStyle) {
    case MathStyle::Normal: ts << "normal"; break;
    case MathStyle::Compact: ts << "compact"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, ContainIntrinsicSizeType containIntrinsicSizeType)
{
    switch (containIntrinsicSizeType) {
    case ContainIntrinsicSizeType::None: ts << "none"; break;
    case ContainIntrinsicSizeType::Length: ts << "length"; break;
    case ContainIntrinsicSizeType::AutoAndLength: ts << "autoandlength"; break;
    case ContainIntrinsicSizeType::AutoAndNone: ts << "autoandnone"; break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, OverflowContinue overflowContinue)
{
    switch (overflowContinue) {
    case OverflowContinue::Auto:
        ts << "auto";
        break;
    case OverflowContinue::Discard:
        ts << "discard";
        break;
    }
    return ts;
}

TextStream& operator<<(TextStream& ts, StyleDifferenceContextSensitiveProperty property)
{
    switch (property) {
    case StyleDifferenceContextSensitiveProperty::Transform: ts << "transform"; break;
    case StyleDifferenceContextSensitiveProperty::Opacity: ts << "opacity"; break;
    case StyleDifferenceContextSensitiveProperty::Filter: ts << "filter"; break;
    case StyleDifferenceContextSensitiveProperty::ClipRect: ts << "clipRect"; break;
    case StyleDifferenceContextSensitiveProperty::ClipPath: ts << "clipPath"; break;
    case StyleDifferenceContextSensitiveProperty::WillChange: ts << "willChange"; break;
    }
    return ts;
}

} // namespace WebCore
