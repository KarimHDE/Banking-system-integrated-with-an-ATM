#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

//-------------------------------------------------------------------------------------------//
struct sinfo {
    string AccountNumber, Name, Pincode;
    int Balance = 0;
    int permission = 0;
    bool ReadyForDelete = false;
    string UserName, Password;
};

enum UserPermissions
{
    ShowClientListPermission = 1,
    AddNewClientPermission = 2,
    DeleteClientPermission = 4,
    UpdateClientInfoPermission = 8,
    FindClientPermission = 16,
    TransactionsPermission = 32,
    ManageUsersPermission = 64
};
const string FILE_PATH = "d:\\file.txt";
const string USER_FILE_PATH = "d:\\users.txt";

enum MenuOptions { SHOW_CLIENTS = 1, ADD_CLIENT = 2, DELETE_CLIENT = 3, UPDATE_CLIENT = 4, FIND_CLIENT = 5, TRANSACTIONS = 6, Manage_Users = 7, Logout_Option = 8 };
enum TransactionsOptions { _Deposite = 1, _Withdraw = 2, Total_Balances = 3, Main_menu = 4 };
enum Manage_Options { List_Users = 1, Add_New_Users = 2, Delete_User = 3, Update_User = 4, Find_User = 5, Main_Menu = 6 };

// Global variable to store the current logged-in user
sinfo currentUser;

//-------------------------------------------------------------------------------------------//
string ReadString(string message)
{
    string text;
    cout << message;
    getline(cin, text);
    return text;
}

int ReadNum(string message)
{
    int number;
    cout << message;
    while (!(cin >> number)) {
        cout << "Invalid input. Please enter a valid number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return number;
}

sinfo ReadInfo()
{
    sinfo info;
    info.AccountNumber = ReadString("\nEnter Account Number: \n");
    info.Name = ReadString("\nEnter Name: ");
    info.Pincode = ReadString("\nEnter PinCode: ");
    info.Balance = ReadNum("\nEnter Balance: ");
    return info;
}

sinfo ReadUserInfo()
{
    sinfo info;
    info.UserName = ReadString("\nEnter a UserName: \n");
    info.Password = ReadString("\nEnter a Password: ");
    return info;
}

void UpdatedReadInfo(sinfo& info)
{
    info.Name = ReadString("Enter Name: \n");
    info.Pincode = ReadString("Enter PinCode: \n");
    info.Balance = ReadNum("Enter Balance: \n");
}

void UpdatedReadUserInfo(sinfo& info)
{
    info.Password = ReadString("Enter Password: \n");
}

void MainMenu();
//-------------------------------------------------------------------------------------------//
vector<string> SplitSreing(string line, string separator = "__")
{
    int pos = 0;
    string word = "";
    vector <string> record;
    while ((pos = line.find(separator)) != string::npos)
    {
        word = line.substr(0, pos);
        record.push_back(word);
        line.erase(0, pos + separator.length());
    }
    if (!(line.empty()))
    {
        record.push_back(line);
    }
    return record;
}

sinfo  ConvertlineToRecord(string line, string separator = "__")
{
    sinfo info;
    vector <string> vinfo;
    vinfo = SplitSreing(line, separator);
    if (vinfo.size() > 3)
    {
        info.AccountNumber = vinfo[0];
        info.Name = vinfo[1];
        info.Pincode = vinfo[2];
        info.Balance = stoi(vinfo[3]);
    }
    else
    {
        info.UserName = vinfo[0];
        info.Password = vinfo[1];
        info.permission = stoi(vinfo[2]);
    }

    return info;
}

string ConvertRecordToLine(sinfo info, string separator = "__")
{
    string S1 = "";
    S1 += info.AccountNumber + separator;
    S1 += info.Name + separator;
    S1 += info.Pincode + separator;
    S1 += to_string(info.Balance);
    return S1;
}

string ConvertRecordToLineForUsers(sinfo info, string separator = "__")
{
    string S1 = "";
    S1 += info.UserName + separator;
    S1 += info.Password + separator;
    S1 += to_string(info.permission);
    return S1;
}

vector <sinfo> LoadLinesFromFile(string filename = FILE_PATH)
{
    fstream file(filename, ios::in);
    string line;
    vector <sinfo> lines;
    if (!file.is_open())
    {
        cout << "Error Can't Open file" << endl;
        return{};
    }

    while (getline(file, line))
    {
        sinfo info = ConvertlineToRecord(line);
        if (!(info.ReadyForDelete))
            lines.push_back(info);
    }
    file.close();

    return lines;
}
//-------------------------------------------------------------------------------------------//
void PrintRecord(sinfo info)
{
    cout << left << setw(24) << info.AccountNumber;
    cout << left << setw(26) << "|" + info.Name;
    cout << left << setw(19) << "|" + info.Pincode;
    cout << left << setw(9) << "|" + (to_string(info.Balance));
    cout << endl;
}

string GetPermissionString(int permission) {
    string permissionsStr = "";
    if (permission & ShowClientListPermission) permissionsStr += "Show Clients, ";
    if (permission & AddNewClientPermission) permissionsStr += "Add Clients, ";
    if (permission & DeleteClientPermission) permissionsStr += "Delete Clients, ";
    if (permission & UpdateClientInfoPermission) permissionsStr += "Update Clients, ";
    if (permission & FindClientPermission) permissionsStr += "Find Clients, ";
    if (permission & TransactionsPermission) permissionsStr += "Transactions, ";
    if (permission & ManageUsersPermission) permissionsStr += "Manage Users, ";


    if (!permissionsStr.empty() && permissionsStr.back() == ' ') {
        permissionsStr.pop_back();
        permissionsStr.pop_back();
    }

    return permissionsStr.empty() ? "No Permissions" : permissionsStr;
}


void PrintUserRecord(sinfo info) {
    cout << left << setw(16) << info.UserName;
    cout << left << setw(22) << "|" + info.Password << setw(15) << "|";
    cout << left << setw(22) << "|" + GetPermissionString(info.permission);
    cout << endl;
}

void PrintHeader(vector <sinfo>data)
{
    cout << "\n\t\t\t\tClient(s) (" << data.size() << ")\t\n";
    cout << "-------------------------------------------------------------------------------------\n";
    cout << left << setw(10) << "   Account Number \t" << left << setw(20) << "|\tName " << left << setw(15) << "|\tPinCode " << left << setw(15) << "|\tBalance " << endl;
    cout << "-------------------------------------------------------------------------------------\n";
}

void PrintUsersHeader(vector <sinfo> data)
{
    cout << "\n\t\t\t\tClient(s) (" << data.size() << ")\t\n";
    cout << "---------------------------------------------------------------------------\n";
    cout << left << setw(10) << "  UserName \t" << left << setw(16) << "|\tPassword " << left << setw(15) << "|\tPermissions " << endl;
    cout << "---------------------------------------------------------------------------\n";
}

void PrintData(vector <sinfo>data)
{
    for (sinfo& line : data)
    {
        PrintRecord(line);
    }
    cout << "-------------------------------------------------------------------------------------\n";
}
void PrintUserData(vector <sinfo>data)
{
    for (sinfo& line : data)
    {
        PrintUserRecord(line);
    }
    cout << "---------------------------------------------------------------------------\n";
}

void PrintTable(vector<sinfo> clients)
{
    PrintHeader(clients);
    PrintData(clients);
}

void PrintUserInfo(sinfo user)
{
    cout << "--------------------------\n";
    cout << "UserName: " << user.UserName << endl;
    cout << "Password: " << user.Password << endl;
    cout << "Permission: " << (int)user.permission << endl;
    cout << "--------------------------\n";
}
void PrintUsersTable(vector<sinfo> clients)
{
    PrintUsersHeader(clients);
    PrintUserData(clients);
}

void PrintLoginScreen()
{
    cout << "===================================================\n";
    cout << "\t\t   Login Screen" << endl;
    cout << "===================================================\n";
}

void PermissionMessage()
{
    cout << "------------------------\n";
    cout << "Access Denied\n";
    cout << "You Don't Have Permission To Do This\n";
    cout << "For More Details Contact Your Admin\n";
    cout << "------------------------\n";
}

void waitForUser(string message = "\n\nPress any key to go back to the main menu... ")
{
    cout << message << endl;
    cin.ignore();
    cin.get();
    system("cls");
    MainMenu();
}
void ManageUsers();
void LoginScreen();
//-------------------------------------------------------------------------------------------//
bool SearchAndReturnClient(string AccountNumber, vector <sinfo>& clients, sinfo*& client)
{
    for (sinfo& one : clients)
    {
        if (one.AccountNumber == AccountNumber)
        {
            PrintHeader(clients);
            PrintRecord(one);
            client = &one;
            return true;
        }
    }

    std::cout << "The client was not found try another account number." << endl;
    return false;
}

bool SearchForAccount(string AccountNumber, vector <sinfo> clients)
{
    for (sinfo& one : clients)
    {
        if (one.AccountNumber == AccountNumber)
        {
            PrintHeader(clients);
            PrintRecord(one);

            return true;
        }
    }

    return false;
}

bool SearchForUserAccount(string UserName, vector <sinfo> clients)
{
    for (sinfo& one : clients)
    {
        if (one.UserName == UserName)
        {
            PrintUserInfo(one);

            return true;
        }
    }

    return false;
}

bool SearchForUser(string UserName, string Password)
{
    vector<sinfo> users = LoadLinesFromFile("d:\\users.txt");

    for (sinfo& one : users)
    {
        if (UserName == one.UserName && Password == one.Password)
        {
            currentUser = one;
            return true;
        }

        else if (UserName == one.UserName)
            cout << "Incorrect Password!" << endl;
        else if (Password == one.Password)
            cout << "Incorrect Username!" << endl;
        else
            cout << "\t   Invalid UserName / Password! ";
    }
    return false;
}
//-------------------------------------------------------------------------------------------//
bool MarkToDelete(string AccountNum, vector<sinfo>& clients)
{
    for (sinfo& b : clients)
    {
        if (b.AccountNumber == AccountNum)
        {
            b.ReadyForDelete = true;

            return true;
        }
    }
    return false;
}

bool MarkToDeleteUser(string UserName, vector<sinfo>& Users)
{
    for (sinfo& b : Users)
    {
        if (b.UserName == UserName)
        {
            b.ReadyForDelete = true;

            return true;
        }
    }
    return false;
}

void SaveDataToFile(vector<sinfo>& clients, string FileName = FILE_PATH)
{
    string line;
    fstream file(FileName, ios::out);
    if (file.is_open())
    {
        for (sinfo& A : clients)
        {
            if (A.ReadyForDelete == false)
            {
                line = ConvertRecordToLine(A);
                file << line << endl;
            }
        }
        file.close();
    }
}
void SaveDataToFileForUsr(vector<sinfo>& clients, string FileName = USER_FILE_PATH)
{
    string line;
    fstream file(FileName, ios::out);
    if (file.is_open())
    {
        for (sinfo& A : clients)
        {
            if (A.ReadyForDelete == false)
            {
                line = ConvertRecordToLineForUsers(A);
                file << line << endl;
            }
        }
        file.close();
    }
}
void Transactions();

void waitForUserToTransactions() {
    cout << "\n\nPress any key to go back to the main menu... ";
    cin.ignore();
    cin.get();
    system("cls");
    Transactions();
}

int CalculateTotalBalances(vector<sinfo>clients)
{
    int Total = 0;
    for (sinfo& client : clients)
    {
        Total += client.Balance;
    }
    return  Total;
}

void Waiting()
{
    cout << "\nPress any key to go back Manage Users Menu Screen" << endl;
    cin.ignore();
    cin.get();
    ManageUsers();
}
//-------------------------------------------------------------------------------------------//
void Deposite()
{
    system("cls");
    cout << "-----------------------------------\n\tDeposite Screen\t\n-----------------------------------\n";
    vector<sinfo>clients = LoadLinesFromFile();
    sinfo* client = nullptr;
    string AccountNumber = ReadString("Enter Account Number: \n");

    if (SearchAndReturnClient(AccountNumber, clients, client))
    {
        int DepositeAmount = (int)ReadNum("\nEnter the amount of the Deposite: \n");
        client->Balance += DepositeAmount;
        SaveDataToFile(clients);
        clients = LoadLinesFromFile();
        cout << "\nDone Succesfully.  " << endl;
        waitForUserToTransactions();
    }
}

void Withdraw()
{
    system("cls");
    cout << "-----------------------------------\n\tWithdraw Screen\t\n-----------------------------------\n";
    vector<sinfo>clients = LoadLinesFromFile();
    sinfo* client = nullptr;
    string AccountNumber = ReadString("Enter Account Number: \n");

    if (SearchAndReturnClient(AccountNumber, clients, client))
    {
        int WithdrawAmount;
        do {
            WithdrawAmount = (int)ReadNum("\nEnter the amount of the Withdraw: \n");
            if (WithdrawAmount > client->Balance)
            {
                cout << "\nYou can only withdraw up to a maximum of " << client->Balance << endl;

            }
        } while (WithdrawAmount > client->Balance);

        client->Balance -= WithdrawAmount;
        SaveDataToFile(clients);
        clients = LoadLinesFromFile();
        cout << "\nDone Succesfully.  " << endl;
        waitForUserToTransactions();
    }
}

void TotalBalance()
{
    vector<sinfo>clients = LoadLinesFromFile();
    PrintTable(clients);
    cout << "\n\t\t\t\t-------------------------\n  \t\t\t\t  Total Balances: " << CalculateTotalBalances(clients);
    cout << "\n\t\t\t\t-------------------------" << endl;
    waitForUserToTransactions();
}
//-------------------------------------------------------------------------------------------//
void ListMainMenu()
{
    cout << "============================================================\n";
    cout << "\t\t\tMain menu screen\t\n";
    cout << "============================================================\n";
    cout << left << setw(40) << "   [1] Show client list." << endl;
    cout << left << setw(40) << "   [2] Add new client." << endl;
    cout << left << setw(40) << "   [3] Delete client." << endl;
    cout << left << setw(40) << "   [4] Update client info." << endl;
    cout << left << setw(40) << "   [5] Find client." << endl;
    cout << left << setw(40) << "   [6] Transactions." << endl;
    cout << left << setw(40) << "   [7] Manage Users." << endl;
    cout << left << setw(40) << "   [8] Logout." << endl;
    cout << "============================================================\n";
}

void ManageUsersMenuScreen()
{
    system("cls");
    cout << "=========================================\n";
    cout << "\tManage Users Menu Screen\n";
    cout << "=========================================\n";
    cout << "[1] List Users" << endl;
    cout << "[2] Add New Users" << endl;
    cout << "[3] Delete User" << endl;
    cout << "[4] Update User" << endl;
    cout << "[5] Find User" << endl;
    cout << "[6] Main Menu" << endl;
    cout << "=========================================\n";
}

void ListUsers()
{
    system("cls");
    vector <sinfo> info = LoadLinesFromFile("d:\\users.txt");
    PrintUsersTable(info);
    Waiting();
}

void TransactionsMenu()
{
    system("cls");
    cout << "============================================================\n";
    cout << "\t\t\tTransactions menu screen\t\n";
    cout << "============================================================\n";
    cout << left << setw(40) << " [1] Deposite." << endl;
    cout << left << setw(40) << " [2] Withdraw." << endl;
    cout << left << setw(40) << " [3] Total Balances." << endl;
    cout << left << setw(40) << " [4] Main Menu." << endl;
    cout << "============================================================\n";
}

void ShowClientlist()
{
    system("cls");
    if ((currentUser.permission & ShowClientListPermission) == ShowClientListPermission)
    {
        vector <sinfo> info = LoadLinesFromFile("d:\\file.txt");
        PrintTable(info);
        waitForUser();
    }
    else
    {
        PermissionMessage();
        waitForUser();
    }
}

void AddNewClient()
{
    system("cls");
    if (currentUser.permission & AddNewClientPermission)
    {
        vector<sinfo> clients = LoadLinesFromFile(FILE_PATH);
        fstream file("d:\\file.txt", ios::app);
        sinfo info = ReadInfo();
        if (SearchForAccount(info.AccountNumber, clients))
        {
            cout << "\nAlready exist Try Another Account Number \n";
            waitForUser();
        }

        string line = ConvertRecordToLine(info);
        if (!file.is_open()) {
            cout << "Error: Could not open the file!" << endl;
            waitForUser();
        }
        if (file.is_open())
        {
            file << line << endl;
            file.close();
        }
        cout << "\nAdded Succesfully. \n" << endl;
        waitForUser();
    }
    else
    {
        PermissionMessage();
        waitForUser();
    }
}

void DeleteClient()
{
    system("cls");
    if (currentUser.permission & DeleteClientPermission)
    {
        char choice = tolower('N');
        vector<sinfo> clients = LoadLinesFromFile();
        PrintTable(clients);
        string AccountNumber = ReadString("Enter The Account Number: \n");

        if (SearchForAccount(AccountNumber, clients))
        {
            cout << "\nAre you sure you want to delete Y/N ? \n\n\n";
            cin >> choice;
            if (choice == tolower('Y'))
            {
                system("cls");
                MarkToDelete(AccountNumber, clients);
                SaveDataToFile(clients);
                clients = LoadLinesFromFile();
                cout << "\n*** Deleted succesfully.*** \n";
                PrintTable(clients);
                waitForUser();
            }
        }
        else
        {
            cout << "Account Number is not exist. " << endl;
            waitForUser();
        }
    }
    else
    {
        PermissionMessage();
        waitForUser();
    }
}

void UpdateClientInfo()
{
    system("cls");
    if (currentUser.permission & UpdateClientInfoPermission)
    {
        char choice = tolower('N');
        vector<sinfo> clients = LoadLinesFromFile();
        PrintTable(clients);
        string AccountNumber = ReadString("Enter the Account number you want to update: \n");

        for (sinfo& client : clients)
        {
            if (client.AccountNumber == AccountNumber)
            {
                cout << "Are you sure you want to update it [Y/N]? \n\n";
                cin >> choice;
                cin.ignore();
                if (tolower(choice) == 'y')
                {
                    UpdatedReadInfo(client);
                    SaveDataToFile(clients);
                    cout << "Updated successfully!\n";
                    PrintRecord(client);
                    waitForUserToTransactions();
                }
            }
        }

        waitForUser();
    }
    else
    {
        PermissionMessage();
        waitForUser();
    }
}

void FindClient()
{
    system("cls");
    if (currentUser.permission & FindClientPermission)
    {
        vector<sinfo> clients = LoadLinesFromFile();
        string AccountNumber = ReadString("\nEnter The Account Number: \n");

        SearchForAccount(AccountNumber, clients);

        waitForUser();
    }
    else
    {
        PermissionMessage();
        waitForUser();
    }
}

void Transactions()
{
    if (currentUser.permission & TransactionsPermission)
    {
        TransactionsMenu();
        short choice = (short)ReadNum("Choose what do you want to do [ 1 -> 4 ] ? -->  ");
        switch (choice)
        {
        case _Deposite:       Deposite();    break;
        case _Withdraw:       Withdraw();    break;
        case Total_Balances:  TotalBalance();  break;
        case Main_menu:       MainMenu();    break;
        default: cout << "Enter correct number: \n"; break;
        }
    }
    else
    {
        system("cls");
        PermissionMessage();
        waitForUser();
    }
}
//-------------------------------------------------------------------------------------------//
void AddNewUsers()
{
    system("cls");

    vector<sinfo> users = LoadLinesFromFile("d:\\users.txt");
    fstream file("d:\\users.txt", ios::app);
    sinfo info = ReadUserInfo();
    if (SearchForUserAccount(info.UserName, users))
    {
        cout << "\nAlready exist Try Another Account Number \n";
        return;
    }

    string line = ConvertRecordToLineForUsers(info);
    if (!file.is_open()) {
        cout << "Error: Could not open the file!" << endl;
        return;
    }
    if (file.is_open())
    {
        file << line << endl;
        file.close();
    }
    cout << "\nAdded Succesfully. \n" << endl;
    Waiting();
}
void DeleteUser()
{
    system("cls");
    char choice = tolower('N');
    vector<sinfo> Users = LoadLinesFromFile("d:\\users.txt");
    PrintUsersTable(Users);
    string UserName = ReadString("Enter The UserName: \n");

    if (SearchForUserAccount(UserName, Users))
    {
        cout << "\nAre you sure you want to delete Y/N ? \n\n\n";
        cin >> choice;
        if (choice == tolower('Y'))
        {
            system("cls");
            MarkToDeleteUser(UserName, Users);
            SaveDataToFile(Users, "d:\\users.txt");
            Users = LoadLinesFromFile("d:\\users.txt");
            cout << "\n*** Deleted succesfully.*** \n";
            PrintUserData(Users);
            Waiting();
        }
    }
    cout << "Account Number is not exist. " << endl;
}
void UpdateUser()
{
    system("cls");
    char choice = tolower('N');
    vector<sinfo> Users = LoadLinesFromFile("d:\\users.txt");
    PrintUsersTable(Users);
    string UserName = ReadString("Enter the UserName you want to update: \n");

    for (sinfo& user : Users)
    {
        if (user.UserName == UserName)
        {
            cout << endl;
            PrintUserRecord(user);
            cout << "\nAre you sure you want to update it [Y/N]? \n\n";
            cin >> choice;
            cin.ignore();
            if (tolower(choice) == 'y')
            {
                UpdatedReadUserInfo(user);
                SaveDataToFile(Users, "d:\\users.txt");
                cout << "\nUpdated successfully!\n\n";
                PrintUserRecord(user);
                Waiting();
            }
        }
    }
    Waiting();
}
void FindUser()
{
    system("cls");
    vector<sinfo> users = LoadLinesFromFile("d:\\users.txt");
    string UserName = ReadString("Enter UserName: ");
    SearchForUserAccount(UserName, users);
    Waiting();
}
void ManageUsers()
{
    if (currentUser.permission & ManageUsersPermission)
    {
        ManageUsersMenuScreen();
        short choice = (short)ReadNum("Choose what do you want to do [ 1 -> 6 ] ? -->  ");
        switch (choice)
        {
        case List_Users:    ListUsers();   break;
        case Add_New_Users: AddNewUsers(); break;
        case Delete_User:   DeleteUser();   break;
        case Update_User:   UpdateUser();   break;
        case Find_User:     FindUser();     break;
        case Main_Menu:     MainMenu();     break;
        default: cout << "Error\n"; break;
        }
    }
    else
    {
        system("cls");
        PermissionMessage();
        waitForUser();
    }
}
void Logout()
{
    system("cls");
    LoginScreen();
}
//-------------------------------------------------------------------------------------------//
void MainMenu()
{
    system("cls");
    ListMainMenu();
    short choice = (short)ReadNum("Choose what do you want to do [ 1 -> 8 ] ? -->  ");

    switch (choice)
    {
    case SHOW_CLIENTS:  ShowClientlist();   break;
    case ADD_CLIENT:    AddNewClient();     break;
    case DELETE_CLIENT: DeleteClient();     break;
    case UPDATE_CLIENT: UpdateClientInfo(); break;
    case FIND_CLIENT:   FindClient();       break;
    case TRANSACTIONS:  Transactions();     break;
    case Manage_Users:  ManageUsers();      break;
    case Logout_Option: Logout();           break;
    default: cout << "Enter correct number: \n"; break;
    }
}
//-------------------------------------------------------------------------------------------//

void LoginScreen()
{
    PrintLoginScreen();
    string UserName = ReadString("    UserName: ");
    string Passwprd = ReadString("    Password: ");
    if (SearchForUser(UserName, Passwprd))
    {
        MainMenu();
    }
    else
    {
        system("pause");
        system("cls");
        LoginScreen();
    }
}

int main()
{
    LoginScreen();
    return 0;
}