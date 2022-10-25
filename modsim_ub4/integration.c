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

double f1(double x) {
}


/**
 * @brief Integration von f mit Untersummen.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_unter(double a, double b, long n, double (*f)(double)) {
}

/**
 * @brief Integration von f mit Mittelsummen.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_mitte(double a, double b, long n, double (*f)(double)) {
}

/**
 * @brief Integration von f mit Obersummen.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_ober(double a, double b, long n, double (*f)(double)) {
}

/**
 * @brief Integration von f mit der Trapez-Regel.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_trapez(double a, double b, long n, double (*f)(double)) {
}

/**
 * @brief Integration von f mit der Simpson-Regel.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_simpson(double a, double b, long n, double (*f)(double)) {

}

/**
 * @brief Integration von f mit der Trapez-Regel.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double bogenlaenge(double a, double b, long n, double (*f)(double)) {
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
 
void findsteps(double a, double b, double (*f)(double), double integral(double, double, long, double (*)(double)), double real, double err) {
  int n = 0;
  double error;

  printf("%6d Stützstellen, err=%le\n", n, error);
}

int main(void) {
  int n;
  double real;
  double a = 0.0;
  double b = 2.0;
  double (*f)(double);

  // Sie können f benutzen und an dieser Stelle abändern wenn Sie fb haben wollen
  f = f1;
  real = 1.6054129768026948485767201; // b=2

  //f = f2;
  //real = exp(b)- exp(a);

  // #double unter = int_unter(a, b, n, f);
  // #printf("%6lf (% .4le)\n", unten, real - unter);

  double err = 1e-3;

  findsteps(a, b, f, int_unter,   real, err);


  return 0;
}
