#include "microplato.h"

MicroPLATO::MicroPLATO() :
      m_mtincnt (0),
      m_mtdrivetemp (0xcb),
      m_mtdrivefunc (0),
      m_clockPhase (true) {}

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
/*******************************************************************************

outputz80:
-----------

   This case switches on the contents of the data.  If the content of data is
equal to 2, ...
If the content of data is any other value, then the request is bad and 
debugging information is printed to STDOUT.


Input:
-----

   data - This variable is the byte after the OUTp instruction, RAM[PC].
   acc - This variable is the accumulator, which is the z80's A register.


Note:
----

   This function is called directly by Z80::emulate.

*******************************************************************************/
void MicroPLATO::outputZ80(u8 data, u8 acc)
{
    u8 comp = ~acc;
    bool ok = comp == m_mtdrivetemp;

    //printf("out: %02x, %02x\n", data, acc);

    switch (data)
    {

    /***********************************************************************
    *   If the value of the z80's memory, RAM[PC], is equal to 2, the
    * content of the A register is moved into the left shift amount.
    ***********************************************************************/
        case 2:
        // below used by mtutor 
        case 0x2b:
            break;

        case 0xab:  // new in level 4

            break;

        case 0xae:      // CDC drive data port
            switch (m_mtdrivefunc)
            {
            case 0: // read disk
            case 2: // write data to disk - and recieve setup data
                //if (m_mtdrivefunc == 10)
                //    printf("CDC drive DATA WRITE");
                switch (m_mtDataPhase++)
                {
                    case 1:
                        m_mtDiskUnit = acc;
                        break;
                    case 2:
                        m_mtDiskTrack = acc;
                        break;
                    case 3:
                        m_mtDiskSector = acc;
                        break;
                    case 4:
                        m_mtDisk1 = acc;
                        break;
                    case 5:
                        m_mtDisk2 = acc;
                        break;
                    case 6:
                        m_mtDiskCheck1 = acc;
                        break;
                    case 7: // 128 bytes/sector plus two check bytes
                        m_mtDiskCheck2 = acc;

                        m_mtSeekPos = (128 * 64 * m_mtDiskTrack) + (128 * (m_mtDiskSector-1));
                        if (m_mtSeekPos < 0)
                            break;
                        m_MTFiles[m_mtDiskUnit&1].Seek(m_mtSeekPos);
                        break;

                    default:   // write data
                        m_MTFiles[m_mtDiskUnit&1].WriteByte(acc);
                        m_mtcanresp = 0x50;
                        break;
                }
                break;

            case 10: // format
                switch (m_mtDataPhase++)
                {
                case 1:
                    m_mtDiskUnit = acc;
                    break;
                case 2:
                    m_mtDiskTrack = acc;
                    break;
                case 3:
                    m_mtDiskSector = acc;
                    break;
                case 4:
                    m_mtDisk1 = acc;
                    break;
                case 5:
                    m_mtDisk2 = acc;
                    //break;
                default:
                    m_MTFiles[m_mtDiskUnit & 1].Format();
                    break;
                }
                break;

            //case 11:
            //    // read millisec clock - noop
            //break;

            //case 4:  // noop
            //case 8:  // d.clear - noop
            //    break;

            default:
                break;

            }
            //printf("CDC drive DATA recieving for: %02x  data:  %02x\n", m_mtdrivefunc, acc);
            break;

        case 0xaf:      // CDC drive control port
            if (ok)     // accept function
            {
                m_mtdrivefunc = m_mtdrivetemp;
                m_mtdrivetemp = 0xcb;
                m_mtDataPhase = 1;

                switch(m_mtdrivefunc)
                {
                case 0:
                case 2:
                case 10:
                case 11:
                    m_mtcanresp = 0x4a;
                    m_clockPhase = true;
                    break;

                case 4:
                    m_mtcanresp = 0x4a;
                    m_mtsingledata = 0x02;
                    if (m_floppy1)
                    {
                        m_mtsingledata |= 0x80;
                    }
                    break;

                case 8:
                    m_mtcanresp = 0x50;
                    break;

                default:
                    m_mtsingledata = 2;  // remove me
                    break;
                }
                //printf("CDC drive control accept: %02x\n", m_mtdrivefunc);
            }
            else
            {
                m_mtdrivetemp = acc; // set function
                //printf("CDC drive control command: %02x\n", m_mtdrivetemp);
                m_mtcanresp = 0x48;
            }
            
            break;


    /***********************************************************************
    *   If the value of the z80's memory, RAM[PC], is not equal to any of
    * the above cases, then the program requested bad output data.  Debug
    * information containing the value of the data and the accumulator are
    * printed to STDOUT.
    ***********************************************************************/
        default:
            printf ("OUTp BAD -> Data = %d   A = %d\n", data, acc);
            break;
    }
}
int MicroPLATO::check_pcZ80(void) {
    return 0;
}