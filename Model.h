// Define Model

#ifndef _MODEL_H_
#define _MODEL_H_

class Model{
public:
    int width;
    int height;
    int size;
    int start;
    int *next;
    Model();
    Model(int _width, int _height, int _size, int _start);
};

#endif 