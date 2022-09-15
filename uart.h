#ifndef uart_H_
#define uart_H_

void initClock(void);
void Delay(unsigned int time);
void send_buf(unsigned char *ptr);
void write_asc(unsigned char asc);
void write_num(unsigned int dat);

#endif
