#include "Grammar.cpp"
#include <unordered_map>
#include <vector>
#include <array>
#include <stack>
#include <assert.h>
#include <iostream>
#include <queue>
#include <functional>

class NFAState {
    NFAState() {}

    ~NFAState() {}

private:
    GrammarState terminalState;
    int stateId;
    std::unordered_map<char, std::vector<NFAState *>> nextStates;
    bool isAccepting;

    friend class NFA;
};


class NFA {
public:
    NFA(const Grammar& g) {
        std::vector<GrammarState> terminalTokens = g.getTerminalStates();
        std::vector<std::array<NFAState *, 2>> individualNFAS;
        for (GrammarState &t: terminalTokens) {
            individualNFAS.push_back(constructNFA(t.getPattern()));
            individualNFAS.back()[1]->terminalState = t;
        }
        this->startingState = new NFAState;
        while (individualNFAS.size() > 0) {
            this->startingState->nextStates[NULL].push_back(individualNFAS.back()[0]);
            individualNFAS.pop_back();
        }
        totalStates.push_back(this->startingState);
        for (int pos = 0; pos < (int) (totalStates).size(); pos++) {
            totalStates[pos]->stateId = pos;
        }
    }

    ~NFA() {
        for (NFAState *state: totalStates) {
            delete state;
        }
    }


    const std::pair<bool, std::vector<std::pair<GrammarState, std::string>>> generateTokens(const std::string &fileContent) const{
        std::vector<std::pair<GrammarState, std::string>> tokens;
        int ptr = -1;
        std::queue<NFAState *> currLevelStates;
        std::vector<bool> visited(totalStates.size(), false);
        std::pair<int, NFAState *> acceptingState = {-1, nullptr};
        std::function<void(NFAState *)> updateCurrAndEpsilonReachAble = [&](NFAState *state) -> void {
            if (!visited[state->stateId]) {
                visited[state->stateId] = true;
                currLevelStates.push(state);
                if (state->isAccepting) {
                    acceptingState = {ptr, state};
                }
                for (NFAState *epsilonTransitionNeighbors: state->nextStates[NULL]) {
                    updateCurrAndEpsilonReachAble(epsilonTransitionNeighbors);
                }
            }

        };
        updateCurrAndEpsilonReachAble(this->startingState);
        ptr++;
        int previousPtr = 0;
        while (ptr < (int) (fileContent.size())) {
            int currSize = currLevelStates.size();
            std::fill(visited.begin(), visited.end(), false);
            while (currSize--) {
                NFAState *curr = currLevelStates.front();
                currLevelStates.pop();
                if (curr->nextStates.find(fileContent[ptr]) != curr->nextStates.end()) {
                    for (NFAState *neighboringToCurrState: curr->nextStates[fileContent[ptr]]) {
                        updateCurrAndEpsilonReachAble(neighboringToCurrState);
                    }
                }


            }
            if (currLevelStates.size() == 0) {
                //file has lexical error
                if (acceptingState.first == -1 && acceptingState.second == nullptr) {
                    return {false, {}};
                }
                ptr = acceptingState.first + 1;
                tokens.push_back(
                        {acceptingState.second->terminalState, fileContent.substr(previousPtr, ptr - previousPtr)});
                previousPtr = ptr;
                std::fill(visited.begin(), visited.end(), false);
                updateCurrAndEpsilonReachAble(this->startingState);
                acceptingState = {-1, nullptr};
            } else { ptr++; }

        }
        if (acceptingState.first == -1 && acceptingState.second == nullptr) {
            return {false, {}};
        }
        tokens.push_back(
                {acceptingState.second->terminalState, fileContent.substr(previousPtr, ptr - previousPtr)});
        return {true, tokens};
    }

private:

    std::array<NFAState *, 2> constructNFA(const std::string &pattern) {
        std::stack<std::pair<std::array<NFAState *, 2>, char>>
                st;
        st.push({{nullptr, nullptr}, NULL});
        char currentExpressionOp = NULL;
        char currentExpressionValue = NULL;
        bool isCharacterPending = false;
        std::function<void(std::array<NFAState *, 2> &)> mergeStates = [&](
                std::array<NFAState *, 2> &currExpressionStates) -> void {
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
        firstAccepting->nextStates[NULL].push_back(secondStart);
        return {firstStart, secondAccepting};
    }

    std::vector<NFAState *> totalStates;
    NFAState *startingState;
};
