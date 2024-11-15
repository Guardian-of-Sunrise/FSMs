#include <iostream>
#include <list>
#include <map>
#include <vector>

template <class T>
class StateMachine {

    std::list<char> alphabet;
    std::list<T> states;
    T initState;
    std::list<T> endStates;
    T curentState;
    std::map <std::pair<T, char> , T>  transit;

public:

    StateMachine(std::list<char> alphabet, std::list<T> states, T initState, std::list<T> endStates, std::map <std::pair<T, char>, T> transit) {
        this->alphabet = alphabet;
        this->states = states;
        this->initState = initState;
        this->endStates = endStates;
        this->curentState = initState;
        this->transit = transit;
    }

    void setInitState(T initState) {

        if (std::find(states.begin(), states.end(), initState) != states.end()) {
          
            this->initState = initState;
            this->curentState = initState;
        }
    }
    void setEndState(T endState) {

        if (std::find(states.begin(), states.end(), endState) != states.end()) {
            if (std::find(endStates.begin(), endStates.end(), endState) != endStates.end()) {
                endStates.push_back(endState);
            }
        }
    }
    void removeEndState(T endState) {
        typename std::list<T>::iterator i = std::find(endStates.begin(), endStates.end(), endState);
        if (i != endStates.end()) {
            endStates.erace(i);
        }
    }

    void addState(T state) {
        if (std::find(states.begin(), states.end(), state) == states.end()){
        states.push_back(state);
        }
    }
    void deleteState(T state) {
        typename std::list<T>::iterator i = std::find(states.begin(), states.end(), state);
        if (i != states.end()){
            T st = (*i);
            std::vector<std::pair<T, char>> sts;
                for (auto j : transit) {
                    if (j.second == st || j.first.first == st) {
                        sts.push_back(j.first);
                    }
            }

                for (int j = 0; j < sts.size(); j++) {
                    auto i = transit.find(sts[j]);
                    transit.erase(i);
            }


        states.erase(i);
        removeEndState(state);
        if (curentState == state) {
            if (states.size() > 0) {
                curentState = states.front();
            }
        }

        }

    }
    void reset() {
        this->curentState = initState;
    }
    void addTransit(T state1,char symb,T state2) {
        if ((std::find(states.begin(), states.end(), state1) != states.end()) && (std::find(states.begin(), states.end(), state2) != states.end())) {
            transit.emplace(std::pair<T, char>(state1, symb), state2);
        }
    }
    void delTransit(T state1, char symb) {
       typename std::map<std::pair<T,char>,T> i = transit.find(std::pair<T, char>(state1, symb));
        if (i != transit.end()) {
            transit.erase(i);
        }

    }


    T getCurentState() {
        return this->curentState;
    }

    int run(char symb) {
        if (std::find(alphabet.begin(), alphabet.end(),symb) != alphabet.end()){
            auto i = transit.find(std::pair<T, char>(curentState, symb));
            if (i != transit.end()) {
                curentState = (*i).second;
                return 0;
            }
            else {
                return 1;
            }
        }
        else {
            return 1;
        }
       
    }

    int run(std::string word, int len) {
        int suc = 0;
        for (int i = 0; i < len; i++) {
            suc = run(word[i]);
            if (suc == 1) {
                return 1;
            }
        }
        if (std::find(endStates.begin(), endStates.end(), curentState) != endStates.end()) {
            return 0;
        }
        else {
            return 1;
        }
    }
     


};

template <typename T>
void cheсk(std::string str, StateMachine<T>& statem) {
    int state = statem.run(str, str.size());
    if (state == 0) {
        std::cout << "SlOVO AVTOMATA"<<std::endl;
    }
    else {
        std::cout << "NE SlOVO AVTOMATA" << std::endl;
    }
}






using namespace std;

int main()
{
    std::list<char> alph{ '0','1' };
    std::list<int> states{ 1, 2 , 3, 4 };
    std::list<int> endstates{ 1  };
    std::map <std::pair<int,char>, int>  transit;
    transit.emplace(std::pair<int, char>(1, '0'), 2);
    transit.emplace(std::pair<int, char>(2, '0'), 1);

    transit.emplace(std::pair<int, char>(1, '1'), 3);
    transit.emplace(std::pair<int, char>(3, '1'), 1);

    transit.emplace(std::pair<int, char>(2, '1'), 4);
    transit.emplace(std::pair<int, char>(4, '1'), 2);

    transit.emplace(std::pair<int, char>(3, '0'), 4);
    transit.emplace(std::pair<int, char>(4, '0'), 3);

    StateMachine<int> statemach(alph, states, 1, endstates, transit);

    cheсk("0100111111111100000110010101", statemach);
    statemach.reset();
    cheсk("010011111111100000110010101", statemach);
 

}

