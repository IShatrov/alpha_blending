#include "my_draw_consts.h"
#include "my_draw.h"

void draw_pxl(sf::RenderWindow& window, float x, float y, sf::Color clr)
{
	sf::RectangleShape pixel(sf::Vector2f(1, 1));
	pixel.setFillColor(clr);

	pixel.setPosition(x, y);
	window.draw(pixel);

	return;
}

size_t get_file_size(FILE *file)
{
    assert(file);

    size_t ans = 0;

    assert(!fseek(file, 0, SEEK_END));   //fseek returns zero upon success and non-zero otherwise
    ans = ftell(file);
    assert(!ferror(file));

    assert(!fseek(file, 0, SEEK_SET));

    return ans;
}

char* read_file(FILE *file)
{
    assert(file);

    size_t len = get_file_size(file);
    assert(len != 0);

    char* content = (char*) calloc(len, sizeof(char));
    assert(content);

    fread(content, sizeof(char), len, file);
    assert(!ferror(file));

    return content;
}

char* prep_pxl_array(char* bmp_content)
{
	PBITMAPFILEHEADER bmp_header = (PBITMAPFILEHEADER)(&bmp_content[0]);

	int pxl_offset = bmp_header->bfOffBits;

	return &bmp_content[pxl_offset];
}

void wait_until_closed(sf::RenderWindow& window)
{
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	return;
}