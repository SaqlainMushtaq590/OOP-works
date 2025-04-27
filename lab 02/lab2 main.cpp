#include<iostream>
#include<string>
#include"Employee.h"
#include"Bankaccount.h"
#include"rectangle.h"
#include"Locker.h"
using namespace std;

//=================================task 1==================================

//int main(){
//	
//	Employee E1;
//	E1.displayDetails();
//	
//	return 0;
//
//}

//===============================task 2=====================================

//int main(){
//	string accNum , accHolder;
//    double balance;
//
//    do{
//    	cout<<"enter Account Number (15 charactor long!): ";
//    	cin>>accNum;
//	}
//	while(accNum.length()!=15);
//	
//	
//	cout<<"\nEnter Account Holder's name :";
//	cin>>accHolder;
//	
//	
//	do{
//		cout<<"\nEnter initial balance(atleast 1000 PKR): ";
//		cin>>balance;
//	}
//	while(balance<1000);
//	
//	
//    
//	BankAccount a(accNum , accHolder,balance );
//	a.ShowDetails();
//	
//	return 0;
//}


//==================================task 3=============================

//int main(){
//	
//	cout<<"\n\nBy default Constractor\n******************* \n"<<endl;
//	Rectangle R1;
//	R1.display();
//	R1.area();
//	
//	cout<<"\n\nBy single Parametric Constractor\n*******************\n "<<endl;
//	Rectangle R2(5.6);
//	R2.display();
//	R2.area();
//	
//	
//	cout<<"\n\nBy Parameterized Constractor\n*******************\n "<<endl;
//	float a,b;
//	cout<<"Enter the length and width of rectangle : ";
//	cin>>a>>b;
//	Rectangle R3(a,b);
//	R3.display();
//	R3.area();
//}


//==============================task 4===================================

int main(){
	Locker L1;

	Locker* L2= new Locker();
	delete L2;	
}
