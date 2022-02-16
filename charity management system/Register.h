#ifndef REGISTER_H_INCLUDED
#define REGISTER_H_INCLUDED

#include "Admin.h"
#include "Donor.h"
#include "Recipient.h"
#include "Register.h"
#include "Request.h"
#include "Donation.h"
#include "Payment.h"


#include <iostream>
using namespace std;
#include <windows.h>
#include <mysql.h>
#include<cstring>
#include<string>
#include <sstream>

char* returnCategory(int value)
{
	char category1[15] = "Education";
	char category2[15] = "Health";
	char category3[15] = "Environment";
	char category4[15] = "Needy";

	char* cat;
	int i = 0;
	if (value == 1)
	{
		int len = strlen(category1);
		cat = new char[len + 1];
		for (i = 0; i < len; i++)
			cat[i] = category1[i];
		cat[i] = '\0';

	}
	else if (value == 2)
	{
		int len = strlen(category2);
		cat = new char[len + 1];
		for (i = 0; i < len; i++)
			cat[i] = category2[i];
		cat[i] = '\0';
	}
	else if (value == 3)
	{
		int len = strlen(category3);
		cat = new char[len + 1];
		for (i = 0; i < len; i++)
			cat[i] = category3[i];
		cat[i] = '\0';
	}
	else if (value == 4)
	{
		int len = strlen(category4);
		cat = new char[len + 1];
		for (i = 0; i < len; i++)
			cat[i] = category4[i];
		cat[i] = '\0';
	}
	else
		return NULL;
	return cat;
}

class Register
{

private:
	Donation* currD;
	Request* currR;
public:
	Donation* makeNewDonation(int cnic)
	{
		currD = new Donation(cnic);

		return currD;
	}
	Payment* makePayment(int amount, int& totalfunds)
	{
		return currD->makepayment(amount, totalfunds);
	}
	Request* makeNewRequest(int cnic)
	{
		int reqAmount = 1;
		cout << "Enter amount to Request : ";
		cin >> reqAmount;
		while (reqAmount < 1)
		{
			cout << "Invalid amount! Enter again : ";
			cin >> reqAmount;
		}

		currR = new Request(cnic, reqAmount);


		cout << "Request amount : " << currR->getAmount() << endl;
		return currR;
	}
	void selectCategory()
	{

		cout << "1.Education\n2.Health\n3.Environment\n4.Needy\n";
		cout << "Enter an option from 1 to 4 : ";
		int opt = 1;
		cin >> opt;
		while (opt < 1 || opt >4)
		{
			cout << "Invalid option Enter again : ";
			cin >> opt;
		}
		currR->setcategory(returnCategory(opt));
		cout << "Enter description : ";
		char* description = new char[100];
		cin.ignore();
		cin.getline(description, 100);
		currR->setDescription(description);

		//Store cnic,name,category,amount in RecipientList

	}
	void changePriority(int value)
	{
		currR->setpriority(value);
	}
};

#endif // REGISTER_H_INCLUDED
