//
// Created by cpasjuste on 21/11/16.
//

#if defined(__MAC__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "cross2d/c2d.h"

using namespace c2d;

SFMLRenderer::SFMLRenderer(const Vector2f &size, const std::string &shaderPath) : Renderer(size) {

    sf::ContextSettings settings(32, 0, 0);
    mode = sf::VideoMode::getDesktopMode();
    sf::Uint32 style = sf::Style::Titlebar | sf::Style::Close; //sf::Style::Fullscreen;

    // windowed
    if (getSize().x != 0 && getSize().y != 0) {
        mode.width = (unsigned int) getSize().x;
        mode.height = (unsigned int) getSize().y;
        //style = sf::Style::Default;
    }

    window = new sf::RenderWindow(mode, "demo", style, settings);
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);

    printf("SFMLRenderer: %i x %i @ %i bpp\n", mode.width, mode.height, mode.bitsPerPixel);
    const unsigned char *glversion = glGetString(GL_VERSION);
    printf("SFMLRenderer: glversion: %s\n", glversion);
    const unsigned char *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("SFMLRenderer: glslversion: %s\n", glslversion);

    shaderList = (ShaderList*) new SFMLShaders(shaderPath);
}

void SFMLRenderer::draw(VertexArray *vertexArray, const Transform &transform, Texture *texture) {

    sf::RenderStates states;
    states.transform = ((sf::Transform &) transform);
    if (texture) {
        states.texture = &((SFMLTexture *) texture)->texture;

        sf::Shader *shader = (sf::Shader *) shaderList->get(0)->data;
        if (shader) {
            shader->setUniform("Texture", ((SFMLTexture *) texture)->texture);
            shader->setUniform("MVPMatrix", sf::Glsl::Mat4(
                    window->getView().getTransform().getMatrix()));
            shader->setUniform("TextureSize", sf::Glsl::Vec2(
                    texture->getTextureRect().width,
                    texture->getTextureRect().height));
            shader->setUniform("InputSize", sf::Glsl::Vec2(
                    texture->getTextureRect().width,
                    texture->getTextureRect().height));
            shader->setUniform("OutputSize", sf::Glsl::Vec2(
                    texture->getSize().x,
                    texture->getSize().y));
            states.shader = shader;
        }

    }

    auto v = *vertexArray;
    window->draw((sf::Vertex *) (*v.getVertices()).data(), v.getVertexCount(),
                (sf::PrimitiveType) v.getPrimitiveType(), states);

}

void SFMLRenderer::flip(bool draw, bool inputs) {

    // clear screen
    auto c = this->getClearColor();
    window->clear(sf::Color(c.r, c.g, c.b, c.a));

    // call base class (draw childs)
    Renderer::flip();

    // flip
    window->display();
}

void SFMLRenderer::delay(unsigned int ms) {
    sf::sleep(sf::milliseconds(ms));
}

SFMLRenderer::~SFMLRenderer() {
    delete window;
}
