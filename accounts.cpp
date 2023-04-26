//
//  main.cpp
//  testCode
//
//  Created by Daniel Irani on 3/29/23.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

void saveLogin(string uName, string pWord);
string addNewUser();
void showUsernames();
void showPasswords();
void showLogins();
bool checkLogin();

/*
int main(int argc, const char * argv[])
{
    int option = 0;
    cout << "###Welcome###\n\n";

    while(option != 6){
        // menu
        cout << "\n##########################\n";
        cout << "Enter 1 - 6 \n\n";
        cout << "1. Add a new user\n";
        cout << "2. Show all user names\n";
        cout << "3. Show all passwords\n";
        cout << "4. Show all logins\n";
        cout << "5. Check login credentials!\n";
        cout << "6. Exit!\n";

        cin >> option;

        switch(option){
            case 1:
                cout << "1 selected!\n";
                addNewUser();
                break;
            case 2:
                cout << "2 selected!\n";
                showUsernames();
                break;
            case 3:
                cout << "3 selected!\n";
                showPasswords();
                break;
            case 4:
                cout << "4 selected!\n";
                showLogins();
                break;
            case 5:
                cout << "Checking login! \n";
                if(!checkLogin()){
                    cout << "Invalid login credentials!\n";
                }
                break;
            case 6:
                cout << "Goodbye!\n"; 
                break;
            default:
                cout << "Error! Please enter a valid input!\n";
                break;
                }
    }


    return 0;
}
*/


// handles adding of a new user account
string addNewUser(string uName, string pWord){

    string userName = uName;
    string passWord = pWord;

    /*
    string userName = "";
    string passWord = "", passwordConfirm = "";

    cout << "Enter username for new user: \n";
    cin >> userName;
    cout << "Enter password for " << userName << ": ";
    cin >> passWord;
    cout << "Please re-enter password to confirm: ";
    cin >> passwordConfirm;

    // checking for password match
    while(passWord != passwordConfirm){
        cout << "Error! Password does not match. \n";

        cout << "Enter password for " << userName << ": ";
        cin >> passWord;
        cout << "Please re-enter password to confirm: ";
        cin >> passwordConfirm;
    }
    */

    cout << "Saving login credentials!\n";
    saveLogin(userName, passWord);
    cout << "Credentials saved!\n";
    return userName;
}

// saves username and password to login saver file
void saveLogin(string uName, string pWord){
    string delim = " ";

    // saving user credentials to file
    ofstream credentials("logins.txt", fstream::app);

    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    credentials << uName << delim << pWord << endl;

    credentials.close();
}

// shows all usernames
void showUsernames(){

    string fileContent = "";
    string username = "";
    string delimiter = " ";
    vector<string> userNames;

    ifstream credentials("logins.txt");

    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    while(getline(credentials, fileContent)){
        // finding where the delimter is while reading from file and saving its spot
        size_t delimSpot = fileContent.find(delimiter);
        // saving a username
        username = fileContent.substr(0,delimSpot);
        // adding username to vector to avoid overwriting if more than one
        userNames.push_back(username);

    }

    cout << "All usernames: \n";
    // printing usernames
    for(int i = 0; i < userNames.size(); ++i){
        cout << i + 1 << ") " << userNames[i] << endl;
    }


    credentials.close();
}


// shows all passwords
void showPasswords(){

    string fileContent = "";
    string password = "";
    string delimiter = " ";
    vector<string> passwords;

    ifstream credentials("logins.txt");

    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    while(getline(credentials, fileContent)){
        // finding where the delimter is while reading from file and saving its spot
        size_t delimSpot = fileContent.find(delimiter);
        // saving a password
        password = fileContent.substr(delimSpot);
        // adding password to vector to avoid overwriting if more than one
        passwords.push_back(password);

    }

    cout << "All passwords: \n";
    // printing passwords
    for(int i = 0; i < passwords.size(); ++i){
        cout << i + 1 << ") " << passwords[i] << endl;
    }


    credentials.close();
}

void showLogins(){
    
    string fileContent = "";
    string username = "";
    string password = "";
    string delimiter = " ";
    vector<string> userNames;
    vector<string> passwords;
    
    ifstream credentials("logins.txt");
    
    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    
    while(getline(credentials, fileContent)){
        // finding where the delimter is while reading from file and saving its spot
        size_t delimSpot = fileContent.find(delimiter);
        // saving a username
        username = fileContent.substr(0,delimSpot);
        // adding username to vector to avoid overwriting if more than one
        userNames.push_back(username);
        // saving a password
        password = fileContent.substr(delimSpot);
        // adding password to vector to avoid overwriting if more than one
        passwords.push_back(password);
    }
    
    
    
    cout << "All usernames and passwords: \n";
    // printing usernames
    for(int i = 0; i < userNames.size(); ++i){
        cout << i + 1 << ") Username: " << userNames[i] << "\nPassword: " << passwords[i] << endl;
    }
}

// checks if a username and password combo is valid or not
bool checkLogin(string un, string pw){

    string pWord = "";
    string uName = "";
    string fileContent = "";
    string username = "";
    string password = "";
    string delimiter = " ";
    vector<string> userNames;
    vector<string> passwords;
    
    //cout << "Enter username: ";
    //cin >> uName;
    //cout << "Enter password: ";
    //cin >> pWord;
    
    uName = un;
    pWord = pw;

    
    pWord.insert(0, " "); // adding delimeter for checking purposes
    
    
    ifstream credentials("logins.txt");
    
    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    
    while(getline(credentials, fileContent)){
        // finding where the delimter is while reading from file and saving its spot
        size_t delimSpot = fileContent.find(delimiter);
        // saving a username
        username = fileContent.substr(0,delimSpot);
        // adding username to vector to avoid overwriting if more than one
        userNames.push_back(username);
        // saving a password
        password = fileContent.substr(delimSpot);
        // adding password to vector to avoid overwriting if more than one
        passwords.push_back(password);
    }
    
    for(int i = 0; i < userNames.size(); ++i){
        if((uName == userNames[i]) && (pWord == passwords[i])){
            cout << "Valid login credentials!\n";
            return true;
        }
    }
    
    return false;
}
