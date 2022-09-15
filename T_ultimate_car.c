#include <msp430f5529.h>
#include <stdint.h>
#include <uart.h>

#define CPU_F                               ((double)8000000)
#define delay_us(x)                       __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x)                      __delay_cycles((long)(CPU_F*(double)x/1000.0))

int liftbi1=0,rightbi1=0;
int liftbi2=0,rightbi2=0;

//指示灯闪灯代码 j为闪灯次数
unsigned int j;

//标志位，确定小车走内外圈
unsigned int a=0;

//模式标志位
unsigned int num=0;

void Time_init(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    //左轮 P1.3&P2.4
    //右轮 P2.0&P7.4
    //P1.3 <<定时器TA0.2
    //P2.4 <<定时器TA2.1
    //P2.0 <<定时器TA1.1
    //P7.4 <<定时器TB0.2

    //开启定时器，复用第二引脚功能
    //复用2^4:左前轮
    P2DIR |=BIT4;
    P2SEL |=BIT4;

    //开启定时器，复用第二引脚功能
    //复用7^4:右前轮
    P7DIR |=BIT4;
    P7SEL |=BIT4;

    //开启定时器，复用第二引脚功能
    //复用1^3:左后轮
    P1DIR |=BIT3;
    P1SEL |=BIT3;

    //开启定时器，复用第二引脚功能
    //复用2^0:右后轮
    P2DIR |=BIT0;
    P2SEL |=BIT0;

    //设置PWM最大周期值并开启对应预分频时钟

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

Key_Init()     //按键中断初始化
{
    P2IE |= BIT1;       //开启中断
    P2IES |= BIT1;      //下降沿触发
    P2IFG &= ~BIT1;     //清楚p2^1口得中断标志

    //配置上拉电阻
    P2REN |=  BIT1;
    P2OUT |=  BIT1;

}
void GPIO_init(void)
{
    //初始化指示灯P1^0
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    //初始化传感器四个接口
    //P3^0，P3^1, P3^2,P3^3
    //out1  0ut2  out3 out4    |out4
    //左边四个有效
    P3IN |=(BIT3+BIT2+BIT1+BIT0);
    P3REN |= (BIT3+BIT2+BIT1+BIT0);
    //初始化使能开关P1^4
    P1REN |= (BIT1+BIT4);
    P1OUT |= (BIT1+BIT4);
    P1DIR &=~(BIT1+BIT4);
    //可删除初始化代码，就是不知道有没有影响，可以试试
    P1DIR |= BIT6;
    //初始化led测试灯
    P1DIR |= 0x01;
    //设置P1.0口为输出模式
    P6DIR |= BIT5;
}
//点亮led函数



//左转电机赋值函数
void Motor_L(float left)
{
    TA2CCR1 = left;//左前
    TA0CCR2 = left;//左后

}
//右转电机赋值函数
void Motor_R(float right)
{
    TB0CCR2 = right;//右前
    TA1CCR1 = right;//右后

}
//左右电机传参函数

void MotorSet(float L,float R)  //(右，左)
{
    Motor_L(L);
    Motor_R(R);
}





//模式一：主车为0.3m/s
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


                                 //由于功能1：小车走外圈需要直行
                                 //此时将速度调至直行
                                 //a为直行
                                 if(a==0)
                                 {
                                     //0001 外圈
                                     if((P3IN|0xf0)==0xf1)
                                         MotorSet(140,154);
                                 }
}
//模式一：主车为0.5m/s
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

                                    //由于功能2：小车走外圈需要直行
                                    //此时将速度调至直行
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

                                    //内外圈判别：1001 0xf9
                                    //外圈：主车直行
                                    //内圈：主车左转

                                    //由于功能3：第一圈：走外圈
                                    //           第二圈：走内圈
                                    //            第三圈：走外圈
                                    if(a==0)
                                    {
                                        //小车走外圈
                                        //0001
                                        if((P3IN|0xf0)==0xf1)
                                            MotorSet(140,154);
                                    }
                                    if(a==1)
                                    {
                                        //小车走内圈
                                        if((P3IN|0xf0)==0xf7)
                                            MotorSet(380,0);
                                        P6OUT |= BIT5;            //P1.0输出高（LED1点亮）
                                        delay_ms(10);
                                        P6OUT &= ~BIT5;             //P1.0输出低（LED1熄灭）
                                        delay_ms(10);
                                    }
                                    if(a==2)
                                     {
                                         //小车走外圈
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
    WDTCTL = WDTPW + WDTHOLD;      //停止看门狗
    _EINT();    //中断使能
// _bis_SR_register(LPM3_bits);
    Time_init();
    Key_Init();
    GPIO_init();
    while(1)
    {
        switch(num){
              //默认状态为模式一
              case 0:a=0;mode_1();break;

              //模式二
              case 1:a=0;mode_2();break;

              //模式三
              case 2:a=0;mode_3();break;

              //模式四
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
              while((P2IN & BIT1)==0);    //等待
              //需要做的操作
              num++;
          }
    }
    P2IFG &=~ BIT1;    //清除中断标志位
}
