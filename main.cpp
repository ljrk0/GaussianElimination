#include "main.h"

// Hauptfunktion. Programmaufrufspunkt.
int main(int argc, char argv[])
{
	double ** ppMatrix = ReadMatrix();
	if(ppMatrix == NULL) {
		printf("Error while reading matrix. Terminate programm. / Fehler beim Einlesen der Matrix. Beende Programm");
	}

	printf("Eingelesen - ");
	PrintMatrix(ppMatrix);

	POSITION posPivot = StepOne(ppMatrix);
	if(posPivot.iColumn == -1) {
		printf("Fettig. Nix zu tun.");
	}


	StepTwo(ppMatrix, posPivot);
	printf("Schritt 1 & 2 - ");
	PrintMatrix(ppMatrix);
	StepThree(ppMatrix, posPivot);
	printf("Schritt 3 - ");
	PrintMatrix(ppMatrix);
	StepFour(ppMatrix, posPivot);
	printf("Schritt 4 - ");
	PrintMatrix(ppMatrix);
	POSITION_ARRAY pivots = StepFive(ppMatrix, posPivot);
	printf("Schritt 5 - ");
	PrintMatrix(ppMatrix);
	AddPosition(&pivots, posPivot);
	StepSix(ppMatrix, pivots);
	printf("Fertig - ");
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

void AddPosition(POSITION_ARRAY * posArray, POSITION posPositionToAdd)
{
	POSITION * tmp = posArray->pPositions;
	posArray->pPositions = (POSITION *)malloc(sizeof(POSITION) * (posArray->iCount + 1));
	for(int i = 0; i < posArray->iCount; i++) {
		posArray->pPositions[i] = tmp[i];
	}
	posArray->pPositions[posArray->iCount] = posPositionToAdd;
	posArray->iCount ++;
}

POSITION StepOne(double ** ppMatrix)
{
	POSITION res = {-1,-1};
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

void StepTwo(double ** ppMatrix, POSITION posPivot)
{
	double * pTemp = ppMatrix[posPivot.iColumn];

	ppMatrix[posPivot.iColumn] = ppMatrix[0];
	ppMatrix[0] = pTemp;
}

void StepThree(double ** ppMatrix, POSITION posPivot)
{
	double fElement = ppMatrix[posPivot.iColumn][0];

	for(int i = 0; i < iColumns; i++) {
		ppMatrix[i][0] *= (1.0f / fElement);
	}
}

void StepFour(double ** ppMatrix, POSITION posPivot)
{
	for(int i = 1; i < iRows; i++) {
		double scale = ppMatrix[posPivot.iColumn][i];

		if(scale == 0.0f) {
			continue; // mit nächster Zeile weitermachen
		}

		for(int g = 0; g < iColumns; g++) {
			ppMatrix[g][i] += - ppMatrix[g][0] * scale;
		}
	}
}

POSITION_ARRAY StepFive(double ** ppMatrix, POSITION posPivot)
{
	POSITION_ARRAY pivots = {NULL, 0};
	int iRowsSave = iRows;
	int iColumnsSave = iColumns;

	if(iRows - 1 == 0) {
		return pivots; // Fertig
	}

	if(iColumns - posPivot.iColumn == 0) {
		return pivots; // Fertig
	}

	double ** ppSmallerMatrix = (double **)malloc(sizeof(double *) * iRows - 1);

	for(int i = 0; i < iRows; i++) {
		ppSmallerMatrix[i] = (double *)malloc(sizeof(double) * (iColumns - posPivot.iColumn - 1));
	}

	for(int i = posPivot.iColumn + 1; i < iColumns; i++) {
		for(int g = 1; g < iRows; g++) {
			ppSmallerMatrix[i - posPivot.iColumn - 1][g - 1] = ppMatrix[i][g];
		}
	}

	iRows--;
	iColumns -= posPivot.iColumn + 1;

	POSITION posNewPivot = StepOne(ppSmallerMatrix);
	if(posNewPivot.iColumn == -1) {
		// Rücksetzen
		iRows = iRowsSave;
		iColumns = iColumnsSave;
		return pivots; // Fertig
	}
	StepTwo(ppSmallerMatrix, posNewPivot);
	//printf("Schritt 1 & 2 - ");
	//PrintMatrix(ppSmallerMatrix);
	StepThree(ppSmallerMatrix, posNewPivot);
	//printf("Schritt 3 - ");
	//PrintMatrix(ppSmallerMatrix);
	StepFour(ppSmallerMatrix, posNewPivot);
	//printf("Schritt 4 - ");
	//PrintMatrix(ppSmallerMatrix);
	pivots = StepFive(ppSmallerMatrix, posNewPivot);
	//printf("Schritt 5 - ");
	//PrintMatrix(ppSmallerMatrix);

	AddPosition(&pivots, posNewPivot);

	for(int i = 0; i < pivots.iCount; i++) {
		pivots.pPositions[i].iColumn += posPivot.iColumn + 1; // Auf neue Größe anpassen
		pivots.pPositions[i].iRow += 1;
	}

	// Rücksetzen
	iRows = iRowsSave;
	iColumns = iColumnsSave;

	// Eingruppieren der neuen Matrix
	for(int i = posPivot.iColumn + 1; i < iColumns; i++) {
		for(int g = 1; g < iRows; g++) {
			ppMatrix[i][g] = ppSmallerMatrix[i - posPivot.iColumn - 1][g - 1];
		}
	}

	return pivots;
}

void StepSix(double ** ppMatrix, POSITION_ARRAY pivots)
{
	for(int i = 0; i < pivots.iCount; i++) {
		for(int g = pivots.pPositions[i].iRow - 1; g >= 0; g--) { // von "unten" (der Matrix) nach "oben" (der Matrix) zählen
			double scale = ppMatrix[pivots.pPositions[i].iColumn][g];

			if(scale == 0.0f) {
				continue; // mit nächster Zeile weitermachen
			}

			for(int h = 0; h < iColumns; h++) {
				ppMatrix[h][g] += - ppMatrix[h][pivots.pPositions[i].iRow] * scale;
			}
		}
	}
}
