#ifndef MICROPLATO_H
#define MICROPLATO_H
#include "Z80.h"
#include "MTFile.h"
typedef unsigned char u8;
typedef short u16;
class MicroPLATO : Z80 {
    MicroPLATO();
    // z80 emulation support
    unsigned char inputZ80(unsigned char data);
    void outputZ80(unsigned char data, unsigned char acc);
    int check_pcZ80(void);
    private:
    MTFile      m_MTFiles[2];
    u8          m_indev;        // input device
    u8          m_outdev;       // output device
    u8          m_mtincnt;      // counter
    u8          m_mtdrivetemp;  // temp drive function - before accept
    u8          m_mtdrivefunc;  // drive function
    u8          m_mtcanresp;    // canned response for control port
    u8          m_mtsingledata; // single byte data preset for data channel
    int         m_mtDataPhase;          // 1..7
    u8          m_mtDiskUnit;
    u8          m_mtDiskTrack;
    u8          m_mtDiskSector;
    u8          m_mtDisk1;
    u8          m_mtDisk2;
    u8          m_mtDiskCheck1;
    u8          m_mtDiskCheck2;
    int         m_mtSeekPos;
    bool        m_clockPhase;   // lower or upper byte of clock
};
#endif