#pragma once

#include <math.h>

/**
 * An InputChannel provides input values from one channel.
 */
class InputChannel
{
private:
    const char *name;
    const char *format;
    double factor;
    double offset;
    double value;

public:
    InputChannel(const char *name = "?", const char *format = "%.1f", double factor = 1.0, double offset = 0.0);

    /* Name of this channel. */
    virtual const char *getName(void) { return name; };

    virtual const char *getFormat(void) { return format; };

    /* Read a new value. */
    virtual void readValue(void);

    /* Get last read already converted final input value or NAN if not available. */
    virtual double getValue(void) { return value; };

    /* 
     * Set conversion factors from raw to final value.
     * Definition: final_value = ( raw_value * factor ) + offset
     */
    virtual void setConversion(double factor, double offset);

    /* 
     * Read a new input not yet converted input value or NAN if invalid or not available.
     * Override this for channel implementations! 
     */
    virtual double readRawValue(void) = 0;
};
