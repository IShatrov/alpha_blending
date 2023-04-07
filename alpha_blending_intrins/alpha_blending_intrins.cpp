#include "alpha_blending_intrins.h"

const char* BACK_FILENAME = "../Table.bmp";
const char* FRONT_FILENAME = "../AskhatCat2.bmp";
const char* WINDOW_NAME = "Alpha blending";

void alpha_blending()
{
    FILE *file = fopen(BACK_FILENAME, "r");
    assert(file);
    char* back_text = read_file(file);
    alignas(ALIGNMENT) char* back_pixels = prep_pxl_array(back_text);
    fclose(file);

    file = fopen(FRONT_FILENAME, "r");
    assert(file);
    char* front_text = read_file(file);
    alignas(ALIGNMENT) char* front_pixels = prep_pxl_array(front_text);
    fclose(file);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), WINDOW_NAME);

    blend(window, front_pixels, back_pixels);
  
    printf("calculatuins done\n");
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

    for(int y = 0; y < HEIGHT; y++)
    {
        for(int x = 0; x < WIDTH; x++, back_pixels += BACK_STEP, front_pixels += FRONT_STEP)
        {
            alignas(ALIGNMENT) __m256i back_clr = set_24_clr(back_pixels);
            //printf("back: %lld\n", &back_clr);

            alignas(ALIGNMENT) __m256i front_clr = set_32_clr(front_pixels);
            //printf("front: %lld\n", &front_clr);

            //printf("%lld %lld\n", &front_clr, &back_clr);
            //putchar('@');
            alignas(ALIGNMENT) __m256i clr;
            //putchar('&');
            clr = blend_clr(&front_clr, &back_clr);

            unsigned long long int red = _mm256_extract_epi64(clr, 1);
            unsigned long long int green = _mm256_extract_epi64(clr, 2);
            unsigned long long int blue = _mm256_extract_epi64(clr, 3);
            unsigned long long int alpha = _mm256_extract_epi64(clr, 0);

            //putchar('$');
            //draw_pxl(window, x, HEIGHT - y, sf::Color(_mm256_extract_epi64(clr, 1), _mm256_extract_epi64(clr, 2), _mm256_extract_epi64(clr, 3), _mm256_extract_epi64(clr, 0)));
            draw_pxl(window, x, HEIGHT - y, sf::Color(red, green, blue, alpha));
            //window.display();
        }
    }

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

__m256i set_24_clr(const char* pixels)
{
    assert(pixels);

    //unsigned char blue = read_byte(pixels + 0);
  //  unsigned char green = read_byte(pixels + 1);
//    unsigned char red = read_byte(pixels + 2);
    alignas(ALIGNMENT) unsigned long long int blue = read_byte(pixels + 0);
    alignas(ALIGNMENT) unsigned long long int green = read_byte(pixels + 1);
    alignas(ALIGNMENT) unsigned long long int red = read_byte(pixels + 2);
    alignas(ALIGNMENT) unsigned long long int a = 255;

    //__m256i ans = _mm256_set_epi64x(read_byte(pixels + 0), read_byte(pixels + 1), read_byte(pixels + 2), MAX_CLR);
    alignas(ALIGNMENT) __m256i ans;
    //printf("ans in 24 %lld\n", &ans);
    ans = _mm256_set_epi64x(blue, green, red, a);
    //printf("ans in 24 %lld after set\n", &ans);
    //printf("set called\n");
    return ans;
}

__m256i set_32_clr(const char* pixels)
{
    assert(pixels);

    alignas(ALIGNMENT) __m256i ans = _mm256_set_epi64x(read_byte(pixels + 0), read_byte(pixels + 1), read_byte(pixels + 2), read_byte(pixels + 3));

    return ans;
}

unsigned int blend_once(unsigned int front_clr, unsigned int back_clr, unsigned int alpha)
{
    return (front_clr*alpha + back_clr*(MAX_CLR - alpha)) >> MAX_CLR_PW;
}

__m256i blend_clr(__m256i* front, __m256i* back)
{
    //putchar('!');
   // printf("%lld\n", &front);

    unsigned int alpha = _mm256_extract_epi64(*front, 0);
    alignas(ALIGNMENT) __m256i alpha_vec = _mm256_set1_epi64x(alpha);
    alignas(ALIGNMENT) __m256i max_vec = _mm256_set1_epi64x(MAX_CLR);

    alignas(ALIGNMENT) __m256i front_frac = _mm256_mul_epu32(*front, alpha_vec);

    alpha_vec = _mm256_sub_epi64(max_vec, alpha_vec);

    __m256i back_frac = _mm256_mul_epu32(*back, alpha_vec);

    alignas(ALIGNMENT) __m256i ans = _mm256_add_epi64(front_frac, back_frac);

    ans = _mm256_srli_epi64(ans, MAX_CLR_PW);

    return ans;
}