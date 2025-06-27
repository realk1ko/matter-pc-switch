![Device](device.png)

# Matter PC Switch

[![License](https://img.shields.io/github/license/realk1ko/matter-pc-switch.svg)](https://github.com/realk1ko/matter-pc-switch/blob/master/LICENSE)

A small device allowing you to check and control your PC's power state with Siri, Google Assistant, Alexa and any other
ecosystem supporting Matter.

## Components

- 1x Arduino Nano Matter
- 2x PC817 optocoupler
- 2x 1 kΩ resistor (to limit current to the optocouplers)
- 1x 10 kΩ resistor (as pulldown)
- 1x prototyping board or breadboard
- Wires and headers as needed

## Assembly

1. Flash the sketch (`matter-pc-switch.ino`) onto your Arduino. Optionally
   flash [unique provisioning data](#a-note-on-matter-pairing-codes) **afterwards**.
2. Pair the Arduino with your ecosystem: A QR code and the manual pairing code are printed to the serial console upon
   startup of the Arduino. You can reset the pairing on the Arduino by pressing the user button, which can be helpful if
   the pairing process fails.
3. Optionally configure your OS to put your PC to sleep when the power button is pressed, if you prefer standby for a
   quicker startup.
4. Optionally configure the BIOS to keep RGB LEDs on during sleep and/or off states, if you'd like to use an RGB LED
   header on your mainboard as power source for the Arduino. Alternatively an internal USB port can be used too.
5. Continue with the assembly of the device according to the [wiring schematic](#wiring-schematic).
6. Once everything is up and running again, you should be able to control your PC within your smart home platform. The
   Arduino's onboard LED should sync with the power state of your PC.

## Wiring Schematic

![Schematic](schematic.png)

## A Note on Matter Pairing Codes

The Arduino Nano Matter comes with test provisioning data out of the factory, which consequently means all boards use
the same Matter pairing code `3497-011-2332`.

To make sure your Arduino has a unique pairing code, you can use
the [Arduino Matter Provision Tool](https://github.com/SiliconLabs/arduino/blob/main/extra/arduino_matter_provision/readme.md#arduino-matter-provision-tool)
by Silicon Labs to flash unique provisioning data.
