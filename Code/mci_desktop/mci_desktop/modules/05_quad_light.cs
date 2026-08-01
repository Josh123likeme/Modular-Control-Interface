using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace mci_desktop.modules
{
    public class _05_quad_light : Module
    {

        public static readonly byte MODULE_ID = 0x05;

        internal _05_quad_light(byte[] modulePointer) : base(modulePointer)
        {
        }

        public void setLightState(_05_LIGHT_POSITION light_index, _05_LIGHT_COLOUR colour)
        {

            switch (light_index)
            {

                case _05_LIGHT_POSITION.TOP_LEFT:
                    moduleInput2 &= 0b00000111;
                    moduleInput2 |= (byte) ((((int) colour) & 0b111) << 4);
                    break;

                case _05_LIGHT_POSITION.TOP_RIGHT:
                    moduleInput2 &= 0b01110000;
                    moduleInput2 |= (byte) ((((int) colour) & 0b111));
                    break;

                case _05_LIGHT_POSITION.BOTTOM_LEFT:
                    moduleInput1 &= 0b00000111;
                    moduleInput1 |= (byte) ((((int) colour) & 0b111) << 4);
                    break;

                case _05_LIGHT_POSITION.BOTTOM_RIGHT:
                    moduleInput1 &= 0b01110000;
                    moduleInput1 |= (byte) ((((int) colour) & 0b111));
                    break;

            }

        }

        public override byte getModuleID()
        {
            return 0x05;
        }
    }
    public enum _05_LIGHT_POSITION
    {

        TOP_LEFT = 0,
        TOP_RIGHT = 1,
        BOTTOM_LEFT = 2,
        BOTTOM_RIGHT = 3,

    }

    public enum _05_LIGHT_COLOUR
    {

        COLOUR_OFF = 0b000,
        COLOUR_RED = 0b001,
        COLOUR_GREEN = 0b100,
        COLOUR_BLUE = 0b010,
        COLOUR_YELLOW = 0b101,
        COLOUR_CYAN = 0b110,
        COLOUR_PURPLE = 0b011,
        COLOUR_WHITE = 0b111,

    }

}
