#include <Matter.h>
#include <MatterOnOffPluginUnit.h>

#define RED_LED LEDR
#define GREEN_LED LEDG
#define BLUE_LED LEDB

#define BUTTON BTN_BUILTIN
#define POWER_IN PIN_A5
#define POWER_OUT PIN_A0

#define OUTPUT_DELAY 500u
#define LOOP_DELAY 500u
#define WAKEUP_DELAY 3000u

#define SERIAL_BAUD_RATE 115200

bool last_power_state = false;
bool last_outlet_state = false;

MatterOnOffPluginUnit outlet;

void set_led_state(const bool red, const bool green, const bool blue)
{
    digitalWrite(RED_LED, red ? LOW : HIGH);
    digitalWrite(GREEN_LED, green ? LOW : HIGH);
    digitalWrite(BLUE_LED, blue ? LOW : HIGH);
}

bool is_button_pressed()
{
    return digitalRead(BUTTON) == LOW;
}

bool is_power_on()
{
    return digitalRead(POWER_IN) == HIGH;
}

void toggle_power_state()
{
    digitalWrite(POWER_OUT, HIGH);

    delay(OUTPUT_DELAY);

    digitalWrite(POWER_OUT, LOW);
}

void setup_serial()
{
    Serial.begin(SERIAL_BAUD_RATE);
}

void setup_gpio()
{
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(POWER_IN, INPUT);
    pinMode(POWER_OUT, OUTPUT);
}

void setup_matter()
{
    Matter.begin();

    outlet.begin();
    outlet.set_device_name("Computer");
    outlet.set_vendor_name("Arduino");
    outlet.set_product_name("Computer Switch");
    outlet.set_serial_number(getDeviceUniqueIdStr().c_str());

    if (!Matter.isDeviceCommissioned())
    {
        Serial.println("Matter device is not paired, pair it to your home ecosystem!");
        Serial.printf("Manual pairing code: %s\r\n", Matter.getManualPairingCode().c_str());
        Serial.printf("Pairing QR code URL: %s\r\n", Matter.getOnboardingQRCodeUrl().c_str());

        set_led_state(true, false, false);
    }

    while (!Matter.isDeviceCommissioned())
    {
        delay(LOOP_DELAY);
    }

    Serial.println("Matter device is paired.");

    set_led_state(false, false, false);
}

void teardown_matter()
{
    Serial.println("Matter device will unpair and reboot.");

    Matter.decommission();
}

void set_initial_state()
{
    last_power_state = last_outlet_state = is_power_on();

    set_led_state(false, false, last_power_state);

    outlet.set_onoff(last_outlet_state);

    Serial.printf("Computer is %s during setup.\r\n", last_outlet_state ? "on" : "off");
}

void handle_power_and_outlet_changes()
{
    // read current power and outlet state
    const bool current_power_state = is_power_on();
    const bool current_outlet_state = outlet.get_onoff();

    // sync power state to LED and outlet state
    if (last_power_state != current_power_state)
    {
        set_led_state(false, false, current_power_state);

        outlet.set_onoff(current_power_state);
    }

    // sync outlet state to power state
    if (!current_power_state && !last_outlet_state && current_outlet_state)
    {
        Serial.println("Outlet was turned on: Turning on computer...");

        toggle_power_state();
    }
    else if (current_power_state && last_outlet_state && !current_outlet_state)
    {
        Serial.println("Outlet was turned off: Turning off computer...");

        toggle_power_state();

        // ensure computer is turned off, if the first toggle only turned on the screen
        delay(WAKEUP_DELAY);

        if (is_power_on())
        {
            toggle_power_state();
        }
    }

    // store updated power and outlet states for next loop
    last_power_state = current_power_state;
    last_outlet_state = current_outlet_state;
}

void handle_button_press()
{
    if (is_button_pressed())
    {
        Serial.println("Button was pressed: Resetting...");

        teardown_matter();
    }
}

void setup()
{
    setup_serial();

    setup_gpio();

    setup_matter();

    set_initial_state();
}

void loop()
{
    handle_button_press();

    handle_power_and_outlet_changes();

    delay(LOOP_DELAY);
}
