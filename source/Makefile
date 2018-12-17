DIR	= mysql-C
CC = gcc
PROGS	= planerOdmora.c sqlFunctions.c
OUTPUT = planerOdmora
CFLAGS	= -g -Wall `mysql_config --cflags --libs`

.PHONY: all create trigger insert beauty dist progs

all: create trigger insert progs

progs:
	$(CC) -o $(OUTPUT) $(PROGS) $(CFLAGS)

create:
	mysql -u root </Users/dusanmilovic/Documents/College/PBPMysqlProject/scripts/create.sql

trigger:
	mysql -u root </Users/dusanmilovic/Documents/College/PBPMysqlProject/scripts/triggers.sql

insert:
	mysql -u root </Users/dusanmilovic/Documents/College/PBPMysqlProject/scripts/queries.sql
	
beauty:
	-indent $(PROGS)

clean:
	-rm -f *~ $(OUTPUT)
	
dist: beauty clean
	-tar -zcvf $(DIR).tar.gz $(DIR)
	
	