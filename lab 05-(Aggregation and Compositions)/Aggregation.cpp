#include <iostream>
using namespace std;

// Calculator class (independent object)
class Calculator {
public:
    int add(int a, int b) {
        return a + b;
    }

    int multiply(int a, int b) {
        return a * b;
    }
};

// Student class (refers to Calculator but does not own it)
class Student {
    string name;
    Calculator* calc; // Aggregation: student has reference to calculator
public:
    Student(string n, Calculator* c) {
        name = n;
        calc = c;
    }

    void solveAddition(int a, int b) {
        cout << name << " used calculator: "
             << a << " + " << b << " = " << calc->add(a, b) << endl;
    }

    void solveMultiplication(int a, int b) {
        cout << name << " used calculator: "
             << a << " * " << b << " = " << calc->multiply(a, b) << endl;
    }
};

// Main program
int main() {
    // Shared calculator
    Calculator sharedCalculator;

    // Students use the shared calculator
    Student s1("Ali", &sharedCalculator);
    Student s2("Sara", &sharedCalculator);

    s1.solveAddition(5, 3);
    s2.solveMultiplication(4, 6);

    return 0;
}

