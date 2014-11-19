#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

// Diese Struktur ist dafür zuständig, eine bleliebige Position in einer Matrix zu speichern
typedef struct Position {
	int iRow; // Zeile des Elements
	int iColumn; // Spalte des Elements
} POSITION;

// Diese Struktur ist dafür zuständig, ein (dynamisches) Array und die dazugehörige Länge zu speichern (diese kann aus dem Array-Pointer nicht bestimmt werden)
typedef struct PositionArray {
	POSITION * pPositions; // Pointer auf das Array
	int iCount; // Länge des Arrays
} POSITION_ARRAY;

#ifndef _GEO
#define _GEO
typedef struct GaussianEliminationOutput {
	double ** ppReducedEcholonForm;
	double ** ppSMatrix;
} GAUSSIAN_ELIMINATION_OUTPUT;
#endif

// Setzt "iRows" und "iColumns"
static void SetMatrixDimensions(int iRows, int iColumns);

// Reserviert den Speicher für eine Matrix
static double ** AllocateMatrixMemory(int iColumns, int iRows);

// Löscht den Speicher für eine Matrix, "iRows" wird hier nicht benötigt, da "free" den Speicher automatisch in der richtigen Länge löscht
static void FreeMatrixMemory(double ** ppMatrix, int iColumns);

// Fügt zu einem Positionsarray ein Element hinzu und übernimmt dabei die Aufgaben der Speicherverwaltung.
static void AddPosition(POSITION_ARRAY * posArray, POSITION posPositionToAdd);

// Führt den ersten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt.
static POSITION StepOne(double ** ppMatrix);

// Führt den zweiten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Position des ersten Elements, was ungleich NULL ist. Diese Position wurde von der vorherigen Funktion bestimmt.
static void StepTwo(double ** ppMatrix, POSITION * posPivot);

// Führt den dritten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Spaltenposition des Pivotelements. Diese Position wurde in Schritt 1 bestimmt
static int StepThree(double ** ppMatrix, POSITION posPivot);

// Führt den vierten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Spaltenposition des Pivotelements. Diese Position wurde in Schritt 1 bestimmt
static void StepFour(double ** ppMatrix, POSITION posPivot);

// Führt den fünften Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Spaltenposition des Pivotelements. Diese Position wurde von Schritt 1 bestimmt. Diese Funktion ist rekursiv und ruft StepOne(...), StepTwo(...), StepThree(...), StepFour(...) und sich selber für eine kleinere Matrix auf ("iRows" und "iColumns" werden dabei temporär verändert und nach vollendung wieder zurückgesetzt)
static POSITION_ARRAY StepFive(double ** ppMatrix, POSITION posPivot);

// Führt den sechsten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Position aller Pivotelemente. Diese wurden von StepFive(...) gesammelt.
static void StepSix(double ** ppMatrix, POSITION_ARRAY pposPivots);

// Kopiert eine Matrix
static void CopyMatrix(double ** ppBuf, double ** ppIn, int iRows, int iColumns);

// Spaltet die errechnete Treppennormalform in die Ausgabematrix und die S-Matrix
static void DivideOutputAndSMatrix(double ** ppWorkMatrix, GAUSSIAN_ELIMINATION_OUTPUT * pOutputBuffer);
