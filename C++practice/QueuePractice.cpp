#include <iostream>
#include <cstring>
using namespace std;

class Node {
public:
	int data;
	Node* next;
public:
	Node() {}
};

class Queue {
public:
	Node* Head;
	Node* Tail;
	Node* Cur;
	int size;
public:
	Queue() : Head(NULL), Tail(NULL), Cur(NULL), size(0) {}
	void push(int x) {
		this->size++;
		Node* newNode = new Node;
		newNode->data = x;
		newNode->next = Head;
		Head = newNode;
		if (Head->next == NULL) Tail = Head;
	}
	int pop() {
		if (Head == Tail) {
			if (Head == NULL) {
				return -1;
			}
			else {
				int temp = Head->data;
				Head = Tail = NULL;
				this->size--;
				return temp;
			}
		}
		Cur = Head;
		while (Cur->next != Tail) {
			Cur = Cur->next;
		}
		int temp = Tail->data;
		Tail = Cur;
		this->size--;
		return temp;
	}
	int isEmpty() {
		if (Head == Tail && Head == NULL) return 1;
		else return 0;
	}
	int front() {
		if (isEmpty()) return -1;
		else return Tail->data;
	}
	int back() {
		if (isEmpty()) return -1;
		else return Head->data;
	}
	/*void showQueue() {
		Cur = Head;
		while (Cur != NULL) {
			cout << Cur->data << " ";
			Cur = Cur->next;
		}
		cout << endl;
	}*/
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int N;
	char query[10];
	int query_int;
	Queue q;
	cin >> N;
	
	for (int i = 0; i < N; i++) {
		cin >> query;
		switch (query[1]) {
		case 'u': // push X
			cin >> query_int;
			q.push(query_int);
			//q.showQueue();
			break;
		case 'o': // pop
			cout << q.pop() << '\n';
			break;
		case 'i': // size
			cout << q.size << '\n';
			break;
		case 'm': // empty
			cout << q.isEmpty() << '\n';
			break;
		case 'r': // front
			cout << q.front() << '\n';
			break;
		case 'a': // back
			cout << q.back() << '\n';
			break;
		}
	}
}