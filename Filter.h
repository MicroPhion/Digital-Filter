// Filter.h: interface for the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILTER_H__6F2BB697_8103_4433_8508_9CDA8E3291E6__INCLUDED_)
#define AFX_FILTER_H__6F2BB697_8103_4433_8508_9CDA8E3291E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"Complex.h"
#define pi  3.14159
#include "math.h"
class CFilter  
{
public:

	CFilter();
	virtual ~CFilter();
	void FFT_1D(CComplex * pCTData, CComplex * pCFData, int nLevel);
	void IFFT_1D(CComplex * pCFData, CComplex * pCTData, int nLevel);
	void FFTChange (double *Data,double *FData, long length);
};

#endif // !defined(AFX_FILTER_H__6F2BB697_8103_4433_8508_9CDA8E3291E6__INCLUDED_)
