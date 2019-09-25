//
// Created by cpasjuste on 01/12/16.
//

#ifndef C2D_SFML_TEXTURE_H_
#define C2D_SFML_TEXTURE_H_

#include "cross2d/skeleton/renderer.h"

namespace c2d {

    class SFMLTexture : public Texture {
    public:
        typedef Texture::Format Format;

    public:

        SFMLTexture(const std::string &path);

        SFMLTexture(const Vector2f &size = Vector2f(0, 0), Format format = Format::RGBA8);

        ~SFMLTexture();

        int lock(FloatRect *rect, void **pixels, int *pitch);

        void unlock();

        void setFiltering(int filter);

        //private:
        sf::Texture texture;
        sf::Uint8 *pixels = nullptr;
    };
}

#endif //_SFML_TEXTURE_H_
