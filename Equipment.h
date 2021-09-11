/*
	电气设备
*/
#pragma once
#include<iostream>
#include<string>
#include<cstring>
#define OK 1
#define ERROR 0
#define APPRO_ZERO 1e-10

using namespace std;

struct Complex
{
	int real, img;

	Complex();
	Complex(int , int);
	Complex operator+(const Complex&z);
	Complex &operator+=(const Complex&z);

	Complex operator-(const Complex&z);
	Complex &operator-=(const Complex&z);

};

class Equipment
{
public:
	Equipment();
	Equipment(int id, int st,string name);
	~Equipment();
	virtual void show_info();

	int id; //ID
	int status; //状态
	string name; //设备名称
};

//发电机
class Generator: public Equipment
{
public:
	Generator();
	Generator(int id, int st, int pid, Complex p);
	~Generator();
	void show_info();

	int pid; //连接点
	Complex power; //设备的功率 
};

//负载
class Load: public Equipment
{
public:
	Load();
	Load(int id, int st, int pid, Complex p);
	~Load();
	void show_info();

	int pid; //连接点
	Complex power; //设备的功率
};

//开关
class Switch: public Equipment
{
public:
	Switch();
	Switch(int id, int st, int pid_1, int pid_2);
	~Switch();
	void show_info();

	int pid_1, pid_2; //两个端口
};

//输电线路 or 二端变压器
class PowerLine: public Equipment
{
public:
	PowerLine();
	PowerLine(int id, int st, int pid_1, int pid_2, Complex im);
	~PowerLine();
	void show_info();

	int pid_1, pid_2; //两个端口
	//Complex power_max; //最大传输功率
	Complex impedance; //阻抗--->已经修改为导纳，名称及时更改
};

//AVL
struct AVLNode{
	Equipment* e; //储存的数据
	int height; //节点所在的高度
	AVLNode *left, *right;
};

int get_tree_height(AVLNode*);//获得节点的高度

AVLNode *insert_AVL(AVLNode*, Equipment*e); //插入数据

AVLNode *delete_AVL(AVLNode*, int id); //删除数据

AVLNode *search_AVL(AVLNode*, int id); //查找关键值为id的数据

AVLNode *modify_AVL(AVLNode*, int id, Equipment*e); //修改关键值为id的数据

void destroy_AVL(AVLNode*); //销毁avl树