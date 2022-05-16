#include "microplato.h"

/*******************************************************************************

inputz80:
----------

   This case switches on the content of data.  If the content of data is equal
to 1, ...
The program counter, named PC, is incremented by one.


Input:
-----

   data - This variable is the byte after the INp instruction, RAM[PC].


Output:
------

   This variable is the data that has been read or calculated from the input
device and returned to the main simulator core.


Note:
----

   This function is called directly by Z80::emulate.

*******************************************************************************/

/*
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    NOTE:  this routine obviously needs some more work, as it has
    paths thru it (case 1, 2) that can pass thru the routine without
    setting the value of -retval-.  --bg 2010/06/18
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/


unsigned char MicroPLATO::inputZ80(unsigned char data)
{

    /***********************************************************************
    * Declaration:
    * -----------
    *
    *   retval - This variable contains data read from an input device.
    ***********************************************************************/
    unsigned char retval;

    switch (data)
    {

    /***********************************************************************
    *   If the value of the z80's memory, RAM[PC], is equal to 1 or 2,
    * then first or second player input is requested.
    ***********************************************************************/
        case 1:
        case 2:
            retval = 0;
            break;
        // below used by mtutor
        case 0x2a:
            retval = 0x37;
            break;
        case 0x2b:
            retval = 1;
            break;
        case 0xaa:  // new in level 4
            retval = 0x37 + 1;
            break;

        case 0xab:  // new in level 4
            retval = 0x01;
            break;

        case 0xae:          // cdc disk data port
            retval = 0;     // default
            switch (m_mtdrivefunc)
            {
            case 0:
                // read next byte of data from disk
                retval = m_MTFiles[m_mtDiskUnit&1].ReadByte();
                break;
            case 2: // write data to disk - noop
                break;
            case 11:
                // read millisec clock - called twice - for lower and upper
                {
                    u16 temp = m_zclock;
                    retval = m_clockPhase ? (temp & 0xff) : ((temp << 8) & 0xff);
                    m_clockPhase = !m_clockPhase;
                }
                break;
            case 4:
                retval = m_mtsingledata;  // d.rcid
                break;
            case 8:     // d.clear
                break;
            default:
                break;
            }
            //printf("CDC drive DATA responding to: %02x  with:  %02x\n", m_mtdrivefunc, retval);
            break;
        case 0xaf:          // cdc disk control port
            retval = m_mtcanresp;
            //printf("CDC drive control responding to: %02x  with:  %02x\n", m_mtdrivefunc, retval);
            switch (m_mtdrivefunc)
            {
            case 0:
            case 2:
            case 11:
                m_mtcanresp = 0x50;
                break;

            default:
                break;

            }

            break;

    /***********************************************************************
    *   If the value of the z80's memory, RAM[PC], is not equal to any of
    * the above cases, then the program requested bad input data. Debug
    * information containing the value of the data is printed to STDOUT.
    ***********************************************************************/
        default:
            printf ("INp BAD -> Data = %d\n", data);

            retval = 0;
            break;
    }

    /***********************************************************************
    *   This keyword returns the read input data to the main simulator core.
    ***********************************************************************/
    return retval;
}
void MicroPLATO::outputZ80(unsigned char data, unsigned char acc) {

}
int MicroPLATO::check_pcZ80(void) {
    return 0;
}