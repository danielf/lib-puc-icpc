#include "../template.cpp"
#include "RMQ.cpp"

int main() {
	vi A{1,2,1,5,7,3,-1,8,10,9};
	RMQ R(A);
	WATCH(R.get(1,1));
	WATCH(R.get(0,9));
	WATCH(R.get(0,5));
	WATCH(R.get(7,9));
	WATCH(R.get(0,0));
	WATCH(R.get(9,9));
}
