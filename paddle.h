#ifndef PADDLE_H
#define PADDLE_H

#include "tools.h"

// Classe représentant la raquette du jeu
// La raquette est un cercle qui doit toucher le bas de l'arène (y = 0)
class Paddle {
    public:
        // Constructeur: initialise la raquette avec un cercle
        Paddle(const Circle& circle_init);
        
        // Retourne le cercle représentant la raquette
        Circle get_circle() const;
        
        // Vérifie que la raquette est valide (dans l'arène et touche le sol)
        // Retourne vrai si la raquette est correctement positionnée
        bool is_valid() const;

    private:
        // Cercle représentant la géométrie de la raquette
        Circle circle;
};

#endif
