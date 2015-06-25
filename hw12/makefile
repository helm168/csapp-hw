objs = echoservers.o ../hw10/rio.o ../hw11/socket_h.o

servers.out : $(objs)
	cc -o servers.out $(objs)

echoservers.o : ../hw10/rio.h ../hw11/socket_h.h
../hw10/rio.o : ../hw10/rio.h
../hw11/socket_h.o : ../hw11/socket_h.h
