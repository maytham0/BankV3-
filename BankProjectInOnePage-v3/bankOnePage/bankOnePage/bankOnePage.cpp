#include<iostream>
#include<vector>
#include<string>
#include<cctype>
#include<iomanip>
#include <fstream>
#include<cmath>
using namespace std;

    string* ptrUser_Logined;
    const string ClientsFileName = "Client_data.txt";
    const string UserFileName = "User.txt";
    struct stUser

    {
        string Username;
        int Password;
        short Permissions;
        bool MarkForDelete = false;
    };

    struct sClient {
        string AccountNumber;
        string PinCode;
        string Name;
        string Phone;
        double AccountBalance;
        bool MarkForDelete = false;
    };

    vector<string> SplitString(string S1, string Delim) {
        vector<string> vString;
        short pos = 0;
        string sWord;
        // define a string variable
        // use find() function to get the position of the delimiters
        while ((pos = S1.find(Delim)) != std::string::npos) {
            sWord = S1.substr(0, pos); // store the word
            if (sWord != "") {
                vString.push_back(sWord);
            }
            S1.erase(0, pos + Delim.length());
        }
        if (S1 != "") {
            vString.push_back(S1); // it adds last word of the string.
        }
        return vString;
    }
    sClient ConvertLinetoRecord(string Line, string Seperator = "#//#") {
        sClient Client;
        vector<string> vClientData;
        vClientData = SplitString(Line, Seperator);
        Client.AccountNumber = vClientData[0];
        Client.PinCode = vClientData[1];
        Client.Name = vClientData[2];
        Client.Phone = vClientData[3];
        Client.AccountBalance = stod(vClientData[4]); // cast string to double
        return Client;
    }
    string ConvertRecordToLine(sClient Client, string Seperator = "#//#") {
        string stClientRecord = "";
        stClientRecord += Client.AccountNumber + Seperator;
        stClientRecord += Client.PinCode + Seperator;
        stClientRecord += Client.Name + Seperator;
        stClientRecord += Client.Phone + Seperator;
        stClientRecord += to_string(Client.AccountBalance);
        return stClientRecord;
    }
    vector<sClient> LoadCleintsDataFromFile(string FileName) {
        vector<sClient> vClients;
        fstream MyFile;
        MyFile.open(FileName, ios::in); // read Mode
        if (MyFile.is_open()) {
            string Line;
            sClient Client;

            while (getline(MyFile, Line)) {
                Client = ConvertLinetoRecord(Line);
                vClients.push_back(Client);
            }
            MyFile.close();
        }
        return vClients;
    }


    bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClients) {
        for (sClient& C : vClients) {
            if (C.AccountNumber == AccountNumber) {
                C.MarkForDelete = true;
                return true;
            }
        }
        return false;
    }
    bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client) {
        for (sClient C : vClients) {
            if (C.AccountNumber == AccountNumber) {
                Client = C;
                return true;
            }
        }
        return false;
    }

    bool FindClientByAccountNumber2(string AccountNumber, sClient& Client) {
        vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
        for (sClient C : vClients) {
            if (C.AccountNumber == AccountNumber) {
                Client = C;
                return true;
            }
        }
        return false;
    }
    vector<sClient> SaveCleintsDataToFileToDelet(string FileName, vector<sClient> vClients) {
        fstream MyFile;
        MyFile.open(FileName, ios::out); // overwrite
        string DataLine;
        if (MyFile.is_open()) {
            for (sClient C : vClients) {
                if (C.MarkForDelete == false) { // we only write records that are not marked for delete.
                    DataLine = ConvertRecordToLine(C);
                    MyFile << DataLine << endl;
                }
            }
            MyFile.close();
        }
        return vClients;
    }
    string ReadClientAccountNumber() {
        string AccountNumber = "";
        cout << "\nPlease enter Account Number? ";
        cin >> AccountNumber;
        return AccountNumber;
    }

    void PrintClientCard(sClient Client) {
        cout << "\nThe following are the client details:\n";
        cout << "\n----------------------------------------------\n";
        cout << "\nAccout Number: " << Client.AccountNumber;
        cout << "\nPin Code     : " << Client.PinCode;
        cout << "\nName         : " << Client.Name;
        cout << "\nPhone        : " << Client.Phone;
        cout << "\nAccount Balance: " << Client.AccountBalance;
        cout << "\n----------------------------------------------\n";
    }
    bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {
        sClient Client;
        char Answer = 'n';
        if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
            PrintClientCard(Client);
            cout << "\n\nAre you sure you want delete this client? y/n ? ";
            cin >> Answer;
            if (Answer == 'y' || Answer == 'Y') {
                MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
                SaveCleintsDataToFileToDelet(ClientsFileName, vClients);
                // Refresh Clients
                vClients = LoadCleintsDataFromFile(ClientsFileName);
                cout << "\n\nClient Deleted Successfully.";
                return true;
            }
        }
        else {
            cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
            return false;
        }
        return false;
    }
    void SaveVectorToFileToUpdate(string FileName, vector <sClient>& vVectorContect)
    {
        fstream MyFile0;
        MyFile0.open(FileName, ios::out);
        string DataLine;
        if (MyFile0.is_open())
        {

            for (sClient Line : vVectorContect)
            {
                DataLine = ConvertRecordToLine(Line);
                MyFile0 << DataLine << endl;
            }
            MyFile0.close();
        }
    }

    void SaveVectorToFileToAdd(string FileName, vector <sClient> vVectorContect)
    {
        fstream MyFile0;
        MyFile0.open(FileName, ios::out | ios::app);

        if (MyFile0.is_open())
        {

            for (sClient Line : vVectorContect)
            {
                MyFile0 << ConvertRecordToLine(Line) << endl;
            }
            MyFile0.close();
        }
    }
    vector<sClient> GetClientInfo()
    {
        sClient D1;
        vector<sClient>vClient;
        cout << "Enter Acount Number :"; getline(cin >> ws, D1.AccountNumber);
        cout << "\nEnter Pin Code :"; getline(cin, D1.PinCode);
        cout << "\nEnter Name :"; getline(cin, D1.Name);
        cout << "\nEnter Phone Number :"; getline(cin, D1.Phone);
        cout << "\nEnter Acount Balanc :"; cin >> D1.AccountBalance;
        cout << endl;

        vClient.push_back(D1);

        return vClient;
    }

    sClient ChangeClientRecord(string AccountNumber) {
        sClient Client;
        Client.AccountNumber = AccountNumber;
        cout << "\n\nEnter PinCode? ";
        getline(cin >> ws, Client.PinCode);
        cout << "Enter Name? ";
        getline(cin, Client.Name);
        cout << "Enter Phone? ";
        getline(cin, Client.Phone);
        cout << "Enter AccountBalance? ";
        cin >> Client.AccountBalance;
        return Client;
    }

    bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {
        sClient Client;
        char Answer = 'n';
        if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
            PrintClientCard(Client);
            cout << "\n\nAre you sure you want update this client? y/n? ";
            cin >> Answer;
            if (Answer == 'y' || Answer == 'Y') {
                for (sClient& C : vClients) {
                    if (C.AccountNumber == AccountNumber) {
                        C = ChangeClientRecord(AccountNumber);
                        break;
                    }
                }
                SaveVectorToFileToUpdate(ClientsFileName, vClients);
                cout << "\n\nClient Updated Successfully.";
                return true;
            }
        }
        else {
            cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
            return false;
        }
        return false;
    }

    void PrintClientRecord(sClient Client) {
        cout << "| " << setw(15) << left << Client.AccountNumber;
        cout << "| " << setw(10) << left << Client.PinCode;
        cout << "| " << setw(40) << left << Client.Name;
        cout << "| " << setw(12) << left << Client.Phone;
        cout << "| " << setw(12) << left << Client.AccountBalance;
    }
    void PrintAllClientsData(vector<sClient> vClients) {
        cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
        cout << "| " << left << setw(15) << "Accout Number";
        cout << "| " << left << setw(10) << "Pin Code";
        cout << "| " << left << setw(40) << "Client Name";
        cout << "| " << left << setw(12) << "Phone";
        cout << "| " << left << setw(12) << "Balance";
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
        for (sClient& Client : vClients) {
            PrintClientRecord(Client);
            cout << endl;
        }
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
    }

    void FindClientByAccountNumberToPrintHim(string fileName)
    {
        vector <sClient>vClient;
        vClient = LoadCleintsDataFromFile(ClientsFileName);
        sClient Client;
        string AccountNumber = ReadClientAccountNumber();
        if (FindClientByAccountNumber2(AccountNumber, Client))
        {
            PrintClientCard(Client);
        }
        else cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }
    int  Validate_Number()
    {
        short Number;
        cin >> Number;

        while (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid Number, Enter a valid one:" << endl;
            cin >> Number;
        }
        return Number;
    }
    int Deposit(string FileName)
    {
        char Answer = 'y';
        bool clientFound = false;
        int DepostNumber;
        vector<sClient> vClient = LoadCleintsDataFromFile(FileName);
        sClient Client;
        string AccountNumber = ReadClientAccountNumber();
        for (sClient& Line : vClient) {
            if (AccountNumber == Line.AccountNumber)
            {
                PrintClientCard(Line);
                clientFound = true;
                cout << "\nPleas Enter Deposit amount? ";
                cin >> DepostNumber;
                cout << "\nAre you sure wont Deposit?(y,n)\n";
                cin >> Answer;
                if (tolower(Answer) == 'y') {

                    Line.AccountBalance = Line.AccountBalance + DepostNumber;
                    SaveVectorToFileToUpdate(FileName, vClient);
                    return Line.AccountBalance;

                }

            }


        }
        if (!clientFound) { cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!"; }
        return -1;
    }
    int WithDrow(string FileName)

    {
        bool clientFund = false;
        char Answer = 'y';
        double WithdrowNumber = 0;
        vector<sClient> vClient = LoadCleintsDataFromFile(FileName);
        sClient Client;
        string AccountNumber = ReadClientAccountNumber();
        for (sClient& Line : vClient) {
            if (AccountNumber == Line.AccountNumber)
            {
                PrintClientCard(Line);
                clientFund = true;


                cout << "\nPleas Enter Withdrow amount? ";
                cin >> WithdrowNumber;

                if (WithdrowNumber > Line.AccountBalance) {
                    cout << "Amount Exceeds the balanc,you can withdrow UP to :" << Line.AccountBalance << endl;
                    cout << "Pleas Enter another amount ?";
                    cin >> WithdrowNumber;
                }
                cout << "\nAre you sure wont Withdrow?(y,n)\n";
                cin >> Answer;
                if (tolower(Answer) == 'y') {

                    Line.AccountBalance = Line.AccountBalance - WithdrowNumber;
                    SaveVectorToFileToUpdate(FileName, vClient);
                    return Line.AccountBalance;

                }

            }

        }
        if (!clientFund) { cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!"; }
        return -1;

    }


    void TotalBalanc(string FileName)
    {

        vector<sClient> vClient = LoadCleintsDataFromFile(FileName);
        sClient Client;

        double TotleBalance = 0;
        for (sClient& Line : vClient) {
            cout << "|" << setw(22) << left << Line.AccountNumber << "|" << setw(32) << left << Line.Name << "|" << Line.AccountBalance << endl;
            TotleBalance += Line.AccountBalance;


        }
        cout << "--------------------------------------------------------------\n\n";
        cout << "The Totle Balance For All client(s). <<" << TotleBalance << ">>\n";


    }
    int HowManyClientHave(string FileName)
    {
        vector<sClient>vClient = LoadCleintsDataFromFile(FileName);
        int Count = 0;
        for (sClient& Line : vClient)
        {
            if (!Line.AccountNumber.empty())
            {
                Count++;
            }
        }
        return Count;
    }


    //to Users Foncion

   
    stUser ConvertLinetoRecord_stUser(string Line, string Seperator = "#//#") {
        stUser User;
        vector<string> vUserData;
        vUserData = SplitString(Line, Seperator);
        User.Username = vUserData[0];
        User.Password = stoi(vUserData[1]);
        User.Permissions = stoi(vUserData[2]);
        return User;
    }
    vector<stUser> LoadUsersDataFromFile_stUser(string FileName) {
        vector<stUser>vUser;
        fstream MyFile;
        MyFile.open(FileName, ios::in); // read Mode
        if (MyFile.is_open()) {
            string Line;
            stUser User;

            while (getline(MyFile, Line)) {
                User = ConvertLinetoRecord_stUser(Line);
                vUser.push_back(User);
            }
            MyFile.close();
        }
        return vUser;
    }
    void PrintUserRecord(stUser User) {
        cout << "| " << setw(15) << left << User.Username;
        cout << "| " << setw(15) << left << User.Password;
        cout << "| " << setw(15) << left << User.Permissions;

    }
    void PrintAllUsersData(vector<stUser> vUser) {
        cout << "\n\t\t\t\t\tUsers List (" << vUser.size() << ") User(s).";
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
        cout << "| " << left << setw(15) << "User Name";
        cout << "| " << left << setw(15) << "Password";
        cout << "| " << left << setw(15) << "Permissions";
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
        for (stUser& User : vUser) {
            PrintUserRecord(User);
            cout << endl;
        }
        cout << "\n_______________________________________________________";
        cout << "_________________________________________\n" << endl;
    }

    bool FindUserByUserName(string UserName) {
        vector<stUser> vUser = LoadUsersDataFromFile_stUser(UserFileName);
        for (stUser C : vUser) {
            if (C.Username == UserName) {

                return true;
            }
        }
        return false;
    }
    string RaedUser_UserName() {
        string UserName = "";
        cout << "\nPlease enter User Name? ";
        getline(cin >> ws, UserName);
        
        return UserName;
    }
    string ConvertRecordToLine_User(stUser User, string Seperator = "#//#") {
        string stUserRecord = "";
        stUserRecord += User.Username + Seperator;
        stUserRecord += to_string(User.Password) + Seperator;
        stUserRecord += to_string(User.Permissions);

        return stUserRecord;
    }
    void SaveVectorToFileToAdd_User(string FileName, vector <stUser> vVectorContect)
    {
        fstream MyFile;
        MyFile.open(FileName, ios::out | ios::app);

        if (MyFile.is_open())
        {

            for (stUser& Line : vVectorContect)
            {
                MyFile << ConvertRecordToLine_User(Line) << endl;
            }
            MyFile.close();
        }
    }
    vector<stUser> vfGetUserInfo()
    {
        stUser D1;
        vector<stUser>vUser;
        cout << "Enter User Name:"; getline(cin >> ws, D1.Username);
        cout << "\nEnter PassWord :"; cin, D1.Password;
        cout << "\nEnter Permissions :"; cin, D1.Permissions;
        cout << endl;

        vUser.push_back(D1);

        return vUser;
    }
    //Permission Funicion
    ///
    ////
    ////
    /////
    //////
    ////////
    short ChossePermissionYorN(short shift) {
        char Answer;
        cin >> Answer;
        return tolower(Answer) == 'y' ? (1 << shift) : 0;
    }

    short AddPermission() {
        short NumberOfPermissionInBinary = 0b00000000;
        cout << "Do you want to let him Show Client List? (y/n)\n";
        NumberOfPermissionInBinary |= ChossePermissionYorN(0);
        cout << "Do you want to let him Add Client? (y/n)\n";
        NumberOfPermissionInBinary |= ChossePermissionYorN(1);
        cout << "Do you want to let him Delete client? (y/n)\n";
        NumberOfPermissionInBinary |= ChossePermissionYorN(2);
        cout << "Do you want to let him Update client? (y/n)\n";
        NumberOfPermissionInBinary |= ChossePermissionYorN(3);
        cout << "Do you want to let him Find Client? (y/n)\n";
        NumberOfPermissionInBinary |= ChossePermissionYorN(4);
        cout << "Do you want to let him open transaction? (y/n)\n";
        NumberOfPermissionInBinary |= ChossePermissionYorN(5);
        cout << "Do you want to let him go to User management? (y/n)\n";
        NumberOfPermissionInBinary |= ChossePermissionYorN(6);

        return NumberOfPermissionInBinary;
    }
    bool CheckPermission(short permissions, short bitPosition)
    {
        //permissions &= 0b01000000;
        return (( permissions & (1 << bitPosition)) != 0) || permissions == -1;
    }
    
    stUser TakeUserInfoFromLogin(string FileName = UserFileName)
    {
        vector<stUser>vUsers = LoadUsersDataFromFile_stUser(FileName);
        stUser D1;
        for (stUser& user : vUsers)
        {
            if (user.Username == *ptrUser_Logined)
            {
                D1.Username = user.Username;
                D1.Password = user.Password;
                D1.Permissions = user.Permissions;
            }
        }
        return D1;
    }

    void PrintUserCard(stUser User) {
        cout << "\nThe following are the User details:\n";
        cout << "\n----------------------------------------------\n";
        cout << "\nUser Name   : " << User.Username;
        cout << "\nPassword    : " << User.Password;
        cout << "\nPermissions : " << User.Permissions;
        cout << "\n----------------------------------------------\n";
    }

    bool MarkUserForDeleteByUserName(string UserName, vector<stUser>& vUser) {
        for (stUser& C : vUser) {
            if (C.Username == UserName) {
                C.MarkForDelete = true;
                return true;
            }
        }
        return false;
    }
    vector<stUser> SaveUserDataToFileToDelet(string FileName, vector<stUser>& vUser) {
        fstream MyFile0;
        MyFile0.open(FileName, ios::out); // overwrite
        string DataLine;
        if (MyFile0.is_open()) {
            for (stUser C : vUser) {
                if (C.MarkForDelete == false) { // we only write records that are not marked for delete.
                    DataLine = ConvertRecordToLine_User(C);//
                    MyFile0 << DataLine << endl;
                }
            }
            MyFile0.close();
        }
        return vUser;
    }
    bool DeleteUserByUserName(string UserName) {
        vector<stUser> vUser;
        vUser = LoadUsersDataFromFile_stUser(UserFileName);
        stUser User;
        char Answer = 'n';
        //
        if (FindUserByUserName(UserName)) {
            //
            for (stUser& User : vUser)
            {
                if (User.Username == UserName)
                    PrintUserCard(User);
            }
            cout << "\n\nAre you sure you want delete this User? y/n ? ";
            cin >> Answer;
            if (Answer == 'y' || Answer == 'Y') {
                //!
                MarkUserForDeleteByUserName(UserName, vUser);
                //
                SaveUserDataToFileToDelet(UserFileName, vUser);

                // Refresh Clients

                cout << "\n\nUser Deleted Successfully.";
                return true;
            }
        }
        else {
            cout << "\nUser with User Name (" << UserName << ") is Not Found!";
            return false;
        }
        return false;
    }

    stUser ChangeUserRecord(string UserName) {
        stUser User;
        User.Username = UserName;
        cout << "\n\nPassword? ";
        cin >> User.Password;
        char Answer = 'n';
        cout << "\nDo You Want give him all permission?(Y/N)";
        cin >> Answer;
        if (tolower(Answer) == 'y')
        {
            User.Permissions = -1;
        }
        else
        {
            User.Permissions = AddPermission();
        }
        return User;
    }
    void SaveVectorToFileToUpdate_User(string FileName, vector <stUser>& vVectorContect)
    {
        fstream MyFile0;
        MyFile0.open(FileName, ios::out);
        string DataLine;
        if (MyFile0.is_open())
        {

            for (stUser Line : vVectorContect)
            {
                DataLine = ConvertRecordToLine_User(Line);
                MyFile0 << DataLine << endl;
            }
            MyFile0.close();
        }
    }
    bool UpdataUserByUserName(string UserName) {
        vector<stUser> vUser = LoadUsersDataFromFile_stUser(UserFileName);
        stUser User;
        char Answer = 'n';
        if (FindUserByUserName(UserName)) {
            for (stUser& User : vUser)
            {
                if (User.Username == UserName)
                    PrintUserCard(User);
            }
            cout << "\n\nAre you sure you want update this client? y/n? ";
            cin >> Answer;
            if (Answer == 'y' || Answer == 'Y') {
                for (stUser& C : vUser) {
                    if (C.Username == UserName) {
                        C = ChangeUserRecord(UserName);
                        break;
                    }
                }
                SaveVectorToFileToUpdate_User(UserFileName, vUser);
                cout << "\n\nClient Updated Successfully.";
                return true;
            }
        }
        else {
            cout << "\nClient with Account Number (" << UserName << ") is Not Found!";
            return false;
        }
        return false;
    }


    void FindUserByUserNameToShowing(string fileName)
    {
        vector<stUser> vUser = LoadUsersDataFromFile_stUser(UserFileName);
        stUser User;
        string UserName = RaedUser_UserName();
        if (FindUserByUserName(UserName))
        {

            for (stUser& User : vUser)
            {
                if (User.Username == UserName)
                    PrintUserCard(User);
            }
        }
        else cout << "\nClient with Account Number (" << UserName << ") is Not Found!";
    }

    ///
    ////
    /////
    ///////
    /////////



       
        enum enUsersTool { UserList = 1, AddUser = 2, DeletUser = 3, UpdateUser = 4, FindUser = 5, MainMenue = 6 };
        void MainScreenForUser()
        {
            cout << "==========================================\n";
            cout << setw(32) << right << "User Managment\n";
            cout << "==========================================\n";
            cout << setw(16) << right << "[1]" << " Show User List.\n";
            cout << setw(16) << right << "[2]" << " Add New User.\n";
            cout << setw(16) << right << "[3]" << " Delete User.\n";
            cout << setw(16) << right << "[4]" << " Update User.\n";
            cout << setw(16) << right << "[5]" << " Find User.\n";
            cout << setw(16) << right << "[6]" << " MainMenue.\n";
            cout << "==========================================\n";

            cout << "Choose what do you want to do ? [1 to 6]?";

        }
        void MainScreenToManagmentUsers();
        void GoBackScreen()
        {
            system("pause");
            system("cls");

            MainScreenToManagmentUsers();
        }
        void ScreenUserList()
        {
            vector<stUser> vUserDataFromFile_AsVector = LoadUsersDataFromFile_stUser(UserFileName);

            system("cls");
            PrintAllUsersData(vUserDataFromFile_AsVector);
            GoBackScreen();

        }
        void ScreenAddUser() {
            system("cls");
            cout << "---------------------------\n";
            cout << "\t Add New User \n";
            cout << "---------------------------\n";

            string UserName = RaedUser_UserName();
            if (!FindUserByUserName(UserName))
            {
                vector<stUser>vUserr;
                stUser D1;
                D1.Username = UserName;
                cout << "\nEnter PassWord :"; cin >> D1.Password;
                char Answer;
                cout << "\nDo You Want give him all permission?(Y/N)";
                cin >> Answer;
                if (tolower(Answer) == 'y')
                {
                    D1.Permissions = -1;
                }
                else
                {
                    D1.Permissions = AddPermission();
                }
                vUserr.push_back(D1);
                SaveVectorToFileToAdd_User(UserFileName, vUserr);
            }
            else
            {
                cout << "\nWrong User Name Use Another One ";
                cin.clear();
                ScreenAddUser();
            }
            GoBackScreen();

        }
        void ScreenToDeleteUser()
        {
            system("cls");
            cout << "---------------------------\n";
            cout << "\t Delete User \n";
            cout << "---------------------------\n";


            //vector<stUser>vUserDataFromFile_AsVector = LoadUsersDataFromFile_stUser(UserFileName);
            DeleteUserByUserName(RaedUser_UserName());
            GoBackScreen();
        }
        void ScreenToUpdataUser()
        {
            system("cls");
            cout << "---------------------------\n";
            cout << "\t Updata User  \n";
            cout << "---------------------------\n";


            UpdataUserByUserName(RaedUser_UserName());
            GoBackScreen();
        }
        void ScreenToFindUserByUserName()
        {
            system("cls");
            cout << "---------------------------\n";
            cout << "\t Find User  \n";
            cout << "---------------------------\n";

            FindUserByUserNameToShowing(UserFileName);
            GoBackScreen();
        }
        void ScreenUser(enUsersTool enUser)
        {

            switch (enUser) {
            case enUsersTool::UserList:
                //

                ScreenUserList();
                break;
            case enUsersTool::AddUser:
                //
                ScreenAddUser();
                break;
            case enUsersTool::DeletUser:
                //
                ScreenToDeleteUser();
                break;
            case enUsersTool::UpdateUser:
                //
                ScreenToUpdataUser();
                break;
            case enUsersTool::FindUser:
                //
                ScreenToFindUserByUserName();
                break;
            case enUsersTool::MainMenue:
                //
                break;

            default:
                system("cls");
                cout << ":( Wrong Chosses";

            }
        }

        void MainScreenToManagmentUsers()
        {

            system("cls");
            MainScreenForUser();
            short Chosse = 0;
            cin >> Chosse;
            ScreenUser((enUsersTool)Chosse);


        }

        /////
        ///////
        /////////
        ///////////

        void _LoginPage();

            const string ClientFileName = "Client_data.txt";

            enum enTransaction_Menue_Screen {
                eDposit = 1, eWithdrow = 2, eTotelBalanc = 3, eMainMene = 4
            };
            void Transactcion();
            void GoBackToTransactionMenue()
            {

                cout << "\n\nPress any key to go back to Transaction Menue...";
                system("pause>0");
                cout << endl;
                Transactcion();
            }
            short ReadNumberToPerfromTransctionMenueScreen()
            {
                short N;

                cin >> N;
                return N;
            }
            void ShowDeposit()
            {
                cout << "---------------------------\n";
                cout << "\t Deposit  \n";
                cout << "---------------------------\n";
                Deposit(ClientFileName);
            }
            void ShowWithdrow() {
                cout << "---------------------------\n";
                cout << "\t Deposit  \n";
                cout << "---------------------------\n";
                WithDrow(ClientFileName);
            }
            void ShowTotelBalanc()
            {
                cout << setw(40) << "Balances List (" << HowManyClientHave(ClientFileName) << ") Client(s).\n";
                cout << "--------------------------------------------------------------\n";
                cout << "| Account Number" << setw(20) << "| Client Name" << setw(30) << "| Balanc \n";
                cout << "--------------------------------------------------------------\n";
                TotalBalanc(ClientFileName);
            }
            void PerfromTransctionMenueScreen(enTransaction_Menue_Screen TransactionMenue)
            {

                switch (TransactionMenue)
                {
                case enTransaction_Menue_Screen::eDposit:
                    system("cls");
                    //Deposit;
                    //Deposit(ClientFileName);
                    ShowDeposit();
                    GoBackToTransactionMenue();
                    break;
                case enTransaction_Menue_Screen::eWithdrow:
                    system("cls");
                    //withdow
                    //WithDrow(ClientsFileName);
                    ShowWithdrow();
                    GoBackToTransactionMenue();
                    break;

                case enTransaction_Menue_Screen::eTotelBalanc:
                    system("cls");

                    //TotleBalace;
                    //TotalBalanc(ClientsFileName);
                    ShowTotelBalanc();
                    GoBackToTransactionMenue();
                    break;
                case enTransaction_Menue_Screen::eMainMene:
                    system("cls");
                    //main Menue 

                    break;

                }
            }
            void Transactcion()
            {
                system("cls");
                cout << "==========================================\n";
                cout << setw(32) << right << "Transaction Menue Screen\n";
                cout << "==========================================\n";
                cout << setw(16) << right << "[1]" << " Deposit.\n";
                cout << setw(16) << right << "[2]" << " Withdrow.\n";
                cout << setw(16) << right << "[3]" << " Total Balances.\n";
                cout << setw(16) << right << "[4]" << " Main Menue.\n";
                cout << "==========================================\n";

                cout << "Choose what do you want to do ? [1 to 4]?";
                PerfromTransctionMenueScreen((enTransaction_Menue_Screen)ReadNumberToPerfromTransctionMenueScreen());
            }
            void MainMenueScreen() {
                cout << "==========================================\n";
                cout << setw(32) << right << "Main Menue Screen\n";
                cout << "==========================================\n";
                cout << setw(16) << right << "[1]" << " Show Client List.\n";
                cout << setw(16) << right << "[2]" << " Add New Client.\n";
                cout << setw(16) << right << "[3]" << " Delete Client.\n";
                cout << setw(16) << right << "[4]" << " Update Client.\n";
                cout << setw(16) << right << "[5]" << " Find Client.\n";
                cout << setw(16) << right << "[6]" << " Transaction.\n";
                cout << setw(16) << right << "[7]" << " User Managment.\n";
                cout << setw(16) << right << "[8]" << " Logout.\n";
                cout << "==========================================\n";

                cout << "Choose what do you want to do ? [1 to 8]?";

            }
            void GoBackTo_LoginPage()
            {
                system("cls");
                _LoginPage();
            }
            void MainMenueChoos(short WhatYouChoos)
           
            {

                vector<sClient>vClient = LoadCleintsDataFromFile(ClientFileName);
                stUser UserLoginedInfo = TakeUserInfoFromLogin();
                switch (WhatYouChoos)
                {
                case 1:
                    if (((CheckPermission(UserLoginedInfo.Permissions & 0b0001, 0)) || UserLoginedInfo.Permissions == -1))
                    {
                        //Show Client 
                        PrintAllClientsData(vClient);
                        break;
                    }
                    else
                    {
                        cout << "No permission to show client list.\n";
                        break;
                    }
                case 2:
                    if ((CheckPermission(UserLoginedInfo.Permissions & 0b0011, 1)) || UserLoginedInfo.Permissions == -1)
                    {
                        //AddNewClient
                        SaveVectorToFileToAdd(ClientFileName, GetClientInfo());
                        break;
                    }
                    else
                    {
                        cout << "No permission to Add New client.\n";
                        break;
                    }
                case 3:
                    if ((CheckPermission(UserLoginedInfo.Permissions & 0b0111, 2)) || UserLoginedInfo.Permissions == -1)
                    {
                        //Delete
                        DeleteClientByAccountNumber(ReadClientAccountNumber(), vClient);
                        break;
                    }
                    else
                    {
                        cout << "No permission to Delet client.\n";
                        break;
                    }
                case 4:
                    if ((CheckPermission(UserLoginedInfo.Permissions & 0b1111, 3)) || UserLoginedInfo.Permissions == -1)
                    {
                        //Update Client
                        UpdateClientByAccountNumber(ReadClientAccountNumber(), vClient);
                        break;
                    }
                    else
                    {
                        cout << "No permission to Updata client.\n";
                        break;
                    }
                case 5:
                    if ((CheckPermission(UserLoginedInfo.Permissions & 0b00011111, 4)) || UserLoginedInfo.Permissions == -1)
                    {
                        //Find Client
                        FindClientByAccountNumberToPrintHim(ClientsFileName);
                        break;
                    }
                    else
                    {
                        cout << "No permission to Find client.\n";
                        break;
                    }
                case 6:
                    if ((CheckPermission(UserLoginedInfo.Permissions & 0b00111111, 5)) || UserLoginedInfo.Permissions == -1)
                    {
                        //Transcaton
                        Transactcion();
                        break;
                    }
                    else
                    {
                        cout << "No permission to Transaction.\n";
                        break;
                    }
                case 7:
                    if ((CheckPermission(UserLoginedInfo.Permissions & 0b01111111, 6)) || UserLoginedInfo.Permissions == -1)
                    {
                        //User Mangment 
                        MainScreenToManagmentUsers();
                        break;
                    }
                    else
                    {
                        cout << "No permission to User Mangment.\n";
                        break;
                    }
                case 8:
                    //Logout 
                    system("cls");
                    //   _LoginPage();
                    
                    GoBackTo_LoginPage();
            
                    break;

                default:
                    cout << "\nYour Choose Wrong! ,Choos Right One Agane...\n";

                    //Make him go bake to start 

                }
            }


            void ElsePage()
            {
                do {
                    MainMenueScreen();
                    //system("cls");
                    MainMenueChoos(Validate_Number());
                    system("pause");

                    system("cls");

                } while (0 == 0);
            }
        
            ///
            ////
            /////
            ///////
            ////////
            //////////


               
                void _LoginPage();
                string ReadUserName()
                {
                    string UserName;
                    cout << "Enter Username ?";
                    getline(cin>>ws, UserName);
                    return UserName;
                }
                int ReadPassword()
                {
                    int Password;
                    cout << "\nEnter Password ?";

                    cin >> Password;
                    return Password;
                }

                bool IS_UserAndPass(string Username, int Password, string FileName)
                {
                    vector<stUser>vUsers = LoadUsersDataFromFile_stUser(FileName);
                    for (stUser& user : vUsers)
                    {
                        if (user.Username == Username && user.Password == Password)
                            return true;
                    }
                    return false;
                }
                void _ReTry()
                {
                    _LoginPage();
                }
                
                void _LoginPage()
                {

                    string User_Logined = ReadUserName();
                    int Password = ReadPassword();

                    ptrUser_Logined = &User_Logined;
                    if (IS_UserAndPass(User_Logined, Password, UserFileName))
                    {
                        system("color 0f");
                        system("cls");
                        ElsePage();

                    }
                    else {
                        system("color 04");
                        cout << "\nWRong!\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        _ReTry();
                    }
                }
            


                ///
                ////
                /////
                ///////

                int main()
                {

                    system("color 0f");

                    _LoginPage();


                    return 0;
                }



