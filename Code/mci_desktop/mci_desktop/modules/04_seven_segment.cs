using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace mci_desktop.modules
{
    public class _04_seven_segment : Module
    {

        public static readonly byte MODULE_ID = 0x04;

        private readonly byte[] hex_segments = new byte[] {
          0b00111111,
          0b00000110,
          0b01011011,
          0b01001111,
          0b01100110,
          0b01101101,
          0b01111101,
          0b00000111,
          0b01111111,
          0b01100111,
          0b01110111,
          0b01111100,
          0b00111001,
          0b01011110,
          0b01111001,
          0b01110001};

        internal _04_seven_segment(byte[] modulePointer) : base(modulePointer)
        {
        }

        public void setHex(byte hex)
        {

            moduleInput1 = hex_segments[((int)hex) & 0xF];
            moduleInput2 = hex_segments[((int)hex) >> 4 & 0xF];
        }

        public void setInteger(int integer)
        {

            moduleInput1 = hex_segments[integer % 10];
            moduleInput2 = hex_segments[integer / 10];

        }

        public override byte getModuleID()
        {
            return 0x04;
        }
    }
}
