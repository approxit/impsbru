#include "atlas.h"

void createAtlasFile(tBitMap *pBitMapAtlas[], UBYTE ubAtlasIndex, char *szBitMapFilePath) {
	pBitMapAtlas[ubAtlasIndex] = bitmapCreateFromFile(szBitMapFilePath);
}

void destroyAtlasFiles(tBitMap *pBitMapAtlas[], UBYTE ubAtlasCount) {
	UBYTE ubAtlasIndex = ubAtlasCount;
	while (ubAtlasIndex--) {
		bitmapDestroy(pBitMapAtlas[ubAtlasIndex]);
	}
}

void createAtlasFileWithMask(tBitMap *pBitMapAtlas[], tBitmapMask *pBitMapMaskAtlas[], UBYTE ubAtlasIndex, char *szBitMapFilePath, char *szBitMapMaskFilePath) {
	pBitMapAtlas[ubAtlasIndex] = bitmapCreateFromFile(szBitMapFilePath);
	pBitMapMaskAtlas[ubAtlasIndex] = bitmapMaskCreateFromFile(szBitMapMaskFilePath);
}

void destroyAtlasFilesWithMask(tBitMap *pBitMapAtlas[], tBitmapMask *pBitMapMaskAtlas[], UBYTE ubAtlasCount) {
	UBYTE ubAtlasIndex = ubAtlasCount;
	while (ubAtlasIndex--) {
		if (pBitMapAtlas[ubAtlasIndex]) {
			bitmapDestroy(pBitMapAtlas[ubAtlasIndex]);
		}

		if (pBitMapMaskAtlas[ubAtlasIndex]) {
			bitmapMaskDestroy(pBitMapMaskAtlas[ubAtlasIndex]);
		}
	}
}