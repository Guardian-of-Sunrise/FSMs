#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <tuple>
#include <stack>

template <class T>
class StateMachine {

    std::list<char> alphabet;
    std::list<char> stackalphabet;
    std::stack<char> stack;
    std::list<T> states;
    T initState;
    std::list<T> endStates;
    T curentState;
    std::map <std::tuple<T, char, char>, std::pair<T, std::string>>  transit;
    char endstacksymb;

public:

    StateMachine(std::list<char> alphabet, std::list<char> stackalphabet, std::list<T> states, T initState, std::list<T> endStates, std::map <std::tuple<T, char, char>, std::pair<T, std::string>> transit, char endstacksymb) {
        this->alphabet = alphabet;
        this->states = states;
        this->initState = initState;
        this->endStates = endStates;
        this->curentState = initState;
        this->transit = transit;
        this->stackalphabet = stackalphabet;
        this->endstacksymb = endstacksymb;
        stack.push(endstacksymb);

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
        if (std::find(states.begin(), states.end(), state) == states.end()) {
            states.push_back(state);
        }
    }
    void deleteState(T state) {
        typename std::list<T>::iterator i = std::find(states.begin(), states.end(), state);
        if (i != states.end()) {
            T st = (*i);
            std::vector<std::tuple<T, char, char>> sts;
            for (auto j : transit) {
                if (j.second.first == st || j.fist[0] == st) {
                    sts.push_back(j.first);
                }
            }

            for (int j = 0; j < sts.size(); j++) {
                auto i = transit.find(sts[j]);
                transit.erase(i);
            }


            states.erase(i);
            removeEndState(state);
            this->stask.clear();
            this->stack.push('~');
            if (curentState == state) {
                if (states.size() > 0) {
                    curentState = states.front();
                }
            }

        }

    }
    void addTransit(T state1, char symbalpha, char symstack, T state2, std::string PDsymb) {
        if ((std::find(states.begin(), states.end(), state1) != states.end()) && (std::find(states.begin(), states.end(), state2) != states.end()) && (std::find(stackalphabet.begin(), stackalphabet.end(), symstack) != states.end()) && (std::find(stackalphabet.begin(), stackalphabet.end(), PDsymb) != states.end())) {
            transit.emplace(std::tuple<T, char, char>{state1, symbalpha, symstack }, std::pair<T, std::string>(state2, PDsymb));
        }
    }
    void delTransit(T state1, char symb, char symstack) {
        typename std::map<std::tuple<T, char, char>, std::pair<T, std::string>> i = transit.find(std::tuple<T, char, char>(state1, symb, symstack));
        if (i != transit.end()) {
            transit.erase(i);
        }

    }


    T getCurentState() {
        return this->curentState;
    }

    int run(char symb) {
        if (std::find(alphabet.begin(), alphabet.end(), symb) != alphabet.end()) {
            auto i = transit.find(std::tuple<T, char, char>(curentState, symb, stack.top()));
            if (i != transit.end()) {
                curentState = (*i).second.first;
                char statestack = stack.top();
                std::string nextstatestack = (*i).second.second;
                if (nextstatestack.size() == 2) {
                    if (nextstatestack[1] == statestack) {
                        stack.push(nextstatestack[0]);
                    }
                }
                else {
                    if (nextstatestack.size() == 1) {
                        if (nextstatestack[0] == '\0') {
                            if (stack.top() != endstacksymb) {
                                stack.pop();
                            }
                            else {
                                return 3;
                            }
                        }
                        else {
                            if (stack.top() != endstacksymb) {
                                stack.pop();
                                stack.push(nextstatestack[0]);
                            }
                            else {
                                return 3;
                            }
                        }
                    }

                }


            }
            else {
                //  "Net f-ii perehoda dlya dannogo sim alfavita" << std::endl;
                return 1;
            }
            return 0;
        }
        else {
            std::cout << "Ne simb alfavita";
            return 2;
        }
    }

    void run(std::string symb, int len_) {
        int len = len_ + 1;
        int state;
        bool t = true;
        for (int i = 0; i < len; i++) {
            if (i == len - 1) {
                auto k = transit.find(std::tuple<T, char, char>(curentState, '\0', stack.top()));
                if (k != transit.end()) {
                    curentState = (*k).second.first;
                    char statestack = stack.top();
                    std::string nextstatestack = (*k).second.second;
                    if (nextstatestack.size() == 2) {
                        if (nextstatestack[1] == statestack) {
                            stack.push(nextstatestack[0]);
                        }
                    }
                    else {
                        if (nextstatestack.size() == 1) {
                            if (nextstatestack[0] == '\0') {
                                stack.pop();
                            }
                            else {
                                stack.pop();
                                stack.push(nextstatestack[0]);
                            }
                        }

                    }
                }
                break;
            }
            state = run(symb[i]);
            if (state == 2) {
                break;
            }
            if (state == 1) {//если мы не нашли функцию перехода для символа, пытаемся найти свободный переход
                auto k = transit.find(std::tuple<T, char, char>(curentState, '\0', stack.top()));
                if (k != transit.end()) {
                    curentState = (*k).second.first;
                    char statestack = stack.top();
                    std::string nextstatestack = (*k).second.second;
                    if (nextstatestack.size() == 2) {
                        if (nextstatestack[1] == statestack) {
                            stack.push(nextstatestack[0]);
                        }
                    }
                    else {
                        if (nextstatestack.size() == 1) {
                            if (nextstatestack[0] == '\0') {
                                stack.pop();
                            }
                            else {
                                stack.pop();
                                stack.push(nextstatestack[0]);
                            }
                        }

                    }
                    state = 0;
                }
                else {
                    state = 2;
                    break;
                }
                --i;
            }
            std::cout << "curState = " << curentState << " " << "els steka = ";
            std::stack<char> test = stack;
            std::vector<char> test1;
            while (!test.empty()) {
                char k = test.top();
                test.pop();
                test1.push_back(k);
            }
            for (auto i : test1) {
                std::cout << i;
            }
            std::cout << std::endl;
        }
    
        if (stack.size() == 1 &&state == 0 ) {
            bool tru = false;
            for (auto i : endStates) {
                if (i == curentState) {
                    tru = true;
                }
            }
            if (tru) {
                std::cout << "SLOVO AVTOMATA" << std::endl;
            }
        }

    }
};




using namespace std;

int main()
{
    std::list<char> alph{ '(',')' };
    std::list<char> alphstack{ '(',')' };
    std::list<int> states{ 1, 2 };
    std::list<int> endstates{ 1 };
    char endstacksimb = '~';
    std::map <std::tuple<int, char, char>, std::pair<int, std::string>>  transit;
    transit.emplace(std::tuple<int, char, char>{1, '(', endstacksimb}, std::pair<int, std::string>(2, std::string({ '(' , endstacksimb })));
    transit.emplace(std::tuple<int, char, char>{2, '(', '(' }, std::pair<int, std::string>(2, std::string({ '(' ,'(' })));
    transit.emplace(std::tuple<int, char, char>{2, ')', '(' }, std::pair<int, std::string>(2, std::string({ '\0' })));
    transit.emplace(std::tuple<int, char, char>{2, '\0', endstacksimb }, std::pair<int, std::string>(1, std::string({ endstacksimb })));



    StateMachine<int> statemach(alph, alphstack, states, 1, endstates, transit, '~');

    std::string str("((((()))))(())");
   


    statemach.run(str,str.size());












    std::list<char> alph1{ '(',')','{','}','[',']' };
    std::list<char> alphstack1{ '(',')','{','}','[',']' };
    std::list<int> states1{ 1, 2 };
    std::list<int> endstates1{ 1 };
    std::map <std::tuple<int, char, char>, std::pair<int, std::string>>  transit1;
    transit1.emplace(std::tuple<int, char, char>{1, '(', endstacksimb}, std::pair<int, std::string>(2, std::string({ '(' , endstacksimb })));
    transit1.emplace(std::tuple<int, char, char>{1, '{', endstacksimb}, std::pair<int, std::string>(2, std::string({ '{' , endstacksimb })));
    transit1.emplace(std::tuple<int, char, char>{1, '[', endstacksimb}, std::pair<int, std::string>(2, std::string({ '[' , endstacksimb })));

    transit1.emplace(std::tuple<int, char, char>{2, '(', '(' }, std::pair<int, std::string>(2, std::string({ '(' ,'(' })));
    transit1.emplace(std::tuple<int, char, char>{2, '(', '[' }, std::pair<int, std::string>(2, std::string({ '(' ,'[' })));
    transit1.emplace(std::tuple<int, char, char>{2, '(', '{' }, std::pair<int, std::string>(2, std::string({ '(' ,'{' })));

    transit1.emplace(std::tuple<int, char, char>{2, '[', '(' }, std::pair<int, std::string>(2, std::string({ '[' ,'(' })));
    transit1.emplace(std::tuple<int, char, char>{2, '[', '[' }, std::pair<int, std::string>(2, std::string({ '[' ,'[' })));
    transit1.emplace(std::tuple<int, char, char>{2, '[', '{' }, std::pair<int, std::string>(2, std::string({ '[' ,'{' })));

    transit1.emplace(std::tuple<int, char, char>{2, '{', '(' }, std::pair<int, std::string>(2, std::string({ '{' ,'(' })));
    transit1.emplace(std::tuple<int, char, char>{2, '{', '[' }, std::pair<int, std::string>(2, std::string({ '{' ,'[' })));
    transit1.emplace(std::tuple<int, char, char>{2, '{', '{' }, std::pair<int, std::string>(2, std::string({ '{' ,'{' })));


    transit1.emplace(std::tuple<int, char, char>{2, ')', '(' }, std::pair<int, std::string>(2, std::string({ '\0' })));
    transit1.emplace(std::tuple<int, char, char>{2, ']', '[' }, std::pair<int, std::string>(2, std::string({ '\0' })));
    transit1.emplace(std::tuple<int, char, char>{2, '}', '{' }, std::pair<int, std::string>(2, std::string({ '\0' })));


    transit1.emplace(std::tuple<int, char, char>{2, '\0', endstacksimb }, std::pair<int, std::string>(1, std::string({ endstacksimb })));



    StateMachine<int> statemach1(alph1, alphstack1, states1, 1, endstates1, transit1, '~');

    std::string str1("({[([{}])]})");

    statemach1.run(str1, str1.size());








}