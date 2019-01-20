/*
 * The MIT License (MIT)
 * Copyright (c) 2019 Matheus Scattolin Anselmo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * The Software is provided “as is”, without warranty of any kind, express or implied,
 * including but not limited to the warranties of merchantability, fitness for a
 * particular purpose and noninfringement. In no event shall the authors or copyright
 * holders be liable for any claim, damages or other liability, whether in an action
 * of contract, tort or otherwise, arising from, out of or in connection with the
 * software or the use or other dealings in the Software.
 */

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
