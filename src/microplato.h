#ifndef MICROPLATO_H
#define MICROPLATO_H
#include "Z80.h"
class MicroPLATO : Z80 {
    // z80 emulation support
    unsigned char inputZ80(unsigned char data);
    void outputZ80(unsigned char data, unsigned char acc);
    int check_pcZ80(void);
};
#endif