/*
	����ϵͳ
*/
#pragma once
#include<iostream>
#include<string>
#include"Equipment.h"

using namespace std;

//�������ӵ��Ӧ�豸������
typedef struct EquipList
{
	Equipment* e;
	EquipList *next;
};

EquipList* create_ListNode(Equipment*e, EquipList*next);

void destroy_List(EquipList *root);

//�������ӵ�
struct Point
{
	int pid; //���ӵ��id
	EquipList* connect_Eqs;//���������������ɵ�����
	int bus_num;
	bool visit; //�Ƿ񱻷���
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
	Point* p; //���������
	int height; //�ڵ����ڵĸ߶�
	AVLNode_p *left, *right;
};

int get_tree_height(AVLNode_p*);//��ýڵ�ĸ߶�

AVLNode_p *insert_AVL(AVLNode_p*, Point*p); //��������

AVLNode_p *delete_AVL(AVLNode_p*, int id); //ɾ������

AVLNode_p *search_AVL(AVLNode_p*, int id); //���ҹؼ�ֵΪid������

AVLNode_p *modify_AVL(AVLNode_p*, int id, Point*p); //�޸Ĺؼ�ֵΪid������

void destroy_AVL(AVLNode_p*); //����avl��

//ĸ��||�ڵ�
struct Bus
{
	PointList *pids;
	int p_num;
	bool visit;//�Ƿ񱻷���
	int isl_mark;//���ı��
};

//���˵�
struct Island
{
	int *b_i_num; //���ϵĽڵ�������
	int b_count; //���ϵĽڵ�����
};

//��·����ͼ
AVLNode_p*init_AVL_p(AVLNode_p*Ps, AVLNode* Eqs); //�������ӵ���

void get_e_num(AVLNode* Eqs, int &num);

void get_p_num(AVLNode_p*Ps, int&num);

//bool all_visit(AVLNode_p*Ps);

void Point_visit(AVLNode_p*Ps_all, AVLNode_p*Ps, int &bus_num);

void init_Bus_num(AVLNode_p*Ps_all, AVLNode_p*Ps, int &num);

Bus *init_Bus(Bus*b_sys, AVLNode_p*Ps, int );//��ʼ���ڵ�

bool is_Point_Connected(AVLNode_p*Ps, int pid_1, int pid_2);

bool is_Bus_Connected(AVLNode_p*Ps, Bus* b_sys, int b_num_1, int b_num_2 );

void Bus_visit(AVLNode_p*Ps, Bus* b_sys, int b_num, int& isl_mark, int bus_num);

Island* init_Island(Island* isl,AVLNode_p*Ps, Bus* b_sys, int bus_num, int &);

//����ϵͳ
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

Complex& search_Line(Complex&Y, AVLNode* Eqs, int pid_1, int pid_2, int type);//type=1������ߣ�type=0�����

Complex search_Y(AVLNode*Eqs, Bus*b_sys,int b_num_1, int b_num_2);

//���㣨��ʾͼ�ıߣ�
typedef struct ArcNode{
       int adjvex;     //�����±�
       ArcNode *nextarc;   //ָ��ָ����һ����
};

//�ڵ�
typedef struct VNode{
	int b_num; //�ڵ�ı��
	ArcNode * firstarc;   //ָ��ָ���һ����
};

//ͼ
typedef struct Graph{
	VNode* vexs;
	int v_num,a_num;     //������������
};

Graph* generator_Graph(PowerSystem&Sys, Island&isl);

void Dfs_Graph(Graph* G, int j, bool*visited, int*depth, int &d);

bool check_Artic(Graph *G, int i);