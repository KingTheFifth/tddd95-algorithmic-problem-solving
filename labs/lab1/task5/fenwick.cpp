#include "stdio.h"
#include <cstdint>
#include <ios>
#include <vector>

typedef int64_t i64;

using namespace std;

struct FenwickTree {
	vector<i64>	 bit;
	i64 length;

	// Add number delta to element at index
	i64 add(i64 index, i64 delta) {
	}

	// Return sum of elements in range [0, index]
	i64 sum(i64 index) {
	}
};

int main() {
}
