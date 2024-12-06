#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <cctype>  // For isdigit()
#include <stdexcept>  // For runtime_error
#include "Deque.h"  // Include the Deque class

using namespace std;

/**
 * @file main.cpp
 * @brief This program evaluates infix expressions by converting them to postfix notation using a Deque 
 *        implementation of a stack and then evaluates the postfix expressions. It reads values for variables
 *        from a file, handles multiple expressions, and includes error checking for division by zero.
 * 
 * @details Expressions use single-letter variables ('a' to 'f'), whose values are read from "values.txt".
 *          The program supports the basic arithmetic operators: +, -, *, and /.
 * 
 * @author Chidubem Okoye
 * @date December 5, 2024
 * @class [Insert Class Name Here]
 * @professor [Insert Professor Name Here]
 */

/**
 * @brief Determines the precedence of an arithmetic operator.
 * 
 * @param op The operator character ('+', '-', '*', '/').
 * @return The precedence level of the operator (1 for + and -, 2 for * and /).
 */
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

/**
 * @brief Converts an infix expression to postfix notation using a Deque as a stack.
 * 
 * @param infix The input infix expression as a string.
 * @return The equivalent postfix expression as a string.
 */
string infixToPostfix(const string& infix) {
    Deque<char> opStack;  // Stack implemented using Deque
    string postfix = "";

    for (char ch : infix) {
        if (isdigit(ch) || isalpha(ch)) { // Operand
            postfix += ch;
        } else if (ch == '(') { // Open parenthesis
            opStack.pushBack(ch);
        } else if (ch == ')') { // Close parenthesis
            while (!opStack.isEmpty() && opStack.back() != '(') {
                postfix += opStack.back();
                opStack.popBack();
            }
            if (!opStack.isEmpty()) {
                opStack.popBack();  // Discard '('
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') { // Operator
            while (!opStack.isEmpty() && precedence(opStack.back()) >= precedence(ch)) {
                postfix += opStack.back();
                opStack.popBack();
            }
            opStack.pushBack(ch);
        }
    }

    while (!opStack.isEmpty()) { // Pop remaining operators
        postfix += opStack.back();
        opStack.popBack();
    }

    return postfix;
}

/**
 * @brief Evaluates a postfix expression using a Deque as a stack.
 * 
 * @param postfix The postfix expression to evaluate.
 * @param values An array of integer values for variables 'a' to 'f'.
 * @return The result of the evaluation as an integer.
 * @throws runtime_error If there is division by zero or an invalid operator.
 */
int evaluatePostfix(const string& postfix, const int values[6]) {
    Deque<int> stack;

    for (char ch : postfix) {
        if (isdigit(ch)) { // Numeric operand
            stack.pushBack(ch - '0');
        } else if (isalpha(ch)) { // Variable operand
            int index = ch - 'a';
            stack.pushBack(values[index]);
        } else { // Operator
            int b = stack.back(); stack.popBack();
            int a = stack.back(); stack.popBack();

            switch (ch) {
                case '+': stack.pushBack(a + b); break;
                case '-': stack.pushBack(a - b); break;
                case '*': stack.pushBack(a * b); break;
                case '/':
                    if (b == 0) throw runtime_error("Division by zero error");
                    stack.pushBack(a / b);
                    break;
                default: throw runtime_error("Unsupported operator");
            }
        }
    }

    if (stack.isEmpty()) {
        throw runtime_error("Error in postfix expression evaluation");
    }

    return stack.back();
}

/**
 * @brief Main function to read variable values from a file, evaluate multiple infix expressions, 
 *        and display their postfix conversions and results.
 * 
 * @return Exit status of the program (0 for success, non-zero for failure).
 */
int main() {
    int values[6] = {0}; // Array to store variable values

    // Reading values from "values.txt"
    ifstream inFile("values.txt");
    if (!inFile) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    string line;
    int index = 0;
    while (getline(inFile, line) && index < 6) { // Read up to 6 values
        stringstream ss(line);
        ss >> values[index++];
    }

    // Array of infix expressions
    string expressions[8] = {
        "a - b + c",
        "a - (b / c * d)",
        "a / (b * c)",
        "a / b / c - (d + e) * f",
        "(a + b) * c",
        "a * (b / c / d) + e",
        "a - (b + c)",
        "a - (b + c * d) / e"
    };

    // Evaluate each expression
    for (const string& infix : expressions) {
        cout << "Infix expression: " << infix << endl;

        // Substitute variables with their values
        string infixWithValues = infix;
        for (int i = 0; i < infixWithValues.length(); i++) {
            if (infixWithValues[i] >= 'a' && infixWithValues[i] <= 'f') {
                infixWithValues[i] = '0' + values[infixWithValues[i] - 'a'];
            }
        }

        cout << "Modified Infix expression: " << infixWithValues << endl;

        // Convert to postfix and evaluate
        string postfix = infixToPostfix(infixWithValues);
        cout << "Postfix expression: " << postfix << endl;

        try {
            int result = evaluatePostfix(postfix, values);
            cout << "The result of the expression is: " << result << endl;
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << endl;
        }

        cout << "----------------------------------------" << endl;
    }

    return 0;
}
