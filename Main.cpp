#include <iostream>
#include "Generator.h"

using namespace std;

int main(){
    Generator *hidato = new Generator();
    hidato -> generate(8, 8);
}