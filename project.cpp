#include <iostream>
#include <vector> //stl vector
#include <fstream> //file handling
#include <algorithm> //stl
#include <stdexcept>  //exception handling
using namespace std;

//          ABSTRACT BASE CLASS 
class Person
{
protected:
    string name;

public:
    virtual void input() = 0;
    virtual void display() = 0;
    virtual ~Person() {}  //It ensures correct destruction of derived class objects.
};

//       MEDICINE CLASS 
class Medicine
{
private:
    int id;
    string name, category;
    int quantity;
    int day, month, year;

public:
    Medicine(int i, string n, string c, int q, int d, int m, int y)
    {
        if (n.empty())
            throw invalid_argument("Name cannot be empty");
        if (q < 0)
            throw invalid_argument("Quantity cannot be negative");
        if (d < 1 || d > 31 || m < 1 || m > 12)
            throw invalid_argument("Invalid date");

        id = i;
        name = n;
        category = c;
        quantity = q;
        day = d;
        month = m;
        year = y;
    }

    ~Medicine() {} //Empty because no dynamic memory is used.

    int getId() { return id; }//These allow controlled access to private members.
    string getName() { return name; } //This is encapsulation.
    string getCategory() { return category; }
    int getQuantity() { return quantity; }
    int getYear() { return year; }

    void reduceQty() //Used when receiver requests medicine.
    {
        if (quantity > 0)
            quantity--;
    }

    void updateQty(int q)
    {
        if (q < 0)
            throw invalid_argument("Invalid quantity");
        quantity = q;
    }

    void display()
    {
        cout << "ID: " << id << " | " << name
             << " | " << category
             << " | Qty: " << quantity
             << " | Exp: " << day << "/" << month << "/" << year << endl;
    }

    string toString()//This converts a Medicine object into text.
    {
        return to_string(id) + " " + name + " " + category + " " +
               to_string(quantity) + " " + to_string(day) + " " +
               to_string(month) + " " + to_string(year);
    }//Because when saving to file: the file needs text.
};

//      GLOBAL STORAGE 
vector<Medicine> meds;  //meds is made global so every function can access it.
int idCounter = 1;

//       FILE HANDLING 
void saveData()
{
    ofstream file("data.txt");
    for (auto &m : meds)
        file << m.toString() << endl;
    file.close();
}

void loadData()
{
    ifstream file("data.txt");
    int id, q, d, m, y;
    string n, c;

    while (file >> id >> n >> c >> q >> d >> m >> y)
    {
        try
        {
            meds.push_back(Medicine(id, n, c, q, d, m, y));
            idCounter = max(idCounter, id + 1);
        }
        catch (...)
        {
        }
    }
    file.close();
}

//          USER BASE 
class User : public Person
{
protected:
    string username, password;

public:
    void setLogin(string u, string p)
    {
        username = u;
        password = p;
    }

    bool login(string u, string p)
    {
        return (u == username && p == password);
    }
};

//          ADMIN 
class Admin : public User
{
public:
    void input() {}
    void display() {}

    void addMedicine()
    {
        try
        {
            string n, c;
            int q, d, m, y;

            cin.ignore();
            cout << "Name: ";
            getline(cin, n);

            cout << "Category: ";
            getline(cin, c);

            cout << "Qty: ";
            cin >> q;

            cout << "Expiry (dd mm yyyy): ";
            cin >> d >> m >> y;

            meds.push_back(Medicine(idCounter++, n, c, q, d, m, y));
            cout << "Added successfully!\n";
        }
        catch (exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

    void deleteMedicine()
    {
        int id;
        cout << "Enter ID: ";
        cin >> id;

        auto oldSize = meds.size();

        meds.erase(remove_if(meds.begin(), meds.end(),
                             [id](Medicine &m)
                             { return m.getId() == id; }),
                   meds.end());

        if (meds.size() < oldSize)
            cout << "Deleted!\n";
        else
            cout << "Medicine not found!\n";
    }

    void updateMedicine()
    {
        int id, q;
        cout << "Enter ID: ";
        cin >> id;

        for (auto &m : meds)
        {
            if (m.getId() == id)
            {
                cout << "New Qty: ";
                cin >> q;
                try
                {
                    m.updateQty(q);
                    cout << "Updated!\n";
                }
                catch (exception &e)
                {
                    cout << e.what() << endl;
                }
                return;
            }
        }
        cout << "Not found!\n";
    }
};

//          DONOR 
class Donor : public User
{
public:
    void input() {}
    void display() {}

    void donate()
    {
        try
        {
            string n, c;
            int q, d, m, y;

            cin.ignore();
            cout << "Medicine: ";
            getline(cin, n);

            cout << "Category: ";
            getline(cin, c);

            cout << "Qty: ";
            cin >> q;

            cout << "Expiry (dd mm yyyy): ";
            cin >> d >> m >> y;

            meds.push_back(Medicine(idCounter++, n, c, q, d, m, y));
            cout << "Donated!\n";
        }
        catch (exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }
};

//          RECEIVER 
class Receiver : public User
{
public:
    void input() {}
    void display() {}

    void request()
    {
        int id;
        cout << "Enter ID: ";
        cin >> id;

        for (auto &m : meds)
        {
            if (m.getId() == id && m.getQuantity() > 0)
            {
                m.reduceQty();
                cout << "Request Approved!\n";
                return;
            }
        }
        cout << "Not available!\n";
    }
};

//        BASIC FEATURES
void viewAll()
{
    for (auto &m : meds)
        m.display();
}

void searchMed()
{
    string n;
    cin.ignore();
    cout << "Search: ";
    getline(cin, n);

    for (auto &m : meds)
        if (m.getName() == n)
            m.display();
}

void filterCategory()
{
    string c;
    cin.ignore();
    cout << "Category: ";
    getline(cin, c);

    for (auto &m : meds)
        if (m.getCategory() == c)
            m.display();
}

void checkExpiry()
{
    int currentYear = 2026;
    for (auto &m : meds)
        if (m.getYear() < currentYear)
            cout << m.getName() << " expired!\n";
}

void report()
{
    cout << "Total medicines: " << meds.size() << endl;
}


int main()
{
    loadData();

    Admin admin;
    admin.setLogin("admin", "dcse");

    Donor donor;
    Receiver receiver;

    int ch;

    do
    {
        cout << "\n1.Admin\n2.Donor\n3.Receiver\n4.View\n5.Search\n6.Filter\n7.Expiry\n8.Report\n9.Exit\nChoice: ";
        cin >> ch;

        if (ch == 1)
        {
            string u, p;
            cout << "User: ";
            cin >> u;
            cout << "Pass: ";
            cin >> p;

            if (admin.login(u, p))
            {
                int a;
                cout << "1.Add 2.Delete 3.Update\n";
                cin >> a;

                if (a == 1)
                    admin.addMedicine();
                else if (a == 2)
                    admin.deleteMedicine();
                else if (a == 3)
                    admin.updateMedicine();
            }
            else
            {
                cout << "Wrong login!\n";
            }
        }
        else if (ch == 2)
            donor.donate();
        else if (ch == 3)
            receiver.request();
        else if (ch == 4)
            viewAll();
        else if (ch == 5)
            searchMed();
        else if (ch == 6)
            filterCategory();
        else if (ch == 7)
            checkExpiry();
        else if (ch == 8)
            report();

    } while (ch != 9);

    saveData();
    return 0;
}