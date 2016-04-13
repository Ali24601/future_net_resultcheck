#ifndef __ROUTE_H__
#define __ROUTE_H__

#undef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#define MAX_NODE_NUMBERS 600
#define MAX_MIDNODE_NUMBERS 50
#define SHORT_INF 0x3f3f
struct path 
{
	unsigned short index;
	unsigned short from;
	unsigned short to;
	unsigned short weight;
};
class node
{
public:
	unsigned short value;
	node *next;

	node(unsigned short a,node *b)
	{
		value=a;
		next=b;
	}
};

class stacks
{
public:
	node *top;
	stacks(node *a=NULL)
	{
		top=NULL;
	}
	void push(unsigned short a)
	{
		if (top==NULL)
			top =new node(a,NULL);
		else top=new node(a,top);
	}
	void pop()
	{
		node *b=top;
		top=top->next;
		delete b;
	}
	void clear()
	{
		while(top!=NULL)
		{
			pop();
		}
	}
}; //保存已加入路径结点的栈

static void parse_paths(char* topo[], unsigned int edge_num, struct path* paths,unsigned short **matrix,unsigned short &maxnodeindex,unsigned short pathnummatrix[][MAX_NODE_NUMBERS],struct path** path2);
static void parse_demand(char* demand,unsigned short vnode[], unsigned short *start_ptr, unsigned short* end_ptr, unsigned short* via_nodes_count_ptr) ;
static void parse_result(char *result,unsigned short path[],unsigned short *num);
void JudegeResult(char *graph[5000], int edge_num, char *condition,char *result);
#endif
