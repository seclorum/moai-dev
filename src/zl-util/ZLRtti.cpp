// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLRtti.h>

//================================================================//
// RTTIRecord
//================================================================//

//----------------------------------------------------------------//
void RTTIRecord::AffirmCasts ( void* ptr ) {

	if ( !this->mIsComplete ) {
		this->Inherit ( *this, ptr, 0 );
		// TODO: maybe sort for binary search? or by frequency of use?
	}
	this->mIsComplete = true;
}

//----------------------------------------------------------------//
void* RTTIRecord::AsType ( RTTIRecord& record, void* ptr ) {
	
	if ( this == &record ) return ptr;
	
	if ( this->mIsComplete == false ) {
		this->AffirmCasts ( ptr );
	}
	
	// TODO: binary search?
	for ( u32 i = 0; i < this->mTypeCount; ++i ) {
		if ( this->mTypeSet [ i ] == &record ) {
			return ( void* )(( ptrdiff_t )ptr + this->mJumpTable [ i ]);
		}
	}
	return 0;
}

//----------------------------------------------------------------//
void RTTIRecord::Complete() {
	this->mIsConstructed = true;
}

//----------------------------------------------------------------//
void RTTIRecord::Inherit ( RTTIRecord& record, void* ptr, ptrdiff_t offset ) {
	
	if ( this != &record ) {
	
		for ( u32 i = 0; i < this->mTypeCount; ++i ) {
			if ( this->mTypeSet [ i ] == &record ) {
				return;
			}
		}
		
		assert ( this->mTypeCount < MAX );
		
		this->mTypeSet [ this->mTypeCount ] = &record;
		this->mJumpTable [ this->mTypeCount ] = offset;
		this->mTypeCount++;
	}
	
	// follow the links and inherit them
	for ( u32 i = 0; i < record.mLinkCount; ++i ) {
		
		RTTILinkBase& link = *record.mLinks [ i ];
		
		ptrdiff_t jump = link.GetOffset ( ptr );
		RTTIRecord& nextRecord = *link.mTarget;
		
		void* nextPtr = ( void* )(( ptrdiff_t )ptr + jump );
		
		this->Inherit ( nextRecord, nextPtr, offset + jump );
	}
}

//----------------------------------------------------------------//
bool RTTIRecord::IsType ( RTTIRecord& record, void* ptr ) {
	
	if ( this == &record ) return true;
	
	this->AffirmCasts ( ptr );

	// TODO: binary search?
	for ( u32 i = 0; i < this->mTypeCount; ++i ) {
		if ( this->mTypeSet [ i ] == &record ) return true;
	}
	return false;
}

//----------------------------------------------------------------//
RTTIRecord::RTTIRecord () :
	mLinkCount ( 0 ),
	mIsConstructed ( false ),
	mTypeCount ( 0 ),
	mIsComplete ( false ) {
}

//----------------------------------------------------------------//
RTTIRecord::~RTTIRecord () {
}

//================================================================//
// RTTIBase
//================================================================//
	
//----------------------------------------------------------------//
RTTIBase::RTTIBase () {
	this->BeginRTTI < RTTIBase >(this);
	this->EndRTTI ();
}

//----------------------------------------------------------------//
RTTIBase::~RTTIBase () {
}

//----------------------------------------------------------------//
cc8* RTTIBase::TypeName () const {
	return "RTTIBase";
}
