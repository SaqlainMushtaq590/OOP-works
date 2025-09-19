#include<iostream>
using namespace std;

class Distance{
	
private:
	double feet;
	double inches;
	 		
public:
    // Constructor to initialize values
    Distance(int f = 0, int i = 0) {
        feet = f;
        inches = i;
    }

    // Overload == operator
    bool operator==(const Distance &d) {
        return (feet == d.feet && inches == d.inches);
    }

    // Function to display distance
    void display() {
        cout << feet << " feet " << inches << " inches";
    }
};

int main() {
    Distance d1(5, 8);  
    Distance d2(5, 8);   
    Distance d3(6, 2);   

    cout << "Comparing d1 and d2: ";
    if (d1 == d2) {
        cout << "Both distances are equal." << endl;
    } else {
        cout << "Distances are NOT equal." << endl;
    }

    cout << "Comparing d1 and d3: ";
    if (d1 == d3) {
        cout << "Both distances are equal." << endl;
    } else {
        cout << "Distances are NOT equal." << endl;
    }

    return 0;
}
