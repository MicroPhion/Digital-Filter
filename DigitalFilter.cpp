// DigitalFilter.cpp: implementation of the CDigitalFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyFilter.h"
#include "DigitalFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDigitalFilter::CDigitalFilter()
{

}

CDigitalFilter::~CDigitalFilter()
{

}
bool CDigitalFilter::Initialization(tagDigitalFilterParameters &m_nFilterparamter,
									const tagInputParameter Input)
{
	m_nFilterparamter.Filter_Class = Input.Filter_Class;
	m_nFilterparamter.Filter_Mode = Input.Filter_Mode;
	m_nFilterparamter.Filter_Type = Input.Filter_Type;
	m_nFilterparamter.SampleFrequency = Input.SampleFrequency;
	double temp;
	switch(Input.Filter_Type) 
	{
	case 1://LowPass
		m_nFilterparamter.Frequency_Pass[0] = Input.LowPass_Frequency_CutOff/(double)Input.SampleFrequency;
		m_nFilterparamter.Frequency_Pass[1] = 0;


		temp = log10(StopGain) / log10(Input.LowPass_Frequency_CutOff_Slope);
		m_nFilterparamter.Frequency_Stop[0] = Input.LowPass_Frequency_CutOff * pow(2,temp)/Input.SampleFrequency;
		m_nFilterparamter.Frequency_Stop[1] = 0;
		if(m_nFilterparamter.Frequency_Stop[0] >= 1)
		{
			AfxMessageBox("低通阻带截止频率过大！");
			return false;
		}

		break;

	case 2://HighPass
		m_nFilterparamter.Frequency_Pass[0] = 0;
		m_nFilterparamter.Frequency_Pass[1] = Input.HighPass_Frequency_CutOff/(double)Input.SampleFrequency;

		temp = log10(StopGain) / log10(Input.HighPass_Frequency_CutOff_Slope);
		m_nFilterparamter.Frequency_Stop[0] = 0;
		m_nFilterparamter.Frequency_Stop[1] =Input.HighPass_Frequency_CutOff / pow(2,temp)/Input.SampleFrequency;

		if(m_nFilterparamter.Frequency_Pass[1] >= 1)
		{
			AfxMessageBox("高通通带截止频率过大！");
			return false;
		}

		break;
	case 3://BandPass
		m_nFilterparamter.Frequency_Pass[0] = Input.LowPass_Frequency_CutOff/(double)Input.SampleFrequency;
		m_nFilterparamter.Frequency_Pass[1] = Input.HighPass_Frequency_CutOff/(double)Input.SampleFrequency;
		
		
		temp = log10(StopGain) / log10(Input.LowPass_Frequency_CutOff_Slope);
		m_nFilterparamter.Frequency_Stop[0] = Input.LowPass_Frequency_CutOff * pow(2, temp)/Input.SampleFrequency;

		temp = log10(StopGain) / log10(Input.HighPass_Frequency_CutOff_Slope);
		m_nFilterparamter.Frequency_Stop[1] = Input.HighPass_Frequency_CutOff / pow(2, temp)/Input.SampleFrequency;
		
		if(m_nFilterparamter.Frequency_Stop[0] >= 1)
		{
			AfxMessageBox("低通阻带截止频率过大！");
			return false;
		}
		if(m_nFilterparamter.Frequency_Pass[0] < m_nFilterparamter.Frequency_Pass[1])
		{
			AfxMessageBox("低通通带截止频率小于高通通带截止频率！");
			return false;
		}
		break;
	case 4://StopPass
		m_nFilterparamter.Frequency_Pass[0] = Input.Start_Frequency /(double)Input.SampleFrequency;
		m_nFilterparamter.Frequency_Pass[1] = Input.Stop_Frequency/(double)Input.SampleFrequency;
		
		
		temp = log10(StopGain) / log10(Input.Start_Frequency_Slope);
		m_nFilterparamter.Frequency_Stop[0] =Input.Start_Frequency * pow(2,temp)/Input.SampleFrequency;
		
		temp = log10(StopGain) / log10(Input.Stop_Frequency_Slope);
		m_nFilterparamter.Frequency_Stop[1] =Input.Stop_Frequency / pow(2,temp)/Input.SampleFrequency;

		if(m_nFilterparamter.Frequency_Pass[0] >= 1)
		{
			AfxMessageBox("高通通带截止频率过大！");
			return false;
		}
		if(m_nFilterparamter.Frequency_Stop[1] < m_nFilterparamter.Frequency_Stop[0])
		{
			AfxMessageBox("低通阻带截止频率大于高阻通带截止频率！");
			return false;
		}

		break;
	default:
		break;
	}

	m_nFilterparamter.Order = GetFilterOrder(Input,m_nFilterparamter);
	return true;
}


bool CDigitalFilter::DesignFIRFilter(	tagDigitalFilterParameters &m_nFilterParameter,
										double *MultinomialCoefficientDenominator,//滤波器分母多项式的系数
										double *MultinomialCoefficientNumerator)//滤波器分子多项式的系数
{



	int i,n2,mid;
	double s,wc1,wc2,delay;
	int n = m_nFilterParameter.Order;
	double * h = MultinomialCoefficientNumerator;
	double pi = 4.0 * atan(1.0);



	wc1 = 0; 
	wc2 = 0;
	if((n % 2) == 0)
	{
        n2 = n / 2 - 1;
        mid = 1;
	}
	else
	{
		n2 = n / 2;
		mid = 0;
	}

	delay = m_nFilterParameter.Order / 2.0;	

	switch(m_nFilterParameter.Filter_Type)
	{
	case 1: 
		wc1 = 2 * pi * (m_nFilterParameter.Frequency_Pass[0] + m_nFilterParameter.Frequency_Stop[0]) / 2;
		for(i = 0; i <= n2; i ++)
		{
			s = i - delay;
			h[i]= (sin(wc1 * s) / (pi * s))	 *	WindowFunction(n + 1, i);
			h[n-i] = h[i];	
		}
		if(mid == 1)
			h[n / 2] = (wc1 / pi);
		break;
	case 2:
		wc2 = 2 * pi * (m_nFilterParameter.Frequency_Pass[1] + m_nFilterParameter.Frequency_Stop[1]) / 2;
		for(i = 0; i <= n2; i ++)
		{
			s = i - delay;
			h[i]= ((sin(pi * s) - sin(wc2 * s)) / (pi * s));
			h[i]= (h[i] * WindowFunction(n+1,i));
			h[n-i]=h[i];
		}
		if(mid==1)
			h[n / 2]=  (1.0-wc2/pi);
		break;
	case 3: 
		wc1 = 2 * pi * (m_nFilterParameter.Frequency_Pass[1] + m_nFilterParameter.Frequency_Stop[1]) / 2;
		wc2 = 2 * pi * (m_nFilterParameter.Frequency_Pass[0] + m_nFilterParameter.Frequency_Stop[0]) / 2;
		for(i = 0; i <= n2; i ++)
		{
			s = i - delay;
			h[i] =  ((sin(wc2 * s) - sin(wc1 * s)) / (pi * s));
			h[i] =  (h[i] * WindowFunction(n + 1, i));
			h[n-i] = h[i];
		}
		if(mid == 1)
			h[n/2]= ((wc2 - wc1) / pi);
		break;
	case 4: 
		wc1 = 2 * pi * (m_nFilterParameter.Frequency_Pass[0] + m_nFilterParameter.Frequency_Stop[0]) / 2;
		wc2 = 2 * pi * (m_nFilterParameter.Frequency_Pass[1] + m_nFilterParameter.Frequency_Stop[1]) / 2;
		for(i = 0; i <= n2; i ++)
		{
			s = i - delay;
			h[i] =  ((sin(wc1 * s) + sin(pi * s) - sin(wc2 * s)) / (pi * s));
			h[i] =  (h[i] * WindowFunction(n+1, i));
			h[n-i] = h[i];
		}
		if(mid == 1)
			h[n / 2] =  ( (wc1 + pi - wc2) / pi);
		break;
	default: 
		break;		
	}
	m_nFilterParameter.Frequency_Cutoff[0] = wc1;
	m_nFilterParameter.Frequency_Cutoff[1] = wc2;
	MultinomialCoefficientDenominator[0] = 1;
	for(i = 1; i < n; i ++)
	{
		MultinomialCoefficientDenominator[i] = 0;
	}
	return false;
}
bool CDigitalFilter::DesignIIRFilter(	tagDigitalFilterParameters &m_nFilterParameter,
										double *MultinomialCoefficientDenominator,//滤波器分母多项式的系数
										double *MultinomialCoefficientNumerator//滤波器分子多项式的系数
									)
{
	int N = m_nFilterParameter.Order;
	double pi = 4 * atan(1.0);
	double OmigaC = m_nFilterParameter.OmigaLowCutOff ;//Ωc

	CComplex s[MAX_ORDER];
	double 	K0;


	int i = 0 ; 
	double BW = 0;
	if (m_nFilterParameter.Filter_Type == 3)
	{
		BW = fabs(m_nFilterParameter.Frequency_Pass[0] - m_nFilterParameter.Frequency_Pass[1]);
	}
	if (m_nFilterParameter.Filter_Type == 4)
	{
		BW = fabs(m_nFilterParameter.Frequency_Stop[0] - m_nFilterParameter.Frequency_Stop[1]);
	}



	
	//H(s) = OmigaC^N  / ((s - s[1]) * (s - s[2]) * ... * (s - s[N]))
	for (int k = 1; k <= N; k ++)
	{
		s[k].r = OmigaC * cos((1.0 / 2 + (2 * k - 1) / (2.0*N)) * pi);
		s[k].i = OmigaC * sin((1.0 / 2 + (2 * k - 1) / (2.0*N)) * pi);
	}		
	//模拟传递函数Ha(s)分式的分子
	K0=pow(OmigaC,N);
	//计算两共轭极点所对应部份分式的分母, 即(s-sk)×(s-sk*)
	double c[MAX_ORDER][6];//二次分式的系数. 从分子到分母、从左到右依次为c[0]、c[1]、c[2]、…、c[5]
	double c4[MAX_ORDER][10];//四次分式的系数。从分子到分母、从左到右依次为c[0]、c[1]、... 、c[9]
	//(c[k][0] * s^2 + c[k][1] * s^1 + c[k][2] * s^0) / (c[k][3] * s^2 + c[k][4] * s^1 + c[k][5] * s^0)
	for (k=1; k<=N/2; k++)
	{
		c[k][0] = 0;
		c[k][1] = 0;
		c[k][2] = OmigaC * OmigaC;
		c[k][3] = 1; 
		c[k][4] = -2 * s[k].r;//[4]代表分母的第二项
		c[k][5] = s[k].r * s[k].r + s[k].i * s[k].i;//[5]代表分母的最后一项
	}


	double tempC[10];
	switch(m_nFilterParameter.Filter_Type)
	{
	case 1:
		//计算模拟滤波器的极点s[k](=a+bi)
		break;
	case 2:
		/*
		SL = OmigaLow * OmigaHigh / SH
		H(SL) = (A1 * SL^2 + A2 * SL^1 + A3) / (B1 * SL^2 +B2 * SL^1 + B3)
		H(SH) = (a1 * SH^2 + a2 * SH^1 + a3) / (b1 * SH^2 +b2 * SH^1 + b3) 
		a1 = A3
		a2 = A2 * OmigaLow * OmigaHigh
		a3 = A1 * (OmigaLow * OmigaHigh)^2
		b1 = B3
		b2 = B2 * (OmigaLow * OmigaHigh)
		b3 = B1 * (OmigaLow * OmigaHigh)^2
		*/
		for(k=1; k<=N/2; k++)
		{
			tempC[0] = c[k][2];
			tempC[1] = c[k][1] * (m_nFilterParameter.OmigaLowPass * m_nFilterParameter.Frequency_Pass[1]);
			tempC[2] = c[k][0] * pow(m_nFilterParameter.OmigaLowPass * m_nFilterParameter.Frequency_Pass[1], 2);
			tempC[3] = c[k][5];
			tempC[4] = c[k][4] * (m_nFilterParameter.OmigaLowPass * m_nFilterParameter.Frequency_Pass[1]);
			tempC[5] = c[k][3] * pow(m_nFilterParameter.OmigaLowPass * m_nFilterParameter.Frequency_Pass[1], 2);
			memcpy(c[k], tempC, 6 * sizeof(double));
		}
		break;
	case 3:
		/*
		 *	SL = OmigaP * (SB^2 + Omiga0^2) / (SB * BW)
		 *  H(SL) = OmigaC * OmigaC / (SL^2 + B1 * SL + B2) = OmigaC * OmigaC / (SL - p)(SL - p*)
		 *	H(SB) = (a0 * SB^4 + a1 * SB^3 + a2 * SB^2 + a3 * SB^1 + a4) /
		 *			(b0 * SB^4 + b1 * SB^3 + b2 * SB^2 + b3 * SB^1 + b4)
		 *	OmigaP = 1
		 *	a0 = a1 = a3 = a4 = 0
		 *	a2 = OmigaC^2 * BW^2
		 *	b0 = 1
		 *	b1 = - 2 * BW * p.r
		 *	b2 = 2 * Omiga0^2 + BW^2 * |p|^2
		 *	b3 = - 2 * BW * Omiga0^2 * p.r
		 *	b4 = Omiga0^4
		 */
		for(k=1; k<=N/2; k++)
		{
			c4[k][0] = 0;
			c4[k][1] = 0;
			c4[k][2] = OmigaC * OmigaC * BW * BW;
			c4[k][3] = 0;
			c4[k][4] = 0;
			c4[k][5] = 1;
			c4[k][6] = - 2 * BW * s[k].r;
			c4[k][7] = 2 * m_nFilterParameter.Frequency_Pass[0] * m_nFilterParameter.Frequency_Pass[1] +
						BW * BW * (s[k].r * s[k].r + s[k].i * s[k].i);
			c4[k][8] = - 2 * BW * s[k].r * 
						m_nFilterParameter.Frequency_Pass[0] * m_nFilterParameter.Frequency_Pass[1];
			c4[k][9] = pow(m_nFilterParameter.Frequency_Pass[0] * m_nFilterParameter.Frequency_Pass[1],2);

		}
		break;
	case 4:
		/*
		 *	SL = OmigaS * (ST * (OmigaS2 - OmigaS1)) / (ST^2 + OmigaS1 * OmigaS2)
		 *  H(SL) = OmigaC * OmigaC / (SL^2 + B1 * SL + B2) = OmigaC * OmigaC / (SL - p)(SL - p*)
		 *	H(ST) = (a0 * ST^4 + a1 * ST^3 + a2 * ST^2 + a3 * ST^1 + a4) /
		 *			(b0 * ST^4 + b1 * ST^3 + b2 * ST^2 + b3 * ST^1 + b4)
		 *  OmigaS = 1
		 *	OmigaC^2 = |p|^2
		 *	a0 = OmigaC^2
		 *	a1 = a3 = 0
		 *	a2 = 2 * OmigaC^2 * Omiga0^2
		 *	a4 = OmigaC^2 * Omiga0^4
		 *	b0 = |p|^2
		 *	b1 = - 2 * p.r * BW
		 *	b2 = |p|^2 * 2 * Omiga0^2 + BW^2
		 *	b3 = - 2 * p.r * BW * Omiga0^2
		 *	b4 = |p|^2 * Omiga0^4
		 */
		for(k = 1; k <= N/2; k ++)
		{
			c4[k][0] =	OmigaC * OmigaC;
			c4[k][1] =	0;
			c4[k][2] =	2 * OmigaC * OmigaC *
						m_nFilterParameter.Frequency_Stop[0] * m_nFilterParameter.Frequency_Stop[1] ;
			c4[k][3] =	0;
			c4[k][4] =	OmigaC * OmigaC 
						* m_nFilterParameter.Frequency_Stop[0] * m_nFilterParameter.Frequency_Stop[1]
						* m_nFilterParameter.Frequency_Stop[0] * m_nFilterParameter.Frequency_Stop[1];
			c4[k][5] =  s[k].r * s[k].r + s[k].i * s[k].i;
			c4[k][6] =	- 2 * s[k].r * BW;
			c4[k][7] =	(s[k].r * s[k].r + s[k].i * s[k].i)
						* 2 * m_nFilterParameter.Frequency_Stop[0] * m_nFilterParameter.Frequency_Stop[1]
						+ BW * BW;
			c4[k][8] =	- 2 * s[k].r * BW *
						m_nFilterParameter.Frequency_Stop[0] * m_nFilterParameter.Frequency_Stop[1];
			c4[k][9] =	(s[k].r * s[k].r + s[k].i * s[k].i) 			
						* m_nFilterParameter.Frequency_Stop[0] * m_nFilterParameter.Frequency_Stop[1]
						* m_nFilterParameter.Frequency_Stop[0] * m_nFilterParameter.Frequency_Stop[1];
			for(i = 0 ; i < 10; i ++)
			{
				c4[k][i] = c4[k][i]  / (OmigaC * OmigaC );
			}
		}
		break;
	default:
		break;
	}
	
	if(m_nFilterParameter.Filter_Type == 1 || m_nFilterParameter.Filter_Type == 2) 	
	{
		/*******s = (z -1) / (z + 1)
		********(A1 * s^2 + A2 * s^1 + A3) / (B1 * s^2 + B2 * s^1 + B3)
		********(a1 * z^2 + a2 * z^1 + a3) / (b1 * z^2 + b2 * z^1 + b3)
		********a1 = A1 + A2 + A3
		********a2 = - 2 * A1 + 2 * A3
		********a3 = A1 - A2 + A3
		********b1 = B1 + B2 + B3
		********b2 = - 2 * B1 + 2 * B3
		********b3 = B1 - B2 + B3
		********/
		double fenzi[MAX_ORDER][3];
		double fenmu[MAX_ORDER][3];
		for(k = 1; k <= N/2; k ++)
		{
			fenzi[k][0] = c[k][0] + c[k][1] + c[k][2];
			fenzi[k][1] = - 2 * c[k][0] + 2 * c[k][2];
			fenzi[k][2] = c[k][0] - c[k][1] + c[k][2];
			fenmu[k][0] = c[k][3] + c[k][5] + c[k][4];
			fenmu[k][1] = - 2 * c[k][3] + 2 * c[k][5];
			fenmu[k][2] = c[k][3] - c[k][4] + c[k][5];
		}
		
		int Nfenzi, Nfenmu;//分子分母多项式的项数
		
		double polytemp2[MAX_ORDER];
		int  ntemp2;
		//H(z)
		polyMultiplyNumber(fenmu[1], 2, 1, MultinomialCoefficientDenominator, &Nfenmu);//累乘各个分母累, 得总的分母
		for (k = 2; k <= N / 2; k ++)
		{
			polyMultiplyPoly(MultinomialCoefficientDenominator, Nfenmu, fenmu[k],2, polytemp2, &ntemp2);//累乘一个分母
			polyMultiplyNumber(polytemp2, ntemp2, 1, MultinomialCoefficientDenominator, &Nfenmu);//结果转回给fenmuSum
		}

		
		polyMultiplyNumber(fenzi[1], 2, 1, MultinomialCoefficientNumerator, &Nfenzi);//累乘各个分子累, 得总的分子
		for (k=2; k<=N/2; k++)
		{
			polyMultiplyPoly(MultinomialCoefficientNumerator, Nfenzi, fenzi[k], 2, polytemp2, &ntemp2);//累乘一个分子
			polyMultiplyNumber(polytemp2, ntemp2, 1, MultinomialCoefficientNumerator, &Nfenzi);//结果转回给fenziSum
		}
	}
	if(m_nFilterParameter.Filter_Type == 3 || m_nFilterParameter.Filter_Type == 4 )
	{
		/*******s = (z -1) / (z + 1)
		********(A1 * s^4 + A2 * s^3 + A3 * s^2 + A4 * s + A5) / (B1 * s^4 + B2 * s^3 + B3 * s^2 + B4 * s^1 + B5)
		********(a1 * z^4 + a2 * z^3 + a3 * z^2 + a4 * z + a5) / (b1 * z^4 + b2 * z^3 + b3 * z^2 + b4 * z + b5)
		********a1 = A1 + A2 + A3 + A4 + A5
		********a2 = - 4 * A1 - 2 * A2 - 0 + 2 * A4 + 4 * A5
		********a3 = 6 * A1 + 0 - 2 * A3 + 0 + 6 * A5
		********a4 = - 4 * A1 + 2 * A2 + 0 - 2 * A4 + 4 * A5 
		********a5 = A1 - A2 + A3 - A4 + A5
		********/
		double fenzi[MAX_ORDER][5];
		double fenmu[MAX_ORDER][5];
		for(k = 1; k <= N/2; k ++)
		{
			fenzi[k][0] = c4[k][0] + c4[k][1] + c4[k][2] + c4[k][3] + c4[k][4];
			fenzi[k][1] = - 4 * c4[k][0] - 2 * c4[k][1] + 2 * c4[k][3] + 4 * c4[k][4];
			fenzi[k][2] = 6 * c4[k][0] -2 * c4[k][2] + 6 * c4[k][4];
			fenzi[k][3] = - 4 * c4[k][0] + 2 * c[k][1] - 2 * c4[k][3] + 4 * c4[k][4];
			fenzi[k][4] = c4[k][0] - c4[k][1] + c4[k][2] - c4[k][3] + c4[k][4];
			fenmu[k][0] = c4[k][5] + c4[k][6] + c4[k][7] + c4[k][8] + c4[k][9];
			fenmu[k][1] = - 4 * c4[k][5] - 2 * c4[k][6] + 2 * c4[k][8] + 4 * c4[k][9];
			fenmu[k][2] = 6 * c4[k][5] -2 * c4[k][7] + 6 * c4[k][9];
			fenmu[k][3] = - 4 * c4[k][5] + 2 * c4[k][6] - 2 * c4[k][8] + 4 * c4[k][9];
			fenmu[k][4] = c4[k][5] - c4[k][6] + c4[k][7] - c4[k][8] + c4[k][9];
		}
		
		int Nfenzi, Nfenmu;//分子分母多项式的项数
		
		double polytemp2[MAX_ORDER];
		int  ntemp2;
		//H(z)
		polyMultiplyNumber(fenmu[1], 4, 1, MultinomialCoefficientDenominator, &Nfenmu);//累乘各个分母累, 得总的分母
		for (k = 2; k <= N / 2; k ++)
		{
			polyMultiplyPoly(MultinomialCoefficientDenominator, Nfenmu, fenmu[k],4, polytemp2, &ntemp2);//累乘一个分母
			polyMultiplyNumber(polytemp2, ntemp2, 1, MultinomialCoefficientDenominator, &Nfenmu);//结果转回给fenmuSum
		}
		
		
		polyMultiplyNumber(fenzi[1], 4, 1, MultinomialCoefficientNumerator, &Nfenzi);//累乘各个分子累, 得总的分子
		for (k=2; k<=N/2; k++)
		{
			polyMultiplyPoly(MultinomialCoefficientNumerator, Nfenzi, fenzi[k], 4, polytemp2, &ntemp2);//累乘一个分子
			polyMultiplyNumber(polytemp2, ntemp2, 1, MultinomialCoefficientNumerator, &Nfenzi);//结果转回给fenziSum
		}
		N *= 2;
		
	}
	m_nFilterParameter.Order = N;
	return true;
}

double CDigitalFilter::WindowFunction(int n,int i)
{
	double pi = 4.0 * atan(1.0);

	double w = 0.54 - 0.46 * cos( 2 * i * pi / (n-1));

	return (w);
}

void CDigitalFilter::Convolution_FormulaMethod(double *x, 
											   double *numerator,
											   double *denominator,
											   long hLength,
											   long xLength,
											   double *y)
{
	if(hLength > xLength)
	{
		AfxMessageBox("the filter length is longer than the signal!");
		return; 
	}
	for(long i = 0; i < xLength; i ++)
	{
		y[i] = 0;
	}
	//y[i] * denominator[0] + y[i - 1] * denominator[1] + ... + y[i - hLength] * denominator[hLength];
	//x[i] * numerator[0] + x[i - 1] * numerator[i] + ... + x[i - hLength] * numerator[hLength];
	for(i = hLength; i < xLength/* + hLength*/; i ++)
	{
		double SumDenominator = 0;
		double SumNumerator = 0;
		for (int j = 0; j <= hLength; j ++)
		{
			SumNumerator += numerator[j] * x[i - j];
		}
		for (j = 1; j <= hLength; j ++)
		{
			SumDenominator +=  denominator[j] * y[i - j];
		}
		y[i] = 1 / denominator[0] * (SumNumerator - SumDenominator);
	}

	return;
}
void CDigitalFilter::Convolution_FFTMethod(double *x,double *h,double *y,long xLength,long hLength)
{

	//x
	CComplex*  m_xTdata;
	CComplex*  m_xFdata;


	int nLevel = (int)(ceil(log10(xLength)/log10(2)));
	long newlen=(long)pow(2,nLevel);
	m_xTdata=new CComplex[newlen];
	m_xFdata=new CComplex[newlen];
	for(long i=0;i<newlen;i++)
	{
		if(i<xLength)
			m_xTdata[i].r=x[i];
		else
			m_xTdata[i].r=0;
		m_xTdata[i].i=0;
		m_xFdata[i].r=0;
		m_xFdata[i].i=0;
	}
	FFT_1D(m_xTdata,m_xFdata,nLevel);


	
	//h
	CComplex*  m_hTdata;
	CComplex*  m_hFdata;
	
	m_hTdata=new CComplex[newlen];
	m_hFdata=new CComplex[newlen];
	for(i=0;i<newlen;i++)
	{
		if(i<hLength)
			m_hTdata[i].r=h[i];
		else
			m_hTdata[i].r=0;
		m_hTdata[i].i=0;
		m_hFdata[i].r=0;
		m_hFdata[i].i=0;
	}
	FFT_1D(m_hTdata,m_hFdata,nLevel);

	//x * h
	CComplex*  m_yTdata;
	CComplex*  m_yFdata;
	m_yTdata=new CComplex[newlen];
	m_yFdata=new CComplex[newlen];
	for (i = 0; i < newlen; i ++)
		m_yFdata[i] = m_xFdata[i] * m_hFdata[i];

	//yt
	IFFT_1D(m_yFdata,m_yTdata,nLevel);
	//xt = inverse yt
	for(i = 0; i < newlen; i ++)
	{
		m_xTdata[i] = m_yTdata[i];
	}
	//xf
	FFT_1D(m_xTdata,m_xFdata,nLevel);

	//yf = xf * hf
	for(i = 0 ; i < newlen; i ++)
		m_yFdata[i] = m_xFdata[i] * m_hFdata[i];




	//yt
	IFFT_1D(m_yFdata,m_yTdata,nLevel);


	//y	//inverse y
	for (i = 0; i < newlen; i ++)
	{
		double temp = sqrt(m_yTdata[i].r * m_yTdata[i].r + m_yTdata[i].i * m_yTdata[i].i);
		y[i] = temp * (m_yTdata[i].r > 0 ? 1 : -1); 
	}


	//delete
	delete m_yTdata;
	delete m_yFdata;
	delete m_xTdata;
	delete m_xFdata;
	delete m_hTdata;
	delete m_hFdata;
}
void CDigitalFilter::Convolution(tagDigitalFilterParameters MyselfFilter,
								double *MultinomialCoefficientDenominator,
								double *MultinomialCoefficientNumerator,
								int dataLenght,
								double *m_dTransformedSignal,
								double *m_dOriginSignal)
{
	if(MyselfFilter.Filter_Mode == 1 && MyselfFilter.Filter_Class == 1)
	{
		Convolution_FFTMethod(	m_dOriginSignal,
								MultinomialCoefficientNumerator,
								m_dTransformedSignal,
								dataLenght,
								MyselfFilter.Order);
	}
	else
	{
	Convolution_FormulaMethod(m_dOriginSignal,
								MultinomialCoefficientNumerator,
								MultinomialCoefficientDenominator,
								MyselfFilter.Order,dataLenght,m_dTransformedSignal);
	}
	return;
}
void CDigitalFilter::FFT_1D(CComplex * pCTData, CComplex * pCFData, int nLevel)
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

	double pi = 4 * atan(1.0);
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
void CDigitalFilter::IFFT_1D(CComplex * pCFData, CComplex * pCTData, int nLevel)
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

int CDigitalFilter::GetFilterOrder(const tagInputParameter Input,tagDigitalFilterParameters &MyFilter)
{
	int Order;
	if(Input.Filter_Class == 1)//FIR
	{
		Order = GetFIRFilterOrder(Input,MyFilter);
	}
	if(Input.Filter_Class == 2)//IIR
	{
		Order = GetIIRFilterOrder(Input,MyFilter);
	}
	return Order;
}


int CDigitalFilter::GetFIRFilterOrder(const tagInputParameter Input,tagDigitalFilterParameters &MyFilter)
{
	int Order = 0;
	double pi = 4 * atan(1);
	
	double dalteW1,dalteW2;
	double dalteW;//过渡带的宽度
	TRY
	{
		switch(Input.Filter_Type) {
		case 1: //LowPass
			dalteW =2 * pi * fabs(MyFilter.Frequency_Stop[0] - MyFilter.Frequency_Pass[0]);
			break;
		case 2: //HighPass  
			dalteW =2 * pi * fabs(MyFilter.Frequency_Pass[1] - MyFilter.Frequency_Stop[1]);
			break;
		case 3: //BandPass
		case 4://StopPass
			dalteW1 =2 * pi * fabs(MyFilter.Frequency_Stop[0] - MyFilter.Frequency_Pass[0]);
			dalteW2 =2 * pi * fabs(MyFilter.Frequency_Stop[1] - MyFilter.Frequency_Pass[1]);
			dalteW =2 *  dalteW1 < dalteW2 ? dalteW1 : dalteW2  ;
			break;
		default:
			break;
		}
		Order = (int)((3.32 * pi) / dalteW * 2 + 1);
	}
	CATCH(CMemoryException, pEx)
	{
		
		AfxAbort();
	}
	END_CATCH
	return Order;
}
/*采用巴特沃茨滤波器，通带增益小于1dB，阻带增益大于60dB*/
int CDigitalFilter::GetIIRFilterOrder(const tagInputParameter Input,tagDigitalFilterParameters &MyFilter)
{
	int Order = 0;
	double pi = 4 * atan(1);
	double OmigaP;
	double OmigaS;
	double OmigaP0,OmigaP1;
	double OmigaS0,OmigaS1;
	OmigaP0 = tan(MyFilter.Frequency_Pass[0] * pi);
	OmigaS0 = tan(MyFilter.Frequency_Stop[0] * pi);
	OmigaP1 = tan(MyFilter.Frequency_Pass[1] * pi);
	OmigaS1 = tan(MyFilter.Frequency_Stop[1] * pi);

	if(OmigaP0 < 0 || OmigaS0 < 0 || OmigaS1 < 0 || OmigaP1 < 0)
	{
		AfxMessageBox("你输入的参数太大，IIR中的频率不能超过采样频率的一半！");
		return -1;
	}

	double OmigaTemp = 0;


	double k;
	double e = sqrt(1 / pow(10,- PassGain / 10.0) - 1);
	double A = sqrt(1 / pow(10,- StopGain / 10.0));
	double k1 = e / sqrt(A * A -1);
	switch(MyFilter.Filter_Type) 
	{
	case 1:
		OmigaP = OmigaP0;
		OmigaS = OmigaS0;
		break;
	case 2:
		OmigaP = 1;
		OmigaS = OmigaP * OmigaP1 / OmigaS1;
		break;
	case 3:
		OmigaP = 1;
		OmigaTemp = (OmigaP0 * OmigaP1 / OmigaS0);
		OmigaS = OmigaP * (OmigaP0 * OmigaP1 -  OmigaTemp * OmigaTemp) / (OmigaS1 * fabs(OmigaP0 - OmigaP1));
		break;
	case 4:
		OmigaS = 1;
		OmigaTemp = OmigaS1 * OmigaS0 / OmigaP1 ;
		OmigaP = OmigaP0 * fabs(OmigaS0 - OmigaS1) / (OmigaS1 * OmigaS0 - OmigaTemp * OmigaTemp);
		break;
	default:
		break;
	}
	k = OmigaP / OmigaS;
	Order = (int)ceil(log10(1 / k1)/log10(1 / k));

	Order = Order + Order%2;
	

	MyFilter.Frequency_Pass[0] = OmigaP0;
	MyFilter.Frequency_Stop[0] = OmigaS0;

	MyFilter.Frequency_Stop[1] = OmigaS1;
	MyFilter.Frequency_Pass[1] = OmigaP1;

	MyFilter.OmigaLowPass = OmigaP;
	MyFilter.OmigaLowStop = OmigaS;
	MyFilter.OmigaLowCutOff = OmigaP / pow(e, 1.0 / Order);
	

	return Order;;
}
bool CDigitalFilter::DesignFilter(	tagDigitalFilterParameters &MyselfFilter,
				  double *MultinomialCoefficientDenominator,
				  double *MultinomialCoefficientNumerator)
{

	switch(MyselfFilter.Filter_Class) 
	{
	case 1:
		DesignFIRFilter(	MyselfFilter,
							MultinomialCoefficientDenominator,
							MultinomialCoefficientNumerator);
		break;
	case 2:
		DesignIIRFilter(	MyselfFilter,
							MultinomialCoefficientDenominator,
							MultinomialCoefficientNumerator);
		break;
	default:
		break;
	}
	return true;
}
bool CDigitalFilter::DisplayFunction(	CWnd * pFunctionWnd, 
					 tagDigitalFilterParameters MyselfFilter,
					 double * MultinomialCoefficientDenominator,
					 double * MultinomialCoefficientNumerator)
{
	CRect FunctionRect;
	pFunctionWnd->GetWindowRect(&FunctionRect);
	CSize cs;
	cs.cx = FunctionRect.right - FunctionRect.left;
	cs.cy = FunctionRect.bottom - FunctionRect.top;
	CPaintDC pDC(pFunctionWnd);
	pDC.TextOut (500, 0,"Function");
	
	CPoint OriginalPoint(30, cs.cy - 15);
	//x
	CPoint MaxXPoint(cs.cx - OriginalPoint.x, OriginalPoint.y);
	pDC.MoveTo(OriginalPoint);
	pDC.LineTo(MaxXPoint);
	pDC.LineTo(MaxXPoint.x - 5, MaxXPoint.y + 5);
	pDC.MoveTo(MaxXPoint);
	pDC.LineTo(MaxXPoint.x - 5, MaxXPoint.y - 5);
	int xLength = (MaxXPoint.x - OriginalPoint.x) / 5;
	CString str;
	for(int i = 0; i <= 5; i ++)
	{
		pDC.MoveTo(OriginalPoint.x + xLength * i, OriginalPoint.y);
		pDC.LineTo(OriginalPoint.x + xLength * i, OriginalPoint.y -5);
		str.Format("%1.0fHz", i * 0.1 * MyselfFilter.SampleFrequency);
		pDC.TextOut(OriginalPoint.x - 15 + xLength * i, OriginalPoint.y + 1, str);
	}
	//y
	CPoint MaxYPoint(OriginalPoint.x, cs.cy - OriginalPoint.y);
	pDC.MoveTo(OriginalPoint);
	pDC.LineTo(MaxYPoint);
	pDC.LineTo(MaxYPoint.x - 5, MaxYPoint.y + 5);
	pDC.MoveTo(MaxYPoint);
	pDC.LineTo(MaxYPoint.x + 5, MaxYPoint.y + 5);
	int yLength = (- MaxYPoint.y + OriginalPoint.y) / 5;
	for(i = 0 ; i <= 5; i ++)
	{
		pDC.MoveTo(OriginalPoint.x, MaxYPoint.y + yLength * i);
		pDC.MoveTo(OriginalPoint.x + 5, MaxYPoint.y + yLength * i);
		str.Format("-%ddB", i * 20);
		pDC.TextOut(OriginalPoint.x - 30, MaxYPoint.y + yLength * i, str);
	}
	
	
	//将H(z)用z=exp(jw)化成H(jw),以便画频率响应图
	double w, moduleHw;
	CComplex z1, z2, Hw;
	double ratioX=0, ratioY=0;
	
	int N = MyselfFilter.Order;
	
	double PI = 4 * atan(1.0);
	for (w=0; w<=PI; w+=0.001 * PI)
	{
		z1.r=0;		z1.i=0;//H(jw)的分子
		for (int k = 0; k <= N; k ++)//将分子按exp(-jw)=cos(w)-jsin(w)展开后, 进行实部和虚部的合并
		{
			z1.r += MultinomialCoefficientNumerator[k]*cos(k*w);
			z1.i += -MultinomialCoefficientNumerator[k]*sin(k*w);
		}
		
		z2.r=0;		z2.i=0;//H(jw)的分母
		for (k = 0; k <= N; k ++)//将分母按exp(-jw)=cos(w)-jsin(w)展开后, 进行实部和虚部的合并
		{
			z2.r+= MultinomialCoefficientDenominator[k]*cos(k*w);
			z2.i+=-MultinomialCoefficientDenominator[k]*sin(k*w);
		}
		
		Hw = Hw.complexDivide(z1,z2);//分子除分母,得H(jw)
		
		moduleHw=sqrt(Hw.complexModule2(Hw));//求|H(jw)|
		
		moduleHw = 20 * log10(moduleHw);
		if(moduleHw < -100)moduleHw = -100;
		moduleHw = moduleHw / 100 + 1;
		//画幅度曲线|H(jw)|=f(w)
		pDC.SetPixel( (int)(w / PI * (cs.cx - OriginalPoint.x * 2) + OriginalPoint.x), 
			(int)(OriginalPoint.y - moduleHw * (- cs.cy + OriginalPoint.y * 2)),
			RGB(255,0,0));
	}
	return false;
	
}


//使用双线性变换法，将s变成z    
//when T = 2 ; then z =(1 + s) / (1 - s) ; s = (z - 1) / (z + 1)

// s - sa = (z - 1) / (z + 1) - sa =( z - 1 - sa * z - sa ) / (z + 1) 
//=(1 - sa) * {z - (sa + 1) / (1 - sa)} / (z + 1)
//Ha(s) = sb / (s - sa)
//H(z) = {sb / (1 - sa)}  * (z + 1) /  * {z - (sa + 1) / (1 - sa)}
//zb = sb / (1 - sa)
//za = (sa + 1) / (1 - sa)
bool CDigitalFilter::DLineTransform(CComplex *sa, double sb, CComplex *za, CComplex *zb,int Order)
{

	CComplex temp1;
	CComplex temp2;
	CComplex R(1.0, 0);
	for(int i = 0; i < Order; i ++)
	{
		zb[i].complexDivide( CComplex(sb,0), temp1.complexSub(R, sa[i]));
		za[i].complexDivide(temp1.complexAdd(sa[i], R),temp2.complexSub(R,sa[i]));
	}
	return true;
}


//ploy

// ploy + ploy
void CDigitalFilter::polyAdd(double* a, int m, double* b, int n, double* p, int* np)
{
	//a、b代表两个多项式序列的系数, m、n分别是它们的自变量最高/最低次数, 它们比多项式的项数少1
	//p代表乘积多项式的系数, N是其自变量最高/最低次数, 它们比多项式的项数少1
	
	int k;
	
	*np=max(m,n);//和的长度等于长序列的长度
	
	if (m>n)//如果a序列更长
	{
		for (k=0; k<=n; k++)
			*(p+k)=*(a+k) + *(b+k);//前半截取a、b序列之和
		for (k=n+1; k<=*np; k++)
			*(p+k)=*(a+k);//后半截取a序列
	}
	else
	{
		for (k=0; k<=m; k++)
			*(p+k)=*(a+k) + *(b+k);//前半截取a、b序列之和
		for (k=m+1; k<=*np; k++)
			*(p+k)=*(b+k);//后半截取b序列
	}
}


//ploy * const
void CDigitalFilter::polyMultiplyNumber(double* a, int m, double c, double* p, int* np)
{
	for (int k=0; k<=m; k++)
		*(p+k)=*(a+k) * c;//各项都乘以系数c
	*np=m;//项数不变
}


//ploy * ploy
void CDigitalFilter::polyMultiplyPoly(double* a, int m, double* b, int n, double* p, int* np)
{
	int k;
	double aa[100], bb[100];
	
	*np=m+n;//积的长度等于两序列长度之和
	
	for (k=0; k<=*np; k++)//分别延长序列a、b成aa、bb, 使其长度都达到m+n, 延长部份补0
	{aa[k]=0;  bb[k]=0;}
	for (k=0; k<=m; k++)//将序列a复制进aa的前半截
		aa[k]=*(a+k);
	for (k=0; k<=n; k++)//将序列b复制进bb的前半截
		bb[k]=*(b+k);
	
	for (k=0;k<=*np;k++)//计算各次项的系数
	{
		double sum = 0;
		for (int i=0;i<=k;i++)//第k次项的系数由k+1个a[i]*b[k-i]相加而成
			sum=sum + aa[i] * bb[k-i];
		*(p+k)=sum ;
	}
}

//ploy * ploy * ... * ploy 
void CDigitalFilter::polyPower(double* a, int m, int k, double* p, int* np)
{
	int i=1, N1;
	double p1[100];
	
	if (k<=0)
	{ AfxMessageBox("多项式的幂不可求, 因为幂指数k<=0 !"); exit(0);}
	
	polyMultiplyNumber(a, m, 1, p1, &N1);//p1=a
	
	do//循环累乘a(共乘k-1个a)
	{
		polyMultiplyNumber(p1, N1, 1, p, np);//p1→p
		
		i++;
		if (i>k) break;
		
		polyMultiplyPoly(p, *np, a, m, p1, &N1);//p1=p*a
	}while(1);
}































































