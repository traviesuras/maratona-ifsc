#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

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

// This is the function that does the main work.
// Given:
//   next -> next party to consider
//   col -> a current set of parties (colligation)
//   needed -> number of votes for majority
//   dep -> dep[i] is the number of votes of party i+1
// Results: adds all sets of parties generated from col and that give
// a majority to all_majorities.
void coligate(int next, std::vector<int> col, 
	      int needed, std::vector<int> dep,
	      majority_list &all_majorities)
{
  int n = dep.size();// Number of parties
  // Is next a valid party? If not, do nothing.
  if (next <= n) {
    // There are three possibilities to find majorities:
    // 1. Party next needs not be included
    // 2. Including next you already have a party
    // 3. You can include next and then others

    // First, consider possibility 1.
    coligate(next+1, col, needed, dep, all_majorities);

    // Now see if 2. is valid
    col.push_back(next);
    int achieved = 0;
    for (auto p: col)
      achieved += dep[p-1];
    if (achieved >= needed) {
      all_majorities.push_back(col);
    }

    // Now consider possibility 3.
    coligate(next+1, col, needed, dep, all_majorities);    
  }
}

majority_list find_majorities(int needed, std::vector<int> dep)
{
  // Starts with an empty list of majorities and 1 as the next party
  // to consider.
  majority_list all_majorities;
  coligate(1, std::vector<int>(), needed, dep, all_majorities);
  return all_majorities;
}

template<typename Container>
void show_elements_in_line(Container cont)
{
  std::copy(cont.begin(), cont.end(), 
	    std::ostream_iterator<typename Container::value_type>(std::cout, " "));
  std::cout << std::endl;
}

void show_majorities(majority_list all)
{
  for (auto majority : all) {
    show_elements_in_line(majority);
  }
}
