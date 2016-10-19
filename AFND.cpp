#include <bits/stdc++.h>
using namespace std;
typedef set<int> si;
typedef vector<si> vsi;
typedef long long intt;

const char epsilon = '#';
set<char> sigma;

struct transition{
    int from;
    int to;
    char symbol;     // NOTA : '#' SERA EL SIMBOLO EPSILON
};

vector<si> MEM;
vector<bool> init_mem;
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

    set<int> epsilonC(int state, vector<bool> visitados){
        set<int> ans;
        if(init_mem[state]) return MEM[state];
        if(visitados[state]) return ans;

        vector<bool> visitadostemp = visitados;
        visitadostemp[state] = 1;
        ans.insert(state);
        for(transition tra: transitions){
            if(tra.from == state && tra.symbol == '#'){
                set<int> temp = epsilonC(tra.to, visitadostemp);
                for(int a: temp){
                    ans.insert(a);
                }
            }
        }

        init_mem[state] = true;
        MEM[state] = ans;
        return ans;
    }

    set<int> epsilonClausure(int state){
        vector<bool> visitados(vertex.size(), 0);
        if(MEM.empty()){
			si temp;
			for(int i=0 ; i<this -> length() ; i++) MEM.push_back(temp);
			for(int i=0 ; i<this -> length() ; i++) init_mem.push_back(false);
		}
        return this -> epsilonC(state, visitados);
    }
};


AFND concat(AFND A, AFND B){
    AFND ans (A.length() + B.length()-1);
    transition tempTrans;

    //Primera parte del AFND
    for(unsigned int i = 0; i< A.transitions.size() ; i++){
        tempTrans = A.transitions[i];
        ans.addTrasition(tempTrans.from, tempTrans.to, tempTrans.symbol);
    }

    //Transicion Epsilon
    //ans.addTrasition(A.final_state , A.length() , '#');

    //SEGUNDA PARTE CORRIDA
    for(unsigned int i = 0; i< B.transitions.size() ; i++){
        tempTrans = B.transitions[i];
        ans.addTrasition(tempTrans.from + A.length() - 1 , tempTrans.to + A.length() - 1 , tempTrans.symbol);
    }

    return ans;
}

AFND cup (AFND A, AFND B){
    AFND ans (A.length() + B.length()+2);
    transition tempTrans;

    // Ver Documentacion para orden

    ans.addTrasition(0, 1 , '#');
    ans.addTrasition(A.final_state + 1, A.length() + B.length() + 1 , '#');
    ans.addTrasition(0, A.length() + 1 , '#');
    ans.addTrasition( A.length() + B.final_state + 1, A.length() + B.length() + 1 , '#');


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
    ans.addTrasition(A.final_state+1, A.length() + 1 , '#');
    ans.addTrasition(A.final_state+1 , 1 , '#');

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
            //(*new_afnd).addTrasition(1,0, epsilon);
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
                if(chartemp == '.') operandos.push(concat(B,A));
                if(chartemp == '|') operandos.push(cup(B,A));
                operadores.pop();
            }
        }
    }
 	return operandos.top();
}

void imprimirset(set<int> a){
    si::iterator it;
    for(it = a.begin() ; it!=a.end() ; it++) cout << *it << " , ";
    cout << endl;
}

void initSigma(string reg){
    for(string::iterator it = reg.begin() ; it != reg.end() ; ++it){
        char currentLetter = *it;
        if(currentLetter != '(' && currentLetter != ')' && currentLetter != '*' && currentLetter != '|' && currentLetter != '.') sigma.insert(currentLetter);
    }
}

vector<vector<int> >powerSet(vector<int> conj){
    vector<vector<int> > ans;
    throw "powerSet not implemented";
    return ans;
}
si cup(si A, si B){
    si ans;
    for(int i: A) ans.insert(i);
    for(int i: B) ans.insert(i);
    return ans;
}
template <typename T>
inline vector<T> setMinus(vector<T>, vector<T>) {
    vector<T> ans;
    throw "setMinus not implemented";
    return ans;
}

template<class T>
vector<set<T>> powerSet(const set<T> &S) {
    vector<set<T>> ans;
    intt full = 1LL << S.size();
    for (intt b = 0; b < full; ++b) {
        set<T> subset;
        intt m = 1;
        for (T x : S) {
            if (m > b)
                break;
            if (b & m)
                subset.insert(x);
            m <<= 1;
        }
        ans.push_back(subset);
    }
    return ans;
}

template<class T>
set<T> setMinus(const set<T> &a, const set<T> &b) {
    set<T> ans;
    set_difference(a.begin(), a.end(), b.begin(), b.end(),
                   inserter(ans, ans.end()));
    return ans;
}




vector<bool> visitados;
class AFD{
public:
    vsi states;
    vector<transition> transitions;
    vector<int> finalstates;
    int init_state;
    map<pair<int,char> , int> mapa_transiciones;

    AFD(AFND A){
		map<si, int> mapa;
        multimap<pair<int, char>, int> mapatr;
        stack<si> dfs_stack;
        int contador = 0;
        int fstate = A.getFinalState();
        //Inicializar mapa de transiciones
        for(transition tra: A.transitions) mapatr.insert(make_pair(make_pair(tra.from, tra.symbol),tra.to ));

        //Estado Inicial
        si epTemp = A.epsilonClausure(0);
        mapa.insert(make_pair(epTemp, contador));
        init_state = mapa[epTemp];
        if(epTemp.find(A.final_state)!= epTemp.end()) finalstates.push_back(mapa[epTemp]);
        //printf("%i\n", init_state);
        dfs_stack.push(epTemp);
        while(!dfs_stack.empty()){
            si Q = dfs_stack.top();
            dfs_stack.pop();

            for(char c : sigma){
                si ans;
                for(int q: Q){
                    multimap<pair<int, char>, int>::iterator it;
                    for (it=mapatr.equal_range(make_pair(q,c)).first; it!=mapatr.equal_range(make_pair(q,c)).second; ++it){
                        auto q1 = it -> second;
                        ans = cup(ans, A.epsilonClausure(q1));
                    }
                }
                if(mapa.find(ans)==mapa.end()){
                    mapa.insert(make_pair(ans,++contador));
                    dfs_stack.push(ans);
                    states.push_back(ans);
                    if(ans.count(fstate)!=0) finalstates.push_back(mapa[ans]);
                }
                this -> addTransition(mapa[Q], mapa[ans], c);
            }
        }
        for(transition tra : transitions) mapa_transiciones.insert(make_pair(make_pair(tra.from, tra.symbol),tra.to ));

    }
    void addTransition(int from, int to, char c){
        transition temp;
        temp.from = from;
        temp.to = to;
        temp.symbol = c;
        transitions.push_back(temp);
    }
    void print(){
        transition tempTrans;
        for(unsigned int i = 0; i<transitions.size() ; i++){
            tempTrans = transitions[i];
            cout << tempTrans.from << " -> " << tempTrans.symbol << " -> " << tempTrans.to << endl;
        }
    }

    void dfs(int state){
        if(visitados[state]) return;

        visitados[state] = true;
        int i = 0;
        for(transition tra: transitions){

            if(tra.from == state){
                cout << state << " -> " << tra.symbol << " -> " << tra.to << endl;
                i++;
                dfs(tra.to);

            }

        }
        //cout << state << "," << i << endl;
    }


};

int main(){
    //string reg = "((u.n)|(n.o))";
    string reg = "((a.b)|((a.b).a))*";
    //string reg = "((a.b).a)";
    AFND temp = fromERtoAFND(reg);
    //temp.print();
    initSigma(reg);
    //cout << temp.final_state << endl;
    AFD temp2(temp);
    for(unsigned int i = 0 ; i< temp2.states.size() ; i++) visitados.push_back(0);
    temp2.dfs(temp2.init_state);
    //temp2.print();
    return 0;
}

