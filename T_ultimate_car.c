#include <msp430f5529.h>
#include <stdint.h>
#include <uart.h>

#define CPU_F                               ((double)8000000)
#define delay_us(x)                       __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x)                      __delay_cycles((long)(CPU_F*(double)x/1000.0))

int liftbi1=0,rightbi1=0;
int liftbi2=0,rightbi2=0;

//ָʾ�����ƴ��� jΪ���ƴ���
unsigned int j;

//��־λ��ȷ��С��������Ȧ
unsigned int a=0;

//ģʽ��־λ
unsigned int num=0;

void Time_init(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    //���� P1.3&P2.4
    //���� P2.0&P7.4
    //P1.3 <<��ʱ��TA0.2
    //P2.4 <<��ʱ��TA2.1
    //P2.0 <<��ʱ��TA1.1
    //P7.4 <<��ʱ��TB0.2

    //������ʱ�������õڶ����Ź���
    //����2^4:��ǰ��
    P2DIR |=BIT4;
    P2SEL |=BIT4;

    //������ʱ�������õڶ����Ź���
    //����7^4:��ǰ��
    P7DIR |=BIT4;
    P7SEL |=BIT4;

    //������ʱ�������õڶ����Ź���
    //����1^3:�����
    P1DIR |=BIT3;
    P1SEL |=BIT3;

    //������ʱ�������õڶ����Ź���
    //����2^0:�Һ���
    P2DIR |=BIT0;
    P2SEL |=BIT0;

    //����PWM�������ֵ��������ӦԤ��Ƶʱ��

    TA0CCR0 = 1000;
    TA0CCTL2 = OUTMOD_7;

    TA2CCR0 = 1000;
    TA2CCTL1 = OUTMOD_7;

    TA1CCR0 = 1000;
    TA1CCTL1 = OUTMOD_7;


    TB0CCR0 = 1000;
    TB0CCTL2 = OUTMOD_7;


    TA0CTL= TASSEL_2 +MC_1;
    TA1CTL= TASSEL_2 +MC_1;
    TA2CTL= TASSEL_2 +MC_1;
    TB0CTL= TASSEL_2 +MC_1;

    TA0CCR2=liftbi1;
    TA2CCR1=liftbi2;
    TA1CCR1=rightbi1;
    TB0CCR2=rightbi2;

}

Key_Init()     //�����жϳ�ʼ��
{
    P2IE |= BIT1;       //�����ж�
    P2IES |= BIT1;      //�½��ش���
    P2IFG &= ~BIT1;     //���p2^1�ڵ��жϱ�־

    //������������
    P2REN |=  BIT1;
    P2OUT |=  BIT1;

}
void GPIO_init(void)
{
    //��ʼ��ָʾ��P1^0
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    //��ʼ���������ĸ��ӿ�
    //P3^0��P3^1, P3^2,P3^3
    //out1  0ut2  out3 out4    |out4
    //����ĸ���Ч
    P3IN |=(BIT3+BIT2+BIT1+BIT0);
    P3REN |= (BIT3+BIT2+BIT1+BIT0);
    //��ʼ��ʹ�ܿ���P1^4
    P1REN |= (BIT1+BIT4);
    P1OUT |= (BIT1+BIT4);
    P1DIR &=~(BIT1+BIT4);
    //��ɾ����ʼ�����룬���ǲ�֪����û��Ӱ�죬��������
    P1DIR |= BIT6;
    //��ʼ��led���Ե�
    P1DIR |= 0x01;
    //����P1.0��Ϊ���ģʽ
    P6DIR |= BIT5;
}
//����led����



//��ת�����ֵ����
void Motor_L(float left)
{
    TA2CCR1 = left;//��ǰ
    TA0CCR2 = left;//���

}
//��ת�����ֵ����
void Motor_R(float right)
{
    TB0CCR2 = right;//��ǰ
    TA1CCR1 = right;//�Һ�

}
//���ҵ�����κ���

void MotorSet(float L,float R)  //(�ң���)
{
    Motor_L(L);
    Motor_R(R);
}





//ģʽһ������Ϊ0.3m/s
void mode_1()
{



    // go
         //1001
         if((P3IN|0xf0)==0xf9)
             MotorSet(140,154);

         //1101
         if((P3IN|0xf0)==0xfd)
             MotorSet(1000,0);
         //1011
         if((P3IN|0xf0)==0xfb)
             MotorSet(0,1000);
     //go left 0111
         if((P3IN|0xf0)==0xf7)
             MotorSet(380,0);
     //stop 1111
         if((P3IN|0xf0)==0xf0){
             for( j=0;j<10;j++)
                  {
                      MotorSet(0,0);
                      delay_ms(100);
                   }
         }


                                 //���ڹ���1��С������Ȧ��Ҫֱ��
                                 //��ʱ���ٶȵ���ֱ��
                                 //aΪֱ��
                                 if(a==0)
                                 {
                                     //0001 ��Ȧ
                                     if((P3IN|0xf0)==0xf1)
                                         MotorSet(140,154);
                                 }
}
//ģʽһ������Ϊ0.5m/s
void mode_2()
{

    // go
         //1001
         if((P3IN|0xf0)==0xf9)
             MotorSet(209,231);
         //0011
         if((P3IN|0xf0)==0xf1)
            MotorSet(140,154);
         //1101
         if((P3IN|0xf0)==0xfd)
             MotorSet(1000,0);
         //1011
         if((P3IN|0xf0)==0xfb)
             MotorSet(0,1000);
     //go left 0111
         if((P3IN|0xf0)==0xf7)
             MotorSet(380,0);
     //stop 1111
         if((P3IN|0xf0)==0xf0){
             for( j=0;j<5;j++)
                  {
                      MotorSet(0,0);
                      delay_ms(100);
                   }
         }

                                    //���ڹ���2��С������Ȧ��Ҫֱ��
                                    //��ʱ���ٶȵ���ֱ��
                                    if(a==0)
                                    {
                                        //0001
                                        if((P3IN|0xf0)==0xf1)
                                            MotorSet(140,154);
                                    }
}

void mode_3()
{

    // go
                                           //1001
                                           if((P3IN|0xf0)==0xf9)
                                               MotorSet(209,231);

                                           //0011
                                           if((P3IN|0xf0)==0xf1)
                                              MotorSet(140,154);
                                           //1101
                                           if((P3IN|0xf0)==0xfd)
                                               MotorSet(1000,0);
                                           //1011
                                           if((P3IN|0xf0)==0xfb)
                                               MotorSet(0,1000);
                                       //go left 0111
                                           if((P3IN|0xf0)==0xf7)
                                               MotorSet(380,0);
                                       //stop 1111
                                           if((P3IN|0xf0)==0xf0){
                                               for( j=0;j<5;j++)
                                                    {
                                                        MotorSet(0,0);
                                                        delay_ms(100);
                                                     }
                                           }

                                    //����Ȧ�б�1001 0xf9
                                    //��Ȧ������ֱ��
                                    //��Ȧ��������ת

                                    //���ڹ���3����һȦ������Ȧ
                                    //           �ڶ�Ȧ������Ȧ
                                    //            ����Ȧ������Ȧ
                                    if(a==0)
                                    {
                                        //С������Ȧ
                                        //0001
                                        if((P3IN|0xf0)==0xf1)
                                            MotorSet(140,154);
                                    }
                                    if(a==1)
                                    {
                                        //С������Ȧ
                                        if((P3IN|0xf0)==0xf7)
                                            MotorSet(380,0);
                                        P6OUT |= BIT5;            //P1.0����ߣ�LED1������
                                        delay_ms(10);
                                        P6OUT &= ~BIT5;             //P1.0����ͣ�LED1Ϩ��
                                        delay_ms(10);
                                    }
                                    if(a==2)
                                     {
                                         //С������Ȧ
                                        if((P3IN|0xf0)==0xf1)
                                            MotorSet(140,154);
                                     }



                                    a++;
}


void mode_4()
{
    // go
                                           //1001
                                           if((P3IN|0xf0)==0xf9)
                                               MotorSet(466,513);

                                           //0011
                                           if((P3IN|0xf0)==0xf1)
                                              MotorSet(140,154);
                                           //1101
                                           if((P3IN|0xf0)==0xfd)
                                               MotorSet(1000,0);
                                           //1011
                                           if((P3IN|0xf0)==0xfb)
                                               MotorSet(0,1000);
                                       //go left 0111
                                           if((P3IN|0xf0)==0xf7)
                                               MotorSet(380,0);
                                       //stop 1111
                                           if((P3IN|0xf0)==0xf0){
                                               for( j=0;j<10;j++)
                                                    {
                                                        MotorSet(0,0);
                                                        delay_ms(100);
                                                     }
                                           }
                                    if(a==0)
                                    {
                                        //0001
                                         if((P3IN|0xf0)==0xf1)
                                         MotorSet(140,154);
                                    }

}


void main()
{
    WDTCTL = WDTPW + WDTHOLD;      //ֹͣ���Ź�
    _EINT();    //�ж�ʹ��
// _bis_SR_register(LPM3_bits);
    Time_init();
    Key_Init();
    GPIO_init();
    while(1)
    {
        switch(num){
              //Ĭ��״̬Ϊģʽһ
              case 0:a=0;mode_1();break;

              //ģʽ��
              case 1:a=0;mode_2();break;

              //ģʽ��
              case 2:a=0;mode_3();break;

              //ģʽ��
              case 3:a=0;mode_4();break;
          }

    }
}

#pragma vector = PORT2_VECTOR
__interrupt void P2_1ISR()
{
    if(P2IFG & BIT1)
    {
        delay_ms(10);
        if(P2IFG & BIT1)
          {
              delay_ms(10);
              while((P2IN & BIT1)==0);    //�ȴ�
              //��Ҫ���Ĳ���
              num++;
          }
    }
    P2IFG &=~ BIT1;    //����жϱ�־λ
}
