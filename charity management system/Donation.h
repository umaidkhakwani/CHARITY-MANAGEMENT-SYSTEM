#ifndef DONATION_H_INCLUDED
#define DONATION_H_INCLUDED

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

class Donation
{
private:
	int cnic;
	int totalamount;
	Payment* Pay;
public:
	Donation(int cn)
	{
		cnic = cn;
		totalamount = 0;
		Pay = NULL;
	}
	int getAmount()
	{
		return totalamount;
	}
	Payment* makepayment(int amount, int& totalfunds) {
		totalamount = amount;
		Pay = new Payment();
		Pay->tranfer(amount, totalfunds);
		return Pay;
	}
};

#endif // DONATION_H_INCLUDED
