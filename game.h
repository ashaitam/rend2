#ifndef GAME_H
#define GAME_H

#include "tools.h"
#include "ball.h"
#include "brick.h"
#include "paddle.h"
#include "constants.h"
#include "message.h"
#include <iostream>
#include <vector>
#include <memory>

class Game {
    public : 
    // Etat initial de la partie.
    Game(int score_init, int lives_init);
    
    // Ajouts avec verification de validite/collision.
    bool add_brick(std::unique_ptr<Brick> b);
    bool add_ball(const Ball& b);
    bool set_paddle(const Paddle& p);

    private : 
        // Score et vies courants.
        int score;
        int lives;

        // Entites en jeu.
        std::unique_ptr<Paddle> paddle;
        std::vector<std::unique_ptr<Brick>> bricks;
        std::vector<Ball> balls;
};

#endif
