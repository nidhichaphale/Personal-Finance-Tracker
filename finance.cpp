#include <iostream>
#include <windows.h>
#include <limits>
using namespace std;

class User {
private:
    string username;
    string password;
    string email;
    // Removed date from User

public:
    void login() {
        cout << "Username: ";
        getline(cin, username);
        cout << "Email: ";
        getline(cin, email);
        cout << "Password: ";
        getline(cin, password);
        // No date input here
    }

    void updateProfile() {
        string passcheck;
        int choice;

        cout << "\nVerification Required: ";
        cout << "\nEnter your password: ";
        getline(cin, passcheck);

        if (passcheck != password) {
            cout << "Wrong Password! Cannot update profile.\n";
            return;
        }

        cout << "\nWhat do you want to update?\n";
        cout << "1. Update Username\n";
        cout << "2. Update Email\n";
        cout << "3. Update Password\n";
        cout << "Enter your choice (1-3): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                cout << "Enter new username: ";
                getline(cin, username);
                cout << "Updated Username: " << username << endl;
                break;
            case 2:
                cout << "Enter new email: ";
                getline(cin, email);
                cout << "Updated Email: " << email << endl;
                break;
            case 3:
                cout << "Enter new password: ";
                getline(cin, password);
                cout << "Updated Password.\n";
                break;
            default:
                cout << "Invalid choice! Please enter a number between 1 and 3.\n";
        }
    }

    void displayProfile() {
        cout << "\n--- User Profile ---\n";
        cout << "Username: " << username << endl;
        cout << "Password: " << password << endl;
        cout << "Email: " << email << endl;
        // No date display here
    }
};

class Transaction {
protected:
    double amount;
    string date;

public:
    virtual void recordTransaction() = 0;
    virtual void displayTransaction() = 0;
};

class Income : public Transaction {
private:
    string income_source;
    bool isRecurring;

public:
    void recordTransaction() override {
        cout << "Enter amount: ";
        cin >> amount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter date (dd-mm-yy): ";
        getline(cin, date);

        cout << "Enter income source: ";
        getline(cin, income_source);

        string input;
        cout << "Is the income recurring? (yes/no): ";
        getline(cin, input);

        if (input == "yes" || input == "Yes")
            isRecurring = true;
        else if (input == "no" || input == "No")
            isRecurring = false;
        else {
            cout << "Invalid input. Assuming not recurring.\n";
            isRecurring = false;
        }
    }

    void displayTransaction() override {
        cout << "\n--- Income ---" << endl;
        cout << "Amount: " << amount << endl;
        cout << "Date: " << date << endl;
        cout << "Income Source: " << income_source << endl;
        cout << "Recurring: " << (isRecurring ? "Yes" : "No") << endl;
    }
};

class Expense : public Transaction {
private:
    string category;
    string payment_method;
    bool isTaxDeductible;

public:
    void recordTransaction() override {
        cout << "Enter amount: ";
        cin >> amount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter date (dd-mm-yy): ";
        getline(cin, date);

        cout << "Enter category: ";
        getline(cin, category);

        cout << "Payment method: ";
        getline(cin, payment_method);

        string input;
        cout << "Is tax deductible? (yes/no): ";
        getline(cin, input);

        if (input == "yes" || input == "Yes") {
            isTaxDeductible = true;
        } else if (input == "no" || input == "No") {
            isTaxDeductible = false;
        } else {
            cout << "Invalid input! Assuming not tax deductible." << endl;
            isTaxDeductible = false;
        }
    }

    void displayTransaction() override {
        cout << "\n--- Expense ---" << endl;
        cout << "Amount: " << amount << endl;
        cout << "Date: " << date << endl;
        cout << "Category: " << category << endl;
        cout << "Payment Method: " << payment_method << endl;
        cout << "Tax Deductible: " << (isTaxDeductible ? "Yes" : "No") << endl;
    }
};

int main() {
    HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << endl;
    SetConsoleTextAttribute(hc, 14);
    cout << "*******************************" << endl;
    cout << "*    PERSONAL FINANCE TRACKER *" << endl;
    cout << "*******************************";
    SetConsoleTextAttribute(hc, 0x07);
    cout << endl;

    User user;
    Income income;
    Expense expense;

    int choice;
    do {
        SetConsoleTextAttribute(hc, 4);
        cout << "\nMenu:\n";
        SetConsoleTextAttribute(hc, 0x07);
        cout << endl;
        SetConsoleTextAttribute(hc, 10);
        cout << "1. Login (Create Profile)\n";
        cout << "2. Update Profile\n";
        cout << "3. View Profile\n";
        cout << "4. Add Income\n";
        cout << "5. Add Expense\n";
        cout << "6. View Last Income\n";
        cout << "7. View Last Expense\n";
        cout << "8. Exit\n";
        SetConsoleTextAttribute(hc, 0x07);
        cout << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clear input buffer

        switch (choice) {
            case 1:
                cout << "\n--- Create Profile ---\n";
                user.login();
                break;
            case 2:
                user.updateProfile();
                break;
            case 3:
                user.displayProfile();
                break;
            case 4:
                cout << "\n--- Add Income ---\n";
                income.recordTransaction();
                break;
            case 5:
                cout << "\n--- Add Expense ---\n";
                expense.recordTransaction();
                break;
            case 6:
                income.displayTransaction();
                break;
            case 7:
                expense.displayTransaction();
                break;
            case 8:
                SetConsoleTextAttribute(hc, 5);
                cout << "\nThank you for using the Personal Finance Tracker!\n";
                SetConsoleTextAttribute(hc, 0x07);
                cout << endl;
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}