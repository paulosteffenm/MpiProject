#include <stdio.h>
#include </usr/include/mpi/mpi.h>

int average(int num)
{
  return (num / 3);
}

int sumArray(int array[])
{
  int sum = 0;
  for (int i = 0; i != 3; i++)
  {
    sum += array[i];
  }

  return sum;
}

int main(int argc, char **argv)
{

  int ierr, numProcs, myId, sendMsg, recvMsg;

  int array[3] = {10, 8, 6};

  ierr = MPI_Init(&argc, &argv);

  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  if (myId == 0)
  {
    sendMsg = sumArray(array);
    MPI_Send(&sendMsg, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
    printf("Total: %d\n", sendMsg);
  }
  else
  {
    MPI_Recv(&recvMsg, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    int media = average(recvMsg);
    printf("Media: %d\n", media);
  }

  ierr = MPI_Finalize();
  return 0;
}
