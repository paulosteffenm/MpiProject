
/* este programa soma todas linhas de um array. O processo principal
    manda uma parte deste array para os processos escravos que deve retornar
    a soma da sua porção.
   */

#include <stdio.h>
#include </usr/include/mpi/mpi.h>
#include <sys/time.h>

#define max_rows 100000
#define send_data_tag 2001
#define return_data_tag 2002

int array[max_rows];
int array2[max_rows];

main(int argc, char **argv)
{
  int sum, partial_sum;
  MPI_Status status;
  int myId, rootProcess, ierr, i, numLinhas, numProcs,
      outroId, numLinhasReceber, mediaLinhasProcessos,
      sender, numLinhasRcv, linhaInicio, linhaFim, numLinhasSend;

  // desta linha para baixo repete o programa para cada processo

  ierr = MPI_Init(&argc, &argv);

  rootProcess = 0;

  /*retorna o numero do processo e o numero total de processos*/

  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  ierr = MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  if (myId == rootProcess)
  {
    struct timeval t1, t2;
    double elapsedTime;
    gettimeofday(&t1, NULL);
    // quando o processo for 0 deve ser o principal, entao deve separar o array

    printf("Entre com o numero de numeros: \n");
    scanf("%d", &numLinhas);

    if (numLinhas > max_rows)
    {
      printf("Muitos numeros\n");
      exit(1);
    }

    mediaLinhasProcessos = numLinhas / numProcs;

    for (i = 0; i < numLinhas; i++)
    {
      array[i] = i + 1;
    }

    /* distribui um pouco do array para cada processo */

    for (outroId = 1; outroId < numProcs; outroId++)
    {
      linhaInicio = outroId * mediaLinhasProcessos + 1;
      linhaFim = (outroId + 1) * mediaLinhasProcessos;

      if ((numLinhas - linhaFim) < mediaLinhasProcessos)
      {
        linhaFim = numLinhas - 1;
      }

      numLinhasSend = linhaFim - linhaInicio + 1;

      ierr = MPI_Send(&numLinhasSend, 1, MPI_INT,
                      outroId, send_data_tag, MPI_COMM_WORLD);

      ierr = MPI_Send(&array[linhaInicio], numLinhasSend, MPI_INT,
                      outroId, send_data_tag, MPI_COMM_WORLD);
    }

    // realiza a soma dos numeros do processo principal

    sum = 0;
    for (i = 0; i < mediaLinhasProcessos + 1; i++)
    {
      sum += array[i];
    }

    printf("Soma calculada pelo processo principal: %d \n", sum);

    // recebe a soma dos processos escravos e soma na soma principal

    for (outroId = 1; outroId < numProcs; outroId++)
    {

      ierr = MPI_Recv(&partial_sum, 1, MPI_LONG, MPI_ANY_SOURCE,
                      return_data_tag, MPI_COMM_WORLD, &status);

      sender = status.MPI_SOURCE;

      printf("soma parcial %d retornado do processo %d\n", partial_sum, sender);

      sum += partial_sum;
    }

    printf("O total eh: %i\n", sum);
    gettimeofday(&t2, NULL);
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;    // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
    printf("%f ms.\n", elapsedTime);
  }

  else
  {

    // provavelmente um processo escravo, deve receber uma parte do array e retornar a soma

    ierr = MPI_Recv(&numLinhasReceber, 1, MPI_INT,
                    rootProcess, send_data_tag, MPI_COMM_WORLD, &status);

    ierr = MPI_Recv(&array2, numLinhasReceber, MPI_INT,
                    rootProcess, send_data_tag, MPI_COMM_WORLD, &status);

    numLinhasRcv = numLinhasReceber;

    partial_sum = 0;
    for (i = 0; i < numLinhasRcv; i++)
    {
      partial_sum += array2[i];
    }

    //manda a soma parcial para o processo principal

    ierr = MPI_Send(&partial_sum, 1, MPI_LONG, rootProcess,
                    return_data_tag, MPI_COMM_WORLD);
  }
  ierr = MPI_Finalize();
  return 0;
}
