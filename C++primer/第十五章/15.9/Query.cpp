#include "Query.h"
using namespace std;

ostream& operator<<(ostream& os, const Query& query) {
	return os << query.rep();
}
