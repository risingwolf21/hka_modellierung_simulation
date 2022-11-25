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
  double ATlogI[2] = {0.0, 0.0};

  double A[2][N - 1];
  double AT[N - 1][2];

  for (int j = 0; j < N - 1; j++)
  {
    A[0][j] = t[j] - t_0;
    A[1][j] = 1;

    AT[j][1] = 1;
    AT[j][0] = t[j] - t_0;
  }

  double ATlogI1 = 0.0;
  double ATlogI0 = 0.0;
  for (int x = 0; x < N; x++)
  {
    ATlogI0 = ATlogI0 + (logI[x] * AT[x][0]);
    ATlogI1 = ATlogI1 + (logI[x] * 1);
  }
  ATlogI[0] = ATlogI0;
  ATlogI[1] = ATlogI1;

  for (int x = 0; x < 2; x++)
  {
    for (int y = 0; y < 2; y++)
    {
      double temp = 0;
      for (int q = 0; q < N - 1; q++)
      {
        temp = temp + (AT[q][y] * A[x][q]);
      }
      ATA[x][y] = temp;
    }
  }
  printf("A Matrix: \n");
  for (int q = 0; q < N - 1; q++)
  {
    printf("%.5lf %.5lf \n", A[0][q], A[1][q]);
  }

  printf("AT Matrix: \n");
  for (int q = 0; q < N - 1; q++)
  {
    printf("%.5lf ", AT[q][0]);
  }
  printf("\n");
  for (int q = 0; q < N - 1; q++)
  {
    printf("%.5lf ", AT[q][1]);
  }

  printf("\nATA Matrix: \n");
  printf("%.5lf %.5lf \n", ATA[0][0], ATA[0][1]);
  printf("%.5lf %.5lf \n", ATA[1][0], ATA[1][1]);

  // Invertierung von ATA (wegen kleiner Groesse)
  double det = ATA[0][0] * ATA[1][1] - ATA[1][0] * ATA[0][1];
  ATA_INV[0][0] = ATA[1][1] / det;      // a
  ATA_INV[1][0] = ATA[1][0] / det * -1; // b
  ATA_INV[0][1] = ATA[0][1] / det * -1; // c
  ATA_INV[1][1] = ATA[0][0] / det;      // d

  printf("\nATA Inv Matrix: \n");
  printf("%.5lf %.5lf \n", ATA_INV[0][0], ATA_INV[0][1]);
  printf("%.5lf %.5lf \n", ATA_INV[1][0], ATA_INV[1][1]);

  // Ermittlung von ln(a) und b aus ATA^{-1} und ATy

  // ATA * delta = ATlogI
  // delta = (ln(a),b)^T
  //  (ln(a))    (ATAlogI[0]) (ATA_INV[0][0] ATA_INV[0][1])
  //           =
  //  ( b   )    (ATAlogI[0]) (ATA_INV[1][0] ATA_INV[1][1])

  double ln_a = ATA_INV[0][0] * ATlogI[0] + ATA_INV[1][0] * ATlogI[0];
  double b = ATA_INV[0][1] * ATlogI[1] + ATA_INV[1][1] * ATlogI[1];

  // Bestimmung von a selbst
  double a = exp(ln_a);

  printf(" a = %.5lf , b = %.5lf", ln_a, b);

  // Plotten wenn plotflag!=0
  long plotflag = 1;

  if (plotflag)
  {
    FILE *gp = popen("gnuplot -p", "w");
    fprintf(gp, "reset; set key left top box; set xlabel \"t - t_0\";\n"
                "set ylabel \"y\";\n"
                "set autoscale fix\n"
                //"set logscale xy\n"
                " f(t) = %le*exp(%le*(t - 23));\n "
                "set terminal png size 1400,1300; set output 'xyz.png';plot f(x) lt -1 lw 2, \"input.dat\" using 1:2 pt 7 title 'measured data';\n", // lt: LineType, lw: Linewidth, using 1:2: zweite spalte verwenden
            a, b);
    pclose(gp);
  }

  return 0;
}
