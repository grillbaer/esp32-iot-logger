#include "U8g2lib.h"

#include "display.h"
#include "input.h"

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 4, 5);

void setupDisplay(void)
{
	u8g2.setBusClock(1000000);
	u8g2.begin();
}

void renderValue(uint16_t x, uint16_t w, uint16_t y, const char *name, const char *value)
{
	u8g2.setFont(u8g2_font_6x13_tr);
	u8g2.setCursor(x, y);
	u8g2.print(name);

	u8g2.setFont(u8g2_font_prospero_nbp_tn);
	u8g2_uint_t wValue = u8g2.getStrWidth(value);
	u8g2.setCursor(x + w - wValue, y);
	u8g2.print(value);
}

void updateDisplay(void)
{
	u8g2.clearBuffer();
	for (int i = 0; i < inputChannelCount; i++)
	{
		char buf[30];
		sprintf(buf, inputChannels[i]->getFormat(), inputChannels[i]->getValue());
		uint16_t y = (i % 3) * 16 + 12;
		uint16_t x = i < 3 ? 0 : 66;
		renderValue(x, 60, y, inputChannels[i]->getName(), buf);
	}
	u8g2.sendBuffer();
}
