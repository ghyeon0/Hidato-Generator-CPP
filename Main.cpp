#include <iostream>
#include "Hidato.h"

using namespace std;

int main(){
    Hidato *hidato = new Hidato();
    hidato -> generate(8, 8);
}