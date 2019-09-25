//
// Created by cpasjuste on 11/01/17.
//

#include <SFML/Window/Event.hpp>
#include "cross2d/platforms/sfml/sfml_renderer.h"
#include "cross2d/platforms/sfml/sfml_input.h"
#include "cross2d/skeleton/input.h"

using namespace c2d;

static int key_id[KEY_COUNT]{
        Input::Key::Up,
        Input::Key::Down,
        Input::Key::Left,
        Input::Key::Right,
        Input::Key::Select,
        Input::Key::Start,
        Input::Key::Fire1,
        Input::Key::Fire2,
        Input::Key::Fire3,
        Input::Key::Fire4,
        Input::Key::Fire5,
        Input::Key::Fire6
};

SFMLInput::SFMLInput(Renderer *r) : Input(), renderer(r) {

    int joy_count = 0;

    for (int i = 0; i < 4; i++) {
        if (sf::Joystick::isConnected(i)) {
            printf("Joystick: %i\n", i);
            players[i].enabled = true;
            players[i].id = i;
            sf::Joystick::Identification id = sf::Joystick::getIdentification(i);
            printf("Buttons %d\n", sf::Joystick::getButtonCount(i));
            joy_count++;
        }
    }

    printf("%d Joystick(s) Found\n", joy_count);

    // allow keyboard mapping to player1
    players[0].enabled = true;

    for (int i = 0; i < PLAYER_MAX; i++) {
        for (int k = 0; k < KEY_COUNT; k++) {
            players[i].mapping[k] = 0;
        }
    }

    for (int i = 0; i < KEY_COUNT; i++) {
        keyboard.mapping[i] = 0;
    }
}

SFMLInput::~SFMLInput() {

    for (int i = 0; i < PLAYER_MAX; i++) {
        players[i].enabled = false;
        players[i].id = -1;
    }
}

int SFMLInput::waitButton(int player) {
    sf::Event event;
    while (((SFMLRenderer *) renderer)->window->pollEvent(event)) {
        if (event.type == sf::Event::JoystickButtonPressed) {
            if ((int) event.joystickButton.joystickId == player) {
                return event.joystickButton.button;
            }
        }
    }
    return -1;
}

Input::Player *SFMLInput::update(int rotate) {

    for (int i = 0; i < PLAYER_MAX; i++) {
        players[i].keys = 0;
    }


    SFMLRenderer *rdr = ((SFMLRenderer *) renderer);

    sf::Event event = {};
    while (((SFMLRenderer *) renderer)->window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            players[0].keys |= EV_QUIT;
            return players;
        }

        if (event.type == sf::Event::Resized) {
            sf::View v = sf::View(
                    sf::FloatRect(0.f, 0.f, rdr->window->getSize().x, rdr->window->getSize().y));
            rdr->window->setView(v);
            players[0].keys |= EV_RESIZE;
            return players;
        }

        if (event.type == sf::Event::KeyPressed) {
            //printf("%i\n", (int) event.key.code);
        }
        if (event.type == sf::Event::JoystickButtonPressed) {
            //printf("%i\n", (int) event.joystickButton.button);
        }

    }

    for (int i = 0; i < PLAYER_MAX; i++) {

        if (!players[i].enabled) {
            continue;
        }

        // hat
        process_hat(players[i], rotate);

        // sticks
        process_axis(players[i], rotate);

        // buttons
        process_buttons(players[i], rotate);
    }

    // keyboard
    process_keyboard(players[0], rotate);

    return players;
}

void SFMLInput::process_axis(Input::Player &player, int rotate) {

    if (!player.enabled) {
        return;
    }

    // X AXIS
    if (sf::Joystick::hasAxis((unsigned int) player.id, sf::Joystick::X)) {
        int x = (int) sf::Joystick::getAxisPosition((unsigned int) player.id, sf::Joystick::X) * 320;
        if (x > player.dead_zone) {
            player.lx.value = (short) x;
            player.keys |= (rotate == 1) ? Input::Key::Down : (rotate == 3) ? Input::Key::Up
                                                                                 : Input::Key::Right;
        } else if (x < -player.dead_zone) {
            player.lx.value = (short) x;
            player.keys |= (rotate == 1) ? Input::Key::Up : (rotate == 3) ? Input::Key::Down
                                                                               : Input::Key::Left;
        } else {
            player.lx.value = 0;
        }
    }

    // Y AXIS
    if (sf::Joystick::hasAxis((unsigned int) player.id, sf::Joystick::Y)) {
        int y = (int) sf::Joystick::getAxisPosition((unsigned int) player.id, sf::Joystick::Y) * 320;
        if (y > player.dead_zone) {
            player.ly.value = (short) y;
            player.keys |= (rotate == 1) ? Input::Key::Left : (rotate == 3) ? Input::Key::Right
                                                                                 : Input::Key::Down;
        } else if (y < -player.dead_zone) {
            player.keys |= (rotate == 1) ? Input::Key::Right : (rotate == 3) ? Input::Key::Left
                                                                                  : Input::Key::Up;
            player.ly.value = (short) y;
        } else {
            player.ly.value = 0;/**/
        }
    }
}

void SFMLInput::process_hat(Input::Player &player, int rotate) {

    if (!player.enabled) {
        return;
    }

    // X AXIS
    if (sf::Joystick::hasAxis((unsigned int) player.id, sf::Joystick::PovX)) {
        int x = (int) sf::Joystick::getAxisPosition((unsigned int) player.id, sf::Joystick::PovX) * 320;
        if (x > player.dead_zone) {
            player.keys |= (rotate == 1) ? Input::Key::Down : (rotate == 3) ? Input::Key::Up
                                                                                 : Input::Key::Right;
        } else if (x < -player.dead_zone) {
            player.keys |= (rotate == 1) ? Input::Key::Up : (rotate == 3) ? Input::Key::Down
                                                                               : Input::Key::Left;
        }
    }

    // Y AXIS
    if (sf::Joystick::hasAxis((unsigned int) player.id, sf::Joystick::PovY)) {
        int y = (int) sf::Joystick::getAxisPosition((unsigned int) player.id, sf::Joystick::PovY) * 320;
        if (y > player.dead_zone) {
            player.keys |= (rotate == 1) ? Input::Key::Left : (rotate == 3) ? Input::Key::Right
                                                                                 : Input::Key::Down;
        } else if (y < -player.dead_zone) {
            player.keys |= (rotate == 1) ? Input::Key::Right : (rotate == 3) ? Input::Key::Left
                                                                                  : Input::Key::Up;
        }
    }

}

void SFMLInput::process_buttons(Input::Player &player, int rotate) {

    if (!player.enabled) {
        return;
    }

    for (int i = 0; i < KEY_COUNT; i++) {

        int mapping = player.mapping[i];
        if (mapping < 0)
            mapping = 0;

        if (sf::Joystick::isButtonPressed((unsigned int) player.id, (unsigned int) mapping)) {
            if (rotate && key_id[i] == Input::Key::Up) {
                if (rotate == 1) {
                    player.keys |= Input::Key::Right;
                } else if (rotate == 3) {
                    player.keys |= Input::Key::Left;
                }
            } else if (rotate && key_id[i] == Input::Key::Down) {
                if (rotate == 1) {
                    player.keys |= Input::Key::Left;
                } else if (rotate == 3) {
                    player.keys |= Input::Key::Right;
                }
            } else if (rotate && key_id[i] == Input::Key::Left) {
                if (rotate == 1) {
                    player.keys |= Input::Key::Up;
                } else if (rotate == 3) {
                    player.keys |= Input::Key::Down;
                }
            } else if (rotate && key_id[i] == Input::Key::Right) {
                if (rotate == 1) {
                    player.keys |= Input::Key::Down;
                } else if (rotate == 3) {
                    player.keys |= Input::Key::Up;
                }
            } else {
                player.keys |= key_id[i];
            }
        }
    }
}

void SFMLInput::process_keyboard(Input::Player &player, int rotate) {

    for (int i = 0; i < KEY_COUNT; i++) {
        sf::Keyboard::Key key((sf::Keyboard::Key) keyboard.mapping[i]);
        if (sf::Keyboard::isKeyPressed(key)) {
            if (rotate && key_id[i] == Input::Key::Up) {
                player.keys |= Input::Key::Right;
            } else if (rotate && key_id[i] == Input::Key::Down) {
                player.keys |= Input::Key::Left;
            } else if (rotate && key_id[i] == Input::Key::Left) {
                player.keys |= Input::Key::Up;
            } else if (rotate && key_id[i] == Input::Key::Right) {
                player.keys |= Input::Key::Down;
            } else {
                player.keys |= key_id[i];
            }
        }
    }
}
