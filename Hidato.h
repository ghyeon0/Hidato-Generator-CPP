//Define Hidato Functions

#include "Model.h"
#include "Grid.h"
#include "Group.h"

#ifndef _HIDATO_H_
#define _HIDATO_H_

class Hidato{
public:
    int rand_int(int n);
    double rand_double();
    int rand_neighbor(int width, int height, int index);
    void display(int width, int height, int *grid);
    void gen_init(Model *model, int width, int height);
    void gen_randomize(Model *model);
    void gen_uninit(Model *model);
    int gen_extract(Model *model, int *grid);
    void gen_display(Model *model);
    int gen_energy(Model *model);
    int gen_do_move(Model *model);
    void gen_undo_move(Model *model, int undo_data);
    void gen_copy(Model *dst, Model *src);
    int gen_anneal(Model *model, double max_temp, double min_temp, int steps);
    void gen(int width, int height, int *output);
    void generate(int width, int height);
};

#endif