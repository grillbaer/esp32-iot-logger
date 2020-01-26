#include "InputChannel.h"
#include <Wire.h>
#include <Adafruit_ADS1015.h>

/**
 * Input channel for reading an ADS1115 ADC converter channel.
 */
class ADS1115InputChannel : public InputChannel
{
private:
    Adafruit_ADS1115 *ads;
    adsGain_t gain;
    uint8_t adsChannel;
    boolean differential;
    double adsReadingToVoltFactor;

    int16_t readSingleValue();

public:
    /**
     * Create a new input channel for an ADS1115 ADC converter.
     * 
     * \param ads The ADS to read from. May be shared with other input channels.
     * \param gain Gain to set for measurement.
     * \param adsChannel The channel 0, 1, 2 or 3 to read from. For differential readings this is 
     *                   the channel to use as positive one.
     * \param differential Differential or single reading. Differential fixed between channels 0 
     *                     and 1 or 2 and 3.
     */
    ADS1115InputChannel(Adafruit_ADS1115 *ads, adsGain_t gain, uint8_t adsChannel, boolean differential,
                        const char *name = "ADS", const char *format = "%.1f", double factor = 1.0, double offset = 0.0);

    virtual double readRawValue(void);
};
