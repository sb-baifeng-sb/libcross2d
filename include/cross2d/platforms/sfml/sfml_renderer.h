//
// Created by cpasjuste on 21/11/16.
//

#ifndef C2D_SFML_RENDERER_H
#define C2D_SFML_RENDERER_H

#include <SFML/Graphics.hpp>
#include "cross2d/skeleton/renderer.h"
#include "sfml_shaders.h"

namespace c2d {

    class SFMLRenderer : public Renderer {

    public:

        SFMLRenderer(const Vector2f &size = Vector2f(0, 0), const std::string &shaderPath = "");

        ~SFMLRenderer();

        void draw(VertexArray *vertexArray, const Transform &transform, Texture *texture) override;

        void flip(bool draw = true, bool inputs = true) override;

        void delay(unsigned int ms) override;

        //private:
        sf::RenderWindow* window;
        sf::VideoMode mode;
    };
}

#endif //SFML_RENDERER_H
