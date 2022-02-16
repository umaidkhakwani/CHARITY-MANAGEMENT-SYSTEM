#ifndef DONOR_H_INCLUDED
#define DONOR_H_INCLUDED

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

void updateFunds(MYSQL* conn,int tamount)
{
	int qstate = 0;
	stringstream ss;
    string s_amount;
	stringstream s1;
	s1 << tamount;
	s1 >> s_amount;
	ss << "UPDATE funds SET totalfunds =  '" + s_amount + "' ";
	string query = ss.str();
	const char* q = query.c_str();
	qstate = mysql_query(conn, q);
	if (qstate == 0)
	{
		cout << "Records updated...\n";
	}
	else
	{
		cout << "Records not updated...\n";
	}

}
void updateBalance(MYSQL* conn,int id1,int bal)
{
	int qstate = 0;
	stringstream ss;
    string s_id;
	stringstream s1;
	s1 << id1;
	s1 >> s_id;
	string s_bal;
	stringstream s2;
	s2 << bal;
	s2 >> s_bal;
	ss << "UPDATE donors SET balance =  '" + s_bal + "' WHERE cnic = '" + s_id + "' ";
	string query = ss.str();
	const char* q = query.c_str();
	qstate = mysql_query(conn, q);
	if (qstate == 0)
	{
		cout << "Records updated...\n";
	}
	else
	{
		cout << "Records not updated...\n";
	}

}

void insertDonationLog(MYSQL* conn,long cn,string n,int amt)
{
	int qstate = 0;
	stringstream s1;
	string ccn,ammt;
	s1 << amt;
	s1 >> ammt;
	stringstream s2;
	s2 << cn;
	s2 >> ccn;
	stringstream ss;

	ss << "INSERT INTO donationlog (cnic, name, amount) VALUES ('" + ccn + "','" + n + "','" + ammt + "')";

	string query = ss.str();
	const char* q = query.c_str();
	qstate = mysql_query(conn, q);

	if (qstate == 0)
	{
		cout << "Records inserted...\n";
	}
	else
	{
		cout << "Records not inserted...\n";
	}
}
class Donor
{
public://change to private and make getter setter
	string name;
	long cnic;
	string username;
	string password;
	int balance;
	Register* R;

public:
	Donor(string n, long cn, string u, string p, int bal);
	int getBalance();
	string getName();
	string getUsername();
	string getPassword();
	long getCnic();
	void createDonation(MYSQL* conn,int totalfunds);
	void Deductammount(int amount);
	void displaynotifications(MYSQL* conn);
	void addBalance();

};

Donor::Donor(string n, long cn, string u, string p, int bal)
{
	name = n;
	cnic = cn;
	username = u;
	password = p;
	balance = bal;
	R = NULL;

}
void Donor::addBalance()
{
    int value;
    cout<<"Enter amount to add : ";
    cin>>value;
    while(value<1)
    {
        cout<<"Invalid amount! Enter amount to add : ";
        cin>>value;
    }
    balance += value;
    cout<<"New balance : "<<balance<<endl<<endl;
}
int Donor::getBalance()
{
	return balance;
}
string Donor::getName()
{
	return name;
}
string Donor::getUsername()
{
	return username;
}
string Donor::getPassword()
{
	return password;
}
long Donor::getCnic()
{
	return cnic;
}
void Donor::createDonation(MYSQL* conn,int totalfunds)
{
	R = new Register();
	R->makeNewDonation(cnic);
	int payAmount = 1;
	cout << "Enter amount to donate from " << balance << " :  ";
	cin >> payAmount;
	while (payAmount<1 || payAmount> balance)
	{
		cout << "Invalid amount or out of balance ! Enter again : ";
		cin >> payAmount;
	}
	Payment* P = R->makePayment(payAmount, totalfunds);

	if (P->getStatus())
	{
		Deductammount(payAmount);
		cout << "Payment successful\n";
		updateFunds(conn,totalfunds);
		cout << "Total funds = " << totalfunds << endl;
		cout << "Balance of Donor " << balance << endl;
        insertDonationLog(conn,cnic,name,P->getAmount());
        updateBalance(conn,cnic,balance);
	}

}
void Donor::Deductammount(int amount)
{
	balance -= amount;
}
void Donor::displaynotifications(MYSQL* conn)
{
    displayUserNoticaions(conn,cnic,1);
}

#endif // DONOR_H_INCLUDED
