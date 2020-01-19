#include "ProductChannel.h"

ProductChannel::ProductChannel(InputChannel *channel1, InputChannel *channel2,
                               const char *name, const char *format, double factor, double offset)
    : InputChannel(name, format, factor, offset), channel1(channel1), channel2(channel2)
{
}
