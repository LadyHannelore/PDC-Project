#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include <stdlib.h>

#define MAXCHAR 100
#define SIZE 10000
bool found=false;
int num_threads;
double temp;
double found_t=0;
int result=-1;
struct csv
{
    int   id;
    char  fname[50];
    char  lname[50];
    float  salary;
}A[SIZE];


//struct MyMax {
  // int max;
   //int index;
//};
const char* delim = ",";
void readCSV(){

    FILE *fp;
    char row[MAXCHAR];
    char *token;
    int i=0;
    

    fp = fopen("file.csv","r");


    while (feof(fp) != true)
    {
        fgets(row, MAXCHAR, fp);
       
        int id = atoi(strtok(row, delim));
        char *fname = strtok(NULL, delim);
        //printf("%s",fname);
        char *lname  = strtok(NULL, delim);
        //strcpy(fname, strtok(row , ","));
        //strcpy(lname, strtok(NULL, ","));
        float sal = atoi(strtok(NULL, delim));
        A[i].id=id;
        A[i].salary=sal;
        strcpy(A[i].fname, fname);
        strcpy(A[i].lname, lname);
        //if(i==2 || i==3)
           // printf("%d\t%s\t%s\t%f\n",A[i].id,A[i].fname,A[i].lname,A[i].salary);
        i++;


    }
    

    return;

}

int searchnum(int value,int s,int e){
//printf("start:%d \t end:%d\n",s,e);
for(int i = s; i < e; i++)
    {
        //printf("%d = %d\n",A[i].id,value);
        if(A[i].id ==value)
        {
            int index=i;
            //found=true;
            //printf("Found at:%d",i);
            
            temp= omp_get_wtime();
            
            return index;
        }
        else
        {
            continue;
        }
        
    }
    return -1;
}


int searchsal(float value,int s,int e){
    //printf("\nstart:%d \t end:%d and value = %f",s,e,value);
    for(int i = s; i < e; i++){
        //printf("\nValue at %d is %f",i,A[i].salary);
            if(A[i].salary == value)
            {
                found=true;
                return i;
            }
            else
            {
                continue;
            }
    }
    return -1;
}


int searchstr(char *str,int colnum,int start,int end){
    char finding[20];
    strcpy(finding,str);
    //printf("Found %s in-%d\n", finding,colnum);
    if (colnum==1)
    {  
        char source[20];
        //strcpy(source,A[start].fname);  
    for (int i = start; i < end; i++)
    {
        strcpy(source,A[i].fname);
        if(strcmp(source, finding) == 0)
        {
            //found=1;
            return i;
            printf("Found %s in-%d\n", finding,i);
        }        
    }
    }
    
    else if (colnum == 0)
    {
        for (int i = start; i < end; i++)
    { 
        char source[20];
        //strcpy(source,A[start].lname);  
    for (int i = start; i < end; i++)
    {
        strcpy(source,A[i].lname);
        if(strcmp(source, finding) == 0)
        {
            //found=1;
            return i;
            printf("Found %s in-%d\n", finding,i);
        }        
    }
    }
    }
    return -1;
    
}

void parallelize(int is,char *f,char *l,float s){
    //printf("%d,%s,%s,%f",is,f,l,s);
    int result=-1;
    double start_t, end_t, total_time;
    //found=false;


    printf("\n****** Now beginning Parallel work with OpenMP ******\n\n");

    printf("Starting Parallel search...\n");

    start_t = omp_get_wtime();

        int chunkSize = SIZE / num_threads;
        int banana=0,start=0,end=0,answer=0;
        //#pragma omp parallel reduction(maximo:result)
        #pragma omp parallel 
        {
                //struct MyMax myMaxStruct;
                //myMaxStruct.max = 0;
                //myMaxStruct.index = 0;   
                //#pragma omp parallel for reduction(maximo : myMaxStruct) 
                #pragma omp parallel for
                    {
                        for (int i = 0; i < num_threads-1; i++) {
                       
                        
                        start = i* chunkSize;
                        end = start + chunkSize - 1;
                        if ((start+chunkSize)>SIZE)
                        {
                            banana=end;
                        }                      
                        if (is !=0 && f==NULL && l==NULL && s == 0)
                        {
                            result=searchnum(is,start,end);
                            if (result!=-1)
                                answer=result;
                            //printf("%d\t%d\n",answer,result);
                            
                            
                            //printf("%d\n",result);

                        }
                        else if (is ==0 && f!=NULL && l==NULL && s == 0)
                        {
                            result=searchstr(f,1,start,end);
                            if (result!=-1)
                                answer=result;
                        }
                            else if (is ==0 && f==NULL && l!=NULL && s == 0)
                        {
                            result=searchstr(l,0,start,end);
                                if (result!=-1)
                                answer=result;
                        }
                            else if (is ==0 && f==NULL && l==NULL && s != 0)
                        {
                           // printf("%d\t%d\n",answer,result);
                            result=searchsal(s,start,end);
                            if (result!=-1)
                                answer=result;
                               // printf("%d\t%d\n",answer,result);
                        }
                    }
                }
            if (result == -1)
            {
                    start = banana;
                    end = SIZE - 1;
                #pragma omp single
                {
                        if (is !=0 && f==NULL && l==NULL && s == 0)
                        {
                            result=searchnum(is,start,end);
                            if (result!=-1 && answer ==-1)
                            answer=result;
                        }
                        else if (is ==0 && f!=NULL && l==NULL && s == 0)
                        {
                            result=searchstr(f,1,start,end);
                            if (result!=-1 && answer ==-1)
                                answer=result;
                        }
                            else if (is ==0 && f==NULL && l!=NULL && s == 0)
                        {
                            result=searchstr(l,0,start,end);
                            if (result!=-1 && answer ==-1)
                                answer=result;
                        }
                            else if (is ==0 && f==NULL && l==NULL && s != 0)
                        {
                            result=searchnum(s,start,end);
                            if (result!=-1 && answer ==-1)
                                answer=result;
                        }
                }
            }    
    }

    end_t = omp_get_wtime();
    total_time = end_t - start_t;
    found_t = temp - start_t;
    //double idle=total_time-found_t;


    printf("Work took %f seconds\n", total_time);
    //printf("Finding took %f seconds\n", found_t);

    // Print results of  search
    if (answer != -1)
    {
        printf("found! At index %d\n", answer);
    }
    else
    {
        printf("not found\n");
    }
    printf("\n");
}