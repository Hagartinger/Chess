#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include <SDL.h>
#include <SDL_image.h>
#include <map>

class TextureHandler{
public:
	TextureHandler();
	~TextureHandler();
	
	//If texture wasn't loaded earlier loads it from "resources/" + textureName + ".png". 
	//Otherwise returns pointer to texture
	SDL_Texture* load(SDL_Renderer* renderer,  std::string textureName);
	
private:
	std::map<std::string , SDL_Texture*> textures;
	
};

#endif