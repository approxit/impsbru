#include "cross.h"

tBitMap *g_pCrossBitMapAtlas[ATLAS_COUNT] = {0};
tBitmapMask *g_pCrossBitMapMaskAtlas[ATLAS_COUNT] = {0};

UBYTE getCrossSideAtlasIndex(UBYTE ubCrossSide, UBYTE ubCrossSideMode) {
	return (CROSS_SIDE_MODE_COUNT * ubCrossSide) + ubCrossSideMode;
}

UBYTE getCrossSideMode(UWORD uwCrossData, UBYTE ubCrossSide) {
	return (uwCrossData >> (CROSS_SIDE_MODE_BYTES * ubCrossSide)) & CROSS_SIDE_MODE_COUNT;
}

void createCrossAtlas() {
	createCrossAtlasFile(0, "/data/cross_frames/cross_frame_center.bm", "/data/cross_frames/cross_frame_center.msk");

	createCrossAtlasFile(1, "/data/cross_frames/cross_frame_a_open.bm", "/data/cross_frames/cross_frame_a_open.msk");
	createCrossAtlasFile(2, "/data/cross_frames/cross_frame_a_closed.bm", "/data/cross_frames/cross_frame_a_closed.msk");
	createCrossAtlasFile(3, "/data/cross_frames/cross_frame_a_final.bm", "/data/cross_frames/cross_frame_a_final.msk");

	createCrossAtlasFile(4, "/data/cross_frames/cross_frame_b_open.bm", "/data/cross_frames/cross_frame_b_closed.msk");
	createCrossAtlasFile(5, "/data/cross_frames/cross_frame_b_closed.bm", "/data/cross_frames/cross_frame_b_closed.msk");
	// Index 6 is empty by default

	createCrossAtlasFile(7, "/data/cross_frames/cross_frame_c_open.bm", "/data/cross_frames/cross_frame_c_open.msk");
	createCrossAtlasFile(8, "/data/cross_frames/cross_frame_c_closed.bm", "/data/cross_frames/cross_frame_c_closed.msk");
	createCrossAtlasFile(9, "/data/cross_frames/cross_frame_c_final.bm", "/data/cross_frames/cross_frame_c_final.msk");

	createCrossAtlasFile(10, "/data/cross_frames/cross_frame_d_open.bm", "/data/cross_frames/cross_frame_d_open.msk");
	createCrossAtlasFile(11, "/data/cross_frames/cross_frame_d_closed.bm", "/data/cross_frames/cross_frame_d_closed.msk");
	// Index 12 is empty by default

	createCrossAtlasFile(13, "/data/cross_frames/cross_frame_e_open.bm", "/data/cross_frames/cross_frame_e_open.msk");
	createCrossAtlasFile(14, "/data/cross_frames/cross_frame_e_closed.bm", "/data/cross_frames/cross_frame_e_closed.msk");
	createCrossAtlasFile(15, "/data/cross_frames/cross_frame_e_final.bm", "/data/cross_frames/cross_frame_e_final.msk");

	createCrossAtlasFile(16, "/data/cross_frames/cross_frame_f_open.bm", "/data/cross_frames/cross_frame_f_open.msk");
	createCrossAtlasFile(17, "/data/cross_frames/cross_frame_f_closed.bm", "/data/cross_frames/cross_frame_f_closed.msk");
	// Index 18 is empty by default
}

void createCrossAtlasFile(UBYTE ubAtlasIndex, char *szBitMapFilePath, char *szBitMapMaskFilePath) {
	g_pCrossBitMapAtlas[ubAtlasIndex] = bitmapCreateFromFile(szBitMapFilePath);
	g_pCrossBitMapMaskAtlas[ubAtlasIndex] = bitmapMaskCreateFromFile(szBitMapMaskFilePath);
}

void destroyCrossAtlas() {
	UBYTE ubAtlasIndex = ATLAS_COUNT;
	while (ubAtlasIndex--) {
		if (g_pCrossBitMapAtlas[ubAtlasIndex]) {
			bitmapDestroy(g_pCrossBitMapAtlas[ubAtlasIndex]);
		}

		if (g_pCrossBitMapMaskAtlas[ubAtlasIndex]) {
			bitmapMaskDestroy(g_pCrossBitMapMaskAtlas[ubAtlasIndex]);
		}
	}
}