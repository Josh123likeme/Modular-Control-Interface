using System;
using System.IO.Ports;
using System.Threading;
using mci_desktop.modules;

namespace mci_desktop
{
    internal class Program
    {
        static void Main(string[] args)
        {

            automatic_self_test();
            //flashy_showcase();
            //simple_echo();

            Console.ReadLine();

        }

        static void automatic_self_test()
        {

            MCI mci = new MCI();

            mci.disableAutomaticModuleInputUpdate();
            mci.disableAutomaticModuleOutputUpdate();

            _01_quad_button quadButton = (_01_quad_button)mci.getNthModuleOccurrence(_01_quad_button.MODULE_ID, 0);
            _03_single_knob singleKnob = (_03_single_knob)mci.getNthModuleOccurrence(_03_single_knob.MODULE_ID, 0);
            _04_seven_segment sevenSegment = (_04_seven_segment)mci.getNthModuleOccurrence(_04_seven_segment.MODULE_ID, 0);
            _05_quad_light quadLight = (_05_quad_light)mci.getNthModuleOccurrence(_05_quad_light.MODULE_ID, 0);

            int iter = 0;

            while (true)
            {

                iter++;

                quadButton = (_01_quad_button)mci.getNthModuleOccurrence(_01_quad_button.MODULE_ID, 0);
                singleKnob = (_03_single_knob)mci.getNthModuleOccurrence(_03_single_knob.MODULE_ID, 0);
                sevenSegment = (_04_seven_segment)mci.getNthModuleOccurrence(_04_seven_segment.MODULE_ID, 0);
                quadLight = (_05_quad_light)mci.getNthModuleOccurrence(_05_quad_light.MODULE_ID, 0);

                mci.updateModuleOutputs();

                if (quadButton != null) mci.requestModuleOutput(quadButton);
                if (singleKnob != null) mci.requestModuleOutput(singleKnob);

                if (quadButton != null)
                {

                    if (quadButton.getButtonState(_01_BUTTON_POSITION.TOP_LEFT) == _01_BUTTON_STATE.PRESSED)
                        quadButton.setLight(_01_BUTTON_POSITION.TOP_LEFT, _01_LIGHT_STATE.ON);
                    else quadButton.setLight(_01_BUTTON_POSITION.TOP_LEFT, _01_LIGHT_STATE.OFF);

                    if (quadButton.getButtonState(_01_BUTTON_POSITION.TOP_RIGHT) == _01_BUTTON_STATE.PRESSED)
                        quadButton.setLight(_01_BUTTON_POSITION.TOP_RIGHT, _01_LIGHT_STATE.ON);
                    else quadButton.setLight(_01_BUTTON_POSITION.TOP_RIGHT, _01_LIGHT_STATE.OFF);

                    if (quadButton.getButtonState(_01_BUTTON_POSITION.BOTTOM_LEFT) == _01_BUTTON_STATE.PRESSED)
                        quadButton.setLight(_01_BUTTON_POSITION.BOTTOM_LEFT, _01_LIGHT_STATE.ON);
                    else quadButton.setLight(_01_BUTTON_POSITION.BOTTOM_LEFT, _01_LIGHT_STATE.OFF);

                    if (quadButton.getButtonState(_01_BUTTON_POSITION.BOTTOM_RIGHT) == _01_BUTTON_STATE.PRESSED)
                        quadButton.setLight(_01_BUTTON_POSITION.BOTTOM_RIGHT, _01_LIGHT_STATE.ON);
                    else quadButton.setLight(_01_BUTTON_POSITION.BOTTOM_RIGHT, _01_LIGHT_STATE.OFF);

                }

                if (sevenSegment != null && singleKnob != null) sevenSegment.setInteger((byte)singleKnob.getRotationValue());
                else if (sevenSegment != null) sevenSegment.setInteger(iter % 100);

                if (quadLight != null)
                {

                    int speed = 10;

                    _05_LIGHT_COLOUR[] colours = (_05_LIGHT_COLOUR[])Enum.GetValues(typeof(_05_LIGHT_COLOUR));

                    quadLight.setLightState(_05_LIGHT_POSITION.TOP_LEFT, colours[(iter / speed) % 8]);
                    quadLight.setLightState(_05_LIGHT_POSITION.TOP_RIGHT, colours[(iter / speed) % 8]);
                    quadLight.setLightState(_05_LIGHT_POSITION.BOTTOM_LEFT, colours[(iter / speed) % 8]);
                    quadLight.setLightState(_05_LIGHT_POSITION.BOTTOM_RIGHT, colours[(iter / speed) % 8]);

                }

                if (quadButton != null) mci.sendModuleInput(quadButton);
                if (sevenSegment != null) mci.sendModuleInput(sevenSegment);
                if (quadLight != null) mci.sendModuleInput(quadLight);

                mci.updateModuleInputs();

                Thread.Sleep(100);

            }

        }
        
        static void flashy_showcase()
        {

            MCI mci = new MCI();

            mci.disableAutomaticModuleInputUpdate();
            mci.disableAutomaticModuleOutputUpdate();

            _01_quad_button quadButton = (_01_quad_button)mci.getNthModuleOccurrence(_01_quad_button.MODULE_ID, 0);
            _03_single_knob singleKnob = (_03_single_knob)mci.getNthModuleOccurrence(_03_single_knob.MODULE_ID, 0);
            _04_seven_segment sevenSegment = (_04_seven_segment)mci.getNthModuleOccurrence(_04_seven_segment.MODULE_ID, 0);
            _05_quad_light quadLight = (_05_quad_light)mci.getNthModuleOccurrence(_05_quad_light.MODULE_ID, 0);

            int iter = 0;

            while (true)
            {

                iter++;

                quadButton = (_01_quad_button)mci.getNthModuleOccurrence(_01_quad_button.MODULE_ID, 0);
                singleKnob = (_03_single_knob)mci.getNthModuleOccurrence(_03_single_knob.MODULE_ID, 0);
                sevenSegment = (_04_seven_segment)mci.getNthModuleOccurrence(_04_seven_segment.MODULE_ID, 0);
                quadLight = (_05_quad_light)mci.getNthModuleOccurrence(_05_quad_light.MODULE_ID, 0);

                mci.updateModuleOutputs();

                if (quadButton != null) mci.requestModuleOutput(quadButton);
                if (singleKnob != null) mci.requestModuleOutput(singleKnob);

                if (quadButton != null)
                {

                    if (iter % 4 == 0)
                        quadButton.setLight(_01_BUTTON_POSITION.TOP_LEFT, _01_LIGHT_STATE.ON);
                    else quadButton.setLight(_01_BUTTON_POSITION.TOP_LEFT, _01_LIGHT_STATE.OFF);

                    if (iter % 4 == 1)
                        quadButton.setLight(_01_BUTTON_POSITION.TOP_RIGHT, _01_LIGHT_STATE.ON);
                    else quadButton.setLight(_01_BUTTON_POSITION.TOP_RIGHT, _01_LIGHT_STATE.OFF);

                    if (iter % 4 == 2)
                        quadButton.setLight(_01_BUTTON_POSITION.BOTTOM_LEFT, _01_LIGHT_STATE.ON);
                    else quadButton.setLight(_01_BUTTON_POSITION.BOTTOM_LEFT, _01_LIGHT_STATE.OFF);

                    if (iter % 4 == 3)
                        quadButton.setLight(_01_BUTTON_POSITION.BOTTOM_RIGHT, _01_LIGHT_STATE.ON);
                    else quadButton.setLight(_01_BUTTON_POSITION.BOTTOM_RIGHT, _01_LIGHT_STATE.OFF);

                }

                if (sevenSegment != null) sevenSegment.setInteger(iter % 100);

                if (quadLight != null)
                {

                    _05_LIGHT_COLOUR[] colours = (_05_LIGHT_COLOUR[])Enum.GetValues(typeof(_05_LIGHT_COLOUR));

                    quadLight.setLightState(_05_LIGHT_POSITION.TOP_LEFT, colours[iter % 8]);
                    quadLight.setLightState(_05_LIGHT_POSITION.TOP_RIGHT, colours[iter % 8]);
                    quadLight.setLightState(_05_LIGHT_POSITION.BOTTOM_LEFT, colours[iter % 8]);
                    quadLight.setLightState(_05_LIGHT_POSITION.BOTTOM_RIGHT, colours[iter % 8]);

                }

                if (quadButton != null) mci.sendModuleInput(quadButton);
                if (sevenSegment != null) mci.sendModuleInput(sevenSegment);
                if (quadLight != null) mci.sendModuleInput(quadLight);

                mci.updateModuleInputs();

                Thread.Sleep(200);

            }

        }

        static void simple_echo()
        {

            MCI mci = new MCI();

            for (int i = 0; i < 10; i++)
            {
                byte echoByte = (byte) i;

                Console.WriteLine("Sending echo request with data: " + Convert.ToHexString(new byte[] { echoByte }));

                byte output = mci.echoByte(echoByte);

                Console.WriteLine("Received echo: " + Convert.ToHexString(new byte[] { output }));
            }

        }

    }

}
