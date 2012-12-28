all:
	gcc -g -o client_sock client_sock.c fdsock.c sendfd.c  ../include/csapp.c -I ../include -lpthread
	gcc -g -o opend server_sock.c sendfd.c fdsock.c  ../include/csapp.c -I ../include -lpthread
#	gcc -g -o client client.c sendfd.c  ../include/csapp.c -I ../include -lpthread
#	gcc -g -o opend server.c sendfd.c  ../include/csapp.c -I ../include -lpthread
	gcc -g -o pipe pipe.c   ../include/csapp.c -I ../include -lpthread
	gcc -g -o pager pager.c   ../include/csapp.c -I ../include -lpthread
	gcc -g -o add2 add2.c   ../include/csapp.c -I ../include -lpthread
	gcc -g -o add2cooperate add2cooperate.c   ../include/csapp.c -I ../include -lpthread
	gcc -g -o datastore_prt datastore_prt.c   ../include/csapp.c -I ../include -lpthread
	gcc -g -o zerommap zerommap.c   ../include/csapp.c -I ../include -lpthread
	gcc -g -o popen popen.c   ../include/csapp.c -I ../include -lpthread
	gcc -g -o add2cooperateflow add2cooperateflow.c   ../include/csapp.c -I ../include -lpthread
	
	