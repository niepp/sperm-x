#ifndef _SNAKE_TYPE_H
#define _SNAKE_TYPE_H

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include <cmath>
#include <list>
#include <queue>
#include <stack>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

const int INF = ~(((int)0x1)<<31);  //正无穷大
const double EPSILON = 1e-6;  // 浮点精度

/*
坐标约定
o-----> x
|
|
\/
y
*/

/*
*	坐标点类型定义
*/
struct skVector;

typedef struct skPoint{
	int dx;
	int dy;
	skPoint();
	skPoint(int x, int y);
	skPoint(const skPoint&);
	skPoint(const skVector&);
	skPoint& operator=(const skPoint&);
	bool operator==( const skPoint& ) const;
	bool operator!=( const skPoint& ) const;
	friend bool operator<(const skPoint& p, const skPoint& q);
}skPoint;

/*
*	矢量类型定义
*/

typedef struct skVector{
	double dx;
	double dy;
	skVector();
	skVector(double x, double y);
	skVector(const skVector&);
	skVector(const skPoint&);
	skVector& operator=(const skVector&);
	bool operator==( const skVector& ) const;
	bool operator!=( const skVector& ) const;
	double GetModule();
	skVector operator+=( const skVector& skv);
	skVector operator-=( const skVector& skv);
	skVector operator*=( const double& val);
	skVector operator/=( const double& val);
}skVector;

skVector operator+( const skVector& left, const skVector& right);
skVector operator-( const skVector& left, const skVector& right);
skVector operator*( const skVector& left, const double& val);
skVector operator*( const double& val, const skVector& right);
skVector operator/( const skVector& numerator, const double& val);
double operator*( const skVector& left, const skVector& right); // 点乘

#endif //_SNAKE_TYPE_H
