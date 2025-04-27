#include<iostream>
#include<string>

using namespace std;

class BankAccount{
	string AccountNumber;
	string AccountHolder;
	double Balance;
	
public:
	
	BankAccount(string a, string b, double c){
	 
		AccountNumber=a;	
		AccountHolder=b;
		Balance=c;
	}
	
	ShowDetails(){
		
		cout<<"\n Account Number : "<<AccountNumber<<endl;
		cout<<"\n Account Holder : "<<AccountHolder<<endl;
		cout<<"\n  Balance      =  "<<Balance<<endl;
		
	}

};
