#include <iostream>
#include <list>
#include <map>
#include <vector>

template <class T>
class StateMachine {

    std::list<std::string> alphabet;
    std::list<T> states;
    std::list<T> initStates;
    std::list<T> endStates;
    std::vector<T> curentStates;
    std::multimap <std::pair<T, std::string>, T>  transit;

public:

    StateMachine(std::list<std::string> alphabet, std::list<T> states, std::list<T> initStates, std::list<T> endStates, std::multimap <std::pair<T, std::string>, T> transit) {
        this->alphabet = alphabet;
        this->states = states;
        this->initStates = initStates;
        this->endStates = endStates;
        this->transit = transit;
        for (auto i : initStates) {
            curentStates.push_back(i);
        }
    }

    void setBaseState() {
        curentStates.clear();
        for (auto i : initStates) {
            curentStates.push_back(i);
        }
    }

    void setInitState(T initState) {

        if (std::find(states.begin(), states.end(), initState) != states.end()) {

            initStates.push_back(initState);
            curentStates.push_back(initState);
        }
    }
    void removeInitState(T initState) {
        typename std::list<T>::iterator i = std::find(initStates.begin(), initStates.end(), initState);
        if (i != initStates.end()) {
            initStates.erace(i);
        }
    }
    void addState(T state) {
        if (std::find(states.begin(), states.end(), state) == states.end()) {
            states.push_back(state);
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
    void removeCurentsate(T curentState) {
        typename std::vector<T>::iterator i = std::find(curentStates.begin(), curentStates.end(), curentState);
        if (i != curentStates.end()) {
            curentStates.erase(i);
        }
    }
    void deleteState(T state) {
        typename std::list<T>::iterator i = std::find(states.begin(), states.end(), state);//находим слово в словаре
        if (i != states.end()) {// если слово есть 
            T st = (*i);
            std::vector<std::pair<T, std::string>> sts; // временный вектор для хранения ключей multimap, 
            // ключи отбираеются по принципу: либо сам ключ содержит удаляемое состояние либо значение по ключу в map есть удаляемое состояние
            for (auto j : transit) {
                if (j.second == st || j.first.first == st) {//добавляем такие ключи в массив
                    sts.push_back(j.first);
                }
            }

            for (int j = 0; j < sts.size(); j++) {//идём по массиву ключей
                if (sts[j].first != state) {// если сам ключ не содержит удаляемое состояние, а есть значние в map по этому ключу соответствует нашему состоянию
                    auto n = transit.equal_range(sts[j]);// находим все записи с таким ключём
                    auto t = n.first;
                    while (t != n.second) {
                        if ((*t).second == state) {//ищем те, в которых значение соотв. нашему состоянию и удаляем
                            transit.erase(t);
                            n = transit.equal_range(sts[j]);
                            t = n.first;
                        }
                        else {
                            ++t;
                        }
                    }
                }
                else {// сам ключ содержит удаляемое состояние 
                    auto u = transit.find(sts[j]);
                    while (u != transit.end()) {//удаляем все объекты mmap с таким ключем
                        transit.erace(u);
                        u = transit.find(sts[j]);
                    }
                }
            }
            states.erase(i);
            removeEndState(state);
            removeInitState(state);
            removeCurentsate(state);
        }

    }
    void addTransit(T state1, std::string symb, T state2) {
        if ((std::find(states.begin(), states.end(), state1) != states.end()) && (std::find(states.begin(), states.end(), state2) != states.end())) {
            bool t = true;
            auto i = transit.equal_range(std::pair<T, std::string>(state1,symb));//спиоск всех эл с ключом state1 symb
            for (auto j = i.first; j != i.second; j++) {
                if (j.first.first == state1 && j.first.second == symb && j.second == state2) {
                    t = false; //защита от добавления пути который уже есть (без аналогов)
                }
            }
            if (t) {
                transit.emplace(std::pair<T, std::string>(state1, symb), state2);
            }
        }
    }
    void deleteTransit(T state1, std::string symb, T state2) {
        auto i = transit.equal_range(std::pair<T, std::string>(state1, symb));
        for (auto j = i.first; j != i.second; j++) {
            if ((*j).second == state2) {
                transit.erase(j);
                break;
            }
        }

    }
    std::vector<T> getCurentStates() {
        return this->curentStates;
    }

    void run(std::string symb) {
        if (std::find(alphabet.begin(), alphabet.end(), symb) != alphabet.end()) {// проверка на наличие символа в алфавите
            std::vector<T> curentStates_temp;
            for (auto p : curentStates){// для каждого текущих состояний ищем следующее
                auto i = transit.equal_range(std::pair<T, std::string>(p, symb));//находим все пути для i-го состояния
                if (i.first!=i.second) {// если путей не нашлось то просто оставляем текущее состояние
                    for (auto j = i.first; j != i.second; j++) {
                        curentStates_temp.push_back((*j).second);//если нашлись пути, добавляем во временный массив новые состояния достигнутые из предыдущего
                    }
                }
                i = transit.equal_range(std::pair<T, std::string>(p, "\0"));
                if (i.first == i.second) {// если нашлись скачки
                    for (auto j = i.first; j != i.second; j++) {
                        curentStates_temp.push_back((*j).second);//если нашлись пути, добавляем во временный массив новые состояния достигнутые из предыдущего
                    }
                }
            }
            curentStates.clear();
            curentStates = curentStates_temp;
            for (auto i : curentStates) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
        
    }
    void run(std::string* symb, int len) {
        for (int i = 0; i < len; i++) {
            run(symb[i]);
        }
    }

    std::vector<T> curentstates() {
        return this->curentStates;
    }

};









using namespace std;

int main()
{
    std::list<std::string> alph{ "a","b","x","y"};
    std::list<int> states{ 0, 1, 2 , 3, 4, 5, 6, 7 };
    std::list<int> endstates{0,1,2,3,5,6,7 };
    std::list<int> initstates{0};
    std::multimap <std::pair<int, std::string>, int> transit;
    transit.emplace(std::pair<int, std::string>(0,"x"),3);
    transit.emplace(std::pair<int, std::string>(0, "a"), 4);
    transit.emplace(std::pair<int, std::string>(0, "y"), 7);
    transit.emplace(std::pair<int, std::string>(0, "a"), 6);
    transit.emplace(std::pair<int, std::string>(0, "x"), 1);
    transit.emplace(std::pair<int, std::string>(0, "x"), 2);
    transit.emplace(std::pair<int, std::string>(2, "x"), 1);
    transit.emplace(std::pair<int, std::string>(2, "y"), 7);
    transit.emplace(std::pair<int, std::string>(3, "x"), 1);
    transit.emplace(std::pair<int, std::string>(3, "y"), 5);
    transit.emplace(std::pair<int, std::string>(3, "y"), 7);
    transit.emplace(std::pair<int, std::string>(4, "b"), 2);
    transit.emplace(std::pair<int, std::string>(5, "y"), 5);
    transit.emplace(std::pair<int, std::string>(5, "x"), 1);
    transit.emplace(std::pair<int, std::string>(5, "y"), 7);
    transit.emplace(std::pair<int, std::string>(6, "a"), 6);
    transit.emplace(std::pair<int, std::string>(6, "y"), 7);
    transit.emplace(std::pair<int, std::string>(6, "x"), 1);
    transit.emplace(std::pair<int, std::string>(7, "y"), 7);

    StateMachine<int> st(alph, states, initstates, endstates, transit);

    string* symbols_1 = new string[4];
    symbols_1[0] = "a";
    symbols_1[1] = "a";
    symbols_1[2] = "a";
    symbols_1[3] = "x";

    string* symbols_2 = new string[4];
    symbols_2[0] = "x";
    symbols_2[1] = "y";
    symbols_2[2] = "y";
    symbols_2[3] = "b";


    st.run(symbols_1,4);
    std::vector<int> p1 = st.curentstates();

    for (auto i : p1) {
        std::cout << i << " ";
    }
    std::cout << endl;
    std::cout <<"Перв.слово"<< std::endl;
    
    st.setBaseState();
    st.run(symbols_2, 4);
    p1 = st.curentstates();
    if (p1.size() == 0) {
        std::cout << "No cur states" << std::endl;
    }
    for (auto i : p1) {
        std::cout << i << " ";
    }
    std::cout << std::endl;


}