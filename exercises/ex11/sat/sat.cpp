#include <cstdint>
#include <iostream>
#include <ios>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
// Represent a clause as two bitmasks (uint32 because why not)
// The first has bit i set to 1 if the variable is in the clause, 0 otherwise
// The second has bit i set to 1 if literal xi is negated, 0 otherwise
using clause = pair<uint32_t, uint32_t>;
using formula = vector<clause>;

bool clause_is_true(clause& c, uint32_t assignment) {
  // Some math I worked out on intuition that evaluates a clause under an 
  // assignment:
  //
  // XORing the negation bits with the assignment gives a bit sequence where 
  // bit i is 1 if literal i is true under the assignment to variable X_i.
  //
  // ANDing this literal value bit sequence with the bit sequence indicating 
  // whether literal i appears in this clause or not sets bit i to 1 only if 
  // the literal is used in the clause and the literal evaluates to true
  //
  // The clause is true if any literal evaluates to true, e.g. if at least one 
  // bit is set to 1. Interpreting the bit sequence as a number then means 
  // that the number is not equal to 0
  
  return (c.first & (c.second ^ assignment)) > 0;
}

bool formula_is_true(formula &f, uint32_t assignment) {
  // If some clause is false under the assignment, then the whole 
  // formula is false (the formula is in CNF)
  for (int i = 0; i < f.size(); ++i) {
    if (!clause_is_true(f[i], assignment)) {
      return false;
    }
  }

  // Else all clauses are true => the formula is true
  return true;
}

bool sat(formula &f, int vars) {
  // Naively try every possible assignment of the variables and evaluate 
  // the formula to see if some assignment satisfies the formula
  uint32_t assignment = 0;
  while (assignment < 1 << vars) {
    if (formula_is_true(f, assignment)) {
      return true;
    }
    assignment += 1;
  }

  return false;
}


int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int cases, vars, clauses;
  string in;
  cin >> cases;
  while (cases-- > 0) {
    cin >> vars >> clauses;
    cin.ignore();

    // Here starts the input handling of the clauses x(
    // Also removes clauses that are tautologies for convenience, i.e.
    // clauses containing both X_i and ~X_i
    formula f;
    while (clauses-- > 0) {
      // Read the clause line and break it into tokens
      getline(cin, in) ;
      stringstream ss(in);
      vector<string> tokens;
      string token;
      while (ss >> token) {
        tokens.push_back(token);
      }

      // Construct the clause from the tokens
      uint32_t vars_in_use = 0;
      uint32_t negated_vars = 0;
      bool tautology = false;
      for (string token : tokens) {
        if (token.compare("v") == 0) continue;

        // Let variable ids start at 0 for convenienve when calculating 
        // satisfiability and bitwise operations
        int var_id = stoi(token.substr(token.find_first_of("X")+1, token.size()))-1;
        bool negated = (token.at(0) == '~');

        // Handle when a variable is present more than once in a clause
        if ((vars_in_use >> var_id) & 0x1) {
          // Appears with same sign as all previously found occurences
          // => just skip the literal
          if (((negated_vars >> var_id) & 0x1) == negated) {
            continue;
          }

          // Appears with the opposite sign so the clause i.e. disjuction 
          // is always true => the entire clause can be skipped
          else {
            tautology = true;
            break;
          }
        }

        // First time processing this variable,
        // set its 'used' bit and sign bit
        vars_in_use = vars_in_use | (0x1 << var_id);
        if (token.at(0) == '~') {
          negated_vars = negated_vars | (0x1 << var_id);
        }
      }

      // skip a clause if it is always true since there is no need to 
      // evaluate it ever
      if (!tautology) {
        f.push_back({vars_in_use, negated_vars});
      }
    }

    if (sat(f, vars)) {
      cout << "satisfiable\n";
    }
    else {
      cout << "unsatisfiable\n";
    }

  }

  cout.flush();
}
