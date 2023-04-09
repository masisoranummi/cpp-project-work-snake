#ifndef SNAKEPART_H
#define SNAKEPART_H

// Tein nyt vielä jälkeenpäin tämän, jotta saa täydet pisteet
// Aika simppeli luokka jossa on x ja y sijainnit. SnakeHead 
// ja SnakeBody periytyvät molemmat tästä
class SnakePart {
public:
    int positionX;
    int positionY;
    SnakePart(int x, int y) {
        positionX = x;
        positionY = y;
    }
};

#endif