////////////////////////////////////////////////////////////////////////
//
// Project Name: Customized DBMS Using C++
// Author: Yogeshwar Prakash Chaudhari
// File Name: Final.cpp
//
////////////////////////////////////////////////////////////////////////

#include<iostream>
#include<fstream>   // File Handling (Read + Write)
#include<list>      // Linked List
#include<string>    // String Operations

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
    Employee(string name, int age, string address, int salary)
    {
        EmpId = Counter++;
        EmpName = name;
        EmpAge = age;
        EmpAddress = address;
        EmpSalary = salary;
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
        // Write employee ID, age, and salary directly
        out.write((char*) &EmpId, sizeof(EmpId));
        out.write((char*) &EmpAge, sizeof(EmpAge));
        out.write((char*) &EmpSalary, sizeof(EmpSalary));

        /* Store the length of a string before writing the 
           string itself into a binary file */
        size_t len;

        // Write name and address
        len = EmpName.size();

        out.write((char*) &len, sizeof(len));
        out.write(EmpName.c_str(), len);    // c_str = const char*

        len = EmpAddress.size();

        out.write((char*) &len, sizeof(len));
        out.write(EmpAddress.c_str(), len);
    }

////////////////////////////////////////////////////////////////////////
//
// Function :  ReadBackup
// Description : Read backup in binary file (config.dat)
//
//////////////////////////////////////////////////////////////////////// 
    bool ReadBackup(ifstream &in)
    {
        if (!in.read((char*) &EmpId, sizeof(EmpId)))
        {
            return false;
        }

        if (!in.read((char*) &EmpAge, sizeof(EmpAge)))
        {
            return false;
        }

        if (!in.read((char*) &EmpSalary, sizeof(EmpSalary)))
        {
            return false;
        }

        size_t len = 0;

        if (!in.read((char*) &len, sizeof(len)))
        {
            return false;
        }

        EmpName.resize(len);

        if (!in.read(&EmpName[0], len))
        {
            return false;
        }

        if (!in.read((char*) &len, sizeof(len)))
        {
            return false;
        }

        EmpAddress.resize(len);

        if (!in.read(&EmpAddress[0], len))
        {
            return false;
        }

        return true;
    }

////////////////////////////////////////////////////////////////////////
//
// Function :  display
// Description : Display the databse on the console
//
//////////////////////////////////////////////////////////////////////// 
    void display() const
    {
        cout << "ID: " << EmpId
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
// Class       :  QuickDBMS
// Description : Handles querys of database
//
//////////////////////////////////////////////////////////////////////// 
class QuickDBMS
{
public:

////////////////////////////////////////////////////////////////////////
//
// STL LinkedList
// Description : For fetching data from the database
//
//////////////////////////////////////////////////////////////////////// 
    list<Employee> lobj;

    QuickDBMS()
    {
        cout << " Quick DBMS started successfully...\n";
    }

////////////////////////////////////////////////////////////////////////
//
// Function :  InsertIntoTable
// Query    : insert into employee values(1,'Amit',21,'Pune',21000)
//
//////////////////////////////////////////////////////////////////////// 
    void InsertIntoTable(string name, int age, string address, int salary)
    {
        Employee e(name, age, address, salary);

        lobj.push_back(e);

        cout << "[DBMS]: Record inserted successfully\n";
    }

////////////////////////////////////////////////////////////////////////
//
// Function    : DisplayTable
// Description : Display data
//
//////////////////////////////////////////////////////////////////////// 
    void DisplayTable()
    {
        if (lobj.empty())
        {
            cout << "[DBMS]: No records found\n";
            return;
        }

        for (auto &emp : lobj)
        {
            emp.display();
        }
    }

////////////////////////////////////////////////////////////////////////
//
// Function    : BackupTable
// Description : Gets backup of added querys
//
//////////////////////////////////////////////////////////////////////// 
    void BackupTable(const string &filename)
    {
        ofstream out(filename, ios::binary);

        if (!out)
        {
            cout << "[DBMS]: Unable to create backup file\n";
            return;
        }

        size_t count = lobj.size();
        out.write((char*)&count, sizeof(count));

        for (auto &emp : lobj)
        {
            emp.WriteBackup(out);
        }

        out.close();
        cout << "[DBMS]: Backup created successfully\n";
    }

////////////////////////////////////////////////////////////////////////
//
// Function    : RestoreTable
// Description : Restores querys
//
//////////////////////////////////////////////////////////////////////// 
    void RestoreTable(const string &filename)
    {
        ifstream in(filename, ios::binary);

        if (!in)
        {
            cout << "[DBMS]: No backup file found\n";
            return;
        }

        size_t count = 0;
        if (!in.read((char*) &count, sizeof(count)))
        {
            cout << "[DBMS]: Backup file corrupted\n";
            return;
        }

        lobj.clear();

        for (size_t i = 0; i < count; i++)
        {
            Employee e;

            if (!e.ReadBackup(in))
            {
                cout << "[DBMS]: Error while reading employee " << i + 1 << endl;
                break;
            }
            lobj.push_back(e);
        }

        
        Employee::Counter = 1;

        for (auto &emp : lobj)
        {
            if (emp.EmpId >= Employee::Counter)
                Employee::Counter = emp.EmpId + 1;
        }

        in.close();
        cout << "[DBMS]: Backup restored successfully\n";
    }

////////////////////////////////////////////////////////////////////////
//
// Function    : SearchById
// Description : Search the employee by Id
//
////////////////////////////////////////////////////////////////////////  
    void SearchById(int id)
    {
        if (id == '\0')
        {
            cout << "[DBMS]: Employee ID not found..!" << endl;
            exit(EXIT_FAILURE);
        }
        
        for (auto &emp : lobj)
        {
            if (emp.EmpId == id)
            {
                emp.display();
                return;
            }
        }
        cout << "[DBMS]: Employee with ID " 
             << id << " not found!" << endl;
    }

////////////////////////////////////////////////////////////////////////
//
// Function    : SearchByName
// Description : Search the employee by Name
//
////////////////////////////////////////////////////////////////////////  
    void SearchByName(const string &name)
    {
        bool found = false;

        for (auto &emp : lobj)
        {
            if (emp.EmpName == name)
            {
                emp.display();
                found = true;
            }
            else if (!found)
            {
                cout << "[DBMS]: Employee with Name " 
                     << name << " not found!" << endl;   
            }
            
        }
    }

////////////////////////////////////////////////////////////////////////
//
// Function    : DeleteById
// Description : Delete the employee by Id
//
//////////////////////////////////////////////////////////////////////// 
    void DeleteById(int id)
    {
        if (id == '\0')
        {
            cout << "[DBMS]: Employee ID not found..!" << endl;
            exit(EXIT_FAILURE);
        }
        

        for (auto it = lobj.begin(); it != lobj.end(); ++it)
        {
            if (it->EmpId == id)
            {
                lobj.erase(it);

                cout << "[DBMS]: Employee with ID " 
                     << id << " deleted successfully..!" << endl;

                return;
            }
        }    
        cout << "[DBMS]: Employee with ID " 
             << id << " not found!" << endl;
    }
};

////////////////////////////////////////////////////////////////////////
// 
// Entry Point Function (main)
//
//////////////////////////////////////////////////////////////////////// 
int main()
{
    QuickDBMS mobj;
    mobj.RestoreTable("config.dat");

    int option = -1;

    while (option != 0)
    {
        cout << "\n----------------------------------\n";
        cout << "1 : Insert into employee\n";
        cout << "2 : Select * from employee\n";
        cout << "3 : Backup table\n";
        cout << "4 : Search by EmpID\n";
        cout << "5 : Search by Name\n";
        cout << "6 : Delete by EmpID\n";
        cout << "0 : Exit\n";
        cout << "----------------------------------\n";
        cout << "Enter option: ";
        cin >> option;

        switch (option)
        {
        case 1:
        {
            string name, address;
            int age, salary;

            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);

            cout << "Enter Age: ";
            cin >> age;
            cin.ignore();

            cout << "Enter Address: ";
            getline(cin, address);

            cout << "Enter Salary: ";
            cin >> salary;

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
            cin.ignore();

            cout << "Enter Name: ";
            getline(cin, name);

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

        case 0:
        {
            cout << "Thank you for using Quick DBMS\n";
            break;
        }

        default:
            cout << "[DBMS]: Invalid option..!\n";
        }
    }

    return 0;
}
