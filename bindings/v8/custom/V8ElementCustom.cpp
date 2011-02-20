/*
 * Copyright (C) 2007-2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "V8Element.h"

#include "Attr.h"
#include "CSSHelper.h"
#include "Document.h"
#include "Element.h"
#include "ExceptionCode.h"
#include "HTMLFrameElementBase.h"
#include "HTMLNames.h"
#include "Node.h"

#include "V8Attr.h"
#include "V8Binding.h"
#include "V8BindingState.h"
#include "V8HTMLElement.h"
#include "V8Proxy.h"
#include "V8IsolatedContext.h"

#if ENABLE(SVG)
#include "V8SVGElement.h"
#endif

#include <wtf/RefPtr.h>

namespace WebCore {

v8::Handle<v8::Value> toV8(Element* impl, bool forceNewObject)
{
	/*
	int worldID = 0;
	V8IsolatedContext* isolatedContext = V8IsolatedContext::getEntered();
	*/
    if (!impl)
        return v8::Null();
	/*
	if (isolatedContext!=0) 
	{
		if (!isolatedContext->is_SharedLib())
		{
			worldID = isolatedContext->getWorldID();
			if ((worldID!=0)&&(impl->isHTMLElement()))
			{
				WTF::String NodeACL = ((Element*)impl)->getAttribute("ACL");
				if ((NodeACL!=0)&&(NodeACL!=""))
				{
					bool flag = false;
					Vector<WTF::String> ACLs;
					NodeACL.split(";",ACLs);
					for (unsigned int i=0; i<ACLs.size(); i++)
					{
						if (worldID==ACLs[i].toInt())
						{
							flag = true;
							break;
						}
					}
					if (flag == false) return v8::Undefined();
					//instead of returning v8::Null(), we return a dummy value injected at the beginning of execution. This would make some 3rd-p scripts work because it does
					//not generate an unrecoverable error to v8.
				}
				else return v8::Undefined();		//default policy is: script w/ worldID cannot access node w/o ACL
			}
		}
	}
	*/
    if (impl->isHTMLElement())
        return toV8(static_cast<HTMLElement*>(impl), forceNewObject);
#if ENABLE(SVG)
    if (impl->isSVGElement())
        return toV8(static_cast<SVGElement*>(impl), forceNewObject);
#endif
    return V8Element::wrap(impl, forceNewObject);
}
} // namespace WebCore
