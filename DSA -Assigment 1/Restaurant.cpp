#include "main.h"

extern int MAXSIZE;


class imp_res : public Restaurant
{
	private:
		class timer{
			public:
			string name;
			int energy;
			bool in;
			timer* next;
			public:
			timer(string name,int energy, bool in){
				this->name = name;
				this->energy = energy;
				this->in = in;
				this->next = NULL;
			}

		};
		// ban an
		customer* cur;
		int count_in;
		// hang chờ
		customer* head_wait;
		customer* tail_wait;
		int count_wait;
		// thu tu
		customer* front;
		customer* rear;
		timer* head_time;
		timer* tail_time;
		int n_timer ;
		friend class customer;	
	public:	
		imp_res() {
			head_wait = NULL;
			tail_wait = NULL;
			count_in = 0;
			count_wait = 0;
			cur = nullptr;
			front = NULL;
			rear = NULL;
			head_time = NULL;
			tail_time = NULL;
			n_timer = 0;
		}

		void insertLast(timer* p){
			if(n_timer == 0){
				head_time = p;
				tail_time = p;
			}
		
			else{
				tail_time->next = p;
				tail_time = p;
			}
			n_timer++;
		}

		void deleteFirst(){
			if(n_timer == 0) return;
			if(n_timer == 1){
				timer* x = head_time;
				head_time = NULL;
				tail_time = NULL;
				delete x;
			}else{
				timer* x = head_time;
				head_time = head_time->next;
				delete x;
			}
			n_timer--;
		}

		void insertLeft(customer* p){
			if(count_in == 1){
				p->next = this->cur;
				this->cur->prev = p;
				this->cur->next = p;
				p->prev = this->cur;
				this->cur = p;
			}
			else{
				p->prev = this->cur->prev;
				this->cur->prev->next = p;
				p->next = this->cur;
				this->cur->prev = p;
				this->cur = p;
			}
		}

		void insertRight(customer* p){
			if(count_in == 1){
				this->cur->next = p;
				p->prev = this->cur;
				p->next = this->cur;
				this->cur->prev = p;
				this->cur = p;
				//cout << cur->name << " " << cur->energy << endl;
			}
			else{
				p->next = this->cur->next;
				this->cur->next->prev = p;
				this->cur->next = p;
				p->prev = this->cur;
				this->cur = p;
			}
		}

		// void enqueue(customer* p){
		// 	if(count_in == 0){
		// 		front = p;
		// 		rear = p;
		// 		//cout << front->name << endl;
		// 	}else{
		// 		rear->next = p;
		// 		p->prev = rear;
		// 		rear = p;
		// 		//cout << rear->name << endl;
		// 	}
		// }

		// void dequeue(){
		// 	if(front==NULL){
		// 		//cout << "11!" << endl;
		// 		return;
		// 	}else{
		// 		//cout << "22!" << endl;
		// 		customer* temp = front;
		// 		front = front->next;
		// 		temp->next = NULL;
		// 		//cout << "25!" << endl;
		// 		//delete temp;
		// 		//cout << "33!" << endl;
		// 	}
		// }

		void deleted(customer* &p){

				//delete p;
				if(count_in == 0)return;
				if(count_in == 1){
					//delete cur;
					cur == NULL;
				}

				else if(count_in==2){
						cur = p->next;
						cur->next = NULL;
						cur->prev = NULL;
						p->next = NULL;
						p->prev = NULL;
						//delete p;
				}
				else{
					if(p->energy > 0){
						cur = p->next;
						//cout << 1 << endl;
						p->prev->next = cur;
						//cout << 2 << endl;
						cur->prev = p->prev;
						//cout << 3 << endl;
						//delete p;
						//cout << 4 << endl;
					}else{
						cur = p->prev;
						cur->next = p->next;
						p->next->prev = cur;
						//delete p;
					}
				}
				//cout << "da tru" << endl;
				count_in--;
				//cout << "tru con " << count_in << endl;
		}

		void clear_in(){
			cur->prev->next =NULL;
			cur->prev = NULL;
			for (int i = 0; i < count_in-1; i++)
			{
				customer* temp = cur;
				cur = cur->next;
				temp->next = NULL;
				cur->prev = NULL;
				delete temp;
			}
			cur = NULL;
			for (int i = 0; i < count_in; i++)
			{
					deleteFirst();
			}
			count_in = 0;

		}

		void RED(string name, int energy)
		{
			if(energy == 0) return;
			customer *temp = cur;
			for(int i = 0 ; i < count_in ; i++){
				if(name == temp->name) return;
				temp = temp->next;
			}
			temp = head_wait;
			for(int i = 0 ; i < count_wait ; i++){
				if(name == temp->name) return;
				temp = temp->next;
			}
			customer* cus = new customer(name,energy,0,0);
			//customer* cus2 = new customer(name,energy,0,0);

		// thêm vào bàn ăn

			if(count_in < MAXSIZE){
				//enqueue(cus2);
				timer* time = new timer(name,energy,1);
				insertLast(time);
				if(count_in == 0){

						cur = cus;
						cur->prev = cus;
						cur->next = cus;
						count_in++;
					}
				else if(count_in < MAXSIZE/2){ // thêm vào bàn ăn trái hoặc phải vị trí X
					if(energy >= cur->energy){
						insertRight(cus);
						count_in++;
					}
					else{
						insertLeft(cus);
						count_in++;
					}
					
				}
				else{ // thêm vào bàn ăn theo chênh lệch energy MAXSIZE >= 2
					customer* flag = cur;
					int max = 0;
					for (int i = 0; i < count_in; i++)
					{
						if(abs(cur->energy-energy)>max){
						flag = cur;
						max = abs(cur->energy-energy);
						}
						cur = cur->next;
					}
					this->cur = flag;
					
						if(energy-cur->energy < 0){
							insertLeft(cus);
							count_in++;
						}else{
							insertRight(cus);
							count_in++;
						}
					
				}
				
			}
			else if(count_in >= MAXSIZE && count_wait < MAXSIZE){ // thêm vào hàng chờ
					if(count_wait == 0){
						head_wait = cus;
						tail_wait = cus;
						count_wait++;
					}
					else{
						tail_wait->next = cus;
						cus->prev = tail_wait;
						tail_wait = cus;
						count_wait++;
					}
					timer* time = new timer(name,energy,0);
					insertLast(time);
			}
			else{
				return;
			}
			
		}
		void BLUE(int num)
		{
			// xoa tat ca cac phan tu trong ban an
			if(num >= MAXSIZE){
				//cout << 1 << endl;
				clear_in();
				//cout << 2 << endl;
				int size = 0;
				if(count_wait > num) size = num;
				else size = count_wait;
				
				for (int i = 0; i < size; i++)
				{
					//cout << 6 << endl;
					if(count_wait == 0) break;
					string namee = head_wait->name;
					int energyy = head_wait->energy;
					customer* t = head_wait;
					head_wait = head_wait->next;
					t->next = NULL;
					delete t;
					count_wait--;
					RED(namee,energyy);
				}
			}
			else{ // khong xoa het trong ban an
					for (int i = 0; i < num; i++)
					{
						customer* temp = cur;
						//cout << "count_in = " << count_in << endl;
						//cout << head_time->name << "vs" << endl;;
						for(int j = 0 ; j < count_in;j++){
							//cout << "j= " << j << endl;
							//cout << temp->name << "==" << front->name << endl;
						//	cout << temp->name << endl;
							if(temp->name == head_time->name){
								//cout << 3 << endl;
								//cout << "vooooooo" << endl;
								deleted(temp);
								deleteFirst();
								break;
								//cout << 5 << endl;
							}
							temp = temp->next;
						}
					}
					for (int i = 0; i < num; i++)
					{
						//cout << 6 << endl;
						if(count_wait == 0) break;
						string namee = head_wait->name;
						int energyy = head_wait->energy;
						customer* t = head_wait;
						head_wait = head_wait->next;
						t->next = NULL;
						delete t;
						count_wait--;
						RED(namee,energyy);
					}
			}

			
			

			
		}

		void moveptrright(customer* &p, int distance){
			int i = 0;
			while (i<distance&&p!=NULL)
			{
				p = p->next;
				i++;
			}
		}
		void moveptrleft(customer* &p, int distance){
			int i = 0;
			while (i<distance&&p!=NULL)
			{
				p = p->prev;
				i++;
			}
		}

		int insertsort(customer* &head, int count_wait, int gap){
			int count = 0;
			customer* p = head;
			moveptrright(p,gap);
			while(p!=NULL){
				customer* temp1 = p;
				customer* temp2 = p;
				moveptrleft(temp1,gap);
				while (temp1!=NULL&&(abs(temp1->energy)<=abs(temp2->energy)))
				{
					if(abs(temp1->energy) == abs(temp2->energy)){
						timer* temp = head_time;
						while (temp->name != temp1->name && temp->name != temp2->name)
						{
							temp = temp->next;
						}
						if(temp->name == temp2->name){
							count+=1;
							int x = temp1->energy;
							temp1->energy = temp2->energy;
							temp2->energy = x;
							string str = temp1->name;
							temp1->name = temp2->name;
							temp2->name = str;
							moveptrleft(temp1,gap);
							moveptrleft(temp2,gap);
						}
						
					}else{
						count+=1;
						int x = temp1->energy;
						temp1->energy = temp2->energy;
						temp2->energy = x;
						string str = temp1->name;
						temp1->name = temp2->name;
						temp2->name = str;
						moveptrleft(temp1,gap);
						moveptrleft(temp2,gap);
					}
				}
				moveptrright(p,gap);
			}
			return count;
		}

		int shellsort(customer* &head,int count_wait){
			customer* ptr = head;
			int countt = 0;
			for (int i = count_wait/2; i > 2; i /=2)
			{
				for(int j = 0; j < i; j++){
					countt += insertsort(ptr,count_wait-j,i);
					ptr = ptr->next;
				}
			}
			countt += insertsort(ptr,count_wait,1);
			return countt;
			
		}
		void PURPLE()
		{
			int num = shellsort(head_wait,count_wait);
			cout << num << endl;
			BLUE(num%MAXSIZE);
		}
		void REVERSAL()
		{
			// customer* p = cur;
			// cout << "--------------------------------\n";
			// for (int i = 0; i < count_in; i++)
			// {
			// 	p->print();
			// 	p = p->next;
			// }

			// cout << "--------------------------------\n";
			if(count_in <= 1) return;
			string flag = cur->name;
		
			customer* left  = cur;
			customer* right = cur->next;
			
			// swap duong
			int dem = 0;
			while(1)
			{
				if(left->energy < 0){
					left = left->prev;
					dem++;
				}
				if(right->energy < 0){
					right = right->next;
					dem++;
				}
				if(left->energy>0&&right->energy>0){
					int temp = left->energy;
					left->energy = right->energy;
					right->energy = temp;
					string name = left->name;
					left->name = right->name;
					right->name = name;
					left=left->prev;
					right = right->next;
					
					
					dem+=2;
				}
				//cout << "demm = " << dem << endl;
				
					if(dem>= count_in-1)break;
			}
			// swap am
			customer* left2  = cur;
			customer* right2 = cur->next;
			dem = 0;

			while(1)
			{
				if(left2->energy > 0){
					left2 = left2->prev;
					dem++;
					//cout << "doi left: dem = " << dem << endl; 
				}
				if(right2->energy > 0){
					right2 = right2->next;
					dem++;
					//cout << "doi right: dem = " << dem << endl; 
				}
				if(left2->energy<0&&right2->energy<0){
					int temp = left2->energy;
					left2->energy = right2->energy;
					right2->energy = temp;
					string name = left2->name;
					left2->name = right2->name;
					right2->name = name;
					left2=left2->prev;
					right2 = right2->next;
					dem+=2;
				}
				if(dem>= count_in-1)break;
				
			}
			// cout << "sau khi reversal:" << endl;
			// p = cur;
			// cout << "--------------------------------\n";
			// for (int i = 0; i < count_in; i++)
			// {
			// 	p->print();
			// 	p = p->next;
			// }

			// cout << "--------------------------------\n";
			
		}
		void UNLIMITED_VOID()
		{
			customer* p = cur;
			int minSum = INT_MAX;
			customer* target = cur;
			int tarlength = 0;
			for (int i = 0; i < count_in; i++)
			{
				p->print();
				int length = 0;
				int sum = 0;
				customer* temp = p;
				for (int j = 0; j < count_in; j++)
				{
					sum += temp->energy;
					length++;
					if(length >= 4){
						if(sum < minSum){
							minSum = sum;
							target = p;
							tarlength = length;
						}else if(sum==minSum){
							if(length >= tarlength){
								minSum = sum;
								target = p;
								tarlength = length;
							}
						}
					}
					temp = temp->next;
				}
				p = p->next;
			}
			customer* start = target;
			customer* m = target;
			int rightlength = 0;
			int min = INT_MAX;
			for (int i = 0; i < tarlength; i++)
			{
				if(m->energy<min){
					min = m->energy;
					start = m;
					rightlength = 0;
				}
				rightlength++;
				m = m->next;
			}
			for (int i = 0; i < rightlength; i++)
			{
				start->print();
				start = start->next;
			}
			for (int i = 0; i < tarlength-rightlength; i++)
			{
				target->print();
				target = target->next;
			}
		}

		// void deleteTHUAT(){
		// 	if(count_in == 0) return;
		// 	if(count_in == 1){
		// 		if(cur->energy<0){
		// 			return;
		// 		}else{
		// 			cur = NULL;
		// 			count_in--;
		// 		}
		// 	}
		// 	else if(count_in == 2){

		// 	}
		// }
		void deletedcir(customer* &temp,int &count_in){
			if(count_in == 0)return;
				if(count_in == 1){
					temp == NULL;
				}
				else if(count_in==2){
					customer* x = temp;
					temp = temp->next;
					temp->next = NULL;
					temp->prev = NULL;
					x->next = NULL;
					x->prev = NULL;
					delete x;
				}else{
					customer* x = temp;
						temp = temp->next;
						x->prev->next = temp;
						temp->prev = x->prev;
						x->next = NULL;
						x->prev = NULL;
						delete x;
				}

		}

		void deleteddouble(customer* &p,customer* &tail,customer* &head,int &count){
			if(count == 0)return;
			if(count == 1){
				p==NULL;
				tail = NULL;
				head = NULL;
			}
			if(p==tail){
				tail = tail->prev;
				tail->next = NULL;
				p->prev = NULL;
				customer* x = p;
				p = tail;
				delete x;
			}else if(p == head){
				head = head->next;
				head->prev = NULL;
				p->next = NULL;
				p == NULL;
			}
			else{
				customer* x = p;
				p = p->prev;
				p->next  = x->next;
				p->next->prev = p;
				x->prev = NULL;
				x->next = NULL;
				delete x;
			}
			count--;
		}
		void DOMAIN_EXPANSION()
		{
			timer* p = head_time;
			int sumOAN = 0;
			int sumTHUAT = 0;
			for (int i = 0; i < n_timer; i++)
			{
				if(p->energy>0){
					sumTHUAT+= p->energy;
				}else{
					sumOAN += abs(p->energy);
				}
				p = p->next;
			}
			// p = head_wait;
			// for (int i = 0; i < count_wait; i++)
			// {
			// 	if(p->energy>0){
			// 		sumTHUAT+= p->energy;
			// 	}else{
			// 		sumOAN += abs(p->energy);
			// 	}
			// 	p = p->next;
			// }
			cout << sumTHUAT <<"     " <<sumOAN << endl;
			bool deletedTHUAT = 0;
			customer* t = cur;
			cout << "cir truoc khi xoa" << endl;
			for (int i = 0; i < count_in; i++)
			{
				t->print();
				t = t->next;
			}
			cout << "------------------------------------" << endl;
			int k = count_in;
			
			if(sumTHUAT>= sumOAN){
				deletedTHUAT = 0;
			}else deletedTHUAT = 1;
			if(deletedTHUAT){
				customer* temp = cur;
				for (int i = 0; i < count_in; i++)
				{
					if(temp->energy > 0){
						cout << "xoa: ";
						temp->print();
						deleted(temp);
					}
					else {
						cout << "khong xoa: ";
						temp->print();
						temp= temp->next;
					}
				}

				customer* m = tail_wait;
				for (int i = 0; i < count_wait; i++)
				{
					if(m->energy > 0){
						m->print();
						deleteddouble(m,tail_wait,head_wait,count_wait);
					}
					else{
						m = m->prev;
					}
				}
				m = rear;
				for (int i = 0; i < k; i++)
				{
					if(m->energy > 0){
						m->print();
						deleteddouble(m,front,rear,k);
					}else{
						m = m->prev;
					}
				}
			}else{
				customer* temp = cur;
				int k = count_in;
				for (int i = 0; i < count_in; i++)
				{
					if(temp->energy < 0){
						deleted(temp);
					}else temp = temp->next;
				}
				customer* m = tail_wait;
				for (int i = 0; i < count_wait; i++)
				{
					if(m->energy < 0){
						m->print();
						deleteddouble(m,tail_wait,head_wait,count_wait);
					}
					else{
						m = m->prev;
					}
				}
				m = rear;
				for (int i = 0; i < k; i++)
				{
					if(m->energy < 0){
						m->print();
						deleteddouble(m,front,rear,k);
					}else{
						m = m->prev;
					}
				}
				
			}
			cout << "cir sau khi xoa" << endl;
			customer* x = cur;
			for (int i = 0; i < count_in; i++)
			{
				x->print();
				x = x->next;
			}
			
		}
		void LIGHT(int num)
		{
			cout << "LIGHT" << endl;
			customer* p = cur;
			if(num>0){
				for (int i = 0; i < count_in; i++)
				{
					p->print();
					p = p->next;
				}
			}else if(num < 0){
				for (int i = 0; i < count_in; i++)
				{
					p->print();
					p = p->prev;
				}
				
			}else{
				p = head_wait;
				for (int i = 0; i < count_wait; i++)
				{
					p->print();
					p = p->next;
				}
				
			}
		}
};