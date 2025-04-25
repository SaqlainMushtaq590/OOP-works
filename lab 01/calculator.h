#include<iostream>
#include<string>
using namespace std;

class Calculator{
	public:
		float num1;
		float num2;
	double add();
	double subtract();
	double multiply();
	double divide();
};
double Calculator::add(){
	double sum=num1+num2;
	cout<<"\n sum  = "<<sum;
}
double Calculator::subtract(){
	double minus=num1-num2;
	cout<<"\n  subtraction = "<<minus;
}
double Calculator::multiply(){
	double mult=num1*num2;
	cout<<"\n  multiplication  = "<<mult;
}
double Calculator::divide(){
	double div=num1/num2;
	cout<<"\n  division  = "<<div;
}
