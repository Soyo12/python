#include <STC89C5xRC.H>
#include <intrins.h> 
#define datas P0
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
unsigned char js=0;
unsigned char code welcome[] = "欢迎使用求助机";
unsigned char code ill[] = "我生病了";
unsigned char code fire[] = "失火了";
unsigned char code error[] = "电路故障";
unsigned char code other[] = "其他求助";
unsigned char code wifi[] = "WiFi连接成功";

void com1() interrupt 4
{
	if(RI)
	{
		RI = 0;
		js = js+1;
	}
}

void UartInit()
{
	TMOD |= 0x20;
	SCON = 0x50;
	PCON = 0x80;
	TH1 = 0xFA; 
	TL1 = 0xFA; 
	ES = 1; 
	EA = 1; 
	TR1 = 1;
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

void keyscan()//按键扫描
{
 if(!key1)
 {
  delay(8);//消抖
   if(!key1)
   {
		beep=0;
		print_text(ill);
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
	 }
    while(!key4);
	 beep=1;
 }
}

void main()
{
	unsigned int a;
	init();
	a=0;
	print_text(welcome);
  while(a<3)
  {
   beep=0;
	 Delay500ms();
	 beep=~beep;
	 Delay500ms();
	 a++;
  }
  UartInit();
	Delay5000ms();
	js=0;
	while(js<25){;}
	print_text(wifi);
	while(1)
	{
	keyscan();
	}
}