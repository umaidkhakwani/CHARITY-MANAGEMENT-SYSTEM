#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED

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

class Request
{
private:
	long cnic;
	int totalamount;
	int priority;
	string description;
	string category;
public:
	Request(int cn, int rA)
	{
		cnic = cn;
		totalamount = rA;
		priority = 100;
		category = "";
		description = "";
	}
	void setAmount(int amount)
	{
		totalamount = amount;
	}
	void setpriority(int value)
	{
		priority = value;
	}
	void setcategory(string cat)
	{
		category = cat;
	}
	void setDescription(string des)
	{
		description = des;
	}
	string getDescription()
	{
		return description;
	}
	string getCategory()
	{
		return category;
	}
	int getPriority()
	{
	    return priority;
	}
	int getAmount()
	{
		return totalamount;
	}

};

#endif // REQUEST_H_INCLUDED
