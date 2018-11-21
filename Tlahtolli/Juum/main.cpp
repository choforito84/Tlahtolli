#include <iostream>
#include "generator.h"
using namespace tlahtolli;

int main(int argc, char* argv[]) {
    Juum j(sawtooth);
    j.play( 1000.0f, 440.0f);
    return 0;
}