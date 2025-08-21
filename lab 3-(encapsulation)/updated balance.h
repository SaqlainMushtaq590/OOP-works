#include <iostream>
using namespace std;

class BankAccount {
private:
    double* balance;

public:
     //constrator
    BankAccount(double initialBalance) {
        balance = new double(initialBalance); 
    }

    // Destructor
    ~BankAccount() {
        delete balance;  
        cout << "Destructor called.\n";
    }

    // Display balance
    void displayBalance() const {
        cout << "Balance: $" << *balance << endl;
    }

    // Modify balance
    void setBalance(double newBalance) {
        *balance = newBalance;
    }
    
    BankAccount(const BankAccount& other) {
        balance = new double(*other.balance);  // Allocate new memory and copy value
        cout << "Deep copy constructor called.\n";
    }
};
