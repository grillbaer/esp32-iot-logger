#include "InputChannel.h"

/**
 * Input channel that returns the product of two other channels.
 */
class ProductChannel : public InputChannel
{
private:
    InputChannel *channel1;
    InputChannel *channel2;

public:
    /**
     * Create a new product channel from two others.
     */
    ProductChannel(InputChannel *channel1, InputChannel *channel2,
                   const char *name = "product", const char *format = "%.1f", double factor = 1.0, double offset = 0.0);

    virtual double getValue(void) { return channel1->getValue() * channel2->getValue(); };

    virtual double readRawValue(void) { return 0.0; /* not used */ };
};
