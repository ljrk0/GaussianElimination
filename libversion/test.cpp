#include <stdio.h>
#include <stdlib.h>

// Liest eine Matrix aus einer Datei ein. Die Werte sowie die Zeilen und Spaltenanzahl müssen über den Bildschrim (nach Aufforderung) eingegeben werden. Die Größe wird dabei in die globalen Variablen "iColumns" und "iRows" geschrieben.
double ** ReadMatrix();

// Gibt eine Matrix auf dem Bildschirm aus.
void PrintMatrix(double ** ppMatrix, int iRowsToPrint, int iColumnsToPrint, FILE * fOutputStream);

// Reserviert den Speicher für eine Matrix
double ** AllocateMatrixMemory(int iColumns, int iRows);

// Löscht den Speicher für eine Matrix, "iRows" wird hier nicht benötigt, da "free" den Speicher automatisch in der richtigen Länge löscht
void FreeMatrixMemory(double ** ppMatrix, int iColumns);

int iColumns, iRows;

#include "..\DLLVersion\GAUSSIAN.h"

int main(int argc, char * argv[])
{
	double ** ppMatrix = ReadMatrix();
	GAUSSIAN_ELIMINATION_OUTPUT geo;
	geo.ppReducedEcholonForm = AllocateMatrixMemory(iColumns, iRows);
	geo.ppSMatrix = NULL;
	GaussianElimination(ppMatrix, iRows, iColumns, &geo);
	PrintMatrix(geo.ppReducedEcholonForm, iRows, iColumns, stdout);
	system("PAUSE");
    return 0;
}

double ** ReadMatrix()
{
	printf("Bitte geben Sie nun, per Komma und Leerzeichen getrennt, die Spalten- und Zeilenanzahl an: ");
	scanf("%d, %d", &iColumns, &iRows);
	
	double ** ppMatrix = AllocateMatrixMemory(iColumns, iRows);

	for(int i = 0; i < iColumns; i++) {
		for(int g = 0; g < iRows; g++) {
			printf("Bitte geben Sie nun das %d. Element der %d. Spalte an: ", g + 1, i + 1);
			scanf("%lf", &ppMatrix[i][g]);
		}
	}

	printf("\n");

	return ppMatrix;
}

double ** AllocateMatrixMemory(int iMatrixColumns, int iMatrixRows)
{
	double ** ppMatrix;
	ppMatrix = (double **)malloc(sizeof(double *) * iMatrixColumns);

	for(int i = 0; i < iMatrixColumns; i++) {
		ppMatrix[i] = (double *)malloc(sizeof(double) * iMatrixRows);
	}
	return ppMatrix;
}

void FreeMatrixMemory(double ** ppMatrix, int iMatrixColumns)
{
	// Freigeben des Speichers
	for(int i = 0; i < iMatrixColumns; i++) {
		free(ppMatrix[i]);
	}
	free(ppMatrix);
}

void PrintMatrix(double ** ppMatrix, int iRowsToPrint, int iColumnsToPrint, FILE * fOutputStream)
{
	for(int i = 0; i < iRows; i++) {
		for(int g = 0; g < iColumnsToPrint; g++) {
			fprintf(fOutputStream, "%lf", ppMatrix[g][i]);
			fprintf(fOutputStream, ";"); // Trennzeichen ausgeben
		}
		fprintf(fOutputStream, "\n");
	}
	fprintf(fOutputStream, "\n");
}
