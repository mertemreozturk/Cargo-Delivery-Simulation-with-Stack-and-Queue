/*Function templates are special functions that can operate 
with generic types. This allows us to create a function 
template whose functionality can be adapted to more than 
one type or class without repeating the entire code for each type.*/

/*To use this function template we use the following format for the function call:

function_name <type> (parameters);

*/

using namespace std;
template <typename T>
class Stack {
	public:
	string name;
	Stack(){
		_head = nullptr;
		_size = 0;
	}
	~Stack(){

	}
	bool isEmpty() const{
		if ( _head == nullptr){
			return true;
		}else{
			return false;
		}
	}

	int size() const{
		return _size;
	}
	
	//Push: Place item on top of the stack
	void push(const T& newItem){
		ListNode* newNode = new ListNode();
		newNode->item = newItem;
		if ( isEmpty()){  // add first element
			_head = newNode;
			newNode->next = nullptr;
		}else{
			newNode->next = _head;      // connect with formal top
			_head = newNode;   // new top is new node
		}
		_size++;
	}
	//Top: Take a look at the topmost item without removing it
	void getTop(T& stackTop) const{
		if ( _head != nullptr){
			stackTop = _head->item;
		}
		
	}
	//Pop: Remove item from the top of the stack
	void pop(){
		if ( !isEmpty()){  /// if list not empty, remove it from stack
			ListNode* temp;
			temp = _head;   // this will deleted
			_head = _head->next;    // new top
			delete temp;   // remove
			_size--;
		}
	}

	void print(ofstream& fout){
		while ( _head != nullptr){    // print element and then remove stack that
			fout << _head->item.name << endl;
			pop();
		}
	}
	private:
	struct ListNode {
		T item;
		ListNode* next;
		};
	ListNode* _head;
	int _size;
};
	
	
