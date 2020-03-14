#ifndef PALETTES_H
#define PALETTES_H

#include <FastLED.h>

DEFINE_GRADIENT_PALETTE( PAL_HEATMAP_TEST ) {
  0,     0,  0,  0,   //black
128,   255,  0,  0,   //red
224,   255,255,  0,   //bright yellow
255,   255,255,255 }; //full white

DEFINE_GRADIENT_PALETTE( PAL_QUAD_TEST ) {
  0,   255,  0,  0,   // red
 62,   255,  0,  0,   // red
125,     0,255,  0,   // green
188,     0,  0,255,   // blue
251,   255,255,  0,   // yellow
255,   255,255,  0, };// yellow

DEFINE_GRADIENT_PALETTE( PAL_HALLOWEEN_GEN ) {
  0,    0,    0,    0,
  32,    0,    21,   140, // midnight blue
  128,   0,    196,  84,  // emerald green
  170,  232,  104,  0,   // pumpkin orange
  230,  232,  104,  0,   // pumpkin orange
  255,  255,  255,  255, // full white
};

DEFINE_GRADIENT_PALETTE( PAL_HALLOWEEN_PUMPKIN ) {
  0,    0,    0,    0,  // black
  16,   100,  25,   25, // dark red
  96,   230,  10,   10, // blood red
  120,  255,  150,  40, // pumpkin orange
  245,  255,  255,  40, // candle yellow
  255,  255,  255,  255, // pure white
};

DEFINE_GRADIENT_PALETTE( PAL_HALLOWEEN_GHOUL ) {
  0,    0,    0,    0,    // black
  16,   90,   0,    120,  // dark purple
  64,   160,  30,   160,  // light purple
  200,  0,    150,  40,   // green
  255,  255,  255,  255   // pure white
};

DEFINE_GRADIENT_PALETTE( PAL_POINTY_PARTY ) {
  0,    0,    0,    0, // black
  16,   150,  44,   44, // Dark Rose
  180,  208,  100,  100, // Old Rose
  210,  220,  220,  220, // Grey
  255,  255,  255,  255, // full while
};

DEFINE_GRADIENT_PALETTE( PAL_REVRAINBOW ) {
  0,     148,   0,    211,   // v
  42,    75,    0,    130,  // i
  84,    0,     0,    255,  // b
  126,   0,     255,  0,    // g
  168,   255,   255,  0,   // y
  210,   255,   127,  0,   // o
  255,   255,   0,    0,    // r
};

DEFINE_GRADIENT_PALETTE( PAL_MAGMA ) {
	0,		48,		11,		102,		// Deep Violet
	50,		132,	32,		130,		// Maximum Purple
	125,	199,	57,		116,		// Fuchsia Rose
	175,	254,	138,	92,			// Coral
	255,	252,	252,	186,		// Very Pale Yellow
};

DEFINE_GRADIENT_PALETTE( PAL_FOREST ) {
	0,		11,		102,	15,		// Deep Green
	50,		9,		155,	106,	// Green (NCS)
	125,	33,		198,	190,	// Maxmimum Blue-Green
	175,	59,		134,	191,	// Cyan-Blue Azure
	255,	164,	165,	191,	// Blue Bell
};

DEFINE_GRADIENT_PALETTE( PAL_MONOCHROME )
{
  0,    1,    1,    1,    // Black
  230,  127,  127,  127,  // Grey
  255,  255,  255,  255,    // White
};

DEFINE_GRADIENT_PALETTE ( PAL_BEACHY )
{
  0,    0,    12,   188,    // Blue
  100,  210,  184,  0,      // Yellow
  180,  0,    175,  0,      // Green
  255,  0,    90,   255,    // Blue, but Different!
};

#endif