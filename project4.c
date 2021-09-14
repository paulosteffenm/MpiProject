//#include <mpi.h>
#include </usr/include/mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define maximoDeLinhas 100000
#define send_data_tag 2001
#define return_data_tag 2002

// Temporary array for slave process
int array[maximoDeLinhas];
int array2[maximoDeLinhas];

int main(int argc, char *argv[])
{

    int idProcesso, numeroDeProcessos,
        linhasPorProcesso,
        qtdeRecebida, numLinhas;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &idProcesso);
    MPI_Comm_size(MPI_COMM_WORLD, &numeroDeProcessos);

    // VERIFICA SE É O PROCESSO PRINCIPAL
    if (idProcesso == 0)
    {
        struct timeval t1, t2;
        double elapsedTime;
        gettimeofday(&t1, NULL);

        printf("Digite o numero de linhas a serem somadas: \n");
        scanf("%d", &numLinhas);

        // PREENCHENDO O ARRAY COM VALORES GERADOS A PARTIR DE UM I QUE SE SOMA EM LOOP
        int i;
        for (i = 0; i < numLinhas; i++)
        {
            array[i] = i + 1;
        }

        // ADICIONANDO UM LIMITE PARA A QUANTIADE DE LINHAS DO PROCESSO
        if (numLinhas > maximoDeLinhas)
        {
            printf("Quantidade de linhas excede o limite\n");
            exit(1);
        }

        int index, j;
        // SEPARANDO UMA QUANTIDADE DE LINHAS A SER DIVIDIDA PARA CADA PROCESSO
        linhasPorProcesso = numLinhas / numeroDeProcessos;

        // SE A QUANTIDADE DE PROCESSOS FOR MAIOR QUE 1 UTILIZA-SE DOS PROCESSOS SECUNDARIOS PARA REALIZAR SOMAS PARCIAIS
        if (numeroDeProcessos > 1)
        {
            // DECLARAÇÃO DA VARIAVEL J E AQUI PARA CADA LOOP DO FOR ELA ASSUME O VALOR QUE SERIA CORRESPONTE A UM ID DE UM PROCESSO
            for (j = 1; j < numeroDeProcessos - 1; j++)
            {
                index = j * linhasPorProcesso;

                // ENVIANDO A QUANTIDADE DE LINHAS QUE CADA PROCESSO FICA RESPONSÁVEL
                MPI_Send(&linhasPorProcesso,
                         1, MPI_INT, j, 0,
                         MPI_COMM_WORLD);
                // ENVIANDO A PARTIR DE QUAL POSIÇÃO O PROCESSO DEVE COMEÇAR A SOMAR
                MPI_Send(&array[index],
                         linhasPorProcesso,
                         MPI_INT, j, 0,
                         MPI_COMM_WORLD);
            }

            // O ÚLTIMO PROCESSO CUIDA DOS ULTIMOS ELEMENTOS DO ARRAY
            index = j * linhasPorProcesso;
            int linhasSobrando = numLinhas - index;

            // ENVIANDO A QUANTIDADE DE LINHAS QUE SOBRAM PARA O PROCESSO SABER A QUANTIDADE A QUAL FICOU RESPONSAVEL
            MPI_Send(&linhasSobrando,
                     1, MPI_INT,
                     j, 0,
                     MPI_COMM_WORLD);
            // ENVIANDO OS ULTIMOS ELEMENTOS
            MPI_Send(&array[index],
                     linhasSobrando,
                     MPI_INT, j, 0,
                     MPI_COMM_WORLD);
        }

        // SOMANDO AS PRIMEIRAS LINHAS DO ARRAY QUE O PROCESSO ROOT FICOU RESPONSAVEL
        int soma = 0;
        for (i = 0; i < linhasPorProcesso; i++)
            soma += array[i];

        printf("O processo principal somou: %d\n", soma);


        // SOMANDO AS SOMAS PARCIAIS RECEBIDAS DOS OUTROS PROCESSOS
        int somaParcial;
        for (i = 1; i < numeroDeProcessos; i++)
        {
            // RECEBE A QUANTIDADE
            MPI_Recv(&somaParcial, 1, MPI_INT,
                     MPI_ANY_SOURCE, 0,
                     MPI_COMM_WORLD,
                     &status);
            // ATRIBUINDO DE QUAL PROCESSO ESSA SOMA VEM
            int sender = status.MPI_SOURCE;

            printf("O processo %d enviou a soma pracial de: %d\n", sender, somaParcial);

            soma += somaParcial;
        }

        printf("Soma total: %d\n", soma);
        gettimeofday(&t2, NULL);
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;    // sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
        printf("tempo para ser executado: %f ms.\n", elapsedTime);
    }
    // PROCESSOS "SECUNDARIOS"
    else
    {
        // RECEBE A QUANTIDADE DE LINHAS QUE ESSE PROCESSO FICA RESPONSAVEL
        MPI_Recv(&qtdeRecebida,
                 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD,
                 &status);

        // RECEBE O ARRAY SECUNDARIO QUE É USADO PARA ARMAZENAR OS RESULTADOS TEMPORARIAMENTE
        MPI_Recv(&array2, qtdeRecebida,
                 MPI_INT, 0, 0,
                 MPI_COMM_WORLD,
                 &status);

        // SOMANDO COM BASE NA QUANTIDADE DE LINHAS RECEBIDAS E NO ARRAY TEMPORARIO RECEBIDO
        int somaParcial = 0;
        for (int i = 0; i < qtdeRecebida; i++)
            somaParcial += array2[i];

        // DEVOLVENDO A SOMA PARCIAL PARA O PROCESSO PRINCIPAL
        MPI_Send(&somaParcial, 1, MPI_INT,
                 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
