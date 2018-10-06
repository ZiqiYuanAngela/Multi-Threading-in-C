/*
 ============================================================================
 Name        : CS149_HW2.c
 Author      : Ziqi_Yuan
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define number_threads 11
#define grid_size 9

typedef struct parameters{
		int row;
		int column;
        int puzzle_number;
        int id;
	}parameters;

int answer=1;//Sudoku is valid in default

int result[number_threads]={0};

pthread_t workerT[number_threads];//11 worker threads in total

struct parameters *data[number_threads];

	int puzzle1[grid_size][grid_size]={
						   {4,1,9,3,7,6,5,8,2},
				           {3,6,7,8,5,2,1,4,9},
						   {2,8,5,1,9,4,3,7,6},
						   {6,4,1,9,3,7,8,2,5},
						   {9,3,2,4,8,5,6,1,7},
						   {5,7,8,2,6,1,4,9,3},
						   {8,5,6,7,4,9,2,3,1},
						   {1,9,4,5,2,3,7,6,8},
						   {7,2,3,6,1,8,9,5,4}
		};

	int puzzle2[grid_size][grid_size]={
			               {7,3,9,8,4,6,1,2,5},
				           {4,1,2,9,7,5,8,6,3},
						   {8,0,5,2,3,1,9,4,7},
						   {5,4,7,6,9,3,2,1,8},
						   {3,9,1,7,8,2,4,5,6},
						   {2,8,6,5,1,4,3,7,9},
						   {9,2,8,4,6,7,5,3,1},
						   {1,7,4,3,5,9,1,8,2},
						   {6,5,3,1,2,8,7,9,4}
		};




	void *check_allRowsPuzzle(void *params){
		parameters *pointer=(parameters *)params;
		int startR=pointer->row;
		int startC=pointer->column;
		int puzzle[grid_size][grid_size];
		if(pointer->puzzle_number==1){
			memcpy(puzzle,puzzle1,sizeof puzzle);
		}else{
			memcpy(puzzle,puzzle2,sizeof puzzle);
		}
		int array[9]={0};
		for(int i=startR;i<grid_size;i++){
			memset(array,0,sizeof(array));
			for(int j=startC;j<grid_size;j++){
				int integer=puzzle[i][j];
				if(integer>0){
				array[integer-1]=1;}
			}

		for(int p=0;p<9;p++){
				if(array[p]==0){
		            result[pointer->id]=0;
                     answer=0;
		            for(int d=0;d<number_threads;d++){
		            pthread_cancel(workerT[d]);
		            	}

	                pthread_exit(NULL);
				}
			}
		}
		result[pointer->id]=1;

	}



	void *check_allColumnsPuzzle(void *params){

		parameters *pointer=(parameters *)params;
			int startR=pointer->row;
			int startC=pointer->column;
			int puzzle[grid_size][grid_size];
			if(pointer->puzzle_number==1){
				memcpy(puzzle,puzzle1,sizeof puzzle);
			}else{
				memcpy(puzzle,puzzle2,sizeof puzzle);
			}
			int array[9]={0};
			for(int i=startC;i<grid_size;i++){
				memset(array,0,sizeof(array));
				for(int j=startR;j<grid_size;j++){
					int integer=puzzle[i][j];
					if(integer>0){
					array[integer-1]=1;}
				}

				for(int p=0;p<9;p++){
					if(array[p]==0){
						result[pointer->id]=0;
						answer=0;
						pthread_exit(NULL);
					}
				}
			}
			result[pointer->id]=1;

	}



	void *check_SquaresPuzzle(void *params){
		parameters *pointer=(parameters *)params;
		int startR=pointer->row;
				int startC=pointer->column;
				int puzzle[grid_size][grid_size];
				if(pointer->puzzle_number==1){
					memcpy(puzzle,puzzle1,sizeof puzzle);

				}else{
					memcpy(puzzle,puzzle2,sizeof puzzle);
				}
				int array[9]={0};
				for(int i=startC;i<startC+3;i++){
					for(int j=startR;j<startR+3;j++){
						int integer=puzzle[i][j];
						if(integer>0){
						array[integer-1]=1;}
					}
				}

				for(int p=0;p<9;p++){
					if(array[p]==0){
						result[pointer->id]=0;
                        answer=0;

						pthread_exit(NULL);
					}
				}
				result[pointer->id]=1;
	}



	void sudoku(int puzzle){
	for(int b=0;b<number_threads;b++){
		data[b]->puzzle_number=puzzle;
	}

	//Create threads

	     for(int k=0;k<9;k++){
	    	 pthread_create(&workerT[k], NULL,check_SquaresPuzzle, data[k]);
	     }
	     pthread_create(&workerT[9],NULL,check_allRowsPuzzle,data[9]);
	     pthread_create(&workerT[10],NULL,check_allColumnsPuzzle,data[10]);

	     //Join threads
	     for(int h=0;h<number_threads;h++){
	    	 pthread_join(workerT[h],NULL);
	     }

	     //Check validity of the entire puzzle



	      if(answer){
	    	  printf("Puzzle %d is valid.\n",puzzle);
	      }
	      else{
	    	  printf("Puzzle %d is not valid.\n",puzzle);
	      }

	     printf("\n");

}




int main(void) {


    printf("CS149 Sudoku from Ziqi Yuan.\n");
    printf("\n");

    //struct parameters *data[11];
    int i=0;
    int j=0;
    int index=0;
	while(i<=6){//Initialize struc elements

		while(j<=6){
		data[index]=(parameters *)malloc(sizeof(parameters));
		data[index]->row=i;
		data[index]->column=j;
		//data[index]->puzzle_number=1;
		data[index]->id=index;
		index++;
		j=j+3;
		}
        i=i+3;
		j=0;


	}
			data[9]=(parameters *)malloc(sizeof(parameters));
			data[9]->row=0;
			data[9]->column=0;
			//data[9]->puzzle_number=1;
			data[9]->id=9;

			data[10]=(parameters *)malloc(sizeof(parameters));
			data[10]->row=0;
		    data[10]->column=0;
		    //data[10]->puzzle_number=1;
		    data[10]->id=10;


		    //Print Puzzle One
		    for(int q=0;q<grid_size;q++){
		        	for(int w=0;w<grid_size;w++){
		        		printf("%d",puzzle1[q][w]);
		        	}
		        	printf("\n");
		        }
		        printf("\n");

		        sudoku(1);//Run Sudoku checker for Puzzle 1
		        memset(result,0,sizeof(result));//Re-set result array get ready for 2nd Sudoku check





		        //Print Puzzle Two
            for(int f=0;f<grid_size;f++){
            	for(int g=0;g<grid_size;g++){
            		printf("%d",puzzle2[f][g]);
            	}
            	printf("\n");
            }
            printf("\n");

            sudoku(2);//Run Sudoku checker for Puzzle 2


 //At the end, free memory
     for(int y=0;y<11;y++){
      free(data[y]);}


     return EXIT_SUCCESS;
}


