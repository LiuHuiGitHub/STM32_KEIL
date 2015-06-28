#ifndef __MYLIBRARY_H__
#define __MYLIBRARY_H__


#define PRINT_EN              1                                                //1ʹ�ܴ�ӡ��Ϣ��0���ܴ�ӡ��Ϣ
//���½�C99��׼֧��
#if PRINT_EN > 0
#define printu(...)  printf(__VA_ARGS__)                                    //��ͨ��Ϣ��ӡ
#define printe(...)  {printu("Error File:%s, Function:%s, Line%d:", __FILE__, __FUNCTION__, __LINE__); printu(__VA_ARGS__);}   //���������Ϣ																												 
#else
#define printu(...)
#define printe(...)
#endif


void Quick_Sort(int *pData, int count); //��������
void Bubble_Sort(int *pData, int count); //ð������
void Select_Sort(int *pData, int count); //ѡ������
void Insert_Sort(int *pData, int count); //��������
unsigned short CRC16RTU(unsigned char *pszBuf, unsigned short unLength);

int RemoveExtremumAverage(int *pData, int count, void (*AverageWay)(), int RemoveNum);//ȥ��ֵƽ���˲�
typedef struct
{
	int *Data;
	int NewData;
	int QueueLength;
	int InsertPlace;
} QueueAverage;
/*ʹ�÷���
	static int ValueBuf[6];
	static QueueAverage Q={ValueBuf, 0, 6, 0};
	Q.NewData = ReadADCAverageValue(0x00);
	temp = RecursiveAverage(&Q);
*/
int RecursiveAverageSort(QueueAverage *queue);//����ƽ���˲�

#endif
