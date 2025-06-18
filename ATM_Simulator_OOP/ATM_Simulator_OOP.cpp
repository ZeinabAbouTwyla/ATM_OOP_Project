#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <cctype>
#include <conio.h> 
using namespace std;


string inputPassword() {
    string pass;
    char ch;

    while (true) {
        pass.clear();  

        while ((ch = _getch()) != '\r') {  
            if (ch == '\b') {  
                if (!pass.empty()) {
                    pass.pop_back();
                    cout << "\b \b";         
                }
            }
            else {
                pass += ch;
                cout << '*';  
            }
        }

        if (pass.empty()) {
            cout << "\nPassword cannot be empty. Please try again: ";
        }
        else {
            cout << endl;
            return pass;
        }
    }
}

class Users {
private:
    string name;
    string password;
    int id;
    static int Id_user;
    double balance = 0;

public:
    Users(string n, string p, double b) {
        name = formatName(n);
        password = p;
        id = ++Id_user;
        balance = b;

        cout << "Hello, " << name << endl
            << "The account is created." << endl
            << "Your ID is: " << id << endl;
    }

    static string formatName(const string& inputName) {
        if (inputName.empty()) return inputName;

        string formattedName;
        bool newWord = false;
        bool firstWord = true;

        for (size_t i = 0; i < inputName.length(); ++i) {
            char c = inputName[i];

            if (isspace(c)) {
                newWord = true;
            }
            else {
                if (firstWord) {
                    formattedName += toupper(c);
                    firstWord = false;
                    newWord = false;
                }
                else if (newWord) {
                    formattedName += ' ';
                    formattedName += toupper(c);
                    newWord = false;
                }
                else {
                    formattedName += tolower(c);
                }
            }
        }

        return formattedName;
    }

    string get_pass() {
        return password;
    }

    int get_id() {
        return id;
    }

    double get_bal() {
        return balance;
    }

    void withdraw_bal(double d) {
        balance -= d;
    }

    void det_bal(double d) {
        balance += d;
    }
    string get_name() {
        return name;
    }
    void set_pass(string pass) {
        password = pass;
    }
};

int Users::Id_user = 100;

class Atm {
private:
    int current_index = -1;
public:
    vector<Users> users;

    void add_member(Users s) {
        users.push_back(s);
    }

    void login(int id, string pass) {
        int d = -1;
        int check = 0;

        do {
            for (int i = 0; i < users.size(); i++) {
                if (users[i].get_id() == id && users[i].get_pass() == pass) {
                    current_index = i;
                    int choice;

                    cout << "\nWelcome back!\n" << users[i].get_name() << endl;
                    cout << "\nWhat do you want to do?" << endl;
                    cout << "1. Check balance\n"
                        << "2. Deposit\n"
                        << "3. Withdraw\n"
                        << "4. Reset password" << endl;

                    if (!(cin >> choice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a number.\n";
                        continue;
                    }

                    switch (choice) {
                    case 1:
                        get_balance();
                        break;
                    case 2: {
                        double amount;
                        cout << "Enter deposit amount: ";
                        if (!(cin >> amount)) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid amount. Please try again.\n";
                            break;
                        }
                        depositAmount(amount);
                        break;
                    }
                    case 3: {
                        double withdAmount;
                        cout << "Enter amount to withdraw: ";
                        if (!(cin >> withdAmount)) {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid amount. Please try again.\n";
                            break;
                        }
                        withdrawAmount(withdAmount);
                        break;
                    }
                    case 4:
                        reset_pass();
                        break;
                    default:
                        cout << "Invalid option.\n";
                    }

                    check = 1;
                    break;
                }
            }

            if (check == 0) {
                string chose;
                cout << "The password is not correct, do you want to try again?\nYes or No? ";
                cin >> chose;
                if (chose == "yes" || chose == "Yes") {
                    int new_id;
                    cout << "Enter ID: ";
                    if (!(cin >> new_id)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid ID. Please try again.\n";
                        continue;
                    }
                    string new_pass;
                    cout << "Enter password: ";
                     new_pass = inputPassword();
                     login(new_id, new_pass);
                    return;
                }
                else {
                    return;
                }
            }

            cout << "\nDo you want to continue? Press -1 to exit or any other number to continue: ";
            if (!(cin >> d)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Exiting...\n";
                d = -1;
            }

        } while (d != -1);

        cout << "Hope you were satisfied with the service. Have a great day!" << endl;
    }

    double get_balance() {
        if (current_index != -1) {
            cout << "Your balance is " << users[current_index].get_bal() << " EGP" << endl;
            return users[current_index].get_bal();
        }
        else {
            cout << "You are not logged in!\n";
            return -1;
        }
    }

    void depositAmount(double d) {
        if (current_index != -1) {
            users[current_index].det_bal(d);
            cout << "Now your balance is: " << users[current_index].get_bal() << endl;
        }
        else {
            cout << "You are not logged in!\n";
        }
    }

    void withdrawAmount(double minus) {
        if (current_index == -1) {
            cout << "You are not logged in!\n";
            return;
        }

        if (minus > users[current_index].get_bal()) {
            cout << "You can't withdraw more than your balance\n";
            string chose;
            cout << "Do you want to try again? (yes or no): ";
            cin >> chose;

            if (chose == "yes" || chose == "Yes") {
                double w;
                cout << "Enter amount to withdraw: ";
                if (!(cin >> w)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid amount. Operation cancelled.\n";
                    return;
                }
                withdrawAmount(w);
            }
            else {
                return;
            }
        }
        else {
            users[current_index].withdraw_bal(minus);
            cout << "Your balance now is: " << users[current_index].get_bal() << endl;
        }
    }

    void reset_pass() {
        if (current_index == -1) {
            cout << "You are not logged in!\n";
            return;
        }

        string new_pass, confirm_pass;
        cout << "Enter new password: ";
        new_pass = inputPassword();
        cout << "Confirm new password: ";
         confirm_pass = inputPassword();

        if (new_pass != confirm_pass) {
            cout << "Passwords do not match. Please try again.\n";
            reset_pass();
            return;
        }

        users[current_index].set_pass(new_pass);
        cout << "The password has been changed successfully!" << endl;
    }
};

int main() {
    Atm atm;
    int choice;

    while (true) {
        cout << "\nATM MENU:\n";
        cout << "1. Create account\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name, pass;
            double balance;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter name: ";
            getline(cin, name);
            cout << "Enter password: ";
            pass = inputPassword();
            cout << "Enter initial balance: ";
            cin >> balance;

            Users u(name, pass, balance);
            atm.add_member(u);
        }
        else if (choice == 2) {
            int id;
            string pass;
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter password: ";
             pass = inputPassword();
            atm.login(id, pass);
        }
        else if (choice == 3) {
            cout << "Thanks for using the ATM!\n";
            break;
        }
        else {
            cout << "Invalid option!\n";
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return 0;
}
