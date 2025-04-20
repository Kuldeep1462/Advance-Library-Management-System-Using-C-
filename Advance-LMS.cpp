#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <conio.h>

using namespace std;

// Function to get the last used ID from a file
int getLastID()
{
    ifstream file("last_id.txt");
    int lastID = 202500; // Default starting ID if file does not exist
    if (file.is_open())
    {
        file >> lastID;
        file.close();
    }
    return lastID;
}

// Function to update the last used ID in a file
void updateLastID(int newID)
{
    ofstream file("last_id.txt");
    if (file.is_open())
    {
        file << newID;
        file.close();
    }
}

class Authenticate
{
protected:
    int id;
    string password;

public:
    Authenticate(int id = 0, string password = "")
    {
        this->id = id;
        this->password = password;
    }

    bool is_Valid_user(int id, string pass)
    {
        return (this->password == pass && this->id == id);
    }

    int countDigits(int num)
    {
        return (num == 0) ? 1 : log10(abs(num)) + 1;
    }

    int role(int id, string pass)
    {
        if (is_Valid_user(id, pass))
        {
            int digitCount = countDigits(id);
            if (digitCount == 4)
                return 1; // Admin
            else if (digitCount == 5)
                return 2; // Librarian
            else if (digitCount == 6)
                return 3; // Member
        }
        return 0; // Invalid role
    }
};


class Member : public Authenticate
{
    string name;
    int roleType;

public:
    Member(int id = 0, string name = "", int role = 0, string pass = "") : Authenticate(id, pass)
    {
        this->name = name;
        this->roleType = role;
    }

    int GetRole()
    {
        return roleType;
    }

    int GetID()
    {
        return id;
    }

    string GetName()
    {
        return name;
    }

    // Function to save user to file
    void saveToFile()
    {
        ofstream file("users.txt", ios::app); // Open file in append mode
        if (file.is_open())
        {
            file << id << "," << name << "," << roleType << "," << password << endl;
            file.close();
        }
        else
        {
            cout << "Error opening file for writing.\n";
        }
    }
};



class Admin : public Authenticate
{
public:
   void create_Member(int pin)
{
    int count = 2;
    do
    {
        if (pin == 12062004)
        {
            string name;
            cout << "Please Enter Your Name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter Password: ";
            string pass = GetPassword();

            int newID = getLastID() + 1; // Get last used ID and increment
            updateLastID(newID);         // Save the new ID to file

            Member temp(newID, name, 3, pass);
            temp.saveToFile(); // Save member to file
            cout << "Member Created Successfully! Your ID: " << newID << endl;
            break;
        } 
        else if (pin == 12345678) 
        {
            string name;
            cout << "Please Enter Your Name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter Password: ";
            string pass = GetPassword();

            int newID = rand() % 90000 + 10000; // Generate a 5-digit random ID
            
            Member temp(newID, name, 2, pass);
            temp.saveToFile(); // Save member to file
            cout << "Member Created Successfully! Your ID: " << newID << endl;
            break;
        }
        else
        {
            cout << "Wrong Pin! " << count << " chances remaining.\n";
            count--;
            if (count > 0)
            {
                cout << "Enter Again: ";
                cin >> pin;
            }
            else
            {
                cout << "Too many failed attempts. Exiting...\n";
                exit(0);
            }
        }
    } while (count > 0);
}

    string GetPassword()
    {
        string pass;
        char ch;
        while ((ch = _getch()) != '\r')
        { // Read until Enter key is pressed
            if (ch == '\b')
            { // Handle backspace
                if (!pass.empty())
                {
                    pass.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                pass.push_back(ch);
                cout << "*"; // Masking password
            }
        }
        cout << endl;
        return pass;
    }
    void delete_Member() {
    int memberID;
    cout<<"Enter Id to delete: ";
    cin>>memberID;
    ifstream file("users.txt");
    ofstream tempFile("Temp.txt");
    string line;
    bool found = false;

    while (getline(file, line)) {
        stringstream ss(line);
        int id;
        string name, password;
        ss >> id;
        getline(ss, name, ',');
        getline(ss, password, ',');

        if (id == memberID) {
            found = true;
        } else {
            tempFile << line << endl;
        }
    }

    file.close();
    tempFile.close();

    if (found) {
        remove("users.txt");
        rename("Temp.txt", "users.txt");
        cout << "Member deleted successfully.\n";
        cout << "----------------------------------------------\n";
    } else {
        remove("Temp.txt");
        cout << "Member not found.\n";
        cout << "----------------------------------------------\n";
    }
}
} A1;

// Function to read user data from file and authenticate
bool authenticateUser(int id, string pass, int &userRole)
{
    ifstream file("users.txt");
    if (!file)
    {
        cout << "Error opening user database.\n";
        return false;
    }

    int fileID, fileRole;
    string fileName, filePass;

    while (file >> fileID)
    {
        file.ignore(); // Ignore comma
        getline(file, fileName, ',');
        file >> fileRole;
        file.ignore(); // Ignore comma
        getline(file, filePass);

        if (fileID == id && filePass == pass)
        {
            userRole = fileRole;
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

// void Member::saveToFile() {
//     ofstream file("users.txt", ios::app);
//     if (file.is_open()) {
//         file << id << "," << name << "," << roleType << "," << password << endl;
//         file.close();
//     }
// }

class Book {
public:
    int ISBN;
    string name;
    string author;
    bool available;
    string issuedTo;

    Book(int id, string name, string author, bool available = true, string issuedTo = "")
        : ISBN(id), name(name), author(author), available(available), issuedTo(issuedTo) {}

    void printToFile(ofstream &file) const
    {
        file << ISBN << "," << name << "," << author << "," << available << "," << issuedTo << endl;
    }
    void saveToFile();
};

void AddBooks()
{
    int ISBN;
    string name, author;
    cout << "Enter ISBN: ";
    cin >> ISBN;
    cin.ignore();
    cout << "Enter Name of the Book: ";
    getline(cin, name);
    cout << "Enter Author of the Book: ";
    getline(cin, author);
    
    ofstream file("Books.txt", ios::app);
    if (file.is_open())
    {
        Book book(ISBN, name, author);
        book.printToFile(file);
        file.close();
        cout << "Book added successfully!\n";
        cout << "----------------------------------------------\n";
    }
}

void Book::saveToFile() {
    ofstream file("Books.txt", ios::app);
    if (file.is_open()) {
        file << ISBN << "," << name << "," << author << "," << available << "," << issuedTo << endl;
        file.close();
    }
}

void Display_Books() {
    ifstream file("Books.txt"); // Open "Books.txt" in read mode
    if (!file.is_open()) {  // Check if the file opened successfully
        cout << "Error opening file.\n"; // Print error message if file cannot be opened
        return;  // Exit the function
    }

    string line;
    cout << "\nList of Books:\n";

    // Read the file line by line
    while (getline(file, line)) {
        size_t pos1 = line.find(",");          // Find the first comma (separating Book ID and Name)
        size_t pos2 = line.find(",", pos1 + 1); // Find the second comma (separating Name and Author)
        size_t pos3 = line.find(",", pos2 + 1); // Find the third comma (separating Author and Availability)
        size_t pos4 = line.find(",", pos3 + 1); // Find the fourth comma (separating Availability and IssuedTo)

        // Ensure that the line contains enough commas (valid format)
        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
            string bookId = line.substr(0, pos1);                  // Extract Book ID (before first comma)
            string name = line.substr(pos1 + 1, pos2 - pos1 - 1);  // Extract Book Name (between commas)
            string author = line.substr(pos2 + 1, pos3 - pos2 - 1); // Extract Author (between second and third comma)
            string availability = line.substr(pos3 + 1, pos4 - pos3 - 1); // Extract Availability (between third and fourth comma)

            // Display book details in formatted output
            cout << "Book ID: " << bookId << "\n";
            cout << "Book Name: " << name << "\n";
            cout << "Book Author: " << author << "\n";
            cout << "Availability: " << (availability == "1" ? "Available" : "Not Available") << "\n";
            cout << "------------------------\n"; // Print a separator line for better readability
        }
    }

    file.close(); // Close the file after reading all data
}

void DeleteBook() {
    int bookIdToDelete;
    cout << "Enter Book ID to delete: ";
    cin >> bookIdToDelete;

    ifstream file("Books.txt");
    ofstream tempFile("Temp.txt");
    string line;
    bool bookFound = false;
    while (getline(file, line)) {
        stringstream ss(line);
        int id;
        ss >> id;
        if (id == bookIdToDelete) {
            bookFound = true;
            continue;
        }
        tempFile << line << endl;
    }
    file.close();
    tempFile.close();
    if (bookFound) {
        remove("Books.txt");
        rename("Temp.txt", "Books.txt");
        cout << "Book deleted successfully.\n";
        cout << "----------------------------------------------\n";
    } else {
        remove("Temp.txt");
        cout << "Book not found.\n";
        cout << "----------------------------------------------\n";
    }
}


void IssueBook() {
    int bookId;
    string memberName;
    cout << "Enter Book ID to issue: ";
    cin >> bookId;
    cout << "Enter Member Name: ";
    cin.ignore();
    getline(cin, memberName);

    ifstream file("Books.txt");
    ofstream tempFile("Temp.txt");
    string line;
    bool found = false;
    while (getline(file, line)) {
        stringstream ss(line);
        int id;
        string name, author, availability, issuedTo;

        ss >> id;
        ss.ignore();
        getline(ss, name, ',');
        getline(ss, author, ',');
        getline(ss, availability, ',');
        getline(ss, issuedTo, ',');

        bool issued = (availability == "0");

        if (id == bookId && !issued) {
            found = true;
            tempFile << id << "," << name << "," << author << ",0," << memberName << endl;
        } else {
            tempFile << line << endl;
        }
    }
    file.close();
    tempFile.close();
    if (found) {
        remove("Books.txt");
        rename("Temp.txt", "Books.txt");
        cout << "Book issued successfully.\n";
        cout << "----------------------------------------------\n";
    } else {
        remove("Temp.txt");
        cout << "Book not available or already issued.\n";
        cout << "----------------------------------------------\n";
    }
}


void ReturnBook() {
    int bookId;
    cout << "Enter Book ID to return: ";
    cin >> bookId;

    ifstream file("Books.txt");
    ofstream tempFile("Temp.txt");
    string line;
    bool found = false;
    while (getline(file, line)) {
        stringstream ss(line);
        int id;
        string name, author, availability, issuedTo;

        ss >> id;
        ss.ignore();
        getline(ss, name, ',');
        getline(ss, author, ',');
        getline(ss, availability, ',');
        getline(ss, issuedTo, ',');

        bool issued = (availability == "0");

        if (id == bookId && issued) {
            found = true;
            tempFile << id << "," << name << "," << author << ",1," << "None" << endl;
        } else {
            tempFile << line << endl;
        }
    }
    file.close();
    tempFile.close();
    if (found) {
        remove("Books.txt");
        rename("Temp.txt", "Books.txt");
        cout << "Book returned successfully.\n";
    } else {
        remove("Temp.txt");
        cout << "Book not found or not issued.\n";
    }
}


void SearchBook() {
    string searchName;
    cout << "Enter book name to search: ";
    cin.ignore();
    getline(cin, searchName);

    ifstream file("Books.txt");
    if (!file.is_open()) {
        cout << "Error opening file.\n";
        return;
    }

    string line;
    bool found = false;
    while (getline(file, line)) {
        stringstream ss(line);
        int id;
        string name, author, availability, issuedTo;

        ss >> id;
        ss.ignore();
        getline(ss, name, ',');
        getline(ss, author, ',');
        getline(ss, availability, ',');
        getline(ss, issuedTo, ',');

        if (name.find(searchName) != string::npos) {
            cout << "Book ID: " << id << "\n";
            cout << "Book Name: " << name << "\n";
            cout << "Book Author: " << author << "\n";
            cout << "Availability: " << (availability == "1" ? "Available" : "Not Available") << "\n";
            cout << "------------------------\n";
            found = true;
        }
    }
    file.close();
    if (!found) {
        cout << "No books found with the given name.\n";
    }
}

void Member_Operations() {
    int choice;
    while (true) {
        cout << "1. Search for a book" << endl;
        cout << "2. Issue a book" << endl;
        cout << "3. Return a book" << endl;
        cout << "4. List all books" << endl;
        cout << "5. Exit.";
        cin >> choice;
        cout << "----------------------------------------------\n";
        switch (choice) {
        case 1:
            SearchBook();
            break;
        case 2:
            IssueBook();
            break;
        case 3:
            ReturnBook();
            break;
        case 4:
            Display_Books();
            break;
        case 5:
            exit(0);
            break;
        default:
            cout<<"Invalid Input!"<<endl;
        }
    }
}


void Librarian_Operations() {
    int choice;
    while (true)
    {
        cout << "Library Management System" << endl;
        cout << "1. Add a book" << endl;
        cout << "2. Search for a book" << endl;
        cout << "3. Issue a book" << endl;
        cout << "4. Return a book" << endl;
        cout << "5. List all books" << endl;
        cout << "6. Delete a book" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << "----------------------------------------------\n";
        switch (choice)
        {
        case 1:
            AddBooks();
            break;
        case 2:
            SearchBook();
            break;
        case 3:
            IssueBook();
            break;
        case 4:
            ReturnBook();
            break;
        case 5:
            Display_Books();
            break;
        case 6:
            DeleteBook();
            break;
        case 7:
            exit(0);
        default:
            cout << "Invalid choice!" << endl;
        }
        cout << endl;
    }
}

void Admin_Operations(){
     int choice;
    while (true)
    {
        cout << "Library Management System" << endl;
        cout << "1. Add a book" << endl;
        cout << "2. Search for a book" << endl;
        cout << "3. Issue a book" << endl;
        cout << "4. Return a book" << endl;
        cout << "5. List all books" << endl;
        cout << "6. Delete a book" << endl;
        cout << "7. Delete a Member"<<endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cout << "----------------------------------------------\n";
        switch (choice)
        {
        case 1:
            AddBooks();
            break;
        case 2:
            SearchBook();
            break;
        case 3:
            IssueBook();
            break;
        case 4:
            ReturnBook();
            break;
        case 5:
            Display_Books();
            break;
        case 6:
            DeleteBook();
            break;
        case 7:
            A1.delete_Member();
            break;
        case 8:
            exit(0);
        default:
            cout << "Invalid choice!" << endl;
        }
        cout << endl;
    }
}
int main()
{
    cout << "Welcome to Library Management System\n";
    cout << "----------------------------------------------\n";
    cout << "# For Sign Up, Press 1\n";
    cout << "# For Login, Press 2\n";

    int choice;
    cin >> choice;
    cout << "----------------------------------------------\n";
    if (choice == 1)
    {
        cout << "Please Enter Secret Pin: ";
        int pin;
        cin >> pin;
        A1.create_Member(pin);
    }

    // Login Process
    int LoginCount = 3; // Allow 3 attempts
    int id;
    string pass;
    int userRole = 0;
    bool loggedIn = false; // Track successful login

    do
    {
        cout << "Enter Your ID: ";
        cin >> id;
        cout << "Enter Your Password: ";
        pass = A1.GetPassword();

        if (authenticateUser(id, pass, userRole))
        {
            loggedIn = true;
            break; // Exit loop if login is successful
        }
        else
        {
            LoginCount--;
            if (LoginCount > 0)
                cout << "Invalid credentials! Try Again. " << LoginCount << " Chances Left" << endl;
        }

    } while (LoginCount > 0);

    if (!loggedIn)
    {
        cout << "Invalid credentials! Exiting LMS.\n";
        return 0; // Exit program if login fails
    }
    cout << "----------------------------------------------\n";
    // Role-Based Functionality
    switch (userRole)
    {
    case 1:
        cout << "Welcome Admin!\n";
        Admin_Operations();
        break;
    case 2:
        cout << "Welcome Librarian!\n";
        Librarian_Operations();
        break;
    case 3:
        cout << "Welcome Member!\n";
        Member_Operations();
        break;
    default:
        cout << "Invalid role assigned!\n";
    }

    return 0;
}