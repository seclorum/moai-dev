// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

SUPPRESS_EMPTY_FILE_WARNING

#ifndef _WIN32

#include <moai-util/MOAIMutex_posix.h>

//================================================================//
// MOAIMutexImpl
//================================================================//

//----------------------------------------------------------------//
void MOAIMutexImpl::Lock () {

	pthread_mutex_lock ( &this->mMutex );
}

//----------------------------------------------------------------//
MOAIMutexImpl::MOAIMutexImpl () {

	memset ( &this->mMutex, 0, sizeof ( pthread_mutex_t ));
	pthread_mutex_init ( &this->mMutex, 0 );
}

//----------------------------------------------------------------//
MOAIMutexImpl::~MOAIMutexImpl () {

	pthread_mutex_destroy ( &this->mMutex );
}

//----------------------------------------------------------------//
void MOAIMutexImpl::Unlock () {

	pthread_mutex_unlock ( &this->mMutex );
}

#endif
