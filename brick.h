#ifndef BRICK_H
#define BRICK_H

#include "tools.h"
#include "constants.h"
#include "message.h"


class Brick {
    public :
        Brick(const Square& square_init, int type_init);
        virtual ~Brick() = default;
        Square get_square() const ;
        int get_type() const ;
        virtual bool is_valid() const ;
        virtual void draw() const = 0 ;

    protected : 
        Square square;
        int type;
};


class RainbowBrick : public Brick {

    public : 
        RainbowBrick(const Square& square_init, int hit_points_init);
        int get_hit_points() const;
        bool is_valid() const override;
        void draw() const override;

    private :
        int hit_points; //entre 1 et 7
};


class BallBrick : public Brick {
    public : 
        BallBrick(const Square& square_init);
        void draw() const override;
        
};

class SplitBrick : public Brick {
    public : 
        SplitBrick(const Square& square_init);
        void draw() const override;
};

#endif
