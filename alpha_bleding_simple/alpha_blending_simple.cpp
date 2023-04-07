#include "alpha_blending_simple.h"

const char* BACK_FILENAME = "../Table.bmp";
const char* FRONT_FILENAME = "../AskhatCat2.bmp";
const char* WINDOW_NAME = "Alpha blending";

void alpha_blending()
{
    FILE *file = fopen(BACK_FILENAME, "r");
    assert(file);
    char* back_text = read_file(file);
    char* back_pixels = prep_pxl_array(back_text);
    fclose(file);

    file = fopen(FRONT_FILENAME, "r");
    assert(file);
    char* front_text = read_file(file);
    char* front_pixels = prep_pxl_array(front_text);
    fclose(file);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), WINDOW_NAME);

    blend(window, front_pixels, back_pixels);
  
    window.display();
    wait_until_closed(window);

    free(back_text);
    free(front_text);

    return;
}

void blend(sf::RenderWindow& window, const char* front_pixels, const char *back_pixels)
{
    assert(front_pixels);
    assert(back_pixels);

    time_t start_time = start_timer();

    for(int y = 0; y < HEIGHT; y++)
    {
        for(int x = 0; x < WIDTH; x++, back_pixels += BACK_STEP, front_pixels += FRONT_STEP)
        {
            RGBQUAD back_clr;
            set_24_clr(&back_clr, back_pixels);

            RGBQUAD front_clr;
            set_32_clr(&front_clr, front_pixels);
            
            RGBQUAD clr;
            blend_clr(&clr, &front_clr, &back_clr);

            draw_pxl(window, x, HEIGHT - y, sf::Color(clr.rgbRed, clr.rgbGreen, clr.rgbBlue, clr.rgbReserved));
        }
    }

    end_timer(start_time);

    return;
}

unsigned char read_byte(const char* array)
{
    assert(array);

    return (unsigned char) *(array);
}

void set_rgb(RGBQUAD* clr, const char* pixels)
{
    assert(clr);
    assert(pixels);

    clr->rgbRed = read_byte(pixels + 2);
    clr->rgbGreen = read_byte(pixels + 1);
    clr->rgbBlue = read_byte(pixels + 0);   

    return;
}

void set_24_clr(RGBQUAD* clr, const char* pixels)
{
    assert(clr);
    assert(pixels);

    set_rgb(clr, pixels);
    clr->rgbReserved = MAX_CLR;

    return;
}

void set_32_clr(RGBQUAD* clr, const char* pixels)
{
    assert(clr);
    assert(pixels);

    set_rgb(clr, pixels);
    clr->rgbReserved = read_byte(pixels + 3);

    return;
}

unsigned int blend_once(unsigned int front_clr, unsigned int back_clr, unsigned int alpha)
{
    return (front_clr*alpha + back_clr*(MAX_CLR - alpha)) >> MAX_CLR_PW;
}

void blend_clr(RGBQUAD* res, RGBQUAD* front, RGBQUAD* back)
{
    assert(res);
    assert(front);
    assert(back);

    unsigned int alpha = front->rgbReserved;

    res->rgbRed = blend_once(front->rgbRed, back->rgbRed, alpha);
    res->rgbGreen = blend_once(front->rgbGreen, back->rgbGreen, alpha);
    res->rgbBlue = blend_once(front->rgbBlue, back->rgbBlue, alpha);
    res->rgbReserved = blend_once(front->rgbReserved, back->rgbReserved, alpha);

    return;
}