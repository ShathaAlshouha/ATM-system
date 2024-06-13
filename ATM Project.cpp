#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std; 

string ClientsFile = "ClientsFile.txt";

void ShowAtM_MainMenuScreen();

void ShowQuickWithdrawScreen();

void ShowNormalWithdrawScreen();

void Login(); 

struct stClients
{
	string AccountNumber;
	string PIN;
	string Name;
	string Phone;
	float AccountBalance;
};

stClients CurrentClient; 

enum enATMmainMenuScreen
{
	eQuickWithDraw = 1, eNormalWithDraw = 2, eDeposet = 3,
	eCheckBalance = 4, eLogout = 5
};

enum enQuickWithdraw
{
	eTwenty = 1, eFifty = 2, eOneHundred = 3, eTwoHundred = 4,
	eForHundred = 5, eSixHundred = 6, eEightHundred = 7,
	eOneThousand = 8, eExit = 9
};

void GoBackToMenue()
{
	cout << "*Press any key to go back to menue..." << endl;
	system("pause>0");
	ShowAtM_MainMenuScreen(); 
}

void GoBackToNormalMenu()
{
	cout << "*Press any key to go back to menue..." << endl;
	system("pause>0");
	ShowNormalWithdrawScreen(); 
}

short ReadATM_MainMenuOption()	
{
	short Choice = 0;
	cout << "Choose what do you want to do? [1 to 5]? ";
	cin >> Choice;
	while (cin.fail() || Choice < 1 || Choice > 5)
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "*The Option Is Not Available ,Choose what do you want to do? [1 to 5]?";
		cin >> Choice;
	}
	return Choice;
}

short ReadQuickWithdrawOption()
{
	short Choice = 0;
	cout << " Choose what do you want to do? [1 to 9]? ";
	cin >> Choice;
	while (cin.fail() || Choice < 1 || Choice > 9)
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "*The Option Is Not Available ,Choose what do you want to do? [1 to 9]?";
		cin >> Choice;
	}
	return Choice;
}

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	short pos = 0;
	string sWord;

	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		S1.erase(0, pos + Delim.length());
	}
	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

stClients ConvertLineToRecord( string Line)
{
	vector<string>vstring = SplitString(Line, "#//#"); 
	stClients Client; 
	Client.Name = vstring[0];
	Client.AccountNumber = vstring[1];
	Client.PIN = vstring[2];
	Client.Phone = vstring[3]; 
	Client.AccountBalance = stof(vstring[4]);
	return Client; 
}

string ConvertRecordToLine(stClients Client, string Delim = "#//#")
{

	string S = "";

	S = Client.Name + Delim;
	S += Client.AccountNumber + Delim;
	S += Client.PIN + Delim;
	S += Client.Phone + Delim;
	S += to_string(Client.AccountBalance);

	return S;
}

vector<stClients> LoadDataClientsToVector()
{
	vector<stClients> vClients; 
	fstream ClientsFile; 
	ClientsFile.open("ClientsFile.txt", ios::in); 
	if (ClientsFile.is_open())
	{
		string Line; 
		stClients Client; 
		while (getline(ClientsFile , Line))
		{
			if (Line != "")
			{
				Client = ConvertLineToRecord(Line);
				vClients.push_back(Client); 

			}


		}
			
		ClientsFile.close(); 
	}

	return vClients; 
}

stClients FindClientStructByAccountNumber(string AccountNumber)
{
	vector< stClients> vsClient;
	vsClient = LoadDataClientsToVector();
	for (stClients& C : vsClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			return C;
		}
	}

}

bool FindClientByAccountNumberAndPinCode(string AccountNumber,string PIN )
{
	vector<stClients>vCleints = LoadDataClientsToVector(); 

	for (stClients& C : vCleints)
	{
		if (C.AccountNumber == AccountNumber && C.PIN == PIN)
		{
			CurrentClient = C;
			return 1;
		}
	}
	return 0; 

}

void LoadVectorToFile(vector<stClients> vClients)
{
	fstream ClientsFile;
	string Line;
	ClientsFile.open("ClientsFile.txt", ios::out);
	if (ClientsFile.is_open())
	{
		for (stClients& C : vClients)
		{
			Line = ConvertRecordToLine(C);
			ClientsFile << Line << endl;

		}

		ClientsFile.close();
	}
}

short getQuickWithDrawAmount(short QuickWithDrawOption) 
{
	switch (QuickWithDrawOption)

	{
	case 1: return  20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <stClients>& vClients) {
	char Answer = 'n';
	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer; if (Answer == 'y' || Answer == 'Y')
	{
		for (stClients& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				LoadVectorToFile(vClients); 
				cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
				return true;
			}
		}
		return false;
	}
}

void PerfromQuickWithdrawOption(short QuickWithDrawOption)
{
	if (QuickWithDrawOption == 9)
		return;
	short WithDrawBalance = getQuickWithDrawAmount(QuickWithDrawOption);
	
	if (WithDrawBalance > CurrentClient.AccountBalance) 
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";	cout << "Press Anykey to continue...";
		system("pause>0");
		ShowQuickWithdrawScreen();
		return;
	}
	vector <stClients> vClients = LoadDataClientsToVector(); 
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
	CurrentClient.AccountBalance -= WithDrawBalance;

}

void ShowQuickWithdrawScreen()
{
	cout << "===============================\n";
	cout << "     Quick Withdraw Screen  ";
	cout << "\n===============================\n";
	cout << "\t[1] 20      [2] 50 \n";
	cout << "\t[3] 100     [4] 200 \n";
	cout << "\t[5] 400     [6] 600 \n";
	cout << "\t[7] 800     [8] 1000 \n";
	cout << "\t[9] Exit" << endl;
	cout << "===============================\n";

	cout << "\n\n Your Balance Is : " << CurrentClient.AccountBalance << endl << endl;

	PerfromQuickWithdrawOption(ReadQuickWithdrawOption());

}

int ReadWithdraw()
{
	int amount;
	do
	{
		cout << " Enter an amount multiple of 5,s :  ";
		cin >> amount;
	} while (amount % 5 != 0);

	return amount;
}

bool NormalWithdraw()
{
	int WithDrawBalance = ReadWithdraw(); 
	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		cout << "Press Anykey to continue...";
		system("pause>0");
		ShowNormalWithdrawScreen();
		return 1;
	}
	vector <stClients> vClients = LoadDataClientsToVector(); 
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawBalance * -1, vClients);
	CurrentClient.AccountBalance -= WithDrawBalance;
}

void ShowNormalWithdrawScreen()
{
	system("cls"); 
	stClients sClient = FindClientStructByAccountNumber(CurrentClient.AccountNumber);
	cout << "===============================\n";
	cout << "     Normal Withdraw Screen  ";
	cout << "\n===============================\n\n";
	NormalWithdraw();


}

double ReadDepositAmount()
{
	double DepositAmount;

	cout << " Enter a positive Amount : ";
	cin >> DepositAmount;

	return DepositAmount;
}

void Deposit(double DepositAmount)
{
	vector<stClients >vClient = LoadDataClientsToVector();
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClient);
	CurrentClient.AccountBalance += DepositAmount; 

}

void ShowDepositScreen()
{
	cout << "=============================\n";
	cout << "        Deposit Screen  \n";
	cout << "=============================\n";
	Deposit(ReadDepositAmount()); 

}

void ShowBalanceScreen()
{
	vector<stClients> vClients = LoadDataClientsToVector(); 
	cout << "=================================\n";
	cout << "       Check Balace Screen ";
	cout << "\n=================================\n";
    cout << "\n Your balance is : " << CurrentClient.AccountBalance << endl << endl;

}

void PerformATMmainMenueScreen(enATMmainMenuScreen ATM_Option)
{

	switch (ATM_Option)
	{

	case eQuickWithDraw:
	{
		system("cls");
		ShowQuickWithdrawScreen();
		GoBackToMenue(); 
		break;

	}
	case eNormalWithDraw:
	{
		system("cls"); 
		ShowNormalWithdrawScreen(); 
		GoBackToMenue();
		break;
	}
	case eDeposet:
	{
		system("cls");
		ShowDepositScreen();
		GoBackToMenue();
		break;
	}
	case eCheckBalance:
	{
		system("Cls");
		ShowBalanceScreen();
		GoBackToMenue(); 
		break;
	}
	
	case eLogout:
	{
		system("cls"); 
		Login(); 
		break;
	}

	}
}

void ShowAtM_MainMenuScreen()
{
	system("cls");
	cout << "=============================================" << endl;
	cout << "\t  ATM Main Menu Screen  " << endl;
	cout << "=============================================" << endl;
	cout << "\t [1] Quick Withdraw ." << endl;
	cout << "\t [2] Normal Withdraw . " << endl;
	cout << "\t [3] Deposit. " << endl;
	cout << "\t [4] Check Balance . " << endl;
	cout << "\t [5] Logout . " << endl;
	cout << "==============================================" << endl;
	PerformATMmainMenueScreen((enATMmainMenuScreen)ReadATM_MainMenuOption());

}

void Login()
{

	cout << "\n----------------------------\n"; 
	cout << "\t Login screen "; 
	cout << "\n----------------------------\n";

	bool FaildLogin = false; 
	string AccountNumber; 
	string PIN; 

	do
	{
		if (FaildLogin)
		{
			cout << " Invalid Account Number/PinCod ! " << endl;
		}
		cout << " Please Enter Account Number :"; 
		cin >> AccountNumber; 
		cout << " Please Enter Pin : "; 
		cin >> PIN; 
		FaildLogin = !FindClientByAccountNumberAndPinCode(AccountNumber, PIN);
		
	} while (FaildLogin);
	ShowAtM_MainMenuScreen(); 


}

int main()
{
	Login(); 

}
