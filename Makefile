all : server client maint pdr

#Modules utilitaires
memoire.o : memoire.c memoire.h
	cc -c memoire.c

semaphore.o : semaphore.c semaphore.h
	cc -c semaphore.c

#Server
server : server.o memoire.o semaphore.o
	cc -o server server.o memoire.o semaphore.o

server.o : server.c server.h
	cc -c server.c

#Client
client : client.c client.h
	cc -o client client.c

#Maintenance
maint : maint.o memoire.o semaphore.o
	cc -o maint maint.o memoire.o semaphore.o

maint.o : maint.c maint.h
	cc -c maint.c

#Depot - Retrait
pdr : pdr.o memoire.o semaphore.o
	cc -o pdr pdr.o memoire.o semaphore.o

pdr.o : pdr.c pdr.h
	cc -c pdr.c

clean : 
	rm *.o
	rm server
	rm client
	rm maint
	rm pdr