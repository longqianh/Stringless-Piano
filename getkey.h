//getkey.h  �����жϳ���

#ifndef _getkey_h_
#define _getkey_h_

uint keyvalue=0;
uchar temp1,temp2,keycode;

//void getkey(void)//����������ȡ //ע��������ΪP3
//{
//	Uchar temp,status;
//    if((P3&0xf0)!=0xf0)
//	{
//		WaitNms(10);
//		if((P3&0xf0)!=0xf0)
//		{
//			status=P3&0xf0;
//			switch(status)
//			{			
//				case 0xe0:keyvalue=1;P6=0x01;break;
//				case 0xd0:keyvalue=2;P6=0x02;break;
//				case 0xb0:keyvalue=3;P6=0x04;break;
//				case 0x70:keyvalue=4;P6=0x08;break;
//				default:keyvalue=0;P6=0x00;break;
//			}
//		}
//	do	{P3=0xf0; temp=P3;}
//	while((temp&0xf0)!=0xf0); 
//	}	
//}

uchar scankey(void)//������̷�תɨ��
{
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
#endif