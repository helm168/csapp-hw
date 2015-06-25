objs = echoservers.o ../hw10/rio.o ../hw11/socket_h.o
objts = echoservert.o ../hw10/rio.o ../hw11/socket_h.o ../hw11/echo.o

servers.out : $(objs)
	cc -o servers.out $(objs)

servert.out : $(objts)
	cc -pthread -o servert.out $(objts)

echoservers.o : ../hw10/rio.h ../hw11/socket_h.h
echoservert.o : ../hw10/rio.h ../hw11/socket_h.h
../hw11/echo.o : ../hw10/rio.h ../hw11/socket_h.h
../hw10/rio.o : ../hw10/rio.h
../hw11/socket_h.o : ../hw11/socket_h.h
