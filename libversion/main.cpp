#include "GAUSSIAN.h"
#include "background.h"

// gaussian elimination - main function
int WINAPI GaussianElimination(double ** ppMatrix, int iRows, int iColumns, GAUSSIAN_ELIMINATION_OUTPUT * pOutputBuffer)
{

	if(ppMatrix == NULL) {
		return -1;
	}

	if(pOutputBuffer->ppSMatrix != NULL) {
		iColumns += iRows;
	}
	SetMatrixDimensions(iRows, iColumns);
	double ** ppWorkMatrix;
	if(pOutputBuffer->ppSMatrix == NULL) {
		ppWorkMatrix = pOutputBuffer->ppReducedEcholonForm;
	} else {
		double ** ppWorkMatrix = AllocateMatrixMemory(iColumns + iRows, iRows);
	}
	if(!ppWorkMatrix) {
		return -1;
	}

	if(pOutputBuffer->ppSMatrix != NULL) {
		for(int i = (iColumns - iRows); i < iColumns; i++) {
			for(int g = 0; g < iRows; g++) {
				if(g == i - iColumns + iRows) {
					ppWorkMatrix[i][g] = 1.0f;

				} else {
					ppWorkMatrix[i][g] = 0.0f;
				}
			}
		}
		CopyMatrix(ppWorkMatrix, ppMatrix, iRows, iColumns - iRows);
	} else {
		CopyMatrix(ppWorkMatrix, ppMatrix, iRows, iColumns);
	}

	POSITION posPivot = StepOne(ppWorkMatrix);
	//printf("%lf, %lf", ppWorkMatrix[0][0], ppWorkMatrix[0][1]);
	StepTwo(ppWorkMatrix, &posPivot);
	//printf("%lf, %lf", ppWorkMatrix[0][0], ppWorkMatrix[0][1]);
	if(StepThree(ppWorkMatrix, posPivot) != 0) {
		return -1;
	}
	//printf("%lf, %lf", ppWorkMatrix[0][0], ppWorkMatrix[0][1]);
	StepFour(ppWorkMatrix, posPivot);
	//printf("%lf, %lf", ppWorkMatrix[0][0], ppWorkMatrix[0][1]);
	POSITION_ARRAY pivots = StepFive(ppWorkMatrix, posPivot);
	//printf("%lf, %lf", ppWorkMatrix[0][0], ppWorkMatrix[0][1]);
	AddPosition(&pivots, posPivot);
	StepSix(ppWorkMatrix, pivots);
	//printf("%lf, %lf", ppWorkMatrix[0][0], ppWorkMatrix[0][1]);

	if(pOutputBuffer->ppSMatrix) {
		DivideOutputAndSMatrix(ppWorkMatrix, pOutputBuffer);
	} else {
		CopyMatrix(pOutputBuffer->ppReducedEcholonForm, ppWorkMatrix, iRows, iColumns);
	}

	return 0;
}

// In iColumns und iRows steht die Spalten- bzw. Zeilenanzahl der aktuell bearbeiteten Matrix
static int iColumns, iRows;

// In iColumnsOffset und iRowsOffset steht die Spalten- bzw. Zeilenanzahl drin, die von der urspünglichen Matrix bereits entfernt wurde. Dies ist wichtig für die Ausgabe der korrekten Elementarmatrizen während der Rekursion.
static int iColumnsOffset = 0;
static int iRowsOffset = 0;

static void AddPosition(POSITION_ARRAY * posArray, POSITION posPositionToAdd)
{
	POSITION * tmp = posArray->pPositions;
	posArray->pPositions = (POSITION *)malloc(sizeof(POSITION) * (posArray->iCount + 1));
	for(int i = 0; i < posArray->iCount; i++) {
		posArray->pPositions[i] = tmp[i];
	}
	posArray->pPositions[posArray->iCount] = posPositionToAdd;
	posArray->iCount ++;
}

static POSITION StepOne(double ** ppMatrix)
{
	POSITION res = {-1,-1};
	for(int i = 0; i < iColumns; i++) {
		for(int g = 0; g < iRows; g++) {
			if(ppMatrix[i][g] > 0.00000001l || ppMatrix[i][g] < -0.00000001l) {
				res.iRow = g;
				res.iColumn = i;
				return res;
			} else {
				ppMatrix[i][g] = 0.0f;
			}
		}
	}
	return res;
}

static void StepTwo(double ** ppMatrix, POSITION * posPivot)
{
	if(posPivot->iRow == 0) {
		return;
	}

	// Zeilen posPivot und 0 tauschen
	for(int i = 0; i < iColumns; i++) {
		double tmp = ppMatrix[i][0];
		ppMatrix[i][0] = ppMatrix[i][posPivot->iRow];
		ppMatrix[i][posPivot->iRow] = tmp;
	}

	//fprintf(fOut, "Füge Elementarmatrix an: P[%d,%d]\n", posPivot->iRow + 1 + iRowsOffset, 1 + iRowsOffset);

	posPivot->iRow = 0;
}

static int StepThree(double ** ppMatrix, POSITION posPivot)
{
	double fElement = ppMatrix[posPivot.iColumn][0];

	if(fElement == 0) {
		return -1;
	}

	for(int i = 0; i < iColumns; i++) {
		ppMatrix[i][0] *= (1.0f / fElement);
	}
	//fprintf(fOut, "Füge Elementarmatrix an: M[%d](%lf)\n", 1 + iRowsOffset, 1.0f / fElement);
	return 0;
}

static void StepFour(double ** ppMatrix, POSITION posPivot)
{
	for(int i = 1; i < iRows; i++) {
		double scale = ppMatrix[posPivot.iColumn][i];

		if(scale < 0.00000001l && scale > -0.00000001l) {
			continue; // mit nächster Zeile weitermachen
		}

		for(int g = 0; g < iColumns; g++) {
			ppMatrix[g][i] += - ppMatrix[g][0] * scale;
		}
		//fprintf(fOut, "Füge Elementarmatrix an: G[%d,%d](%lf)\n", 1 + i + iRowsOffset, 1 + iRowsOffset, -scale);
	}
}

static POSITION_ARRAY StepFive(double ** ppMatrix, POSITION posPivot)
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
	
	double ** ppSmallerMatrix = AllocateMatrixMemory(iColumns - posPivot.iColumn - 1, iRows - 1);

	for(int i = posPivot.iColumn + 1; i < iColumns; i++) {
		for(int g = 1; g < iRows; g++) {
			ppSmallerMatrix[i - posPivot.iColumn - 1][g - 1] = ppMatrix[i][g];
		}
	}

	iRows--; 
	iRowsOffset++;
	iColumns -= posPivot.iColumn + 1;
	iColumnsOffset += posPivot.iColumn + 1;

	POSITION posNewPivot = StepOne(ppSmallerMatrix);
	if(posNewPivot.iColumn == -1) {
		// Rücksetzen
		iRowsOffset -= (iRowsSave - iRows);
		iRows = iRowsSave;
		iRowsOffset -= (iColumnsSave - iColumns);
		iColumns = iColumnsSave;
		return pivots; // Fertig
	}
	
	StepTwo(ppSmallerMatrix, &posNewPivot);
	StepThree(ppSmallerMatrix, posNewPivot);
	StepFour(ppSmallerMatrix, posNewPivot);
	pivots = StepFive(ppSmallerMatrix, posNewPivot);

	AddPosition(&pivots, posNewPivot);

	for(int i = 0; i < pivots.iCount; i++) {
		pivots.pPositions[i].iColumn += posPivot.iColumn + 1; // Auf neue Grösse anpassen
		pivots.pPositions[i].iRow += 1;
	}

	// Rücksetzen
	iRowsOffset -= (iRowsSave - iRows);
	iRows = iRowsSave;
	iColumnsOffset -= (iColumnsSave - iColumns);
	iColumns = iColumnsSave;

	// Eingruppieren der neuen Matrix
	for(int i = posPivot.iColumn + 1; i < iColumns; i++) {
		for(int g = 1; g < iRows; g++) {
			ppMatrix[i][g] = ppSmallerMatrix[i - posPivot.iColumn - 1][g - 1];
		}
	}

	// Freigeben des Speichers
	FreeMatrixMemory(ppSmallerMatrix, iColumns - posPivot.iColumn - 1);

	return pivots;
}

static void StepSix(double ** ppMatrix, POSITION_ARRAY pivots)
{
	for(int i = 0; i < pivots.iCount; i++) {
		for(int g = pivots.pPositions[i].iRow - 1; g >= 0; g--) { // von "unten" (der Matrix) nach "oben" (der Matrix) zählen
			double scale = ppMatrix[pivots.pPositions[i].iColumn][g];

			if(scale < 0.00000001l && scale > -0.00000001l) {
				continue; // mit nächster Zeile weitermachen
			}

			for(int h = 0; h < iColumns; h++) {
				ppMatrix[h][g] += - ppMatrix[h][pivots.pPositions[i].iRow] * scale;
			}
			//fprintf(fOut, "Füge Elementarmatrix an: G[%d,%d](%lf)\n", 1 + i + iRowsOffset, 1 + iRowsOffset, -scale);
		}
	}
}

static double ** AllocateMatrixMemory(int iMatrixColumns, int iMatrixRows)
{
	double ** ppMatrix;
	ppMatrix = (double **)malloc(sizeof(double *) * iMatrixColumns);

	for(int i = 0; i < iMatrixColumns; i++) {
		ppMatrix[i] = (double *)malloc(sizeof(double) * iMatrixRows);
	}
	return ppMatrix;
}

static void FreeMatrixMemory(double ** ppMatrix, int iMatrixColumns)
{
	// Freigeben des Speichers
	for(int i = 0; i < iMatrixColumns; i++) {
		free(ppMatrix[i]);
	}
	free(ppMatrix);
}

static void CopyMatrix(double ** ppBuf, double ** ppIn, int iRowsToCopy, int iColumnsToCopy)
{
	for(int i = 0; i < iRowsToCopy; i++) {
		for(int g = 0; g < iColumnsToCopy; g++) {
			ppBuf[g][i] = ppIn[g][i];
		}
	}
}

static void SetMatrixDimensions(int iNewRows, int iNewColumns)
{
	iRows = iNewRows;
	iColumns = iNewColumns;
}

static void DivideOutputAndSMatrix(double ** ppWorkMatrix, GAUSSIAN_ELIMINATION_OUTPUT * pOutputBuffer)
{
	// Treppennormalform
	for(int i = 0; i < iColumns - iRows; i++) {
		for(int g = 0; g < iRows; g++) {
			pOutputBuffer->ppReducedEcholonForm[i][g] = ppWorkMatrix[i][g];
		}
	}
	// S-Matrix
	for(int i = iColumns - iRows; i < iColumns; i++) {
		for(int g = 0; g < iRows; g++) {
			pOutputBuffer->ppSMatrix[i - (iColumns - iRows)][g] = ppWorkMatrix[i][g];
		}
	}
}
