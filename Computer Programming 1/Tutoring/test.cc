#include <iostream>
#include <string.h>
using namespace std;

void push(int &, char *);
char *pop(int &);
int evaluate(int&);

const int MAX_SIZE = 10;
char *stack[MAX_SIZE];

int main(int argc, char *argv[]) {
	if(argc==1){
		cout<<"Too few arguments"<<endl;
	} else if (argc>=MAX_SIZE){
		cout<<"Too many arguments"<<endl;
	}
	
	int last=-1;
	for (int i=1; i<argc; i++){
		push(last, argv[i]);
	}
	
	
	int res =evaluate(last);
	cout<<"The result of the operation is "<<res;
	return 0;
}


void push (int &last, char *number) {
	last+=1;
	stack[last]=number;
}

char *pop (int &last) {
	char *number = stack[last];
	stack[last]=0;
	last-=1;
	return number;
}
	
int evaluate (int &last) {
	
	char *p=pop(last);
    cout << p;
	
	if(p[0]=='+'||p[0]=='-'||p[0]=='*'||p[0]=='/') {
	
		int v1=evaluate(last);
		int v2=evaluate(last);
		
		switch(p[0]) {
			case '+':
			    return v1+v2;
			    break;
			case '-':
			    return v1-v2;
			    break;
			case '*':
			    return v1*v2;
                break;
			case '/':
			    return v1/v2;
                break;
		}
        return 0;
	} 
	else{
		int res=stoi(p);
		return res;
        return 0;
	}
}