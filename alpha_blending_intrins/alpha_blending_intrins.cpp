#include "alpha_blending_intrins.h"

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

    time_t star_time = start_timer();

    for(int y = 0; y < HEIGHT; y++)
    {
        for(int x = 0; x < WIDTH; x++, back_pixels += BACK_STEP, front_pixels += FRONT_STEP)
        {
            __m256i* back_clr = set_24_clr(back_pixels);

            __m256i* front_clr = set_32_clr(front_pixels);

            __m256i* clr = blend_clr(front_clr, back_clr);

            unsigned long long int red = _mm256_extract_epi64(*clr, 1);
            unsigned long long int green = _mm256_extract_epi64(*clr, 2);
            unsigned long long int blue = _mm256_extract_epi64(*clr, 3);
            unsigned long long int alpha = _mm256_extract_epi64(*clr, 0);

            draw_pxl(window, x, HEIGHT - y, sf::Color(red, green, blue, alpha));

            _aligned_free(back_clr);
            _aligned_free(front_clr);
            _aligned_free(clr);
        }
    }

    end_timer(star_time);

    return;
}

unsigned char read_byte(const char* array)
{
    assert(array);

    return (unsigned char) *(array);
}

__m256i* set_24_clr(const char* pixels)
{
    assert(pixels);

    unsigned long long int blue = read_byte(pixels + 0); //i know that uint64_t exists, just like it more this way
    unsigned long long int green = read_byte(pixels + 1);
    unsigned long long int red = read_byte(pixels + 2);
    unsigned long long int alpha = MAX_CLR;

    __m256i* ans = (__m256i*) _aligned_malloc(sizeof(__m256i), ALIGNMENT);

    *ans = _mm256_set_epi64x(blue, green, red, alpha);

    return ans;
}

__m256i* set_32_clr(const char* pixels)
{
    assert(pixels);

    unsigned long long int blue = read_byte(pixels + 0);
    unsigned long long int green = read_byte(pixels + 1);
    unsigned long long int red = read_byte(pixels + 2);
    unsigned long long int alpha = read_byte(pixels + 3);
    
    __m256i* ans = (__m256i*) _aligned_malloc(sizeof(__m256i), ALIGNMENT);

    *ans = _mm256_set_epi64x(blue, green, red, alpha);

    return ans;
}

__m256i* blend_clr(__m256i* front, __m256i* back)
{
    assert(front);
    assert(back);

    unsigned int alpha = _mm256_extract_epi64(*front, 0);
    __m256i alpha_vec = _mm256_set1_epi64x(alpha);
    __m256i max_vec = _mm256_set1_epi64x(MAX_CLR);

    __m256i front_frac = _mm256_mul_epu32(*front, alpha_vec);

    alpha_vec = _mm256_sub_epi64(max_vec, alpha_vec);

    __m256i back_frac = _mm256_mul_epu32(*back, alpha_vec);

    __m256i* ans = (__m256i*) _aligned_malloc(sizeof(__m256i), ALIGNMENT);

    *ans = _mm256_add_epi64(front_frac, back_frac);
    *ans = _mm256_srli_epi64(*ans, MAX_CLR_PW);

    return ans;
}