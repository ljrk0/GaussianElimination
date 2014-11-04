#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct result1 {
	int iRow;
	int iColumn;
} RESULT1;

double ** ReadMatrix();
RESULT1 StepOne(double ** ppMatrix);
void StepTwo(double ** ppMatrix, RESULT1 result1);
void StepThree(double ** ppMatrix, RESULT1 result1);
void StepFour(double ** ppMatrix, RESULT1 result1);
void StepFive(double ** ppMatrix, RESULT1 result1);

int iColumns, iRows;

int main(int argc, char argv[])
{
	double ** ppMatrix = ReadMatrix();

	RESULT1 result1 = StepOne(ppMatrix);
	if(result1.iColumn == -1) {
		printf("Fettig. Nix zu tun.");
	}

	StepTwo(ppMatrix, result1);
	StepThree(ppMatrix, result1);
	StepFour(ppMatrix, result1);
	StepFive(ppMatrix, result1);

}

double ** ReadMatrix()
{
	FILE * fFile = fopen("matrix.txt", "r");

	fscanf(fFile, "%d;%d", &iColumns, &iRows);

	double ** ppMatrix;
	ppMatrix = malloc(sizeof(double *) * iRows);

	for(int i = 0; i < iRows; i++) {
		ppMatrix[i] = malloc(sizeof(double) * iColumns);
	}

	char str[1000];

	for(int i = 0; i < iColumns; i++) {
		fgets(str, 1000, fFile);

		char var[1000];

		int pos = 0;
		for(int g = 0; g < iRows; g++) {
			for(int h = 0; h < strlen(str); g++) {
				if(str[pos] == ';') {
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
		ppMatrix[i][0] /= (1.0f / fElement);
	}
}

void StepFour(double ** ppMatrix, RESULT1 result1)
{
	for(int i = 1; i < iRows; i++) {
		double scale = ppMatrix[result1.iColumn][i];

		for(int g = 0; g < iColumns; g++) {
			ppMatrix[g][i] = - ppMatrix[g][0] * scale;
		}
	}
}

void StepFive(double ** ppMatrix, RESULT1 result1)
{
	int iRowsSave = iRows;
	int iColumnsSave = iColumns;



	double ** ppSmallerMatrix = malloc(sizeof(double *) * iRows - 1);

	for(int i = 0; i < iRows; i++) {
		ppSmallerMatrix[i] = malloc(sizeof(double) * (iColumns - result1.iColumn));
	}

	for(int i = result1.iColumn; i < iColumns; i++) {
		for(int g = 1; g < iRows; g++) {
			ppSmallerMatrix[i - result1.iColumn][g - 1] = ppMatrix[i][g];
		}
	}

	iRows--;
	iColumns -= result1.iColumn;

	RESULT1 res1 = StepOne(ppSmallerMatrix);
	StepTwo(ppSmallerMatrix, res1);
	StepThree(ppSmallerMatrix, res1);
	StepFour(ppSmallerMatrix, res1);
	StepFive(ppSmallerMatrix, res1);

	iRows = iRowsSave;
	iColumns = iColumnsSave;

	return;
}
