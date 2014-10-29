#!/bin/bash


#OSX
cd `dirname $0`/..
cd cmake
mkdir projects
cd projects
rm -rf moai-osx
mkdir moai-osx
cd moai-osx

cmake \
-G "Xcode" \
-DBUILD_OSX=TRUE \
-DSDL_HOST=TRUE \
-DMOAI_BOX2D=TRUE \
-DMOAI_CHIPMUNK=TRUE \
-DMOAI_CURL=TRUE \
-DMOAI_CRYPTO=TRUE \
-DMOAI_EXPAT=TRUE \
-DMOAI_FREETYPE=TRUE \
-DMOAI_JSON=TRUE \
-DMOAI_JPG=TRUE \
-DMOAI_LUAEXT=TRUE \
-DMOAI_MONGOOSE=TRUE \
-DMOAI_OGG=TRUE \
-DMOAI_OPENSSL=TRUE \
-DMOAI_SQLITE3=TRUE \
-DMOAI_TINYXML=TRUE \
-DMOAI_PNG=TRUE \
-DMOAI_SFMT=TRUE \
-DMOAI_VORBIS=TRUE \
-DMOAI_WEBP=TRUE \
-DMOAI_UNTZ=TRUE \
-DMOAI_LUAJIT=TRUE \
-DMOAI_HTTP_CLIENT=TRUE \
../../

