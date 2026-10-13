SolarTilter

An interactive pan-tilt solar panel tracker built on an STM32F103C8T6 ("Blue Pill") microcontroller. Users adjust a solar panel's tilt and pan angle using four push buttons, while an INA219 current/voltage sensor and an SSD1306 OLED display show live voltage, current, power, and an efficiency percentage — giving immediate, hands-on feedback as the panel is angled toward or away from a light source.

Overview
Two-axis control- one servo tilts the panel up/down, a second servo pans it left/right, each driven by a dedicated pair of push buttons
Live power monitoring — an INA219 sensor measures the panel's output in real time
OLED readout- voltage, current, power, and an efficiency percentage (relative to the peak power observed) are displayed live
No instructions needed- press a button, watch the panel move and the numbers respond; built for casual audience interaction at a demo or exhibit
Hardware
Component	Qty	Notes
STM32F103C8T6 "Blue Pill"	1	Main microcontroller
SG90 servo motor	2	One for tilt, one for pan
Tactile push button	4	Tilt up / tilt down / pan left / pan right
Solar panel (3-6V)	1	Small demo panel
INA219 current/voltage sensor	1	I2C, measures panel output
SSD1306 OLED display (128x64)	1	I2C, shows live readings
ST-Link V2	1	Programming/debugging
USB to TTL converter	1	UART fallback flashing
2x 18650 battery pack	1	Servo power
Breadboard, jumper wires, resistors, capacitors	-	Supporting components
Pin Mapping
Function	Pin
Tilt servo signal	PA6
Pan servo signal	PA7
Button 1 - tilt up	PA0
Button 2 - tilt down	PA1
Button 3 - pan left	PA2
Button 4 - pan right	PA3
I2C SCL (OLED + INA219)	PB6
I2C SDA (OLED + INA219)	PB7

All four buttons use the Blue Pill's internal pull-up resistors (INPUT_PULLUP), so each button's other leg connects straight to ground

no external resistors required.
Wiring Notes
Both servos' ground wires, the battery pack's ground, and the Blue Pill's own ground pin must all share a common ground - this is essential for the PWM signal and I2C bus to work correctly.
Servo power comes from a separate 5-6V source (battery pack), not from the Blue Pill's own 3.3V/5V rail, to avoid brownouts when the servos draw current.
The INA219 sits in series between the solar panel and a small load (resistor + LED), since it measures current flowing through it - an open circuit with no load will always read 0 mA.
Software Tools Used

The project was developed using the STM32 toolchain, then migrated to a simpler workflow once the core logic was working:

STM32CubeMX - configured TIM3 PWM channels (CH1 = PA6, CH2 = PA7) for the two servos, and GPIO inputs with pull-ups for the four buttons; generated the initial HAL project.
STM32CubeIDE - used to write and build the HAL-based firmware, editing the CubeMX-generated main.c to add servo control and button handling logic.
STM32CubeProgrammer - used to flash and verify firmware over ST-Link (SWD), read back chip information, and diagnose SWD connection issues during hardware debugging.
Arduino IDE (STM32duino core) - adopted as the final, simplified development workflow. The STM32 board package was installed via Boards Manager, with "Generic STM32F1 series" / "BluePill F103C8" selected as the target and STM32CubeProgrammer (SWD) as the upload method. The firmware was rewritten using the Arduino Servo library and standard digitalRead/pinMode calls, significantly simplifying the code compared to raw HAL.
Firmware

The main sketch (SolarTilter.ino) handles:

Attaching both servos and setting their starting position (90 degrees)
Reading all four buttons each loop, with a 150ms hold-repeat delay so held buttons move the servo smoothly rather than jumping
Clamping both servo angles to a safe 0-180 degree range
Reading the INA219 every ~300ms for bus voltage, current, and power
Tracking the highest power reading seen so far as a reference point, and computing an efficiency percentage relative to it
Updating the OLED with the current readings each cycle
Getting Started
Wire the hardware according to the pin mapping above.
Install the Arduino IDE and add the STM32 board package: https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
Install the Adafruit INA219 and Adafruit SSD1306 libraries via the Arduino Library Manager.
Select Generic STM32F1 series -> BluePill F103C8 as the board, and STM32CubeProgrammer (SWD) as the upload method.
Connect ST-Link to the Blue Pill (SWCLK, SWDIO, GND).
Open SolarTilter.ino and click Upload.
Project Status
Servo and button control: working
INA219 + OLED integration: in progress / under debugging
License

MIT
