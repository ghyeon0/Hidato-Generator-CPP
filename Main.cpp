#include <iostream>
#include "Hidato.h"

using namespace std;

int main(){
    Hidato *hidato = new Hidato();
    hidato -> generate(4, 4);
}