#include"Equipment.h"
//复数加减法
Complex::Complex(){}
Complex::Complex(int a, int b): real(a), img(b){}
Complex Complex::operator+(const Complex& z)
{ return Complex(real + z.real, img + z.img);}
Complex &Complex::operator+=(const Complex&z){
	real += z.real, img += z.img;
	return *this;
}
Complex Complex::operator-(const Complex& z)
{ return Complex(real - z.real, img - z.img);}
Complex &Complex::operator-=(const Complex&z){
	real -= z.real, img -= z.img;
	return *this;
}

//设备
Equipment::Equipment(){}
Equipment::Equipment(int id, int st, string name) :
	id(id), status(st), name(name){}
Equipment::~Equipment(){}
void Equipment::show_info(){}

//发电机
Generator::Generator(){
	name = "发电机";
}
Generator::Generator(int id, int st, int pid, Complex p) : 
	Equipment(id, st, "发电机"), pid(pid), power(p){
}
Generator::~Generator(){
}
void Generator::show_info(){
	//printf("%d\t%s\tAT %d\tSTATUS %d\tP %d+j%d\n", id, name, pid, status, power.real, power.img);
	cout << id << ' ' << name << "\tAT " << pid <<'\t' << "STATUS " << status << " P " << 
		power.real << "+j" << power.img << endl; 
}

//负载
Load::Load(){
	name = "负荷";
}
Load::Load(int id, int st, int pid, Complex p): 
	Equipment(id, st, "负荷"), pid(pid), power(p){
}
Load::~Load(){}
void Load::show_info(){
	//printf("%d\t%s\tAT %d\tSTATUS %d\tP %d+j%d\n", id, name, pid, status);
	cout << id << ' ' << name << "\tAT " << pid <<'\t' << "STATUS " << status << " P " << 
		power.real << "+j" << power.img << endl; 
}

//开关
Switch::Switch(){
	name = "开关";
}
Switch::Switch(int id, int st, int pid_1, int pid_2): 
	Equipment(id, st, "开关"), pid_1(pid_1), pid_2(pid_2){
}
Switch::~Switch()
{
}
void Switch::show_info(){
	//printf("%d\t%s\tFROM %d TO %d\tSTATUS %d\n", id, name, pid_1, pid_2, status);
	cout << id << ' ' << name << "\tFROM " << pid_1 << " TO " << pid_2 <<'\t' << "STATUS " << status <<endl; 
}

//输电线路
PowerLine::PowerLine(){
	name = "输电线路";
}
PowerLine::PowerLine(int id, int st, int pid_1, int pid_2, Complex im):
	Equipment(id, st, "输电线路"), pid_1(pid_1), pid_2(pid_2), impedance(im){
}
PowerLine::~PowerLine(){
}

void PowerLine::show_info(){
	cout << id << ' ' << name << "\tFROM " << pid_1 << " TO " << pid_2 <<'\t' << "STATUS " 
		<< status << " Y " << impedance.real <<"+j" << impedance.img << endl; 
}

//AVL
int get_height(AVLNode* t){//获得节点的高度
	return (t == NULL)? -1 : t->height;
}

AVLNode *create_AVLNode(Equipment*e, AVLNode*left, AVLNode*right){
	AVLNode *newNode = new AVLNode;

	if(newNode = new AVLNode){
		newNode->e = e;
		newNode->left = left;
		newNode->right = right;
		newNode->height = 0;//初始高度设为0
	}
	return newNode;
}

AVLNode *LL_Rotation(AVLNode* t){
	AVLNode* lt = t->left;
	t->left = lt->right;
	lt->right = t;
	
	t->height = max(get_height(t->left), get_height(t->right));
	lt->height = max(get_height(lt->left), get_height(lt->right));

	return (t = lt);
}//右旋

AVLNode *RR_Rotation(AVLNode* t){
	AVLNode* rt = t->right;
	t->right = rt->left;
	rt->left = t;
	
	t->height = max(get_height(t->left), get_height(t->right)) + 1;
	rt->height = max(get_height(rt->left), get_height(rt->right)) + 1;

	return (t = rt);
}//左旋

AVLNode *LR_Rotation(AVLNode* t){
	t->left = RR_Rotation(t->left);
	return LL_Rotation(t);
}//先左后右

AVLNode *RL_Rotation(AVLNode* t){
	t->right = LL_Rotation(t->right);
	return RR_Rotation(t);
}//先右后左

AVLNode *insert_AVL(AVLNode* t, Equipment*e){//插入数据
	if(t == NULL)//空树
	{
		t = new AVLNode;
		t->e = e;
		t->left = NULL;
		t->right = NULL;
		t->height = 0;
	}
	else if(e->id < t->e->id){
		t->left = insert_AVL(t->left, e);
		if(get_height(t->left) - get_height(t->right) == 2){
			if(e->id < t->left->e->id){
				t = LL_Rotation(t);
			}
			else{
				t = LR_Rotation(t);
			}
		}
	}
	else if(e->id > t->e->id){
		t->right = insert_AVL(t->right, e);
		if(get_height(t->right) - get_height(t->left) == 2){
			if(e->id > t->right->e->id){
				t = RR_Rotation(t);
			}
			else{
				t = RL_Rotation(t);
			}
		}
	}
	
	t->height = max(get_height(t->left), get_height(t->right)) + 1;

	return t;
}

AVLNode *delete_AVL(AVLNode* t, int id){ //删除数据
	if(t == NULL){
		return NULL;
	}

	if(id < t->e->id){
		t->left = delete_AVL(t->left, id);
		if(get_height(t->right) - get_height(t->left) == 2){
			if(get_height(t->right->left) > get_height(t->right->right)){
				t = RL_Rotation(t);
			}
			else{
				t = RR_Rotation(t);
			}
		}
	}
	else if(id > t->e->id){
		t->right = delete_AVL(t->right, id);
		if(get_height(t->left) - get_height(t->right) == 2){
			if(get_height(t->left->right) > get_height(t->left->left)){
				t = LR_Rotation(t);
			}
			else{
				t = LL_Rotation(t);
			}
		}
	}
	else{ //该节点即为要删除的节点
		if((t->left) && (t->right)){ //左右子树存在
			if(get_height(t->left) > get_height(t->right)){
				AVLNode*lrNode = t->left;
				while (lrNode->right)
				{
					lrNode = lrNode->right;
				}
				delete t->e;
				t->e = lrNode->e;
				t->left = delete_AVL(t->left, id);
			}
			else{
				AVLNode*rlNode = t->right;
				while(rlNode->left){
					rlNode = rlNode->left;
				}
				delete t->e;
				t->e = rlNode->e;
				t->right = delete_AVL(t->right, id);
			}
		}
		else{ //左右子树至多存在一个
			AVLNode* temp = t;
			t = (t->left)?t->left:t->right;
			delete t;
		}
	}

	return t;
}

AVLNode *search_AVL(AVLNode* t, int id){//查找关键值为key的数据
	if(!t || t->e->id == id){
		return t;
	}
	else if(id < t->e->id){
		return search_AVL(t->left, id);
	}
	else{
		return search_AVL(t->right, id);
	}
}

AVLNode *modify_AVL(AVLNode* t, int id, Equipment* e){//修改关键值为key的数据
	AVLNode *find = search_AVL(t, id);
	if(find){
		delete t->e;
		t->e = e;
	}
	return find;
}

void destroy_AVL(AVLNode* t){//销毁avl树
	if(!t){
		return;
	}

	if(t->left != NULL){
		destroy_AVL(t->left);
	}
	if(t->right != NULL){
		destroy_AVL(t->right);
	}

	delete t->e;
	delete t;
}
