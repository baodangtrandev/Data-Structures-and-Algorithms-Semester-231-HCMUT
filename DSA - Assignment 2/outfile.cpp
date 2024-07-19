#include<bits/stdc++.h> 
#include<string>

using namespace std;


ofstream out("output.txt");
int MAXSIZE;
// Customer class
#define COUNT 10
// ofstream out("output.txt");


class Customer;
class Restaurant;
class Area;
class GRestaurant;
class SRestaurant;
class imp_res;
class HuffNode;
class HuffTree;
struct compare;
class minHeapNode;

int time_queue = 0;
class minHeapNode{
	public:
	char data;
	int freq;
	int height;
	int time;
	minHeapNode* left;
	minHeapNode* right;

	minHeapNode(int freq,char data){
		//cout << "da vao day"<< endl;
		left = right = NULL;
		this->data = data;
		this->freq = freq;
		this->height = 1;
		this->time = time_queue++;
	}

	minHeapNode(char data, int freq){
		left = right = NULL;
		this->data = data;
		this->freq = freq;
		this->height = 1;

	}

};

int timeCount = 0;
class Customer {
    
	public:
    
		string name;
		string Caesar_name;
        int timein;
		long long result;
		vector <pair<char,int>> list_X;
		map <char,string> binary_code;
		int size;
		minHeapNode* root;

		friend minHeapNode;
		friend compare;
		
	public:
        Customer(){
            this->timein = timeCount++;
            this->name = "";
            this->Caesar_name = "";
            this->result = 0;
            this->size = 0;
            this->root = NULL;
        }
		Customer(string name){
			//this->count_AVL = 0;
            this->timein = timeCount++;
			this->name = name;
			//cout << this->name << endl;
			this->Caesar_name = name;
			this->Caesar_name = Caesar_func(name);
			this->size = list_X.size();
			this->root = HuffmanCodes(list_X,size);
			storeCodes(this->root,"");
			result = EncodeCaesarName();

		}
		~Customer(){
			if (root != nullptr) {
            destroyTree(root);
        	}
		}
		void destroyTree(minHeapNode* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
		string Caesar_func(string);
		minHeapNode* HuffmanCodes(vector <pair<char,int>>,int);
		void storeCodes(minHeapNode*,string);
		minHeapNode* convertToAVL(minHeapNode* root,int &count_AVL);
		long long EncodeCaesarName();
        void print2DUtil(minHeapNode* root, int space)
        {
            if (root == NULL)
                return;
            space += COUNT;
            print2DUtil(root->right, space);
            cout << endl;
            for (int i = COUNT; i < space; i++)
                cout << " ";
            cout << root->data << " " << root->freq << "\n";
            print2DUtil(root->left, space);
        }

        void print2D(minHeapNode* root)
        {
            print2DUtil(root, 0);
        }
};

// ---------- convert to AVL Tree -----------------//
int getHeight(minHeapNode* node){
	return (node == nullptr) ? 0 : node->height;
}

void updateHeight(minHeapNode* node) {
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
}

void UpdateHeightFull(minHeapNode *root) {
    if (root == nullptr) {
        return;
    }
    UpdateHeightFull(root->left);
    UpdateHeightFull(root->right);

    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
}

minHeapNode* rotateRight(minHeapNode* node) {
    minHeapNode* node1 = node->left;
    minHeapNode* node2 = node1->right;

    node1->right = node;
    node->left = node2;

    // // Update heights
    updateHeight(node);
    updateHeight(node1);

    return node1;
}

minHeapNode* rotateLeft(minHeapNode* node) {
    minHeapNode* node1 = node->right;
    minHeapNode* node2 = node1->left;

    node1->left = node;
    node->right = node2;

		updateHeight(node);
		updateHeight(node1);

    return node1;
}

minHeapNode* leftRightRotate(minHeapNode* root) {
    root->left = rotateLeft(root->left);
    return rotateRight(root);
}

minHeapNode* rightLeftRotate(minHeapNode* root) {
    root->right = rotateRight(root->right);
    return rotateLeft(root);
}

int getBalanceFactor(minHeapNode* node) {
    return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
}

minHeapNode* getBalanceNode(minHeapNode* root,int &count_AVL){
	 if (root == nullptr) {
        return nullptr;
    }
	if(root != NULL && count_AVL>= 3){
		return root;
   }
    int balance = getBalanceFactor(root);
        if (balance > 1) {
        if (getBalanceFactor(root->left) >= 0) {
				count_AVL++;
                root =  rotateRight(root);
			
        } else {
                count_AVL++;
                root =  leftRightRotate(root);
        }
    } else if (balance < -1) {
        if (getBalanceFactor(root->right) <= 0) {
                count_AVL++;
                root =  rotateLeft(root);
        } else {
                count_AVL++;
                root =  rightLeftRotate(root);
        }
    }
	UpdateHeightFull(root);
	return root;
}

minHeapNode* Customer::convertToAVL(minHeapNode* root,int &count_AVL) {
	 if (root == nullptr) {
        return nullptr;
    }
   if(root!= NULL && count_AVL>= 3){
		return root;
   }
  	root = getBalanceNode(root,count_AVL);
	root = getBalanceNode(root,count_AVL);
	root = getBalanceNode(root,count_AVL);
	root->left = convertToAVL(root->left,count_AVL);
	UpdateHeightFull(root->left);
    root->right = convertToAVL(root->right,count_AVL);
	UpdateHeightFull(root->right);

    return getBalanceNode(root,count_AVL);
}

struct compare {
		int flag = 0;
    bool operator()(minHeapNode* l, minHeapNode* r) {
        if (l->freq != r->freq) {
            return l->freq > r->freq;
		}
		else if(l->freq == r->freq&&l->data == '$' && r->data == '$')return l>r;
        else {
			if(l->data == '$' && r->data == '$'){
				return l>r;
			}
			else if((l->data == '$')&&(r->data!= '$')){
				return true;
			}else if((l->data != '$')&&(r->data == '$')){
				return false;
			}
            else if (islower(l->data) && isupper(r->data)) {
                return false;
            } else if (islower(r->data) && isupper(l->data)) {
                return true;
            }
			else if(l->data > r->data) {
                return true;
            }
			else if(l->data < r->data) {
				return false;
			}else return false;
       		}
	}

};

struct compare1{
    bool operator()(minHeapNode* l, minHeapNode* r) {
		if(l->freq == r->freq){
			return l->time > r->time;
		}else return l->freq > r->freq;
}
};

int k = 0;
minHeapNode* Customer::HuffmanCodes(vector<pair<char,int>> list_X,int size){
	priority_queue <minHeapNode*,vector<minHeapNode*>,compare> minHeap;
	priority_queue <minHeapNode*,vector<minHeapNode*>,compare1> minHeap1;

	minHeapNode* left ;
	minHeapNode* right;
	minHeapNode* top;
	
	for (int i = 0; i < size; i++)
	{
		minHeapNode* temp = new minHeapNode(list_X[i].first,list_X[i].second);
		//cout << temp->data << " " << temp->freq << endl;
		minHeap.push(temp);
		//delete temp;
	}

	while (minHeap.size() > 0)
	{
		minHeapNode* temp = new minHeapNode(minHeap.top()->freq, minHeap.top()->data);
		minHeap1.push(temp);
		minHeap.pop();
	}

	while (minHeap1.size() > 1)
	{
		left = minHeap1.top();
		//cout <<"left    "<< left->data << " " << left->freq <<" "<< left->time << endl;	
		minHeap1.pop();
		right = minHeap1.top();
		//cout << "right     "<<right->data << " " << right->freq <<" "<< right->time<< endl;
		minHeap1.pop();

		top = new minHeapNode(left->freq + right->freq,'$');
		//cout <<"top      " <<top->data << " " << top->freq <<" "<< top->time<< endl;
		top->left = left;
		top->right = right;
		updateHeight(top);

		int count_AVL = 0;
		top = convertToAVL(top,count_AVL);
        top->time = time_queue++;

        // cout << "Time Top: " << top->time << endl;
		// cout << "--------------------------------------------------------" << endl;
		
		// print2D(top);

		// cout << "--------------------------------------------------------" << endl;

		UpdateHeightFull(top);
		minHeap1.push(top);
		
	}
	minHeapNode* root = minHeap1.top();
	return root;
}

//--------------------------- hàm mã hóa caesar ---------------------------//

vector<pair<char, int>> countAndSortCharacters(const string& input) {
    vector<pair<char, int>> charFrequency;

    int frequency[256] = {0};
    for (char c : input) {
        frequency[int(c)]+=1;
    }

    for (int i = 0; i < 256; i++) {
        if (frequency[i] > 0) {
            charFrequency.push_back(make_pair(static_cast<char>(i), frequency[i]));
        }
    }
    sort(charFrequency.begin(), charFrequency.end(), [](const pair<char, int>& a, const pair<char, int>& b) {
        return (a.second != b.second) ? a.second < b.second : a.first < b.first;
    });

    return charFrequency;
}
char caesarCipher(char c, int shift) {
    if (isalpha(c)) {
        char base = isupper(c) ? 'A' : 'a';
        return static_cast<char>((c - base + shift) % 26 + base);
    }
    return c;
}

string Customer::Caesar_func(string name){
	vector <pair<char,int>> charFrequency;
	charFrequency = countAndSortCharacters(name);
	for(const auto & pair:charFrequency){
		char char_Caesar = caesarCipher(pair.first,pair.second);
		for(unsigned int i = 0; i < name.length(); i++){
			if(name[i] == pair.first){
				Caesar_name[i] = char_Caesar;
			}
		}
	}
	charFrequency = countAndSortCharacters(Caesar_name);
	for (unsigned int i = 0; i < charFrequency.size(); i++)
	{
		list_X.push_back(make_pair(charFrequency[i].first,charFrequency[i].second));
		this->size++;
	}
	return Caesar_name;
}

void Customer::storeCodes(minHeapNode* root, string str){
	if(root == NULL){
		return;
	}
	if(root->data != '$'){
		binary_code[root->data] = str;
	}
	storeCodes(root->left,str+"0");
	storeCodes(root->right,str+"1");
}

int binaryToDecimal(const std::string& binaryString) {
    return std::bitset<10>(binaryString).to_ulong();
}

long long Customer::EncodeCaesarName(){
	if(root == NULL){
		return 0;
	}
	string binaryString = "";
	for(char c:Caesar_name){
		binaryString += binary_code[c];
	}
	int length = static_cast<int>(binaryString.length());
    long long decimalValue;
	string binary = "";
	for (int i = length-1; i >=0; i--)
	{
		binary += binaryString[i];
	}
	if(length < 10){
		 decimalValue = binaryToDecimal(binary);
	}else{
		 decimalValue = binaryToDecimal(binary.substr(0, 10));
	}
	return decimalValue;

}

// ----------------------- Area ---------------------------//
class AreaG{
	public:
	class NodeBST{
		public:
        Customer* cus;
		NodeBST* left;
		NodeBST* right;
		friend class AreaG;
		public:
		NodeBST(Customer* cus){
			this->cus = cus;
            left = NULL;
            right = NULL;
		}
		
	};
	int sizeBST;
	int ID;
	NodeBST* root;
	AreaG(int ID){ 
        this->ID = ID;
		sizeBST = 0;
		root = NULL;
	}

	~AreaG(){
		destroyTree(root);
	}

	void destroyTree(NodeBST* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node->cus; // Delete the Customer object
            delete node;      // Delete the tree node
        }
    }

    std::vector<int> postorderTraversal(NodeBST* root) {
    std::vector<int> result;
    if (root == nullptr) {
        return result;
    }
    // Traverse left subtree
    auto leftResult = postorderTraversal(root->left);
    result.insert(result.end(), leftResult.begin(), leftResult.end());

    // Traverse right subtree
    auto rightResult = postorderTraversal(root->right);
    result.insert(result.end(), rightResult.begin(), rightResult.end());

    // Visit the current node
    result.push_back(root->cus->result);

    return result;
}

    // void calculateFact(int fact[], int N)
    // {
    //         fact[0] = 1;
    //     for (long long int i = 1; i < N; i++) {
    //         fact[i] = fact[i - 1] * i;
    //     }
    // }

    // int nCr(int fact[], int N, int R)
    //     {
    //         if (R > N)
    //             return 0;
        
    //         // nCr= fact(n)/(fact(r)*fact(n-r))
    //         int res = fact[N] / fact[R];
    //         res /= fact[N - R];
        
    //         return res;
    //     }

//     int countWays(vector<int>& arr, int fact[])
//     {
//     // Store the size of the array
//     int N = arr.size();
 
//     // Base case
//     if (N <= 2) {
//         return 1;
//     }
 
//     // Store the elements of the
//     // left subtree of BST
//     vector<int> leftSubTree;
 
//     // Store the elements of the
//     // right subtree of BST
//     vector<int> rightSubTree;
 
//     // Store the root node
//     int root = arr[0];
 
//     for (int i = 1; i < N; i++) {
 
//         // Push all the elements
//         // of the left subtree
//         if (arr[i] < root) {
//             leftSubTree.push_back(
//                 arr[i]);
//         }
 
//         // Push all the elements
//         // of the right subtree
//         else {
//             rightSubTree.push_back(
//                 arr[i]);
//         }
//     }
 
//     // Store the size of leftSubTree
//     int N1 = leftSubTree.size();
 
//     // Store the size of rightSubTree
//     int N2 = rightSubTree.size();
 
//     // Recurrence relation
//     int countLeft
//         = countWays(leftSubTree,
//                     fact);
//     int countRight
//         = countWays(rightSubTree,
//                     fact);
 
//     return nCr(fact, N - 1, N1)
//            * countLeft * countRight;
// } 

int Mod = 1e9 + 7;
	//Calcute C_{n} choose k by Pascal's equation
	int C_NofK(int N, int K) {
		if (K > N / 2) K = N - K;	//C_N choose K =C_N choose N-K
		vector<int> C_N(K + 1, 0), prevC(K + 1, 0);
		for (int i = 0; i <= N; i++) {
			C_N[0] = 1;
			for (int j = 1; j <= min(i, K); j++) {
				C_N[j] = ((long)prevC[j - 1] + prevC[j]) % Mod;
			}
			prevC = C_N;
		}
		return C_N[K];
	}

long countPermutation(vector<int>& nums) {
	int n = nums.size();
	if (n <= 2) return 1;
	vector<int> left, right;
	int root = nums[0];
	for (int i = 1; i < n; i++) {
		if (root < nums[i]) right.push_back(nums[i]);
		else left.push_back(nums[i]);
	}
	long r = countPermutation(right), l = countPermutation(left);
	return C_NofK(n - 1, left.size()) * r % Mod * l % Mod;

}

NodeBST* findFirstCustomer(NodeBST* root){
    if(root == NULL){
       return NULL;
    }
    NodeBST* l = findFirstCustomer(root->left);
    NodeBST* r = findFirstCustomer(root->right);

    if(l == NULL && r == NULL){
        return root;    
    }else if(l == NULL){
        if(r->cus->timein < root->cus->timein){
            return r;
        }else{
            return root;
        }
    }else if(r == NULL){
        if(l->cus->timein < root->cus->timein){
            return l;
        }else{
            return root;
        }
    }else if(l->cus->timein < r->cus->timein){
        if(l->cus->timein < root->cus->timein){
            return l;
        }else{
            return root;
        }
    }else{
        if(r->cus->timein < root->cus->timein){
            return r;
        }else{
            return root;
        }
    }
}

NodeBST* findMin(NodeBST* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

NodeBST* deleteNode(NodeBST* root, NodeBST* node) {
    // if (root == NULL) {
    //     return NULL;
    // }
    // if(node == NULL)return root;
	// return root;

    // if (root->cus->result > node->cus->result) {
    //     root->left = deleteNode(root->left, node);
    //     return root;
    // } else if (root->cus->result < node->cus->result) {
    //     root->right = deleteNode(root->right, node);
    //     return root;
    // }
    // if (root->cus->timein > node->cus->timein) {
    //     root->left = deleteNode(root->left, node);
    //     return root;
    // } else if (root->cus->timein < node->cus->timein) {
    //     root->right = deleteNode(root->right, node);
    //     return root;
    // }
    // if (root->left == NULL) {
    //     NodeBST* temp = root->right;
    //     sizeBST--;
    //     if(root!=NULL)
    //     delete root;
    //     return temp;
    // } else if (root->right == NULL) {
    //     NodeBST* temp = root->left;
    //     sizeBST--;
    //     if(root!=NULL)
    //      delete root;
    //     return temp;
    // } else {
    //     NodeBST* succParent = root;
    //     NodeBST* succ = root->right;

    //     while (succ->left != NULL) {
    //         succParent = succ;
    //         succ = succ->left;
    //     }

    //     if (succParent != root) {
    //         succParent->left = succ->right;
    //     } else {
    //         succParent->right = succ->right;
    //     }

    //     root->cus->result = succ->cus->result;
    //     root->cus->timein = succ->cus->timein;
    //     sizeBST--;
    //     if(succ!=NULL)
    //     delete succ;
    //     return root;
    // }
	// Base case: If the tree is empty
    if (root == NULL) {
        return NULL;
    }

    // Recursive cases
    if (node->cus->result < root->cus->result) {
        root->left = deleteNode(root->left, node);
    } else if (node->cus->result > root->cus->result) {
        root->right = deleteNode(root->right, node);
    } else {
        // Node with only one child or no child
        if (root->left == NULL) {
            NodeBST* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == NULL) {
            NodeBST* temp = root->left;
            delete root;
            return temp;
        }

        // Node with two children: Get the inorder successor (smallest in the right subtree)
        NodeBST* temp = findMin(root->right);

        // Copy the inorder successor's data to this node
        root->cus->result = temp->cus->result;
        root->cus->timein = temp->cus->timein;

        // Delete the inorder successor
        root->right = deleteNode(root->right, temp);
    }

    return root;
}

void deleteYNode(){
	if(root == NULL) return;
    vector<int> result = postorderTraversal(root);
    // int N = result.size();
    // int fact[N];
    // calculateFact(fact, N);
    int Y = countPermutation(result);
	//cout << "so hoan vi: " << Y << endl;
    Y = Y%MAXSIZE;
	//cout << "Y: " << Y << endl;
	if(Y>sizeBST){
		Y = sizeBST;
	}
	//cout << "sizeBST: "<<sizeBST << endl;
    for(int i = 0; i < Y; i++){
        NodeBST* temp = findFirstCustomer(root);
		if(temp == NULL)break;
		//cout << "xoa ID "<< ID<<"  " <<temp->cus->result << endl;
        root = deleteNode(root,temp);
    }
}
void AddCustomer(Customer* cus){
		if(cus == NULL)return;
		//int ID = cus->result%MAXSIZE+1;
		sizeBST++;
		if(root == NULL){
			root = new NodeBST(cus);
		}else{
			NodeBST* temp = root;
			while(temp != NULL){
				if(temp->cus->result > cus->result){
					if(temp->left == NULL){
						temp->left = new NodeBST(cus);
						break;
					}else{
						temp = temp->left;
					}
				}else{
					if(temp->right == NULL){
						temp->right = new NodeBST(cus);
						break;
					}else{
						temp = temp->right;
					}
				}
			}
		}
}

void printBST(NodeBST* root){
    if(root == NULL)return;
    printBST(root->left);
    out << root->cus->result <<"\n";
    printBST(root->right);
}

void print2DUtil(NodeBST* root, int space)
        {
            if (root == NULL)
                return;
            space += COUNT;
            print2DUtil(root->right, space);
            cout << endl;
            for (int i = COUNT; i < space; i++)
                cout << " ";
            cout << root->cus->result << " "<< "\n";
            print2DUtil(root->left, space);
        }

        void print2D(NodeBST* root)
        {
            print2DUtil(root, 0);
        }
};
int timeID = 0;
class AreaS{
	public:
	int ID;
	int count;
    int timeArea;
	vector <Customer*> customers;
	public:
	AreaS(int ID){
		this->ID = ID;
		count = 0;
	}

	~AreaS() {
        for (Customer* cus : customers) {
            delete cus;
        }
	}

	void insertCustomer(Customer* cus){
		customers.push_back(cus);
		this->timeArea = timeID++;
        count++;
    }

    void printNumCustomer(int num){
		//cout << "count: " << count << endl;
        if(num > count){
            num = count;
        }
        for (int i = count-1; i >= count -  num; i--)
        {
            out << ID << "-" << customers[i]->result <<"\n";
        }
        
    }

    void print(Customer* cus){
        out << cus->result << "-" << ID << "\n";
    }
    
    void deleteCustomer(int num){
		this->timeArea = timeID++; 
		if(num > count){
			num = count;
		}
        for (int i = 0; i < num; i++)
        {
            if(!customers.empty()){
				//cout << "xoa customer: "; 
                print(customers.front());
                customers.erase(customers.begin());
                count--;
            }else{
                break;
            }
        }
    }

	
};

// ----------------------- lapse function ---------------------------//

class Restaurant {
	//vector<Area*> areas;
	public:
		Customer* lastCustomer=NULL;
		Restaurant() {};
		~Restaurant() {};
		
		public:
		virtual void AddCustomer(Customer*)=0;
		virtual void DeleteCustomer(int num)=0;
		virtual void DeleteCustomer()=0;
		void printHuffTree(minHeapNode* root){
				if (root == nullptr) {
					return;
   			}
			printHuffTree(root->left);													
			if(root->data != '$'){
			out << root->data << "\n" ;
			}else{
			out << root->freq << "\n" ;
			}
			printHuffTree(root->right);
		}
		virtual void printCustomer(int num)=0;
};



class GRestaurant:public Restaurant 
{
	public:
	vector<AreaG*> areas;
	GRestaurant(){
		areas.resize(MAXSIZE,NULL);
	}

	~GRestaurant() {
        for (AreaG* area : areas) {
            delete area;
        }
    }

	void AddCustomer(Customer* cus){
		if(cus == NULL)return;
		 int ID = cus->result%MAXSIZE+1;
		//int ID  = 1;
		if(areas[ID-1] == NULL){
			areas[ID-1] = new AreaG(ID);
			areas[ID-1]->AddCustomer(cus);
		}else{
			areas[ID-1]->AddCustomer(cus);
		}
	}
	void DeleteCustomer(int num){return;};
	void DeleteCustomer(){
		for (int i = 0; i < MAXSIZE; i++)
		{
			if(areas[i] != NULL){
				areas[i]->deleteYNode();
			}
		}
	}

	void printCustomer(int num){
		if(num<=0|| num > MAXSIZE) return;
		if(areas[num-1] == NULL)return;
		areas[num-1]->printBST(areas[num-1]->root);
	}
	void printTreeCustomer(int num){
		if(num<=0|| num > MAXSIZE) return;
		if(areas[num-1] == NULL) return;
		areas[num-1]->print2D(areas[num-1]->root);
	}
};

bool find(int ID, vector<AreaS*> areas){
	for(unsigned int i = 0; i < areas.size(); i++){
		if(areas[i]->ID == ID)return true;
	}
	return false;
}

struct greaters{ 
bool operator()(const AreaS* a,const AreaS* b) const{ 
    if(a->count == b->count){
		return a->timeArea > b->timeArea;
	} 
	else return a->count>b->count;
}	
};

struct sorters{ 
bool operator()(const AreaS* a,const AreaS* b) const{ 
    if(a->count == b->count){
		return a->timeArea < b->timeArea;
	} 
	else return a->count<b->count;
}	
};

int left(int i){return 2 * i + 1;}
    // Returns the right child
int right(int i){return 2 * i + 2;}
int parent(int i){return (i - 1) / 2;}

class SRestaurant:public Restaurant{
	public:
	vector<AreaS*> areas;
	void printCustomer(int num){return;};

	~SRestaurant() {
        for (AreaS* area : areas) {
            delete area;
        }
    }




    void AddCustomer(Customer* cus){
        if(cus == NULL)return;
		int ID = cus->result%MAXSIZE+1;
        if(!find(ID,areas)){
            AreaS* newArea = new AreaS(ID);
			areas.push_back(newArea);
            int index = areas.size()-1;
			areas[index]->insertCustomer(cus);
            ReHeapUp(index);

			// for(unsigned int i = 0; i < areas.size(); i++){
			// 	if(areas[i]->ID == ID){
			// 		// areas[i]->insertCustomer(cus);
			// 		reHeapDown(i);
			// 		// for (int i = 0; i < areas.size(); i++)
			// 		// 	{
			// 		// 		cout << areas[i]->ID << " " << areas[i]->count << endl;
			// 		// 	}
			// 		break;
			// 	}
			// }

        }
        else{
            for(unsigned int i = 0; i < areas.size(); i++){
				if(areas[i]->ID == ID){
					areas[i]->insertCustomer(cus);
					ReHeapDown(i);
					break;
				}
			}	
        }

    }

	void extractMin(unsigned int i){
    // Check if the heap is empty
	if(i>=areas.size()|| i<0)return;
    if(areas.size() == 0){
      //  cout << "EMPTY HEAP" << endl;
        return;
    // Check if there is only 1 element
    }else if(areas.size() == 1){
		areas.pop_back();
        return ;
    // Normal extraction
    }else{
        areas[i] = areas[areas.size() - 1];
		areas.pop_back();
        ReHeapDown(i);

        // Return min element
        return;
    }
}

int checkIndex(int idx1, int idx2)
{
    if (areas[idx1]->count > areas[idx2]->count)
        return idx2;
    else if (areas[idx1]->count == areas[idx2]->count)
    {
        if (areas[idx1]->timeArea > areas[idx2]->timeArea)
            return idx2;
        else
            return idx1;
    }
    else
        return idx1;
}

	void ReHeapDown(int idx)
{	
	int sizearea = areas.size();
    while (idx < sizearea / 2)
    {
        int temp = idx;
        int left = idx * 2 + 1;
        int right = left + 1;
        int min = 0;

        if (right >= sizearea)
            min = left;
        else
            min = checkIndex(left, right);

        if (areas[min]->count < areas[idx]->count)
            idx = min;
        else if (areas[min]->count == areas[idx]->count)
        {
            if (areas[idx]->timeArea > areas[min]->timeArea)
                idx = min;
        }

        if (idx == temp)
            return;
        else
        {
            swap(areas[temp], areas[idx]);
        }

        if (right >= sizearea)
            return;
    }
}
	
	void ReHeapUp(int idx)
	{
    while (idx > 0)
    {
        int par = (idx - 1) / 2;

        if (areas[par]->count > areas[idx]->count)
        {
            swap(areas[par], areas[idx]);
            idx = par;
        }
        else if (areas[par]->count == areas[idx]->count)
        {
            if (areas[par]->timeArea > areas[idx]->timeArea)
            {
                swap(areas[par], areas[idx]);
                idx = par;
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
    }
}

	void DeleteCustomer(){return;};

	int findIndex(int ID){
		for(unsigned int i = 0; i < areas.size(); i++){
			if(areas[i]->ID == ID)return i;
		}
		return -1;
	}
	void DeleteCustomer(int num){
		if(num<=0)return;
		int numarea = num;
		int size = areas.size();
		if(numarea >= size){
			numarea = size;
		}
		vector<AreaS*> sortedAreas = areas;

		sort(sortedAreas.begin(), sortedAreas.end(), sorters());

		for(int i = 0 ; i < numarea;i++){
			sortedAreas[i]->deleteCustomer(num);
			unsigned int index = findIndex(sortedAreas[i]->ID);
			if(areas[index]->count == 0){ // nếu xóa areas
				extractMin(index);	
			}else{
				ReHeapUp(index);      // neu ko xoa areas
			}

		}

	}

	void printHeap(){
		out << "---------------------------------------------------"<< endl;
		for (unsigned int i = 0; i < areas.size(); i++)
		{
			out <<"[ID " <<areas[i]->ID << " COUNT " << areas[i]->count <<" TIME " << areas[i]->timeArea << "]\t";
		}
		out << "\n---------------------------------------------------"<< endl;
	}

// in ra num khách hàng trong min Heap theo thứ tự preorder
	void printCustomer(int num,int index){
		if(num<=0||areas.size()==0)return;
		int size = areas.size();
		if(index>= size){
			return;
		}
		areas[index]->printNumCustomer(num);

		printCustomer(num,2*index+1);
		printCustomer(num,2*index+2);
	}

};

int differenceCharacter(string name){
	unordered_set<char> uniqueChars;

    for (char ch : name) {
        // Insert the character into the set
        uniqueChars.insert(ch);
    }
	return uniqueChars.size();
}

class imp_res:Restaurant
{
	public:
		// khai báo thuộc tính
		GRestaurant* G_restaurant;
		SRestaurant* S_restaurant;
	public:	
		imp_res() {
			srand (time(NULL));
			G_restaurant = new GRestaurant();
			S_restaurant = new SRestaurant();
		}
		~imp_res() {
			// Delete the dynamically allocated memory
			delete G_restaurant;
			delete S_restaurant;
    	}
		void AddCustomer(Customer* cus){
			return;
		}
		void DeleteCustomer(){
			return;
		}
		void DeleteCustomer(int num){
			return;
		}
		void printCustomer(int num){
			return;
		}
		//int static inline dem = 1;

		void LAPSE(string name){
			Customer* new_customer = new Customer(name);
			int diff = differenceCharacter(name);
			if(diff < 3){
			//	cout << "da xoa customer khong hop le" << endl;	
				delete new_customer;
				return;
			}
			if(new_customer->size == 1){
				new_customer->result = 0;
			}else if(new_customer->root->data != '$'){
				delete new_customer;
				return;
			}
			lastCustomer = new_customer;
			out <<"[Debug] Result=" << new_customer->result << endl;
			
			// cout << new_customer->result << endl;
			// cout<< "--------------------------------------------" << endl;
			// new_customer->print2D(new_customer->root);

				//new_customer->result = (rand()%100)*2;

			if(new_customer->result % 2 == 1){
				G_restaurant->AddCustomer(new_customer);
				// int ID = new_customer->result%MAXSIZE+1;
				//  cout << "AREA      " << ID << endl;
				//  cout << "ADD CUSTOMER " << new_customer->result << endl;
                //  cout <<"Tree ID "<< ID << "----------------------------------------------"<< endl;
				//  G_restaurant->printTreeCustomer(ID);
				//  cout << "----------------------------------------------" << endl;
                // vector <int> result = G_restaurant->areas[ID-1]->postorderTraversal(G_restaurant->areas[ID-1]->root);

                // for (int i = 0; i < result.size(); i++)
                // {
                //     cout << result[i] << " ";
                // }
                
				// cout << "GRestaurant: "<< ID << endl;
				// cout << G_restaurant->areas[ID-1]->root->cus->name << endl;

			}else{
				out << "SRestaurant: ID "<< new_customer->result%MAXSIZE+1 <<"  Customer: " << new_customer->result <<endl;
				S_restaurant->AddCustomer(new_customer);
				// for (int i = 0; i < S_restaurant->areas.size(); i++)
				// {
				// 	for (int j = 0; j < S_restaurant->areas[i]->count; j++)
				// 	{
				// 		cout << S_restaurant->areas[i]->customers[j]->result <<" ";
				// 	}
				// 	cout << endl;
				// }
				
				S_restaurant->printHeap();
				// cout << S_restaurant->areas[0]->customers[0]->name << endl;
			}
		} ;
		void KOKUSEN(){
			// cout <<"truoc khi xoa -----------------------------------------------------"<< endl;
			// for (int i = 0; i < G_restaurant->areas.size(); i++)
			// {
			// 	G_restaurant->printTreeCustomer(i+1);
			// 	cout <<"-----------------------------------------------"<< endl;
			// }
			// cout << "delete customer ----------------------------------------" << endl;
			G_restaurant->DeleteCustomer();
			// cout <<"sau khi xoa ----------------------------------------------------"<< endl;
			// for (int i = 0; i < G_restaurant->areas.size(); i++)
			// {
			// 	G_restaurant->printTreeCustomer(i+1);
			// 	cout <<"-----------------------------------------------"<< endl;	
			// }
			
		}
		void KEITEIKEN(int num){
			S_restaurant->DeleteCustomer(num); // xong
		}
		void HAND(){
            if(lastCustomer == NULL)return; 
			printHuffTree(lastCustomer->root); // xong
		}
		void LIMITLESS(int num){
			//cout << "LIMITNESS" << endl;
			//G_restaurant->printTreeCustomer(num);// xong
			G_restaurant->printCustomer(num);
		}
		void CLEAVE(int num){
			//S_restaurant->printHeap();
			//cout << "CLEAVE" << endl;
			S_restaurant->printCustomer(num,0);
		}

};


void simulate(string filename)
{
	imp_res* r = NULL;
	ifstream ss(filename);
	string str, maxsize, name, num;
	while(ss >> str)
	{ 
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			out << "MAXSIZE " << maxsize << endl;
			MAXSIZE = stoi(maxsize); 
			r = new imp_res();
    	}
        else if(str == "LAPSE")
        {
            ss >> name;
			out << "LAPSE " << name << endl;
			//cout << "LAPSE "<< endl;
			r->LAPSE(name); // xong
    	}
    	else if(str == "KOKUSEN") 
    	{
				out << "KOKUSEN" << endl; 
    			r->KOKUSEN(); // xong
		}
    	else if(str == "KEITEIKEN") 
    	{
				ss >> num;
				out << "KEITEIKEN " <<num <<endl;
    			r->KEITEIKEN(stoi(num)); // xong
		}
		else if(str == "HAND") 
		{
			out << "HAND"<< endl;
    		r->HAND();	
		}
    	else if(str == "LIMITLESS") 
     	{   
				ss >> num;	
				out << "LIMITLESS " << num << endl;
    			r->LIMITLESS(stoi(num));
    	}
    	else if(str == "CLEAVE") 
    	{
				ss >> num;
				out << "CLEAVE " << num << endl;
    			r->CLEAVE(stoi(num));
    	}
    }
	//cout << "Good Luck";
	ss.close();
	delete r;
	return;
}

int main(int argc, char* argv[]) {
    string fileName = "test.txt";
    simulate(fileName);

    return 0;
}