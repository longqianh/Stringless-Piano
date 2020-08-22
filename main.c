/***********************************************
����STC15F2K60S2ϵ�е�Ƭ��C���Ա��ʵ��
ʹ������ͷ�ļ������������ٰ���"REG51.H"
#include <STC15F2K60S2.h>
***********************************************/
#include "sys_fun.h"
#include "songs.h"
#include "display.h"
#include <string.h>
#include <stdio.h>

#define PWMOUT DLED_1
uchar port_status;
uchar key;
uchar tune_cnt=0;
uchar low_note;
uchar hi_note;
uchar PWM_cnt;  //����
uchar HighRH = 0; //�ߵ�ƽ����ֵ�ĸ��ֽ�
uchar HighRL = 0; //�ߵ�ƽ����ֵ�ĵ��ֽ�
uchar LowRH = 0; //�͵�ƽ����ֵ�ĸ��ֽ�
uchar LowRL = 0; //�͵�ƽ����ֵ�ĵ��ֽ�
uchar T1RH = 0; //T1 ����ֵ�ĸ��ֽ�
uchar T1RL = 0; //T1 ����ֵ�ĵ��ֽ�	
uchar bee_cnt=0;
uchar dc;
uchar dc_threshold;
uchar tune_tag=1;
uchar timer_cnt=0;
uchar beat_idx=0;
uchar cover_page=1;

bit stop_flag=0;
bit bee_flag=0;
bit pwm_flag=0;
bit back_flag=0;
bit song_flag=0;
bit continue_flag=0;
bit tie_flag=0;

static bit dir = 0;
static unsigned char index = 0;
unsigned long PeriodCnt = 0; //PWM ���ڼ���ֵ
unsigned int high, low;

unsigned short code notes[]={0x0000,0xF88C,0xF95B,0xFA15,0xFA67,0xFB04,0xFB90,0xFC0C,
0xFC44,0xFCAC,0xFD09,0xFD34,0xFD82,0xFDC8,0xFE06,
0xFE22,0xFE57,0xFE86,0xFE99,0xFEC0,0xFEE3,0xFE02};
	
uchar code jia[]={//��[16*16]����
0x10,0x00,0x10,0x00,0x10,0x04,0x12,0x7E,
0xFF,0x44,0x12,0x44,0x12,0x44,0x12,0x44,
0x12,0x44,0x22,0x44,0x22,0x44,0x22,0x44,
0x22,0x44,0x4A,0x7C,0x84,0x44,0x00,0x00};

uchar code you[]={//��[16*16]����
0x00,0x40,0x40,0x40,0x30,0x40,0x10,0x44,
0x87,0xFE,0x64,0x44,0x24,0x44,0x0C,0x44,
0x14,0x44,0x27,0xFC,0xE4,0x44,0x24,0x44,
0x24,0x44,0x24,0x44,0x27,0xFC,0x24,0x04};

uchar code xue[]={//ѧ[16*16]����
0x22,0x08,0x11,0x08,0x11,0x10,0x00,0x20,
0x7F,0xFE,0x40,0x02,0x80,0x04,0x1F,0xE0,
0x00,0x40,0x01,0x84,0xFF,0xFE,0x01,0x00,
0x01,0x00,0x01,0x00,0x05,0x00,0x02,0x00};

uchar code xi[]={//ϰ[16*16]����
0x00,0x00,0x00,0x04,0xFF,0xFE,0x00,0x04,
0x08,0x04,0x04,0x04,0x02,0x04,0x02,0x24,
0x00,0xC4,0x03,0x04,0x0C,0x04,0x30,0x04,
0x10,0x04,0x00,0x44,0x00,0x28,0x00,0x10};

uchar code STAR[]={//��[16*16]����
0x00,0x80,0x00,0x80,0x01,0x40,0x01,0x40,
0x02,0x20,0x7C,0x1F,0x20,0x02,0x10,0x04,
0x08,0x08,0x04,0x10,0x08,0x88,0x09,0x48,
0x16,0x34,0x18,0x0C,0x20,0x02,0x00,0x00};

uchar code robot[] = {
0x08,0x10,0x08,0x10,0x08,0x10,0x7F,0xFE,
0x20,0x04,0x12,0x48,0x08,0x10,0x05,0xA0,
0x02,0x40,0x01,0x80,0x01,0x80,0x07,0xE0,
0x09,0x90,0x11,0x88,0x11,0x88,0x11,0x88}; 


uchar code ting[]={
0x00,0x10,0x08,0x38,0x7D,0xC0,0x49,0x00,
0x49,0x00,0x49,0x04,0x49,0xFE,0x49,0x10,
0x49,0x10,0x49,0x10,0x49,0x10,0x4A,0x10,
0x7A,0x10,0x44,0x10,0x08,0x10,0x00,0x10};


unsigned char code zju[]={
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xA0,0x00,0xFF,0xF0,0x00,0x7F,0x00,0x00,0x03,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x1F,0xE0,0x00,0x38,0x00,0x00,0x03,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x0F,0xE0,0x00,0x70,0x00,0x00,0x07,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x07,0xC0,0x03,0xE0,0x00,0x00,0x07,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x03,0xC0,0x0F,0xC0,0x00,0x00,0x1F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xC0,0x07,0xC0,0x00,0x00,0x3F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x01,0xC0,0x07,0x80,0x00,0xFB,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x80,0x03,0x80,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x80,0x03,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xEF,0x80,0x00,0x00,0x03,0x00,0x00,0x01,0x7F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFC,0x60,0x00,0x00,0x00,0x00,0x04,0x1F,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0xBF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x4F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC0,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};

/*	uchar code table[] = { //ռ�ձȵ�����
		//50,50};
		0,9,17,26,34,42,50,57,64,71,77,82,87,91,94,97,98,99,100}; */
uchar code PWM_table[] = {
		1 ,2, 3 ,4 ,5 ,6 ,7 ,8, 9 ,10 ,
		11 ,12 ,13 ,14, 15, 16, 17 ,18,19 ,20,
		 21,22,23, 24 ,25 ,26 ,27, 28, 29 ,30, 
		 31 ,32 ,33 ,34 ,35 ,36 ,37 ,38, 39 ,40,
		  41 ,42 ,43, 44 ,45 ,46, 47 ,48, 49, 50 ,
		  51 ,52, 53, 54 ,55 ,56, 57, 58, 59, 60 ,
		  61, 62 ,63 ,64 ,65 ,66, 67 ,68 ,69 ,70, 
		  71 ,72 ,73,74, 75, 76 ,77 ,78, 79 ,80,
		  81 ,82,83, 84 ,85, 86 ,87, 88 ,89, 90,
		   91 ,92 ,93 ,94 ,95 ,96, 97, 98, 99, 100 }  ;

unsigned char xdata recorded_songs[100]={0};



void main()
{
	P5M1=0x00;
	P5M0=0x30;	
	P4M1=0xC0;
	P4M0=0x00;	
	P3M1=0x00;
	P3M0=0x00;
	
	Ini_Lcd();//Һ����ʼ���ӳ���
	WRGDRAM(0x80,16,32,zju);
	clear_LCD();
	DLED_1=0;
	DLED_2=0;
	while(1)
	{	
		draw_cover();	
		if((ctrl_port_check()&0xF0)==0xF0) //ʲôҲ������ȫ1
		{  	
			clear_LCD();
			while((ctrl_port_check()&0xF0)==0xF0)  // ��ֹһֱˢ��
			{
				P4M1=0x00;
				P4M0=0x00;
				Disp(1,0,16,"�㽭��ѧ���ѧԺ");//��ʾ���ݵ�LCD12864�ӳ���			
				Disp(2,1,12,"����ϵͳ���");//��ʾ���ݵ�LCD12864�ӳ���
				set_CGRAM(STAR);
				Disp(3,2,8,"��¡��\00\00");//��ʾ���ݵ�LCD12864�ӳ���
				Disp(4,2,8,"2020--08");
				set_CGRAM(jia);
				set_CGRAM(you);
				set_CGRAM(xue);
				set_CGRAM(xi);			
				set_CGRAM(STAR);
		
			}
			
			//Delay_xMs(2500);		
		}		

		if(!(ctrl_port_check()&0x80)) //ctrl_port_check()==0x70 ��
		{ 
		
			KeyIO=0xF0;
			clear_LCD();
			Disp(1,0,16,"ϵͳ���а������");//��ʾ���ݵ�LCD12864�ӳ���			
			while(!(ctrl_port_check()&0x80))
			{				
				if((P1&0xf0)!=0xf0) //����м�����
		        {
		            Delay_xMs(100);   //��ʱȥ����
		            if((KeyIO&0xf0)!=0xf0)   //���ж�
		            {
						key=scankey();
						switch(key)
						{
							case 11:Disp(2,1,12,"KEY_VALUE:01");break;
							case 12:Disp(2,1,12,"KEY_VALUE:02");break;
							case 13:Disp(2,1,12,"KEY_VALUE:03");break;
							case 14:Disp(2,1,12,"KEY_VALUE:04");break;
							case 21:Disp(2,1,12,"KEY_VALUE:05");break;
							case 22:Disp(2,1,12,"KEY_VALUE:06");break;
							case 23:Disp(2,1,12,"KEY_VALUE:07");break;
							case 24:Disp(2,1,12,"KEY_VALUE:08");break;
							case 31:Disp(2,1,12,"KEY_VALUE:09");break;	
							case 32:Disp(2,1,12,"KEY_VALUE:10");break;
							case 33:Disp(2,1,12,"KEY_VALUE:11");break;
							case 34:Disp(2,1,12,"KEY_VALUE:12");break;
							case 41:Disp(2,1,12,"KEY_VALUE:13");break;
							case 42:Disp(2,1,12,"KEY_VALUE:14");break;
							case 43:Disp(2,1,12,"KEY_VALUE:15");break;
							case 44:Disp(2,1,12,"KEY_VALUE:16");break;
						}
					}   
				}
			}
		}
		else if(!(ctrl_port_check()&0x40))
		{ 
			clear_LCD();
			while(!(ctrl_port_check()&0x40))
			{
				Disp(1,2,8,"���ֲ��Ų���");
				Disp(2,1,12,"(4,1):Lemon~");
				Play_Song(3);

			}
			//timer0_init();

			//Play_Song(0);			 
		} 


		else if(!(ctrl_port_check()&0x20))
		{ 
			 clear_LCD();
			 Disp(1,0,16,"ϵͳ���й�����");
			 OPT_CHECK = 0xFF;
			 note=note_0;
			 while((!(ctrl_port_check()&0x20))&&((OPT_CHECK&0x7F) != 0x7F))
			 {
			 	if(OPT_CHECK&0x01)
				 {
				   ShowQQChar(0x90,"1:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x90,"1:��",2);
				 }
  				 if(OPT_CHECK&0x02)
				 {
				   ShowQQChar(0x92,"2:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x92,"2:��",2);
				 }
  				 if(OPT_CHECK&0x04)
				 {
				   ShowQQChar(0x94,"3:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x94,"3:��",2);
				 }
  				 if(OPT_CHECK&0x08)
				 {
				   ShowQQChar(0x96,"4:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x96,"4:��",2);
				 }
  				 if(OPT_CHECK&0x10)
				 {
				   ShowQQChar(0x88,"5:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x88,"5:��",2);
				 }
  				 if(OPT_CHECK&0x20)
				 {
				   ShowQQChar(0x8A,"6:��",2);
				 }
				 else
				 {
				   ShowQQChar(0x8A,"6:��",2);
				 }
  				 if(OPT_CHECK&0x40)
				 {
				   ShowQQChar(0x8C,"7:��  ",3);
				 }
				 else
				 {
				   ShowQQChar(0x8C,"7:��  ",3);
				 }	
			
			}
		}

		else if(!(ctrl_port_check()&0x10))
		{

			uchar listen_page=1;
			KeyIO=0xF0;
			draw_cover();
			back_flag=0;
			while(!(ctrl_port_check()&0x10))
			{	
				if((P1&0xf0)!=0xf0) //����м�����
	        	{
		           // Delay_xMs(100);   //��ʱȥ����
		           // if((KeyIO&0xf0)!=0xf0)   //���ж�
		            //{
						key=scankey();
						switch(key)
						{
							case 11:
							{
								 clear_LCD();
								 OPT_CHECK = 0xFF;
	 							 note=note_m;
								 timer1_init();
								 if(cover_page==1)play();
								 else if(cover_page==2)record();
								break;
							}

							case 12:
							{
								
								if(cover_page==1)
								{
									bit tmp_flag=1;
									draw_listen_p1();
									while(tmp_flag)
									{
										uchar key1=scankey();
									
										switch(key1)
									 	{
										 	case 11: 
											{
												Play_Song(1);
												break;
											}
											
											case 14: //��һҳ 
											{	
												bit tmp_flag2=1;
												draw_listen_p2();
												
												while(tmp_flag2)
												{
													uchar key2=scankey();								
													switch(key2)
													{
														case 11: Play_Song(2);break;
														case 12: Play_Song(3);break;
														case 44: {tmp_flag2=0;break;}
													}
												}
												
												break;
											 }
											
											case 44: {tmp_flag=0;break;}

								 		}

									}
								 }
																						
								else if(cover_page==2)
								{
									draw_record_p2();
									DLED_1=1;
									DLED_2=1;
									delay_ms(3);
									Play_Song(0);
								} 
								
								break;
							
							}
							case 13:
							{
								if(cover_page==1)
								{
									cover_page=2;
									draw_cover_p2();
								}
								
								else if(cover_page==2)
								{
									clear_LCD();
									Disp(1,0,10,"С������ϰ");
									Disp(2,0,11,"(1,3):Start");
									Disp(3,0,9,"(2,3):End");
									Disp(4,0,13,"(4,3):Restart");
									practice();
									draw_cover_p2();

								}								
								break;
								
								
							}
						
							case 44:
							{
								cover_page=1;
								listen_page=1;
								draw_cover();
								break;
							}

							
						}

				}
		   }
		}

	}
}


//ռ�ձȵ���������Ƶ�ʲ���ֻ����ռ�ձ�
/*
void AdjustDutyCycle(unsigned char dc)
{
	unsigned int high, low;
	high = (PeriodCnt*dc) / 100; //����ߵ�ƽ����ļ���ֵ
	low = PeriodCnt - high; //����͵�ƽ����ļ���ֵ
	high = 65536 - high ; //����ߵ�ƽ�Ķ�ʱ������ֵ
	low = 65536 - low ; //����͵�ƽ�Ķ�ʱ������ֵ
	HighRH = (unsigned char)(high>>8); //�ߵ�ƽ����ֵ���Ϊ�ߵ��ֽ�
	HighRL = (unsigned char)high;
	LowRH = (unsigned char)(low>>8); //�͵�ƽ����ֵ���Ϊ�ߵ��ֽ�
	LowRL = (unsigned char)low;
}
*/
void timer0_isr() interrupt 1  
{	
	
	TR0=0;
	timer_cnt++;
	if(timer_cnt==5000)stop_flag=1;
/*	//DLED_2=~DLED_2;
	tune_cnt++;
	if(tune_tag==0)
	{
		if(tune_cnt==beat)
		{
			tune_cnt==0;
			continue_flag=0;
			
		}
	}*/ 
}

void timer1_isr() interrupt 3
{
	//TL1=low_note;
	//TH1=hi_note;
	bee_Speak=~bee_Speak;
	bee_cnt++;
	//PWMOUT=~PWMOUT;
	/*
	dc=PWM_table[index];	
	high = (PeriodCnt*dc) / 100; //����ߵ�ƽ����ļ���ֵ
	low = PeriodCnt - high; //����͵�ƽ����ļ���ֵ
	high = 65536 - high ; //����ߵ�ƽ�Ķ�ʱ������ֵ
	low = 65536 - low ; //����͵�ƽ�Ķ�ʱ������ֵ
	
	//AdjustDutyCycle(pwm_table[index]); //���� PWM ��ռ�ձ�
	if (!dir){ //������ռ�ձ�	
		index++;	
		if (index >=99){	
			dir = 1;}	
	}
	else{ //�𲽼�Сռ�ձ�	
		index--;	
		if (index == 0){	
			dir = 0;}
	}
	if (PWMOUT)
	{ //��ǰ���Ϊ�ߵ�ƽʱ��װ�ص͵�ƽֵ������͵�ƽ	

		TH1 = (unsigned char)(low>>8);		
		TH1 = (unsigned char)low;
		PWMOUT=0;
		bee_Speak=0;
					
	}
	else
	{ //��ǰ���Ϊ�͵�ƽʱ��װ�ظߵ�ƽֵ������ߵ�ƽ
		
		TH1 = (unsigned char)(high>>8); 		
		TL1 = (unsigned char)high;
		PWMOUT=1;	
		bee_Speak=1;
			
	}  */
  
		
/*
		
		if(bee_cnt<=dc_threshold)
		{
			DLED_1=1;
			bee_Speak=1;
		}
		else if(bee_cnt==10)
		{
			bee_cnt=0;

		}
		else{
			DLED_1=0;
			bee_Speak=0;
		}
		*/

		/*
		if(dc<=dc_threshold)
		{
			bee_Speak=1;
			dc++;
		}
		else
		{
			bee_Speak=0;
			dc++;			
		}
		
		if(dc==100)
		{

			dc=0;
			if(dc_threshold<100)
			{
				dc_threshold+=20;
			}
			else
			{
				dc_threshold-=20;
			}
		}		*/
		
}




// ���ò����� PWM��fr-Ƶ�ʣ�dc-ռ�ձ� 
void ConfigPWM(unsigned int iT, unsigned char dc)
{
	//PeriodCnt = (12000000/12) / fr; //����һ����������ļ���ֵ
	PeriodCnt=65536-iT;
	high = (PeriodCnt*dc) / 100; //����ߵ�ƽ����ļ���ֵ
	low = PeriodCnt - high; //����͵�ƽ����ļ���ֵ
	high = 65536 - high ; //����ߵ�ƽ�Ķ�ʱ������ֵ
	low = 65536 - low ; //����͵�ƽ�Ķ�ʱ������ֵ
	HighRH = (unsigned char)(high>>8); //�ߵ�ƽ����ֵ���Ϊ�ߵ��ֽ�
	HighRL = (unsigned char)high;
	LowRH = (unsigned char)(low>>8); //�͵�ƽ����ֵ���Ϊ�ߵ��ֽ�
	LowRL = (unsigned char)low;

	TH1 = HighRH; //���� T1 ����ֵ
	TL1 = HighRL;
	PWMOUT = 1; //����ߵ�ƽ
	bee_Speak=0;
	TR1=1;
	//if(bee_flag)bee_Speak=1;
}




/*******************��ʱ����ʼ��********************/
void timer0_init() //1ms��ʱ
{

	TMOD |=0x01;		//���ö�ʱ��ģʽ

	TL0 = (65536-1000)/256;		//���ö�ʱ��ֵ
	TH0 = (65536-1000)%256;		//���ö�ʱ��ֵ
	ET0=1;
	EA=1;
	TR0 = 0;		
	timer_cnt=0;
}

void timer1_init()
{
	TR1=0;
	TMOD |=0x01; //B0001_0000?
	ET1=1;
	EA=1;
	DLED_1=0 ;
	DLED_2=0;
}
/****************��������*************************/
void s1_s2_check(void)
{
	if(P4&0x40) 
		DLED_1 =1;
	else	
		DLED_1 =0;
	
	if((P4&0x80)) 
		DLED_2 =1;
	else	
		DLED_2 =0;
	}

/****************��������*************************/
void Play_Song(unsigned char song_idx)
{
	uchar * song;
	uchar* song_name;
	uchar name_len;
	uchar clear_flag=1;	// ֻ����һ������
	uchar note_idx,i=0,key_tmp;	
	timer1_init();
//	timer0_init();
	song_flag=1;

	switch(song_idx)
	{
		case 0: // unsigned -1����!
		{
			song=recorded_songs;
			song_name="recorded_song";
			name_len=14;
			break;//����break�ˣ�����һ��doremi
		}
		case 1: 
		{	song=doremi;			
			song_name="DoReMi";
			name_len=6;
			break;
		}
		case 2: {song=little_star; song_name="С����"; name_len=6;  break;}
		case 3: {song=lemon; song_name="Lemon"; name_len=5;  break;}
	}
	draw_songname(song_name,name_len);
	delay_ms(5);
	clear_LCD();
	while(song_flag)
	{
		KeyIO=0xf0;
		if((KeyIO&0xf0)!=0xf0) 
		{
			 key_tmp=scankey();
		}

		if(key_tmp==14)
		{
			uchar key_tmp2=0;
			KeyIO=0xf0;
			TR1=0;
			bee_Speak=0;
		
			while(1)
			{
				
				Disp(4,0,16,"����ֹ,(2,4)����");
				DLED_1=1;
				DLED_2=1;
				if((KeyIO&0xf0)!=0xf0) 
				{
					key_tmp2=scankey();
					if(key_tmp2==24)
					{
						draw_songname(song_name,name_len);
						break;
					}
					if(key_tmp2==44)
					{
						song_flag=0;
						draw_listen_p1();
						break;
					}
				}
				
			}
			TR1=1;
			bee_Speak=1;
		}
		else if(key_tmp==44){KeyIO=0xf0;song_flag=0;break;}
			
		note_idx=song[i];
		beat_idx=song[i+1];	  
		tie_flag=song[i+2];	  //  �Ƿ�����һ������
		
		draw_notes(note_idx,beat_idx,tie_flag,clear_flag);		
		clear_flag=0;
		if(note_idx==0)
		{	
			TR1=0;
			song_flag=0;
			break;
		}
		
		if(!tie_flag&&i>1)//i==3,note_idx=song[2]
		{
			if(note_idx==song[i-3])
			{
				TR1=0;
				bee_Speak=0;
				DLED_1=1;
				DLED_2=1;
				delay_us(100);
				TR1=1;
			}

		}
	  
		TH1=(notes[note_idx]>>8);
		TL1=(notes[note_idx]&0x00FF);
		TR1=1;

		delay_ms(beats[beat_idx]) ; 
		i+=3;
	}
	TR1=0;




}

/*****************�����ƶ˿ڵ�״̬*****************/
uchar ctrl_port_check(void)
{
	SW_CTRL = 0xFF;
	return 	SW_CTRL&0xF0;
}


uchar scankey(void)//������̷�תɨ��
{
	uint keyvalue=0;
	uchar temp1,temp2,keycode;

	KeyIO=0xf0;		//����0����1
	temp1=KeyIO&0xf0;
	if((temp1&0xf0)==0xf0) return(0);  //���޼����·���0
	KeyIO=0x0f;
	temp2=KeyIO&0x0f;					   //���м����£�����1����0
	keycode=~(temp1|temp2);			   //��ü�����
	switch(keycode)					   //���ݼ����뷵�ؼ�ֵ
	{
		case 0x11:return(11);break;
		case 0x21:return(12);break;
		case 0x41:return(13);break;
		case 0x81:return(14);break;
		case 0x12:return(21);break;
		case 0x22:return(22);break;
		case 0x42:return(23);break;
		case 0x82:return(24);break;
		case 0x14:return(31);break;
		case 0x24:return(32);break;
		case 0x44:return(33);break;
		case 0x84:return(34);break;
		case 0x18:return(41);break;
		case 0x28:return(42);break;
		case 0x48:return(43);break;
		case 0x88:return(44);break;
		default:  return(0);break;
	}
	return(0);
}
	 
/****************�ӳٺ�����************************/
void Delay_xMs(unsigned int x)
{
    unsigned int i,j;
    for( i =0;i < x;i++ )
    {
        for( j =0;j<2;j++ );
    }
}

// ΢�뼶
void delay_us(unsigned int x)
{
	unsigned char t=11;
	unsigned int i; // 255�����ã�
	for(i=0;i<x;i++)
	{
		while(t--);
	}
	
}

// ���뼶
void delay_ms(unsigned int x)
{
	unsigned int t=11,i;
	for(i=0;i<x;i++)
	{
		delay_us(550);
	}
	
}


// LCD������ƺ���
void draw_songname(unsigned char* song_name,unsigned char len)
{	
	Disp(1,0,10,"Playing---");
	Disp(2,0,len,song_name);
	
}

void draw_notes(unsigned char note_idx,unsigned char beat_idx,bit tie_flag,bit clear_flag)
{
	 unsigned char * str;
	 static unsigned char loc=0;
	 static unsigned char col=2;
	 if(clear_flag==1){clear_LCD();loc=0;col=2;}
	 if(loc>7){
	 	loc=0;
		col++;
		if(col>4)col=2;
	 }

	 if(note_idx==0)str="0";
	 else{
	  	switch (note_idx%7)
		{
			case 1: {
				if(note_idx>7&&note_idx<14)str="1M";
				else if(note_idx<7)str="1L";
				else str="1H";
				//WRCGRAM1(0x80);
				break;
			}
	
			case 2:
			{
			 	if(note_idx>7&&note_idx<14)str="1M";
				else if(note_idx<7)str="1L";
				else str="1H";
				break;
			}
			case 3:
			{
			 	if(note_idx>7&&note_idx<14)str="3M";
				else if(note_idx<7)str="3L";
				else str="3H";
				break;
			}
			case 4:
			{
			 	if(note_idx>7&&note_idx<14)str="4M";
				else if(note_idx<7)str="4L";
				else str="4H";
				break;
			}
			case 5:
			{
			 	if(note_idx>7&&note_idx<14)str="5M";
				else if(note_idx<7)str="5L";
				else str="5H";
				break;
			}
			case 6:
			{
			 	if(note_idx>7&&note_idx<14)str="6M";
				else if(note_idx<7)str="6L";
				else str="6H";
				break;
			}
			case 0:
			{
			 	if(note_idx==14)str="7M";
				else if(note_idx==7)str="7L";
				else str="7H";
				break;
			}
	 	}
	 }
	 	 
	 Disp(1,0,10,"Song-Notes");
	 Disp(col,loc,2,str);
	 loc++;
}

void draw_listen_p1()
{
	clear_LCD();
	Disp(1,0,10,"��ѡ����Ŀ:");
	Disp(2,0,10,"(4,4):����");
	Disp(3,0,12,"(1,1):DoReMi");
	Disp(4,0,12,"(1,4):��һҳ");
}


void draw_listen_p2()
{
	clear_LCD();
	Disp(1,0,12,"(1,1):С����");
	Disp(2,0,12,"(1,2):Lemon");
	Disp(3,0,10,"(4,4):����");

}

void draw_cover()
{
	clear_LCD();
	Disp(1,0,16,"��ӭʹ��������!");
	Disp(2,1,12,"��(1,1):����");
	Disp(3,1,12,"��(1,2):����");
	Disp(4,1,14,"��(1,3):��һҳ");
}

void draw_cover_p2()
{
	clear_LCD();
	Disp(1,0,12,"��(1,1):¼��");
	Disp(2,0,16,"��(1,2):����¼��");
	Disp(3,0,16,"��(1,3):�����ʦ");
	Disp(4,0,14,"��(4,4):��һҳ");
}

void draw_record_p2()
{
	 clear_LCD();
	 Disp(1,0,10,"����¼��--");
	 Disp(2,0,14,"Recorded-Songs");
}





// ����ģ��
void play()
{
	 bit tmp_flag=1;
	 clear_LCD();
	 Disp(1,0,8,"׼������");
	 Disp(2,0,15,"(1,4)-L,(2,4)-M");
	 Disp(3,0,16,"(3,4)-H,(4,4)-Bk");
	 delay_ms(5);
	 clear_LCD();
	 Disp(1,0,8,"Playing-");
	 while(tmp_flag&&(OPT_CHECK&0x7F) != 0x7F)
	 {
	 	 if((KeyIO&0xf0)!=0xf0)	
		 {									 	
		 	uchar key1=scankey();								
			switch(key1)
		 	{
			 	case 14: {note=note_l;tune_tag=0;break;}
				case 24: {note=note_m;tune_tag=1;break;}
				case 34: {note=note_h;tune_tag=2;break;}
				case 44: {tmp_flag=0;bee_Speak=0;TR1=0;tune_tag=1;break;}

			 }
		 }
		 
		 if(OPT_CHECK&0x01)
			{
			   if(bee_cnt>250)
			   {
				draw_notes(1+tune_tag*7,1,0,0);	
			/*	if(bee_cnt>500)
				{
					if(idx>1&&recorded_songs[idx-3]==1+tune_tag*7)
					{
						recorded_songs[idx+2]=1; //����
					}
				} */
				bee_cnt=0;
				DLED_1=~DLED_1;
				}
			  
			   TH1=note[0]>>8;
			   TL1=note[0]&0x00FF;
			   //ConfigPWM(note[0],0);
			   //while(OPT_CHECK&0x01);
			   TR1=1;
			   //timer_cnt=0;
			 }
		
			 else if(OPT_CHECK&0x02)
			 {
			   if(bee_cnt>210)
			   {
				draw_notes(2+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
			   }
	
			   TH1=note[1]>>8;
			   TL1=note[1]&0x00FF;
			   //ConfigPWM(note[1],0);
			   //while(OPT_CHECK&0x02);
			   TR1=1;
			   //timer_cnt=0;
			 }
			
			 else if(OPT_CHECK&0x04)
			 {
			   if(bee_cnt>200)
			   {
				draw_notes(3+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
			   }
	
			   TH1=note[2]>>8;
			   TL1=note[2]&0x00FF;
			   //ConfigPWM(note[2],0);
			   //while(OPT_CHECK&0x04);
			   TR1=1;
			  // timer_cnt=0;
			   
			 }
			 
			 else if(OPT_CHECK&0x08)
			 {
			   if(bee_cnt>200)
			   {
				draw_notes(4+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
			   }
	
			   TH1=note[3]>>8;
			   TL1=note[3]&0x00FF;
			   //ConfigPWM(note[3],0);
			   //while(OPT_CHECK&0x08);
			   TR1=1;
			  // timer_cnt=0;
			 }
			 
			 else if(OPT_CHECK&0x10)
			 {
			   if(bee_cnt>250)
			   {
				draw_notes(5+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
				}
	
			   TH1=note[4]>>8;
			   TL1=note[4]&0x00FF;
			   //ConfigPWM(note[4],0);
			   //while(OPT_CHECK&0x10);
			   TR1=1;
			  // timer_cnt=0;
			 }
			 
			 else if(OPT_CHECK&0x20)
			 {
			   if(bee_cnt>200)
			   {
				draw_notes(6+tune_tag*7,1,0,0);	
				bee_cnt=0;
				DLED_1=~DLED_1;
			  }
	
			   TH1=note[5]>>8;
			   TL1=note[5]&0x00FF;
			   //ConfigPWM(note[5],0);
			   //while(OPT_CHECK&0x20);
			   TR1=1;
			  // timer_cnt=0;
			 }
			 
			 else if(OPT_CHECK&0x40)
			 {
			   if(bee_cnt>200)
			   	{
					draw_notes(7+tune_tag*7,1,0,0);	
					DLED_1=~DLED_1;
					bee_cnt=0;
					//bee_Speak=0;
				}
	
			   TH1=note[6]>>8;
			   TL1=note[6]&0x00FF;
			   /*ConfigPWM(note[6],0);
			   while(OPT_CHECK&0x40);
			   TR1=0;*/
			   TR1=1;
			  // timer_cnt=0;
			 }
		 

		 else
		 {
		   //draw_notes(0,1,0,0);
		   TR1=0;
		   bee_Speak=0;
		 }				 

		
	}
}


// ¼������

void recording(unsigned char tune_tag,unsigned char note_idx,unsigned char idx)
{

	recorded_songs[idx]=note_idx+tune_tag*7;

	recorded_songs[idx+1]=1; // �ɸĽ�
	recorded_songs[idx+2]=0; // �ɸĽ�	


}


// ¼��ģ��
void record()
{
	unsigned char idx=0;
	 bit stop=0;
	 bit start=0;
	 Disp(1,0,10,"׼��¼��--");	
	 Disp(2,0,11,"(1,3):Start");
	 Disp(3,0,9,"(2,3):End");
	 Disp(4,0,13,"(4,3):Restart");
	 						 
	 while(!stop)
	 {
	 	uchar key1=scankey();
		switch(key1)
		{
		
			case 13:
			{
				if(start==0)
				{
					clear_LCD();
					timer1_init();
					start=1;
				}	
				break;	
			}
			case 23:
			{
				stop=1;
				TR1=0;
				bee_Speak=0;
				cover_page=2;
				draw_cover_p2();
				break;
			}
			case 14: {note=note_l;tune_tag=0;break;}
			case 24: {note=note_m;tune_tag=1;break;}
			case 34: {note=note_h;tune_tag=2;break;}
			case 44: {stop=1;draw_notes(0,1,0,0);break;}
			case 43: 
			{
				uchar i =0;
				Disp(1,0,10,"Deleting--");
				delay_ms(5);
				clear_LCD();
				Disp(1,0,10,"׼��¼��--");	
				Disp(2,0,11,"(1,3):Start");
	 			Disp(3,0,9,"(2,3):End");
	 			Disp(4,0,10,"(4,4):����");
				DLED_1=0;
				DLED_2=1;
				idx=0;
				for(i=0;i<100;i++)
				{
					recorded_songs[i]=0;
				}
				break;
			}
	
	 	}

		if(start)
		{
			if(idx>96){start=0;idx=0;clear_LCD();Disp(1,0,8,"¼������");}
			if(OPT_CHECK&0x01)
			 {
			   if(bee_cnt>250)
			   {
			   	recording(tune_tag,1,idx);
				draw_notes(1+tune_tag*7,1,0,0);	
			/*	if(bee_cnt>500)
				{
					if(idx>1&&recorded_songs[idx-3]==1+tune_tag*7)
					{
						recorded_songs[idx+2]=1; //����
					}
				} */
				bee_cnt=0;
				DLED_1=~DLED_1;
				idx+=3;
				}
			  
			   TH1=note[0]>>8;
			   TL1=note[0]&0x00FF;
			   //ConfigPWM(note[0],0);
			   //while(OPT_CHECK&0x01);
			   TR1=1;
			   //timer_cnt=0;
			 }
		
			 else if(OPT_CHECK&0x02)
			 {
			   if(bee_cnt>210)
			   {
			   	recording(tune_tag,2,idx);
				draw_notes(2+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
				idx+=3;
			   }
	
			   TH1=note[1]>>8;
			   TL1=note[1]&0x00FF;
			   //ConfigPWM(note[1],0);
			   //while(OPT_CHECK&0x02);
			   TR1=1;
			   //timer_cnt=0;
			 }
			
			 else if(OPT_CHECK&0x04)
			 {
			   if(bee_cnt>200)
			   {
			   	recording(tune_tag,3,idx);
				draw_notes(3+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
				idx+=3;
			   }
	
			   TH1=note[2]>>8;
			   TL1=note[2]&0x00FF;
			   //ConfigPWM(note[2],0);
			   //while(OPT_CHECK&0x04);
			   TR1=1;
			  // timer_cnt=0;
			   
			 }
			 
			 else if(OPT_CHECK&0x08)
			 {
			   if(bee_cnt>200)
			   {
			   	recording(tune_tag,4,idx);
				draw_notes(4+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
				idx+=3;
			   }
	
			   TH1=note[3]>>8;
			   TL1=note[3]&0x00FF;
			   //ConfigPWM(note[3],0);
			   //while(OPT_CHECK&0x08);
			   TR1=1;
			  // timer_cnt=0;
			 }
			 
			 else if(OPT_CHECK&0x10)
			 {
			   if(bee_cnt>250)
			   {
			   	recording(tune_tag,5,idx);
				draw_notes(5+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
				idx+=3;
				}
	
			   TH1=note[4]>>8;
			   TL1=note[4]&0x00FF;
			   //ConfigPWM(note[4],0);
			   //while(OPT_CHECK&0x10);
			   TR1=1;
			  // timer_cnt=0;
			 }
			 
			 else if(OPT_CHECK&0x20)
			 {
			   if(bee_cnt>200)
			   {
			   	recording(tune_tag,6,idx);
				draw_notes(6+tune_tag*7,1,0,0);	
				bee_cnt=0;
				DLED_1=~DLED_1;
				idx+=3;
			  }
	
			   TH1=note[5]>>8;
			   TL1=note[5]&0x00FF;
			   //ConfigPWM(note[5],0);
			   //while(OPT_CHECK&0x20);
			   TR1=1;
			  // timer_cnt=0;
			 }
			 
			 else if(OPT_CHECK&0x40)
			 {
			   if(bee_cnt>200)
			   	{
					recording(tune_tag,7,idx);
					draw_notes(7+tune_tag*7,1,0,0);	
					DLED_1=~DLED_1;
					bee_cnt=0;
					idx+=3;
					//bee_Speak=0;
				}
	
			   TH1=note[6]>>8;
			   TL1=note[6]&0x00FF;
			   /*ConfigPWM(note[6],0);
			   while(OPT_CHECK&0x40);
			   TR1=0;*/
			   TR1=1;
			  // timer_cnt=0;
			 }
			 else
			 {
			   TR1=0;
			   bee_Speak=0;
			  /* if(bee_cnt>50)
			   {
			   	recording(tune_tag,0);
				bee_cnt=0;
				stop=1;
				TR1=0;
				bee_Speak=0;
				cover_page=2;
				draw_cover_p2();
				DLED_1=0;
				DLED_2=1;	
				idx=0;
				break;
			   }  */
		/*	   bee_Speak=0;
			   TR0=1;
			   if(stop_flag==0)
			   {
			   	 recording(tune_tag,0);
			   	 stop_flag=1;
			   	 TR0=0;
				}  */
			 }				 
		
			}													
			 
	}
}

// �����ʦ��С������ϰ
void practice()
{
	 unsigned char idx=0;
	 bit stop=0;
	 bit start=0;
	 uchar i=0;
	 uchar score=0;

	 while(!stop)
	 {
	 	uchar key1=scankey();
		switch(key1)
		{
		
			case 13:
			{
				if(start==0)
				{
					clear_LCD();
					timer1_init();
					start=1;
				}	
				break;	
			}
			case 23:
			{
				stop=1;
				TR1=0;
				bee_Speak=0;
				cover_page=2;
				draw_cover_p2();
				break;
			}
			case 14: {note=note_l;tune_tag=0;break;}
			case 24: {note=note_m;tune_tag=1;break;}
			case 34: {note=note_h;tune_tag=2;break;}
			case 44: {stop=1;draw_notes(0,1,0,0);break;}
			case 43: 
			{
				uchar i =0;
				Disp(1,0,10,"Deleting--");
				delay_ms(5);
				clear_LCD();
				Disp(1,0,10,"С������ϰ");	
				Disp(2,0,11,"(1,3):Start");
	 			Disp(3,0,9,"(2,3):End");
	 			Disp(4,0,10,"(4,4):����");
				DLED_1=0;
				DLED_2=1;
				idx=0;
				for(i=0;i<100;i++)
				{
					recorded_songs[i]=0;
				}
				break;
			}
	
	 	}

		if(start)
		{
			if(idx>96){start=0;idx=0;clear_LCD();Disp(1,0,8,"��ϰ����");}
			if(OPT_CHECK&0x01)
			 {
			   if(bee_cnt>250)
			   {
			   	recording(tune_tag,1,idx);
				draw_notes(1+tune_tag*7,1,0,0);	
			/*	if(bee_cnt>500)
				{
					if(idx>1&&recorded_songs[idx-3]==1+tune_tag*7)
					{
						recorded_songs[idx+2]=1; //����
					}
				} */
				bee_cnt=0;
				DLED_1=~DLED_1;
				idx+=3;
				}
			  
			   TH1=note[0]>>8;
			   TL1=note[0]&0x00FF;
			   //ConfigPWM(note[0],0);
			   //while(OPT_CHECK&0x01);
			   TR1=1;
			   //timer_cnt=0;
			 }
		
			 else if(OPT_CHECK&0x02)
			 {
			   if(bee_cnt>210)
			   {
			   	recording(tune_tag,2,idx);
				draw_notes(2+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
				idx+=3;
			   }
	
			   TH1=note[1]>>8;
			   TL1=note[1]&0x00FF;
			   //ConfigPWM(note[1],0);
			   //while(OPT_CHECK&0x02);
			   TR1=1;
			   //timer_cnt=0;
			 }
			
			 else if(OPT_CHECK&0x04)
			 {
			   if(bee_cnt>200)
			   {
			   	recording(tune_tag,3,idx);
				draw_notes(3+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
				idx+=3;
			   }
	
			   TH1=note[2]>>8;
			   TL1=note[2]&0x00FF;
			   //ConfigPWM(note[2],0);
			   //while(OPT_CHECK&0x04);
			   TR1=1;
			  // timer_cnt=0;
			   
			 }
			 
			 else if(OPT_CHECK&0x08)
			 {
			   if(bee_cnt>200)
			   {
			   	recording(tune_tag,4,idx);
				draw_notes(4+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
				idx+=3;
			   }
	
			   TH1=note[3]>>8;
			   TL1=note[3]&0x00FF;
			   //ConfigPWM(note[3],0);
			   //while(OPT_CHECK&0x08);
			   TR1=1;
			  // timer_cnt=0;
			 }
			 
			 else if(OPT_CHECK&0x10)
			 {
			   if(bee_cnt>250)
			   {
			   	recording(tune_tag,5,idx);
				draw_notes(5+tune_tag*7,1,0,0);	
				DLED_1=~DLED_1;
				bee_cnt=0;
				idx+=3;
				}
	
			   TH1=note[4]>>8;
			   TL1=note[4]&0x00FF;
			   //ConfigPWM(note[4],0);
			   //while(OPT_CHECK&0x10);
			   TR1=1;
			  // timer_cnt=0;
			 }
			 
			 else if(OPT_CHECK&0x20)
			 {
			   if(bee_cnt>200)
			   {
			   	recording(tune_tag,6,idx);
				draw_notes(6+tune_tag*7,1,0,0);	
				bee_cnt=0;
				DLED_1=~DLED_1;
				idx+=3;
			  }
	
			   TH1=note[5]>>8;
			   TL1=note[5]&0x00FF;
			   //ConfigPWM(note[5],0);
			   //while(OPT_CHECK&0x20);
			   TR1=1;
			  // timer_cnt=0;
			 }
			 
			 else if(OPT_CHECK&0x40)
			 {
			   if(bee_cnt>200)
			   	{
					recording(tune_tag,7,idx);
					draw_notes(7+tune_tag*7,1,0,0);	
					DLED_1=~DLED_1;
					bee_cnt=0;
					idx+=3;
					//bee_Speak=0;
				}
	
			   TH1=note[6]>>8;
			   TL1=note[6]&0x00FF;
			   /*ConfigPWM(note[6],0);
			   while(OPT_CHECK&0x40);
			   TR1=0;*/
			   TR1=1;
			  // timer_cnt=0;
			 }
			 else
			 {
			   TR1=0;
			   bee_Speak=0;

			 }				 
		
			}													
			 
	}

	clear_LCD();
	Disp(1,0,8,"Result--");
	
	for(i=0;i<42;i+=3)
	{
		if(recorded_songs[i]==little_star[i])score++;	
	}
	if(score>35)Disp(2,0,6,"Great!");
	else if(score>20&&score<=35)Disp(2,0,6,"Good~~");
	else if(score>10&&score<=20)Disp(2,0,6,"Nice~~");
	else Disp(2,0,8,"Not-Good");
	delay_ms(6);
	clear_LCD();
	idx=0;
	for(i=0;i<100;i++)
	{
		recorded_songs[i]=0;
	}


}
