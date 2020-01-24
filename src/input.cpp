#include "input.h"

#include "ADS1115InputChannel.h"
#include "ProductChannel.h"

static Adafruit_ADS1115 ads1(0x48); // chip ADDR connected to GND
static Adafruit_ADS1115 ads2(0x49); // chip ADDR connected to VCC

ADS1115InputChannel ads1_1to0_U_in(&ads1, GAIN_TWOTHIRDS, 1, true, "So V", "%.2f", 11.0 * 1.008);
ADS1115InputChannel ads1_3to2_I_in(&ads1, GAIN_FOUR, 3, true, "So mA", "%.1f", 1e3 * 1.006);
ProductChannel Pin(&ads1_1to0_U_in, &ads1_3to2_I_in, "So mW");

ADS1115InputChannel ads2_1to0_U_bat(&ads2, GAIN_TWOTHIRDS, 1, true, "Ba V", "%.2f", 11.0 * 1.012);
ADS1115InputChannel ads2_3to2_I_bat(&ads2, GAIN_FOUR, 3, true, "Ba mA", "%.1f", -1e3 * 1.002);
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
    Wire.begin(26, 25, 400000L);
    ads1.begin();
    ads2.begin();
}
