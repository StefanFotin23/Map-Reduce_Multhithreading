#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

#define FILE_NAME_SIZE 50
#define VECTOR_SIZE 500

pthread_barrier_t barrier;
pthread_mutex_t mutex1, mutex2, mutex3, mutex4, mutex5;
int putere_2[VECTOR_SIZE], putere_3[VECTOR_SIZE], putere_4[VECTOR_SIZE], putere_5[VECTOR_SIZE], putere_6[VECTOR_SIZE], check[5];
int p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0;
char putere_2_out[FILE_NAME_SIZE], putere_3_out[FILE_NAME_SIZE], putere_4_out[FILE_NAME_SIZE], putere_5_out[FILE_NAME_SIZE];

struct args {
    int thread_id;
    int totalFilesNumber;
    char **inputFile;
    int *fileChecked;
    int putere_maxima;
    //pthread_barrier_t barrier;
    //pthread_mutex_t mutex;
};

int fast_exponentation(int a, int n) {
    int k = a * a;
    while (n - 2 > 0) {
        k = k * a;
        n--;
    }
    return k;
}

size_t number_of_bits(int n) {
    size_t bits = 0;
    while (n != 0) {
        ++bits;
        n >>= 1;
    }
    return bits;
}

//code inspired by a post on stack exchange and other internet sources
int is_power(int n) {
    if (n == 1) {
        return 1; //if it returns 1, it's the case for 1, so it matches every array of powers
    }
    if (n <= 0) {
        return 0; //if it returns 0, false
    }

    long lgn = number_of_bits(n) + 1;

    for (int b = 2; b < lgn; b++) {
        long lowa = 1, ab;
        long higha = 1 << (lgn / b + 1);

        while (lowa < higha - 1) {
            long mida = (lowa + higha) >> 1;
            ab = fast_exponentation(mida, b);
            if (ab > n) { higha = mida; }
            else if (ab < n) { lowa = mida; }
            else {
                if (b % 2 == 0) {
                    int sr = sqrt(mida);
                    if (sr * sr == mida) {
                        return b * 20 + b;
                    }
                }
                return b;
            }
        }
    }

    return 0;
}

void *map(void *arguments)
{
	struct args arg = *(struct args *)arguments;
    //pthread_mutex_t mutex = arg.mutex;
    int totalFilesNumber = arg.totalFilesNumber, ok;
    char **inputFile = arg.inputFile;
    //int thread_id = arg.thread_id;
    for (int i = 0; i < totalFilesNumber; i++) {
        ok = 0;
        pthread_mutex_lock(&mutex1);
        if (arg.fileChecked[i] == 0) {
            arg.fileChecked[i] = 1;
            ok = 1;
        }
        pthread_mutex_unlock(&mutex1);

        //execute the map part of the program on the "i" file
        if (ok == 1) {
            char nr_string[FILE_NAME_SIZE];
            int n = strlen(inputFile[i]);
            if (inputFile[i][n - 1] != 't') {
                inputFile[i][n - 1] = '\0';
            }
            
            FILE *f = fopen((inputFile[i]), "r");
            if (NULL == f) {
                printf("file can't be opened \n");
            }
            
            fgets(nr_string, FILE_NAME_SIZE, f);
            int nr = atoi(nr_string), number, aux, putere_maxima = arg.putere_maxima, done = 0, result;
            char number_string[FILE_NAME_SIZE];

            for (int j = 0; j < nr; j++) {
                fgets(number_string, FILE_NAME_SIZE, f);
                number = atoi(number_string);

                result = is_power(number);
                if (result == 2) {
                    pthread_mutex_lock(&mutex2);
                    putere_2[p2] = number;
                    p2++;
                    pthread_mutex_unlock(&mutex2);
                } else if (result == 3) {
                    pthread_mutex_lock(&mutex3);
                    putere_3[p3] = number;
                    p3++;
                    pthread_mutex_unlock(&mutex3);
                } else if (result == 42) {
                    pthread_mutex_lock(&mutex2);
                    putere_2[p2] = number;
                    p2++;
                    pthread_mutex_unlock(&mutex2);

                    pthread_mutex_lock(&mutex4);
                    putere_4[p4] = number;
                    p4++;
                    pthread_mutex_unlock(&mutex4);
                } else if (result == 5) {
                    pthread_mutex_lock(&mutex5);
                    putere_5[p5] = number;
                    p5++;
                    pthread_mutex_unlock(&mutex5);
                } else if (result == 1) {
                    pthread_mutex_lock(&mutex2);
                    putere_2[p2] = number;
                    p2++;
                    pthread_mutex_unlock(&mutex2);

                    pthread_mutex_lock(&mutex3);
                    putere_3[p3] = number;
                    p3++;
                    pthread_mutex_unlock(&mutex3);

                    pthread_mutex_lock(&mutex4);
                    putere_4[p4] = number;
                    p4++;
                    pthread_mutex_unlock(&mutex4);

                    pthread_mutex_lock(&mutex5);
                    putere_5[p5] = number;
                    p5++;
                    pthread_mutex_unlock(&mutex5);
                }
                // done = 0;
                // for (int k = 2; k <= sqrt(number); k++) {
                //     aux = k * k;
                //     if (aux > number || done == 1) { //for 2...sqrt(number) + 1
                //         break;
                //     }
                //     for (int l = 0; l <= putere_maxima; l++) {
                //         if (aux == number) {
                //             //adaug in lista potrivita (number = k la puterea l + 2)
                //             done = 1;
                //             break;
                //         } else {
                //             aux = aux * k;
                //         }
                //     }
                // }
            }
            fclose(f);
        }
    }
    //pthread_barrier_wait(&(arg.barrier));
    pthread_barrier_wait(&barrier);
	pthread_exit(NULL);
}

int compar(const void *l, const void *r) {
    const int* lhs = l;
    const int* rhs = r;
    if(*lhs < *rhs) return -1; // left side is less than right side: -1
    if(*lhs > *rhs) return 1;  // left side is greater than right side: 1
    return 0;                  // they are equal: 0
}

int distinct(int arr[], int N){
    // sort the numbers
    qsort(arr, N, sizeof *arr, compar);

    int count = 0;
    for(int i=0; i < N; ++count) {
        int curr = arr[i];
        // skip all numbers equal to curr as shown in the graph above:
        for(++i; i < N; ++i) {
            if(arr[i] != curr) break;
        }
    }
    return count;
}

void *reduce(void *arguments)
{   
    struct args arg = *(struct args *)arguments;
    //pthread_barrier_wait(&(arg.barrier));
    pthread_barrier_wait(&barrier);
    int ok;

    //for iterates through putere2, putere3, putere4, putere5
    for (int i = 0; i < 4; i++) {
        ok = 0;
        pthread_mutex_lock(&mutex1);
        if (check[i] == 0) {
            check[i] = 1;
            ok = 1;
        }
        pthread_mutex_unlock(&mutex1);

        if (ok == 1) {
            if (i == 0) {
                //case for putere2
                strcpy(putere_2_out, "out2.txt");
                FILE *f2 = fopen(putere_2_out, "w");
                if (f2 == NULL) {
                    printf("Open file error!\n");
                }
                p2 = distinct(putere_2, p2);
                fprintf(f2, "%d", p2);
                fclose(f2);
            } else if (i == 1) {
                //case for putere3
                strcpy(putere_3_out, "out3.txt");
                FILE *f3 = fopen(putere_3_out, "w");
                if (f3 == NULL) {
                    printf("Open file error!\n");
                }
                p3 = distinct(putere_3, p3);
                fprintf(f3, "%d", p3);
                fclose(f3);
            } else if (i == 2) {
                //case for putere4
                strcpy(putere_4_out, "out4.txt");
                FILE *f4 = fopen(putere_4_out, "w");
                if (f4 == NULL) {
                    printf("Open file error!\n");
                }
                p4 = distinct(putere_4, p4);
                fprintf(f4, "%d", p4);
                fclose(f4);
            } else if (i == 3) {
                //case for putere5
                strcpy(putere_5_out, "out5.txt");
                FILE *f5 = fopen(putere_5_out, "w");
                if (f5 == NULL) {
                    printf("Open file error!\n");
                }
                p5 = distinct(putere_5, p5);
                fprintf(f5, "%d", p5);
                fclose(f5);
            }
        }
    }
    
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int M, R, P, noOfFiles;
    char noOfFiles_String[FILE_NAME_SIZE];
    //pthread_barrier_t barrier;
    //pthread_mutex_t mutex;

    // getting args
    if(argc < 4) {
		printf("Numar insuficient de parametri: ./tema1 M R FileName\n");
		exit(1);
	}
    M = atoi(argv[1]);
	R = atoi(argv[2]);
    char* inputFileName = argv[3];
    P = M + R;

    FILE *input = fopen(inputFileName, "r");
    if (NULL == input) {
        printf("file can't be opened \n");
    }
	
    pthread_barrier_init(&barrier, NULL, P);
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_mutex_init(&mutex3, NULL);
    pthread_mutex_init(&mutex4, NULL);
    pthread_mutex_init(&mutex5, NULL);

	pthread_t tid[P];
    struct args arg;

    if (fgets(noOfFiles_String, FILE_NAME_SIZE, input) != NULL) {
        noOfFiles = atoi(noOfFiles_String);
    }
    char **inputFiles = (char**) calloc(noOfFiles, sizeof(char*)); // vector of files (name as string)
    int *fileChecked = (int*) calloc(noOfFiles, sizeof(int));

    for (int j = 0; j < noOfFiles; j++) {
        inputFiles[j] = calloc(FILE_NAME_SIZE, sizeof(char));
        fgets(inputFiles[j], FILE_NAME_SIZE, input);
        fileChecked[j] = 0;
    }
    
    arg.totalFilesNumber = noOfFiles;
    arg.inputFile = inputFiles;
    arg.fileChecked = fileChecked;
    arg.putere_maxima = R + 1;
	// se creeaza thread-urile
	for (int i = 0; i < P; i++) {
        //arg[i].mutex = mutex;
        //arg[i].barrier = barrier;
        arg.thread_id = i;

        if (i < M) {
		    pthread_create(&tid[i], NULL, map, &arg);
        }
        if (i >= M) {
            pthread_create(&tid[i], NULL, reduce, &arg);
        }
	}

	// se asteapta thread-urile
	for (int i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	pthread_barrier_destroy(&barrier);
    fclose(input);
	return 0;
}
