#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32 || _WIN64 // weitere Widowsdefinitionen hinzufügen
#define _USE_WINDOWS_CODE
#endif

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

// Verwaltet die Eingabemöglichkeiten
double ** ReadMatrix();

// Liest eine Matrix aus einer Datei ein. Der Dateiname muss über den Bildschrim (nach Aufforderung) eingegeben werden. Die Größe wird dabei in die globalen Variablen "iColumns" und "iRows" geschrieben.
double ** ReadMatrixFromFile();

// Liest eine Matrix aus einer Datei ein. Die Werte sowie die Zeilen und Spaltenanzahl müssen über den Bildschrim (nach Aufforderung) eingegeben werden. Die Größe wird dabei in die globalen Variablen "iColumns" und "iRows" geschrieben.
double ** ReadMatrixFromInput();

// Gibt eine Matrix auf dem Bildschirm aus.
void PrintMatrix(double ** ppMatrix);

// Reserviert den Speicher für eine Matrix
double ** AllocateMatrixMemory(int iColumns, int iRows);

// Löscht den Speicher für eine Matrix, "iRows" wird hier nicht benötigt, da "free" den Speicher automatisch in der richtigen Länge löscht
void FreeMatrixMemory(double ** ppMatrix, int iColumns);

// Fügt zu einem Positionsarray ein Element hinzu und übernimmt dabei die Aufgaben der Speicherverwaltung.
void AddPosition(POSITION_ARRAY * posArray, POSITION posPositionToAdd);

// Führt eine Betriebsystemspefizische Funktion aus, damit das Programm stoppt, bis der Benutzer die Fortführung bestätigt.
void WaitForKey();

// Führt den ersten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt.
POSITION StepOne(double ** ppMatrix);

// Führt den zweiten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Position des ersten Elements, was ungleich NULL ist. Diese Position wurde von der vorherigen Funktion bestimmt.
void StepTwo(double ** ppMatrix, POSITION * posPivot);

// Führt den dritten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Spaltenposition des Pivotelements. Diese Position wurde in Schritt 1 bestimmt
void StepThree(double ** ppMatrix, POSITION posPivot);

// Führt den vierten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Spaltenposition des Pivotelements. Diese Position wurde in Schritt 1 bestimmt
void StepFour(double ** ppMatrix, POSITION posPivot);

// Führt den fünften Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Spaltenposition des Pivotelements. Diese Position wurde von Schritt 1 bestimmt. Diese Funktion ist rekursiv und ruft StepOne(...), StepTwo(...), StepThree(...), StepFour(...) und sich selber für eine kleinere Matrix auf ("iRows" und "iColumns" werden dabei temporär verändert und nach vollendung wieder zurückgesetzt)
POSITION_ARRAY StepFive(double ** ppMatrix, POSITION posPivot);

// Führt den sechsten Schritt zur Treppennormalform für eine beliebige Matrix aus. Die Größe der Matrix wird dabei aus den globalen Variablen "iRows" und "iColumns" bestimmt. Die Funktion verwendet dabei die Position aller Pivotelemente. Diese wurden von StepFive(...) gesammelt.
void StepSix(double ** ppMatrix, POSITION_ARRAY pposPivots);

// Multipliziert an die Matrix "Matrix" die Matrix "MatrixToAdd" von Rechts heran.
void RightAddMultiply(double ** ppMatrix, double ** ppMatrixToAdd);

// Erstellt eine P-Matrix
void CreateExcangcheMatrix(double ** ppMatrixBuffer, int iRow1, int iRow2);

// Erstellt eine M-Matrix
void CreateScaleMatrix(double ** ppMatrixBuffer, int iRow, double fScale);

// Erstellt eine G-Matrix
void CreateGaussMatrix(double ** ppMatrixBuufer, int iRow1, int iRow2, double fScale);

// In iColumns und iRows steht die Spalten- bzw. Zeilenanzahl der aktuell bearbeiteten Matrix
int iColumns, iRows;

// In iColumnsOffset und iRowsOffset steht die Spalten- bzw. Zeilenanzahl drin, die von der urspünglichen Matrix bereits entfernt wurde. Dies ist wichtig für die Ausgabe der korrekten Elementarmatrizen während der Rekursion.
int iColumnsOffset = 0;
int iRowsOffset = 0;

// In fOut steht ein Stream zur Ausgabedatei der Elementarmatrizen.
FILE * fOut;

