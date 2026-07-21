# simple_attiny13a_devboard
A simpple development board with an LED and GPIOs on Pin header, includes also ISP Header, Power supply with screw terminal.

## Project Structure
- **/hardware**: Contains the KiCad schematic and PCB layout files.
- **/firmware**: Contains the Microchip Studio 7 C/ASM source code.

## Programming Notes
- Designed for the **ATtiny13A** microcontroller.
- Programmed using **Atmel Studio 7** and an **AVRISP mkII** programmer.
- *Note:* The board requires an external power supply (e.g., 5V) during programming, as the AVRISP mkII does not supply power to the target VCC.
