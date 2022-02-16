#ifndef RECIPIENT_H_INCLUDED
#define RECIPIENT_H_INCLUDED

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

void displayUserNoticaions(MYSQL* conn,int id,int UserNo)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	string s_id;
	stringstream s1;
	s1 << id;
	s1 >> s_id;
	stringstream ss;
	bool notPresent = 1;
	if (conn) {
		if(UserNo == 1)
        {
            ss << "SELECT * FROM donornotifications WHERE cnic = '" + s_id + "' ";
        }
        else if(UserNo == 2)
        {
            ss << "SELECT * FROM recipientnotifications WHERE cnic = '" + s_id + "' ";
        }
		string query = ss.str();
		const char* q = query.c_str();
		int qstate = mysql_query(conn, q);
		cout<<"\tID\tCNIC\tSubject\tDescription\n";
		if (!qstate) {
			res = mysql_store_result(conn);
			int count = mysql_num_fields(res);

			while (row = mysql_fetch_row(res))
			{
			    notPresent = 0;
				for (int i = 0; i < count; i++) {
					cout << "\t" << row[i];
				}
				cout << "\n";
			}
		}
		if(notPresent)
        {
            cout<<"There isn't any notification\n";
        }
	}
	else
	{
		cout << "Failed to fetch\n";
	}
}

void insertRequestLog(MYSQL* conn,long cn,int amt,string cat,int prior, string desc)
{
	int qstate = 0;
	stringstream s1;
	string ccn,ammt,prr;
	s1 << amt;
	s1 >> ammt;
	stringstream s2;
	s2 << cn;
	s2 >> ccn;
	stringstream s3;
	s3 << prior;
	s3 >> prr;
	stringstream ss;

	ss << "INSERT INTO requestlog (cnic, amount, category, priority, description) VALUES ('" + ccn + "','" + ammt + "','" + cat + "','" + prr + "','" + desc + "')";

	string query = ss.str();
	const char* q = query.c_str();
	qstate = mysql_query(conn, q);

	if (qstate == 0)
	{
		cout << "Request submitted successfully\n";
	}
	else
	{
		cout << "Request not submitted...\n";
	}
}

class Recipient
{
private:
	string name;
	long cnic;
	string username;
	string password;
	Register* R;

public:
	Recipient(string n, long cn, string u, string p)
	{
		name = n;
		cnic = cn;
		username = u;
		password = p;
		R = NULL;
	}
	void displaynotifications(MYSQL* conn)
    {
        displayUserNoticaions(conn,(int)cnic,2);
    }
	void createRequest(MYSQL* conn)
	{
		R = new Register();
		Request* Req = R->makeNewRequest(cnic);
		R->selectCategory();
        insertRequestLog(conn,cnic,Req->getAmount(),Req->getCategory(),Req->getPriority(),Req->getDescription());
	}
		void changepriority(int value)
		{
			R->changePriority(value);
		}

	};

#endif // RECIPIENT_H_INCLUDED
