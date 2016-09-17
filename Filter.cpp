// Filter.cpp: implementation of the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Filter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilter::CFilter()
{

}

CFilter::~CFilter()
{

}
void CFilter::FFT_1D(CComplex * pCTData, CComplex * pCFData, int nLevel)
{
		// ѭ�����Ʊ���
	int		i;
	int     j;
	int     k;

	// ����Ҷ�任����
	int	nCount =1 ;

	// ���㸵��Ҷ�任����
	//nCount =(int)pow(2,nLevel) ;
	for(i=0;i<nLevel;i++)
	{
		nCount*=2;
	}
	
	// ĳһ���ĳ���
	int		nBtFlyLen;
	nBtFlyLen = 0 ;
	
	// �任ϵ���ĽǶ� ��2 * PI * i / nCount
	double	dAngle;
	
	CComplex *pCW ;
	
	// �����ڴ棬�洢����Ҷ�仯��Ҫ��ϵ����
	pCW  = new CComplex[nCount / 2];

    // ���㸵��Ҷ�任��ϵ��
	for(i = 0; i < nCount / 2; i++)
	{
		dAngle = -2 * pi * i / nCount;
		pCW[i] = CComplex( cos(dAngle), sin(dAngle) );
	}

	// �任��Ҫ�Ĺ����ռ�
	CComplex *pCWork1,*pCWork2; 
	
	// ���乤���ռ�
	pCWork1 = new CComplex[nCount];

	pCWork2 = new CComplex[nCount];

	
	// ��ʱ����
	CComplex *pCTmp;
	
	// ��ʼ����д������
	memcpy(pCWork1, pCTData, sizeof(CComplex) * nCount);

	// ��ʱ����
	int nInter; 
	nInter = 0;

	// �����㷨���п��ٸ���Ҷ�任
	for(k = 0; k < nLevel; k++)
	{
		for(j = 0; j < (int)pow(2,k); j++)
		{
			//���㳤��
			nBtFlyLen = (int)pow( 2,(nLevel-k) );
			
			//�������ţ���Ȩ����
			for(i = 0; i < nBtFlyLen/2; i++)
			{
				nInter = j * nBtFlyLen;
				pCWork2[i + nInter] =
					pCWork1[i + nInter]+pCWork1[i + nInter + nBtFlyLen / 2];
				pCWork2[i + nInter + nBtFlyLen / 2] =
					(pCWork1[i + nInter]-pCWork1[i + nInter + nBtFlyLen / 2])*pCW[(int)(i * pow(2,k))];
			}
		}

		// ���� pCWork1��pCWork2������
		pCTmp   = pCWork1	;
		pCWork1 = pCWork2	;
		pCWork2 = pCTmp		;
	}
	
	// ��������
	for(j = 0; j < nCount; j++)
	{
		nInter = 0;
		for(i = 0; i < nLevel; i++)
		{
			if ( j&(1<<i) )
			{
				nInter += 1<<(nLevel-i-1);
			}
		}
		pCFData[j]=pCWork1[nInter];
	}
	
	// �ͷ��ڴ�ռ�
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW		=	NULL	;
	pCWork1 =	NULL	;
	pCWork2 =	NULL	;

}
void CFilter::IFFT_1D(CComplex * pCFData, CComplex * pCTData, int nLevel)
{
	// ѭ�����Ʊ���
	int		i;

	// ����Ҷ���任����
	int nCount;

	// ���㸵��Ҷ�任����
	nCount = (int)pow(2,nLevel) ;
	
	// �任��Ҫ�Ĺ����ռ�
	CComplex *pCWork;	
	
	// ���乤���ռ�
	pCWork = new CComplex[nCount];
	
	// ����Ҫ���任������д�빤���ռ�pCWork
	memcpy(pCWork, pCFData, sizeof(CComplex) * nCount);
	
	// Ϊ�����ø���Ҷ���任,���԰Ѹ���ҶƵ�������ȡ����
	// Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ���
	for(i = 0; i < nCount; i++)
	{
		pCWork[i].r=pCWork[i].r;
		pCWork[i].i=pCWork[i].i;
	}
	
	// ���ÿ��ٸ���Ҷ�任ʵ�ַ��任������洢��pCTData��
	FFT_1D(pCWork, pCTData, nLevel);
	
	// ��ʱ���Ĺ��������ս��
	// ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������
	// ���һ��ϵ��nCount
	for(i = 0; i < nCount; i++)
	{
		pCTData[i] = 
			CComplex(pCTData[i].r / nCount, -pCTData[i].i / nCount);
	}
	
	// �ͷ��ڴ�
	delete pCWork;
	pCWork = NULL;
}
void CFilter::FFTChange (double *Data,double *FData, long length)
{
 	int	x;
	CComplex * pCTData;
	CComplex * pCFData;
 	
	// ��ʱ����
 	double	dTmpOne;
	double  dTmpTwo;
	int level;
	
 	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���2���������ݣ�
 	// ͼ��Ŀ�Ⱥ͸߶Ȳ�һ��Ϊ2����������
	int		nTransWidth;
 	
 	// ������и���Ҷ�任�Ŀ��	��2���������ݣ�
 	dTmpOne = log(length)/log(2);
 	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
 	nTransWidth = (int) dTmpTwo	   ;
	level=(int)ceil(dTmpOne);
 	
 	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ�
    pCTData=new CComplex[nTransWidth];
	pCFData=new CComplex[nTransWidth];	
	for(int j=0;j<nTransWidth;j++)
	{
		pCTData[j].r=Data[j];
 		pCTData[j].i=0;			
 	}
 	FFT_1D(pCTData, pCFData,level) ;					// ����Ҷ���任		
	for(x = 0; x < nTransWidth; x++)
	{
		double temp=sqrt(pCFData[x].r*pCFData[x].r+pCFData[x].i*pCFData[x].i);
		FData[x]=temp;
 	}
	delete pCTData;
 	delete pCFData;
}

