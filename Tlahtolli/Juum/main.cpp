#include <iostream>
#include "generator.h"
using namespace tlahtolli;

int main(int argc, char* argv[]) {
    Juum j(triangle);
    j.play(Stereo, 4000.0f, 440.0f);
    return 0;
}