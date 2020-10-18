
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

all: simplex_talk_client simplex_talk_server
		./simplex_talk_server &
		echo ${MESSAGE} | ./simplex_talk_client ${HOST}
		killall simplex_talk_server
		make clean

run-s: simplex_talk_server
		./simplex_talk_server
run: simplex_talk_server simplex-talk
		echo ${MESSAGE} | ./simplex_talk_client ${HOST}
simplex-talk: simplex-talk.c
		cc -o $@ $<

simplex_talk_server: simplex_talk_server.c
		cc -o $@ $<

clean:
		#-rm simplex_talk_client simplex_talk_server
