#include"Equipment.h"
#include"PowerSystem.h"
#define OP_FILE_NAME "op2.txt"
#define EQ_FILE_NAME "EquipmentData3.txt"

AVLNode* init_EquipmentData(FILE*fp) //初始化数据
{
	char name[20];
	int id;
	AVLNode*Eqs = NULL;
	char temp[50];
	fgets(temp, 39, fp); //读取第一行
	while(!feof(fp)){
		if(!fscanf(fp, "%d %s ", &id, name)){
			break;
		}
		if(strcmp(name, "发电机") == 0){
			Generator* e = new Generator;
			e->id = id;
			fscanf(fp, "AT %d STATUS %d P %d+j%d\n",
				&e->pid, &e->status, &e->power.real, &e->power.img);
			cout << "Data have read：\n";
			e->show_info();
			Eqs = insert_AVL(Eqs, e);
		}
		else if(strcmp(name, "负荷") == 0){
			Load* e = new Load;
			e->id = id;
			fscanf(fp, "AT %d STATUS %d P %d+j%d\n",
				&e->pid, &e->status, &e->power.real, &e->power.img);
			cout << "Data have read：\n";
			e->show_info();
			Eqs = insert_AVL(Eqs, e);
		}
		else if(strcmp(name, "开关") == 0){
			Switch* e = new Switch;
			e->id = id;
			fscanf(fp, "FROM %d TO %d STATUS %d\n",
				&e->pid_1, &e->pid_2, &e->status);
			cout << "Data have read：\n";
			e->show_info();
			Eqs = insert_AVL(Eqs, e);
		}
		else if(strcmp(name, "输电线路") == 0){
			PowerLine *e = new PowerLine;
			e->id = id;
			fscanf(fp, "FROM %d TO %d STATUS %d Y %d+j%d\n",
				&e->pid_1, &e->pid_2, &e->status, &e->impedance.real, &e->impedance.img);
			cout << "Data have read：\n";
			e->show_info();
			Eqs = insert_AVL(Eqs, e);
		}if(fgetc(fp) == EOF){
			break;
		}else{
			fseek(fp, -1, SEEK_CUR);
		}
	}
	return Eqs;
}

int insert_EquipmentData(FILE*fp, AVLNode*Eqs) //插入数据
{
	char insert_info[40];
	char name[20];
	int id;
	long cur = ftell(fp);
	fgets(insert_info, 39, fp);
	
	cout << ">>insert" << insert_info; 
	fseek(fp, cur, SEEK_SET);
	if(!fscanf(fp,  "%d %s ", &id, name)){
		cout << "Error reading\n";
		return ERROR;
	}
	
	if(strcmp(name, "发电机") == 0){
		Generator* e = new Generator;
		e->id = id;
		fscanf(fp, "AT %d STATUS %d P %d+j%d\n",
			&e->pid, &e->status, &e->power.real, &e->power.img);
		Eqs = insert_AVL(Eqs, e);
	}
	else if(strcmp(name, "负荷") == 0){
		Load* e = new Load;
		e->id = id;
		//char temp[30];
		//fgets(temp,30,fp);cout<<temp;
		fscanf(fp, "AT %d STATUS %d P %d+j%d\n",
			&e->pid, &e->status, &e->power.real, &e->power.img);
		Eqs = insert_AVL(Eqs, e);
	}
	else if(strcmp(name, "开关") == 0){
		Switch* e = new Switch;
		e->id = id;
		fscanf(fp, "FROM %d TO %d STATUS %d\n",
			&e->pid_1, &e->pid_2, &e->status);
		Eqs = insert_AVL(Eqs, e);
	}
	else if(strcmp(name, "输电线路") == 0){
		PowerLine *e = new PowerLine;
		e->id = id;
		fscanf(fp, "FROM %d TO %d STATUS %d Y %d+j%d\n",
			&e->pid_1, &e->pid_2, &e->status, &e->impedance.real, &e->impedance.img);
		Eqs = insert_AVL(Eqs, e);
	}
	return OK;
}

int delete_EquipmentData(FILE*fp, AVLNode*Eqs){
	char delete_info[40];
	int id_delete;
	fgets(delete_info, 39, fp);
	cout << ">>delete" << delete_info;
	if(!sscanf(delete_info, " ID=%d", &id_delete)){
		cout << "Error reading id \n";
		return ERROR;
	}
	AVLNode*find;
	if(!(find = search_AVL(Eqs, id_delete)))//未找到
	{
		cout << "Failed to delete, the device with the target ID does not exist \n";
		return ERROR;
	}
	cout << "Equipment to be deleted:\n";
	find->e->show_info();
	Eqs = delete_AVL(Eqs, id_delete);
	return OK;
}

int change_EquipmentData(FILE*fp, AVLNode*Eqs)
{
	char change_info[40];
	int id_change;
	char Attr[10];
	
	int cur = ftell(fp);
	fgets(change_info, 39, fp);
	cout << ">>change" << change_info;
	fseek(fp, cur, SEEK_SET);
	fscanf(fp, " ID=%d", &id_change);
	AVLNode*find = search_AVL(Eqs, id_change);
	if(!find){
		cout << "Failed to change, the device with the target ID does not exist \n";
		return ERROR;
	}
	cout << "Equipment to be changed:\n";
	find->e->show_info();
	while(fscanf(fp, "%s", Attr)){
		if(strcmp(Attr, "STATUS") == 0){
			int st;
			fscanf(fp, "%d", &st);
			find->e->status = st;
		}else if(strcmp(Attr, "AT") == 0){
			int pid;
			fscanf(fp, "%d", &pid);
			if(find->e->name == "发电机"){
				(dynamic_cast<Generator*>(find->e))->pid = pid;
			}else if(find->e->name == "负荷"){
				(dynamic_cast<Load*>(find->e))->pid = pid;
			}
		}else if(strcmp(Attr, "Power") == 0){
			Complex p; p.real = 0, p.img = 0;
			fscanf(fp, "%d+j%d", p.real, p.img);
			if(find->e->name == "发电机"){
				(dynamic_cast<Generator*>(find->e))->power = p;
			}else if(find->e->name == "负载"){
				(dynamic_cast<Load*>(find->e))->power = p;
			}
		}else if(strcmp(Attr, "Y") == 0){
			Complex y; y.real = 0, y.img = 0;
			fscanf(fp, "%d+j%d", y.real, y.img);
			dynamic_cast<PowerLine*>(find->e)->impedance = y;
		}
		else{
			fseek(fp, -strlen(Attr), SEEK_CUR);
			break;
		}
		if(fgetc(fp) == '\n'){
			break;
			fseek(fp, -1, SEEK_CUR);
		}
	}
	cout << "After modified:\n";
	find->e->show_info();
	return OK;
}

int print_EquipmentData(FILE* fp, AVLNode*Eqs)
{
	char print_info[40];
	int id_print;
	fgets(print_info, 39, fp);
	cout << ">>print" << print_info;
	sscanf(print_info, " ID=%d", &id_print);
	AVLNode*find = search_AVL(Eqs, id_print);
	if(!find){
		cout << "Failed to print, the device with the target ID does not exist \n";
		return ERROR;
	}
	find->e->show_info();
	return OK;
}

void find_id_By_pid(AVLNode*Eqs, int pid_find, int&num_find){
	if(!Eqs){
		return;
	}
	find_id_By_pid(Eqs->left, pid_find, num_find);
	find_id_By_pid(Eqs->right, pid_find, num_find);
	if(Eqs->e->name == "发电机"){
		if(dynamic_cast<Generator*>(Eqs->e)->pid == pid_find){
			cout << Eqs->e->id <<endl;
			++num_find;
		}
	}else if(Eqs->e->name == "负载"){
		if(dynamic_cast<Load*>(Eqs->e)->pid == pid_find){
			cout << Eqs->e->id <<endl;
			++num_find;
		}
	}else if(Eqs->e->name == "开关"){
		Switch*s = dynamic_cast<Switch*>(Eqs->e);
		if(s->pid_1 == pid_find || s->pid_2 == pid_find){
			cout << Eqs->e->id <<endl;
			++num_find;
		}
	}else if(Eqs->e->name == "输电线路"){
		PowerLine*l = dynamic_cast<PowerLine*>(Eqs->e);
		if(l->pid_1 == pid_find || l->pid_2 == pid_find){
			cout << Eqs->e->id <<endl;
			++num_find;
		}
	}
}

int find_EquipmentData(FILE*fp, AVLNode*Eqs)
{
	char find_info[40];
	int pid_find;
	fgets(find_info, 39, fp);
	cout << ">>find" << find_info;
	sscanf(find_info, " fromto=%d", &pid_find);
	int num_find = 0;
	find_id_By_pid(Eqs, pid_find, num_find);
	if(!num_find){
		cout << "No Equipment found \n";
		return ERROR;
	}
	return OK;
}

void update_system_model(PowerSystem&Sys){
	Sys.reset();
	get_e_num(Sys.Eqs, Sys.e_num);
	Sys.Ps = init_AVL_p(NULL, Sys.Eqs);
	get_p_num(Sys.Ps, Sys.p_num);
	//cout << p_num;
	init_Bus_num(Sys.Ps, Sys.Ps, Sys.bus_num);
	Sys.b_sys = init_Bus(Sys.b_sys, Sys.Ps, Sys.bus_num);
	//cout << b_sys[0].pids->p->pid << " " << b_sys[0].pids->next->p->pid;
	//cout << b_sys[1].pids->p->pid << " " << b_sys[1].pids->next->p->pid;
	//cout << is_Bus_Connected(Ps, b_sys, 1, 2);
	Sys.isl = init_Island(Sys.isl, Sys.Ps, Sys.b_sys, Sys.bus_num, Sys.isl_num);
	cout << "Bus: " << Sys.bus_num << "\nIsland: " << Sys.isl_num <<"\nIsland bus num: ";
	for(int i = 0;i < Sys.isl_num; ++i){
		cout << Sys.isl[i].b_count << " ";
	}
	cout << "\n";
}

bool is_num_in(int na[], int n, int find){
	for(int i = 0;i < n; ++i){
		if(na[i] == find) return true;
	}
	return false;
}

void get_Sys_Artic(PowerSystem&Sys){
	for(int i = 0; i < Sys.isl_num; ++i){
		Graph* G = generator_Graph(Sys, Sys.isl[i]);
		for(int j = 0; j < G->v_num; ++j){
			if(check_Artic(G, j)){
				cout << G->vexs[j].b_num<< " ";
			}
		}
	}
}

void get_Sys_Info(FILE*fp, PowerSystem&Sys){
	char get_info[20];
	int cur = ftell(fp);
	fgets(get_info, 19, fp);
	cout << ">>get" << get_info;
	fseek(fp, cur, SEEK_SET);

	char order_2[20];
	fscanf(fp, "%s", order_2);
	if(strcmp(order_2, "artic") == 0){
		get_Sys_Artic(Sys);
		cout << endl;
	}

	else if(strcmp(order_2, "island") == 0){
		int isl_num = 0;
		fscanf(fp, "%d\n", &isl_num);
		cout << "Bus in this island: ";
		for(int i = 0; i < Sys.isl[isl_num - 1].b_count; ++i)
		{
			cout << Sys.isl[isl_num - 1].b_i_num[i] << " ";
		}cout << endl;
	}

	else if(strcmp(order_2, "equip") == 0){
		int bus_num = 0;
		fscanf(fp, "%d\n", &bus_num);
		int n = Sys.b_sys[bus_num - 1].p_num; //该节点上的电气连接点数量
		int *visited_switch = new int [n * n]; //访问过的开关
		int s_num = 0;//开关数量
		for(int i =0; i <n*n;++i){visited_switch[i] = 0;}
		cout << "Equipments in this bus:\n";
		PointList*pl = Sys.b_sys[bus_num - 1].pids;
		for(int i = 0;i < n; ++i){
			EquipList*el = pl->p->connect_Eqs;
			while(el){
				if((el->e->name == "发电机" || el->e->name == "负荷") && el->e->status == 1){
					cout << el->e->id << endl;
				}else if(el->e->name == "开关"){
					int s_id = el->e->id;
					if(!is_num_in(visited_switch, s_num, s_id)){
						visited_switch[s_num++] = s_id;
						if(el->e->status == 1){
							cout << el->e->id << endl;
						}//如果开关是闭合的，那么开关必然在节点内
					}
				}
				el = el->next;
			}
			pl = pl->next;
		}
	}

	else if(strcmp(order_2, "bus") == 0){
		int id = 0;
		fscanf(fp, "%d", &id);
		AVLNode*e = search_AVL(Sys.Eqs, id);
		cout << "Bus which the equipment is connected with:\n";
		if(e->e->name == "输电线路"){
			PowerLine* ec = dynamic_cast<PowerLine*>(e->e);
			AVLNode_p* p1 = search_AVL(Sys.Ps, ec->pid_1);
			AVLNode_p* p2 = search_AVL(Sys.Ps, ec->pid_2);
			cout << p1->p->bus_num << " " << p2->p->bus_num << endl; 
		}
		else if(e->e->name == "开关"){
			Switch* s = dynamic_cast<Switch*>(e->e);
			if(s->status == 1){
				cout << search_AVL(Sys.Ps, s->pid_1)->p->bus_num <<  endl;		 
			}
			else{
				AVLNode_p* p1 = search_AVL(Sys.Ps, s->pid_1);
				AVLNode_p* p2 = search_AVL(Sys.Ps, s->pid_2);
				cout << p1->p->bus_num << " " << p2->p->bus_num << endl;
			}
		}
		else if(e->e->name == "发电机"){
			Generator* g = dynamic_cast<Generator*>(e->e);
			cout << search_AVL(Sys.Ps, g->pid)->p->bus_num <<  endl;
		}
		else if(e->e->name == "负荷"){
			Load* l = dynamic_cast<Load*>(e->e);
			cout << search_AVL(Sys.Ps, l->pid)->p->bus_num <<  endl;
		}
	}

	else if(strcmp(order_2, "line") == 0){
		int b_1 = 0, b_2 =0;
		fscanf(fp, " %d %d\n", &b_1, &b_2);
		Complex temp(0,0);
		Bus b1 = Sys.b_sys[b_1 - 1];
		Bus b2 = Sys.b_sys[b_2 - 1];
		PointList*pl_1 = b1.pids;
		for(int i = 0;i < b1.p_num; ++i){
			PointList*pl_2 = b2.pids;
			for(int i = 0;i < b2.p_num; ++i){
				search_Line(temp, Sys.Eqs, pl_1->p->pid, pl_2->p->pid, 1);
				pl_2 = pl_2->next;
			}
			pl_1 = pl_1->next;
		}
	}
	else if(strcmp(order_2, "Y") == 0){
		int b_1 = 0, b_2 =0;
		fscanf(fp, " %d %d\n", &b_1, &b_2);
		Complex Y(0, 0);
		if(b_1 == b_2){
			for(int i = 1; i <= Sys.bus_num; ++i){
				if(i == b_1){continue;}
				Y-=search_Y(Sys.Eqs, Sys.b_sys, b_1, i);
			}
		}else{
			Y+=search_Y(Sys.Eqs, Sys.b_sys, b_1, b_2);
		}
		cout << Y.real << "+j" << Y.img << endl;
	}
	else{
		return;
	}
}

void operate_EquipmentData(FILE*fp, PowerSystem&Sys)
{
	while(!feof(fp))
	{
		char order[20];
		fscanf(fp, "%s", order);
		if(strcmp(order, "insert") == 0){
			if(insert_EquipmentData(fp, Sys.Eqs)){
				cout<< "OK\n";
			}
		}
		
		else if(strcmp(order, "delete") == 0){
			if(delete_EquipmentData(fp, Sys.Eqs)){
				cout<< "OK\n";
			}
		}
		
		else if(strcmp(order, "change") == 0){
			if(change_EquipmentData(fp, Sys.Eqs)){
				cout << "OK\n";
			}
		}
		
		else if(strcmp(order, "print") == 0){
			print_EquipmentData(fp, Sys.Eqs);
		}
		
		else if(strcmp(order, "find") == 0){
			find_EquipmentData(fp, Sys.Eqs);
		}
		
		else if(strcmp(order, "update") == 0){
			fgets(order, 19, fp);
			cout << ">>update model\n";
			update_system_model(Sys);
		}
		
		else if(strcmp(order, "get") == 0){
			get_Sys_Info(fp, Sys);
		}	
		
		else{
			cout << "wrong order!\n";
			exit(0);
		}
		if(fgetc(fp) == EOF){
			break;
		}else{
			fseek(fp, -1, SEEK_CUR);
		}
	}
}

int main()
{
	PowerSystem Sys = PowerSystem(); //初始化电力系统
	FILE*fp;
	if((fp = fopen(EQ_FILE_NAME,"r")) == NULL)
	{
		cout << "cannot open ths file\n";
		exit(0);
	}
	Sys.Eqs = init_EquipmentData(fp);
	fclose(fp);
	cout<< "\nexecute op.txt:\n";
	if((fp = fopen(OP_FILE_NAME,"r")) == NULL)
	{
		cout << "cannot open ths file\n";
		exit(0);
	}
	operate_EquipmentData(fp, Sys);
	fclose(fp);

	system("pause");
	return 0;
}