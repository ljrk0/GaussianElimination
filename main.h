#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Liest eine Matrix aus einer Datei namens "Matrix.txt" ein. Die Größe wird dabei in die globalen Variablen "iColumns" und "iRows" geschrieben.
double ** ReadMatrix();

// Gibt eine Matrix auf dem Bildschirm aus.
void PrintMatrix(double ** ppMatrix);

// Fügt zu einem Positionsarray ein Element hinzu und übernimmt dabei die Aufgaben der Speicherverwaltung.
void AddPosition(POSITION_ARRAY * posArray, POSITION posPositionToAdd);

// Führt den ersten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt.
POSITION StepOne(double ** ppMatrix);

// Führt den zweiteh Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Position des ersten Elements, was ungleich NULL ist. Diese Position wurde von der vorherigen Funktion bestimmt.
void StepTwo(double ** ppMatrix, POSITION posPivot);

// Führt den zweiteh Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Spaltenposition des Pivotelements. Diese Position wurde in Schritt 1 bestimmt
void StepThree(double ** ppMatrix, POSITION posPivot);

// Führt den zweiteh Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Spaltenposition des Pivotelements. Diese Position wurde in Schritt 1 bestimmt
void StepFour(double ** ppMatrix, POSITION posPivot);

// Führt den fünften Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Spaltenposition des Pivotelements. Diese Position wurde von Schritt 1 bestimmt. Diese Funktion ist rekursiv und ruft StepOne(...), StepTwo(...), StepThree(...), StepFour(...) und sich selber für eine kleinere Matrix auf ("iRows" und "iColumns" werden dabei temporär verändert und nach vollendung wieder zurückgesetzt)
POSITION_ARRAY StepFive(double ** ppMatrix, POSITION posPivot);

// Führt den sechsten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Position aller Pivotelemente. Diese wurden von StepFive(...) gesammelt.
void StepSix(double ** ppMatrix, POSITION_ARRAY pposPivots);

// In iColumns und iRows steht die Spalten- bzw. Zeilenanzahl der aktuell bearbeiteten Matrix
int iColumns, iRows;
