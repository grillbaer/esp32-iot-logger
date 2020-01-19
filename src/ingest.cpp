#include <WiFi.h>

#include "input.h"

#include "credentials.h"

static const char *thingsPeakUrl = "api.thingspeak.com";

static bool connect(void)
{
	uint16_t retries = 3;
	while (WiFi.status() != WL_CONNECTED && (--retries) > 0)
	{
		Serial.print("Trying to connect to ");
		Serial.print(wifiSsid);
		Serial.print(" ... ");
		WiFi.begin(wifiSsid, wifiPassword);
		uint16_t waitRemaining = 8;
		while (WiFi.status() != WL_CONNECTED && (--waitRemaining) > 0)
		{
			delay(500);
		}
		if (WiFi.status() == WL_CONNECTED)
		{
			Serial.println("successful");
			return true;
		}
		else
		{
			Serial.print("failed status=");
			Serial.println(WiFi.status());
		}
	}

	return WiFi.status() == WL_CONNECTED;
}

static void disconnect(void)
{
	Serial.println("Disconnecting WiFi");
	WiFi.disconnect(true, true);
}

void setupIngest(void)
{
	connect();
}

void teardownIngest(void)
{
	if (WiFi.status() == WL_CONNECTED)
	{
		disconnect();
	}
}

void ingest(void)
{
	if (!connect())
		return;

	WiFiClient client;
	if (!client.connect(thingsPeakUrl, 80))
	{
		Serial.print("Connecting to ");
		Serial.print(thingsPeakUrl);
		Serial.println(" failed");
		// disconnect from WiFi to trigger fresh connect on next ingest
		disconnect();
	}
	else
	{
		client.setTimeout(30);

		Serial.print("Ingesting cpm=");
		String content = "api_key=" + String(thingspeakApiKey);
		for (int i = 0; i < inputChannelCount; i++)
		{
			content += "&field";
			content += String(i + 1);
			content += "=";
			content += String(inputChannels[i]->getValue());

			Serial.printf(" %s=%f", inputChannels[i]->getName(), inputChannels[i]->getValue());
		}

		Serial.print(" ... ");

		client.print("POST /update HTTP/1.1\n");

		client.print("Host: ");
		client.print(thingsPeakUrl);
		client.print("\n");

		client.print("Connection: close\n");

		client.print("Content-Type: application/x-www-form-urlencoded\n");
		client.print("Content-Length: ");
		client.print(content.length());
		client.print("\n\n");

		client.print(content);

		uint16_t timeout = 40;
		while (client.available() == 0 && (--timeout) > 0)
		{
			delay(50);
		}
		if (client.available() == 0)
		{
			Serial.println("failed (no response)");
		}

		Serial.println("response:");
		while (client.available())
		{
			char c = client.read();
			Serial.write(c);
		}
		Serial.println();

		client.stop();
	}
}
