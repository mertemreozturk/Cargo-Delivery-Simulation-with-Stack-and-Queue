/*Function templates are special functions that can operate 
with generic types. This allows us to create a function 
template whose functionality can be adapted to more than 
one type or class without repeating the entire code for each type.*/

/*To use this function template we use the following format for the function call:

function_name <type> (parameters);
*/

using namespace std;

template<typename T>
class Queue {
	public:
	string name;
	Queue(){
		_firstNode = nullptr, _lastNode = nullptr;
		_size = 0;
	}
	~Queue(){

	}
	bool isEmpty() const{
		if ( _lastNode == nullptr){
			return true;
		}else{
			return false;
		}
	}
	int size() const{
		return _size;
	}
	//Enqueue: Items are added to the back of the queue
	void enqueue(const T& newItem){
		QueueNode* newNode = new QueueNode();
		newNode->item = newItem;
		newNode->next = nullptr;
		if ( isEmpty()){
			_firstNode = newNode;
			_lastNode = newNode;
		}else{
			_lastNode->next = newNode;
			_lastNode = newNode;
		}
		_size++;
	}
	//Dequeue: Items are removed from the front of the queue
	void dequeue(){
		if ( !isEmpty()){ 
			QueueNode* temp = _firstNode;
			_firstNode = _firstNode->next;
			delete temp;
			_size--;
		}
	}
	//Get Front: Take a look at the first item
	void getFront(T& queueTop) const{
		if ( _firstNode != nullptr){
			queueTop = _firstNode->item;
		}
	}

	void print(ofstream& fout){
		while ( _firstNode != nullptr){   // print element and then remove that from queue
			fout << _firstNode->item.name << endl;
			dequeue();
		}
	}
	private:
	struct QueueNode {
		T item;
		QueueNode *next;
		};
	int _size;
	/* to avoid the traversal to the last node, 
	 an additional pointer to the last node is defined*/
	QueueNode *_lastNode;
	QueueNode *_firstNode;
};
