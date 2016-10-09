#include <bits/stdc++.h>
using namespace std;

struct transition{
    int from;
    int to;
    char symbol;
};

class AFND{
public:
    vector<int> vertex;
    vector<transition> transitions;
    int final_state;

    AFND(){
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
    int size(){
        return vertex.size();
    }
    void print(){
        transition tempTrans;
        for(int i = 0; i<transitions.size() ; i++){
            tempTrans = transitions[i];
            cout << tempTrans.from << " " + "->" << tempTrans.symbol << " " + "->" << tempTrans.to << endl;
        }
    }
};
