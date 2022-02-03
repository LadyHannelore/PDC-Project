#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/time.h> // High resolution timer
#include <omp.h>
#include "csv.h"

const int ARR_SIZE = 2000000; // Add 200,000,000 elements into array
int randomNums[ARR_SIZE];       // Array of random integers
int parallel_search_found = 0;
double omp_get_wtime(void);

// High resolution timer
inline uint64_t rdtsc()
{
    uint32_t lo, hi;
    __asm__ __volatile__(
        "xorl %%eax, %%eax\n"
        "cpuid\n"
        "rdtsc\n"
        : "=a"(lo), "=d"(hi)
        :
        : "%ebx", "%ecx");
    return (uint64_t)hi << 32 | lo;
}

void readInputFile()
{
    FILE *inputFile;
    inputFile = fopen("input1.txt", "r");

    int i;

    if (inputFile == NULL)
    {
        printf("Error Reading File input1.txt\n");
        exit(0);
    }

    printf("Populating randomNums[] with data from input1.txt...\n");
    printf("(This may take a while)\n");
    // Place file contents into array randomNums[]
    for (i = 0; i < ARR_SIZE; i++)
    {
        fscanf(inputFile, "%d,", &randomNums[i]);
    }

    // // Print a few values from array to verify elements
    // for (int i = 0; i < 4; i++) {
    //     printf("%d\n", randomNums[i]);
    // }

    printf("Finished inserting %d elements into randomNums[]\n", ARR_SIZE);

    fclose(inputFile);

    printf("\n");
}

int linearSearch_serial(int searchVal)
{
    int result=-1;
    for (int i = 0; i < ARR_SIZE; i++){
        result=i;
        if (randomNums[i] == searchVal)
            return result+1;
    }
    return -1;
}


int linearSearch_parallel(int start,int max,int searchVal)
{

    for (int i=start; i < max; i++){
        if (found==true)
        {
            return -1;
        }
        
        if (randomNums[i] == searchVal){
            found=true;
            found_t = omp_get_wtime();
            return i++;
        }
    }
    return -1;
}

void parallel_work(int searchVal, int num_threads)
{
    int result=-1;
    double start_t, end_t, total_time;


    printf("\n****** Now beginning Parallel work with OpenMP ******\n\n");

    printf("Starting Linear search...\n");

    start_t = omp_get_wtime();

        int chunkSize = ARR_SIZE / num_threads;
        int banana=0,start,end;
        #pragma omp parallel reduction(max:result)
        {   
            
                {
                #pragma omp parallel for reduction(max : result,banana)
                    {
                        for (int i = 0; i < num_threads-1; i++) {
                        if (found == true)
                        {
                           continue;
                        }
                        
                        int start = i* chunkSize;
                        int end = start + chunkSize - 1;
                        banana=end;
                        result=linearSearch_parallel(start, end ,searchVal);
                        }
                    }
                }
            if (result == -1)
            {
                #pragma omp single
                {
                    start = banana;
                    end = ARR_SIZE - 1;
                    result=linearSearch_parallel(start, end,searchVal);
                }
            }
        }


    end_t = omp_get_wtime();
    total_time = end_t - start_t;

    printf("Work took %f seconds\n", total_time);
    printf("Finding took %f seconds\n", found_t);

    // Print results of serial Linear search
    if (result != -1)
    {
        result=searchVal+1;
        printf("Element %d found! At index %d\n", searchVal, result);
    }
    else
    {
        printf("Element %d not found\n", searchVal);
    }
    printf("\n");
}

void serial_work(int searchVal)
{
    int result;
    double start, end, total_time;

    printf("\n****** Now beginning Serial work ******\n\n");

    printf("Starting Linear search...\n");

    start = omp_get_wtime();
    // Perform serial linear search with timing
    result = linearSearch_serial(searchVal); // linear search here

    end = omp_get_wtime();
    total_time = end - start;

    printf("Work took %f seconds\n", total_time);

    // Print results of serial linear search
    if (result != -1)
    {
        printf("Element %d found! At index %d\n", searchVal, result);
    }
    else
    {
        printf("Element %d not found\n", searchVal);
    }
    printf("\n");
}

//#pragma omp declare reduction(maximo : struct MyMax : omp_out = omp_in.max > omp_out.max ? omp_in : omp_out)

int main(int argc, char *argv[])
{
    //int searchVal;
    //printf("Enter value to search for: \n");
    //scanf("%d", &searchVal);

    printf("How many threads to run on: \n");
    scanf("%d", &num_threads);
    //input.txt numbers
    // Read contents of input file and populate array
    //readInputFile();

    // Perform all serial work
    //serial_work(searchVal);

    // Rewrite contents to array for consistency purposes
    //readInputFile();

    // Perform all parallelized work
    //parallel_work(searchVal, num_threads);

    readCSV();
    
    char operator;
    printf("\nEnter s for salary,i for id,f for fname and l for lname:");
    scanf(" %c",&operator);
    switch (operator)
    {
    case 'i': ;
        int idf;
        printf("Enter Search Value:\t");
        scanf("%d",&idf);
        parallelize(idf,NULL,NULL,0);
        break;
    case 's': ;
        float salf;
        printf("Enter Search Value:\t");
        scanf("%f",&salf);
        parallelize(0,NULL,NULL,salf);
        break;
    case 'f': ;
        char f[20];
        printf("Enter Search Value:\t");
        scanf("%s",f);
        parallelize(0,f,NULL,0);
        break;
    case 'l': ;
        char l[20];
        printf("Enter Search Value:\t");
        scanf("%s",l);
        parallelize(0,NULL,l,0);
        break;
    
    default:
        break;
    }
    
    
    return 0;
}
