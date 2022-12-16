#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief fa(x)=sic(x)
 *
 * @param x x
 *
 * @return f(x)
 */

double f1(double x)
{
  if (x == 0)
    return 1;

  return sin(x) / x;
}

double f2(double x)
{
  return exp(x);
}

/**
 * @brief Integration von f mit Untersummen.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_unter(double a, double b, long n, double (*f)(double))
{
  double result = 0.0;

  double step = (b - a) / n;

  for (int i = 0; i < n; i++)
  {
    result = result + f(a + (step * i));
  }

  return result * ((b - a) / n);
}

/**
 * @brief Integration von f mit Mittelsummen.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_mitte(double a, double b, long n, double (*f)(double))
{
  double result = 0.0;
  double h = (b - a) / n;

  double step = (b - a) / n;

  for (int i = 0; i < n; i++)
  {
    result += f(0.5 * ((a + step * (i + 1)) + (a + step * i)));
  }
  return result * h;
}

/**
 * @brief Integration von f mit Obersummen.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_ober(double a, double b, long n, double (*f)(double))
{
  double result = 0.0;
  double step = (b - a) / n;

  for (int i = 0; i < n; i++)
  {
    result += f(a + step * (i + 1));
  }
  return result * step;
}

/**
 * @brief Integration von f mit der Trapez-Regel.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_trapez(double a, double b, long n, double (*f)(double))
{
  double h = (b - a) / n;
  double result = 0.0;
  double step = (b - a) / n;
  for (int i = 0; i < n; i++)
  {
    result = result + (0.5 * (f(a + step * i) + f(a + step * (i + 1))) * h);
  }
  return result;
}

/**
 * @brief Integration von f mit der Simpson-Regel.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_simpson(double a, double b, long n, double (*f)(double))
{
  double h = (b - a) / n;
  double result = 0.0;
  double step = (b - a) / n;
  for (int i = 0; i < n; i++)
  {
    double x = a + step * i;
    double xi = a + step * (i + 0.5);
    double xii = a + step * (i + 1);
    result += (h / 6.0) * (f(x) + 4 * f(xi) + f(xii));
  }
  return result;
}

/**
 * @brief Integration von f mit der Trapez-Regel.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double bogenlaenge(double a, double b, long n, double (*f)(double))
{
  double h = (b - a) / n;
  double result = 0.0;

  for (int i = 0; i < n - 1; i++)
  {
    double x1 = a + (h * i);
    double x2 = a + (h * (i + 1));
    double y1 = f(x1);
    double y2 = f(x2);

    result += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
  }
  return result;
}

/**
 * @brief Sucht die Anzahl der Stützstellen, die benötigt werden um einen Approximationsfehler < err zu haben.
 *
 * @param a        untere Grenze des Integrationsbereiches
 * @param b        obere Grenze des Integrationsbereiches
 * @param f        zu integrierende Funktion
 * @param integral zu verwendendes Integrationsverfahren
 * @param real     analytische Lösung
 * @param err      Fehlertoleranz
 */

void findsteps(double a, double b, double (*f)(double), double integral(double, double, long, double (*)(double)), double real, double err)
{
  int n = 0;
  double error = 0;

  do
  {
    n++;
    double result = integral(a, b, n, f);
    error = fabs(result - real);
  } while (error > err);

  printf("%6d Stützstellen, err=%le\n", n, error);
}

int main(void)
{
  double real;
  double a = 0.0;
  double b = 2.0;
  double (*f)(double);

  // Sie können f benutzen und an dieser Stelle abändern wenn Sie fb haben wollen
  f = f1;
  real = 1.6054129768026948485767201; // b=2

  int strips = 4;
  for (int i = 0; i < 6; i++)
  {
    double h = (b - a) / strips;
    double simpson = int_simpson(a, b, strips, f);
    double trapez = int_trapez(a, b, strips, f);
    double upper = int_ober(a, b, strips, f);
    double middle = int_mitte(a, b, strips, f);
    double lower = int_unter(a, b, strips, f);

    printf("Stützstellen  h        Simpson       Trapez        Upper         Middle        Lower \n");
    printf("%d           %.5lf  %e  %e  %e  %e  %e \n", strips, h, simpson, trapez, upper, middle, lower);
    printf("%d           %.5lf  %e  %e  %e  %e  %e \n", strips, h, (real - simpson), (real - trapez), (real - upper), (real - middle), (real - lower));
    strips = strips << 1;
  }

  // f = f2;
  // real = exp(b)- exp(a);

  // double unter = int_unter(a, b, n, f);
  // double mitte = int_mitte(a, b, n, f);
  // double ober = int_ober(a, b, n, f);
  // double trapez = int_trapez(a, b, n, f);
  // double simpson = int_simpson(a, b, n, f);
  // printf("Unter: %6lf (% .4le)\n", unter, real - unter);
  // printf("Mitte: %6lf (% .4le)\n", mitte, real - mitte);
  // printf("Ober: %6lf (% .4le)\n", ober, real - ober);
  // printf("Trapez: %6lf (% .4le)\n", trapez, real - trapez);
  // printf("Simpson: %6lf (% .4le)\n", simpson, real - simpson);

  for (int i = 3; i < 9; i++)
  {
    double err = pow(10, -i);
    printf("\nStützstellen mit Fehler %le \n", err);
    printf("Simpson:      ");
    findsteps(a, b, f, int_simpson, real, err);
    printf("Trapez:       ");
    findsteps(a, b, f, int_trapez, real, err);
    printf("Mittelsumme:  ");
    findsteps(a, b, f, int_mitte, real, err);
  }

  double bogenlaengeresult = bogenlaenge(0, 2, 100, f2);
  printf("\nBogenlänge: %.5lf \n", bogenlaengeresult);

  return 0;
}
