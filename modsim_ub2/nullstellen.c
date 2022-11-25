#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// f1(x) = x^3 - 2*x + 2
// f'(x) = 3x^2 -2

double f1(double x)
{
  return pow(x, 3) - (2 * x) + 2;
}

double df1(double x)
{
  return 3 * pow(x, 2) - 2;
}

// sin(x*PI / 2) = cos(x*PI)*e^(-3*x)
// sin(PI/2) = 1

double f2(double x)
{
  return cos(x * M_PI) * exp((-3 * x)) - sin((x * M_PI) / 2);
}

double df2(double x)
{
  double h = 0.00001;

  return (f2(x + h) - f2(x-h)) / (2*h);
  //return 0.5 * exp(-3 * x) * (2 * M_PI * sin(M_PI * x) + M_PI * exp(3 * x) * cos((M_PI * x) / 2) + 6 * cos(M_PI * x));
}

double Bisektion(double (*f)(double), double xa, double xb, int n)
{
  // Kopfzeile für die Ausgabe
  //  [-3, -0.5] [xa, xb]

  // Genauigkeit: 10^-12

  double fx1 = f(xa);
  double fx2 = f(xb);

  double x3 = 0.5 * (xa + xb);
  double fx3 = f(x3);
  printf("  n:  l                 r                  f(l)          f(r)\n");
  printf("  %d:  %0.5lf          %0.5lf           %0.5lf       %0.5f \n", n, xa, xb, fx1, fx2);

  if (fabs(xb - xa) <= pow(10, -12) || fabs(fx3) <= pow(10,-12))
  {
    return x3;
  }

  if (fx3 * fx2 <= 0)
  {
    return Bisektion(f, x3, xb, n + 1);
  }
  else
  {
    return Bisektion(f, xa, x3, n + 1);
  }
}

double Sekanten(double (*f)(double), double x0, double x1, int n)
{
  // x0 = -3 & x1 = -0.5 & abbruch = 10^-12
  double x2 = x0 - f(x0) * ((x1 - x0) / (f(x1) - f(x0)));

  printf("  n:  l                 r                  f(l)          f(r)\n");
  printf("  %d:  %0.5lf          %0.5lf           %0.5lf       %0.5f \n", n, x0, x1, f(x0), f(x1));

  if (fabs(x2 - x1) < pow(10, -12))
  {
    return x2;
  }

  return Sekanten(f, x1, x2, n + 1);
}

// -1 anstatt -0.5 als startwert liefert ergebnis
// mit startwert -0.5 wird n >= 100
double Newton(double (*f)(double), double (*dfdx)(double), double x0, int n)
{
  if (n >= 100)
  {
    return NAN;
  }
  double x1 = x0 - (f(x0) / dfdx(x0));

  printf("  n:  x0                 x1                  f(x0)          f'(x0)\n");
  printf("  %d:  %0.5lf          %0.5lf           %0.5lf       %0.5f \n", n, x0, x1, f(x0), dfdx(x0));

  if (fabs(x1 - x0) < pow(10, -12))
  {
    return x1;
  }
  else
  {
    return Newton(f, dfdx, x1, n + 1);
  }
}

int main()
{
  double result;

  printf("\n\nBisektion\n–––––––––\n");
  result = Bisektion(f1, -3.0, -0.5, 1);
  printf("Nullstelle Bisektion: %.14lf\n", result);

  printf("\n\nSekanten\n––––––––\n");
  result = Sekanten(f1, -3.0, -0.5, 1);
  printf("Nullstelle Sekanten: %.14lf\n", result);

  printf("\n\nNewton\n––––––\n");
  result = Newton(f1, df1, -0.5, 1);
  printf("Nullstelle: %.14lf\n", result);

  printf("\n\n ----------------------------");

  printf("\n\nBisektionsverfahren\n––––––\n");
  result = Bisektion(f2, 0, 3, 1);
  printf("Nullstelle: %.14lf\n", result);

  printf("\n\nSekanten\n––––––––\n");
  result = Sekanten(f2, 0, 1, 1);
  printf("Nullstelle Sekanten: %.14lf\n", result);

  printf("\n\nNewton\n––––––––\n");
  result = Newton(f2, df2, 0.9, 1);
  printf("Nullstelle Newton: %.14lf\n", result);

  return 0;
}
