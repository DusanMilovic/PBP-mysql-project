#include "planerOdmora.h"
#include "sqlFunctions.h"

int main (int argc, char const *argv[])
{
  Sql sql;
  sql.connection = mysql_init(NULL);

  if (mysql_real_connect (sql.connection, "localhost", "root", "", "PlanerOdmora", 0, NULL, 0) == NULL) {
    printf ("Connection refused\n");
  } else {
    printf("Connection successful\n");
  }

  glavniMeni(&sql);
  return 0;
}
