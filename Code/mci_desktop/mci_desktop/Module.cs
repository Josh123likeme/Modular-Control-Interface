using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using mci_desktop.modules;

namespace mci_desktop
{
    public abstract class Module
    {

        private readonly byte[] modulePointer;

        public byte moduleInput1;
        public byte moduleInput2;

        public byte moduleOutput;

        internal Module(byte[] modulePointer)
        {
        
            this.modulePointer = modulePointer;
        
        }

        public byte[] getModulePointer()
        {
            return modulePointer;
        }

        public static Module createModule(byte moduleID, byte[] modulePointer)
        {

            switch (moduleID)
            {

                case 0x01: return new _01_quad_button(modulePointer);
                case 0x03: return new _03_single_knob(modulePointer);
                case 0x04: return new _04_seven_segment(modulePointer);
                case 0x05: return new _05_quad_light(modulePointer);

            }

            throw new ArgumentException("No module registered with that ID");

        }
        public abstract byte getModuleID();

    }

}
