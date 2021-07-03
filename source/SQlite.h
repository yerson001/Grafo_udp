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
      string res="";
      query_client = argv;
      for (int i = 0; i < argc; i++)
      {
         //cout << "[" << colNames[i] << "] " << argv[i] << endl;
         string gaa(argv[i]);
         rpt_query+=gaa+"+";
         //cout<<"GAA: "<<gaa<<endl;
      }
      cout << endl;
      return 0;
   }
   string select_db(string table);
   void insert_db(string table, string campos);
   void delete_element_db(string table, string campo, string value);

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
   //cout<<"+-----------Select------------+"<<endl;
   string SELECT = "SELECT * FROM " + table + ";";
   //query
   res = sqlite3_exec(db, SELECT.c_str(), selectCb,0, &error);
   if (res != SQLITE_OK)
   {
      fprintf(stderr, "query error%s\n", error);
      sqlite3_free(error);
   }
   //cout<<"+----------------------------+"<<endl;
   return rpt_query;
}

void SQlite::insert_db(string table, string campos)
{
   string INSERT = "INSERT INTO " + table +
                   " VALUES (" + campos + ");";
   //query
   
   //insert INTO nodos VALUES('C');
   //INSERT INTO atributos VALUES ('B','edad','yhon');
   //INSERT INTO relaciones VALUES ('B','A');

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