#include "main.h"

// Hauptfunktion. Programmaufrufspunkt.
int main(int argc, char * argv[])
{
	PARSE_RESULT pr = ParseArguments(argc, argv);

	bImmediateExit = pr.bImmediateExit;
	bNoSCalculation = pr.bNoSCalculation;

	if(pr.bPrintHelp) {
		PrintHelp();
		WaitForKey();
		return 0;
	}

	double ** ppMatrix = ReadMatrix(pr);
	
	if(!pr.bNoSCalculation) {
		iColumns += iRows;
		PrintMatrix(ppMatrix, FALSE, stdout);
		double ** ppTmpMatrix = AllocateMatrixMemory(iColumns + iRows, iRows);

		for(int i = (iColumns - iRows); i < iColumns; i++) {
			for(int g = 0; g < iRows; g++) {
				if(g == i - iColumns + iRows) {
					ppTmpMatrix[i][g] = 1.0f;

				} else {
					ppTmpMatrix[i][g] = 0.0f;
				}
			}
		}
		CopyMatrix(ppTmpMatrix, ppMatrix, iRows, iColumns - iRows);
		ppMatrix = ppTmpMatrix;
	}

	if(ppMatrix == NULL) {
		printf("Error while reading matrix. Terminate programm. / Fehler beim Einlesen der Matrix. Beende Programm.\n");
		//system("pause");
		WaitForKey();
		return 0;
	}

	printf("Eingelesen - ");
	PrintMatrix(ppMatrix, FALSE, stdout);
	if(pr.bVerbose) {
		fprintf(fOut, "Eingelesen - ");
		PrintMatrix(ppMatrix, FALSE, fOut);
	}

	POSITION posPivot = StepOne(ppMatrix);
	if(posPivot.iColumn == -1) {
		printf("Fettig. Nix zu tun.\n");
		//system("pause");
		WaitForKey();
		return 0;
	}

	StepTwo(ppMatrix, &posPivot);
	printf("\nSchritt 1 & 2 - ");
	PrintMatrix(ppMatrix, FALSE, stdout);
	if(pr.bVerbose) {
		fprintf(fOut, "\nSchritt 1 & 2 - ");
		PrintMatrix(ppMatrix, FALSE, fOut);
	}
	StepThree(ppMatrix, posPivot);
	printf("\nSchritt 3 - ");
	PrintMatrix(ppMatrix, FALSE, stdout);
	if(pr.bVerbose) {
		fprintf(fOut, "\nSchritt 3 - ");
		PrintMatrix(ppMatrix, FALSE, fOut);
	}
	StepFour(ppMatrix, posPivot);
	printf("\nSchritt 4 - ");
	PrintMatrix(ppMatrix, FALSE, stdout);
	if(pr.bVerbose) {
		fprintf(fOut, "\nSchritt 4 - ");
		PrintMatrix(ppMatrix, FALSE, fOut);
	}
	POSITION_ARRAY pivots = StepFive(ppMatrix, posPivot);
	printf("\nSchritt 5 - ");
	PrintMatrix(ppMatrix, FALSE, stdout);
	if(pr.bVerbose) {
		fprintf(fOut, "\nSchritt 5 - ");
		PrintMatrix(ppMatrix, FALSE, fOut);
	}
	AddPosition(&pivots, posPivot);
	StepSix(ppMatrix, pivots);
	printf("\nFertig - ");
	PrintMatrix(ppMatrix, FALSE, stdout);
	if(pr.bVerbose) {
		fprintf(fOut, "\nFertig - ");
		PrintMatrix(ppMatrix, FALSE, fOut);
	}
	if(!pr.bNoSCalculation) {
		PrintMatrix(ppMatrix, TRUE, stdout);
		if(pr.bVerbose) {
			PrintMatrix(ppMatrix, TRUE, fOut);
		}
	}
	//system("pause");
	WaitForKey();
	FreeMatrixMemory(ppMatrix, iColumns);
	return 0;
}

PARSE_RESULT ParseArguments(int argc, char * argv[])
{
	PARSE_RESULT pr = {
		FALSE, 
		FALSE, 
		FALSE, 
		FALSE, 
		FALSE, 
		FALSE,
		FALSE,
		NULL, 
		NULL};
	if(argc < 2) {
		return pr;
	} else {
		int iFilePlacesUsed = 0;
		FILE_TYPE ftFileInput[3] = {NOTHING, NOTHING, NOTHING};
		for(int i = 1 /* erstes Argument = Dateipfad = ignorieren */; i < argc; i++) {
			if(iFilePlacesUsed > 0) {
				if(ftFileInput[0] == INPUT) {
					pr.sInputFileName = (char *)malloc(strlen(argv[i]) + 1);
					strcpy(pr.sInputFileName, argv[i]);
				} else if(ftFileInput[0] == OUTPUT) {
					pr.sOutputFileName = (char *)malloc(strlen(argv[i]) + 1);
					strcpy(pr.sOutputFileName, argv[i]);
				}
				ftFileInput[0] = ftFileInput[1];
				ftFileInput[1] = ftFileInput[2];
				ftFileInput[2] = NOTHING;
				iFilePlacesUsed--;
			} else if(argv[i][0] == '-' && argv[i][1] != '-') { // "-(...)" und nicht "--(...)"
				for(int g = 1; argv[i][g] != '\0'; g++) {
					switch(argv[i][g]) {
						case 'i':
							pr.bUseInput = TRUE;
							ftFileInput[iFilePlacesUsed] = INPUT;
							iFilePlacesUsed ++;
							break;
						case 'o':
							pr.bUseOutput = TRUE;
							ftFileInput[iFilePlacesUsed] = OUTPUT;
							iFilePlacesUsed ++;
							break;
						case 'v':
							pr.bVerbose = TRUE;
							break;
						case 'a':
							pr.bAppend = TRUE;
							break;
						case 'n':
							pr.bNoSCalculation = TRUE;
							break;
						case 'e':
							pr.bImmediateExit = TRUE;
							break;
						case 'h':
							pr.bPrintHelp = TRUE;
							break;
						default:
							printf("Unknown argument: %c. It is ignored. / Unbekannter Parameter: %c. Wird ignoriert.\n", argv[i][g]);
					}
				}
			} else if(argv[i][0] == '-' && argv[i][1] == '-') {
				if(strcmp(argv[i], "--output") == 0) {
					pr.bUseInput = TRUE;
					ftFileInput[iFilePlacesUsed] = OUTPUT;
					iFilePlacesUsed ++;
				} else if(strcmp(argv[i], "--input") == 0) {
					pr.bUseOutput = TRUE;
					ftFileInput[iFilePlacesUsed] = INPUT;
					iFilePlacesUsed ++;
				} else if(strcmp(argv[i], "--verbose") == 0) {
					pr.bVerbose = TRUE;
				} else if(strcmp(argv[i], "--append") == 0) {
					pr.bAppend = TRUE;
				} else if(strcmp(argv[i], "--noS") == 0) {
					pr.bNoSCalculation = TRUE;
				} else if(strcmp(argv[i], "--help") == 0) {
					pr.bPrintHelp = TRUE;
				} else if(strcmp(argv[i], "--exit_immediately") == 0) {
					pr.bImmediateExit = TRUE;
				} else {
					printf("Unknown argument. Ignore. / Unbekannter Parameter. Wird ignoriert.\n");
				}
			}
		}
		if(iFilePlacesUsed != 0) {
#ifdef _USE_WINDOWS_CODE
			printf("Please give all file paths for the specified arguments! / Bitte geben Sie alle Dateipfade f\x81r die spezifizierten Parameter an!\n");
#else
			printf("Please give all file paths for the specified arguments! / Bitte geben Sie alle Dateipfade für die spezifizierten Parameter an!\n");
#endif
			if(!pr.bImmediateExit) {
				WaitForKey();
			}
			exit(-1);
		}
	}
	return pr;
}

void PrintHelp()
{
	printf("Help to Gaussian Elimination Calculator\n");
	printf("\n");
	printf("This small console programm calculates the gaussian elimination algorithm for a given matrix.\n");
	printf("\n");
	printf("You can use this command line arguments to specify programm's behaviour:\n");
	printf("	-o/--output: Write the elementary matrixs uses while algorithm to a file. In standard mode, the file will be overwrited.\n");
	printf("	-i/--input: Read the input matrix from a file.\n");
	printf("	-v/--verbose: Gives more detailed output to the output file. For this parameter, you have to specify parameter '-o/--output', otherwise the parameter would be ignored.\n");
	printf("	-a/--append: Prevent owerwriting the output file. The content will be append to file. For this parameter, you have to specify parameter '-o/--output', otherwise the parameter would be ignored.\n");
	printf("	-n/--noS: Suppress calculation of the S matrix.\n");
	printf("	-e/--exit_immediately: Suppress the secure query befor terminate programm.\n");
	printf("	-h/--help: Gives the help. After this, programm terminate.\n");
	printf("\n");
	printf("Hilfe zum Treppennormalformberechner\n");
	printf("\n");
#ifdef _USE_WINDOWS_CODE
	printf("Dieses kleine Konsolenprogramm berechnet \x81 \bber das Gauss'sche Eliminationsverfahren die Treppennormalform f\x81r eine gegebene Matrix aus.");
#else
	printf("Dieses kleine Konsolenprogramm berechnet über das Gauss'sche Eliminationsverfahren die Treppennormalform für eine gegebene Matrix aus.");
#endif
	printf("\n");
#ifdef _USE_WINDOWS_CODE
	printf("Sie k\x94nnen folgene Parameter benutzen, um das Verhalten des Programms anzupassen:\n");
#else
	printf("Sie können folgene Parameter benutzen, um das Verhalten des Programms anzupassen:\n");
#endif
	printf("	-o/--output: Schreibt die Elementarmatrizen, die zur Berechnung genutzt werden, in eine Datei.\n");
	printf("	-i/--input: Liest die Eingabematrix aus einer Datei.\n");
#ifdef _USE_WINDOWS_CODE
	printf("	-v/--verbose: Gibt mehr detailliertere Ergebnisse in die Ausgabedatei. Um diesen Parameter nutzen zu k\x94nnen, m\x81ssen Sie auch den Parameter '-o/--output' angeben, sonst wird der Parameter ignoriert.\n");
#else
	printf("	-v/--verbose: Gibt mehr detailliertere Ergebnisse in die Ausgabedatei. Um diesen Parameter nutzen zu können, müssen Sie auch den Parameter '-o/--output' angeben, sonst wird der Parameter ignoriert.\n");
#endif
#ifdef _USE_WINDOWS_CODE
	printf("	-v/--verbose: Verhinder das \x9A \bberschreiben der Ausgabedatei. Der Inhalt wird stattdessen angeh\x84ngt. Um diesen Parameter nutzen zu k\x94nnen, m\x81ssen Sie auch den Parameter '-o/--output' angeben, sonst wird der Parameter ignoriert.\n");
#else
	printf("	-v/--verbose: Verhinder das Überschreiben der Ausgabedatei. Der Inhalt wird stattdessen angehängt. Um diesen Parameter nutzen zu können, müssen Sie auch den Parameter '-o/--output' angeben, sonst wird der Parameter ignoriert.\n");
#endif
	printf("	-n/--noS: Verhindert das Berechnen der S-Matrix.\n");
	printf("	-e/--exit_immediately: Verhindert die Sicherheitsabfrage vor dem Beenden des Programms.\n");
	printf("	-h/--help: Gibt die Hilfe aus. Das Programm wird sich danach beenden.\n");
}

double ** ReadMatrix(PARSE_RESULT prOptions)
{
	double ** ppMatrix;
	char answer[100];
	if(prOptions.bUseInput) {
		strcpy(answer, "Datei");
	} else {
#ifdef _USE_WINDOWS_CODE
		printf("Wollen Sie die Matrix aus einer Datei laden oder \x81 \bber den Bildschirm eingeben? [Datei/Bildschirm]\n");
#else
		printf("Wollen Sie die Matrix aus einer Datei laden oder über den Bildschirm eingeben? [Datei/Bildschirm]\n");
#endif
		printf("  ");
		scanf("%s", answer);
		fflush(stdin);
	}

	if(strcmp(answer, "Datei") == 0 || strcmp(answer, "D") == 0) {
		if(prOptions.bUseInput) {
			ppMatrix = ReadMatrixFromFile(prOptions.sInputFileName);
		} else {
			ppMatrix = ReadMatrixFromFile(NULL);
		}
	} else {
		ppMatrix = ReadMatrixFromInput();
	}
	
	char answer2[100];
	if(prOptions.bUseOutput) {
		strcpy(answer2, "Datei");
	} else {
		printf("Wollen Sie die Elementarmatrizen in eine Datei schreiben oder auf dem Bildschirm ausgeben? [Datei/Bildschirm]\n");
		printf("  ");
		scanf("%s", answer2);
		fflush(stdin);
	}

	if(strcmp(answer2, "Datei") == 0) {
		char * sOutputFileName;
		if(!prOptions.bUseOutput) {
			sOutputFileName = (char *)malloc(sizeof(char) * 200);
			printf("Bitte geben Sie den Dateinamen an: ");
			fgets(sOutputFileName, 100, stdin);
			sOutputFileName[strlen(sOutputFileName) - 1] = '\0';
		} else {
			sOutputFileName = (char *)malloc(sizeof(char) * (strlen(prOptions.sOutputFileName) + 1));
			strcpy(sOutputFileName, prOptions.sOutputFileName);
		}

		if(prOptions.bAppend) {
			fOut = fopen(sOutputFileName, "a");
		} else {
			fOut = fopen(sOutputFileName, "w");
		}

		if(fOut == NULL) {
#ifdef _USE_WINDOWS_CODE
			printf("Fatal Error: Cannot open file: / Fataler Fehler: Kann Datei nicht \x94 \bffnen: %s", answer);
#else
			printf("Fatal Error: Cannot open file: / Fataler Fehler: Kann Datei nicht öffnen: %s", answer);
#endif
		}
		free(sOutputFileName);
	} else {
		fOut = stdout;
	}

	return ppMatrix;
}

double ** ReadMatrixFromFile(char * sFileName)
{
	char * sLocalFileName;
	if(sFileName == NULL) {
		sLocalFileName = (char *)malloc(sizeof(char) * 200);
		printf("Bitte geben Sie den Dateinamen an: ");
		fgets(sLocalFileName, 200, stdin);
		sLocalFileName[strlen(sLocalFileName) - 1] = '\0';
	} else {
		sLocalFileName = (char *)malloc(sizeof(char) * (strlen(sFileName) + 1));
		strcpy(sLocalFileName, sFileName);
	}
	
	FILE * fFile = fopen(sLocalFileName, "r");

	// stream überprüfen
	if(fFile == NULL) {
#ifdef _USE_WINDOWS_CODE
		printf("Fatal Error: Cannot open file: / Fataler Fehler: Kann Datei nicht \x94 \bffnen: %s", sLocalFileName);
#else
		printf("Fatal Error: Cannot open file: / Fataler Fehler: Kann Datei nicht öffnen: %s", sLocalFileName);
#endif
		return NULL;
	}
	free(sLocalFileName);

	fscanf(fFile, "%d;%d\n", &iColumns, &iRows);
	
	double ** ppMatrix = AllocateMatrixMemory(iColumns, iRows);

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
			for(int l = 0; l < 1000; l++) {
				var[l] = '\0';
			}
		}
	}

	fclose(fFile);

	return ppMatrix;
}

double ** ReadMatrixFromInput()
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

void PrintMatrix(double ** ppMatrix, int bPrintSMatrix, FILE * fOutputStream)
{
	if(bPrintSMatrix == TRUE) {
		fprintf(fOutputStream, "Print S-Matrix\n");
	} else {
		fprintf(fOutputStream, "Print Matrix\n");
	}
	for(int i = 0; i < iRows; i++) {
		int g;
		if(bPrintSMatrix) {
			g = iColumns - iRows;
		} else {
			g = 0;
		}
		int for_end;
		if(bPrintSMatrix || bNoSCalculation) {
			for_end = iColumns;
		} else {
			for_end = iColumns - iRows;
		}
		for( /* g schon gesetzt */; g < for_end /* for_end ist die endbedingung der for-schleife */; g++) {
			fprintf(fOutputStream, "%lf", ppMatrix[g][i]);
			fprintf(fOutputStream, ";"); // Trennzeichen ausgeben
		}
		fprintf(fOutputStream, "\n");
	}
	fprintf(fOutputStream, "\n");
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

void StepTwo(double ** ppMatrix, POSITION * posPivot)
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

	fprintf(fOut, "Füge Elementarmatrix an: P[%d,%d]\n", posPivot->iRow + 1 + iRowsOffset, 1 + iRowsOffset);

	posPivot->iRow = 0;
}

void StepThree(double ** ppMatrix, POSITION posPivot)
{
	double fElement = ppMatrix[posPivot.iColumn][0];

	if(fElement == 0) {
		printf("Fat(al) error... Error in previous steps. Terminate programm. / Fett(al)er Fehler... Fehler in vorherigen Schritten. Beende Programm.\n");
		//system("pause");
		WaitForKey();
		exit(-1);
	}

	for(int i = 0; i < iColumns; i++) {
		ppMatrix[i][0] *= (1.0f / fElement);
	}
	fprintf(fOut, "Füge Elementarmatrix an: M[%d](%lf)\n", 1 + iRowsOffset, 1.0f / fElement);
}

void StepFour(double ** ppMatrix, POSITION posPivot)
{
	for(int i = 1; i < iRows; i++) {
		double scale = ppMatrix[posPivot.iColumn][i];

		if(scale < 0.00000001l && scale > -0.00000001l) {
			continue; // mit nächster Zeile weitermachen
		}

		for(int g = 0; g < iColumns; g++) {
			ppMatrix[g][i] += - ppMatrix[g][0] * scale;
		}
		fprintf(fOut, "Füge Elementarmatrix an: G[%d,%d](%lf)\n", 1 + i + iRowsOffset, 1 + iRowsOffset, -scale);
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
//		iRowsOffset -= (iRowsSave - iRows);
//		iRows = iRowsSave;
//		iRowsOffset -= (iColumnsSave - iColumns);
//		iColumns = iColumnsSave;
		return pivots; // Fertig
	}
	
	StepTwo(ppSmallerMatrix, &posNewPivot);
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

void StepSix(double ** ppMatrix, POSITION_ARRAY pivots)
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
			fprintf(fOut, "Füge Elementarmatrix an: G[%d,%d](%lf)\n", 1 + i + iRowsOffset, 1 + iRowsOffset, -scale);
		}
	}
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

void WaitForKey()
{
	if(!bImmediateExit) {
#ifdef _USE_WINDOWS_CODE
		system("pause");
#else
		system("read -n1 -r");
#endif
	}
}

void CopyMatrix(double ** ppBuf, double ** ppIn, int iRowsToCopy, int iColumnsToCopy)
{
	for(int i = 0; i < iRowsToCopy; i++) {
		for(int g = 0; g < iColumnsToCopy; g++) {
			ppBuf[g][i] = ppIn[g][i];
		}
	}
}
