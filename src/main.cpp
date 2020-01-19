#include <Arduino.h>
#include "driver/pcnt.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "input.h"
#include "display.h"
#include "ingest.h"
#include "ADS1115InputChannel.h"

// blinky state
int blinky = 1;

const uint32_t sampleIntervalMicros = 1000000L;
uint32_t sampleStartMicros;

const int16_t ingestIntervalSecs = 30;
int16_t ingestCountdown;

const int watchdogTimeoutMicros = 40000000L;
hw_timer_t *watchdogTimer = NULL;

uint32_t calcRemainingWait();
void IRAM_ATTR watchdogExpired();

void setup()
{
	Serial.begin(115200);
	delay(500); // wait to avoid serial problems
	Serial.println("Starting!");

	// initialize watchdog timer
	watchdogTimer = timerBegin(0, 80, true);
	timerAttachInterrupt(watchdogTimer, &watchdogExpired, true);
	timerAlarmWrite(watchdogTimer, watchdogTimeoutMicros, false);
	timerAlarmEnable(watchdogTimer);

	// blinky
	pinMode(LED_BUILTIN, OUTPUT);

	setupInput();
	setupIngest();
	setupDisplay();

	ingestCountdown = ingestIntervalSecs;
}

void loop()
{
	sampleStartMicros = micros();

	// blinky
	digitalWrite(LED_BUILTIN, blinky);
	blinky = !blinky;

	// reset watchdog
	timerWrite(watchdogTimer, 0);

	Serial.print("Sample:");
	for (int i = 0; i < inputChannelCount; i++)
	{
		inputChannels[i]->readValue();
		Serial.print(" ");
		Serial.print(inputChannels[i]->getName());
		Serial.print("=");
		Serial.printf(inputChannels[i]->getFormat(), inputChannels[i]->getValue());
	}
	Serial.println();

	ingestCountdown--;
	if (ingestCountdown <= 0)
	{
		ingestCountdown = ingestIntervalSecs;
		ingest();
	}

	updateDisplay();

	int32_t remainingWait = calcRemainingWait();
	delayMicroseconds(remainingWait);
}

uint32_t calcRemainingWait()
{
	const uint32_t remainingMicros = sampleIntervalMicros - (micros() - sampleStartMicros);
	return remainingMicros > sampleIntervalMicros ? 0 : remainingMicros; // handle overflow
}

void IRAM_ATTR watchdogExpired()
{
	esp_restart();
}
