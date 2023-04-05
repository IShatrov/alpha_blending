#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <wingdi.h>

#include "my_draw_consts.h"

void draw_pxl(sf::RenderWindow& window, float x, float y, sf::Color clr);

size_t get_file_size(FILE *file);

char* read_file(FILE *file);

char* prep_pxl_array(char* bmp_content);

void wait_until_closed(sf::RenderWindow& window);