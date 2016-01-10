/*
 * GccApplication1.c
 *
 * Created: 2015-12-10 08:42:00
 *  Author: Administrator
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define PIN_E PA4
#define PIN_RS PA5


void WriteNibble(unsigned char nibbleToWrite)
{
    PORTA |= (1 << PIN_E);
    PORTA = (PORTA & 0xF0) | (nibbleToWrite & 0x0F);
    PORTA &= ~(1 << PIN_E);
}


void WriteByte(unsigned char dataToWrite)
{
    WriteNibble(dataToWrite >> 4);
    WriteNibble(dataToWrite);
}


void LCD_Command(unsigned char stuffToSend)
{
    PORTA &= ~(1 << PIN_RS);
    WriteByte(stuffToSend);
};

void LCD_Text(char *txt)
{
    PORTA |= (1 << PIN_RS);
    while (*txt) {
        WriteByte(*txt++);
        _delay_us(50);
    }
};

void LCD_GoToXY(unsigned char x, unsigned char y)
{
    unsigned char adr = y * 0x40 + x;
    LCD_Command(adr | 0x80);
    _delay_us(50);
};

void LCD_Clear(void)
{
};

void LCD_Home(void)
{
};

void LCD_Move(void){
    while(1){
        LCD_Command(0b00011011);
        _delay_ms(500);
    }
}

void LCD_CursorOff(void){
    LCD_Command(0b00001100);
}

void LCD_Initalize(void)
{
    _delay_ms(50);
    for (int i = 0; i < 3; i++){
        WriteNibble(3);
        _delay_ms(5);
    }
    WriteNibble(2);
    _delay_ms(2);
    LCD_Command(0x28);
    _delay_ms(2);
    LCD_Command(0x08);
    _delay_ms(2);
    LCD_Command(0x01);
    _delay_ms(2);
    LCD_Command(0x06);
    _delay_ms(2);
    LCD_Command(0x0f);
    _delay_ms(2);
       
};

int main(void)
{
    DDRA = 0xFF;
    LCD_Initalize();
    LCD_GoToXY(0, 0);
    LCD_Text("DUPA");
    LCD_CursorOff();
   
    while(1)
    {
       
    }
}
