#ifndef __MDNIE_TABLE_H__
#define __MDNIE_TABLE_H__

#include "mdnie.h"


static unsigned short tune_dynamic_gallery[] = {
	/*start Q1 dynamic gallery */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x03ff, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x00a0, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0x0000, /*gc R */
	0x00ca, 0xffff, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xffff, /*gc G */
	0x00ce, 0x0000, /*rm G */
	0x00cf, 0xffff, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x00ff, /*gc B */
	0x00d2, 0x00ff, /*rm B */
	0x00d3, 0x00ff, /*yw B */
	0x00d6, 0x1c00, /*GAMMA Q1d */
	0x00d7, 0x1c00,
	0x00d8, 0x1c00,
	0x00d9, 0x1c00,
	0x00da, 0x1c00,
	0x00db, 0x1c00,
	0x00dc, 0x1c00,
	0x00dd, 0x1c00,
	0x00de, 0x1c00,
	0x00df, 0x1c00,
	0x00e0, 0x1c00,
	0x00e1, 0x1c00,
	0x00e2, 0x1c00,
	0x00e3, 0xa40d,
	0x00e4, 0xa40d,
	0x00e5, 0xa40d,
	0x00e6, 0xa40d,
	0x00e7, 0xa40d,
	0x00e8, 0xa40d,
	0x00e9, 0xa40d,
	0x00ea, 0xa40d,
	0x00eb, 0x1c25,
	0x00ec, 0x174a,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_dynamic_ui[] = {
	/*start Q1 dynamic ui */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x0030, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x00a0, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0x0000, /*gc R */
	0x00ca, 0xffff, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xffff, /*gc G */
	0x00ce, 0x0000, /*rm G */
	0x00cf, 0xffff, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x00ff, /*gc B */
	0x00d2, 0x00ff, /*rm B */
	0x00d3, 0x00ff, /*yw B */
	0x00d6, 0x1c00, /*GAMMA Q1d */
	0x00d7, 0x1c00,
	0x00d8, 0x1c00,
	0x00d9, 0x1c00,
	0x00da, 0x1c00,
	0x00db, 0x1c00,
	0x00dc, 0x1c00,
	0x00dd, 0x1c00,
	0x00de, 0x1c00,
	0x00df, 0x1c00,
	0x00e0, 0x1c00,
	0x00e1, 0x1c00,
	0x00e2, 0x1c00,
	0x00e3, 0xa40d,
	0x00e4, 0xa40d,
	0x00e5, 0xa40d,
	0x00e6, 0xa40d,
	0x00e7, 0xa40d,
	0x00e8, 0xa40d,
	0x00e9, 0xa40d,
	0x00ea, 0xa40d,
	0x00eb, 0x1c25,
	0x00ec, 0x174a,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_dynamic_video[] = {
	/*start Q1 dynamic video */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x03ff, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x00a0, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0x0000, /*gc R */
	0x00ca, 0xffff, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xffff, /*gc G */
	0x00ce, 0x0000, /*rm G */
	0x00cf, 0xffff, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x00ff, /*gc B */
	0x00d2, 0x00ff, /*rm B */
	0x00d3, 0x00ff, /*yw B */
	0x00d6, 0x1c00, /*GAMMA Q1d */
	0x00d7, 0x1c00,
	0x00d8, 0x1c00,
	0x00d9, 0x1c00,
	0x00da, 0x1c00,
	0x00db, 0x1c00,
	0x00dc, 0x1c00,
	0x00dd, 0x1c00,
	0x00de, 0x1c00,
	0x00df, 0x1c00,
	0x00e0, 0x1c00,
	0x00e1, 0x1c00,
	0x00e2, 0x1c00,
	0x00e3, 0xa40d,
	0x00e4, 0xa40d,
	0x00e5, 0xa40d,
	0x00e6, 0xa40d,
	0x00e7, 0xa40d,
	0x00e8, 0xa40d,
	0x00e9, 0xa40d,
	0x00ea, 0xa40d,
	0x00eb, 0x1c25,
	0x00ec, 0x174a,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_dynamic_vt[] = {
	/*start Q1 dynamic vt */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff,	/*DNR dirTh */
	0x002d, 0x19ff,	/*DNR dirnumTh decon7Th */
	0x002e, 0xff16,	/*DNR decon5Th maskTh */
	0x002f, 0x0000,	/*DNR blTh */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x03ff, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x0100, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0x0000, /*gc R */
	0x00ca, 0xffff, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xffff, /*gc G */
	0x00ce, 0x0000, /*rm G */
	0x00cf, 0xffff, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x00ff, /*gc B */
	0x00d2, 0x00ff, /*rm B */
	0x00d3, 0x00ff, /*yw B */
	0x00d6, 0x1c00, /*GAMMA Q1d */
	0x00d7, 0x1c00,
	0x00d8, 0x1c00,
	0x00d9, 0x1c00,
	0x00da, 0x1c00,
	0x00db, 0x1c00,
	0x00dc, 0x1c00,
	0x00dd, 0x1c00,
	0x00de, 0x1c00,
	0x00df, 0x1c00,
	0x00e0, 0x1c00,
	0x00e1, 0x1c00,
	0x00e2, 0x1c00,
	0x00e3, 0xa40d,
	0x00e4, 0xa40d,
	0x00e5, 0xa40d,
	0x00e6, 0xa40d,
	0x00e7, 0xa40d,
	0x00e8, 0xa40d,
	0x00e9, 0xa40d,
	0x00ea, 0xa40d,
	0x00eb, 0x1c25,
	0x00ec, 0x174a,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_movie_gallery[] = {
	/*start Q1 movie gallery */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x0009, /*HDTR CS */
	0x003f, 0x0000, /*CS GAIN */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0xa4aa, /*gc R */
	0x00ca, 0xd5fe, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xfffd, /*gc G */
	0x00ce, 0x2c4a, /*rm G */
	0x00cf, 0xf5f5, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x5cf0, /*gc B */
	0x00d2, 0x2aff, /*rm B */
	0x00d3, 0x63ee, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_movie_ui[] = {
	/*start Q1 movie ui */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x0009, /*HDTR CS */
	0x003f, 0x0000, /*CS GAIN */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0xa4aa, /*gc R */
	0x00ca, 0xd5fe, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xfffd, /*gc G */
	0x00ce, 0x2c4a, /*rm G */
	0x00cf, 0xf5f5, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x5cf0, /*gc B */
	0x00d2, 0x2aff, /*rm B */
	0x00d3, 0x63ee, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_movie_video[] = {
	/*start Q1 movie video */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x0009, /*HDTR CS */
	0x003f, 0x0000, /*CS GAIN */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0xa4aa, /*gc R */
	0x00ca, 0xd5fe, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xfffd, /*gc G */
	0x00ce, 0x2c4a, /*rm G */
	0x00cf, 0xf5f5, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x5cf0, /*gc B */
	0x00d2, 0x2aff, /*rm B */
	0x00d3, 0x63ee, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_movie_vt[] = {
	/*start Q1 movie vt */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR dirTh */
	0x002d, 0x19ff, /*DNR dirnumTh decon7Th */
	0x002e, 0xff16, /*DNR decon5Th maskTh */
	0x002f, 0x0000, /*DNR blTh */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x0000, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x0030, /*CS GAIN */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0xa4aa, /*gc R */
	0x00ca, 0xd5fe, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xfffd, /*gc G */
	0x00ce, 0x2c4a, /*rm G */
	0x00cf, 0xf5f5, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x5cf0, /*gc B */
	0x00d2, 0x2aff, /*rm B */
	0x00d3, 0x63ee, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_standard_gallery[] = {
	/*start Q1 standard gallery */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x0300, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x0030, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0x0000, /*gc R */
	0x00ca, 0xffff, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xffff, /*gc G */
	0x00ce, 0x0000, /*rm G */
	0x00cf, 0xffff, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x00ff, /*gc B */
	0x00d2, 0x00ff, /*rm B */
	0x00d3, 0x00ff, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_standard_ui[] = {
	/*start Q1 standard ui */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x0000, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x0030, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0x0000, /*gc R */
	0x00ca, 0xffff, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xffff, /*gc G */
	0x00ce, 0x0000, /*rm G */
	0x00cf, 0xffff, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x00ff, /*gc B */
	0x00d2, 0x00ff, /*rm B */
	0x00d3, 0x00ff, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_standard_video[] = {
	/*start Q1 standard video */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x0300, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x0030, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0x0000, /*gc R */
	0x00ca, 0xffff, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xffff, /*gc G */
	0x00ce, 0x0000, /*rm G */
	0x00cf, 0xffff, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x00ff, /*gc B */
	0x00d2, 0x00ff, /*rm B */
	0x00d3, 0x00ff, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_standard_vt[] = {
	/*start Q1 standard vt */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR dirTh */
	0x002d, 0x19ff, /*DNR dirnumTh decon7Th */
	0x002e, 0xff16, /*DNR decon5Th maskTh */
	0x002f, 0x0000, /*DNR blTh */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x03f8, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x00a0, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0x0000, /*gc R */
	0x00ca, 0xffff, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xffff, /*gc G */
	0x00ce, 0x0000, /*rm G */
	0x00cf, 0xffff, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x00ff, /*gc B */
	0x00d2, 0x00ff, /*rm B */
	0x00d3, 0x00ff, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_natural_gallery[] = {
	/*start Q1 natural gallery */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x0300, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x0180, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0xa4aa, /*gc R */
	0x00ca, 0xd5fe, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xfffd, /*gc G */
	0x00ce, 0x2c4a, /*rm G */
	0x00cf, 0xf5f9, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x5cf0, /*gc B */
	0x00d2, 0x2aff, /*rm B */
	0x00d3, 0x63f8, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_natural_ui[] = {
	/*start Q1 natural video */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x0000, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x0180, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0xa4aa, /*gc R */
	0x00ca, 0xd5fe, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xfffd, /*gc G */
	0x00ce, 0x2c4a, /*rm G */
	0x00cf, 0xf5f9, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x5cf0, /*gc B */
	0x00d2, 0x2aff, /*rm B */
	0x00d3, 0x63f8, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_natural_video[] = {
	/*start Q1 natural video */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x0300, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x0180, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0xa4aa, /*gc R */
	0x00ca, 0xd5fe, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xfffd, /*gc G */
	0x00ce, 0x2c4a, /*rm G */
	0x00cf, 0xf5f9, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x5cf0, /*gc B */
	0x00d2, 0x2aff, /*rm B */
	0x00d3, 0x63f8, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_natural_vt[] = {
	/*start Q1 natural vt */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR dirTh */
	0x002d, 0x19ff, /*DNR dirnumTh decon7Th */
	0x002e, 0xff16, /*DNR decon5Th maskTh */
	0x002f, 0x0000, /*DNR blTh */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x03f8, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x0180, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0xa4aa, /*gc R */
	0x00ca, 0xd5fe, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xfffd, /*gc G */
	0x00ce, 0x2c4a, /*rm G */
	0x00cf, 0xf5f9, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x5cf0, /*gc B */
	0x00d2, 0x2aff, /*rm B */
	0x00d3, 0x63f8, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

static unsigned short tune_camera[] = {
	/*start */
	0x0001, 0x0040, /*SCR HDTR */
	0x002c, 0x0fff, /*DNR bypass 0x003C */
	0x002d, 0x1900, /*DNR bypass 0x0a08 */
	0x002e, 0x0000, /*DNR bypass 0x1010 */
	0x002f, 0x0fff, /*DNR bypass 0x0400 */
	0x003a, 0x000d, /*HDTR DE CS */
	0x003b, 0x0030, /*DE SHARPNESS */
	0x003c, 0x0000, /*NOISE LEVEL */
	0x003f, 0x0030, /*CS GAIN */
	0x0042, 0x0000, /*DE TH (MAX DIFF) */
	0x00c8, 0x0000, /*kb R	SCR */
	0x00c9, 0x0000, /*gc R */
	0x00ca, 0xffff, /*rm R */
	0x00cb, 0xffff, /*yw R */
	0x00cc, 0x0000, /*kb G */
	0x00cd, 0xffff, /*gc G */
	0x00ce, 0x0000, /*rm G */
	0x00cf, 0xffff, /*yw G */
	0x00d0, 0x00ff, /*kb B */
	0x00d1, 0x00ff, /*gc B */
	0x00d2, 0x00ff, /*rm B */
	0x00d3, 0x00ff, /*yw B */
	0x00d6, 0x3f00, /*GAMMA bp2 */
	0x00d7, 0x2003,
	0x00d8, 0x2003,
	0x00d9, 0x2003,
	0x00da, 0x2003,
	0x00db, 0x2003,
	0x00dc, 0x2003,
	0x00dd, 0x2003,
	0x00de, 0x2003,
	0x00df, 0x2003,
	0x00e0, 0x2003,
	0x00e1, 0x2003,
	0x00e2, 0x2003,
	0x00e3, 0x2003,
	0x00e4, 0x2003,
	0x00e5, 0x2003,
	0x00e6, 0x2003,
	0x00e7, 0x2003,
	0x00e8, 0x2003,
	0x00e9, 0x2003,
	0x00ea, 0x2003,
	0x00eb, 0x2003,
	0x00ec, 0x2003,
	0x00ed, 0xff00,
	0x00d5, 0x0001,
	0x0028, 0x0000, /*Register Mask */
	/*end */
	END_SEQ, 0x0000
};

struct mdnie_tuning_info tuning_table[CABC_MAX][MODE_MAX][SCENARIO_MAX] = {
	{
		{
			{"dynamic_ui",		tune_dynamic_ui},
			{"dynamic_video",	tune_dynamic_video},
			{"dynamic_video",	tune_dynamic_video},
			{"dynamic_video",	tune_dynamic_video},
			{"camera",		tune_camera},
			{"dynamic_ui",		tune_dynamic_ui},
			{"dynamic_gallery",	tune_dynamic_gallery},
			{"dynamic_vt",		tune_dynamic_vt}
		}, {
			{"standard_ui",		tune_standard_ui},
			{"standard_video",	tune_standard_video},
			{"standard_video",	tune_standard_video},
			{"standard_video",	tune_standard_video},
			{"camera",		tune_camera},
			{"standard_ui",		tune_standard_ui},
			{"standard_gallery",	tune_standard_gallery},
			{"standard_vt",		tune_standard_vt}
		}, {
			{"natural_ui",		tune_natural_ui},
			{"natural_video",	tune_natural_video},
			{"natural_video",	tune_natural_video},
			{"natural_video",	tune_natural_video},
			{"camera",		tune_camera},
			{"natural_ui",		tune_natural_ui},
			{"natural_gallery",	tune_natural_gallery},
			{"natural_vt",		tune_natural_vt}
		}, {
			{"movie_ui",		tune_movie_ui},
			{"movie_video",		tune_movie_video},
			{"movie_video",		tune_movie_video},
			{"movie_video",		tune_movie_video},
			{"camera",		tune_camera},
			{"movie_ui",		tune_movie_ui},
			{"movie_gallery",	tune_movie_gallery},
			{"movie_vt",		tune_movie_vt}
		}
	}
};

#endif /* __MDNIE_TABLE_H__ */
