#include<iostream>
using namespace std;

class Printer{
	public:
		
	void print_document(){
		cout<<"Document is printed "<<endl;
	}	
	
};

class Scanner{
	public:  
	
	void scan_document(){
		cout<<"Document is scaned "<<endl;
	}
	
};


class Photocopier: public Printer, public Scanner{
	public: 
	
	void photocopy(){
		print_document();
		scan_document();
	}
};


int main(){
	Photocopier f;
	f.photocopy();
}
	
	
	
	
	
