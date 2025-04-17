#include <iostream>
#include "clsLoginScreen.h"

using namespace std;
int main()
{

    while (!clsLoginScreen::ShowLoginScreen())
    {
        clsLoginScreen::ShowLoginScreen();
    }




    return 0;
}

