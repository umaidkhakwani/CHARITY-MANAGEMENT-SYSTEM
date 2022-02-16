#ifndef PAYMENT_H_INCLUDED
#define PAYMENT_H_INCLUDED

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

class Payment
{
private:
	int amount;
	bool status;//True if successful
public:
	Payment()
	{
		amount = 0;
		status = 0;
	}
	bool getStatus()
	{
		return status;
	}
	int getAmount()
	{
	    return amount;
	}
	void tranfer(int bill, int& totalfunds)
	{
		amount = bill;
		totalfunds += amount;
		status = 1;
	}
};

#endif // PAYMENT_H_INCLUDED
