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
		// 循环控制变量
	int		i;
	int     j;
	int     k;

	// 傅立叶变换点数
	int	nCount =1 ;

	// 计算傅立叶变换点数
	//nCount =(int)pow(2,nLevel) ;
	for(i=0;i<nLevel;i++)
	{
		nCount*=2;
	}
	
	// 某一级的长度
	int		nBtFlyLen;
	nBtFlyLen = 0 ;
	
	// 变换系数的角度 ＝2 * PI * i / nCount
	double	dAngle;
	
	CComplex *pCW ;
	
	// 分配内存，存储傅立叶变化需要的系数表
	pCW  = new CComplex[nCount / 2];

    // 计算傅立叶变换的系数
	for(i = 0; i < nCount / 2; i++)
	{
		dAngle = -2 * pi * i / nCount;
		pCW[i] = CComplex( cos(dAngle), sin(dAngle) );
	}

	// 变换需要的工作空间
	CComplex *pCWork1,*pCWork2; 
	
	// 分配工作空间
	pCWork1 = new CComplex[nCount];

	pCWork2 = new CComplex[nCount];

	
	// 临时变量
	CComplex *pCTmp;
	
	// 初始化，写入数据
	memcpy(pCWork1, pCTData, sizeof(CComplex) * nCount);

	// 临时变量
	int nInter; 
	nInter = 0;

	// 蝶形算法进行快速傅立叶变换
	for(k = 0; k < nLevel; k++)
	{
		for(j = 0; j < (int)pow(2,k); j++)
		{
			//计算长度
			nBtFlyLen = (int)pow( 2,(nLevel-k) );
			
			//倒序重排，加权计算
			for(i = 0; i < nBtFlyLen/2; i++)
			{
				nInter = j * nBtFlyLen;
				pCWork2[i + nInter] =
					pCWork1[i + nInter]+pCWork1[i + nInter + nBtFlyLen / 2];
				pCWork2[i + nInter + nBtFlyLen / 2] =
					(pCWork1[i + nInter]-pCWork1[i + nInter + nBtFlyLen / 2])*pCW[(int)(i * pow(2,k))];
			}
		}

		// 交换 pCWork1和pCWork2的数据
		pCTmp   = pCWork1	;
		pCWork1 = pCWork2	;
		pCWork2 = pCTmp		;
	}
	
	// 重新排序
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
	
	// 释放内存空间
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW		=	NULL	;
	pCWork1 =	NULL	;
	pCWork2 =	NULL	;

}
void CFilter::IFFT_1D(CComplex * pCFData, CComplex * pCTData, int nLevel)
{
	// 循环控制变量
	int		i;

	// 傅立叶反变换点数
	int nCount;

	// 计算傅立叶变换点数
	nCount = (int)pow(2,nLevel) ;
	
	// 变换需要的工作空间
	CComplex *pCWork;	
	
	// 分配工作空间
	pCWork = new CComplex[nCount];
	
	// 将需要反变换的数据写入工作空间pCWork
	memcpy(pCWork, pCFData, sizeof(CComplex) * nCount);
	
	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭
	for(i = 0; i < nCount; i++)
	{
		pCWork[i].r=pCWork[i].r;
		pCWork[i].i=pCWork[i].i;
	}
	
	// 调用快速傅立叶变换实现反变换，结果存储在pCTData中
	FFT_1D(pCWork, pCTData, nLevel);
	
	// 求时域点的共轭，求得最终结果
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据
	// 相差一个系数nCount
	for(i = 0; i < nCount; i++)
	{
		pCTData[i] = 
			CComplex(pCTData[i].r / nCount, -pCTData[i].i / nCount);
	}
	
	// 释放内存
	delete pCWork;
	pCWork = NULL;
}
void CFilter::FFTChange (double *Data,double *FData, long length)
{
 	int	x;
	CComplex * pCTData;
	CComplex * pCFData;
 	
	// 临时变量
 	double	dTmpOne;
	double  dTmpTwo;
	int level;
	
 	// 进行傅立叶变换的宽度和高度，（2的整数次幂）
 	// 图像的宽度和高度不一定为2的整数次幂
	int		nTransWidth;
 	
 	// 计算进行傅立叶变换的宽度	（2的整数次幂）
 	dTmpOne = log(length)/log(2);
 	dTmpTwo = ceil(dTmpOne)		   ;
	dTmpTwo = pow(2,dTmpTwo)	   ;
 	nTransWidth = (int) dTmpTwo	   ;
	level=(int)ceil(dTmpOne);
 	
 	// 计算进行傅立叶变换的高度 （2的整数次幂）
    pCTData=new CComplex[nTransWidth];
	pCFData=new CComplex[nTransWidth];	
	for(int j=0;j<nTransWidth;j++)
	{
		pCTData[j].r=Data[j];
 		pCTData[j].i=0;			
 	}
 	FFT_1D(pCTData, pCFData,level) ;					// 傅立叶正变换		
	for(x = 0; x < nTransWidth; x++)
	{
		double temp=sqrt(pCFData[x].r*pCFData[x].r+pCFData[x].i*pCFData[x].i);
		FData[x]=temp;
 	}
	delete pCTData;
 	delete pCFData;
}

