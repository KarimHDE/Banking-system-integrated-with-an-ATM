#pragma once
#include "iostream"
#include "clsScreen.h"
#include "clsUser.h"
#include "clsInputValidate.h"
#include "clsMainScreen.h"
#include "Global.h"
using namespace std;

class clsLoginScreen : protected clsScreen
{

private:



	static bool _Login()
	{
		bool logInFailed = false;
		short failedLogInCount = 0;

		do
		{
			if (logInFailed)
			{
				failedLogInCount++;

				system("cls");
				_drawScreenHeader("  Login Screen");

				cout << "\nInvalid username/password!\n";
				cout << "You Have " << (3 - failedLogInCount) << " Trial(s) To Login.\n\n";

				if (failedLogInCount == 3) 
				{
					cout << "\nYou Are Locked After 3 Failed Trial(s)\n\n";
					return false;
				}
			}


			string username = clsInputValidate::readString("\tEnter Username :  ");
			string password = clsInputValidate::readString("\tEnter Password :  ");

			currentUser = clsUser::find(username, password);

			(currentUser.isEmpty()) ? logInFailed = true : logInFailed = false;

		} while (logInFailed);


		currentUser.registerLogin();

		clsMainScreen::showMainScreen();

		return true;
	}


public:

	static bool ShowLoginScreen()
	{
		return _Login();
	}
};




