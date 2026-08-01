using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace mci_desktop.modules
{
    public class _01_quad_button : Module
    {

        public static readonly byte MODULE_ID = 0x01;

        internal _01_quad_button(byte[] modulePointer) : base(modulePointer)
        {
        }

        public _01_LIGHT_STATE getLight(_01_BUTTON_POSITION light)
        {
            
            if ((moduleInput1 >> (7 - (byte) light) & 1) == 1) return _01_LIGHT_STATE.ON;
            else return _01_LIGHT_STATE.OFF;
        }

        public void setLight(_01_BUTTON_POSITION button, _01_LIGHT_STATE lightState)
        {

            if (lightState == _01_LIGHT_STATE.ON)
            {
                moduleInput1 |= (byte) (1 << ((int) button));
                moduleInput2 |= (byte)(1 << ((int)button));
            }
            else
            {
                moduleInput1 &= (byte)~(1 << ((int)button));
                moduleInput2 &= (byte)~(1 << ((int)button));
            }

        }

        public _01_BUTTON_STATE getButtonState(_01_BUTTON_POSITION button)
        {

            if ((moduleOutput >> ((byte)button) & 1) == 1) return _01_BUTTON_STATE.PRESSED;
            else return _01_BUTTON_STATE.RELEASED;

        }

        public override byte getModuleID()
        {
            return 0x01;
        }

    }

    public enum _01_BUTTON_POSITION
    {

        TOP_LEFT = 0,
        TOP_RIGHT = 1, 
        BOTTOM_LEFT = 2, 
        BOTTOM_RIGHT = 3,

    }

    public enum _01_LIGHT_STATE
    {

        OFF = 0,
        ON = 1,

    }

    public enum _01_BUTTON_STATE
    {

        PRESSED = 1,
        RELEASED = 0,

    }

}
