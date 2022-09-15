#include<uart.h>
#include<msp430.h>

void Delay(unsigned int time)
{
    unsigned int i,k;

    for(i=0;i<255;i++)
        for(k=0;k<time;k++)
            _NOP();
}
//发送字母
void send_buf(unsigned char *ptr)
{
    while(*ptr !='\0')                // End with '\0' in C language ( ASCII:0 )
    {
        while (!(UCA1IFG&UCTXIFG));

        UCA1TXBUF = *ptr;             // Send ASCII then next
        ptr++;
        Delay(50);
    }
}

// Send ASCII
void write_asc(unsigned char asc)
{
    while (!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = asc;                    // Send ASCII
    Delay(50);
}

// 发送数字，范例为三位，可根据需要修改
void write_num(unsigned int dat)
{
    write_asc(0x30+dat / 100 % 10);
    write_asc(0x30+dat / 10  % 10);
    write_asc(0x30+dat       % 10);
}
