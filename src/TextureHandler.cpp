#include <iostream>

#include "TextureHandler.h"


TextureHandler::TextureHandler(){
}

TextureHandler::~TextureHandler(){
}

SDL_Texture* TextureHandler::load(SDL_Renderer* renderer, std::string textureName)
{
	if(textures[textureName] == 0)
	{//don't have such texture
		std::string filePath =  "../resources/" + textureName + ".png";
		SDL_Surface* tempSurface = 0;
		tempSurface = IMG_Load(filePath.c_str());
		if (tempSurface == 0)
		{
			std::cerr<<"Couldn't open File from: "<<filePath<<std::endl;
		}
		else
		{
			SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
			if(newTexture == 0)
			{
				std::cerr<<"Creation of Texture from surace went wrong"<<std::endl;
			}
			else
			{
				textures[textureName] =  newTexture ;
			}
			SDL_FreeSurface(tempSurface);
		}
	}
	return textures[textureName];
}

