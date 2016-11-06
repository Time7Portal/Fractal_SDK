#pragma once
#include <vector>


namespace Fractal
{
	extern int DecodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true); // PNG �̹��� ���ڵ� �ϴ� �Լ� | Decode a png image file
}