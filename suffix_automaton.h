#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace AlgoLib {

class SuffixAutomaton {
 public:
  SuffixAutomaton(const std::string& input);

  struct State {
    int length, link;
    std::unordered_map<char, int> transitions;
  };

 private:
  std::vector<State> states_;
  int last_;
  constexpr static int kNoLink = -1;

 private:
  SuffixAutomaton();
  SuffixAutomaton(const SuffixAutomaton&);
  SuffixAutomaton(const SuffixAutomaton&&);

  void ProcessNextChar(char element);
  void CloneStateAndUpdate(int p, int current, char element);
};

SuffixAutomaton::SuffixAutomaton(const std::string& input)
  :  states_(1),
     last_(0)
{
  states_.front().length = 0;
  states_.front().link = kNoLink;
  for (char element : input) {
    ProcessNextChar(element);
  }
}

void SuffixAutomaton::ProcessNextChar(char element) {
  states_.emplace_back(states_[last_].length + 1, kNoLink);
  int current = states_.size() - 1;
  last_ = current;

  int p = last_;
  for (; p != kNoLink && !states_[p].transitions.count(element); p = states_[p].link) {
    states_[p].transitions[element] = current;
  }

  if (p == kNoLink) {
    states_[current].link = 0;
    return;
  }

  CloneStateAndUpdate(p, current, element);
}

void SuffixAutomaton::CloneStateAndUpdate(int p, int current, char element) {
  int q = states_[p].transitions[element];
  if (states_[p].length + 1 == states_[q].length) {
    states_[current].link = q;
  } else {
    states_.emplace_back(states_[q]);
    int clone = states_.size() - 1;
    states_.back().length = states_[p].length + 1;
    for (; p != kNoLink && states_[p].transitions[element] == q; p = states_[p].link) {
      states_[p].transitions[element] = clone;
    }
    states_[q].link = states_[current].link = clone;
  }
}

} // namespace AlgoLib
