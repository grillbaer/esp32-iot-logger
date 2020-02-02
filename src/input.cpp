#include "input.h"

#include "ADS1115InputChannel.h"
#include "ProductChannel.h"

static Adafruit_ADS1115 ads1(0x48); // chip ADDR connected to GND
static Adafruit_ADS1115 ads2(0x49); // chip ADDR connected to VCC

// This is a set-up for two complete power-meters with U, I and P.
//
// - setup differential ADC measurements for U and I
// - calculate power from U and I
// - do this once for solar input and once for battery output
// - define battery charging as positive current
// - use measured calibration factors to compensate deviations of resistors and ADCs

// voltage divider 47 kOhm - 4.7 kOhm, U2/(U1+U2) = R2/(R1+R2) => voltage[V] = 11 * ADC-voltage[V]
ADS1115InputChannel ads1_1to0_U_in(&ads1, GAIN_ONE, 1, true, "So V", "%.2f", 11.0 * 1.008);
// current shunt resistor 0.1 Ohm, U=R*I => current[mA] = 10 * 1e3 * ADC-voltage[V]
ADS1115InputChannel ads1_3to2_I_in(&ads1, GAIN_SIXTEEN, 3, true, "So mA", "%.1f", 10e3 * 0.996);
// P=U*I => power[mW] = voltage[V] * current[mA]
ProductChannel Pin(&ads1_1to0_U_in, &ads1_3to2_I_in, "So mW");

// voltage divider 47 kOhm - 4.7 kOhm, U2/(U1+U2) = R2/(R1+R2) => voltage[V] = 11 * ADC-voltage[V]
ADS1115InputChannel ads2_1to0_U_bat(&ads2, GAIN_ONE, 1, true, "Ba V", "%.2f", 11.0 * 1.013);
// current shunt resistor 0.1 Ohm, U=R*I => current[mA] = 10 * 1e3 * ADC-voltage[V]
ADS1115InputChannel ads2_3to2_I_bat(&ads2, GAIN_SIXTEEN, 3, true, "Ba mA", "%.1f", -10e3 * 1.008);
// P=U*I => power[mW] = voltage[V] * current[mA]
ProductChannel Pbat(&ads2_1to0_U_bat, &ads2_3to2_I_bat, "Ba mW", "%.1f");

InputChannel *inputChannels[] = {
    &ads1_1to0_U_in,
    &ads1_3to2_I_in,
    &Pin,
    &ads2_1to0_U_bat,
    &ads2_3to2_I_bat,
    &Pbat};

uint16_t inputChannelCount = 6;

void setupInput()
{
    Wire.begin(26, 25, 100000L);
    ads1.begin();
    ads2.begin();
}
