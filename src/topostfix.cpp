#include<bits/stdc++.h>
#include "topostfix.hpp"
#include <QDebug>

using namespace std;

string translate(string infix){
    string postfix = "";
    stack <char> myStack;
    char ch;

    for(int i=0; infix[i]; i++){
        ch = infix[i];

        if(ch=='(') //if found opening bracket
            myStack.push(ch);
        else if(ch==')'){ //if found closing bracket
            while(!myStack.empty() && myStack.top()!='('){
                postfix = postfix + myStack.top();
                myStack.pop();
            }

            if(!myStack.empty() && myStack.top()=='(')
               myStack.pop();
        }else{ //found operator or operand
            int priority = operatorPrecedence(ch);

            if(priority==0) //found operand
                postfix = postfix + ch;
            else{ //found operator
                if(myStack.empty())
                    myStack.push(ch);
                else{
                    while(!myStack.empty() && myStack.top()!='(' && priority<=operatorPrecedence(myStack.top())){
                        postfix = postfix + myStack.top();
                        myStack.pop();
                    }
                    myStack.push(ch);
                }
            }
        }
    }

    while(!myStack.empty()){
        postfix += myStack.top();
        myStack.pop();
    }

    return postfix;
}

int operatorPrecedence(char ch){
    if(ch=='+' || ch=='-')
        return 1;
    else
        return 0;
}
