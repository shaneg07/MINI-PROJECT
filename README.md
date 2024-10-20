To Compile server.c and client.c, respectively:
gcc server.c -o server.o -L/lib/x86_64-linux-gnu/libcrypt.so -lcrypt
gcc client.c -o client.o

To Run:
./server.o and
./client.o on an another shell
