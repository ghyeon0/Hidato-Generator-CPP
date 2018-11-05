#include "Generator.h"
#include <ctime>
#include <cmath>    
#include <cstdlib>
#include <fstream>
#include <iostream>

void Generator::generate(int width, int height){
    srand((unsigned int)time(NULL));
    int output[width * height];
    gen(width, height, output);
    display(width, height, output);
    std::ofstream out("output.txt");
    out << height << " " << width << std::endl;
    for(int i = 0;i < height;i++){
        for(int j = 0;j < width;j++){
            out << output[i * width + j] << " ";
        }
        out << std::endl;
    }
}

int Generator::rand_int(int n){
    int result;
    while(n <= (result = rand() / (RAND_MAX / n)));
    return result;
}

double Generator::rand_double(){
    return (double)rand() / (double)RAND_MAX;
}

int Generator::rand_neighbor(int width, int height, int index) {
    int x = index % width;
    int y = index / width;
    while (1) {
        int dx = rand_int(3) - 1;
        int dy = rand_int(3) - 1;
        if (dx == 0 && dy == 0) {
            continue;
        }
        int nx = x + dx;
        int ny = y + dy;
        if (nx < 0 || nx >= width) {
            continue;
        }
        if (ny < 0 || ny >= height) {
            continue;
        }
        return ny * width + nx;
    }
}

void Generator::display(int width, int height, int *grid) {
    int size = width * height;
    for (int y = 0; y < height; y++) {
        std::cout << "+";
        for (int x = 0; x < width; x++) {
            std::cout << "---+";
        }
        std::cout << "\n|";
        for (int x = 0; x < width; x++) {
            int i = y * width + x;
            if (grid[i]) {
                std::cout.width(3);
                std::cout << grid[i] << "|";
            }
            else {
                std::cout << "   |";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "+";
    for (int x = 0; x < width; x++) {
        std::cout << "---+";
    }
    std::cout << "\n" << std::endl;
}

void Generator::gen_init(Model *model, int width, int height) {
    int size = width * height;
    model -> width = width;
    model -> height = height;
    model -> size = size;
    model -> start = rand_int(size);
    model -> next = new int[size];
    for(int i = 0;i < size;i++){
        model -> next[i] = 0;
    }
}

void Generator::gen_randomize(Model *model) {
    for (int i = 0; i < model->size; i++) {
        model->next[i] = rand_neighbor(model->width, model->height, i);
    }
}

void Generator::gen_uninit(Model *model) {
    delete[] model -> next;
}

int Generator::gen_extract(Model *model, int *grid) {
    for (int i = 0; i < model->size; i++) {
        grid[i] = 0;
    }
    int index = model->start;
    int number = 1;
    int result = 0;
    while (grid[index] == 0) {
        result++;
        grid[index] = number++;
        index = model->next[index];
    }
    return result;
}

void Generator::gen_display(Model *model) {
    int grid[model->size];
    gen_extract(model, grid);
    display(model->width, model->height, grid);
}

int Generator::gen_energy(Model *model) {
    int grid[model->size];
    int count = gen_extract(model, grid);
    return model->size - count;
}

int Generator::gen_do_move(Model *model) {
    int index = rand_int(model->size);
    int before = model->next[index];
    int after;
    do {
        after = rand_neighbor(model->width, model->height, index);
    } while (after == before);
    model->next[index] = after;
    return (index << 16) | before;
}

void Generator::gen_undo_move(Model *model, int undo_data) {
    int index = (undo_data >> 16) & 0xffff;
    int value = undo_data & 0xffff;
    model->next[index] = value;
}

void Generator::gen_copy(Model *dst, Model *src) {
    dst->width = src->width;
    dst->height = src->height;
    dst->size = src->size;
    dst->start = src->start;
    for (int i = 0; i < src->size; i++) {
        dst->next[i] = src->next[i];
    }
}

int Generator::gen_anneal(Model *model, double max_temp, double min_temp, int steps) {
    Model *best = new Model;
    gen_init(best, model->width, model->height);
    gen_copy(best, model);
    double factor = -log(max_temp / min_temp);
    int energy = gen_energy(model);
    int previous_energy = energy;
    int best_energy = energy;
    for (int step = 0; step < steps; step++) {
        double temp = max_temp * exp(factor * step / steps);
        int undo_data = gen_do_move(model);
        energy = gen_energy(model);
        double change = energy - previous_energy;
        if (change > 0 && exp(-change / temp) < rand_double()) {
            gen_undo_move(model, undo_data);
        }
        else {
            previous_energy = energy;
            if (energy < best_energy) {
                best_energy = energy;
                gen_copy(best, model);
                if (energy <= 0) {
                    break;
                }
            }
        }
    }
    gen_copy(model, best);
    gen_uninit(best);
    return best_energy;
}

void Generator::gen(int width, int height, int *output) {
    Model *model = new Model;
    while (1) {
        gen_init(model, width, height);
        gen_randomize(model);
        int energy = gen_anneal(model, 10, 0.1, 100000);
        if (energy == 0) {
            gen_extract(model, output);
        }
        gen_uninit(model);
        if (energy == 0) {
            break;
        }
    }
}