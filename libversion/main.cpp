#include "GAUSSIAN.h"
#include "background.h"

// gaussian elimination - main function
int WINAPI GaussianElimination(double ** ppMatrix, int iRows, int iColumns, GAUSSIAN_ELIMINATION_OUTPUT * pOutputBuffer)
{

	if(ppMatrix == NULL) {
		return -1;
	}

	iColumns += iRows;
	SetMatrixDimensions(iRows, iColumns);
	double ** ppWorkMatrix = AllocateMatrixMemory(iColumns + iRows, iRows);
	if(!ppWorkMatrix) {
		return -1;
	}

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

	DivideOutputAndSMatrix(ppWorkMatrix, pOutputBuffer);

	return 0;
}
