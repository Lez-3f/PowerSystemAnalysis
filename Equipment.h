/*
	�����豸
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
	int status; //״̬
	string name; //�豸����
};

//�����
class Generator: public Equipment
{
public:
	Generator();
	Generator(int id, int st, int pid, Complex p);
	~Generator();
	void show_info();

	int pid; //���ӵ�
	Complex power; //�豸�Ĺ��� 
};

//����
class Load: public Equipment
{
public:
	Load();
	Load(int id, int st, int pid, Complex p);
	~Load();
	void show_info();

	int pid; //���ӵ�
	Complex power; //�豸�Ĺ���
};

//����
class Switch: public Equipment
{
public:
	Switch();
	Switch(int id, int st, int pid_1, int pid_2);
	~Switch();
	void show_info();

	int pid_1, pid_2; //�����˿�
};

//�����· or ���˱�ѹ��
class PowerLine: public Equipment
{
public:
	PowerLine();
	PowerLine(int id, int st, int pid_1, int pid_2, Complex im);
	~PowerLine();
	void show_info();

	int pid_1, pid_2; //�����˿�
	//Complex power_max; //����书��
	Complex impedance; //�迹--->�Ѿ��޸�Ϊ���ɣ����Ƽ�ʱ����
};

//AVL
struct AVLNode{
	Equipment* e; //���������
	int height; //�ڵ����ڵĸ߶�
	AVLNode *left, *right;
};

int get_tree_height(AVLNode*);//��ýڵ�ĸ߶�

AVLNode *insert_AVL(AVLNode*, Equipment*e); //��������

AVLNode *delete_AVL(AVLNode*, int id); //ɾ������

AVLNode *search_AVL(AVLNode*, int id); //���ҹؼ�ֵΪid������

AVLNode *modify_AVL(AVLNode*, int id, Equipment*e); //�޸Ĺؼ�ֵΪid������

void destroy_AVL(AVLNode*); //����avl��