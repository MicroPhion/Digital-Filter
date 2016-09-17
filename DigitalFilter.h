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


#define MAX_ORDER	1000				//�˲������Ľ���
#define PassGain	1					//ͨ����ֹƵ�ʵ�����
#define StopGain    60					//�����ֹƵ�ʵ�����


/*
Octave mean that f1/f2 = 2
X dB/Oct mean that Gain_f1/Gain_f2 = X
������׷�Ƶ���Ľ�ֹ��˥��б��Ϊ-6dB/Oct,
 ��ʾ�����ߵĽ�ֹ����Ƶ��ÿ�仯һ��ʱ����ƽ˥��6��dB
*/

typedef struct tagInputParameter
{
	int Filter_Class;			//�˲���������(FIR/IIR)
	int Filter_Mode;			//FIR��ģʽ(Zero Phase shift or analysis simulate)
	int Filter_Type;			//�˲���������(LowPass/HighPass/BandPass/BandStop)
	int SampleFrequency;			//����Ƶ��
	int LowPass_Frequency_CutOff_Slope;	//��ͨ�˲����Ĺ��ɴ���˥��б��
	int LowPass_Frequency_CutOff;		//��ͨ�˲�����ͨ����ֹƵ��
	int HighPass_Frequency_CutOff_Slope;	//��ͨ�˲����Ĺ��ɴ���˥��б��
	int HighPass_Frequency_CutOff;		//��ͨ�˲�����ͨ����ֹƵ��
	int Start_Frequency_Slope;		//�����˲�������ߴ����ɴ���˥��б��
	int Start_Frequency;			//�����˲�������ߴ���ͨ����ֹƵ��
	int Stop_Frequency_Slope;		//�����˲������ұߴ����ɴ���˥��б��
	int Stop_Frequency;			//�����˲������ұߴ���ͨ����ֹƵ��
}InputParameter;

typedef struct tagDigitalFilterParameters
{
	int Filter_Class;			//�˲���������(FIR/IIR)
	int Filter_Type;			//�˲���������(LowPass/HighPass/BandPass/BandStop)
	int Filter_Mode;			//FIR��ģʽ(Zero Phase shift or analysis simulate)
	int SampleFrequency;			//����Ƶ��
	double  Frequency_Cutoff[2];		//���ֽ�ֹƵ��
	double	Frequency_Pass[2];		//ͨ����ֹƵ��
	double	Frequency_Stop[2];		//�����ֹƵ��
	double  OmigaLowPass;			//˫�������ʱ����Ӧ��ģ���ͨ�˲�����ͨ����ֹƵ��
	double  OmigaLowStop;			//˫�������ʱ����Ӧ��ģ���ͨ�˲����������ֹƵ��
	double  OmigaLowCutOff;			//˫�������ʱ����Ӧ��ģ���ͨ�˲����Ľ�ֹƵ��
	int Order;				//�˲����Ľ���	
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
							double *MultinomialCoefficientDenominator,//�˲�����ĸ����ʽ��ϵ��
							double *MultinomialCoefficientNumerator);//�˲������Ӷ���ʽ��ϵ��
	bool DesignIIRFilter(	tagDigitalFilterParameters &m_nFilterParameter,
							double *MultinomialCoefficientDenominator,//�˲�����ĸ����ʽ��ϵ��
							double *MultinomialCoefficientNumerator);//�˲������Ӷ���ʽ��ϵ��
								

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










































