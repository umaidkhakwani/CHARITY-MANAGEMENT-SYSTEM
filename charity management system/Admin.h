#ifndef ADMIN_H_INCLUDED
#define ADMIN_H_INCLUDED

#include "Admin.h"
#include "Donor.h"
#include "Recipient.h"
#include "Register.h"
#include "Request.h"
#include "Donation.h"
#include "Payment.h"

#include<limits>
#include <iostream>
using namespace std;
#include <windows.h>
#include <mysql.h>
#include<cstring>
#include<string>
#include <sstream>

void deleteRequest(MYSQL* conn,int id )
{
	int qstate = 0;
    string s_id;
	stringstream s1;
	s1 << id;
	s1 >> s_id;
	stringstream ss;

        ss << "DELETE FROM requestlog WHERE id = '" + s_id + "' ";
	string query = ss.str();
	const char* q = query.c_str();
	qstate = mysql_query(conn, q);

	if (qstate == 0)
	{
	    cout << "Request fulfilled\n";
		cout << "Records removed...\n";
	}
	else
	{
	    cout << "Request not fulfilled\n";
		cout << "Records not removed...\n";
	}
}

void insertNotification(MYSQL* conn,string cn,string sub,string des,int UserNo)
{
	int qstate = 0;

	stringstream ss;

	if(UserNo == 1 )
        ss << "INSERT INTO donornotifications (cnic, subject, description) VALUES ('" + cn + "','" + sub + "','" + des + "')";
    else
        ss << "INSERT INTO recipientnotifications (cnic, subject, description) VALUES ('" + cn + "','" + sub + "','" + des + "')";
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

string* idPresentinRequests(MYSQL* conn, int id)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	string s_id;
	stringstream s1;
	s1 << id;
	s1 >> s_id;
	string* sst = new string[2];
	sst[0]="0";
	stringstream ss;

	if (conn) {
		ss << "SELECT amount FROM requestlog WHERE id = '" + s_id + "' ";
		string query = ss.str();
		const char* q = query.c_str();
		int qstate = mysql_query(conn, q);
		cout<<"Here\n";
		if (!qstate) {
            res = mysql_store_result(conn);
			int count = mysql_num_fields(res);

			while (row = mysql_fetch_row(res))
			{
			    sst[0]="1";
                stringstream s2(row[0]);
                s2 >> sst[1];
			}
		}
	}
	else
	{
		cout << "Failed to fetch\n";
	}
	return sst;
}

string* idPresentinUser(MYSQL* conn, int id,int UserNo)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	string s_id;
	stringstream s1;
	s1 << id;
	s1 >> s_id;
	string* sst = new string[2];
	sst[0]="0";
	stringstream ss;

	if (conn) {
        if(UserNo == 1)
        {
            ss << "SELECT * FROM donors WHERE cnic = '" + s_id + "' ";
        }
        else if(UserNo == 2)
        {
            ss << "SELECT * FROM recipients WHERE cnic = '" + s_id + "' ";
        }
		string query = ss.str();
		const char* q = query.c_str();
		int qstate = mysql_query(conn, q);

		if (!qstate) {
            res = mysql_store_result(conn);
			int count = mysql_num_fields(res);

			while (row = mysql_fetch_row(res))
			{
			    sst[0]="1";
                stringstream s2(row[0]);
                s2 >> sst[1];
			}
		}
	}
	else
	{
		cout << "Failed to fetch\n";
	}
	return sst;
}

void updatePriority(MYSQL* conn,int id1,int prior)
{
	int qstate = 0;
	stringstream ss;
    string s_id;
	stringstream s1;
	s1 << id1;
	s1 >> s_id;
	string s_prior;
	stringstream s2;
	s2 << prior;
	s2 >> s_prior;
	ss << "UPDATE requestlog SET priority =  '" + s_prior + "' WHERE id = '" + s_id + "' ";
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
bool displayUser(MYSQL* conn,int UserNo)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	stringstream ss;
    bool rec=false;
	if (conn) {
        if(UserNo == 1)
        {
            cout<<"\tCNIC\tName\tUsername\tBalance\n";
            ss << "SELECT cnic,name,username,balance FROM donors";
        }
        else if(UserNo == 2)
        {
            cout<<"\tCNIC\tName\tUsername\n";
            ss << "SELECT cnic,name,username FROM recipients";
        }
		string query = ss.str();
		const char* q = query.c_str();
		int qstate = mysql_query(conn, q);
		if (!qstate) {
			res = mysql_store_result(conn);
			int count = mysql_num_fields(res);

			while (row = mysql_fetch_row(res))
			{
                rec = true;
				for (int i = 0; i < count; i++) {
					cout << "\t" << row[i];
				}
				cout << "\n";
			}
		}
	}
	else
	{
		cout << "Failed to fetch\n";
	}
	return rec;
}
void confirmNotification(MYSQL* conn,int UNo)
{
            if(displayUser(conn,UNo))
            {
                int id;
                string notification="",subject="",cnic_st="";
                EnterCNICAgain:
                cout<<"Enter CNIC to Send Notification : ";
                cin>>id;
                while(id<1)
                {
                    cout<<"Invalid CNIC! Enter again : ";
                    cin>>id;
                }
                string* str = idPresentinUser(conn,id,UNo);
                if(str[0]=="0")
                {
                    cout<<"Id not present \n  ";
                    goto EnterCNICAgain;
                }
                else
                {
                    cout<<"Enter Subject of notification : ";
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    cin>>subject;
                    cout<<"Enter Notification to send : ";
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    cin>>notification;
                    stringstream itos;
                    itos << id;
                    itos >> cnic_st;
                    insertNotification(conn,cnic_st,subject,notification,UNo);
                }
            }
            else
            {
                cout<<"No records found\n";
            }
}

bool sortedRecipients(MYSQL* conn)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	stringstream ss;
    bool rec=false;
	if (conn) {
		ss << "SELECT * FROM requestlog ORDER BY priority ";
		string query = ss.str();
		const char* q = query.c_str();
		int qstate = mysql_query(conn, q);
		cout<<"\tID\tCNIC\tAmount\tCategory\tPriority\tDescription\n";
		if (!qstate) {
			res = mysql_store_result(conn);
			int count = mysql_num_fields(res);

			while (row = mysql_fetch_row(res))
			{
                rec = true;
				for (int i = 0; i < count; i++) {
					cout << "\t" << row[i];
				}
				cout << "\n";
			}
		}
	}
	else
	{
		cout << "Failed to fetch\n";
	}
	return rec;
}

class Admin
{
private:
	string name;
	long cnic;
	string username;
	string password;
	Register* R;

public:
	Admin(string n, long cn, string u, string p)
	{
		name = n;
		cnic = cn;
		username = u;
		password = p;
		R = NULL;
	}
	void display()
	{
		cout << "Here are\n";
		cout << name << cnic << username << password << endl;
	}
	void manageFunds(MYSQL* conn,int& totalfunds)
	{
	    DonateAgain:
	    if(sortedRecipients(conn))
        {
            int id;
            EnterIdAgain:
            cout<<"Enter id to Donate amount : ";
            cin>>id;
            while(id<1)
            {
                cout<<"Invalid id! Enter again : ";
                cin>>id;
            }
            string* str = idPresentinRequests(conn,id);
            if(str[0]=="0")
            {
                cout<<"Id not present\n  ";
                goto EnterIdAgain;
            }
            else
            {
                int reqammt=0;
                stringstream stst;
                stst << str[1];
                stst >> reqammt;

                if(reqammt <= totalfunds)
                {
                    cout<<"Request fulfilled \n";
                    totalfunds -= reqammt;
                    updateFunds(conn,totalfunds);
                    deleteRequest(conn,id);
                }
                else
                {
                    cout<<"Not enough balance to donate to this Request.\n";
                }

            }
	    }
	    else
        {
            cout<<"No records found\n";
        }

	}
	void sendNotifications(MYSQL* conn)
	{
	    SelectReceiver:
	    int receiver =0 ;
	    cout<<"Enter 1 to send notification to Donor.\n";
	    cout<<"Enter 2 to send notification to Recipient.\n";
	    cout<<"Enter 3 to go to Main menu.\n";
	    cout<<"Enter Choice : ";
	    cin>> receiver;
	    if(receiver<1 || receiver>3)
        {
            cout<<"Invalid input! Enter again \n";
            goto SelectReceiver;
        }

	    if(receiver == 1)
	    {
            confirmNotification(conn,1);
	    }
	    else if(receiver==2)
        {
            confirmNotification(conn,2);
        }
        else
        {
            return;
        }
	}
	void priortizeRequestList(MYSQL* conn)
	{
        if(sortedRecipients(conn))
        {
            int id;
            EnterIdAgain2:
            cout<<"Enter id to set priority : ";
            cin>>id;
            while(id<1)
            {
                cout<<"Invalid id! Enter again : ";
                cin>>id;
            }
            string* str = idPresentinRequests(conn,id);
            if(str[0]=="0")
            {
                cout<<"Id not present\n  ";
                goto EnterIdAgain2;
            }
            else
            {
                int prior=100;
                cout<<"Enter priority to set from 1 to 100: ";
                cin>>prior;
                while(prior<1 || prior>100)
                {
                    cout<<"Invalid Priority! Enter again : ";
                    cin>>prior;
                }
                updatePriority(conn,id,prior);
                sortedRecipients(conn);
            }
        }
	}
};

#endif // ADMIN_H_INCLUDED
