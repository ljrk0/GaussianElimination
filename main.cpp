#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct result1 {
	int iRow;
	int iColumn;
} RESULT1;

double ** ReadMatrix();
void PrintMatrix(double ** ppMatrix);
RESULT1 StepOne(double ** ppMatrix);
void StepTwo(double ** ppMatrix, RESULT1 result1);
void StepThree(double ** ppMatrix, RESULT1 result1);
void StepFour(double ** ppMatrix, RESULT1 result1);
void StepFive(double ** ppMatrix, RESULT1 result1);

int iColumns, iRows;

int main(int argc, char argv[])
{
	double ** ppMatrix = ReadMatrix();
	if(ppMatrix == NULL) {
		printf("Error while reading matrix. Terminate programm. / Fehler beim Einlesen der Matrix. Beende Programm");
	}

	PrintMatrix(ppMatrix);

	RESULT1 result1 = StepOne(ppMatrix);
	if(result1.iColumn == -1) {
		printf("Fettig. Nix zu tun.");
	}

	StepTwo(ppMatrix, result1);
	PrintMatrix(ppMatrix);
	StepThree(ppMatrix, result1);
	PrintMatrix(ppMatrix);
	StepFour(ppMatrix, result1);
	PrintMatrix(ppMatrix);
	StepFive(ppMatrix, result1);
	PrintMatrix(ppMatrix);
	system("pause");
}

double ** ReadMatrix()
{
	FILE * fFile = fopen("matrix.txt", "r");

	// stream überprüfen
	if(fFile == NULL) {
		printf("Fatal Error: Cannot open file: / Fataler Fehler: Kann Datei nicht öffnen: matrix.txt");
		return NULL;
	}

	fscanf(fFile, "%d;%d\n", &iColumns, &iRows);

	double ** ppMatrix;
	ppMatrix = (double **)malloc(sizeof(double *) * iColumns);

	for(int i = 0; i < iColumns; i++) {
		ppMatrix[i] = (double *)malloc(sizeof(double) * iRows);
	}

	char str[1000];

	for(int i = 0; i < iRows; i++) {
		fgets(str, 1000, fFile);

		char var[1000];

		int pos = 0;
		for(int g = 0; g < iColumns; g++) {
			for(int h = 0; pos < strlen(str); h++) {
				if(str[pos] == ';' || str[pos] == '\n') {
					pos++;
					break;
				}
				var[h] = str[pos];
				pos++;
			}
			ppMatrix[g][i] = atof(var);
		}
	}

	fclose(fFile);

	return ppMatrix;
}

void PrintMatrix(double ** ppMatrix)
{
	printf("Print Matrix\n");
	for(int i = 0; i < iRows; i++) {
		for(int g = 0; g < iColumns; g++) {
			printf("%lf", ppMatrix[g][i]);
			if(g == iColumns - 1) { // Letzte Spalte 
				printf("\n"); // Auf neue Zeile wechseln
			} else {
				printf(";"); // Trennzeichen ausgeben
			}
		}
	}
	printf("\n");
}

RESULT1 StepOne(double ** ppMatrix)
{
	RESULT1 res = {-1,-1};
	for(int i = 0; i < iColumns; i++) {
		for(int g = 0; g < iRows; g++) {
			if(ppMatrix[i][g] != 0.0f) {
				res.iRow = i;
				res.iColumn = g;
				return res;
			}
		}
	}
	return res;
}

void StepTwo(double ** ppMatrix, RESULT1 result1)
{
	double * pTemp = ppMatrix[result1.iColumn];

	ppMatrix[result1.iColumn] = ppMatrix[0];
	ppMatrix[0] = pTemp;
}

void StepThree(double ** ppMatrix, RESULT1 result1)
{
	double fElement = ppMatrix[result1.iColumn][result1.iRow];

	for(int i = 0; i < iColumns; i++) {
		ppMatrix[i][0] *= (1.0f / fElement);
	}
}

void StepFour(double ** ppMatrix, RESULT1 result1)
{
	for(int i = 1; i < iRows; i++) {
		double scale = ppMatrix[result1.iColumn][i];

		if(scale == 0.0f) {
			continue; // mit nächster Zeile weitermachen
		}

		for(int g = 0; g < iColumns; g++) {
			ppMatrix[g][i] += - ppMatrix[g][0] * scale;
		}
	}
}

void StepFive(double ** ppMatrix, RESULT1 result1)
{
	int iRowsSave = iRows;
	int iColumnsSave = iColumns;

	if(iRows - 1 == 0) {
		return; // Fertig
	}

	if(iColumns - result1.iColumn == 0) {
		return; // Fertig
	}

	double ** ppSmallerMatrix = (double **)malloc(sizeof(double *) * iRows - 1);

	for(int i = 0; i < iRows; i++) {
		ppSmallerMatrix[i] = (double *)malloc(sizeof(double) * (iColumns - result1.iColumn - 1));
	}

	for(int i = result1.iColumn + 1; i < iColumns; i++) {
		for(int g = 1; g < iRows; g++) {
			ppSmallerMatrix[i - result1.iColumn - 1][g - 1] = ppMatrix[i][g];
		}
	}

	iRows--;
	iColumns -= result1.iColumn + 1;

	RESULT1 res1 = StepOne(ppSmallerMatrix);
	if(res1.iColumn == -1) {
		// Rücksetzen
		iRows = iRowsSave;
		iColumns = iColumnsSave;
		return; // Fertig
	}
	StepTwo(ppSmallerMatrix, res1);
	StepThree(ppSmallerMatrix, res1);
	StepFour(ppSmallerMatrix, res1);
	StepFive(ppSmallerMatrix, res1);

	// Rücksetzen
	iRows = iRowsSave;
	iColumns = iColumnsSave;

	return;
}
