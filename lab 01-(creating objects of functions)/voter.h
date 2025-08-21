#include<iostream>
#include<string>
using namespace std;

class Voter{
	public:
	string name;
	int age;
	bool isEligible();
};
bool Voter::isEligible() {
    if (age > 18) {
        return true;
    } else {
        return false;
    }
}
