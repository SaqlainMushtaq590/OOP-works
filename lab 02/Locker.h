#include<iostream>
#include<string>

using namespace std;

class Locker{
public:
	Locker(){
	cout<<"Locker allocated to customer"<<endl;
	}

	~Locker(){
	cout<<"Locker returned by customer"<<endl;
	}
	
	
};
