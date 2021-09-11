#include"PowerSystem.h"

//设备链表
EquipList* create_ListNode(Equipment*e, EquipList*next){
	EquipList* node = new EquipList;
	node->e = e;
	node->next = next;
	return node;
}

void destroy_List(EquipList *root)
{
	if(!root){
		return;
	}else{
		EquipList* temp = root;
		destroy_List(root->next);
		delete temp;
		temp = NULL;
	}
}

//电气连接点
PointList* create_ListNode(Point*p, PointList*next){
	PointList* node = new PointList;
	node->p = p;
	node->next = next;
	return node;
}

void destroy_List(PointList *root)
{
	if(!root){
		return;
	}else{
		PointList* temp = root;
		destroy_List(root->next);
		delete temp;
		temp = NULL;
	}
}

AVLNode_p*init_AVL_p(AVLNode_p*Ps, AVLNode* Eqs) //生成连接点树
{
	if(!Eqs){
		return Ps;
	}
	
	if(Eqs->e->name == "开关"){
		int id1 = dynamic_cast<Switch*>(Eqs->e)->pid_1;
		int id2 = dynamic_cast<Switch*>(Eqs->e)->pid_2;
		AVLNode_p *p_find;
		if(p_find = search_AVL(Ps, id1)){
			EquipList*en = create_ListNode(Eqs->e, NULL);
			EquipList*pe = p_find->p->connect_Eqs;
			p_find->p->connect_Eqs = en;
			en->next = pe;
		}else{
			Point*p= new Point;
			p->pid = id1;
			p->visit = false;
			p->connect_Eqs = create_ListNode(Eqs->e, NULL);
			Ps = insert_AVL(Ps, p);
		}
		
		if(p_find = search_AVL(Ps, id2)){
			EquipList*en = create_ListNode(Eqs->e, NULL);
			EquipList*pe = p_find->p->connect_Eqs;
			p_find->p->connect_Eqs = en;
			en->next = pe;
		}else{
			Point*p= new Point;
			p->pid = id2;
			p->visit = false;
			p->connect_Eqs = create_ListNode(Eqs->e, NULL);
			Ps = insert_AVL(Ps, p);
		}
	}else if(Eqs->e->name == "输电线路"){
		int id1 = dynamic_cast<PowerLine*>(Eqs->e)->pid_1;
		int id2 = dynamic_cast<PowerLine*>(Eqs->e)->pid_2;
		AVLNode_p *p_find;
		if(p_find = search_AVL(Ps, id1)){
			EquipList*en = create_ListNode(Eqs->e, NULL);
			EquipList*pe = p_find->p->connect_Eqs;
			p_find->p->connect_Eqs = en;
			en->next = pe;
		}else{
			Point*p= new Point;
			p->pid = id1;
			p->visit = false;
			p->connect_Eqs = create_ListNode(Eqs->e, NULL);
			Ps = insert_AVL(Ps, p);
		}
		
		if(p_find = search_AVL(Ps, id2)){
			EquipList*en = create_ListNode(Eqs->e, NULL);
			EquipList*pe = p_find->p->connect_Eqs;
			p_find->p->connect_Eqs = en;
			en->next = pe;
		}else{
			Point*p= new Point;
			p->pid = id2;
			p->visit = false;
			p->connect_Eqs = create_ListNode(Eqs->e, NULL);
			Ps = insert_AVL(Ps, p);
		}
	}else if(Eqs->e->name == "发电机"){
		int id = dynamic_cast<Generator*>(Eqs->e)->pid;
		AVLNode_p *p_find;
		if(p_find = search_AVL(Ps, id)){
			EquipList*en = create_ListNode(Eqs->e, NULL);
			EquipList*pe = p_find->p->connect_Eqs;
			p_find->p->connect_Eqs = en;
			en->next = pe;
		}else{
			Point*p= new Point;
			p->pid = id;
			p->visit = false;
			p->connect_Eqs = create_ListNode(Eqs->e, NULL);
			Ps = insert_AVL(Ps, p);
		}
	}else if(Eqs->e->name == "负荷"){
		int id = dynamic_cast<Load*>(Eqs->e)->pid;
		AVLNode_p *p_find;
		if(p_find = search_AVL(Ps, id)){
			EquipList*en = create_ListNode(Eqs->e, NULL);
			EquipList*pe = p_find->p->connect_Eqs;
			p_find->p->connect_Eqs = en;
			en->next = pe;
		}else{
			Point*p = new Point;
			p->pid = id;
			p->visit = false;
			p->connect_Eqs = create_ListNode(Eqs->e, NULL);
			Ps = insert_AVL(Ps, p);
		}
	}
	Ps = init_AVL_p(Ps, Eqs->left);
	Ps = init_AVL_p(Ps, Eqs->right);
	return Ps;
}

/*
无向图的结构
节点集合 ------ 顶点
节点 ----- 节点的
*/
void get_e_num(AVLNode*Eqs, int&num){
	if(!Eqs){
		return;
	}else{
		++num;
		get_e_num(Eqs->left, num), get_e_num(Eqs->right, num);
	}
}

void get_p_num(AVLNode_p*Ps, int&num){
	if(!Ps){
		return;
	}else{
		++num;
		get_p_num(Ps->left, num), get_p_num(Ps->right, num);
	}
} //求电气连接点的数目

void Point_visit(AVLNode_p*Ps_all, AVLNode_p*Ps,  int &bus_num){
	if(!Ps || Ps->p->visit){
		return;
	}
	else{
		//cout << Ps->p->pid << " ";
		Ps->p->visit = true;
		Ps->p->bus_num = bus_num;
		EquipList* pe = Ps->p->connect_Eqs; //相连的设备
		while(pe){
			if(pe->e->name == "开关" && pe->e->status == 1){
				Switch*s = dynamic_cast<Switch*>(pe->e);
				int id = s->pid_1 == Ps->p->pid?
					s->pid_2 : s->pid_1;
				AVLNode_p* next_visit = search_AVL(Ps_all, id);
				if(next_visit){
					Point_visit(Ps_all, next_visit, bus_num);
				}
			}
			pe = pe->next;
		}
	}
}

void init_Bus_num(AVLNode_p* Ps_all, AVLNode_p*Ps, int &bus_num){
	if(!Ps){
		return;
	}
	init_Bus_num(Ps_all, Ps->left, bus_num);
	if(!Ps->p->visit){
		++bus_num;
		Point_visit(Ps_all, Ps, bus_num);
	}
	init_Bus_num(Ps_all, Ps->right, bus_num);
}

void init_B_sys(Bus* b_sys, AVLNode_p* Ps, int bus_num){
	if(!Ps){ return;}
	for(int i=1;i <= bus_num; ++i){
		if(Ps->p->bus_num == i){
			PointList*temp = b_sys[i - 1].pids;
			PointList*newNode = create_ListNode(Ps->p, NULL);
			b_sys[i - 1].pids = newNode;
			newNode->next = temp;
			++b_sys[i - 1].p_num;
		}
	}
	init_B_sys(b_sys, Ps->left, bus_num);
	init_B_sys(b_sys, Ps->right, bus_num);
}

Bus* init_Bus(Bus* b_sys, AVLNode_p* Ps, int bus_num){
	b_sys = new Bus[bus_num];
	for(int i = 1; i <= bus_num; ++i){
		b_sys[i - 1].p_num = 0;
		b_sys[i - 1].visit = false;
	}//初始化节点的连接点数量
	init_B_sys(b_sys, Ps, bus_num);
	return b_sys;
}

//初始化拓扑岛
bool is_Point_Connected(AVLNode_p*Ps, int pid_1, int pid_2){
	AVLNode_p*find_1 = search_AVL(Ps, pid_1);
	EquipList*Eq_1 = find_1->p->connect_Eqs;
	while(Eq_1){
		if(Eq_1->e->name == "开关" && Eq_1->e->status == 1){
			Switch*s = dynamic_cast<Switch*>(Eq_1->e);
			if(s->pid_1 == pid_2 || s->pid_2==pid_2){ return true; }
		}else if(Eq_1->e->name == "输电线路" && Eq_1->e->status == 1){
			PowerLine*s = dynamic_cast<PowerLine*>(Eq_1->e);
			if(s->pid_1 == pid_2 || s->pid_2==pid_2){ return true; }
		}
		Eq_1 = Eq_1->next;
	}
	return false;
}

bool is_Bus_Connected(AVLNode_p*Ps, Bus* b_sys, int b_num_1, int b_num_2 ){
	PointList* pl_1 = b_sys[b_num_1 - 1].pids; //节点1 的 连接点链表
	int i = 0;
	while(i < b_sys[b_num_1 - 1].p_num){
		int j = 0;
		PointList* pl_2 = b_sys[b_num_2 - 1].pids;//节点2 的 连接点链表
		while(j < b_sys[b_num_2 - 1].p_num){
			if(is_Point_Connected(Ps, pl_1->p->pid, pl_2->p->pid)){
				return true;
			}
			pl_2 = pl_2->next; ++j;
		}
		pl_1 = pl_1->next; ++i;
	}
	return false;
}

void Bus_visit(AVLNode_p*Ps, Bus* b_sys, int b_num, int &isl_mark, int bus_num){ //给相同的节点做同样的标记
	if(b_sys[b_num - 1].visit){
		return;
	}else{
		b_sys[b_num - 1].visit = true;
		b_sys[b_num - 1].isl_mark = isl_mark;
		PointList* pl = b_sys[b_num - 1].pids;
		int i =  0;
		while(i < b_sys[b_num - 1].p_num){
			for(int k = 1; k <= bus_num; ++k){
				if(k != b_num && is_Bus_Connected(Ps, b_sys, b_num, k)){
					int next_b_num = k;
					Bus_visit(Ps, b_sys, next_b_num, isl_mark, bus_num);	
				}
			}
			pl = pl->next;++i;
		}
	}
}

Island* init_Island(Island* isl,AVLNode_p*Ps, Bus* b_sys, int bus_num, int&isl_num){
	int isl_mark = 0;
	for(int i = 1;i <= bus_num; ++i){
		if(!b_sys[i - 1].visit){
			++isl_mark;
			Bus_visit(Ps, b_sys, i, isl_mark, bus_num);
		}
	}
	isl = new Island[isl_mark];
	for(int i = 1; i <= isl_mark; ++i){
		isl[i - 1].b_count = 0;
	}
	for(int j = 1; j <= isl_mark; ++j){
		for(int i = 1;i <= bus_num; ++i){
			if(b_sys[i - 1].isl_mark == j){
				++isl[j - 1].b_count;
			}
		}
		isl[j - 1].b_i_num = new int[isl[j - 1].b_count];
		int k  = 0;
		for(int i = 1;i <= bus_num; ++i){
			if(b_sys[i - 1].isl_mark == j){
				isl[j - 1].b_i_num[k++] = i;//获得岛内的节点数目
			}
		}
	}

	for(int j = 1;j < isl_mark; ++j){
		for(int i = 0; i <= isl_mark - j - 1; ++i){
			if(isl[i].b_count < isl[i + 1].b_count){
				Island temp = isl[i + 1];
				isl[i + 1] = isl[i];
				isl[i] = temp;
			}
		}
	}
	isl_num = isl_mark;
	return isl;
}

//电力系统
PowerSystem::PowerSystem(){
	Eqs = NULL;
	e_num = 0;
	Ps = NULL;
	p_num = 0;
	b_sys = NULL;
	bus_num = 0;
	isl = NULL;
	isl_num = 0;
}

PowerSystem::~PowerSystem(){
	destroy_AVL(Eqs);
	destroy_AVL(Ps);
	delete[] b_sys; //destroy bus
	delete[] isl; //destroy island
}

void PowerSystem::reset(){
	destroy_AVL(Ps);
	delete[] b_sys;
	delete[] isl;
	e_num = 0;
	Ps = NULL;
	p_num = 0;
	b_sys = NULL;
	bus_num = 0;
	isl = NULL;
	isl_num = 0;
}

Complex& search_Line(Complex&Y, AVLNode* Eqs, int pid_1, int pid_2, int type){
	if(!Eqs){
		return Y;
	}
	if(Eqs->e->name == "输电线路"){
		PowerLine*p = dynamic_cast<PowerLine*>(Eqs->e);
		if((pid_1==p->pid_1&&pid_2==p->pid_2) || (pid_1==p->pid_2&&pid_2==p->pid_1)){
			if(type) cout << p->id << endl;
			Y += p->impedance;
		}
	}
	search_Line(Y, Eqs->left, pid_1, pid_2, type);
	search_Line(Y, Eqs->right, pid_1, pid_2, type);
	return Y;
}

Complex search_Y(AVLNode*Eqs, Bus*b_sys,int b_num_1, int b_num_2)//b_num1!=b_num2
{
	Complex Y(0, 0);
	Bus b1 = b_sys[b_num_1 - 1];
	Bus b2 = b_sys[b_num_2 - 1];
	PointList*pl_1 = b1.pids;
	for(int i = 0;i < b1.p_num; ++i){
		PointList*pl_2 = b2.pids;
		for(int j = 0;j < b2.p_num; ++j){
			Complex Y1(0, 0);
			Y += search_Line(Y1, Eqs, pl_1->p->pid, pl_2->p->pid, 0);
			pl_2 = pl_2->next;
		}
		pl_1 = pl_1->next;
	}
	return Y;
}

//AVL_Point
int get_height(AVLNode_p* t){//获得节点的高度
	return (t == NULL)? -1 : t->height;
}

AVLNode_p *create_AVLNode(Point*p, AVLNode_p*left, AVLNode_p*right){
	AVLNode_p *newNode = new AVLNode_p;

	if(newNode = new AVLNode_p){
		newNode->p = p;
		newNode->left = left;
		newNode->right = right;
		newNode->height = 0;//初始高度设为0
	}
	return newNode;
}

AVLNode_p *LL_Rotation(AVLNode_p* t){
	AVLNode_p* lt = t->left;
	t->left = lt->right;
	lt->right = t;
	
	t->height = max(get_height(t->left), get_height(t->right));
	lt->height = max(get_height(lt->left), get_height(lt->right));

	return (t = lt);
}//右旋

AVLNode_p *RR_Rotation(AVLNode_p* t){
	AVLNode_p* rt = t->right;
	t->right = rt->left;
	rt->left = t;
	
	t->height = max(get_height(t->left), get_height(t->right)) + 1;
	rt->height = max(get_height(rt->left), get_height(rt->right)) + 1;

	return (t = rt);
}//左旋

AVLNode_p *LR_Rotation(AVLNode_p* t){
	t->left = RR_Rotation(t->left);
	return LL_Rotation(t);
}//先左后右

AVLNode_p *RL_Rotation(AVLNode_p* t){
	t->right = LL_Rotation(t->right);
	return RR_Rotation(t);
}//先右后左

AVLNode_p *insert_AVL(AVLNode_p* t, Point*p){//插入数据
	if(t == NULL)//空树
	{
		t = new AVLNode_p;
		t->p = p;
		t->left = NULL;
		t->right = NULL;
		t->height = 0;
	}
	else if(p->pid < t->p->pid){
		t->left = insert_AVL(t->left, p);
		if(get_height(t->left) - get_height(t->right) == 2){
			if(p->pid < t->left->p->pid){
				t = LL_Rotation(t);
			}
			else{
				t = LR_Rotation(t);
			}
		}
	}
	else if(p->pid > t->p->pid){
		t->right = insert_AVL(t->right, p);
		if(get_height(t->right) - get_height(t->left) == 2){
			if(p->pid > t->right->p->pid){
				t = RR_Rotation(t);
			}
			else{
				t = RL_Rotation(t);
			}
		}
	}else{} //已有元素，不会添加
	
	t->height = max(get_height(t->left), get_height(t->right)) + 1;

	return t;
}

AVLNode_p *delete_AVL(AVLNode_p* t, int id){ //删除数据
	if(t == NULL){
		return NULL;
	}

	if(id < t->p->pid){
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
	else if(id > t->p->pid){
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
				AVLNode_p*lrNode = t->left;
				while (lrNode->right)
				{
					lrNode = lrNode->right;
				}
				delete t->p;
				t->p = lrNode->p;
				t->left = delete_AVL(t->left, id);
			}
			else{
				AVLNode_p*rlNode = t->right;
				while(rlNode->left){
					rlNode = rlNode->left;
				}
				delete t->p;
				t->p = rlNode->p;
				t->right = delete_AVL(t->right, id);
			}
		}
		else{ //左右子树至多存在一个
			AVLNode_p* temp = t;
			t = (t->left)?t->left:t->right;
			delete t;
		}
	}

	return t;
}

AVLNode_p *search_AVL(AVLNode_p* t, int id){//查找关键值为key的数据
	if(!t || t->p->pid == id){
		return t;
	}
	else if(id < t->p->pid){
		return search_AVL(t->left, id);
	}
	else{
		return search_AVL(t->right, id);
	}
}

AVLNode_p *modify_AVL(AVLNode_p* t, int id, Point*p){//修改关键值为key的数据
	AVLNode_p *find = search_AVL(t, id);
	if(find){
		delete t->p;
		t->p = p;
	}
	return find;
}

void destroy_AVL(AVLNode_p* t){//销毁avl树
	if(!t){
		return;
	}

	if(t->left != NULL){
		destroy_AVL(t->left);
	}
	if(t->right != NULL){
		destroy_AVL(t->right);
	}

	delete t->p;
	delete t;
}

//由拓扑岛构建连通图
Graph* generator_Graph(PowerSystem&Sys,Island&isl)
{
	Graph* G = new Graph;
	G->v_num = isl.b_count;
	G->vexs = new VNode[G->v_num];
	for(int i = 0; i < G->v_num; ++i){
		//初始化节点
		VNode vex;
		vex.b_num = isl.b_i_num[i];
		vex.firstarc = NULL;
		G->vexs[i] = vex;
	}
	for(int i = 0; i < G->v_num; ++i){
		//初始化边
		for(int j = 0; j < G->v_num; ++j){
			if(i == 2){
				//cout << "#";
			}
			if(j != i){
				if(is_Bus_Connected(Sys.Ps, Sys.b_sys, G->vexs[i].b_num, G->vexs[j].b_num)){
					ArcNode* newarc = new ArcNode;
					newarc->adjvex = j; //下一个节点在图中的下标
					newarc->nextarc = NULL;
					ArcNode* temp = G->vexs[i].firstarc;
					G->vexs[i].firstarc = newarc;
					newarc->nextarc = temp;
				}
			}
		}
	}
	return G;
}

void Dfs_Graph(Graph* G, int j, bool*visited, int*depth, int &d){
	if(visited[j]){
		return;
	}
	visited[j] = true;depth[j] = d;
	++d;
	for(ArcNode*p = G->vexs[j].firstarc ; p; p = p->nextarc){
		Dfs_Graph(G, p->adjvex, visited, depth, d);
	} 
	--d;
}

bool check_Artic(Graph *G, int i){
	//int *parent = new int[G->v_num];
	bool *visited = new bool[G->v_num];
	int *depth = new int[G->v_num];
	for(int j = 0;j < G->v_num; ++j){
		visited[j] = false;
		depth[j] = 0;
	}
	int d = 0;
	Dfs_Graph(G, i , visited, depth, d);
	int child_num = 0;
	for(int l = 0; l < G->v_num; ++l){
		if(depth[l] == 1){
			++child_num;
		} 
	}
	if(child_num >= 2){
		return true;
	}else{
		return false;
	}
	delete []visited;
	delete []depth;
}//求图的第i个元素是不是关节点
	