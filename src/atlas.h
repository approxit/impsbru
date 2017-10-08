#ifndef GUARD_IMPSBRU_ATLAS_H
#define GUARD_IMPSBRU_ATLAS_H

#include <ace/utils/bitmap.h>
#include <ace/utils/bitmapmask.h>

void createAtlasFile(
	INOUT tBitMap **pBitMapAtlas,
	IN UBYTE ubAtlasIndex,
	IN char *szBitMapFilePath
);

void destroyAtlasFiles(
	INOUT tBitMap **pBitMapAtlas,
	IN UBYTE ubAtlasCount
);

void createAtlasFileWithMask(
	INOUT tBitMap **pBitMapAtlas,
	INOUT tBitmapMask **pBitMapMaskAtlas,
	IN UBYTE ubAtlasIndex,
	IN char *szBitMapFilePath,
	IN char *szBitMapMaskFilePath
);

void destroyAtlasFilesWithMask(
	INOUT tBitMap **pBitMapAtlas,
	INOUT tBitmapMask **pBitMapMaskAtlas,
	IN UBYTE ubAtlasCount
);

#endif