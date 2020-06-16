#include "Texture2D.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

Texture2D& Texture2D::LoadTexFromFile(SDL_Renderer* renderer ,const std::string& path)
{
	SDL_DestroyTexture(texture);

	//The final texture
	SDL_Texture* finalTex = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		//Create texture from surface pixels
		finalTex = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (finalTex == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			Height = loadedSurface->h;
			Width = loadedSurface->w;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	texture = finalTex;

	return *this;

}

Texture2D& Texture2D::loadFromRenderedText(SDL_Renderer* renderer,TTF_Font* font,const std::string& textureText, const SDL_Color& textColor)
{
	//TODO: Add font loading function
	SDL_DestroyTexture(texture);
	//Render text surface
	
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == nullptr)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			Width = textSurface->w;
			Height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
		textSurface = nullptr;
	}
	return *this;
}

void Texture2D::SetColor(Uint8 r, Uint8 g, Uint8 b) const
{
	SDL_SetTextureColorMod(texture, r, g, b);
}

void Texture2D::setBlendMode(SDL_BlendMode blending) const
{
	SDL_SetTextureBlendMode(texture, blending);
}

void Texture2D::setAlpha(Uint8 alpha) const
{
	SDL_SetTextureAlphaMod(texture, alpha);
}

