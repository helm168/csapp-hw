objs = echoclient.o ../hw10/rio.o socket_h.o

client.out : $(objs)
	cc -o client.out $(objs)

echoclient.o : ../hw10/rio.h socket_h.h
../hw10/rio.o : ../hw10/rio.h
socket_h.o : socket_h.h

