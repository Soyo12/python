#include <STC89C5xRC.H>
#include <intrins.h> 
#include <stdio.h>
#include <string.h>
#define datas P0
#define MAX 165
sbit RS = P2^2;
sbit RW = P2^3;
sbit E = P2^4;
sbit PSB = P1^0;
sbit RST = P1^1;
sbit beep = P1^6;
sbit key1 = P1^2;
sbit key2 = P1^3;
sbit key3 = P1^4;
sbit key4 = P1^5;

unsigned char p[] = {0x80,0x90,0x88,0x98};
unsigned char strt =0;
unsigned char recv[MAX];
unsigned char cnt;
unsigned char t;
unsigned char flag;
unsigned char code welcome[] = "欢迎使用求助机";
unsigned char code ill[] = "我生病了";
unsigned char code fire[] = "失火了";
unsigned char code error[] = "电路故障";
unsigned char code other[] = "其他求助";
unsigned char code wifi[] = "WiFi连接成功";

void uart_ISR() interrupt 4
{
	if(RI)
	{
			RI = 0;
		strt = 1;
		if(cnt<MAX)
		{
			recv[cnt] = SBUF;
			cnt++;
		}
		else
		{cnt = MAX;}
		t=0;
	}
}

void Timer0Init(void)		//1毫秒@11.0592MHz
{
	//AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	ET0 = 1;
	TR0 = 1;		//定时器0开始计时
}

void timer0_ISR() interrupt 1
{
	TR0 = 0;
	if(strt == 1)
	{
		t++;
		if(t>3)
		{
			t = 0;
			cnt = 0;
			flag = 1;
		}
	}
	TL0 = 0x66;
	TH0 = 0xFC;
	TR0 = 1;
}

void UartInit(void)		//9600bps@11.0592MHz
{
TMOD|=0X20;   //设置计数器工作方式2:串口
SCON=0X50;   //设置为工作方式1
PCON=0X80;   //波特率加倍
TH1=0xfa;   //计数器初始值设置 9600bps
TL1=0xfa;
ES=1;   //打开接收中断
TR1=1;   //打开计数器
}

void recv_cls(unsigned char *o)
{
 unsigned char i;
	for(i=0;i<MAX;i++)
	{
		o[i] = 0;
	}
}

void Delay100us()		//@11.0592MHz
{
	unsigned char i;
	_nop_();
	i = 43;
	while (--i);
}

void delay(unsigned int x)
{unsigned int b;
	while(b<x)
	{b++;
	}
}

void Delay1200ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 9;
	j = 104;
	k = 139;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 4;
	j = 129;
	k = 119;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay5000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 36;
	j = 5;
	k = 211;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void send_byte(unsigned char dat)
{
	SBUF = dat;
	while(!TI);
	TI = 0;
}

void send_string(unsigned char *string)
{
    while(*string != '\0')
    {
        send_byte(*string);
				string++;
    }
} 

void is_busy(void)
{
	RS = 0;
	RW = 1;
	E = 1;
	while((datas & 0x80) == 0x80)
	 {;}
	E = 0;
}

void send_sth(unsigned char type,unsigned char sth) //0代表指令，1代表数据
{
	is_busy();
	RS = type;
	RW = 0;
	E = 0;
	Delay100us();
	datas = sth;
	E = 1;
	Delay100us();
	E = 0;
}

void init(void)
{
	PSB = 1;
	RST = 1;
	send_sth(0,0x30);
	send_sth(0,0x0c);
	send_sth(0,0x01);
}

void set(unsigned char x,unsigned char y)
{
	unsigned char pos = 0;
	pos = p[y-1] + (x-1);
	send_sth(0,pos);
}

void print_text(unsigned char cha[])
{
	unsigned char a,i;
	i = 0;
	send_sth(0,0x01);
	set(1,1);  // 设置汉字从（1，1）开始显示
	while(cha[i]!='\0')  // 判断字符串是否结束
	{
		send_sth(1,cha[i]);  // 发送汉字，一个汉字两个字节，一次发送一字节
		i++;
		if(i % 16 == 0) // 每八个字（16个字节）换一次行
			{a = i / 16 + 1;
			set(1,a);}
	}
}

void send_string_to_esp(unsigned char *cmd)
{
	send_string(cmd);
	if(flag)
	{
		flag = 0;
		strt = 0;
		print_text(recv);
		recv_cls(recv);
	}
}

void esp_init()
{
	send_string_to_esp("AT+CIPSTART=\"TCP\",\"101.42.135.100\",5000\r\n");
	Delay1200ms();
	Delay1200ms();
	Delay1200ms();
	Delay1200ms();
	send_string_to_esp("AT+CIPMODE=1\r\n");
	Delay1200ms();
	Delay1200ms();
	send_string_to_esp("AT+CIPSEND\r\n");
	Delay1200ms();
	Delay1200ms();
}

void send_get(unsigned int a)
{
	unsigned char cmd[28];
	unsigned char dest[33] = {""};
	unsigned char b;
	sprintf(cmd,"GET /wtf/?situation=%d\r\n\r\n",a);
	b=0;
	while(b == 0)
	{
		send_string(cmd);
		if(flag)
		{
			flag = 0;
			strt = 0;
			if(strstr(recv,"HTTP/1.1 200 OK"))
			{
				if(strstr(recv,"ok"))
				{b = 1;}
				else
				{strncpy(dest, recv+116, 32);
					dest[33] = '\0';
					print_text(dest);
					Delay500ms();
					Delay500ms();
				b = 1;}
			recv_cls(recv);
			}
			recv_cls(recv);
		}
		Delay1200ms();
	}
}

void keyscan()//按键扫描
{
 if(!key1)
 {
  delay(8);//消抖
   if(!key1)
   {
		beep=0;
		print_text(ill);
		send_get(1);
	 }
    while(!key1);//抬手为止
	 beep=1;
 }
   if(!key2)
 {
  delay(8);
   if(!key2)
   {
		beep=0;
		print_text(fire);
		send_get(2);
	 }
    while(!key2);
	 beep=1;
 }
 if(!key3)
 {
  delay(8);
   if(!key3)
   {
		beep=0;
		print_text(error);
		send_get(3);
	 }
    while(!key3);
	 beep=1;
 }
 if(!key4)
 {
  delay(8);
   if(!key4)
   {
		beep=0;
		print_text(other);
		send_get(4);
	 }
    while(!key4);
	 beep=1;
 }
}

void main()
{
	unsigned int a,b;
	b=0;
	a=0;
	init();
	print_text(welcome);
	Timer0Init();
  UartInit();
	EA = 1;
	while(a<3)
  {
  beep=0;
	Delay500ms();
	beep=~beep;
	Delay500ms();
	a++;
  }
	Delay5000ms();
	while(b == 0)
	{
		if(flag)
		{
			flag = 0;
			strt = 0;
			if(strstr(recv,"WIFI GOT IP"))
			{
			print_text(wifi);
			recv_cls(recv);
			b = 1;
			}
			recv_cls(recv);
		}
	}
	print_text(" ");
	b=0;
	while(b == 0)
	{
		send_string("AT\r\n");
		if(flag)
		{
			flag = 0;
			strt = 0;
			print_text(recv);
			Delay1200ms();
			if(strstr(recv,"OK"))
			{
			print_text("测试成功");
			recv_cls(recv);
			b = 1;
			}
			recv_cls(recv);
		}
	}
	esp_init();
	while(1)
	{
	keyscan();
	send_get(0);
	print_text(" ");
	}
}
