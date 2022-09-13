#include "TerminalToken.cpp"
#include <unordered_map>
#include <vector>
#include <array>
#include <stack>


class NFAState {
    NFAState() {}

private:
    TerminalToken tok;

    std::unordered_multimap<char, NFAState *> nextStates;
    bool isAccepting;

    friend class TinyBasicNFA;
};


class TinyBasicNFA {
public:
    TinyBasicNFA(std::vector<TerminalToken> terminalTokens) {
        std::vector<std::array<NFAState *, 2>> individualNFAS;
        for (TerminalToken &t: terminalTokens) {
            individualNFAS.push_back(constructNFA(t.getPattern()));
            individualNFAS.back()[1]->tok = t;
        }
        std::array<NFAState *, 2> cumulativeNFA = individualNFAS.back();
        individualNFAS.pop_back();
        while (individualNFAS.size() > 0) {
            cumulativeNFA = joinOr(cumulativeNFA, individualNFAS.back());
        }
        this->startingState = cumulativeNFA[0];
        this->currStates.push_back(startingState);
    }

private:

    std::array<NFAState *, 2> constructNFA(const std::string &pattern) {
        std::stack<std::pair<NFAState *, char>>
                st;
        st.push({nullptr, NULL});
        char currentExpressionOp = NULL;
        char currentExpressionValue = NULL;
        auto mergeStates = [&](std::array<NFAState *, 2> &currExpressionStates) -> void {
            if (st.top()[0] == nullptr) {
                st.top()[0] = currExpressionStates;
            } else {
                if (st.top()[1] == '+') {
                    st.top()[0] = joinConcat(currExpressionStates, st.top()[0]);
                } else if (st.top()[1] == '|') {
                    st.top()[0] = joinOr(currExpressionStates, st.top()[0]);
                } else if (st.top()[1] == '*') {
                    st.top()[0] = joinKleene(currExpressionStates, st.top()[0]);
                } else {
                    st.top()[0] = currExpressionStates;

                }
            }
        };
        for (int i = 0; i < (int) (pattern.size()); i++) {
            if (pattern[i] == '(') {
                st.push({nullptr, currentExpressionOp});
                currentExpressionOp = NULL;
            } else if (pattern[i] == ')') {
                std::array<NFAState *, 2> currExpressionStates = constructNFASingleChar(currentExpressionValue);
                st.pop();
                mergeStates(currExpressionStates);


            } else if (pattern[i] == '+' || pattern[i] == '*' || pattern[i] == '|') {
                currentExpressionOp = pattern[i];
            } else if (pattern[i] == '\\') {
                i++;
                currentExpressionValue = pattern[i];
            } else {
                currentExpressionValue = pattern[i];
            }
        }
        return st.top()[0];

    }

    std::array<NFAState *, 2> constructNFASingleChar(const char &value) {
        NFAState *currStartingState = new NFAState;
        NFAState *currAcceptingState = new NFAState;
        currStartingState->nextStates.insert({value, currAcceptingState});
        currAcceptingState->isAccepting = true;
        return {currStartingState, currAcceptingState};
    }

    std::pair<bool, TerminalToken> transition(const char &ch) {
        std::vector<NFAState *> newStates;
        for (NFAState *state: currStates) {
            if (state->nextStates.find(ch) != state->nextStates.end()) {
                newStates.push_back(state->nextStates[ch]);
                if (newStates.back()->isAccepting) {
                    this->acceptingStatesInPath.push_back(newStates.back());
                }
            }
        }
        TerminalToken currToken;
        if (newStates.size() == 0) {
            //error state
            if (acceptingStatesInPath.size() == 0) {
                return {false, currToken}
            } else {
                this->currStates.clear();
                this->currStates.push_back(this->startingState);
                NFAState *stateToReturn = this->acceptingStatesInPath.back();
                this->acceptingStatesInPath.clear();
                return {true, stateToReturn->tok};
            }
        } else {
            return {true, currToken}
        }
    }

    std::array<NFAState *, 2>
    joinOr(NFAState *firstStart, NFAState *firstAccepting, NFAState *secondStart, NFAState *secondAccepting) {
        firstAccepting->isAccepting = false;
        secondAccepting->isAccepting = false;
        NFAState *newStartingState = new NFAState;
        NFAState *newAcceptingState = new NFAState;
        newAcceptingState->isAccepting = true;
        newStartingState->nextStates.insert({NULL, firstStart});
        newStartingState->nextStates.insert({NULL, secondStart});
        firstAccepting->nextStates.insert({NULL, newAcceptingState});
        secondAccepting->nextStates.insert({NULL, newAcceptingState});
        return {newStartingState, newAcceptingState};
    }

    std::array<NFAState *, 2>
    joinKleene(NFAState *firstStart, NFAState *firstAccepting) {
        NFAState *newStartingState = new NFAState;
        NFAState *newAcceptingState = new NFAState;
        newStartingState->nextStates.insert({NULL, newAcceptingState});
        newStartingState->nextStates.insert({NULL, firstStart});
        newAcceptingState->isAccepting = true;
        firstAccepting->nextStates.insert({NULL, firstStart});
        firstAccepting->nextStates.insert({NULL, newAcceptingState});
        firstAccepting->isAccepting = false;
        return {newStartingState, newAcceptingState};

    }

    std::array<NFAState *, 2>
    joinConcat(NFAState *firstStart, NFAState *firstAccepting, NFAState *secondStart, NFAState *secondAccepting) {
        firstAccepting->isAccepting = false;
        firstAccepting->nextStates.insert({NULL, secondStart});
        return {firstStart, secondAccepting};
    }


    std::vector<NFAState *> currStates;
    std::vector<NFAState *> acceptingStatesInPath;
    NFAState *startingState;

};
