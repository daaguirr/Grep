#include <bits/stdc++.h>
using namespace std;

const char epsilon = '#';

struct transition{
    int from;
    int to;
    char symbol;     // NOTA : '#' SERA EL SIMBOLO EPSILON
};

class AFND{
public:
    vector<int> vertex;
    vector<transition> transitions;
    int final_state;

    AFND(){

    }
    AFND(int state_number){
        for(int i = 0; i < state_number ; i++){
            vertex.push_back(i);
        }
        final_state = state_number - 1;
    }

    vector<int> getVertex(){
        return vertex;
    }

    int getFinalState(){
        return final_state;
    }

    void setFinalState(int newState){
        final_state = newState;
    }
    void addTrasition(int vertexFrom , int vertexTo , char symbol){
        transition newTrans;
        newTrans.from = vertexFrom;
        newTrans.to = vertexTo;
        newTrans.symbol = symbol;
        transitions.push_back(newTrans);
    }
    int length(){
        return vertex.size();
    }
    void print(){
        transition tempTrans;
        for(unsigned int i = 0; i<transitions.size() ; i++){
            tempTrans = transitions[i];
            cout << tempTrans.from << " -> " << tempTrans.symbol << " -> " << tempTrans.to << endl;
        }
    }

    vector<int> epsilonC(int state, vector<bool> visitados){
        vector<int> ans;
        if(visitados[state]) return ans;

        vector<bool> visitadostemp = visitados;
        visitadostemp[state] = 1;
        ans.push_back(state);
        for(transition tra: transitions){
            if(tra.from == state && tra.symbol == epsilon){
                vector<int> temp = epsilonC(tra.to, visitadostemp);
                for(int a: temp){
                    ans.push_back(a);
                }
            }
        }
        return ans;
    }

    vector<int> epsilonClausure(int state){
        vector<bool> visitados(vertex.size(), 0);
        return this -> epsilonC(state, visitados);
    }
};


AFND concat(AFND A, AFND B){
    AFND ans (A.length() + B.length());
    transition tempTrans;

    //Primera parte del AFND
    for(unsigned int i = 0; i< A.transitions.size() ; i++){
        tempTrans = A.transitions[i];
        ans.addTrasition(tempTrans.from, tempTrans.to, tempTrans.symbol);
    }

    //Transicion Epsilon
    ans.addTrasition(A.final_state , A.length() , '#');

    //SEGUNDA PARTE CORRIDA
    for(unsigned int i = 0; i< B.transitions.size() ; i++){
        tempTrans = B.transitions[i];
        ans.addTrasition(tempTrans.from + A.length() , tempTrans.to + A.length() , tempTrans.symbol);
    }

    return ans;
}

AFND cup (AFND A, AFND B){
    AFND ans (A.length() + B.length()+2);
    transition tempTrans;

    // Ver Documentacion para orden

    ans.addTrasition(0, 1 , '#');
    ans.addTrasition(A.final_state, A.length() + B.length() + 1 , '#');
    ans.addTrasition(0, A.length() + 1 , '#');
    ans.addTrasition( A.length() + B.final_state , A.length() + B.length() + 1 , '#');


    for(unsigned int i = 0 ; i < A.transitions.size(); i++) {
        tempTrans = A.transitions[i];
        ans.addTrasition(tempTrans.from + 1 , tempTrans.to + 1 , tempTrans.symbol);
    }

    for(unsigned int i = 0 ; i < B.transitions.size(); i++) {
        tempTrans = B.transitions[i];
        ans.addTrasition(tempTrans.from + A.length() + 1 , tempTrans.to + A.length() + 1 , tempTrans.symbol);
    }

    return ans;
}

AFND kleene(AFND A){
    AFND ans (A.length() + 2);
    transition tempTrans;

    ans.addTrasition(0, 1 , '#');
    ans.addTrasition(0, A.length() + 1 , '#');
    ans.addTrasition(A.final_state , A.length() + 1 , '#');
    ans.addTrasition(A.final_state , 1 , '#');

    for(unsigned int i = 0 ; i < A.transitions.size(); i++) {
        tempTrans = A.transitions[i];
        ans.addTrasition(tempTrans.from + 1 , tempTrans.to + 1 , tempTrans.symbol);
    }

    return ans;
}

AFND fromERtoAFND(string er){
    stack<char> operadores;
 	stack<AFND> operandos;
    AFND *new_afnd;

 	char currentLetter;

 	for(string::iterator it = er.begin() ; it != er.end() ; ++it){
        currentLetter = *it;
        //CASO LETRA
        if(currentLetter != '(' && currentLetter != ')' && currentLetter != '*' && currentLetter != '|' && currentLetter != '.') {
 			//CONSTRUCTOR TROLL
 			new_afnd = new AFND (2);
            (*new_afnd).addTrasition(0, 1, currentLetter);
            (*new_afnd).addTrasition(1,0, epsilon);
            operandos.push(*new_afnd);
            delete new_afnd;
        } else {
 		    //OPERADORES
 			if(currentLetter == '*') {
                AFND toKleen = operandos.top();
 				operandos.pop();
 				operandos.push(kleene(toKleen));
 			} else if(currentLetter == '|') {
 				operadores.push(currentLetter);
 			} else if(currentLetter == '.') {
 				operadores.push(currentLetter);
 			} else if(currentLetter == '(') {
 				operadores.push(currentLetter);
 			} else {
 			    //CIERRO PARENTESIS
                char chartemp = operadores.top();
                operadores.pop();
                //if(chartemp == '(') continue;
                AFND A = operandos.top();
                operandos.pop();
                AFND B = operandos.top();
                operandos.pop();
                if(chartemp == '.') operandos.push(concat(A,B));
                if(chartemp == '|') operandos.push(cup(A,B));
            }
        }
    }
 	return operandos.top();
}

vector<vector<int> >powerSet(vector<int> conj){
    vector<vector<int> > ans;
    return ans;
}

template <typename T>
inline vector<T> setMinus(vector<T>, vector<T>) {
    vector<T> ans;
    return ans;
}


class AFD{
public:
    vector<vector<int> > states;
    vector<transition> transitions;
    vector<int> finalstates;
    int init_state;

    AFD(AFND A){
        // ARREGLAR CON UN MAP POR BUSQUEDAS EFICIENTES
        vector<int> initTemp;
        for(int i = 0; i< A.length() ; i++) initTemp.push_back(i);
        states = powerSet(initTemp);

        vector<int> epTemp = A.epsilonClausure(0);
        for(unsigned int i = 0 ; i<states.size() ; i++){
            vector<int> temp1 = states[i];
            if(true /* buscar temp 1 los estados*/) init_state = i;
        }

        vector<vector<int> > finalstatestemp = states;
        vector<int> temp2 (1,A.final_state);
        vector<int> temp3= setMinus(initTemp,temp2);
        vector<vector<int> > temp4 = powerSet(temp3);
        finalstatestemp = setMinus(finalstatestemp, temp4);
        for(vector<int> state : states){ //falta for para los chars
            for(int state2 :  state){
                //ans = (existstransition(state2, c , q') ? : cup(ans, A.epsilonClausure(q1)) ans;
            }
            // transitions.push_back(id(Q) , c , id(ans))
        }

    }
    void addTransition(int from, int to, char c){
        transition temp;
        temp.from = from;
        temp.to = to;
        temp.symbol = c;
        transitions.push_back(temp);
    }

};

int main(){
    return 0;
}
