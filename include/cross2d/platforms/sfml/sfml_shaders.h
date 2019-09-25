//
// Created by cpasjuste on 27/01/17.
//

#ifndef C2D_SFML_SHADERS_H
#define C2D_SFML_SHADERS_H

#include "cross2d/skeleton/shader_list.h"

namespace c2d {

    class SFMLShaders : ShaderList {

    public:

        SFMLShaders(const std::string &shadersPath);

        virtual ~SFMLShaders();
    };
}

#endif //CROSS2D_SFML_SHADERS_H
