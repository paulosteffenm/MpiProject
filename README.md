# MpiProject
This is a small project to test the mpi.h

Message Passing Interface (MPI)

# Build Setup
```bash
# install dependecies
sudo apt install libopenmpi-dev
```

# How to execute

Use the following command
```bash
mpiCC hello.c -o hello
```

It will compile a MPI Program and create a .exe file

And then run

```bash
mpirun -np 2 hello
```

It will execute the hello.exe

The number 2 represents the number process, you can change if you want to

Exemple

```c
   #include <stdio.h>
   #include <mpi.h>

   main(int argc, char **argv)
   {
      int ierr, numProcs, myId;

      ierr = MPI_Init(&argc, &argv);

      // find out MY process ID, and how many processes were started.

      ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myId);
      ierr = MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

      printf("Hello world! I'm process %i out of %i processes\n",
         myId, numProcs);

      ierr = MPI_Finalize();
   }
```

Running with
```bash
  mpirun -np 2 hello
```

Output
```cmd

   Hello world! I'm process 0 out of 2 processes.
   Hello world! I'm process 1 out of 2 processes.

```

Running with
```bash
  mpirun -np 4 hello
```

Output
```cmd

   Hello world! I'm process 0 out of 4 processes.
   Hello world! I'm process 1 out of 4 processes.
   Hello world! I'm process 2 out of 4 processes.
   Hello world! I'm process 3 out of 4 processes.
```
