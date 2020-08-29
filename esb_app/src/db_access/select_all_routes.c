/**
 * @file select_all_routes.c
 * @author MubeenS 
 * @brief  The program is a C API for the SQL query 
 * SELECT * FROM routes WHERE sender=? AND message_type=? and is_active=1;
 * @version 0.1
 * @date 2020-08-29
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
/* Contains necessary C functions of mysql */
#include <mysql.h>

/**
 * @brief Parameters requiered for 
 * mysql_real_connect() function are declared as
 * global variables.
 * 
 */
static char *g_host = "localhost";/*hostname*/
static char *g_user = "root";     /*username*/
static char *g_password=""; /*blank password this user*/
static char *g_db_name ="esb_db";/*name of the database*/

unsigned int g_port = 3306; /*port number*/
static char *g_unix_socket = NULL; /*unix socket*/
unsigned int g_flag = 0; /*last parameter to mysql_real_connect*/

/**
 * @brief Whenever there is an error opening a database
 * or executing a query finish_with_error prints error
 * specifying error number and error message
 * 
 * @param con database connection handle
 */
void finish_with_error(MYSQL *con) {

  fprintf(stderr, "Error [%d]: %s \n",mysql_errno(con),mysql_error(con));
  mysql_close(con);

  exit(1);        
}

int main(int argc, char **argv) {      

  MYSQL *con ;  /*database connection handle*/
  /**
   * @brief Allocates or initialises a MYSQL object 
   * suitable for mysql_real_connect() function
   * 
   */
  con = mysql_init(NULL);

  /**
   * @brief Prints error message incase
   * initialisation of con fails.
   */
  if (con == NULL) {

      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }  
  
  /**
   * @brief Checks if connection is 
   * properly established.
   * 
   */
  if (mysql_real_connect(con, g_host, g_user, g_password,       
          g_db_name, g_port, g_unix_socket, g_flag) == NULL) {
      finish_with_error(con);
  }    
  
  /*checks execution of SQL statement*/
  if (mysql_query(con, "SELECT * FROM routes")) {
      finish_with_error(con);
  }

  MYSQL_RES *result;/*structure that holds result set*/

  /* Gets the result set and stores in result */
  result = mysql_store_result(con); 
  
  /* if there is no result error will be returned*/
  if (result == NULL) {
      finish_with_error(con);
  }

  /* stores number of fields in the result*/ 
  int num_fields = mysql_num_fields(result);
  /*mysql structure that holds a row*/
  MYSQL_ROW row;

  /*prints all the rows from the result*/
  while ((row = mysql_fetch_row(result))) { 
      for(int i = 0; i < num_fields; i++) { 
          printf("%s ", row[i] ? row[i] : "NULL"); 
      } 
          printf("\n"); 
  }
  
  /*frees the result*/
  mysql_free_result(result);
  /*closes the database connection*/
  mysql_close(con);
  
  exit(0);
}