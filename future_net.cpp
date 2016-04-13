#include "route.h"
#include "lib/lib_io.h"
#include "lib/lib_time.h"
#include "stdio.h"
#include <iostream>
using namespace std;

#define POSTO "\\topo.csv"
#define POSDE "\\demand.csv"
#define POSRE "\\result.csv"
int main(int argc, char *argv[])
{
	print_time("Begin");
	char *topo[5000];
	int edge_num;
	char *demand;
	char *result;
	int demand_num;
	int result_num;

	char *folder = argv[1];

	char *stopo_file = new char[strlen(folder)+strlen(POSTO)+1];
	strcpy(stopo_file,folder);
	strcat(stopo_file,POSTO);

	edge_num = read_file(topo, 5000, stopo_file);
	if (edge_num == 0)
	{
		printf("Please input valid topo file.\n");
		return -1;
	}

	char *demand_file1 = new char[strlen(folder)+strlen(POSDE)+1];
	strcpy(demand_file1,folder);
	strcat(demand_file1,POSDE);

	demand_num = read_file(&demand, 1, demand_file1);
	if (demand_num != 1)
	{
		printf("Please input valid demand file.\n");
		return -1;
	}
	

	char *result_file1 = new char[strlen(folder)+strlen(POSRE)+1];
	strcpy(result_file1,folder);
	strcat(result_file1,POSRE);

	result_num=read_file(&result,1,result_file1);
	if (result_num!=1)
	{
		printf("Please input valid demand file.\n");
		return -1;
	}
	
	//write_result(result_file);

	JudegeResult(topo, edge_num, demand,result);

	
	release_buff(topo, edge_num);
	release_buff(&demand, 1);
	release_buff(&result, 1);

	print_time("End");

	return 0;
}

