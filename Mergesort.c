#include <stdio.h>
#include "Mergesort.h"
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#define SHM_MODE 0600

/** @brief Merge the right and left halves together to become a sorted array.
 *  @param Address to the first element in left half.
 *  @param The size of left half array.
 *  @param Address to the first element in right half.
 *  @param The size of right half array.
 *  @return void.
 */
void Merge(int  *left, int sizeLeft,int  *right, int sizeRight){
  //Get the total size of left and right half narray.
  int totalSize=sizeLeft+sizeRight;
//Allocate memory to tempory story sorted array.
  int *temp= (int *) malloc(sizeof(int)*totalSize);
  int a=0;
  int b=0;
  int c=0;
  int f;
//To compare the values in left and right half and sort them.
  while (a<sizeLeft && b<sizeRight){
    if(left[a] < right[b]){
      temp[c]=left[a];
      a+=1;
      }
    else{
      temp[c]=right[b];
      b+=1;
    }
    c+=1;
  }
// If left array have elements left, put in the sorted array.
  while (a<sizeLeft){
     temp[c]=left[a];
     a+=1;
     c+=1;
  }
// If right array have elements left, put in the sorted array.
  while (b<sizeRight){
    temp[c]=right[b];
    b+=1;
    c+=1;
  }
//Copy the sorted array from tempory address to shared memory.
  memcpy(left,temp,sizeof(int)*totalSize);
//Free the tempory memory.
   free(temp);
 
  
}

/** @brief MergeSort function to recursively split the number array to lift half and right half.
*   Call fork functions to create children processes and call merge function to merge them together.
 *  @param The size of number array.
 *  @param The addres of the first element in the number array.
 *  @return void.
 */
void MergeSort(int *ToBeSort, int size){

// Base case
  if (size<=1){
    return;
  }
  else{
//To split the ToBeSort array, to know the size for left half and right half.
    int middlePoint=size/2;
    int rightSize=size-middlePoint;
    int *left = ToBeSort;
    int *right= ToBeSort+middlePoint;
//Call fork to create child process.
    pid_t pidLeft;
    pidLeft=fork();
//Reprot an error if fork didn't successfully created.
    if(pidLeft<0){
      perror(" fork error");
      exit(1);
    }
//If in child process, call MergeSort with a paramater of left half.
    else if (pidLeft==0){
      MergeSort(left,middlePoint);
      exit(0);
    }
//If in parent process, call fork again to create a child process for the right half.
    else{
      
      pid_t pidRight;
      pidRight=fork();
//Report error if fork fails.
      if(pidRight<0){
        perror(" fork error");
        exit(1);
      }
//If in child process, call MergeSort with a paramater of right half.
      else if (pidRight==0){
        MergeSort(right,rightSize);
        exit(0);
      }
// If in parent process, wait until both process finished, call merge function to merge them together.
      else{
//Wait for both child process to finish.
	if (waitpid(pidLeft,NULL,0) != pidLeft){
		perror("waitpid error");
	}
	if (waitpid(pidRight,NULL,0) != pidRight){
		perror("waitpid error");
	}
        Merge(left,middlePoint,right,rightSize);
      }
      
    }

  }
  
}
/** @brief Main function to get input file name, read file and  call mergesort
*          and print the sorted numbers.
 *  @param number of command line arguments.
 *  @param The address to the inputed file name.
 *  @return 0.
 */
int main(int argc, char *argv[])
{
//Get the input file name.
  int length=10;
  printf("\n");
  printf("Sorting file : %s\n", argv[1]);
  int index=0;
//Open the input file.
  FILE *fp = fopen(argv[1], "r");
  if (!fp) { 
    perror("encountered fopen error");
    exit(10);
  }
//Allocate memory to store numbers in file.
  int *numbers = (int *) malloc(sizeof(int)*length);
//Read numbers from the input file
  while (fscanf(fp, "%d", &(numbers[index]))==1){
    if (index==(length-1)){
      length=length*2;
      numbers = (int *) realloc(numbers,sizeof(int)*length);
    }
     index+=1;
  }
// Print the number of elements read.
  printf("%d elements read\n",index);
  printf("\n");

//Print the orginal numbers in the input file and put them in a shared memory.
  printf("Input Numbers:\n");
  int size=index*sizeof(int);
  int shmid;
  int *shmptr;
  if ((shmid=shmget(IPC_PRIVATE, size , SHM_MODE))<0){
    perror("shmget error");
    exit(1);
  }
  if ((shmptr=shmat(shmid,0,0))==(void *)-1){
    perror("shmat error");
    exit(1);
  }
    
  int i;
  for (i=0;i<index;i++){
    shmptr[i]=numbers[i];
    printf("%d ",shmptr[i]);
  }
  free(numbers);
  
//Call mergesort function and print sorted numbers.
  printf("\n");
  printf("\n");
  printf("Sorted Numbers:\n");
  if (fclose(fp)) {
    perror("encountered fclose error");
    exit(10);
  }


  MergeSort(shmptr,index);
  for (i=0;i<index;i++){
    printf("%d ",shmptr[i]);
  }
  printf("\n");
  printf("\n");
  if (shmctl(shmid, IPC_RMID,0)<0){
    perror("shmctl error");
    exit(10);
  } 
   return 0;
}
