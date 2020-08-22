#ifndef _SYS_FUN_H_
#define _SYS_FUN_H_

//#include <STC15F2K60S2.h>
#include "sys_cfg.h"
 

void timer0_init();//��ʱ����ʼ��
void timer1_init();
void timer1_isr();
void timer0_isr();
void s1_s2_check(void);//S1_S2���
void Play_Song(unsigned char i);//��������
uchar ctrl_port_check(void);//�����ƶ˿ڵ�״̬
uchar scankey(void);//������̷�תɨ��
void Delay_xMs(unsigned int x);//�ӳٺ�����
void delay_us(unsigned int x);
void delay_ms(unsigned int x);
void ConfigPWM(unsigned int fr, unsigned char dc);
char * num2str(unsigned int x);
void draw_songname(unsigned char* song_name,unsigned char len);
void draw_notes(unsigned char note_idx,unsigned char beat_idx,bit tie_flag,bit clear_flag);
//void AdjustDutyCycle(unsigned char dc);
void draw_cover();
void draw_listen_p1();
void draw_listen_p2();
void draw_record_p2();
void recording(unsigned char tune_tag,unsigned char note_idx,unsigned char idx);
void draw_cover_p2();
void record();
void play();
void practice();

#endif