// Complex.h: interface for the CComplex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPLEX_H__EABCEA4D_5D7A_47E3_9C4C_5A750139E2F0__INCLUDED_)
#define AFX_COMPLEX_H__EABCEA4D_5D7A_47E3_9C4C_5A750139E2F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CComplex  
{
public:
	CComplex(){	r = i = 0;};
	CComplex(double x, double y){r = x; i = y; };
	CComplex operator+ (const CComplex &y) {return CComplex(r+y.r,i+y.i);}
	CComplex operator- (const CComplex &y) {return CComplex(r-y.r,i-y.i);}
	CComplex operator- () {return CComplex(-r,-i);}
	CComplex operator* (const CComplex &y) {return CComplex(r*y.r-i*y.i,r*y.i+i*y.r);}
	CComplex operator* (const double a) {return CComplex(r*a,i*a);};
	int operator== (const CComplex &y) { return r==y.r && i == y.i;}
	CComplex &operator= (const CComplex &y) {r= y.r;i= y.i; return *this;}
	CComplex operator~() { return CComplex(r,-i);}
	CComplex operator/(double y) { if (y) { r/=y; i/= y; return *this;} else return * this;}
	CComplex &operator/=(double y) { if (y) { r/=y; i/=y;return *this;} else return * this;}

	CComplex complexAdd(CComplex z1, CComplex z2)//复数加法
	{
		CComplex z;
		z.r=z1.r + z2.r;
		z.i=z1.i + z2.i;
		return z;
	}
	
	CComplex complexSub(CComplex z1, CComplex z2)//复数减法
	{
		CComplex z;
		z.r=z1.r - z2.r;
		z.i=z1.i - z2.i;
		return z;
	}
	
	CComplex complexMultiply(CComplex z1, CComplex z2)//复数乘法
	{
		CComplex z;
		z.r=z1.r*z2.r - z1.i*z2.i;
		z.i=z1.r*z2.i + z1.i*z2.r;
		return z;
	}
	
	CComplex complexGonge(CComplex z1)//共轭复数
	{
		CComplex z;
		z.r=  z1.r;
		z.i= -z1.i;
		return z;
	}
	
	double complexModule2(CComplex z)//求复数模的平方
	{
		return (z.r*z.r + z.i*z.i);
	}
	
	CComplex complexDivide(CComplex z1, CComplex z2)//复数除法
	{
		CComplex z=complexMultiply(z1,complexGonge(z2));
		double module=complexModule2(z2);
		
		z.r/=module;
		z.i/=module;
		return z;
	}
	
	CComplex complexInvert(CComplex z1)//复数倒法
	{
		CComplex z=complexGonge(z1);
		double module=complexModule2(z1);
		
		z.r/=module;
		z.i/=module;
		return z;
	}
public:
	double r;
	double i;
};

#endif // !defined(AFX_COMPLEX_H__EABCEA4D_5D7A_47E3_9C4C_5A750139E2F0__INCLUDED_)
