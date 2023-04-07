#ifndef SNAKEBODY_H
#define SNAKEBODY_H

class SnakeBody {
public:
    SnakeBody(int x, int y);
    void move(int x, int y);
    void setNext(SnakeBody* next);
    void moveNext(int x, int y);
    int getX();
    int getY();
    void paint(int screenWidth, int screenHeight, wchar_t* screen);

private:
    int positionX;
    int positionY;
    SnakeBody* next;
};

#endif
