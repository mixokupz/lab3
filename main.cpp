#include <iostream>
#include "Sound_Proc.h"

int main(int argc, char* argv[]) {
    Sound_Proc sound_proc = Sound_Proc(argc, argv);
    sound_proc.run();
    return 0;
}