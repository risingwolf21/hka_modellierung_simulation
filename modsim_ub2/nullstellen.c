#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// f1(x) = x^3 - 2*x + 2

double f1(double x)
{
  return pow(x, 3) - (2 * x) + 2;
}

double df1(double x)
{
}

double f2(double x)
{
}

double df2(double x)
{
}

double Bisektion(double (*f)(double), double xa, double xb, int n)
{
  double m;

  // Kopfzeile für die Ausgabe
  //  [-3, -0.5] [xa, xb]

  // Genauigkeit: 10^-12

  double fx1 = f1(xa);
  double fx2 = f1(xb);

  double x3 = 0.5 * (xa + xb);
  double fx3 = f1(x3);
  printf("  n:  l                 r                  f(l)          f(r)\n");
  printf("  %d:  %0.5lf          %0.5lf           %0.5lf       %0.5f \n", n, xa, xb, fx1, fx2);

  if (fabs(xb - xa) <= pow(10, -12))
  {
    return x3;
  }

  if (fx3 * fx2 <= 0)
  {
    return Bisektion(f1, x3, xb, n + 1);
  }
  else
  {
    return Bisektion(f1, xa, x3, n + 1);
  }
}

double Sekanten(double (*f)(double), double x0, double x1)
{
  // x0 = -3 & x1 = -0.5 & abbruch = 10^-12
  double x_2 = x0 - f1(x0) * ((x1 - x0) / (f1(x1) - f1(x0)));

  if (fabs(x_2 - x1) < pow(10, -12))
  {
    return x_2;
  }

  return Sekanten(f1, x_2, x1);
}

double Newton(double (*f)(double), double (*dfdx)(double), double x0)
{
}

int main()
{
  double result;

  printf("\n\nBisektion\n–––––––––\n");
  result = Bisektion(f1, -3.0, -0.5, 1);
  printf("Nullstelle Bisektion: %.14lf\n", result);

  printf("\n\nSekanten\n––––––––\n");
  result = Sekanten(f1, -3.0, -0.5);
  printf("Nullstelle Sekanten: %.14lf\n", result);

  printf("\n\nNewton\n––––––\n");
  result = Newton(f1, df1, -0.5);
  printf("Nullstelle: %.14lf\n", result);

  return 0;
}
