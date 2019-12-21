#include "MicroBitPin.h"
#include "mbed.h"
#include "LCD_Driver.h"
#include "SPI_RAM.h"

//spi
SPI lcd_spi(MOSI, MISO, SCK);
#define LCD_SPI_Write_Byte(value) lcd_spi.write(value)

//LCD
DigitalOut LCD_RST(MICROBIT_PIN_P8);
DigitalOut LCD_DC(MICROBIT_PIN_P12);
DigitalOut LCD_CS(MICROBIT_PIN_P16);
PwmOut LCD_BL(MICROBIT_PIN_P1);

#define LCD_RST_0 LCD_RST = 0
#define LCD_RST_1 LCD_RST = 1
#define LCD_DC_0 LCD_DC = 0
#define LCD_DC_1 LCD_DC = 1
#define LCD_CS_0 LCD_CS = 0
#define LCD_CS_1 LCD_CS = 1

//delay
#define Driver_Delay_ms(xms) wait_ms(xms)

//SPI Ram
SPIRAM *spiram;

/*********************************************
function:
    Initialization system
*********************************************/
void LCD_Driver::LCD_SPI_Init(void)
{
    lcd_spi.format(8,0);
    lcd_spi.frequency(1000000);
	LCD_CS_1;
}

/*******************************************************************************
function:
    Hardware reset
*******************************************************************************/
void LCD_Driver::LCD_Reset(void)
{
    LCD_RST_1;
    Driver_Delay_ms(100);
    LCD_RST_0;
    Driver_Delay_ms(100);
    LCD_RST_1;
    Driver_Delay_ms(100);
}

/*******************************************************************************
function:
    Write register address and data
*******************************************************************************/

void LCD_Driver::LCD_WriteReg(UBYTE Reg)
{
    LCD_DC_0;
    LCD_CS_0;
    LCD_SPI_Write_Byte(Reg);
    LCD_CS_1;
}

void LCD_Driver::LCD_WriteData_8Bit(UBYTE Data)
{
    LCD_DC_1;
    LCD_CS_0;
    LCD_SPI_Write_Byte(Data);
    LCD_CS_1;
}

void LCD_Driver::LCD_WriteData_Buf(UWORD Buf, unsigned long Len)
{
    unsigned long i;
    LCD_DC_1;
    LCD_CS_0;
    for(i = 0; i < Len; i++) {
        LCD_SPI_Write_Byte((int)(Buf >> 8));
        LCD_SPI_Write_Byte((int)(Buf & 0XFF));
    }
    LCD_CS_1;
}
void LCD_Driver::LCD_REG_DATA(UWORD Reg ,UWORD Data)
{
    LCD_WriteReg(Reg>>8);
    LCD_WriteReg(Reg&0xff);
    LCD_WriteData_8Bit(Data >> 8);
    LCD_WriteData_8Bit(Data & 0xff);
}
/*******************************************************************************
function:
    Common register initialization
*******************************************************************************/
void LCD_Driver::LCD_InitReg(void)
{
        //9106
	LCD_WriteReg(0xfe);
	LCD_WriteReg(0xfe);
	LCD_WriteReg(0xef);
	LCD_WriteReg(0xb3);  
	LCD_WriteData_8Bit(0x03);	
	LCD_WriteReg(0xb6);  
	LCD_WriteData_8Bit(0x01);	
	LCD_WriteReg(0xa3);  
	LCD_WriteData_8Bit(0x11);	
	LCD_WriteReg(0x21);	
	LCD_WriteReg(0x36);  
	LCD_WriteData_8Bit(0x67); //08	
	LCD_WriteReg(0x3a);  
 	LCD_WriteData_8Bit(0x05);	
	LCD_WriteReg(0xb4);  
	LCD_WriteData_8Bit(0x21);
	LCD_WriteReg(0xF0);
	LCD_WriteData_8Bit(0x31);	 		
	LCD_WriteData_8Bit(0x26);
	LCD_WriteData_8Bit(0x28);	 	
	LCD_WriteData_8Bit(0x00);
 	LCD_WriteData_8Bit(0x2C);	 	
	LCD_WriteData_8Bit(0x0C);
	LCD_WriteData_8Bit(0x0C);	 	
	LCD_WriteData_8Bit(0x15);
 	LCD_WriteData_8Bit(0x15);	 	
	LCD_WriteData_8Bit(0x0f);	
	LCD_WriteReg(0xF1);
	LCD_WriteData_8Bit(0x0E);	 	
	LCD_WriteData_8Bit(0x12);
 	LCD_WriteData_8Bit(0x13);	 	
	LCD_WriteData_8Bit(0x00);
	LCD_WriteData_8Bit(0x0A);	 	
	LCD_WriteData_8Bit(0x0D);
 	LCD_WriteData_8Bit(0x0D);	 	
	LCD_WriteData_8Bit(0x14);	
	LCD_WriteData_8Bit(0x13);	 	
	LCD_WriteData_8Bit(0x0f);
	LCD_WriteReg(0xfe);
	LCD_WriteReg(0xff);
	LCD_WriteReg(0x11);
	//delayms(120);
	LCD_WriteReg(0x29);
  
}

/********************************************************************************
function:   Sets the start position and size of the display area
parameter:
    Xstart  :   X direction Start coordinates
    Ystart  :   Y direction Start coordinates
    Xend    :   X direction end coordinates
    Yend    :   Y direction end coordinates
********************************************************************************/
void LCD_Driver::LCD_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    //set the X coordinates
    //set the X coordinates
    LCD_WriteReg(0x2A);
    //LCD_WriteData_8Bit(0x00);
    LCD_WriteData_8Bit(Xstart>>8);
    LCD_WriteData_8Bit((Xstart & 0xff) );
    LCD_WriteData_8Bit(Xend>>8 );
    LCD_WriteData_8Bit(((Xend - 1) & 0xff) );

    //set the Y coordinates
    LCD_WriteReg(0x2B);
    LCD_WriteData_8Bit(Ystart>>8);
    LCD_WriteData_8Bit((Ystart & 0xff) );
    LCD_WriteData_8Bit(Yend>>8 );
    LCD_WriteData_8Bit(((Yend - 1) & 0xff));

    LCD_WriteReg(0x2C);
}

/********************************************************************************
function:
    Set the display point (Xpoint, Ypoint)
********************************************************************************/
void LCD_Driver::LCD_SetCursor(UWORD Xpoint, UWORD Ypoint)
{
    LCD_SetWindows(Xpoint, Ypoint, Xpoint, Ypoint);
}

/********************************************************************************
function:
    Set show color
********************************************************************************/
void LCD_Driver::LCD_SetColor(UWORD Color, UWORD Xpoint, UWORD Ypoint)
{
    LCD_WriteData_Buf(Color, (unsigned long)Xpoint * (unsigned long)Ypoint);
}

/********************************************************************************
function:
    initialization
********************************************************************************/
void LCD_Driver::LCD_Init(void)
{
	//start spi
	SPI spi(MOSI, MISO, SCK);
	spi.write(0x00);
	
	//back light
    LCD_BL.period(0.02);
    LCD_BL.write(1);
	
    LCD_SPI_Init();

    //spiram->SPIRAM_SPI_Init();
    //spiram->SPIRAM_Set_Mode(BYTE_MODE);

    //Hardware reset
    LCD_Reset();

    //Set the initialization register
    LCD_InitReg();

    //sleep out
    LCD_WriteReg(0x11);
    Driver_Delay_ms(120);

    //Turn on the LCD display
    LCD_WriteReg(0x29);
}

void LCD_Driver::LCD_SetBL(int Lev)
{
    float light = (float)Lev / 10;
    LCD_BL.write(light);
}

/********************************************************************************
function:
    Clear screen
********************************************************************************/
void LCD_Driver::LCD_Clear(UWORD Color)
{
    LCD_SetWindows(0, 0, LCD_WIDTH, LCD_HEIGHT);
    LCD_SetColor(Color, LCD_WIDTH + 2, LCD_HEIGHT + 2);

}

void LCD_Driver::LCD_ClearBuf(void)
{
    UWORD x, y;
    UWORD Color = 0xffff;
    for (y = 0; y < 128; y++) {
        for (x = 0; x < 160; x++ ) {//1 pixel = 2 byte
            //spiram->SPIRAM_WR_Byte((x + y * 160)* 2, Color >> 8);
            LCD_REG_DATA((x + y * 160)* 2, Color >> 8);
            //spiram->SPIRAM_WR_Byte((x + y * 160)* 2 + 1, Color);
            LCD_REG_DATA((x + y * 160)* 2 + 1, Color);
        }
    }
}

void LCD_Driver::LCD_SetPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color)
{

    LCD_SetWindows(Xpoint, Ypoint, Xpoint+1, Ypoint+1);
    LCD_WriteData_8Bit(Color>>8);
    LCD_WriteData_8Bit(Color); 
    LCD_WriteReg(0x29);

} 

void LCD_Driver::LCD_DrawPoint(int x, int y, int Color, int Dot)
{
    int XDir_Num ,YDir_Num;
    for(XDir_Num = 0; XDir_Num < Dot; XDir_Num++) {
        for(YDir_Num = 0; YDir_Num < Dot; YDir_Num++) {
            LCD_SetPoint(x + XDir_Num - Dot, y + YDir_Num - Dot, Color);
        }
    }
}

void LCD_Driver::LCD_Display(void)
{
    UWORD x, y;
    UBYTE RBUF[160 * 2 * 2];//read tow lines
    memset(RBUF, 0xff, sizeof(RBUF));

    spiram->SPIRAM_Set_Mode(STREAM_MODE);
    LCD_SetWindows(0, 0, 160, 128);
    for (y = 0; y < 128 / 2; y++) {//line
        spiram->SPIRAM_RD_Stream(y * 160 * 2 * 2, RBUF, 160 * 2 * 2);

        LCD_DC_1;
        LCD_CS_0;
        for (x = 0; x < 160 * 2; x++ ) {
            LCD_SPI_Write_Byte((uint8_t)RBUF[x * 2]);
            LCD_SPI_Write_Byte((uint8_t)RBUF[x * 2 + 1]);
        }
        LCD_CS_1;
    }
}

void LCD_Driver::LCD_DisplayWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    UWORD x, y;
    UBYTE BUF[(Xend - Xstart + 1) * 2];
    memset(BUF, 0xff, sizeof(BUF));

    //spiram->SPIRAM_Set_Mode(STREAM_MODE);
    LCD_SetWindows(Xstart, Ystart, Xend, Yend);
    for (y = Ystart; y < Yend; y++) {//line
        //spiram->SPIRAM_RD_Stream((y * 160 + Xstart) * 2, BUF, sizeof(BUF));

        LCD_DC_1;
        LCD_CS_0;
        for (x = 0; x < Xend - Xstart; x++) {
            LCD_SPI_Write_Byte((uint8_t)BUF[x * 2]);
            LCD_SPI_Write_Byte((uint8_t)BUF[x * 2 + 1]);
        }
        LCD_CS_1;
    }
}



void LCD_Driver::LCD_DisChar_1207(int Xchar, int Ychar, int Char_Offset, int Color)
{
    int Page = 0, Column = 0;
    const unsigned char *ptr = &Font12_Table[Char_Offset];

    for(Page = 0; Page < 12; Page ++ ) {
        for(Column = 0; Column < 7; Column ++ ) {
            if(*ptr & (0x80 >> (Column % 8)))
                LCD_SetPoint(Xchar + Column, Ychar + Page, Color);

            //One pixel is 8 bits
            if(Column % 8 == 7)
                ptr++;
        }// Write a line
        if(7 % 8 != 0)
            ptr++;
    }// Write all
}
