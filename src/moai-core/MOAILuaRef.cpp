// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaClass.h>
#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAILuaRuntime.h>

//================================================================//
// MOAILuaRef
//================================================================//

//================================================================//
// MOAILuaRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaRef::Clear () {

	if ( this->mOwnsRef && MOAILuaRuntime::IsValid () ) {
		MOAILuaRuntime::Get ().ClearRef ( this->mRef );
	}
	this->mOwnsRef = false;
	this->mRef = LUA_NOREF;
}

//----------------------------------------------------------------//
u32 MOAILuaRef::GetID () {

	return this->mRef;
}

//----------------------------------------------------------------//
MOAIScopedLuaState MOAILuaRef::GetSelf () {

	assert ( !this->IsNil ());

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushRef ( state );
	return state;
}

//----------------------------------------------------------------//
bool MOAILuaRef::IsNil () {

	return ( this->mRef == LUA_NOREF );
}

//----------------------------------------------------------------//
MOAILuaRef::MOAILuaRef () :
	mOwnsRef ( false ),
	mRef ( LUA_NOREF ) {
}

//----------------------------------------------------------------//
MOAILuaRef::MOAILuaRef ( const MOAILuaRef& assign ) :
	mOwnsRef ( false ),
	mRef ( LUA_NOREF ) {
	this->Take ( assign );
}

//----------------------------------------------------------------//
MOAILuaRef::~MOAILuaRef () {

	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAILuaRef::PushRef ( MOAILuaState& state ) {

	bool result = MOAILuaRuntime::Get ().PushRef ( state, this->mRef );
	
	if ( !result ) {
		this->Clear ();
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAILuaRef::SetRef ( MOAILuaState& state, int idx, bool weak ) {

	this->Clear ();
	this->mRef = MOAILuaRuntime::Get ().GetRef ( state, idx, weak );
	this->mOwnsRef = ( this->mRef != LUA_NOREF );
}

//----------------------------------------------------------------//
void MOAILuaRef::SetStrongRef ( MOAILuaState& state, int idx ) {

	this->SetRef ( state, idx, false );
}

//----------------------------------------------------------------//
void MOAILuaRef::SetWeakRef ( MOAILuaState& state, int idx ) {

	this->SetRef ( state, idx, true );
}

//----------------------------------------------------------------//
void MOAILuaRef::Take ( const MOAILuaRef& assign ) {

	this->Clear ();

	this->mRef = assign.mRef;
	this->mOwnsRef = assign.mOwnsRef;

	// cast the const away
	(( MOAILuaRef& )assign ).mOwnsRef = false;
}

//================================================================//
// MOAILuaStrongRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaStrongRef::SetRef ( MOAILuaState& state, int idx ) {
	MOAILuaRef::SetRef ( state, idx, false );
}

//================================================================//
// MOAILuaWeakRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaWeakRef::SetRef ( MOAILuaState& state, int idx ) {
	MOAILuaRef::SetRef ( state, idx, true );
}

//================================================================//
// MOAILuaMemberRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaMemberRef::Clear () {

	if ( this->mRef != LUA_NOREF ) {

		if (( !this->mOwner->mCollected ) && MOAILuaRuntime::IsValid ()) {
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			if ( this->mOwner->PushRefTable ( state )) {
				luaL_unref ( state, -1, this->mRef );
			}
		}
		this->mRef = LUA_NOREF;
		this->mOwner = 0;
	}
}

//----------------------------------------------------------------//
MOAILuaMemberRef::MOAILuaMemberRef () :
	mRef ( LUA_NOREF ),
	mOwner ( 0 ) {
}

//----------------------------------------------------------------//
MOAILuaMemberRef::~MOAILuaMemberRef () {
	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAILuaMemberRef::PushRef ( MOAILuaState& state ) {

	if ( this->mRef != LUA_NOREF ) {

		bool isNil = true;

		if ( this->mOwner->IsBound ()) {
			if ( this->mOwner->PushRefTable ( state )) {
				lua_rawgeti ( state, -1, this->mRef );
				lua_replace ( state, -2 );
			}
			isNil = lua_isnil ( state, -1 );
		}

		if ( isNil ) {
			this->mRef = LUA_NOREF;
			this->mOwner = 0;
			return false;
		}
		return true;
	}
	lua_pushnil ( state );
	return false;
}

//----------------------------------------------------------------//
void MOAILuaMemberRef::SetRef ( MOAILuaObject& owner, MOAILuaState& state, int idx ) {

	this->Clear ();

	if ( lua_isnil ( state, idx ) == false ) {

		idx = state.AbsIndex ( idx );

		this->mOwner = &owner;
		this->mOwner->PushRefTable ( state );

		lua_pushvalue ( state, idx );
		this->mRef = luaL_ref ( state, -2 );

		lua_pop ( state, 1 );
	}
}
