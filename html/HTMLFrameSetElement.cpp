/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Simon Hausmann (hausmann@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2006, 2009, 2010 Apple Inc. All rights reserved.
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
 */

#include "config.h"
#include "HTMLFrameSetElement.h"

#include "Attribute.h"
#include "CSSPropertyNames.h"
#include "Document.h"
#include "Event.h"
#include "EventNames.h"
#include "HTMLNames.h"
#include "Length.h"
#include "MouseEvent.h"
#include "RenderFrameSet.h"
#include "ScriptEventListener.h"
#include "Text.h"

namespace WebCore {

using namespace HTMLNames;

HTMLFrameSetElement::HTMLFrameSetElement(const QualifiedName& tagName, Document* document)
    : HTMLElement(tagName, document)
    , m_totalRows(1)
    , m_totalCols(1)
    , m_border(6)
    , m_borderSet(false)
    , m_borderColorSet(false)
    , frameborder(true)
    , frameBorderSet(false)
    , noresize(false)
{
    ASSERT(hasTagName(framesetTag));
}

PassRefPtr<HTMLFrameSetElement> HTMLFrameSetElement::create(const QualifiedName& tagName, Document* document)
{
    return adoptRef(new HTMLFrameSetElement(tagName, document));
}

bool HTMLFrameSetElement::mapToEntry(const QualifiedName& attrName, MappedAttributeEntry& result) const
{
    if (attrName == bordercolorAttr) {
        result = eUniversal;
        return true;
    }

    return HTMLElement::mapToEntry(attrName, result);
}

void HTMLFrameSetElement::parseMappedAttribute(Attribute* attr)
{
    if (attr->name() == rowsAttr) {
        if (!attr->isNull()) {
            m_rowLengths.set(newLengthArray(attr->value().string(), m_totalRows));
            setNeedsStyleRecalc();
        }
    } else if (attr->name() == colsAttr) {
        if (!attr->isNull()) {
            m_colLengths.set(newLengthArray(attr->value().string(), m_totalCols));
            setNeedsStyleRecalc();
        }
    } else if (attr->name() == frameborderAttr) {
        if (!attr->isNull()) {
            // false or "no" or "0"..
            if (attr->value().toInt() == 0) {
                frameborder = false;
                m_border = 0;
            }
            frameBorderSet = true;
        } else {
            frameborder = false;
            frameBorderSet = false;
        }
    } else if (attr->name() == noresizeAttr) {
        noresize = true;
    } else if (attr->name() == borderAttr) {
        if (!attr->isNull()) {
            m_border = attr->value().toInt();
            if (!m_border)
                frameborder = false;
            m_borderSet = true;
        } else
            m_borderSet = false;
    } else if (attr->name() == bordercolorAttr) {
        m_borderColorSet = attr->decl();
        if (!attr->decl() && !attr->isEmpty()) {
            addCSSColor(attr, CSSPropertyBorderColor, attr->value());
            m_borderColorSet = true;
        }
    } else if (attr->name() == onloadAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().loadEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onbeforeunloadAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().loadEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onunloadAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().loadEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onblurAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().loadEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onfocusAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().focusEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onfocusinAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().focusinEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onfocusoutAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().focusoutEvent, createAttributeEventListener(this, attr));
	}
#if ENABLE(ORIENTATION_EVENTS)
    else if (attr->name() == onorientationchangeAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().orientationchangeEvent, createAttributeEventListener(this, attr));
	}
#endif
    else if (attr->name() == onhashchangeAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().hashchangeEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onresizeAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().resizeEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onscrollAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().scrollEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onstorageAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().storageEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == ononlineAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().onlineEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onofflineAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().offlineEvent, createAttributeEventListener(this, attr));
	}
    else if (attr->name() == onpopstateAttr)
	{
		RefPtr<V8LazyEventListener> eventlistener = createAttributeEventListener(document()->frame(), attr);
		if (attr->getWorldID()!=0) eventlistener->setWorldID(attr->getWorldID());
        setAttributeEventListener(eventNames().popstateEvent, createAttributeEventListener(this, attr));
	}
    else
        HTMLElement::parseMappedAttribute(attr);
}

bool HTMLFrameSetElement::rendererIsNeeded(RenderStyle *style)
{
    // For compatibility, frames render even when display: none is set.
    // However, we delay creating a renderer until stylesheets have loaded. 
    return style->isStyleAvailable();
}

RenderObject *HTMLFrameSetElement::createRenderer(RenderArena *arena, RenderStyle *style)
{
    if (style->contentData())
        return RenderObject::createObject(this, style);
    
    return new (arena) RenderFrameSet(this);
}

void HTMLFrameSetElement::attach()
{
    // Inherit default settings from parent frameset
    // FIXME: This is not dynamic.
    for (Node* node = parentNode(); node; node = node->parentNode()) {
        if (node->hasTagName(framesetTag)) {
            HTMLFrameSetElement* frameset = static_cast<HTMLFrameSetElement*>(node);
            if (!frameBorderSet)
                frameborder = frameset->hasFrameBorder();
            if (frameborder) {
                if (!m_borderSet)
                    m_border = frameset->border();
                if (!m_borderColorSet)
                    m_borderColorSet = frameset->hasBorderColor();
            }
            if (!noresize)
                noresize = frameset->noResize();
            break;
        }
    }

    HTMLElement::attach();
}

void HTMLFrameSetElement::defaultEventHandler(Event* evt)
{
    if (evt->isMouseEvent() && !noresize && renderer()) {
        if (toRenderFrameSet(renderer())->userResize(static_cast<MouseEvent*>(evt))) {
            evt->setDefaultHandled();
            return;
        }
    }
    HTMLElement::defaultEventHandler(evt);
}

void HTMLFrameSetElement::recalcStyle(StyleChange ch)
{
    if (needsStyleRecalc() && renderer()) {
        renderer()->setNeedsLayout(true);
        clearNeedsStyleRecalc();
    }
    HTMLElement::recalcStyle(ch);
}

} // namespace WebCore
