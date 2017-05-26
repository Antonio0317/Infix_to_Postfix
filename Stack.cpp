#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

#define LEFT_PARENTHESIS '('
#define RIGHT_PARENTHESIS ')'

using namespace std;

class Stack {
	private: 
		class StackNode {
			public :
				char op;
				StackNode* preNode;
				StackNode* NextNode;

				StackNode(char op = ' ');	//인자가 있으면 그 문자로 없으면 공백으로 생성
				~StackNode();
		};
		StackNode* first;
		StackNode* top;
	public:
		void push(char data);
		bool Is_Empty();
		void pop();
		char Top();
		Stack();
		~Stack();
};

class Postfix {
	private:
		string postord;
	public:
		int IsPrior(char op1, char op2);
		int GetPriority(char op, int InStack);
		void GetPostfix(string Infix);
		Postfix();
};

int main () {
	Stack test;
	string infix;
	while(cin >> infix) {
		Postfix* postfix = new Postfix();
		postfix->GetPostfix(infix);
		delete postfix;
	}
	return 0;
}

Stack::StackNode::StackNode(char op) {		//스택노드 생성자
	this->op = op;		//op를 초기화
	preNode = NULL;		
	NextNode = NULL;	//preNode랑 NextNode를 NULL로 초기화
}

Stack::Stack() {	//스택 생성자
	first = new StackNode();	//새로운 스택 노드를 만들고
	top = first;	
	first->preNode = first;		//자기 자신을 가리키게 함
}


void Stack::push(char data) {	
	StackNode* NewNode = new StackNode(data);	//인자로 받은 값으로 새로운 노드를 만들고 
	NewNode->NextNode = NULL;	//노드의 다음 노드를 널로 초기화
	top->NextNode = NewNode;	//탑의 다음노드는 NewNode를 하고
	NewNode->preNode = top;		//탑의 전 노드는 top
	top = NewNode;	//탑에는 NewNode를 넣어준다.
}

bool Stack::Is_Empty() {	
	if(first->NextNode == NULL) {	//널이면
		return 1;	//비었다.
	}
	else{	//아니면
		return 0;	//안비었다.
	}
}

void Stack::pop() {		
	if(first == top) {	//first랑 탑이랑 같으면
		first -> NextNode = NULL;
		top -> NextNode = NULL;
	}	//둘다 널로 초기화
	else {	//아니면
		top = top->preNode;		//탑을 탑의 전노드로 이동하고
		delete top->NextNode;	//탑의 NextNode를 delete하고
		top->NextNode = NULL;	//탑의 NextNode를 널로 초기화
	}
}

char Stack::Top() {
	if(!Is_Empty()) {
		return top->op;		//탑에 있는 문자를 리턴
	}
}

Stack::~Stack() {
	while(!Is_Empty()) {
		pop();
	}
	delete first;
}

Stack::StackNode::~StackNode() {	
}

int Postfix::GetPriority(char op1, int InStack) {	//우선순위 계산
	int Priority = -1;		//초기화
	switch (op1) {		
		case LEFT_PARENTHESIS:	//'('가 오면
			if( InStack) {	//스택 안에 있을땐
				Priority = 3;	//우선 순위 최하
			}
			else {	//밖에있으면
				Priority = 0;	//우선 순위 최고
			}
			break;	
		case '*':
		case '/':
			Priority = 1;	
			break;

		case '+':
		case '-':
			Priority = 2;
			break;
	}
	return Priority;	//우선순위 값을 리턴
}

Postfix::Postfix() {
	postord = "";
}

int Postfix::IsPrior(char opInStack, char opInToken) {
	return (GetPriority(opInStack, 1) <= GetPriority(opInToken, 0));		//
}

void Postfix::GetPostfix(string infix) {
	int minus_flag = 0, i = 0;
	Stack stack;
	int length = infix.length();
	if(infix[0] == '-') {
		minus_flag = 1;
		i=1;
	}
	else if (infix[0] == '+') {
		i=1;
	}
	for(i ; i< length ; i++){
		if(isdigit(infix[i])) {
			if(minus_flag) {
				postord += '-';
				minus_flag = 0;
			}
			while(isdigit(infix[i])) {
				postord += infix[i++];
			}
			i--;
			postord += ' ';
		}
		else if(infix[i] == RIGHT_PARENTHESIS) {
			while(stack.Top() != LEFT_PARENTHESIS) {
				postord += stack.Top();
				postord += ' ';
				stack.pop();
			}
			stack.pop();
		}
		else {
			if(infix[i] == '+') {
				if(infix[i-1] == LEFT_PARENTHESIS || infix[i-1] == '*' || infix[i-1] == '/' || infix[i-1] == '-' || infix[i-1] == '+') {
					continue;
				}
			}
			else if (infix[i] == '-') {
				if(infix[i-1] == LEFT_PARENTHESIS || infix[i-1] == '*' || infix[i-1] == '/' || infix[i-1] == '+' || infix[i-1] == '-') {
					minus_flag=1;
					continue;
				}
			}
			while(!stack.Is_Empty() && IsPrior(stack.Top(), infix[i])) {
				postord += stack.Top();
				postord += ' ';
				stack.pop();
			}
			stack.push(infix[i]);
		}
	}
	while(!stack.Is_Empty()) {
		postord += stack.Top();
		postord += ' ';
		stack.pop();
	}
	cout << postord << endl;
}
