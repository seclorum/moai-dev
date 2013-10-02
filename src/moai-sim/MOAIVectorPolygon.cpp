// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIVectorDrawing.h>
#include <moai-sim/MOAIVectorPolygon.h>
#include <tesselator.h>

//================================================================//
// MOAIVectorPolygon
//================================================================//

//----------------------------------------------------------------//
bool MOAIVectorPolygon::GroupVertices ( MOAIVectorDrawing& drawing, u32 total ) {
	
	if ( total ) {
		this->mVertices.Init ( total );
		drawing.CopyVertexStack ( this->mVertices, total );
	}
	return true;
}

//----------------------------------------------------------------//
MOAIVectorPolygon::MOAIVectorPolygon () {
}

//----------------------------------------------------------------//
MOAIVectorPolygon::~MOAIVectorPolygon () {
}

//----------------------------------------------------------------//
void MOAIVectorPolygon::SetVertices ( USVec2D* vertices, u32 total ) {

	if ( total ) {
		this->mVertices.Init ( total );
		memcpy ( this->mVertices.Data (), vertices, total * sizeof ( USVec2D ));
		this->SetOpen ( false );
	}
}

//----------------------------------------------------------------//
void MOAIVectorPolygon::ToOutline ( TESStesselator* outline ) {

	tessAddContour ( outline, 2, this->mVertices.Data (), sizeof ( USVec2D ), this->mVertices.Size ());
}
