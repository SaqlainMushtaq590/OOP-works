#include<iostream>
using namespace std;


class Person{
    public:
    string name;
    int age;



    void display_person(){
        cout<<"Name : "<<name<<endl;
        cout<<"Age  : "<<age<<endl;
    }
};

class Employee: public Person{
    public:
     string employee_id;



     void display_employee(){
        display_person();
        cout<<"Employee Id : "<<employee_id<<endl;

     }
};

class Manager:public Employee{
    public: 
     string department;


     void display_manager(){
        display_employee();
        cout<<"Department : "<<department<<endl;
     }

};



int main(){
    Manager m1;
    m1.name ="saqlian";
    m1.age = 12;
    m1.employee_id= "23qw";
    m1.department= "AI";
    m1.display_manager();
}
