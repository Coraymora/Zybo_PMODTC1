# Zybo_PMODTC1
Built on the ZYNQ-7020 development board, this SoC project uses a Pmod thermocouple from Digilent to take temperature measurements and display them on an LCD with scale conversion

Project Objectives:
* Measure temperature data using a K-type thermocouple-to-digital converter from Digilent.
* Allow users to select “Fahrenheit”, “Celsius”, or “Kelvin” using integrated push buttons.
* Perform calculations in Zybo board to convert from Celsius to desired temperature scale.
* Display measurements on 16x02 LCD in real time.

Components used
* ZYNQ-7020 Development Board
* Pmod TC1: K-Type Thermocouple Module with Wire
* 1602A-4 LCD 16x02 Display
* 10 kΩ potentiometer
* L7805CV 5V regulator
* 0.47 μF electrolytic capacitor
* 0.1 μF electrolytic capacitor
* 9V battery with buckle connector
* 15 x male-to-male jumper cables

The bulk of the work in this design is being done by the ARM processor, and the HDL wrapper can be created using almost all graphical tools in Vivado. I've included both the wrapper file and C code (implemented using Vitis) in this repository, although some additional troubleshooting might be required if you are starting from scratch.

Overall, the project was a good exploration in SoC development (I haven't used soft processors in Vivado, but the onboard ARM processor made writing C code really easy) and shows off the simplicity of PMOD modules; however, it certainly doesn't take advantage of all the board's features, and I would like in the future to take on a project that includes writing more VHDL.

Beyond the work in Vivado and Vitis, the LCD screen also required a higher voltage to operate than the Zybo board could provide; Figure 1 shows a simple solution using a 9V battery and 5V regulator.

![image](https://github.com/Coraymora/Zybo_PMODTC1/assets/68129788/ce034d7b-a834-44f6-be92-8a26bef98adb)
Fig 1. A photo of the completed system taking measurements in Celsius

![image](https://github.com/Coraymora/Zybo_PMODTC1/assets/68129788/b7428f49-ebfe-44e8-9ba0-2d8626e82820)
Fig 2. Block design in Vivado

I had originally wanted to communicate with an LCD baord that had an I2C extension - this would simplify the design and use fewer cables - but I decided instead to interact with a standard LCD board directly by using six GPIO pins on the Zybo board. As you can see in Figure 2, four GPIO pins are used for the LCD lines (operating the display in half-byte mode) and two GPIO pins are used for the LCD reset and enable pins. The constraints used for the GPIO operation can be seen below in Figure 3.

![image](https://github.com/Coraymora/Zybo_PMODTC1/assets/68129788/e9ab305a-f8cf-4d9f-a543-f75caece9c4e)
Fig 3. Zybo-Z7-Master constraints file

If using an LCD with a built-in I2C chip, you could use the peripheral I/O settings in Vivado which have built-in I2C IP blocks, but I didn't explore this solution very far.

![image](https://github.com/Coraymora/Zybo_PMODTC1/assets/68129788/3159b25a-10ae-4aff-826c-ed49bdd374d9)
Fig 4. MIO configuration for built-in I2C IP
