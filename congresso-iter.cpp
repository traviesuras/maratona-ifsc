#include <iostream>
#include <vector>
#include <queue>
#include <iterator>
#include <algorithm>

typedef std::pair<int, std::vector<int>> candidate_t;
typedef std::vector<std::vector<int>> majority_list;

// Finds all party sets summing at least "needed" votes
majority_list find_majorities(int needed, std::vector<int> dep);
// Send to standar out all sets, one per line
void show_majorities(majority_list all);

// The main routine reads the input, computes needed votes for
// majority and calls the functions to find majorities and print them.

int main()
{
  int N, // Number of parties
    total = 0; // Total number of votes (deputies)
  std::cin >> N;
  std::vector<int> dep(N);
  for (int i = 0; i < N; i++) {
    std::cin >> dep[i];
    total += dep[i];
  }
  // Majority is half the votes plus one
  // Remember: integer division!
  int needed = total/2 + 1;
  // Compute and show majorities
  show_majorities(find_majorities(needed, dep));
}

majority_list find_majorities(int needed, std::vector<int> dep)
{
  majority_list all_majorities;
  // Pending has a queue of pending combinations of parties to be
  // analized.
  std::queue<candidate_t> pending;
  // Start with all parties, and none already considered.
  std::vector<int> all(dep.size());
  std::iota(all.begin(), all.end(), 1);
  pending.push(std::make_pair(0,all));
  // While there are pending combinations...
  while ( ! pending.empty() ) {
    // Get first pending combination
    candidate_t colcand = pending.front();
    pending.pop();
    auto level = colcand.first;
    auto col = colcand.second;
    // See how many votes it has
    int achieved = 0;
    for (auto p : col) {
      achieved += dep[p - 1];
    }
    if (achieved >= needed) {
      // If there are already enough votes, register.
      all_majorities.push_back(col);
      // Now insert in pending combinations without excluding each of
      // the parties.
      for (auto i : col) {
	if (i > level) {
	  std::vector<int> newcol(col.size()-1);
	  std::remove_copy(col.begin(), col.end(), newcol.begin(), i); 
	  pending.push(std::make_pair(i,newcol));
	}
      }
    }
  }
  return all_majorities;
}

void show_majorities(majority_list all)
{
  for (auto majority : all) {
    std::copy(majority.begin(), majority.end(), 
	      std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
  }
}
