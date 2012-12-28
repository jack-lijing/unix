



int main(int argc, char *argv[])
{
	DB	*db;
	if((db = db_open("db4", O_RDWR | O_CREAT | O_TRUNC, NULL)) == NULL)
		printf("db_open error\n");
	if(db_store(db, "Alpha", "data1", DB_INSERT) != 0)
		printf("db_store error 1\n");
	if(db_store(db, "beta", "data for beta", DB_INSERT) != 0)
		printf("db_store error 2\n");
	if(db_store(db, "gamma", "record2", DB_INSERT) != 0)
		printf("db_store error 3\n");
	db_close(db);
	return 0;
}
