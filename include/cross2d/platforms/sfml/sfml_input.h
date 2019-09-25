//
// Created by cpasjuste on 11/01/17.
//

#ifndef C2D_SFML_INPUT_H
#define C2D_SFML_INPUT_H

namespace c2d {

    class SFMLInput : public Input {

    public:

        SFMLInput(Renderer *renderer);

        virtual ~SFMLInput();

        virtual int waitButton(int player = 0) override;

        virtual Player *update(int rotate = 0) override;

    private:
        virtual void process_axis(Player &player, int rotate = 0);

        virtual void process_hat(Player &player, int rotate = 0);

        virtual void process_buttons(Player &player, int rotate = 0);

        virtual void process_keyboard(Player &player, int rotate = 0);

    private:
        Renderer* renderer;
    };

}

#endif //_SFML_INPUT_H
