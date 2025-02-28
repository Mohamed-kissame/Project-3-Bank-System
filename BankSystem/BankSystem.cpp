#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

const string ClientFileName = "Client.txt";

const string UserFileName = "User.txt";

enum enMenuChoice {

	ShowClient = 1, AddClient = 2, DeleteClient = 3, UpdateClient = 4, FindClient = 5, MenuTransactions = 6, ManageUsers = 7, Logout = 8

};

enum enMenuTransaction {

	Deposit = 1, Withdraw = 2, TotalBalances = 3, MainMenu = 4
};

enum enMenuManageUsers {

	ListUser = 1, AddNewUsesr = 2, DeleteUser = 3, UpdateUser = 4, FindUser = 5, MainMenue = 6
};

enum enMainMenuPermissions {

	ALL = -1, pLisCLinet = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClients = 8,

	pFindClient = 16, pTrnsaction = 32, pManageUsers = 64
};

void ShowMenuScreen();
void StartBankingSystem();
void StartWithTransactionMenu();
void StartWithManageMenu();
bool checkAccessPermission(enMainMenuPermissions Permissions);
int ReadPermissionsToSet();
void ShowAccessDeniedMessage();
void GoBackToMainMenue();
void Login();

struct stClient {

	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0;
	bool MarkDelete = false;

};

struct stUser {

	string UserName;
	string Password;
	int Permission = 0;
	bool MarkDelete = false;

};

stUser CurrentUser;

vector <string> SplitString(string s1, string Delim) {

	vector <string> vString;

	short pos = 0;

	string sWord = "";

	while ((pos = s1.find(Delim)) != std::string::npos) {

		sWord = s1.substr(0, pos);

		if (sWord != "") {

			vString.push_back(sWord);
		}

		s1.erase(0, pos + Delim.length());

	}

	if (s1 != "") {

		vString.push_back(s1);
	}

	return vString;
}

string ConvertRecordToLine(stClient Client, string seperator = "#//#") {

	string DataLine = "";

	DataLine += Client.AccountNumber + seperator;
	DataLine += Client.PinCode + seperator;
	DataLine += Client.Name + seperator;
	DataLine += Client.Phone + seperator;
	DataLine += to_string(Client.AccountBalance);

	return DataLine;

}

vector <string> SaveLineInVector(stClient Client) {

	vector <string> vClient;

	string Line = ConvertRecordToLine(Client);

	vClient.push_back(Line);

	return vClient;
}

void SaveVectorToLine(string FileName, vector <string>& vFileContent) {

	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open()) {

		for (string& Dataline : vFileContent) {

			if (Dataline != "") {

				MyFile << Dataline << endl;
			}
		}

		MyFile.close();
	}
}

stClient ConvertLineToRecord(string s1, string seperator = "#//#") {

	stClient Client;

	vector <string> vString = SplitString(s1, seperator);

	Client.AccountNumber = vString.at(0);
	Client.PinCode = vString.at(1);
	Client.Name = vString.at(2);
	Client.Phone = vString.at(3);
	Client.AccountBalance = stod(vString.at(4));

	return Client;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {

	vector <stClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber) {

				MyFile.close();
				return true;
			}

			vClients.push_back(Client);

		}

		MyFile.close();
	}

	return false;
}

stClient ReadinfoClint() {

	stClient Client;

	cout << "\nEnter Account Number ? ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientFileName)) {

		cout << "\nClient with [" << Client.AccountNumber << "] already exists , Enter another Account Number ?";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter Pin Code?";
	getline(cin, Client.PinCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBlance ? ";
	cin >> Client.AccountBalance;
	cin.ignore();

	return Client;

}

void HeadOfAddClient() {

	cout << "\n________________________________________________\n";
	cout << "\n\t\tAdd New Clients Screen\t\t";
	cout << "\n________________________________________________\n";

	cout << "\nAdding New Client:\n\n";
}

void AddNewClient() {

	stClient Client;

	vector <string> vFileContent;

	HeadOfAddClient();

	Client = ReadinfoClint();

	vFileContent = SaveLineInVector(Client);

	SaveVectorToLine(ClientFileName, vFileContent);
}

void ShowAddClientsScreen() {

	if (!checkAccessPermission(enMainMenuPermissions::pAddNewClient)) {

		ShowAccessDeniedMessage();
		return;
	}

	char AddMore = 'Y';

	do {

		AddNewClient();

		cout << "\nClient Added Successfully, Do you want to add more clients ? Y/N : ";
		cin >> AddMore;

		cin.ignore();

		system("cls");

	} while (toupper(AddMore) == 'Y');


}

vector <stClient>  LoadDataFromFileToVector(string FileName) {

	vector <stClient> vClient;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;
		stClient Client;

		while (getline(MyFile, Line)) {

			Client = ConvertLineToRecord(Line);
			vClient.push_back(Client);
		}

		MyFile.close();
	}

	return vClient;
}

void PrintClientRecord(stClient client) {

	cout << "\n|" << setw(15) << left << client.AccountNumber;
	cout << "|" << setw(10) << left << client.PinCode;
	cout << "|" << setw(40) << left << client.Name;
	cout << "|" << setw(12) << left << client.Phone;
	cout << "|" << setw(12) << left << client.AccountBalance;

}

void PrintAllClient(vector <stClient>  vClient) {

	cout << "\n\t\t\t\tClient List (" << vClient.size() << ") Client(s).";
	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n";

	cout << "\n|" << left << setw(15) << "Account Number";
	cout << "|" << left << setw(10) << "Pin Code";
	cout << "|" << left << setw(40) << "Client Name";
	cout << "|" << left << setw(12) << "Phone";
	cout << "|" << left << setw(12) << "Account Blance";

	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n";

	if (vClient.size() == 0) {

		cout << "\t\t\tNo Clients Available In the System!";
	}

	for (stClient& client : vClient) {

		PrintClientRecord(client);

		cout << endl;
	}

	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n";

}

void ShowListClientScreen() {

	if (!checkAccessPermission(enMainMenuPermissions::pLisCLinet)) {

		ShowAccessDeniedMessage();
		return;
	}

	vector <stClient> vClient = LoadDataFromFileToVector(ClientFileName);

	PrintAllClient(vClient);
}

void tHeadOfFindClient() {

	cout << "\n________________________________________________\n";
	cout << "\n\t\tFind Client Screen\t\t";
	cout << "\n________________________________________________\n\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector <stClient>& vClient, stClient& Client) {

	for (stClient& c : vClient) {

		if (c.AccountNumber == AccountNumber) {

			Client = c;

			return true;
		}
	}

	return false;

}

void PrintClientCard(stClient Client) {

	cout << "\nThe following are the client Details:\n";

	cout << "__________________________________________\n";

	cout << "\nAccount Number: " << Client.AccountNumber << endl;
	cout << "Pin Code        : " << Client.PinCode << endl;
	cout << "Name            : " << Client.Name << endl;
	cout << "Phone           : " << Client.Phone << endl;
	cout << "Account Balanace : " << Client.AccountBalance << endl;

	cout << "__________________________________________";

}

string ReadAccountNumber() {

	string AccountNumber = "";

	cout << "\nPlease Enter AccountNumber ? ";
	cin >> AccountNumber;

	return AccountNumber;
}

void ShowFindClientScreen() {

	if (!checkAccessPermission(enMainMenuPermissions::pFindClient)) {

		ShowAccessDeniedMessage();
		return;
	}

	string AccountNumber;
	vector <stClient> vClient;

	stClient Client;

	tHeadOfFindClient();

	AccountNumber = ReadAccountNumber();

	vClient = LoadDataFromFileToVector(ClientFileName);

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{

		PrintClientCard(Client);
	}
	else {

		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";

	}

}

void HeadOfDeleteClient() {

	cout << "\n________________________________________________\n";
	cout << "\n\t\tDelete Client Screen\t\t";
	cout << "\n________________________________________________\n\n";

}

vector <stClient> SaveClientsDatToFile(string FileName, vector <stClient> vClient) {

	fstream MyFile;

	MyFile.open(FileName, ios::out);

	string DatLine;

	if (MyFile.is_open()) {

		for (stClient c : vClient) {

			if (c.MarkDelete == false) {

				DatLine = ConvertRecordToLine(c);
				MyFile << DatLine << endl;

			}
		}

		MyFile.close();

	}

	return vClient;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClient) {

	for (stClient& c : vClient) {

		if (c.AccountNumber == AccountNumber) {

			c.MarkDelete = true;

			return true;
		}
	}

	return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClient>& vClient) {

	stClient Client;

	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client)) {

		PrintClientCard(Client);

		cout << "\n\nAre you sure you want delete this client? y/n ?";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {

			MarkClientForDeleteByAccountNumber(AccountNumber, vClient);
			SaveClientsDatToFile(ClientFileName, vClient);

			vClient = LoadDataFromFileToVector(ClientFileName);

			cout << "\n\nClient Deleted Successfully.";

			return true;
		}
	}
	else {

		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}

}

void ShowDeleteClientScreen() {

	if (!checkAccessPermission(enMainMenuPermissions::pDeleteClient)) {

		ShowAccessDeniedMessage();
		return;
	}

	vector <stClient> vClient = LoadDataFromFileToVector(ClientFileName);

	HeadOfDeleteClient();

	string AccountNumber = ReadAccountNumber();

	DeleteClientByAccountNumber(AccountNumber, vClient);
}

void HeadOfUpdateClient() {

	cout << "\n________________________________________________\n";
	cout << "\n\t\tUpdate Client Screen\t\t";
	cout << "\n________________________________________________\n\n";

}

stClient ChangeClientRecord(string AccountNumber) {

	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\nEnter Pin Code ?";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance ? ";
	cin >> Client.AccountBalance;

	return Client;

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <stClient>& vClient) {

	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client)) {

		PrintClientCard(Client);

		cout << "\n\nAre you sure you want update this client ? y/n ?";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {

			for (stClient& C : vClient) {

				if (C.AccountNumber == AccountNumber) {

					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientsDatToFile(ClientFileName, vClient);

			cout << "\nClient update Successfully!\n";

			return true;
		}
	}

	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";

		return false;
	}

}

void DepositToBalance(stClient& Client) {

	double DepositBlance = 0;

	cout << "\n\nPlease enter deposit amount ?";
	cin >> DepositBlance;

	if (DepositBlance > 0) {

		Client.AccountBalance += DepositBlance;

		cout << "\nDeposit Successful. New Blance is :" << Client.AccountBalance << endl;
	}
	else {

		cout << "\nInvalid deposit account. Please enter a positive number";
	}
}

void ShowDepositScreen() {

	cout << "__________________________________________________________\n";
	cout << "\n\t\t Deposit Screen\t\n";
	cout << "__________________________________________________________\n";

	string AccountNumber;
	vector <stClient> vClinet = LoadDataFromFileToVector(ClientFileName);
	stClient Client;

	char YouSure = 'Y';

	do {

		AccountNumber = ReadAccountNumber();

		if (!FindClientByAccountNumber(AccountNumber, vClinet, Client)) {

			cout << "\nClient with [" << AccountNumber << "] does not exist" << endl;

		}
	} while (!FindClientByAccountNumber(AccountNumber, vClinet, Client));


	PrintClientCard(Client);
	DepositToBalance(Client);

	for (stClient& Cl : vClinet) {

		if (Cl.AccountNumber == AccountNumber) {

			Cl = Client;
			break;
		}
	}

	cout << "\nAre you sure you want perfrom this transaction? y/n ?";
	cin >> YouSure;

	if (toupper(YouSure) == 'Y') {

		SaveClientsDatToFile(ClientFileName, vClinet);
	}
}

void WithDrawBalance(stClient& Client) {

	double WithDrawAmount = 0;
	bool validWithdrawal = false;

	do {
		cout << "\nPlease enter withdraw amount? ";
		cin >> WithDrawAmount;

		if (WithDrawAmount > Client.AccountBalance) {
			cout << "\n\nAmount Exceeds the balance, you can withdraw up to: " << Client.AccountBalance << endl;
			cout << "Please enter another amount: ";
		}
		else if (WithDrawAmount > 0) {
			Client.AccountBalance -= WithDrawAmount;
			cout << "\nWithdrawal successful. New balance: " << Client.AccountBalance << endl;
			validWithdrawal = true;
		}
		else {
			cout << "\nInvalid amount. Please enter a positive number." << endl;
		}

	} while (!validWithdrawal);
}

void ShowWithDrawScreen() {

	cout << "__________________________________________________________\n";
	cout << "\n\t\t WithDraw Screen\t\n";
	cout << "__________________________________________________________\n";

	string AccountNumber;
	vector <stClient> vClient = LoadDataFromFileToVector(ClientFileName);
	stClient Client;

	char AreYouSure = 'Y';

	do {

		AccountNumber = ReadAccountNumber();

		if (!FindClientByAccountNumber(AccountNumber, vClient, Client)) {

			cout << "\nClient with [" << AccountNumber << "] does not exist" << endl;

		}
	} while (!FindClientByAccountNumber(AccountNumber, vClient, Client));

	PrintClientCard(Client);
	WithDrawBalance(Client);

	cout << "\nAre you sure you want perfrom this transaction? y/n ?";
	cin >> AreYouSure;

	if (toupper(AreYouSure) == 'Y') {

		for (stClient& Cl : vClient) {

			if (Cl.AccountNumber == AccountNumber) {

				Cl = Client;
				break;
			}
		}

		SaveClientsDatToFile(ClientFileName, vClient);

		cout << "\n\nTransaction Completed successfully , now your Balance is : " << Client.AccountBalance << endl;
	}
	else {

		cout << "\nTransaction canceled.\n";
	}


}

void PrintBlanaceRecord(stClient Client) {

	cout << "\n|" << setw(18) << left << Client.AccountNumber;
	cout << "|" << setw(40) << left << Client.Name;
	cout << "|" << setw(12) << left << Client.AccountBalance;
}

void PrintListBalanceOfClients(vector <stClient> vClient) {

	cout << "\n\t\t\t\t Balances List (" << vClient.size() << ") Client(s).";
	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n";

	cout << "\n|" << left << setw(18) << "Account Number";
	cout << "|" << left << setw(40) << "Client Name";
	cout << "|" << left << setw(12) << "Balance";

	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n";

	if (vClient.size() == 0) {

		cout << "\t\t\tNo Clients Available In the System!";

	}

	double TotalBalance = 0;

	for (stClient& Client : vClient) {

		PrintBlanaceRecord(Client);
		TotalBalance += Client.AccountBalance;
		cout << endl;
	}

	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n";

	cout << "\n\t\t\t\tTotal Balances = " << TotalBalance << endl;
}

void ShowTotalBalancesScreen() {

	vector <stClient> vClient = LoadDataFromFileToVector(ClientFileName);
	PrintListBalanceOfClients(vClient);
}

void GoBackToTransactionMenu() {

	cout << "\nPress Any Key To Return to the Transaction main... ";
	system("pause>0");
	StartWithTransactionMenu();
}

void ShowMenuTransaction() {

	system("cls");

	cout << "============================================================\n";
	cout << "\t\tMenu Transaction Screen\t";
	cout << "\n============================================================\n";

	cout << "\t\t[1] Deposit." << endl;
	cout << "\t\t[2] WithDraw." << endl;
	cout << "\t\t[3] Total Balances." << endl;
	cout << "\t\t[4] Main Menu." << endl;

	cout << "============================================================\n";
}

enMenuTransaction GetUserChoice() {

	short Choice = 0;

	cout << "Choose what do you want to do? [1 to 4 ] ?";
	cin >> Choice;

	while (cin.fail() || Choice < 1 || Choice > 4) {

		cin.clear();

		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		cout << "\nInvalide Input :(- ,Enter a Number between 1 & 4 ?";
		cin >> Choice;
	}

	return (enMenuTransaction)Choice;
}

void GetChoiceFromTransactionMenu(enMenuTransaction Transaction) {

	switch (Transaction) {

	case enMenuTransaction::Deposit: {

		system("cls");
		ShowDepositScreen();
		GoBackToTransactionMenu();
		break;
	}
	case enMenuTransaction::Withdraw:
	{
		system("cls");
		ShowWithDrawScreen();
		GoBackToTransactionMenu();
		break;
	}
	case enMenuTransaction::TotalBalances:
	{
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionMenu();
		break;
	}
	case enMenuTransaction::MainMenu: {

		system("cls");
		StartBankingSystem();
		return;
	}
	}
}

void StartWithTransactionMenu() {

	if (!checkAccessPermission(enMainMenuPermissions::pTrnsaction)) {

		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}

	enMenuTransaction Transaction;

	do {

		ShowMenuTransaction();
		Transaction = GetUserChoice();
		GetChoiceFromTransactionMenu(Transaction);
	} while (Transaction != 5);
}

void ShowAccessDeniedMessage()
{
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this, \nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";

}

bool checkAccessPermission(enMainMenuPermissions Permissions) {

	if (CurrentUser.Permission == enMainMenuPermissions::ALL)
		return true;
	if ((Permissions & CurrentUser.Permission) == Permissions)
		return true;
	else
		return false;
}

void ShowUpdateClientScreen() {

	if (!checkAccessPermission(enMainMenuPermissions::pUpdateClients)) {

		ShowAccessDeniedMessage();
		return;
	}


	vector <stClient> vClient = LoadDataFromFileToVector(ClientFileName);

	HeadOfUpdateClient();

	string AccountNumber = ReadAccountNumber();

	UpdateClientByAccountNumber(AccountNumber, vClient);

}

void ShowExitSceen() {

	cout << "\n________________________________________________\n";
	cout << "\n\t\tProgram Ends :-)\t\t";
	cout << "\n________________________________________________\n\n";

}

void GoBackToMainMenue() {

	cout << "\nPress any key to return to the main menu...";
	system("pause>0");
	ShowMenuScreen();

}

void GoBackToManageMenu() {

	cout << "\nPress Any Key To Return to the Manage User main... ";
	system("pause>0");
	StartWithManageMenu();
}

void HeadOfAddUser() {


	cout << "\n________________________________________________\n";
	cout << "\n\t\tAdd Users Screen\t\t";
	cout << "\n________________________________________________\n\n";

}

void HeadOfDeleteUser() {


	cout << "\n________________________________________________\n";
	cout << "\n\t\tDelete Users Screen\t\t";
	cout << "\n________________________________________________\n\n";

}

void HeadOfUpdateUser() {


	cout << "\n________________________________________________\n";
	cout << "\n\t\tUpdate Users Screen\t\t";
	cout << "\n________________________________________________\n\n";

}

void HeadOfFindUser() {


	cout << "\n________________________________________________\n";
	cout << "\n\t\tFind Screen\t\t";
	cout << "\n________________________________________________\n\n";

}

stUser ConvertLineToRecordUser(string s1, string seperator = "#//#") {

	stUser User;

	vector <string> vString = SplitString(s1, seperator);

	User.UserName = vString.at(0);
	User.Password = vString.at(1);
	User.Permission = stoi(vString.at(2));

	return User;
}

vector <stUser> LoadDataFromFileUserToVector(string FileName) {

	vector <stUser> vUser;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;
		stUser User;

		while (getline(MyFile, Line)) {

			User = ConvertLineToRecordUser(Line);
			vUser.push_back(User);
		}

		MyFile.close();
	}

	return vUser;
}

void PrintUserCard(stUser Users) {

	cout << "\n|" << setw(18) << left << Users.UserName;
	cout << "|" << setw(12) << left << Users.Password;
	cout << "|" << setw(12) << left << Users.Permission;

}

void PrintAllUsers(vector <stUser> vUsers) {

	cout << "\n\t\t\t\t Users List (" << vUsers.size() << ") User(s).";
	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n";

	cout << "\n|" << left << setw(18) << "User Name";
	cout << "|" << left << setw(12) << "Password";
	cout << "|" << left << setw(12) << "Permissions";

	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n";

	if (vUsers.size() == 0) {

		cout << "\t\t\tNo Users Available In the System!";

	}


	for (stUser Users : vUsers) {

		PrintUserCard(Users);

		cout << endl;
	}

	cout << "\n__________________________________________________________";
	cout << "__________________________________________________________\n";


}

void ShowListUserScreen() {

	vector <stUser> vUser = LoadDataFromFileUserToVector(UserFileName);

	PrintAllUsers(vUser);
}

bool UsersExistsByAccoutName(string UserName, string FileName) {

	vector <stUser> vUser;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;
		stUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertLineToRecordUser(Line);
			if (User.UserName == UserName) {

				MyFile.close();
				return true;
			}

			vUser.push_back(User);

		}

		MyFile.close();
	}

	return false;
}

string ConvertRecordToLine(stUser Users, string seperator = "#//#") {

	string DataLine = "";

	DataLine += Users.UserName + seperator;
	DataLine += Users.Password + seperator;
	DataLine += to_string(Users.Permission) + seperator;


	return DataLine;

}

int ReadPermissionsToSet() {

	int Permissions = 0;

	char Answer = 'n';

	cout << "\nDo you want to give full access? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {

		return -1;
	}

	cout << "\nDo you want to give access to : \n ";

	cout << "\nShow Client List? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {

		Permissions += enMainMenuPermissions::pLisCLinet;
	}

	cout << "\nAdd New Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {

		Permissions += enMainMenuPermissions::pAddNewClient;
	}

	cout << "\nDelete Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {

		Permissions += enMainMenuPermissions::pDeleteClient;
	}

	cout << "\nUpdate Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {

		Permissions += enMainMenuPermissions::pUpdateClients;
	}

	cout << "\nFind Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {

		Permissions += enMainMenuPermissions::pFindClient;
	}

	cout << "\nTransaction Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {

		Permissions += enMainMenuPermissions::pTrnsaction;
	}

	cout << "\nMange Users? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y') {

		Permissions += enMainMenuPermissions::pManageUsers;
	}

	return Permissions;

}

stUser ReadUserInfo() {

	stUser User;

	cout << "\nEnter UserName ? ";
	getline(cin >> ws, User.UserName);

	while (UsersExistsByAccoutName(User.UserName, UserFileName)) {

		cout << "\nUser with [" << User.UserName << "] already exists , Enter another Account Number ?";
		getline(cin >> ws, User.UserName);
	}

	cout << "Enter PassWord ? ";
	getline(cin, User.Password);

	User.Permission = ReadPermissionsToSet();

	return User;
}

vector <string> SaveLineInVector(stUser Users) {

	vector <string> vUsers;

	string Line = ConvertRecordToLine(Users);

	vUsers.push_back(Line);

	return vUsers;
}

void AddNewUser() {

	stUser User;

	vector <string> vFileContent;

	User = ReadUserInfo();

	vFileContent = SaveLineInVector(User);

	SaveVectorToLine(UserFileName, vFileContent);
}

void ShowADDUserScreen() {

	HeadOfAddUser();

	char AddMore = 'Y';

	do {

		AddNewUser();

		cout << "\nUser Added Successfully, Do you want to add more Users ? Y/N : ";
		cin >> AddMore;

		cin.ignore();

		system("cls");

	} while (toupper(AddMore) == 'Y');
}

void PrintCardUser(stUser User) {

	cout << "\nThe following are the User Details:\n";

	cout << "__________________________________________\n";

	cout << "\nUserName    : " << User.UserName << endl;
	cout << "PassWord      : " << User.Password << endl;
	cout << "Permissions   : " << User.Permission << endl;

	cout << "__________________________________________";

}

bool FindUserByAccountName(string AccountNumber, vector <stUser>& vUser, stUser& User) {

	for (stUser& s : vUser) {

		if (s.UserName == AccountNumber) {

			User = s;

			return true;
		}
	}

	return false;

}

vector <stUser> SaveUserDataToFile(string FileName, vector <stUser> vUser) {

	fstream MyFile;

	MyFile.open(FileName, ios::out);

	string DataLine;

	if (MyFile.is_open()) {

		for (stUser User : vUser) {

			if (User.MarkDelete == false) {

				DataLine = ConvertRecordToLine(User);
				MyFile << DataLine << endl;

			}
		}

	}

	return vUser;
}

bool MarkUserForDeleteByAccountNumber(string UserNAme, vector <stUser>& vUser) {

	for (stUser& s : vUser) {

		if (s.UserName == UserNAme) {

			s.MarkDelete = true;

			return true;
		}
	}

	return false;
}

bool DeleteUserByAccountName(string UserNAme, vector <stUser>& vUser) {

	stUser User;

	if (UserNAme == "Admin") {

		cout << "\n\nYou cannot Delete This User.";
		return false;
	}

	char Answer = 'n';

	if (FindUserByAccountName(UserNAme, vUser, User)) {

		PrintCardUser(User);

		cout << "\n\nAre you sure you want delete this client? y/n ?";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {

			MarkUserForDeleteByAccountNumber(UserNAme, vUser);
			SaveUserDataToFile(UserFileName, vUser);

			vUser = LoadDataFromFileUserToVector(UserFileName);

			cout << "\n\nClient Deleted Successfully.";

			return true;
		}
	}
	else {

		cout << "\nUser with Account Number (" << UserNAme << ") is Not Found!";
		return false;
	}

}

stUser ReadCahngeUserRecord(string UserName) {

	stUser User;

	User.UserName = UserName;

	cout << "Enter Password ? ";
	getline(cin >> ws, User.Password);

	User.Permission = ReadPermissionsToSet();


	return User;
}

string ReadUserName() {

	string UserNAME;

	cout << "Enter UserName ? ";
	cin >> UserNAME;

	return UserNAME;
}

bool UpdateUserByUserName(string UserName, vector <stUser>& vUser) {

	stUser User;
	char Answer = 'n';

	if (FindUserByAccountName(UserName, vUser, User)) {

		PrintCardUser(User);

		cout << "\n\nAre you sure you want update this user ? y/n ?";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {

			for (stUser& s : vUser) {

				if (s.UserName == UserName) {

					s = ReadCahngeUserRecord(UserName);
					break;
				}
			}

			SaveUserDataToFile(UserFileName, vUser);

			cout << "\nClient update Successfully!\n";

			return true;
		}
	}

	else
	{
		cout << "\nClient with Account Number (" << UserName << ") is Not Found!";

		return false;
	}

}

void ShowDeleteUserScreen() {

	vector <stUser> vUser = LoadDataFromFileUserToVector(UserFileName);

	HeadOfDeleteUser();

	string UserNAme = ReadUserName();

	DeleteUserByAccountName(UserNAme, vUser);

}

void ShowUpdateUserScreen() {

	vector <stUser> vUser = LoadDataFromFileUserToVector(UserFileName);

	HeadOfUpdateUser();

	string UserName = ReadUserName();

	UpdateUserByUserName(UserName, vUser);
}

void ShowFindUserScreen() {

	string UserName;

	vector <stUser> vUser;

	stUser user;

	HeadOfFindUser();

	UserName = ReadUserName();

	vUser = LoadDataFromFileUserToVector(UserFileName);

	if (FindUserByAccountName(UserName, vUser, user)) {

		PrintCardUser(user);

	}
	else {

		cout << "\nUser with UserName (" << UserName << ") is Not Found!";

	}
}

void GetChoiceManageUsersMenu(enMenuManageUsers ManageUserMenu) {

	switch (ManageUserMenu) {

	case enMenuManageUsers::ListUser: {

		system("cls");
		ShowListUserScreen();
		GoBackToManageMenu();

		break;
	}
	case enMenuManageUsers::AddNewUsesr: {

		system("cls");
		ShowADDUserScreen();
		GoBackToManageMenu();

		break;
	}
	case enMenuManageUsers::DeleteUser: {

		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageMenu();

		break;
	}
	case enMenuManageUsers::UpdateUser: {

		system("cls");
		ShowUpdateUserScreen();
		GoBackToManageMenu();

		break;
	}
	case enMenuManageUsers::FindUser: {

		system("cls");
		ShowFindUserScreen();
		GoBackToManageMenu();

		break;
	}
	case enMenuManageUsers::MainMenue: {

		system("cls");
		StartBankingSystem();

		return;
	}
	}
}

enMenuManageUsers ChoiceForManageUser() {

	short MangeUser = 0;

	cout << "Choose what do you want to do? [1 to 6] ? ";
	cin >> MangeUser;

	while (cin.fail() || MangeUser < 1 || MangeUser > 6)
	{

		cin.clear();

		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		cout << "\nInvalide Input :(- ,Enter a Number between 1 & 6 ?";
		cin >> MangeUser;
	}

	return (enMenuManageUsers)MangeUser;
}

void ShowMenuManageUsers() {

	system("cls");

	cout << "============================================================\n";
	cout << "\t\tManage Users Menu Screen\t";
	cout << "\n============================================================\n";

	cout << "\t\t[1] List User." << endl;
	cout << "\t\t[2] Add New User." << endl;
	cout << "\t\t[3] Delete User." << endl;
	cout << "\t\t[4] Update User." << endl;
	cout << "\t\t[5] Find User." << endl;
	cout << "\t\t[6] Main Menu." << endl;

	cout << "\n============================================================\n";

}

void StartWithManageMenu() {

	if (!checkAccessPermission(enMainMenuPermissions::pManageUsers)) {

		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}

	enMenuManageUsers Manage;

	do {

		ShowMenuManageUsers();
		Manage = ChoiceForManageUser();
		GetChoiceManageUsersMenu(Manage);

	} while (Manage != 7);
}

enMenuChoice GetChoiceFromUser() {

	short Choice = 0;

	cout << "Choose what do you want to do? [1 to 8] ?";
	cin >> Choice;

	while (cin.fail() || Choice < 1 || Choice > 8) {

		cin.clear();

		cin.ignore(std::numeric_limits < std::streamsize>::max(), '\n');

		cout << "\nInvalide Input :(- .Enter A number between 1 & 8 ?";
		cin >> Choice;
	}

	return (enMenuChoice)Choice;
}

void GetMenuChoice(enMenuChoice GetChoice) {

	system("cls");

	switch (GetChoice) {

	case enMenuChoice::ShowClient: {

		ShowListClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enMenuChoice::AddClient: {

		ShowAddClientsScreen();
		GoBackToMainMenue();
		break;
	}
	case enMenuChoice::FindClient: {

		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enMenuChoice::DeleteClient:
	{

		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;
	}

	case enMenuChoice::UpdateClient: {

		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;

	}
	case enMenuChoice::MenuTransactions: {

		StartWithTransactionMenu();

		break;
	}
	case enMenuChoice::ManageUsers: {

		StartWithManageMenu();

		break;
	}

	case enMenuChoice::Logout:

		Login();

		return;
	}

}

void ShowMenuScreen() {

	system("cls");

	cout << "=========================================================\n";
	cout << "\t\t\tMain Menue Screen\t\n";
	cout << "=========================================================\n";

	cout << "\t\t[1] Show Client List." << endl;
	cout << "\t\t[2] Add New Client." << endl;
	cout << "\t\t[3] Delete Client." << endl;
	cout << "\t\t[4] Update Client Info." << endl;
	cout << "\t\t[5] Find Client." << endl;
	cout << "\t\t[6] Transaction." << endl;
	cout << "\t\t[7] Manage Users." << endl;
	cout << "\t\t[8] Logout." << endl;

	cout << "=========================================================\n";
}

void StartBankingSystem() {

	enMenuChoice choice;

	do {

		ShowMenuScreen();
		choice = GetChoiceFromUser();
		GetMenuChoice(choice);
	} while (choice != Logout);

}

void ShowLogingScreen() {

	cout << "=========================================================\n";
	cout << "\t\tWelcome to the Banking System\t\t\n";
	cout << "=========================================================\n";

}

string ReadPassWord() {

	string PassWord;

	cout << "Enter Password ? ";
	cin >> PassWord;

	return PassWord;
}

bool FindUserByUserNameAndPassword(string Username, string Password, stUser& User) {

	vector <stUser> vUser = LoadDataFromFileUserToVector(UserFileName);

	for (stUser U : vUser) {

		if (U.UserName == Username && U.Password == Password) {

			User = U;
			return true;
		}

	}

	return false;

}

bool LoadUserInfo(string Username, string Password) {

	if (FindUserByUserNameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;
}

void Login() {

	string UserName, Password;

	bool LoginFaild = false;

	do {

		system("cls");

		ShowLogingScreen();

		if (LoginFaild) {

			cout << "Invalide Username/Password!\n";
		}

		UserName = ReadUserName();
		Password = ReadPassWord();

		LoginFaild = !LoadUserInfo(UserName, Password);


	} while (LoginFaild);

	StartBankingSystem();

}

int main() {


	Login();

	system("pause>0");

	return 0;

}