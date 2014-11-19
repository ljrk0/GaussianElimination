#ifndef _GAUSSIAN_H
#define _GAUSSIAN_H

#ifndef WINAPI
#define WINAPI  __stdcall
#endif

#ifndef _GEO
#define _GEO
typedef struct GaussianEliminationOutput {
	double ** ppReducedEcholonForm;
	double ** ppSMatrix;
} GAUSSIAN_ELIMINATION_OUTPUT;
#endif

// Führt den Gauß-Algorithmus zur Berechnung der Treppennormalform durch. 
// ppMatrix bleibt dabei unverändert. Der Ausgabe-Puffer muss so angepasst sein, dass 
//   1) in "ppReducedEcholonForm" genügend Speicher reserviert ist zur Aufnahme einer Matrix der gleichen Dimension wie der gegebenen Matrix und 
//   2) in "ppSMatrix" genügend Speicher reserviert ist zur Aufnahme einer quadratischen Matrix mit der Dimension "iRows | iRows"
// 			-> Wenn in ppSMatrix auf NULL verwiesen wird, wird die Berechnung der S-Matrix nicht unterdrückt
int WINAPI GaussianElimination(double ** ppMatrix, int iRows, int iColumns, GAUSSIAN_ELIMINATION_OUTPUT * pOutputBuffer);

#endif /* _GAUSSIAN_H */
