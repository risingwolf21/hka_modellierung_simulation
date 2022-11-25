#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*********************************************************************/
/*                                                                   */
/* Es liegen Messdaten "input.dat" in Form von Wertepaaren vor.      */
/* Durch diese Punkte muss eine Ausgleichskurve gelegt werden.       */
/*                                                                   */
/*********************************************************************/
/*             Zu Beginn einige Hilfsfunktionen                      */
// --------------------------------------------------------------------

/*
------- AT*A ---------
2.500000e+01 3.000000e+02
3.000000e+02 4.900000e+03
------- AT*logI -----
8.282651e+01
1.223997e+03
------- ATAinv ------
1.507692e-01 -9.230769e-03
-9.230769e-03 7.692308e-04
------- lambda -----
ln_a: 1.189257e+00
b:    1.769836e-01
*/

/** @brief Funktion um die Wertepaare abzuzählen.
 */
int getNumberofPoints(char *name)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;

  if ((fp = fopen(name, "r")) == NULL)
  {
    exit(EXIT_FAILURE);
  }

  int cnt = 0;
  while (getline(&line, &len, fp) != -1)
  {
    cnt++;
  }

  free(line);
  fclose(fp);

  return cnt;
}
/** @brief In dieser Funktion werden die Wertepaare eingelesen und in Form von Arrays t[N] und I[N] übergeben.
 */
void readFile(char *name, int t[], double I[])
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;

  if ((fp = fopen(name, "r")) == NULL)
  {
    exit(EXIT_FAILURE);
  }

  int cnt = 0;
  while (getline(&line, &len, fp) != -1)
  {
    sscanf(line, "%i %lf", &t[cnt], &I[cnt]);
    cnt++;
  }

  free(line);
  fclose(fp);
}

// --------------------------------------------------------------------
// --------------------------------------------------------------------

int main(int argc, char *argv[])
{
  // Abzaehlen der Wertepaare
  int N = getNumberofPoints("input.dat");

  int t[N];    // Vektor für den Kalenderwoche der Messung
  double I[N]; // Vektor für den gemessenen Inzidenzwerte
  // Einlesen der Daten
  readFile("input.dat", t, I);

  // Hilsvariabelen fuer t_0 und Logarithmen von I
  int t_0 = t[0];

  double logI[N];

  for (int x = 0; x < N; x++)
  {
    logI[x] = log(I[x]);
  }

  //_____________________________________________________________________
  // Berechnung der Matrix A^T*A und der rechten Seiten A^T*y
  double ATA[2][2] = {{0.0, 0.0},
                      {0.0, 0.0}};
  double ATA_INV[2][2] = {{0.0, 0.0},
                          {0.0, 0.0}};
  double ATy[2] = {0.0, 0.0};

  double A[2][N];
  double AT[N][2];

  for (int j = 0; j < N; j++)
  {
    A[1][j] = t[j] - t_0;
    A[0][j] = 1;

    AT[j][1] = t[j] - t_0;
    AT[j][0] = 1;

    // printf("%.0lf %.0lf\n", A[0][j], A[1][j]);
  }

  // for (int j = 0; j < N; j++)
  // {
  //   printf("%.0lf ", AT[j][0]);
  // }
  // printf("\n");
  // for (int j = 0; j < N; j++)
  // {
  //   printf("%.0lf ", AT[j][1]);
  // }
  // printf("\n");

  // for (int j = 0; j < N; j++)
  // {
  //   printf("%.5lf \n", logI[j]);
  // }

  for (int j = 0; j < N; j++)
  {
    ATy[0] += AT[j][0] * logI[j];
    ATy[1] += AT[j][1] * logI[j];
  }

  for (int x = 0; x < 2; x++)
  {
    for (int y = 0; y < 2; y++)
    {
      for (int q = 0; q < N; q++)
      {
        ATA[x][y] += (AT[q][y] * A[x][q]);
      }
    }
  }

  printf("\nATA Matrix: \n");
  printf("%e %e \n", ATA[0][0], ATA[0][1]);
  printf("%e %e \n", ATA[1][0], ATA[1][1]);

  // Invertierung von ATA (wegen kleiner Groesse)
  double det = ATA[0][0] * ATA[1][1] - ATA[1][0] * ATA[0][1];
  ATA_INV[0][0] = ATA[1][1] / det;
  ATA_INV[1][0] = ATA[1][0] / det * -1;
  ATA_INV[0][1] = ATA[0][1] / det * -1;
  ATA_INV[1][1] = ATA[0][0] / det;

  printf("\nATA Inv Matrix: \n");
  printf("%e %e \n", ATA_INV[0][0], ATA_INV[0][1]);
  printf("%e %e \n", ATA_INV[1][0], ATA_INV[1][1]);

  printf("\nATy Matrix:\n");
  printf("%e\n", ATy[0]);
  printf("%e\n", ATy[1]);

  // Ermittlung von ln(a) und b aus ATA^{-1} und ATy
  // delta = ATy * ATA^-1

  double ln_a = ATA_INV[0][0] * ATy[0] + ATA_INV[1][0] * ATy[1];
  double b = ATA_INV[0][1] * ATy[0] + ATA_INV[1][1] * ATy[1];

  // Bestimmung von a selbst
  double a = exp(ln_a);

  printf("\nln_a = %e\n", ln_a);
  printf("b = %e\n", b);

  // Plotten wenn plotflag!=0
  long plotflag = 1;

  if (plotflag)
  {
    FILE *gp = popen("gnuplot -p", "w");
    fprintf(gp, "reset; set key left top box; set xlabel \"t - t_0\";\n"
                "set ylabel \"y\";\n"
                "set autoscale fix\n"
                "set logscale xy\n"
                " f(t) = %le*exp(%le*(t - 23));\n "
                "set terminal png size 1400,1300; set output 'xyz.png';plot f(x) lt -1 lw 2, \"input.dat\" using 1:2 pt 7 title 'measured data';\n", // lt: LineType, lw: Linewidth, using 1:2: zweite spalte verwenden
            a, b);
    pclose(gp);
  }

  return 0;
}
