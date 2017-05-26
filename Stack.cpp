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

				StackNode(char op = ' ');	//���ڰ� ������ �� ���ڷ� ������ �������� ����
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

Stack::StackNode::StackNode(char op) {		//���ó�� ������
	this->op = op;		//op�� �ʱ�ȭ
	preNode = NULL;		
	NextNode = NULL;	//preNode�� NextNode�� NULL�� �ʱ�ȭ
}

Stack::Stack() {	//���� ������
	first = new StackNode();	//���ο� ���� ��带 �����
	top = first;	
	first->preNode = first;		//�ڱ� �ڽ��� ����Ű�� ��
}


void Stack::push(char data) {	
	StackNode* NewNode = new StackNode(data);	//���ڷ� ���� ������ ���ο� ��带 ����� 
	NewNode->NextNode = NULL;	//����� ���� ��带 �η� �ʱ�ȭ
	top->NextNode = NewNode;	//ž�� �������� NewNode�� �ϰ�
	NewNode->preNode = top;		//ž�� �� ���� top
	top = NewNode;	//ž���� NewNode�� �־��ش�.
}

bool Stack::Is_Empty() {	
	if(first->NextNode == NULL) {	//���̸�
		return 1;	//�����.
	}
	else{	//�ƴϸ�
		return 0;	//�Ⱥ����.
	}
}

void Stack::pop() {		
	if(first == top) {	//first�� ž�̶� ������
		first -> NextNode = NULL;
		top -> NextNode = NULL;
	}	//�Ѵ� �η� �ʱ�ȭ
	else {	//�ƴϸ�
		top = top->preNode;		//ž�� ž�� ������ �̵��ϰ�
		delete top->NextNode;	//ž�� NextNode�� delete�ϰ�
		top->NextNode = NULL;	//ž�� NextNode�� �η� �ʱ�ȭ
	}
}

char Stack::Top() {
	if(!Is_Empty()) {
		return top->op;		//ž�� �ִ� ���ڸ� ����
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

int Postfix::GetPriority(char op1, int InStack) {	//�켱���� ���
	int Priority = -1;		//�ʱ�ȭ
	switch (op1) {		
		case LEFT_PARENTHESIS:	//'('�� ����
			if( InStack) {	//���� �ȿ� ������
				Priority = 3;	//�켱 ���� ����
			}
			else {	//�ۿ�������
				Priority = 0;	//�켱 ���� �ְ�
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
	return Priority;	//�켱���� ���� ����
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
