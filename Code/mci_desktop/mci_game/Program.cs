using mci_desktop;
using mci_desktop.modules;

namespace mci_game
{
    internal class Program
    {

        static Random random = new Random();

        static double[] coolingPacks = new double[4];

        static double coolingPacksMax = 100;
        static double coolingPacksWarning = 50;
        static double coolingPacksDanger = 80;
        static double coolingPacksMeanHeatRate = 2;

        static int fuel = 99;

        static int shipX = 0;

        static int WIDTH = 16;
        static int HEIGHT = 20;

        static bool dead = false;

        static int ticks = 0;

        static List<int> fuelXs = new List<int>();
        static List<int> fuelYs = new List<int>();

        static _01_quad_button coolingPackButtons = null;
        static _03_single_knob shipXControl = null;
        static _04_seven_segment fuelDisplay = null;
        static _05_quad_light dangerLights = null;

        static void Main(string[] args)
        {

            MCI mci = new MCI();

            while (coolingPackButtons == null || shipXControl == null || fuelDisplay == null || dangerLights == null)
            {

                Console.Clear();

                coolingPackButtons = (_01_quad_button)mci.getNthModuleOccurrence(_01_quad_button.MODULE_ID, 0);
                shipXControl = (_03_single_knob)mci.getNthModuleOccurrence(_03_single_knob.MODULE_ID, 0);
                fuelDisplay = (_04_seven_segment)mci.getNthModuleOccurrence(_04_seven_segment.MODULE_ID, 0);
                dangerLights = (_05_quad_light)mci.getNthModuleOccurrence(_05_quad_light.MODULE_ID, 0);

                Console.WriteLine("Initialising game...");

                if (coolingPackButtons == null) Console.WriteLine("Please connect the quad button module");
                if (shipXControl == null) Console.WriteLine("Please connect the single knob module");
                if (fuelDisplay == null) Console.WriteLine("Please connect the seven segment module");
                if (dangerLights == null) Console.WriteLine("Please connect the quad light module");

                Thread.Sleep(1000);

            }

            while (true)
            {

                mci.requestModuleOutput(coolingPackButtons);
                mci.requestModuleOutput(shipXControl);

                update();

                mci.sendModuleInput(coolingPackButtons);
                mci.sendModuleInput(fuelDisplay);
                mci.sendModuleInput(dangerLights);

                render();

                ticks++;

                Thread.Sleep(500);

            }

        }

        static void update()
        {

            //do cooling pack venting
            if (coolingPackButtons.getButtonState(_01_BUTTON_POSITION.TOP_LEFT) == _01_BUTTON_STATE.PRESSED) coolingPacks[0] = 0;
            if (coolingPackButtons.getButtonState(_01_BUTTON_POSITION.TOP_RIGHT) == _01_BUTTON_STATE.PRESSED) coolingPacks[1] = 0;
            if (coolingPackButtons.getButtonState(_01_BUTTON_POSITION.BOTTOM_LEFT) == _01_BUTTON_STATE.PRESSED) coolingPacks[2] = 0;
            if (coolingPackButtons.getButtonState(_01_BUTTON_POSITION.BOTTOM_RIGHT) == _01_BUTTON_STATE.PRESSED) coolingPacks[3] = 0;

            //update cooling packs
            for (int i = 0; i < coolingPacks.Length; i++)
            {

                coolingPacks[i] += random.NextDouble() * coolingPacksMeanHeatRate * 2;

            }

            //update fuel
            fuel -= 1;

            //set cooling pack warning lights
            if (coolingPacks[0] >= coolingPacksWarning) coolingPackButtons.setLight(_01_BUTTON_POSITION.TOP_LEFT, _01_LIGHT_STATE.ON);
            else coolingPackButtons.setLight(_01_BUTTON_POSITION.TOP_LEFT, _01_LIGHT_STATE.OFF);
            if (coolingPacks[1] >= coolingPacksWarning) coolingPackButtons.setLight(_01_BUTTON_POSITION.TOP_RIGHT, _01_LIGHT_STATE.ON);
            else coolingPackButtons.setLight(_01_BUTTON_POSITION.TOP_RIGHT, _01_LIGHT_STATE.OFF);
            if (coolingPacks[2] >= coolingPacksWarning) coolingPackButtons.setLight(_01_BUTTON_POSITION.BOTTOM_LEFT, _01_LIGHT_STATE.ON);
            else coolingPackButtons.setLight(_01_BUTTON_POSITION.BOTTOM_LEFT, _01_LIGHT_STATE.OFF);
            if (coolingPacks[3] >= coolingPacksWarning) coolingPackButtons.setLight(_01_BUTTON_POSITION.BOTTOM_RIGHT, _01_LIGHT_STATE.ON);
            else coolingPackButtons.setLight(_01_BUTTON_POSITION.BOTTOM_RIGHT, _01_LIGHT_STATE.OFF);

            //set cooling pack danger lights

            if (ticks % 2 == 0)
            {

                if (coolingPacks[0] >= coolingPacksDanger) dangerLights.setLightState(_05_LIGHT_POSITION.TOP_LEFT, _05_LIGHT_COLOUR.COLOUR_RED);
                if (coolingPacks[1] >= coolingPacksDanger) dangerLights.setLightState(_05_LIGHT_POSITION.TOP_RIGHT, _05_LIGHT_COLOUR.COLOUR_RED);
                if (coolingPacks[2] >= coolingPacksDanger) dangerLights.setLightState(_05_LIGHT_POSITION.BOTTOM_LEFT, _05_LIGHT_COLOUR.COLOUR_RED);
                if (coolingPacks[3] >= coolingPacksDanger) dangerLights.setLightState(_05_LIGHT_POSITION.BOTTOM_RIGHT, _05_LIGHT_COLOUR.COLOUR_RED);
            }
            else
            {

                dangerLights.setLightState(_05_LIGHT_POSITION.TOP_LEFT, _05_LIGHT_COLOUR.COLOUR_OFF);
                dangerLights.setLightState(_05_LIGHT_POSITION.TOP_RIGHT, _05_LIGHT_COLOUR.COLOUR_OFF);
                dangerLights.setLightState(_05_LIGHT_POSITION.BOTTOM_LEFT, _05_LIGHT_COLOUR.COLOUR_OFF);
                dangerLights.setLightState(_05_LIGHT_POSITION.BOTTOM_RIGHT, _05_LIGHT_COLOUR.COLOUR_OFF);

            }

            //pick up fuel
            for (int i = 0; i < fuelXs.Count; i++)
            {

                if (fuelXs[i] == shipX && fuelYs[i] == 0)
                {

                    fuelXs.RemoveAt(i);
                    fuelYs.RemoveAt(i);

                    fuel += 20;

                    if (fuel > 99) fuel = 99;

                    break;

                }

            }

            //spawn fuel
            if (random.NextDouble() < 0.1)
            {

                fuelXs.Add(random.Next(WIDTH));
                fuelYs.Add(HEIGHT);

            }

            //update and remove fuel
            for (int i = 0; i < fuelYs.Count; i++)
            {

                fuelYs[i]--;

                if (fuelYs[i] < 0)
                {

                    fuelXs.RemoveAt(i);
                    fuelYs.RemoveAt(i);

                }

            }

            //move ship
            double shipPosRatio = shipXControl.getRotationRatio();
            shipX = (int) (WIDTH * (1 - shipPosRatio));
            if (shipX >= WIDTH) shipX = WIDTH - 1;

            //update fuel on display
            fuelDisplay.setInteger(fuel);

            //detect loss
            if (fuel <= 0) dead = true;

            foreach (double heat in coolingPacks)
            {
                if (heat >= coolingPacksMax) dead = true;
            }

        }

        static void render()
        {

            Console.Clear();

            if (dead)
            {
                Console.WriteLine("YOU DIED");
                return;

            }

            //top
            for (int i = 0; i < WIDTH + 2; i++)
            {

                Console.Write("██");
            }

            Console.WriteLine();

            //space
            for (int row = 0; row < HEIGHT; row++)
            {

                for (int col = 0; col < WIDTH + 2; col++)
                {

                    if (col == 0 || col == WIDTH + 1)
                    {
                        Console.Write("██");
                        continue;
                    }

                    bool thereIsFuel = false;

                    for (int i = 0; i < fuelXs.Count; i++)
                    {

                        if (fuelXs[i] == col - 1 && fuelYs[i] == HEIGHT - row)
                        {
                            thereIsFuel = true;
                            break;
                        }
                        
                    }

                    if (thereIsFuel) Console.Write("FF");
                    else Console.Write("  ");

                }

                Console.WriteLine();

            }

            //ship
            Console.Write("██");

            for (int i = 0; i < shipX; i++)
            {

                Console.Write("--");

            }

            Console.Write("WW");

            for (int i = shipX + 1; i < WIDTH; i++)
            {

                Console.Write("--");

            }

            Console.Write("██");

            Console.WriteLine();

            //bottom
            for (int i = 0; i < WIDTH + 2; i++)
            {

                Console.Write("██");
            }

            Console.WriteLine();

        }
    }
}
