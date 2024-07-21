#ifndef MATHMATICAL_H__
#define MATHMATICAL_H__
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <stdbool.h>

#define PI 3.14159265358979323846

    double Abs(double x);
    double Asin(double x);
    double Atan(double x);
    double Atan2(double y, double x);
    int Ceil(double x);
    double Clip1Y(double x);
    double Clip1C(double x);
    double Clip3(double x, double y, double z);
    double Cos(double x);
    double Floor(double x);
    int InverseRasterScan(int a, int b, int c, int d, int e);
    double Ln(double x);
    double Log2(double x);
    double Log10(double x);
    double Median(double x, double y, double z);
    bool Min(double x, double y);
    bool Max(double x, double y);
    double Round(double x);
    double Sign(double x);
    double Sin(double x);
    double Sqrt(double x);
    double Tan(double x);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MATHMATICAL_H__