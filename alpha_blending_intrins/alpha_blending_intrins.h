#pragma once

#include "../my_draw.h"
#include <immintrin.h>

#include <stdalign.h>

const int MAX_CLR = 255;
const int MAX_CLR_PW = 8; // MAX_CLR /approx 2^{MAX_CLR_PW}

const int BACK_STEP = 3;
const int FRONT_STEP = 4;

const int ALIGNMENT = 32;

void alpha_blending();

void blend(sf::RenderWindow& window, const char* front_pixels, const char *back_pixels);

unsigned char read_byte(const char* array);

__m256i* set_24_clr(const char* pixels);

__m256i* set_32_clr(const char* pixels);

__m256i* blend_clr(__m256i* front, __m256i* back);