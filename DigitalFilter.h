// DigitalFilter.h: interface for the CDigitalFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIGITALFILTER_H__F237D5D4_66CE_4D9E_8E28_9BFA30FCCCE2__INCLUDED_)
#define AFX_DIGITALFILTER_H__F237D5D4_66CE_4D9E_8E28_9BFA30FCCCE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Complex.h"
#include <math.h>


#define MAX_ORDER	1000				//滤波器最大的阶数
#define PassGain	1					//通带截止频率的增益
#define StopGain    60					//阻带截止频率的增益


/*
Octave mean that f1/f2 = 2
X dB/Oct mean that Gain_f1/Gain_f2 = X
例如二阶分频器的截止带衰减斜率为-6dB/Oct,
 表示在曲线的截止带，频率每变化一倍时，电平衰减6个dB
*/

typedef struct tagInputParameter
{
	int Filter_Class;			//滤波器的类型(FIR/IIR)
	int Filter_Mode;			//FIR的模式(Zero Phase shift or analysis simulate)
	int Filter_Type;			//滤波器的类型(LowPass/HighPass/BandPass/BandStop)
	int SampleFrequency;			//采样频率
	int LowPass_Frequency_CutOff_Slope;	//低通滤波器的过渡带的衰减斜率
	int LowPass_Frequency_CutOff;		//低通滤波器的通带截止频率
	int HighPass_Frequency_CutOff_Slope;	//高通滤波器的过渡带的衰减斜率
	int HighPass_Frequency_CutOff;		//高通滤波器的通带截止频率
	int Start_Frequency_Slope;		//带阻滤波器的左边带过渡带的衰减斜率
	int Start_Frequency;			//带阻滤波器的左边带的通带截止频率
	int Stop_Frequency_Slope;		//带阻滤波器的右边带过渡带的衰减斜率
	int Stop_Frequency;			//带阻滤波器的右边带的通带截止频率
}InputParameter;

typedef struct tagDigitalFilterParameters
{
	int Filter_Class;			//滤波器的类型(FIR/IIR)
	int Filter_Type;			//滤波器的类型(LowPass/HighPass/BandPass/BandStop)
	int Filter_Mode;			//FIR的模式(Zero Phase shift or analysis simulate)
	int SampleFrequency;			//采样频率
	double  Frequency_Cutoff[2];		//数字截止频率
	double	Frequency_Pass[2];		//通带截止频率
	double	Frequency_Stop[2];		//阻带截止频率
	double  OmigaLowPass;			//双线性设计时，对应的模拟低通滤波器的通带截止频率
	double  OmigaLowStop;			//双线性设计时，对应的模拟低通滤波器的阻带截止频率
	double  OmigaLowCutOff;			//双线性设计时，对应的模拟低通滤波器的截止频率
	int Order;				//滤波器的阶数	
}DFParameter;

class CDigitalFilter  
{
public:
	//initial
	bool Initialization(tagDigitalFilterParameters &m_nFilterparamter,
						const tagInputParameter Input);
	
	//design filter
	bool DesignFilter(	tagDigitalFilterParameters &MyselfFilter,
						double *MultinomialCoefficientDenominator,
						double *MultinomialCoefficientNumerator);
	bool DesignFIRFilter(	tagDigitalFilterParameters &m_nFilterParameter,
							double *MultinomialCoefficientDenominator,//滤波器分母多项式的系数
							double *MultinomialCoefficientNumerator);//滤波器分子多项式的系数
	bool DesignIIRFilter(	tagDigitalFilterParameters &m_nFilterParameter,
							double *MultinomialCoefficientDenominator,//滤波器分母多项式的系数
							double *MultinomialCoefficientNumerator);//滤波器分子多项式的系数
								

	//input to output
	void Convolution(	tagDigitalFilterParameters MyselfFilter,
						double *MultinomialCoefficientDenominator,
						double *MultinomialCoefficientNumerator,
						int dataLenght,
						double *m_dTransformedSignal,
						double *m_dOriginSignal);
	
	void Convolution_FFTMethod(	double *x,
								double *h,
								double *y,
								long xLength,
								long hLength);

	void Convolution_FormulaMethod(	double *x, 
									double *numerator,
									double *denominator,
									long hLength,
									long xLength,
									double *y);

	//display H(z)
	bool DisplayFunction(	CWnd * pFunctionWnd, 
							tagDigitalFilterParameters MyselfFilter,
							double * MultinomialCoefficientDenominator,
							double * MultinomialCoefficientNumerator);
		
public:
	//FFT and IFFT
	void FFT_1D(CComplex * pCTData, CComplex * pCFData, int nLevel);
	void IFFT_1D(CComplex * pCFData, CComplex * pCTData, int nLevel);


	//ploy
	void polyAdd(double* a, int m, double* b, int n, double* p, int* np);
	void polyMultiplyNumber(double* a, int m, double c, double* p, int* np);
	void polyMultiplyPoly(double* a, int m, double* b, int n, double* p, int* np);
	void polyPower(double* a, int m, int k, double* p, int* np);

	//line transform (used in IIR)
	bool DLineTransform(CComplex *sa, double sb, CComplex *za, CComplex *zb,int Order);

	//Hamming window
	double WindowFunction(int n,int i);

	//get Order
	int GetIIRFilterOrder(const tagInputParameter Input,tagDigitalFilterParameters &MyFilter);
	int GetFIRFilterOrder(const tagInputParameter Input,tagDigitalFilterParameters &MyFilter);
	int GetFilterOrder( const tagInputParameter Input,tagDigitalFilterParameters &MyFilter);

	//constructor
	CDigitalFilter();
	virtual ~CDigitalFilter();
	
};

#endif // !defined(AFX_DIGITALFILTER_H__F237D5D4_66CE_4D9E_8E28_9BFA30FCCCE2__INCLUDED_)










































