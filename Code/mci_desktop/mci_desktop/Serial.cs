using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace mci_desktop
{
    internal class Serial
    {

        private SerialPort port;

        public Serial()
        {

            port = new SerialPort("COM5", 9600);

            port.DtrEnable = true; //resets arduino when launching desktop program
            port.Parity = Parity.None;
            port.StopBits = StopBits.One;

            port.Open();

        }

        public byte[] readBytes(int count)
        {

            byte[] bytes = new byte[count];

            for (int i = 0; i < count; i++)
            {

                port.Read(bytes, i, 1);

            }

            return bytes;

        }

        public void writeBytes(byte[] data)
        {

            port.Write(data, 0, data.Length);

        }

    }

}
