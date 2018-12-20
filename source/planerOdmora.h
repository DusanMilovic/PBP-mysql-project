#ifndef PLANERODMORA_H
#define PLANERODMORA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdbool.h>

#define QUERY_LENGHT 2048

typedef struct {
	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL_FIELD *column;
	char query[QUERY_LENGHT];
}Sql;

/*void print(SQL* sql);*/
#endif