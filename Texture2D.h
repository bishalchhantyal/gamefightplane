#pragma once
#include <SDL.h>
#include <iostream>
#include <SDL_ttf.h>
using namespace std;
class Texture2D
{
public:

	Texture2D() { texture = nullptr; }
	//Copy constructor
	explicit Texture2D(SDL_Texture* tex) :texture(tex) {}
	Texture2D(const Texture2D&) = default;
	//Assignment
	Texture2D& operator =(const Texture2D& tex) {
		if (texture == tex.texture)return *this;
		if (texture) {
			SDL_DestroyTexture(texture);
		}
		texture = tex.texture;
		Width = tex.Width;
		Height = tex.Height;
		return *this;
	}

	Texture2D& operator =(Texture2D&& tex) noexcept
	{
		if (texture == tex.texture)return *this;
		if (texture) {
			SDL_DestroyTexture(texture);
		}
		texture = tex.texture;
		Width = tex.Width;
		Height = tex.Height;
		tex.texture = nullptr;
		return *this;
	}
	//Destructor
	~Texture2D() {
		if (texture) {
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}

	//Get image's dimensions
	int getWidth() const { return Width; }
	int getHeight() const { return Height; }


	//Get SDL_Texture* pointer
	SDL_Texture* GetPtr() const { return texture; }
	//Load Texture2D from file
	Texture2D& LoadTexFromFile(SDL_Renderer* renderer, const std::string& path);
	//Creates image from font string
	Texture2D& Texture2D::loadFromRenderedText(SDL_Renderer* renderer, 
		TTF_Font* font, 
		const std::string& textureText,
		const SDL_Color& textColor);
	//Set picture color
	void SetColor(Uint8 r, Uint8 g, Uint8 b) const;
	//Set blending
	void setBlendMode(SDL_BlendMode blending) const;
	//Set alpha modulation
	void setAlpha(Uint8 alpha) const;

private:

	SDL_Texture* texture = nullptr;
	//Image dimensions
	int Width = 0;
	int Height = 0;

};

