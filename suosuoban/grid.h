#ifndef GRID_H
#define GRID_H

#include <cstdlib>
#include <cstdio>
using namespace std;

template <typename T> class Grid
{
public:
    Grid(int xsize, int ysize)
    {
        this->xsize = xsize;
        this->ysize = ysize;
        int i;

        value = new T* [ysize];
        for (i=0;i<ysize;i++){
            *(value+i) = new T[xsize];
        }
    }
    ~Grid(){
        int i;


        for (i=0;i<ysize;i++){
            delete *(value+i);
        }
        delete value;
    }

    int xsize;//how many col
    int ysize;//how many row

    T getValue(int x, int y){
        return value[y][x];
    }

    void setValue(int x, int y, T value){
        this->value[y][x] = value;
    }

private:
    T** value;
};

#endif // GRID_H
