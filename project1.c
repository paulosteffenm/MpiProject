#include <stdio.h>
#include </usr/include/mpi/mpi.h>

main(int argc, char **argv)
{
  int ierr, numProcs, myId;

  ierr = MPI_Init(&argc, &argv);

  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  printf("Processo %d de %d Processos\n",
         myId, numProcs);

  ierr = MPI_Finalize();
}
