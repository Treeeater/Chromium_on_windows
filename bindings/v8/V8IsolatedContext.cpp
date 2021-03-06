/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
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

#include "V8IsolatedContext.h"

#include "Frame.h"
#include "FrameLoaderClient.h"
#include "HashMap.h"
#include "ScriptController.h"
#include "V8DOMWindow.h"
#include "V8HiddenPropertyName.h"

namespace WebCore {


void V8IsolatedContext::contextWeakReferenceCallback(v8::Persistent<v8::Value> object, void* isolatedContext)
{
    // Our context is going away.  Time to clean up the world.
    V8IsolatedContext* context = static_cast<V8IsolatedContext*>(isolatedContext);
    delete context;
}

V8IsolatedContext::V8IsolatedContext(V8Proxy* proxy, int extensionGroup, int WID, String SharedLibId, String UseLibId)
    : m_world(IsolatedWorld::create())
{
	m_worldID = WID;
	m_sharedLibId = "";
	isSharedLib = false;
    v8::HandleScope scope;
    // FIXME: We should be creating a new V8DOMWindowShell here instead of riping out the context.
    m_context = SharedPersistent<v8::Context>::create(proxy->windowShell()->createNewContext(v8::Handle<v8::Object>(), extensionGroup));
    if (m_context->get().IsEmpty())
        return;
    // Run code in the new context.
    v8::Context::Scope contextScope(m_context->get());

    getGlobalObject(m_context->get())->SetPointerInInternalField(V8DOMWindow::enteredIsolatedWorldIndex, this);
    V8DOMWindowShell::installHiddenObjectPrototype(m_context->get());
    // FIXME: This will go away once we have a windowShell for the isolated world.
    proxy->windowShell()->installDOMWindow(m_context->get(), proxy->frame()->domWindow());

    // Using the default security token means that the canAccess is always
    // called, which is slow.
    // FIXME: Use tokens where possible. This will mean keeping track of all
    //        created contexts so that they can all be updated when the
    //        document domain
    //        changes.
    m_context->get()->UseDefaultSecurityToken();

    proxy->frame()->loader()->client()->didCreateIsolatedScriptContext();
	//inject dummy object for all third-p scripts
	/*
	if (WID!=0)
	{
		v8::Local<v8::Value> dummy = v8::String::New("dummy_obj_zyc");
		v8::Local<v8::Value> empty = v8::String::New("");
		m_context->get()->Global()->Set(dummy, empty);
	}
	*/
	//Acquire all used libraries
	if (UseLibId!="")
	{
		v8::Local<v8::Value> foo = v8::String::New("foo");
		m_context->get()->SetSecurityToken(foo);
		Vector<WTF::String> Libs;
		UseLibId.split(";",Libs);
		int world;
		for (unsigned int i=0; i<Libs.size(); i++)
		{
			if (proxy->getLibMap().contains(Libs[i]))
			{
				world = proxy->getLibMap().get(Libs[i]);
				m_context->get()->Global()->Set(v8::String::New(Libs[i].utf8().data()), proxy->getIWMap().get(world)->context()->Global());
			}
		}
	}
	//Share the shared lib with main world
	if ((SharedLibId!="")&&(!proxy->mainWorldContext().IsEmpty()))
	{
		v8::Local<v8::Value> foo = v8::String::New("foo");
		proxy->mainWorldContext()->SetSecurityToken(foo);
		v8::Handle<v8::Object> shared_lib_obj = m_context->get()->Global();
		proxy->mainWorldContext()->Global()->Set(v8::String::New(SharedLibId.utf8().data()), shared_lib_obj);
		proxy->addLibToMap(SharedLibId,WID);
		m_sharedLibId = SharedLibId;
		isSharedLib = true;
	}
	m_writable = true;
}

void V8IsolatedContext::destroy()
{
    m_context->get().MakeWeak(this, &contextWeakReferenceCallback);
}

V8IsolatedContext::~V8IsolatedContext()
{
    m_context->disposeHandle();
}

} // namespace WebCore
