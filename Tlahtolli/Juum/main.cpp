#include <iostream>
#include "generator.h"
using namespace tlahtolli;

int main(int argc, char* argv[]) {
    Juum j(square);
    j.playLinux( 5000.0f, 440.0f);
    return 0;
}