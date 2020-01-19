#include <math.h>

#include "InputChannel.h"

InputChannel::InputChannel(const char *name, const char *format, double factor, double offset)
    : name(name), format(format), factor(factor), offset(offset), value(NAN) {}

void InputChannel::setConversion(double factor, double offset)
{
    this->factor = factor;
    this->offset = offset;
}

void InputChannel::readValue(void)
{
    const double rawValue = readRawValue();
    value = isnan(rawValue) ? rawValue : (rawValue * factor) + offset;
}
