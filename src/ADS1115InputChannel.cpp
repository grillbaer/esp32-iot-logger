#include "ADS1115InputChannel.h"

ADS1115InputChannel::ADS1115InputChannel(Adafruit_ADS1115 *ads, adsGain_t gain, uint8_t adsChannel, boolean differential,
                                         const char *name, const char *format, double factor, double offset)
    : InputChannel(name, format, factor, offset),
      ads(ads), gain(gain), adsChannel(adsChannel), differential(differential)
{
    if (adsChannel < 0 || adsChannel > 3)
    {
        Serial.printf("ERROR: ADS1115 unsupported channel %d\n", adsChannel);
    }

    // from the docs:
    // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
    // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
    // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
    // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
    // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

    adsReadingToVoltFactor = 1.0;
    switch (gain)
    {
    case GAIN_TWOTHIRDS:
        adsReadingToVoltFactor = 0.1875e-3;
        break;
    case GAIN_ONE:
        adsReadingToVoltFactor = 0.125e-3;
        break;
    case GAIN_TWO:
        adsReadingToVoltFactor = 0.0625e-3;
        break;
    case GAIN_FOUR:
        adsReadingToVoltFactor = 0.03125e-3;
        break;
    case GAIN_EIGHT:
        adsReadingToVoltFactor = 0.015625e-3;
        break;
    case GAIN_SIXTEEN:
        adsReadingToVoltFactor = 0.0078125e-3;
        break;
    default:
        Serial.printf("ERROR: ADS1115 unsupported gain value %d\n", gain);
    };
}

double ADS1115InputChannel::readRawValue()
{
    ads->setGain(gain);

    uint16_t rawReading;
    if (differential && adsChannel == 0)
        rawReading = ads->readADC_Differential_0_1();
    else if (differential && adsChannel == 1)
        rawReading = -(ads->readADC_Differential_0_1());
    else if (differential && adsChannel == 2)
        rawReading = ads->readADC_Differential_2_3();
    else if (differential && adsChannel == 3)
        rawReading = -(ads->readADC_Differential_2_3());
    else
        rawReading = ads->readADC_SingleEnded(adsChannel);

    return adsReadingToVoltFactor * rawReading;
}
