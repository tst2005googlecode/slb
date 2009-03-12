/*
    SLB - Simple Lua Binder
    Copyright (C) 2007 Jose L. Hidalgo Valiño (PpluX)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

	Jose L. Hidalgo (www.pplux.com)
	pplux@pplux.com
*/

#ifndef __SLB_STATEFUL_HYBRID__
#define __SLB_STATEFUL_HYBRID__

#include "Hybrid.hpp" 
#include "Script.hpp" 

namespace SLB {
	
	/* S -> Requires to have a method "getState" and "close" */
	template<class T, class S = SLB::Script>
	class StatefulHybrid :
		public S, public Hybrid< T >
	{	
	public:
		StatefulHybrid(){}
		virtual ~StatefulHybrid() { S::close(); }

		virtual bool isAttached() const;
		virtual lua_State *getLuaState() { return S::getState(); }

	protected:
		virtual void onNewState(lua_State *L) { HybridBase::attach( L ); S::onNewState(L); }
		virtual void onCloseState(lua_State *L) { HybridBase::unAttach(); S::onCloseState(L); }

	};
	
	template<class T, class S >
	inline bool StatefulHybrid<T,S>::isAttached() const
	{
		//StatefulHybrids are always attached (but as we use a lazy attachment, here
		// we have to simulate and do the attachment, that means throw away constness)
		
		StatefulHybrid<T,S> *me = const_cast< StatefulHybrid<T,S>* >(this);
		me->getState(); // That's enought to ensure the attachment
		return true;
	}

}

#endif
