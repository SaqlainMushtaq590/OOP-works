#include<iostream>
using namespace std;

class Students  {
	public:
		int id;
		static int count;
		
		Students(){
			count++;
		}
		
		static void showCount(){
			
			cout<<" total  students "<<count<<endl;
		//	cout<<" id of students "<<id<<end;
		}
};

int Students::count = 0;

int main(){
	Students a;
	Students b;
	Students c;
	
	Students::showCount();
	
	return 0;
	
}
