////////////////////////////////////////////////////////////////////////
// Project Name: Customized DBMS Using C++.
// Author: Yogeshwar Prakash Chaudhari.
// File Name: Final.cpp
////////////////////////////////////////////////////////////////////////

#include<iostream>
#include<fstream>
#include<list>
#include<string>
using namespace std;

////////////////////////////////////////////////////////////////////////
//
// Class : Employee
// Description : Handles employees data
//
////////////////////////////////////////////////////////////////////////
class Employee
{
public:
    int EmpId;
    string EmpName;
    int EmpAge;
    string EmpAddress;
    int EmpSalary;

    static int Counter;

////////////////////////////////////////////////////////////////////////
//
// Default Constructor : Employee
// Description : Handles static void counter
//
////////////////////////////////////////////////////////////////////////   
    Employee() {} 

////////////////////////////////////////////////////////////////////////
//
// Paramerized Constructor : Employee
// Description : Handles employee data
//
////////////////////////////////////////////////////////////////////////  
    Employee(string b, int c, string d, int e)
    {
        EmpId = Counter++;
        EmpName = b;
        EmpAge = c;
        EmpAddress = d;
        EmpSalary = e;
    }

////////////////////////////////////////////////////////////////////////
//
// static counter 
// Description : Add Id of employees
//
////////////////////////////////////////////////////////////////////////   
    static void init()
    {
        Counter = 1;
    }

////////////////////////////////////////////////////////////////////////
//
// Function :  WriteBackup
// Description : Write backup in binary file (config.dat)
//
//////////////////////////////////////////////////////////////////////// 
    void WriteBackup(ofstream &out)
    {
        out.write((char*)&EmpId, sizeof(EmpId));

        size_t len = EmpName.size();
        out.write((char*)&len, sizeof(len));
        out.write(EmpName.c_str(), len);

        out.write((char*)&EmpAge, sizeof(EmpAge));

        len = EmpAddress.size();
        out.write((char*)&len, sizeof(len));
        out.write(EmpAddress.c_str(), len);

        out.write((char*)&EmpSalary, sizeof(EmpSalary));
    }

////////////////////////////////////////////////////////////////////////
//
// Function :  ReadBackup
// Description : Read backup in binary file (config.dat)
//
//////////////////////////////////////////////////////////////////////// 
    void ReadBackup(ifstream &in)
    {
        in.read((char*)&EmpId, sizeof(EmpId));

        size_t len;
        in.read((char*)&len, sizeof(len));
        string buffer(len, '\0');
        in.read(&buffer[0], len);
        EmpName = buffer;

        in.read((char*)&EmpAge, sizeof(EmpAge));

        in.read((char*)&len, sizeof(len));
        buffer.resize(len);
        in.read(&buffer[0], len);
        EmpAddress = buffer;

        in.read((char*)&EmpSalary, sizeof(EmpSalary));
    }

////////////////////////////////////////////////////////////////////////
//
// Function :  display
// Description : Display the databse on the console
//
//////////////////////////////////////////////////////////////////////// 
    void display() const
    {
        cout << "Id: " << EmpId
             << " | Name: " << EmpName
             << " | Age: " << EmpAge
             << " | Address: " << EmpAddress
             << " | Salary: " << EmpSalary
             << endl;
    }
};

////////////////////////////////////////////////////////////////////////
//
// Define Counter 
// Description : Inherit class Employee to Counter
//
//////////////////////////////////////////////////////////////////////// 
int Employee::Counter = 1;

////////////////////////////////////////////////////////////////////////
//
// Class :  MarvellousDBMS
// Description : Handles querys of database
//
//////////////////////////////////////////////////////////////////////// 
class MarvellousDBMS
{
public:
////////////////////////////////////////////////////////////////////////
//
// STL LinkedList
// Description : For fetching data from the database
//
//////////////////////////////////////////////////////////////////////// 
    list<Employee> lobj;

    MarvellousDBMS()
    {
        cout<<"Marvellous DBMS started successfully..."<<endl;
    }

////////////////////////////////////////////////////////////////////////
//
// Function :  InsertIntoTable
// Query : insert into employee values(1,'Amit',21,'Pune',21000)
//
//////////////////////////////////////////////////////////////////////// 
    void InsertIntoTable(string name, int age, string address, int salary)
    {
        Employee eobj(name, age, address, salary);
        lobj.push_back(eobj);
        cout<<"[DBMS]: New record inserted successfully..."<<endl;
    }

////////////////////////////////////////////////////////////////////////
//
// Function :  DisplayTable
// Description : Display data
//
//////////////////////////////////////////////////////////////////////// 
    void DisplayTable()
    {
        if (lobj.empty())
        {
            cout << "[DBMS]: No records found!" << endl;
            return;
        }
        for (const auto &emp : lobj)
        {
            emp.display();
        }
    }

////////////////////////////////////////////////////////////////////////
//
// Function :  BackupTable
// Description : Gets backup of added querys
//
//////////////////////////////////////////////////////////////////////// 
    void BackupTable(const string &filename)
    {
        ofstream out(filename, ios::binary);
        size_t count = lobj.size();
        out.write((char*)&count, sizeof(count));

        for (auto &emp : lobj)
        {
            emp.WriteBackup(out);
        }
        out.close();
        cout << "[DBMS]: Backup created successfully!" << endl;
    }

    void RestoreTable(const string &filename)
    {
        ifstream in(filename, ios::binary);
        if (!in)
        {
            cout << "[DBMS]: No backup file found!" << endl;
            return;
        }

        size_t count = 0;
        if (!in.read((char*)&count, sizeof(count)))
        {
            cout << "[DBMS]: Backup file corrupted!" << endl;
            return;
        }

        lobj.clear();
        for (size_t i = 0; i < count; i++)
        {
            Employee e;
            try 
            {
                e.ReadBackup(in);
            } 
            catch (...) 
            {
                cout << "[DBMS]: Error while reading employee " << i+1 << endl;
                break;
            }
            lobj.push_back(e);
        }
        in.close();
        cout << "[DBMS]: Backup restored successfully!" << endl;
    }


    void SearchById(int id)
    {
        for (auto &emp : lobj)
        {
            if (emp.EmpId == id)
            {
                emp.display();
                return;
            }
        }
        cout << "[DBMS]: Employee with ID " << id << " not found!" << endl;
    }


    void SearchByName(const string &name)
    {
        for (auto &emp : lobj)
        {
            if (emp.EmpName == name)
            {
                emp.display();
                return;
            }
        }
        cout << "[DBMS]: Employee with Name " << name << " not found!" << endl;
    }

    void DeleteById(int id)
    {
        for (auto it = lobj.begin(); it != lobj.end(); ++it)
        {
            if (it->EmpId == id)
            {
                lobj.erase(it);
                cout << "[DBMS]: Employee with ID " << id << " deleted successfully!" << endl;
                return;
            }
        }
        cout << "[DBMS]: Employee with ID " << id << " not found!" << endl;
    }
};

int main()
{
    MarvellousDBMS mobj;
    mobj.RestoreTable("config.dat"); 

    int iOption = 0;

    while (iOption != 20)
    {
        cout<<"--------------------------------------------------"<<endl;
        cout<<"1  : Insert into employee"<<endl;
        cout<<"2  : Select * from employee"<<endl;
        cout<<"3  : Take a backup of table"<<endl;
        cout<<"4  : Select * from employee where EmpID = ___"<<endl;
        cout<<"5  : Select * from employee where EmpName = ___"<<endl;
        cout<<"6  : Delete from employee where EmpID = ___"<<endl;
        cout<<"20 : Terminate the DBMS"<<endl;
        cout<<"--------------------------------------------------"<<endl;
        cout<<"Please select the desired operation: ";
        cin>>iOption;

        switch (iOption)
        {
        case 1:
        {
            string name, address;
            int age, salary;
            cout << "Enter Name: "; cin >> name;
            cout << "Enter Age: "; cin >> age;
            cout << "Enter Address: "; cin >> address;
            cout << "Enter Salary: "; cin >> salary;
            mobj.InsertIntoTable(name, age, address, salary);
            break;
        }
        case 2:
            mobj.DisplayTable();
            break;
        case 3:
            mobj.BackupTable("config.dat");
            break;
        case 4:
        {
            int id;
            cout << "Enter EmpID: ";
            cin >> id;
            mobj.SearchById(id);
            break;
        }
        case 5:
        {
            string name;
            cout << "Enter Name: ";
            cin >> name;
            mobj.SearchByName(name);
            break;
        }
        case 6:
        {
            int id;
            cout << "Enter EmpID to delete: ";
            cin >> id;
            mobj.DeleteById(id);
            break;
        }
        case 20:
            cout<<"Thank you for using Marvellous DBMS"<<endl;
            break;
        default:
            cout<<"[DBMS]: Invalid option!"<<endl;
            break;
        }
    }

    return 0;
}
