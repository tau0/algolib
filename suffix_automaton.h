#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace AlgoLib {


class SuffixAutomaton {
 public:
  explicit SuffixAutomaton(const std::string& input);

  constexpr static int kNoLink = -1;

  struct State {
    State()
      : length(0),
        link(kNoLink) 
    {}

    State(int length_, int link_) 
      : length(length_),
        link(link_) 
    {}

    int length, link;
    std::unordered_map<char, int> transitions;
  };

  std::vector<State> GetStates() const {
    return states_;
  }

 private:
  std::vector<State> states_;
  int last_;

 private:
  SuffixAutomaton();
  SuffixAutomaton(const SuffixAutomaton&);
  SuffixAutomaton(const SuffixAutomaton&&);

  void ProcessNextChar(char element);
  void CloneStateAndUpdate(int pp, int current, char element);
};

SuffixAutomaton::SuffixAutomaton(const std::string& input)
  :  states_(1),
     last_(0)
{
  for (char element : input) {
    ProcessNextChar(element);
  }
}

void SuffixAutomaton::ProcessNextChar(char element) {
  states_.emplace_back(states_[last_].length + 1, kNoLink);
  int current = states_.size() - 1;
  int pp = last_;
  last_ = current;
  for (; pp != kNoLink && !states_[pp].transitions.count(element); pp = states_[pp].link) {
    states_[pp].transitions[element] = current;
  }

  if (pp == kNoLink) {
    states_[current].link = 0;
    return;
  }

  CloneStateAndUpdate(pp, current, element);
}

void SuffixAutomaton::CloneStateAndUpdate(int pp, int current, char element) {
  int qq = states_[pp].transitions[element];
  if (states_[pp].length + 1 == states_[qq].length) {
    states_[current].link = qq;
  } else {
    states_.emplace_back(states_[qq]);
    int clone = states_.size() - 1;
    states_.back().length = states_[pp].length + 1;
    for (; pp != kNoLink && states_[pp].transitions[element] == qq; pp = states_[pp].link) {
      states_[pp].transitions[element] = clone;
    }
    states_[qq].link = states_[current].link = clone;
  }
}

constexpr int SuffixAutomaton::kNoLink;

} // namespace AlgoLib
