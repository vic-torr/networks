
#
# example networking program from
# "computer networks a systems apprroach" by Peterson and Davie
#
# makefile author: bjr
# created: 23 jan 2017
# last-update: 23 jan 2017
#

HOST= localhost
MESSAGE= "hello world"

all: client server
		./server &
		echo ${MESSAGE} | ./client ${HOST}
		killall server
		make clean

run-s: server
		./server

run: server simplex-talk
		echo ${MESSAGE} | ./client ${HOST}

simplex-talk: simplex-talk.c
		cc -o $@ $<

server: server.c
		cc -o $@ $<

clean:
		#-rm client server
