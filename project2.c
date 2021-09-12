#include <stdio.h>
#include </usr/include/mpi/mpi.h>

int sumNumbers(int array[])
{
  int sum = 0;
  for (int i = 0; i != 5; i++)
  {
    sum += array[i];
  }

  return sum;
}

int subNumbers(int array[])
{
  int sum = 0;
  for (int i = 0; i != 5; i++)
  {
    sum -= array[i];
  }

  return sum;
}

int main(int argc, char **argv)
{

  int ierr, numProcs, myId;

  int array[5] = {1, 2, 3, 4, 5};
  int sumArray, subArray;

  ierr = MPI_Init(&argc, &argv);

  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  if (myId == 0)
  {
    sumArray = sumNumbers(array);
    printf("Sum: %d\n", sumArray);
  }
  else
  {
    subArray = subNumbers(array);
    printf("Sub: %d\n", subArray);
  }

  ierr = MPI_Finalize();
  return 0;
}
