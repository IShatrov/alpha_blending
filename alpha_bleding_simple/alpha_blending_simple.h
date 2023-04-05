#pragma once

#include "../my_draw.h"

const int MAX_CLR = 255;
const int MAX_CLR_PW = 8; // MAX_CLR /approx 2^{MAX_CLR_PW}

const int BACK_STEP = 3;
const int FRONT_STEP = 4;

void alpha_blending();

void blend(sf::RenderWindow& window, const char* front_pixels, const char *back_pixels);

unsigned char read_byte(const char* array);

void set_rgb(RGBQUAD* clr, const char* pixels);

void set_24_clr(RGBQUAD* clr, const char* pixels);

void set_32_clr(RGBQUAD* clr, const char* pixels);

unsigned int blend_once(unsigned int front_clr, unsigned int back_clr, unsigned int alpha);

void blend_clr(RGBQUAD* res, RGBQUAD* front, RGBQUAD* back);