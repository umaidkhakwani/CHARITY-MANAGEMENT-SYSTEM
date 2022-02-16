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

const char* hostname = "localhost";
const char* username = "root";
const char* password = "";
const char* database = "cppdb";
unsigned int port = 3306;
const char* unixsocket = NULL;
unsigned long clientflag = 0;

MYSQL* connectdatabase() {
	MYSQL* conn;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, hostname, username, password, database, port, unixsocket, clientflag);
	if (conn)
	{
		cout << "connected to the database " << endl;
		return conn;
	}
	else {

		cout << "Failed to connect to database" << endl;
		return conn;
	}

}
int fetchFunds(MYSQL* conn)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	stringstream ss;
	int ammt_st=0;
	if (conn) {
		ss << "SELECT totalfunds FROM funds";
		string query = ss.str();
		const char* q = query.c_str();
		int qstate = mysql_query(conn, q);
		if (!qstate) {
			res = mysql_store_result(conn);
			int count = mysql_num_fields(res);

			while (row = mysql_fetch_row(res))
			{
			    for (int i = 0; i < count; i++) {
                    stringstream s1(row[i]);
                        s1 >> ammt_st;
                }

            }
		}
		return ammt_st;
	}
	else
	{
		cout << "Failed to fetch\n";
	}
}

void displayDonationLog(MYSQL* conn)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	stringstream ss;
	if (conn) {
		ss << "SELECT * FROM donationlog";
		string query = ss.str();
		const char* q = query.c_str();
		int qstate = mysql_query(conn, q);
		if (!qstate) {
			res = mysql_store_result(conn);
			int count = mysql_num_fields(res);
            cout<<"ID\tCNIC\tName\tAmount\n";
			while (row = mysql_fetch_row(res))
			{
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
}

bool checkDuplicateCnic(MYSQL* conn, int id,int UserNo)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	string s_id;
	stringstream s1;
	s1 << id;
	s1 >> s_id;

	stringstream ss;

	if (conn) {
        if(UserNo == 2)
        {
            ss << "SELECT * FROM donors WHERE cnic = '" + s_id + "' ";
        }
        else if(UserNo == 1)
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
			    return true;
			}
		}
	}
	else
	{
		cout << "Failed to fetch\n";
	}
	return false;
}

bool UserSignUp(MYSQL* conn,int userNo)
{
    int qstate = 0;
	stringstream ss;
	int cnic=0;
	int bal=0;
	CnicAgain:
	cout<<"Enter CNIC : ";
	cin>>cnic;
	if(checkDuplicateCnic(conn,cnic,userNo))
    {
        cout<<"CNIC already present. Enter again : \n";
        goto CnicAgain;
    }
	string usernamedb, namedb, passworddb,cnicdb,baldb;
	stringstream s1;
	s1 << cnic;
	s1 >> cnicdb;
	cout << "Enter Name : ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin >> namedb;
	cout << "Enter User-name : ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin >> usernamedb;
	cout << "Enter password : ";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin >> passworddb;

	if(userNo==1)
    {
        ss << "INSERT INTO recipients (cnic, username, name, password) VALUES ('" + cnicdb + "','" + usernamedb + "','" + namedb + "','" + passworddb + "')";
    }
    else if(userNo==2)
    {
        cout<<"Enter Balance : ";
        cin>>bal;
        while(bal < 0 )
        {
            cout<<"Invalid Balance ! Enter again : ";
            cin>>bal;
        }
        stringstream sbal;
        sbal << bal;
        sbal >> baldb;
        ss << "INSERT INTO donors (cnic, username, name, password, balance) VALUES ('" + cnicdb + "','" + usernamedb + "','" + namedb + "','" + passworddb + "','" + baldb + "')";
    }
	string query = ss.str();
	const char* q = query.c_str();
	qstate = mysql_query(conn, q);
	if (qstate == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

string* checkValidRecipient(MYSQL* conn,string uname,string pass)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	string* st=new string[5];
	st[0]="0";
	stringstream ss;

	if (conn) {
        ss << "SELECT * FROM recipients WHERE username =  '" + uname + "' AND password = '" + pass + "' ";
        string query = ss.str();
        const char* q = query.c_str();
		int qstate = mysql_query(conn,q );
		if (!qstate) {
			res = mysql_store_result(conn);
			int count = mysql_num_fields(res);

			while (row = mysql_fetch_row(res))
			{
			    st[0]="1";
			    for (int i = 0; i < count; i++) {
					if(i<4)
					{
					    stringstream ss(row[i]);
                        ss >> st[i+1];
                    }
				}
				cout << "\n";
			}
		}
	}
	else
	{
		cout << "Failed to fetch\n";
	}
	return st;
}
string* checkValidAdmin(MYSQL* conn,string uname,string pass)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	string* st=new string[5];
	st[0]="0";
	stringstream ss;

	if (conn) {
        ss << "SELECT * FROM Admins WHERE username =  '" + uname + "' AND password = '" + pass + "' ";
        string query = ss.str();
        const char* q = query.c_str();
		int qstate = mysql_query(conn,q );
		if (!qstate) {
			res = mysql_store_result(conn);
			int count = mysql_num_fields(res);

			while (row = mysql_fetch_row(res))
			{
			    st[0]="1";
			    for (int i = 0; i < count; i++) {
					if(i<4)
					{
					    stringstream ss(row[i]);
                        ss >> st[i+1];
                    }
				}
				cout << "\n";
			}
		}
	}
	else
	{
		cout << "Failed to fetch\n";
	}
	return st;
}

string* checkValidDonor(MYSQL* conn,string uname,string pass)
{

	MYSQL_ROW row;
	MYSQL_RES* res;
	string* st=new string[6];
	st[0]="0";
	stringstream ss;

	if (conn) {
        ss << "SELECT * FROM donors WHERE username =  '" + uname + "' AND password = '" + pass + "' ";
        string query = ss.str();
        const char* q = query.c_str();
		int qstate = mysql_query(conn,q );
		if (!qstate) {
			res = mysql_store_result(conn);
			int count = mysql_num_fields(res);

			while (row = mysql_fetch_row(res))
			{
			    st[0]="1";
			    for (int i = 0; i < count; i++) {
					if(i<5)
					{
					    stringstream ss(row[i]);
                        ss >> st[i+1];
                    }
				}
				cout << "\n";
			}
		}
	}
	else
	{
		cout << "Failed to fetch\n";
	}
	return st;
}
char selectTypeOfUser()
{
    char userType = '0';
    SelectTypeAgain:
    cout<<"\n=============================================================\n";
    cout<<"Enter 1 for Admin login.\n";
    cout<<"Enter 2 for Donor login.\n";
    cout<<"Enter 3 for Recipient login.\n";
    cout<<"Enter 4 to Exit.\n";
    cout<<"=============================================================\n\t Enter Choice : ";
    cin>>userType;
    if(userType!='1' && userType!='2' && userType!='3' && userType!='4')
    {
       cout<<"Invalid Choice!. Select again \n";
       cin.ignore(numeric_limits<streamsize>::max(),'\n');
       goto SelectTypeAgain;
    }
    return userType;
}
char selectAdminChoice()
{
    char adminOption = '0';
    SelectAdmChoice:
    cout<<"\n=============================================================";
    cout<<"\nEnter 1 to Manage Funds\n";
    cout<<"Enter 2 to Priortize Requests\n";
    cout<<"Enter 3 to CheckDonationLog\n";
    cout<<"Enter 4 to Send Notifications\n";
    cout<<"Enter 5 to LogOut\n";
    cout<<"=============================================================\n\t Enter Choice : ";
    adminOption = 0;
    cin>>adminOption;

    if(adminOption!='1' && adminOption!='2' && adminOption!='3' && adminOption!='4' && adminOption!='5')
    {
        cout<<"Invalid Choice!. Select again \n";
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        goto SelectAdmChoice;
    }
    return adminOption;
}
char selectDonorChoice()
{
    char donorOption = '0';
    SelectDonChoice:
    cout<<"\n=============================================================";
    cout<<"\nEnter 1 to Donate\n";
    cout<<"Enter 2 to AddBalance\n";
    cout<<"Enter 3 to Check Notifications\n";
    cout<<"Enter 4 to LogOut\n";
    cout<<"=============================================================\n\t Enter Choice : ";
    cin>>donorOption;
    if(donorOption!='1' && donorOption!='2' && donorOption!='3' && donorOption!='4')
    {
        cout<<"Invalid Choice!. Select again \n";
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        goto SelectDonChoice;
    }
    return donorOption;
}
int selectReciChoice()
{
    char recOption = '0';
    SelectRecChoice:
    cout<<"\n=============================================================";
    cout<<"\nEnter 1 to Request\n";
    cout<<"Enter 2 to Check Notifications\n";
    cout<<"Enter 3 to LogOut\n";
    cout<<"=============================================================\n\t Enter Choice : ";
    cin>>recOption;
    if(recOption!='1' && recOption!='2' && recOption!='3')
    {
        cout<<"Invalid Choice!. Select again \n";
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        goto SelectRecChoice;
    }
    return recOption;
}
char selectLoginChoice()
{
    char loginChoice = '0';
    AgainLoginOption:
    cout<<"\n=============================================================";
    cout<<"\nEnter 1 to Sign Up\n";
    cout<<"Enter 2 to Log In\n";
    cout<<"=============================================================\n\t Enter Choice : ";
    cin>>loginChoice;
    if(loginChoice!='1' && loginChoice!='2')
    {
        cout<<"Invalid Option!. Enter again \n";
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        goto AgainLoginOption;

    }
    return loginChoice;
}
int main()
{
    MYSQL* conn = connectdatabase();
	int totalfunds = fetchFunds(conn);
    string name, username, password;

    LogOutUser:
    char userType = selectTypeOfUser();
    if(userType == '1')
    {
        Admin* admin;
        string un,pass;
        WrongAdminCredentials:
        cout<<"Username : ";
        cin>>un;
        cout<<"Password : ";
        cin>>pass;

        string* st = checkValidAdmin(conn,un,pass);
        if(st[0]=="1")
        {
            stringstream ss(st[1]);
            long cc=0;
            ss >> cc;
            Admin adm(st[2],cc,st[3],st[4]);
            admin = &adm;
        }
        else
        {
            cout<<"No record Found!\nEnter again \n";
            goto WrongAdminCredentials;
        }

        AgainAdmOption:
        char adminOption = selectAdminChoice();
        if(adminOption == '1')
        {
            admin->manageFunds(conn,totalfunds);
            cout<<totalfunds<<endl;
        }
        else if(adminOption == '2')
        {
            admin->priortizeRequestList(conn);
        }
        else if(adminOption == '3')
        {
            displayDonationLog(conn);
        }
        else if(adminOption == '4')
        {
            admin->sendNotifications(conn);
        }
        else if(adminOption == '5')
        {
            cout<<"Logging Out\n";
            goto LogOutUser;
        }
        cout<<"\n";
        goto AgainAdmOption;

    }
    else if(userType=='2')
    {
        Donor* Doni;
        SelectLoginDonor:
        char loginChoice = selectLoginChoice();
        if(loginChoice=='1')
        {
            DonorSignUpAgain:
            bool ag=UserSignUp(conn,2);
            if(ag==1)
            {
                cout<<"\nSign Up successful\n\n";
                cout<<"You can now login to the system\n";
                goto SelectLoginDonor;
            }
            else
            {
                cout<<"Sign Up unsuccessful. Try again \n";
                goto DonorSignUpAgain;
            }

        }
        else if(loginChoice=='2')
        {

            string un,pass;
            WrongDonorCredentials:
            cout<<"Username : ";
            cin>>un;
            cout<<"Password : ";
            cin>>pass;
            string* st = checkValidDonor(conn,un,pass);
            if(st[0]=="1")
            {
                stringstream ss(st[1]);
                long cc=0;
                ss >> cc;
                stringstream ss2(st[5]);
                long bb=0;
                ss2 >> bb;

                Donor dn(st[2],cc,st[3],st[4],bb);
                Doni=&dn;
                cout<<"\nLogin Successful\n";

            }
            else
            {
                cout<<"No record Found!\nEnter again \n";
                goto WrongDonorCredentials;
            }
        }
        else
            goto LogOutUser;

        AgainDonorOption:
        char donorOption = selectDonorChoice();
        if(donorOption == '1')
        {
            Doni->createDonation(conn,totalfunds);
        }
        else if(donorOption == '2')
        {
            Doni->addBalance();
        }
        else if(donorOption == '3')
        {
            Doni->displaynotifications(conn);
        }
        else if(donorOption == '4')
        {
            cout<<"Logging Out\n";
            goto LogOutUser;
        }
        cout<<"\n";
        goto AgainDonorOption;
    }
    else if(userType=='3')
    {
        Recipient* Reci;
        SelectLoginReci:
        char loginChoice = selectLoginChoice();
        if(loginChoice=='1')
        {
            ReciSignUpAgain:
            bool ag=UserSignUp(conn,1);
            if(ag==1)
            {
                cout<<"\nSign Up successful\n\n";
                cout<<"You can now login to the system\n";
                goto SelectLoginReci;
            }
            else
            {
                cout<<"Sign Up unsuccessful. Try again \n";
                goto ReciSignUpAgain;
            }

        }
        else if(loginChoice=='2')
        {

            string un,pass;
            WrongReciCredentials:
            cout<<"Username : ";
            cin>>un;
            cout<<"Password : ";
            cin>>pass;
            string* st = checkValidRecipient(conn,un,pass);
            if(st[0]=="1")
            {
                stringstream ss(st[1]);
                long cc=0;
                ss >> cc;
                Recipient rec(st[2],cc,st[3],st[4]);
                Reci= &rec;
                cout<<"\nLogin Successful\n";
            }
            else
            {
                cout<<"No record Found!\nEnter again \n";
                goto WrongReciCredentials;
            }
        }
        else
            goto LogOutUser;

        AgainRecOption:
        char recOption = selectReciChoice();

        if(recOption == '1')
        {
            Reci->createRequest(conn);
        }
        else if(recOption == '2')
        {
            Reci->displaynotifications(conn);
        }
        else if(recOption == '3')
        {
            cout<<"Logging Out\n";
            goto LogOutUser;
        }
        cout<<"\n";
        goto AgainRecOption;

    }
    else if(userType=='4')
    {
        cout<<"Exiting\n";
        return 0;
    }

}


