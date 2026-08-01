using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace mci_desktop
{
    public class MCI
    {

        private Serial serial;

        public MCI()
        {

            serial = new Serial();

            byte firstByte = 0;

            while(firstByte != '!')
            {
                firstByte = serial.readBytes(1)[0];
            }

            byte[] readySignal = serial.readBytes(3);

            
            if (readySignal[0] == 'r' && readySignal[1] == 'd' && readySignal[2] == 'y')
            {
                return;
            }
            else
            {
                throw new Exception("Received message from MCI, but was not the correct ready signal. Received: \"" + Encoding.ASCII.GetString(readySignal) + "\", Should be: \"rdy\"");
            }

        }
        public void sendModuleInput(Module module)
        {

            serial.writeBytes(new byte[] { 0x00,
                module.getModulePointer()[0],
                module.getModulePointer()[1],
                module.moduleInput1,
                module.moduleInput2});

        }

        public void requestModuleOutput(Module module)
        {

            serial.writeBytes(new byte[] { 0x01,
                module.getModulePointer()[0],
                module.getModulePointer()[1]});

            byte[] response = serial.readBytes(1);

            module.moduleOutput = response[0];

        }

        public Module getNthModuleOccurrence(byte moduleID, int n)
        {

            serial.writeBytes(new byte[] { 0x02, moduleID, (byte) n });

            byte[] response = serial.readBytes(2);

            if (response[0] == 0 && response[1] == 0)
                return null;

            return Module.createModule(moduleID, response);

        }

        public void updateModuleInputs() 
        {
            serial.writeBytes(new byte[] { 0x10 });
        }

        public void enableAutomaticModuleInputUpdate()
        {
            serial.writeBytes(new byte[] { 0x11 });
        }

        public void disableAutomaticModuleInputUpdate()
        {
            serial.writeBytes(new byte[] { 0x12 });
        }
        public void updateModuleOutputs()
        {
            serial.writeBytes(new byte[] { 0x20 });

        }

        public void enableAutomaticModuleOutputUpdate()
        {
            serial.writeBytes(new byte[] { 0x21 });
        }

        public void disableAutomaticModuleOutputUpdate()
        {
            serial.writeBytes(new byte[] { 0x22 });
        }

        public byte echoByte(byte b)
        {

            serial.writeBytes(new byte[] { 0xFF, b });

            byte[] output = serial.readBytes(1);

            return output[0];

        }

    }

}
