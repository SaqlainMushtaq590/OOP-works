#include <iostream>
using namespace std;

// Display class (used inside Calculator)
class Display {
    int lastResult; // keeps track of last result shown
public:
    Display() : lastResult(0) {}

    void showResult(int result) {
        lastResult = result;
        cout << "Result: " << result << endl;
    }

    int getLastResult() {
        return lastResult;
    }
};

// Calculator class (has a Display object -> Composition)
class Calculator {
    Display display; // Strong ownership (composition)
public:
    void add(int a, int b) {
        int result = a + b;
        display.showResult(result); // delegate to Display
    }

    void multiply(int a, int b) {
        int result = a * b;
        display.showResult(result); // delegate to Display
    }

    void showLastResult() {
        cout << "Last result stored in display: " 
             << display.getLastResult() << endl;
    }
};

// Main program
int main() {
    Calculator calc;

    calc.add(5, 7);        // performs addition and shows result
    calc.multiply(3, 4);   // performs multiplication and shows result
    calc.showLastResult(); // display remembers last result

    return 0;
}

