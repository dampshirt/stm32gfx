
#include "lcd.h"
#include "main.h"
#include "spi.h"
#include "uart.h"

#define DELAY 0x80

static const uint8_t
    commandList1[] = 
    {            
        15,                       
        ST7735_SWRESET,   DELAY,  
        150,                   
        ST7735_SLPOUT ,   DELAY, 
        255,
        ST7735_FRMCTR1, 3,
        0x01, 0x2C, 0x2D,
        ST7735_FRMCTR2, 3,
        0x01, 0x2C, 0x2D,
        ST7735_FRMCTR3, 6,
        0x01, 0x2C, 0x2D,
        0x01, 0x2C, 0x2D,
        ST7735_INVCTR, 1,
        0x07,
        ST7735_PWCTR1, 3,
        0xA2, 0x02, 0x84,
        ST7735_PWCTR2, 1,
        0xC5,
        ST7735_PWCTR3, 2,
        0x0A, 0x00,
        ST7735_PWCTR4, 2,
        0x8A, 0x2A,
        ST7735_PWCTR5, 2,
        0x8A, 0xEE,
        ST7735_VMCTR1, 1,
        0x0E,
        ST7735_INVOFF, 0,
        ST7735_MADCTL, 1,
        ST7735_ROTATION,
        ST7735_COLMOD, 1,
        0x05 
    },

    commandList2[] = 
    {            
        3,
        ST7735_CASET, 4,
        0x00, 0x00, 0x00, 0x4F,
        ST7735_RASET, 4,
        0x00, 0x00, 0x00, 0x9F ,
        ST7735_INVON, 0 
    },

    commandList3[] = 
    {            
        4,                        
        ST7735_GMCTRP1, 16,
        0x02, 0x1c, 0x07, 0x12,
        0x37, 0x32, 0x29, 0x2d,
        0x29, 0x25, 0x2B, 0x39,
        0x00, 0x01, 0x03, 0x10,
        ST7735_GMCTRN1, 16,
        0x03, 0x1d, 0x07, 0x06,
        0x2E, 0x2C, 0x29, 0x2D,
        0x2E, 0x2E, 0x37, 0x3F,
        0x00, 0x00, 0x02, 0x10,
        ST7735_NORON, DELAY,
        10,
        ST7735_DISPON, DELAY,
        100 
    };

static void chipSelect() 
{
	LL_GPIO_ResetOutputPin(PORT_CS, PIN_CS);
}

void chipUnselect() 
{
	LL_GPIO_SetOutputPin(PORT_CS, PIN_CS);
}

static void chipReset() 
{
    LL_GPIO_ResetOutputPin(PORT_RES, PIN_RES);
    LL_mDelay(5);
    LL_GPIO_SetOutputPin(PORT_RES, PIN_RES);
}

static void writeCommand(uint8_t cmd) 
{
	LL_GPIO_ResetOutputPin(PORT_DC, PIN_DC);
	writeSPI(&cmd, sizeof(cmd));
}

static void writeData(uint8_t * buf, size_t size) 
{
	LL_GPIO_SetOutputPin(PORT_DC, PIN_DC);
	writeSPI(buf, size);
}

static void xCommandList(const uint8_t * addr) 
{
    uint8_t numCommands, numArgs;
    uint16_t ms;

    numCommands = *addr++;
    while(numCommands--) 
    {
        uint8_t cmd = *addr++;
        writeCommand(cmd);

        numArgs = *addr++;
        ms = numArgs & DELAY;
        numArgs &= ~DELAY;
        if(numArgs) 
        {
        	writeData((uint8_t*)addr, numArgs);
            addr += numArgs;
        }

        if(ms) 
        {
            ms = *addr++;
            if(ms == 255) ms = 500;
            LL_mDelay(ms);
        }
    }
}

static void setAddress(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	writeCommand(ST7735_CASET);
    uint8_t data[] = { 0x00, x0 + ST7735_XSTART, 0x00, x1 + ST7735_XSTART };
    writeData(data, sizeof(data));

    writeCommand(ST7735_RASET);
    data[1] = y0 + ST7735_YSTART;
    data[3] = y1 + ST7735_YSTART;
    writeData(data, sizeof(data));

    writeCommand(ST7735_RAMWR);
}

void invertColors(unsigned invert)
{
    chipSelect();
    writeCommand(invert ? ST7735_INVOFF : ST7735_INVON);
    chipUnselect();
}

void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
    if((x + w - 1) >= ST7735_WIDTH) w = ST7735_WIDTH - x;
    if((y + h - 1) >= ST7735_HEIGHT) h = ST7735_HEIGHT - y;

    chipSelect();
    setAddress(x, y, x+w-1, y+h-1);

    unsigned char data[] = { color >> 8, color & 0xFF };

    LL_GPIO_SetOutputPin(PORT_DC, PIN_DC);

    for(y = h; y > 0; y--)
    {
        for(x = w; x > 0; x--)
        {
        	writeSPI(data, sizeof(data));
        }
    }

    chipUnselect();
}

void fillScreen(uint16_t color)
{
	fillRect(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
#ifdef ST7735_WRAP
    x %= ST7735_WIDTH;
    y %= ST7735_HEIGHT;
#else
    if((x >= ST7735_WIDTH) || (y >= ST7735_HEIGHT)) return;
#endif

    chipSelect();

    setAddress(x, y, x + 1, y + 1);

    uint8_t data[] = { color >> 8, color & 0xff };
    writeData(data, sizeof(data));

    chipUnselect();
}

void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    int16_t D = 2 * dy - dx;
    uint16_t Y = y0;

    int i = 0;
    for (i = x0; i < x1; ++i)
    {
        drawPixel(i, Y, ST7735_RED);
        if (D > 0)
        {
            Y = Y + 1;
            D = D - 2 * dx;
        }
        D = D + 2 * dy;
    }
}

void initLCD() {
    chipSelect();
    chipReset();
    xCommandList(commandList1);
    xCommandList(commandList2);
    xCommandList(commandList3);
    chipUnselect();
}



