

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

void saveLogin(string uName, string pWord);
void addNewUser();
bool checkUsername(string);
void showUsernames();
void showPasswords();
void showLogins();
bool checkLogin();
int getLineCount();
void removeLogin();
void encryptAll(int);
void decryptAll(int);

/*int main()
{
    int option = 0;
    // will be part of admin account where they pick a key in later sprint instead of hardcoded
    int key = 2;
    cout << "###Welcome###\n\n";

    while(option != 9){
        // menu
        cout << "\n##########################\n";
        cout << "Enter 1 - 9 \n\n";
        cout << "1. Add a new user\n";
        cout << "2. Show all user names\n";
        cout << "3. Show all passwords\n";
        cout << "4. Show all logins\n";
        cout << "5. Check login credentials!\n";
        cout << "6. Remove a user\n";
        cout << "7. Encrypt and print encrypted accounts\n";
        cout << "8. Decrypt and print decrypted accounts\n";
        cout << "9. Exit!\n";

        cin >> option;

        switch(option){
            case 1:
                addNewUser();
                break;
            case 2:
                showUsernames();
                break;
            case 3:
                showPasswords();
                break;
            case 4:
                showLogins();
                break;
            case 5:
                if(!checkLogin()){
                    cout << "Invalid login credentials!\n";
                }
                break;
            case 6:
                removeLogin();
                break;
            case 7:
                encryptAll(key);
                break;
            case 8:
                decryptAll(key);
                break;
            case 9:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Error! Please enter a valid input!\n";
                break;
                }
    }


    return 0;
}*/

// handles adding of a new user account
void addNewUser(){

    string userName = "";
    string passWord = "",
    passwordConfirm = "";
    
    // getting username and password from the user
    cout << "Enter username for new user: \n";
    cin >> userName;
    while(checkUsername(userName) == true){
        cout << "Error! Username already in use! Please try again.\n";
        cin >>userName;
        checkUsername(userName);
    }
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

    // confirming credentials were saved
    cout << "Saving login credentials!\n";
    saveLogin(userName, passWord);
    cout << "Credentials saved!\n";
}

// checking for duplicate username
bool checkUsername(string userN){
    string lineContent;
    string existingUsername;
    ifstream credentials("logins.txt");

        if (credentials.fail()) {
            cerr << "Error opening file" << endl;
            exit(1);
        }

    while (getline(credentials, lineContent)) {
            size_t pos = lineContent.find(' ');

            if (pos != string::npos) {
                existingUsername = lineContent.substr(0, pos);

                if (existingUsername == userN) {
                    credentials.close();
                    return true;
                }
            }
        }
        credentials.close();
    return false;
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
    
    // saving username and corresponding password to a line in the file
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

    // error handling if issue with file
    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    //saving usernames to string from file
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

    // error handling if issue with file
    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    // saving passwords to string variable
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

// displaying login credentials that have been saved
void showLogins(){
    
    string fileContent = "";
    string username = "";
    string password = "";
    string delimiter = " ";
    vector<string> userNames;
    vector<string> passwords;
    
    ifstream credentials("logins.txt");
    
    // error handling if issue with file
    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    
    // saving username and passwords to strings
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
bool checkLogin(){
    string pWord = "";
    string uName = "";
    string fileContent = "";
    string username = "";
    string password = "";
    string delimiter = " ";
    vector<string> userNames;
    vector<string> passwords;
    
    //getting username and password for checking
    cout << "Enter username: ";
    cin >> uName;
    cout << "Enter password: ";
    cin >> pWord;
    
    // adding delimeter for checking purposes
    pWord.insert(0, " ");
    
    ifstream credentials("logins.txt");
    
    // error handling if issue with file
    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    
    // getting login credentials for reference from the save file
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
    
    // checking for a match in login credentials
    for(int i = 0; i < userNames.size(); ++i){
        if((uName == userNames[i]) && (pWord == passwords[i])){
            cout << "Valid login credentials!\n";
            return true;
        }
    }
    
    return false;
}

// getting the number of lines in the login credentials file
int getLineCount(){

    string fileContent = "";
    int numLines = 0;
    
    ifstream credentials("logins.txt");
    
    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    
    
    // counting number of lines
    while(getline(credentials, fileContent)){
        numLines++;
    }
    return numLines;
}

// removes a login account
void removeLogin(){
    
    int removeSpot = 0;
    int numLines = getLineCount();
    string lineContent = "";
    string fileContent;
    string fContent[numLines];
    string username = "";
    string password = "";
    string delimiter = " ";
    vector<string> userNames;
    vector<string> passwords;
    
    
    
    cout << "You will need to enter username and password for removal: \n";
    
    ifstream credentials("logins.txt");
    
    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    
    // checking if login credentisals are valid
    if(checkLogin()){
        cout << "Please re-enter the username.\n";
        cin >> username;
        // saving content of file to string array
        for (int i = 0; i < numLines; ++i) {
            getline(credentials, lineContent);
            fContent[i] = lineContent;
            if(fContent[i].find(username) != string::npos){
                removeSpot = i;
            }
        }
    }
    // prepparing file to overwrite with upadated logins
    credentials.close();
    ofstream credentials2("logins.txt");
    
    
        // finidng what line to remove login credentials from
    cout << "username is: " << username << endl;
    
    for(int i = 0; i < numLines; ++i){
        if(i != removeSpot){
            credentials2 << fContent[i] << endl;
        }
    }
                
        credentials2.close();
}
    
// encrypts all data via ceasar cipher
void encryptAll(int key){
    int numLines = getLineCount();
    string lineContent = "";
    string fileContent = "";
    string encLine = "";
    string decLine = "";
    string fContent[numLines];
    string encContent[numLines];
    string decContent[numLines];
    

    ifstream credentials("logins.txt");
    
    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    
    
    // saving content of file to string array
    for (int i = 0; i < numLines; ++i) {
        getline(credentials, lineContent);
        fContent[i] = lineContent;
    }
    
    // showing current cleartext
    for (int i = 0; i < numLines; ++i) {
        cout << fContent[i] << endl;
    }
    
    
    // encrypting data
    for (int i = 0; i < numLines; ++i) {
        encLine = fContent[i];
        for(int j = 0; j < encLine.length(); ++j){
            encLine[j] += key;
        }
        encContent[i] = encLine;
    }
    
    // showing encrypted text
    cout << "Encrypted: \n";
    for (int i = 0; i < numLines; ++i) {
        cout << encContent[i] << endl;
    }

    
    // prepparing file to overwrite with upadated logins
    credentials.close();
    ofstream credentials2("logins.txt");
    
    
       
    // saving encrypted text
    for(int i = 0; i < numLines; ++i){
            credentials2 << encContent[i] << endl;
    }
                
        credentials2.close();
}

void decryptAll(int key){
    int numLines = getLineCount();
    string lineContent = "";
    string fileContent = "";
    string encLine = "";
    string decLine = "";
    string fContent[numLines];
    string encContent[numLines];
    string decContent[numLines];
  
 
    

    ifstream credentials("logins.txt");
    
    if (credentials.fail()) {
        cerr << "Error opening file" << endl;
        exit(1);
    }
    
    
    // saving content of file to string array
    for (int i = 0; i < numLines; ++i) {
        getline(credentials, lineContent);
        encContent[i] = lineContent;
    }
    
    // showing current encrypted text
    cout << "Encrypted: \n";
    for (int i = 0; i < numLines; ++i) {
        cout << encContent[i] << endl;
    }
    
    // decrypting data
    for (int i = 0; i < numLines; ++i) {
        decLine = encContent[i];
        for(int j = 0; j < decLine.length(); ++j){
            decLine[j] -= key;
        }
        decContent[i] = decLine;
    }
    
    // showing decrypted text
    cout << "Decrypted: \n";
    for (int i = 0; i < numLines; ++i) {
        cout << decContent[i] << endl;
    }
    
    
    // prepparing file to overwrite with upadated logins
    credentials.close();
    ofstream credentials2("logins.txt");
    
    // putting decrypted text back into file
    for(int i = 0; i < numLines; ++i){
            credentials2 << decContent[i] << endl;
    }
        credentials2.close();
}
