#include "TerminalToken.cpp"
#include <unordered_map>
#include <vector>
#include <array>
#include <stack>
#include <assert.h>
#include <iostream>

class NFAState {
    NFAState() {}

    ~NFAState() {}

private:
    TerminalToken tok;

    std::unordered_map<char, std::vector<NFAState *>> nextStates;
    bool isAccepting;

    friend class NFA;
};


class NFA {
public:
    NFA(std::vector<TerminalToken> terminalTokens) {
        std::vector<std::array<NFAState *, 2>> individualNFAS;
        for (TerminalToken &t: terminalTokens) {
            individualNFAS.push_back(constructNFA(t.getPattern()));
            individualNFAS.back()[1]->tok = t;
        }
        std::array<NFAState *, 2> cumulativeNFA = individualNFAS.back();
        individualNFAS.pop_back();
        while (individualNFAS.size() > 0) {
            cumulativeNFA = joinOr(cumulativeNFA[0], cumulativeNFA[1], individualNFAS.back()[0],
                                   individualNFAS.back()[1]);
            individualNFAS.pop_back();
        }
        this->startingState = cumulativeNFA[0];
        this->currStates.push_back(startingState);
    }

    ~NFA() {
        for(NFAState* state:totalStates){
            delete state;
        }
    }

    std::pair<bool, TerminalToken> transition(const char &ch) {
        /*
        std::vector<NFAState *> newStates;
        for (NFAState *state: currStates) {
            if (state->nextStates.find(ch) != state->nextStates.end()) {
                std::cout << "Moved" << std::endl;
                for (NFAState *neighbouringState: state->nextStates[ch]) {
                    newStates.push_back(neighbouringState);
                    if (newStates.back()->isAccepting) {
                        this->acceptingStatesInPath.push_back(newStates.back());
                    }
                }
            }
        }
        currStates.clear();
        currStates = newStates;

        TerminalToken currToken;
        if (acceptingStatesInPath.size() > 0) {
            return {true, acceptingStatesInPath.back()->tok};
        } else {
            return {false, currToken};
        }
        /*
        TerminalToken currToken;
        if (currStates.size() == 0) {
            //error state
            if (acceptingStatesInPath.size() == 0) {
                return {false, currToken};
            } else {
                this->currStates.clear();
                this->currStates.push_back(this->startingState);
                NFAState *stateToReturn = this->acceptingStatesInPath.back();
                this->acceptingStatesInPath.clear();
                return {true, stateToReturn->tok};
            }
        } else {
            return {true, currToken};
        }
         */

    }

private:

    std::array<NFAState *, 2> constructNFA(const std::string &pattern) {
        std::stack<std::pair<std::array<NFAState *, 2>, char>>
                st;
        st.push({{nullptr, nullptr}, NULL});
        char currentExpressionOp = NULL;
        char currentExpressionValue = NULL;
        bool isCharacterPending = false;
        auto mergeStates = [&](std::array<NFAState *, 2> &currExpressionStates) -> void {
            if (st.top().first[0] == nullptr) {
                if (st.top().second == '*') {
                    st.top().first = joinKleene(currExpressionStates[0], currExpressionStates[1]);
                } else {
                    st.top().first = currExpressionStates;
                }
            } else {
                if (st.top().second == '+') {
                    st.top().first = joinConcat(st.top().first[0], st.top().first[1], currExpressionStates[0],
                                                currExpressionStates[1]);
                } else if (st.top().second == '|') {
                    st.top().first = joinOr(st.top().first[0], st.top().first[1], currExpressionStates[0],
                                            currExpressionStates[1]);
                } else {
                    //unreachable
                    assert(false);
                }
            }
        };
        for (int i = 0; i < (int) (pattern.size()); i++) {

            if (pattern[i] == '(') {
                st.push({{nullptr, nullptr}, currentExpressionOp});
                currentExpressionOp = NULL;
                isCharacterPending = true;
            } else if (pattern[i] == ')') {
                std::array<NFAState *, 2> currExpressionStates = (isCharacterPending == false ? st.top().first
                                                                                              : constructNFASingleChar(
                                currentExpressionValue));
                st.pop();
                mergeStates(currExpressionStates);
                currentExpressionValue = NULL;
                isCharacterPending = false;


            } else if (pattern[i] == '+' || pattern[i] == '*' || pattern[i] == '|') {
                currentExpressionOp = pattern[i];
            } else if (pattern[i] == '\\') {
                i++;
                currentExpressionValue = pattern[i];
            } else {
                currentExpressionValue = pattern[i];
            }
        }
        return st.top().first;

    }

    std::array<NFAState *, 2> constructNFASingleChar(const char &value) {
        NFAState *currStartingState = new NFAState;
        NFAState *currAcceptingState = new NFAState;
        currStartingState->nextStates[value].push_back(currAcceptingState);
        currAcceptingState->isAccepting = true;
        (this->totalStates).push_back(currAcceptingState);
        (this->totalStates).push_back(currStartingState);
        return {currStartingState, currAcceptingState};
    }


    std::array<NFAState *, 2>
    joinOr(NFAState *firstStart, NFAState *firstAccepting, NFAState *secondStart, NFAState *secondAccepting) {
        assert(firstStart != nullptr && firstAccepting != nullptr && secondStart != nullptr &&
               secondAccepting != nullptr);
        firstAccepting->isAccepting = false;
        secondAccepting->isAccepting = false;
        NFAState *newStartingState = new NFAState;
        NFAState *newAcceptingState = new NFAState;
        newAcceptingState->isAccepting = true;
        newStartingState->nextStates[NULL].push_back(firstStart);
        newStartingState->nextStates[NULL].push_back(secondStart);
        firstAccepting->nextStates[NULL].push_back(newAcceptingState);
        secondAccepting->nextStates[NULL].push_back(newAcceptingState);
        (this->totalStates).push_back(newAcceptingState);
        (this->totalStates).push_back(newStartingState);
        return {newStartingState, newAcceptingState};
    }

    std::array<NFAState *, 2>
    joinKleene(NFAState *firstStart, NFAState *firstAccepting) {
        NFAState *newStartingState = new NFAState;
        NFAState *newAcceptingState = new NFAState;
        newStartingState->nextStates[NULL].push_back(newAcceptingState);
        newStartingState->nextStates[NULL].push_back(firstStart);
        newAcceptingState->isAccepting = true;
        firstAccepting->nextStates[NULL].push_back(firstStart);
        firstAccepting->nextStates[NULL].push_back(newAcceptingState);
        firstAccepting->isAccepting = false;
        (this->totalStates).push_back(newStartingState);
        (this->totalStates).push_back(newAcceptingState);
        return {newStartingState, newAcceptingState};

    }

    std::array<NFAState *, 2>
    joinConcat(NFAState *firstStart, NFAState *firstAccepting, NFAState *secondStart, NFAState *secondAccepting) {
        firstAccepting->isAccepting = false;
        firstAccepting->nextStates = secondStart->nextStates;
        delete secondStart;
        return {firstStart, secondAccepting};
    }


    std::vector<NFAState *> currStates;
    std::vector<NFAState *> totalStates;
    NFAState *startingState;
};
