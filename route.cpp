
#include "route.h"
#include "lib/lib_record.h"
#include "lib/lib_time.h"
#include <string.h>  
#include <stdlib.h>  
#include <stdio.h>
//你要完成的功能总入口
void JudegeResult(char *topo[5000], int edge_num, char *demand,char *result)
{
	unsigned short **matrix;
	matrix = (unsigned short **)malloc(MAX_NODE_NUMBERS*sizeof(unsigned short *));
	for(int j=0;j<MAX_NODE_NUMBERS;j++)
	{
		matrix[j] = (unsigned short *)malloc(MAX_NODE_NUMBERS*sizeof(unsigned short));  //这个指针数组的每个指针元素又指向一个数组。
		memset(matrix[j],SHORT_INF,MAX_NODE_NUMBERS*sizeof(unsigned short));  
	}

	//unsigned short *matrix[MAX_NODE_NUMBERS][MAX_NODE_NUMBERS]=new unsigned short [MAX_NODE_NUMBERS][MAX_NODE_NUMBERS]*//邻接矩阵
	unsigned short visit[MAX_NODE_NUMBERS];          //标记是否被访问
	unsigned short demand_path[50];    //要求路径经过的点
	unsigned short *pathindex = (unsigned short*) malloc(edge_num * sizeof(unsigned short));
	unsigned short answernum=0;
	//memset(matrix,SHORT_INF,sizeof(matrix));  
	memset(visit,0,sizeof(visit)); 

	unsigned short pathnummatrix[MAX_NODE_NUMBERS][MAX_NODE_NUMBERS];     //邻接矩阵
	memset(pathnummatrix,-1,sizeof(pathnummatrix)); 
	//读取边
	struct path* paths = (struct path*) malloc(edge_num * sizeof(struct path));
	struct path** path2 = (struct path**) malloc(5000 * sizeof(struct path**));

	memset(path2,NULL,5000 * sizeof(struct path**)); 
	unsigned short max_node_index=0;//所有点的最大序号
	parse_paths(topo, (unsigned int) edge_num, paths,matrix,max_node_index,pathnummatrix,path2);

	unsigned short start_index,end_index,via_nodes_count;
	parse_demand(demand, demand_path, &start_index, &end_index, &via_nodes_count);

	int tempbegin=start_index;
	parse_result(result,pathindex,&answernum);
	
	bool error=false;
	//Repass link
	unsigned short *samepath=(unsigned short*) malloc(5000 * sizeof(unsigned short));//检测各边出现次数
	memset(samepath,0,5000 * sizeof(unsigned short));  
	unsigned short *samenode=(unsigned short*) malloc((max_node_index+1) * sizeof(unsigned short));//检测各点出现次数
	memset(samenode,0,(max_node_index+1) * sizeof(unsigned short));  
	samenode[tempbegin]+=1;
	int *MapFromIndexToMiddlePointIndex = (int*) malloc((max_node_index+1) * sizeof(int));
	memset(MapFromIndexToMiddlePointIndex,-1,(max_node_index+1) * sizeof(int)); 
	//int MapFromIndexToMiddlePointIndex[max_node_index+1]={-1};
	for(int i=0;i<via_nodes_count;++i)
		MapFromIndexToMiddlePointIndex[demand_path[i]]=i;
	int actual_via_nodes_count=0;//为了方便，通过计数方式判断是否经过所有必经点
	if (!error)
	{
		for (int i=0;i<answernum;++i)
		{
			samepath[pathindex[i]]+=1;
			if (samepath[pathindex[i]]>=2)
			{
				printf("Repass link\n");
				error=true;
				break;
			}
		}
	}
	if (!error)
	{
		for (int i=0;i<answernum;++i)
		{
			if (path2[pathindex[i]])
			{

				if (path2[pathindex[i]]->from==tempbegin)
				{
					tempbegin=path2[pathindex[i]]->to;

					samenode[tempbegin]+=1;
					if (samenode[tempbegin]>1)
					{
						printf("Repass node\n");
						error=true;
						break;
					}
					if (MapFromIndexToMiddlePointIndex[tempbegin]!=-1)
					{
						actual_via_nodes_count++;
					}
				}
				else if (i==0)
				{
					printf("First node wrong\n");
					error=true;
					break;
				}
				else
				{
					printf("Path break\n");
					error=true;
					break;
				}

			}
			else
			{
				printf("Link unexist\n");
				error=true;
				break;
			}
		}
		if (!error)
		{
			if (actual_via_nodes_count<via_nodes_count)
			{
				printf("Unreached node\n");
				error=true;
			}
			
		}
	}
	if (!error)
	{
		if (tempbegin==!end_index)
		{
			printf("Last node wrong\n");
			error=true;
		}
		else
		{
			printf("Right Answer\n");
			error=true;
		}
	}
	

	free(paths);
	free(path2);
	free(samenode);
	free(pathindex);
	free(samepath);
	free(MapFromIndexToMiddlePointIndex);
	for(int j=0;j<MAX_NODE_NUMBERS;j++)
	{
		free(matrix[j]);
	}
	free(matrix);
}


static void parse_paths(char* topo[], unsigned int edge_num, struct path* paths,unsigned short **matrix,unsigned short &maxnodeindex,unsigned short pathnummatrix[][MAX_NODE_NUMBERS],struct path** path2) 
{
	unsigned short index;
	unsigned short from, to;
	unsigned char weight;
	struct path* path;
	char* p;
	for (unsigned short i = 0; i < edge_num; ++i) 
	{
		path = &paths[i];
		p = topo[i];
		index=0;
		do 
		{
			index *= 10;
			index += *p - '0';
			p++;
		} while (*p != ',');
		p++;
		path->index = index;
		from = 0;
		do 
		{
			from *= 10;
			from += *p - '0';
			p++;
		} while (*p != ',');
		p++;
		path->from = from;
		if (maxnodeindex<from)
		{
			maxnodeindex=from;
		}

		to = 0;
		do 
		{
			to *= 10;
			to += *p - '0';
			p++;
		} while (*p != ',');
		p++;
		path->to = to;
		if (maxnodeindex<to)
		{
			maxnodeindex=to;
		}
		//matrix[from][to]=1;

		weight = 0;
		do 
		{
			weight *= 10;
			weight += *p - '0';
			p++;
		} while (*p != '\r'&&*p != '\n'&&*p != '\0');
		path->weight = weight;
		if (*(*(matrix+from)+to)>weight)
		{
			*(*(matrix+from)+to)=weight;
			pathnummatrix[from][to]=index;
		}	
		path2[path->index]=path;
	}
	for (unsigned short i = 0; i <= maxnodeindex; ++i) 
	{
		matrix[i][i]=0;
	}
}

static void parse_demand(char* demand,unsigned short vnode[], unsigned short * start_ptr, unsigned short* end_ptr, unsigned short* via_nodes_count_ptr)
{
	unsigned short start = 0, end = 0, index = 0, via_nodes_count = 0;
	char* p = demand;

	do {
		start = start * 10 + *p - '0';
		p++;
	} while (*p != ',');
	*start_ptr = start;
	p++;

	do {
		end = end * 10 + *p - '0';
		p++;
	} while (*p != ',');
	*end_ptr = end;
	p++;

	do {
		index = index * 10 + *p - '0';
		p++;
		if (*p == '|') 
		{
			vnode[via_nodes_count++]=index;
			p++;
			index = 0;
			continue;
		}
	} while (*p != '\r'&&*p != '\n'&&*p != '\0');
	vnode[via_nodes_count++]=index;
	*via_nodes_count_ptr = via_nodes_count;
}
static void parse_result(char *result,unsigned short path[],unsigned short *num)
{
	unsigned short index = 0,count = 0;
	char* p = result;


	do {
		index = index * 10 + *p - '0';
		p++;
		if (*p == '|') 
		{
			path[count++]=index;
			p++;
			index = 0;
			continue;
		}
	} while (*p != '\r'&&*p != '\n'&&*p != '\0');
	path[count++]=index;
	*num = count;
}