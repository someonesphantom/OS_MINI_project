#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define num_threads 27
/*
	Initialize the array which worker threads can update to 1 if the
	corresponding region of the sudoku puzzle they were responsible
 	for is valid.
*/
int valid[num_threads] = {0};

// Struct that stores the data to be passed to threads
typedef struct {
	int row;
	int column;
} parameters;
int sudoku[9][9];
char file_name[100];
void ins();
void getsudoku();
void printsudo()
{
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			printf("\t %d",sudoku[i][j]);
		}
		printf("\n");
	}
}
void ins() {
	FILE *fptr;
  // integer variable
	int num;
  // open the file in write mode
	fptr = fopen(file_name, "w");

	if (fptr != NULL) {
		printf("File created successfully!\n");
	}
	else {
		printf("Failed to create the file.\n");
    // exit status for OS that an error occurred
		exit(0);
	}
	printf("Enter:\n 1.User Input\n2.Random value");
	int inp;
	scanf("%d",&inp);
	if(inp==1)
	{
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				scanf("%d", &num);
				if(num<1||num>10)
				{
					printf("Invalid Inputs.......Exiting Program");
					exit(0);
				}
				putw(num, fptr);
			}
		}
	}
	else
	{
		for(int i=0;i<9;i++)
		{
			for(int j=0;j<9;j++)
			{
				printf("\nRow:%d and Col:%d::",(i+1),(j+1));
				num=(rand()%(9 -1 + 1)) + 1;
				putw(num, fptr);
			}
		}

	}
	fclose(fptr);
}
void getsudoku()
{
	FILE *fptr;
	int num;
	fptr = fopen(file_name, "rb");
	int i=0;
	int j=0;
	//printf("Enter sudoku:\n");
	//for(int i=0;i<9;i++)
	//	for (int j = 0; j < 9; j++)
	//	scanf("%d",&sudoku[i][j]);
	while ( (num = getw(fptr)) != EOF )
	{
		sudoku[i][j]=num;
		if(j==8)
		{
			j=0;
			i++;
		}
		else
			j++;
	}
}
// Method that determines if numbers 1-9 only appear once in a column
void *isColumnValid(void* param) {
	// Confirm that parameters indicate a valid col subsection
	parameters *params = (parameters*) param;
	int row = params->row;
	int col = params->column;
	if (row != 0 || col > 8) {
		fprintf(stderr, "Invalid row or column for col subsection! row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}

	// Check if numbers 1-9 only appear once in the column
	int validityArray[9] = {0};
	int i;
	for (i = 0; i < 9; i++) {
		int num = sudoku[i][col];
		if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
			pthread_exit(NULL);
		} else {
			validityArray[num - 1] = 1;
		}
	}
	// If reached this point, col subsection is valid.
	valid[18 + col] = 1;
	pthread_exit(NULL);
}

// Method that determines if numbers 1-9 only appear once in a row
void *isRowValid(void* param) {
	// Confirm that parameters indicate a valid row subsection
	parameters *params = (parameters*) param;
	int row = params->row;
	int col = params->column;
	if (col != 0 || row > 8) {
		fprintf(stderr, "Invalid row or column for row subsection! row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}

	// Check if numbers 1-9 only appear once in the row
	int validityArray[9] = {0};
	int i;
	for (i = 0; i < 9; i++) {
		// If the corresponding index for the number is set to 1, and the number is encountered again,
		// the valid array will not be updated and the thread will exit.
		int num = sudoku[row][i];
		if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
			pthread_exit(NULL);
		} else {
			validityArray[num - 1] = 1;
		}
	}
	// If reached this point, row subsection is valid.
	valid[9 + row] = 1;
	pthread_exit(NULL);
}

// Method that determines if numbers 1-9 only appear once in a 3x3 subsection
void *is3x3Valid(void* param) {
	// Confirm that parameters indicate a valid 3x3 subsection
	parameters *params = (parameters*) param;
	int row = params->row;
	int col = params->column;
	if (row > 6 || row % 3 != 0 || col > 6 || col % 3 != 0) {
		fprintf(stderr, "Invalid row or column for subsection! row=%d, col=%d\n", row, col);
		pthread_exit(NULL);
	}
	int validityArray[9] = {0};
	int i, j;
	for (i = row; i < row + 3; i++) {
		for (j = col; j < col + 3; j++) {
			int num = sudoku[i][j];
			if (num < 1 || num > 9 || validityArray[num - 1] == 1) {
				pthread_exit(NULL);
			} else {
				validityArray[num - 1] = 1;
			}
		}
	}
	// If reached this point, 3x3 subsection is valid.
	valid[row + col/3] = 1; // Maps the subsection to an index in the first 8 indices of the valid array
	pthread_exit(NULL);
}

int main() {
	pthread_t threads[num_threads];

	int threadIndex = 0;
	int i,j,inp;
	printf("Select the following:\n1:New\n2:Play a game\nelse:Read from existing file\n");
	scanf("%d",&inp);
	// printf("Enter file_name:\n");
	// scanf("%s",file_name);
	if(inp==1)
	{
		ins();
	}
	else if(inp==2)
	{
		FILE * fptr1;
		FILE * fptr2;
		int num = 0, a1 = 0, num1;
		fptr1 = fopen("play_sudoku.bin","rb");
		printf("Given Incomplete Sudoku\n");
		for(int i=0;i<9;i++)
	  {
	    for(int j=0;j<9;j++)
	    {
	      fread(&num1,sizeof(int),1,fptr1);
	      printf("%d\t",num1);
	    }
	    printf("\n");
	  }
		printf("Enter your choice, where it's 0\n");
		fptr2 = fopen("a.bin","wb+");
		for(int x = 0;x < 9; x++)
		{
			for(int y = 0;y < 9; y++)
			{
				fread(&num,sizeof(int),1,fptr1);
				if(num == 0)
				{
					printf("Enter your number: ");
					scanf("%d",&a1);
					fwrite(&a1,sizeof(int),1,fptr2);
				}
				else
				{
					fwrite(&num,sizeof(int),1,fptr2);
				}
			}
		}
		strcpy(file_name, "a.bin");
		fclose(fptr1);
		fclose(fptr2);
	}
	getsudoku();
	// Create 9 threads for 9 3x3 subsections, 9 threads for 9 columns and 9 threads for 9 rows.
	// This will end up with a total of 27 threads.
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (i%3 == 0 && j%3 == 0) {
				parameters *data = (parameters *) malloc(sizeof(parameters));
				data->row = i;
				data->column = j;
				pthread_create(&threads[threadIndex++], NULL, is3x3Valid, data); // 3x3 subsection threads
			}
			if (i == 0) {
				parameters *columnData = (parameters *) malloc(sizeof(parameters));
				columnData->row = i;
				columnData->column = j;
				pthread_create(&threads[threadIndex++], NULL, isColumnValid, columnData);	// column threads
			}
			if (j == 0) {
				parameters *rowData = (parameters *) malloc(sizeof(parameters));
				rowData->row = i;
				rowData->column = j;
				pthread_create(&threads[threadIndex++], NULL, isRowValid, rowData); // row threads
			}
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);			// Wait for all threads to finish
	}
	printsudo();

	// If any of the entries in the valid array are 0, then the sudoku solution is invalid
	for (i = 0; i < num_threads; i++) {
		if (valid[i] == 0) {
			printf("Sudoku solution is invalid!\n");
			return EXIT_SUCCESS;
		}
	}
	printf("Sudoku solution is valid!\n");
	return EXIT_SUCCESS;
}
