/*
	电力系统
*/
#pragma once
#include<iostream>
#include<string>
#include"Equipment.h"

using namespace std;

//电气连接点对应设备的链表
typedef struct EquipList
{
	Equipment* e;
	EquipList *next;
};

EquipList* create_ListNode(Equipment*e, EquipList*next);

void destroy_List(EquipList *root);

//电气连接点
struct Point
{
	int pid; //连接点的id
	EquipList* connect_Eqs;//连接所有仪器构成的链表
	int bus_num;
	bool visit; //是否被访问
};

struct PointList
{
	Point*p;
	PointList*next;
};

PointList* create_ListNode(Point*p, PointList*next);

void destroy_List(PointList *root);

//AVL for point
struct AVLNode_p{
	Point* p; //储存的数据
	int height; //节点所在的高度
	AVLNode_p *left, *right;
};

int get_tree_height(AVLNode_p*);//获得节点的高度

AVLNode_p *insert_AVL(AVLNode_p*, Point*p); //插入数据

AVLNode_p *delete_AVL(AVLNode_p*, int id); //删除数据

AVLNode_p *search_AVL(AVLNode_p*, int id); //查找关键值为id的数据

AVLNode_p *modify_AVL(AVLNode_p*, int id, Point*p); //修改关键值为id的数据

void destroy_AVL(AVLNode_p*); //销毁avl树

//母线||节点
struct Bus
{
	PointList *pids;
	int p_num;
	bool visit;//是否被访问
	int isl_mark;//岛的标记
};

//拓扑岛
struct Island
{
	int *b_i_num; //岛上的节点编号数组
	int b_count; //岛上的节点数量
};

//电路抽象图
AVLNode_p*init_AVL_p(AVLNode_p*Ps, AVLNode* Eqs); //生成连接点树

void get_e_num(AVLNode* Eqs, int &num);

void get_p_num(AVLNode_p*Ps, int&num);

//bool all_visit(AVLNode_p*Ps);

void Point_visit(AVLNode_p*Ps_all, AVLNode_p*Ps, int &bus_num);

void init_Bus_num(AVLNode_p*Ps_all, AVLNode_p*Ps, int &num);

Bus *init_Bus(Bus*b_sys, AVLNode_p*Ps, int );//初始化节点

bool is_Point_Connected(AVLNode_p*Ps, int pid_1, int pid_2);

bool is_Bus_Connected(AVLNode_p*Ps, Bus* b_sys, int b_num_1, int b_num_2 );

void Bus_visit(AVLNode_p*Ps, Bus* b_sys, int b_num, int& isl_mark, int bus_num);

Island* init_Island(Island* isl,AVLNode_p*Ps, Bus* b_sys, int bus_num, int &);

//电力系统
class PowerSystem{
public:
	PowerSystem();
	~PowerSystem();

	void reset();

	AVLNode *Eqs;
	int e_num;
	AVLNode_p *Ps;
	int p_num;
	Bus *b_sys;
	int bus_num;
	Island *isl;
	int isl_num;
};

Complex& search_Line(Complex&Y, AVLNode* Eqs, int pid_1, int pid_2, int type);//type=1输出电线，type=0不输出

Complex search_Y(AVLNode*Eqs, Bus*b_sys,int b_num_1, int b_num_2);

//表结点（表示图的边）
typedef struct ArcNode{
       int adjvex;     //顶点下标
       ArcNode *nextarc;   //指针指向下一条边
};

//节点
typedef struct VNode{
	int b_num; //节点的编号
	ArcNode * firstarc;   //指针指向第一条边
};

//图
typedef struct Graph{
	VNode* vexs;
	int v_num,a_num;     //顶点数，边数
};

Graph* generator_Graph(PowerSystem&Sys, Island&isl);

void Dfs_Graph(Graph* G, int j, bool*visited, int*depth, int &d);

bool check_Artic(Graph *G, int i);