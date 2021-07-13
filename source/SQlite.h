#include <iostream>
#include <sqlite3.h>
using namespace std;
char **query_client;
string rpt_query;
class SQlite
{
private:
   /* data */
public:
   sqlite3 *db;
   char *error = 0;
   int res;
   string name_db;
   string res_query;
   SQlite(string);
   void init();
   static int selectCb(void *data, int argc, char **argv, char **colNames)
   {
      string res = "";
      query_client = argv;
      for (int i = 0; i < argc; i++)
      {
         //cout << "[" << colNames[i] << "] " << argv[i] << endl;
         string gaa(argv[i]);
         rpt_query += gaa + "|";
         //cout<<"GAA: "<<gaa<<endl;
      }
      cout << endl;

      return 0;
   }

   static int callback_count(void *count, int argc, char **argv, char **azColName)
   {
      int *c = (int *)count;
      *c = atoi(argv[0]);
      return 0;
   }

   string select_db(string table);
   string select_dep(string table, string value, int dep);
   void insert_db(string table, string campos);
   void delete_element_db(string table, string campo, string value);
   int count_db(string table, string value);
   void delete1_db(string table, string column, string value);
   void delete2_db(string table, string column1, string value1, string column2, string value2);
   void update_db(string table, string column1, string newValue,
             string column2, string oldValue);
   ~SQlite();
};

SQlite::SQlite(string db)
{
   name_db = db;
}

SQlite::~SQlite()
{
   sqlite3_close(db);
}

void SQlite::init()
{
   //open data base
   res = sqlite3_open(name_db.c_str(), &db);
   if (res)
   {
      fprintf(stderr, "error_open_db:%s\n", sqlite3_errmsg(db));
      exit(0);
   }
}

//   query

string SQlite::select_db(string table)
{
   rpt_query.clear();
   //cout<<"+-----------Select------------+"<<endl;
   string SELECT = "SELECT * FROM " + table + ";";
   //query
   res = sqlite3_exec(db, SELECT.c_str(), selectCb, 0, &error);
   if (res != SQLITE_OK)
   {
      fprintf(stderr, "query error%s\n", error);
      sqlite3_free(error);
   }
   //system("clear");
   //cout<<"+----------------------------+"<<endl;
   return rpt_query;
}

string SQlite::select_dep(string table, string value, int dep = 0)
{
   rpt_query.clear();
   string SELECT = "SELECT * FROM " + table + " WHERE nodo_inicial = " + value + ";";
   res = sqlite3_exec(db, SELECT.c_str(), selectCb, 0, &error);
   if (res != SQLITE_OK)
   {
      fprintf(stderr, "query error%s\n", error);
      sqlite3_free(error);
   }
   return rpt_query;
}

void SQlite::insert_db(string table, string campos)
{
   string INSERT = "INSERT INTO " + table +
                   " VALUES (" + campos + ");";
   res = sqlite3_exec(db, INSERT.c_str(), NULL, 0, &error);
   if (res != SQLITE_OK)
   {
      fprintf(stderr, "query error%s\n", error);
      sqlite3_free(error);
   }
}

void SQlite::delete_element_db(string table, string campo, string value)
{
   const char *data = "Callback function called";
   string DELETE = "DELETE FROM " + table + " WHERE " + campo + "=" + value + ";";
   string sql = "DELETE FROM nodos where name = 'C';";
   //query
   //res = sqlite3_exec(db,DELETE.c_str(),selectCb,(void*)data,&error);
   if (res != SQLITE_OK)
   {
      fprintf(stderr, "query error%s\n", error);
      sqlite3_free(error);
   }
}

int SQlite::count_db(string table, string value)
{
   string SELECT = "SELECT * FROM " + table + " WHERE nodo_inicial = " + value + ";";
   res = sqlite3_exec(db, SELECT.c_str(), callback_count, 0, &error);
   if (res != SQLITE_OK)
   {
      fprintf(stderr, "query error%s\n", error);
      sqlite3_free(error);
   }
   return res;
}

void SQlite::delete1_db(string table, string column, string value)
{
   string DELETE = "DELETE FROM " + table + " WHERE " + column + " = " + value + ";";
   //cout << DELETE << endl;
   res = sqlite3_exec(db, DELETE.c_str(), NULL, 0, &error);
   if (res != SQLITE_OK)
   {
      fprintf(stderr, "query error %s\n", error);
      sqlite3_free(error);
   }
}

void SQlite::delete2_db(string table, string column1, string value1, string column2, string value2)
{
   string DELETE = "DELETE FROM " + table + " WHERE " + column1 + " = '" + value1 +
                   "' AND " + column2 + " = '" + value2 + "';";
   //cout << DELETE << endl;
   res = sqlite3_exec(db, DELETE.c_str(), NULL, 0, &error);
   if (res != SQLITE_OK)
   {
      fprintf(stderr, "query error %s\n", error);
      sqlite3_free(error);
   }
}

void SQlite::update_db(string table, string column1, string newValue,
                       string column2, string oldValue)
{
   string UPDATE = "UPDATE " + table + " SET " + column1 + " = '" + newValue +
                   "' WHERE " + column2 + " = '" + oldValue + "';";
   //cout << UPDATE << endl;
   res = sqlite3_exec(db, UPDATE.c_str(), NULL, 0, &error);
   if (res != SQLITE_OK)
   {
      fprintf(stderr, "query error %s\n", error);
      sqlite3_free(error);
   }
}