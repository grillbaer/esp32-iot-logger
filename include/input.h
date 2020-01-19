#pragma once

#include <Arduino.h>
#include <InputChannel.h>

extern InputChannel *inputChannels[];
extern uint16_t inputChannelCount;

void setupInput();
