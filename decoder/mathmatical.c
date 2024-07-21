#include <math.h>
#include "mathmatical.h"

#define EPSILON 1e-7

static bool eq(double x, double y)
{
    return fabs(x - y) < EPSILON;
}

static bool ge(double x, double y)
{
    return fabs(x - y) < EPSILON || x > y;
}

static bool le(double x, double y)
{
    return fabs(x - y) < EPSILON || x < y;
}

// 5.7 Mathematical functions
// (5-1)
double Abs(double x)
{
    return x >= 0 ? x : -x;
}
// (5-2)
double Asin(double x)
{
    return asin(x);
}
// (5-3)
double Atan(double x)
{
    return atan(x);
}
// (5-4)
double Atan2(double y, double x)
{
    if (x > 0)
        return Atan(y / x);
    else if (x < 0 && ge(y, 0))
        return Atan(y / x) + PI;
    else if (x < 0 && y < 0)
        return Atan(y / x) - PI;
    else if (eq(x, 0) && ge(y, 0))
        return PI / 2;
    else
        return -PI / 2;
}
// (5-5)
int Ceil(double x)
{
    return ceil(x);
}
// (5-6)
double Clip1Y(double x);
// (5-7)
double Clip1C(double x);
// (5-8)
double Clip3(double x, double y, double z)
{
    if (z < x)
        return x;
    else if (z > y)
        return y;
    else
        return z;
}
// (5-9)
double Cos(double x)
{
    return cos(x);
}
// (5-10)
double Floor(double x)
{
    return floor(x);
}
// (5-11)
int InverseRasterScan(int a, int b, int c, int d, int e)
{
    if (e == 0)
        return (a % (d / b)) * b;
    else if (e == 1)
        return (a / (d / b)) * c;
    return -1;
}
// (5-12)
double Ln(double x)
{
    return log(x);
}
// (5-13)
double Log2(double x)
{
    return log2(x);
}
// (5-14)
double Log10(double x)
{
    return log10(x);
}
// (5-15)
double Median(double x, double y, double z);
// (5-16)
bool Min(double x, double y)
{
    return x > y ? y : x;
}
// (5-17)
bool Max(double x, double y)
{
    return x < y ? y : x;
}
// (5-18)
double Round(double x)
{
    return Sign(x) * Floor(Abs(x) + 0.5);
}
// (5-19)
double Sign(double x)
{
    return x < 0 ? -1 : 1;
}
// (5-20)
double Sin(double x)
{
    return sin(x);
}
// (5-21)
double Sqrt(double x)
{
    return sqrt(x);
}
// (5-22)
double Tan(double x)
{
    return tan(x);
}