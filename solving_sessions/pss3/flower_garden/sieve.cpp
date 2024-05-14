/**
 * Johannes Kung johku144
 *
 * Sieve of Erathostenes.
 *
 * Time complexity: 
 * - Constructor: O(n*log(n))
 * - Querying: O(1)
 *
 * Memory consumption: O(n)
 */
#include <vector>

using namespace std;

/** 
 * Straightforward implementation of the sieve of Erathostenes as a class.
 * An instance of the class for the number n can be queried for all primes 
 * up to N and whether a given number in the interval [0, N] is a prime 
 * in constant time.
 * Constructing an instance is O(N*log(N))
 */
class PrimeSieve {
  vector<bool> _is_prime;
  int primes = 0;

  /**
   * Input: an integer n for which all prime numbers up to n will be calculated
   * Time complexity: O(n*log(n))
   */
public:
  PrimeSieve(int n) {
    _is_prime = vector<bool>(n+1, true);
    _is_prime[0] = false;
    _is_prime[1] = false;
    for (int i = 2; i <= n; ++i) {
      if (_is_prime[i]) {
        primes++;
        for (int j = 2; j <= n / i; ++j) {
          _is_prime[i*j] = false;
        }
      }
    }
  }

  /**
   * Returns true if the number n is prime, false otherwise.
   */
  bool is_prime(int n) {
    return _is_prime[n];
  }

  /**
   * Returns the number of primes between 0 and n.
   */
  int get_primes() {
    return primes;
  }
};
