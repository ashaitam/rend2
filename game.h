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
#include <fstream>
#include <sstream>

class Game {
    public : 
    // Etat initial de la partie.
    Game(int score_init, int lives_init);
    Game();

    // Ajouts avec verification de validite/collision.
    bool add_brick(std::unique_ptr<Brick> b);
    bool add_ball(const Ball& b);
    bool set_paddle(const Paddle& p);

    int get_score() const;
    int get_lives() const;
    std::size_t get_nb_bricks() const;
    std::size_t get_nb_balls() const;
    const Paddle* get_paddle() const;
    const std::vector<std::unique_ptr<Brick>>& get_bricks() const;
    const std::vector<Ball>& get_balls() const;
    
    void clear();

    bool load(const std::string& file_name);
    bool save(const std::string& file_name) const;



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
