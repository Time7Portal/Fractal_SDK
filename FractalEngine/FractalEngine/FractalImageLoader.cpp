#include "FractalImageLoader.h"


/* - GLTexture 로 이미지 파일을 반환하는 함수 | Loads images into GLTexturs (File path) - */
Fractal::GLTexture Fractal::ImageLoader::LoadPNG(std::string filePath)
{
	// Create a GLTexture and initialize all its fields to 0
	GLTexture texture = {};

	// This is the input data to decodePNG, which we load from a file
	std::vector<unsigned char> in;
	// This is the output data from decodePNG, which is the pixel data for our texture
	std::vector<unsigned char> out;

	unsigned long width, height;

	// Read in the image file contents into a buffer
	if (FileIOManager::ReadFileToBuffer(filePath, in) == false)
	{
		Fractal::Debug::GetInstance()->ConsoleError("Unable to read a PNG file!");
	}

	// Decode the .png format into an array of pixels
	int errorCode = DecodePNG(out, width, height, &(in[0]), in.size());

	if (errorCode != 0)
	{
		Fractal::Debug::GetInstance()->ConsoleError(std::to_string(errorCode) + " - Unable to decode the PNG file!");
	}

	// Generate the openGL texture object
	glGenTextures(1, &(texture.id));

	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, texture.id);

	// Upload the pixels to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

	// Set some texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	return texture;
}