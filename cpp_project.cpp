#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <sstream>

using namespace std;

string server = "MANUAEL\\SQLEXPRESS01"; // Escape backslash
string database = "project";
string username = "sa";
string password = "ilikesql0989";

string connStr = "sqlcmd -S " + server + " -d " + database + " -U " + username + " -P " + password;

class shopping
{
private:
    int pcode;
    float price;
    float discount;
    string pname; // p stands for product
public:
    void menu();
    void administrator();
    void buyer();
    void add();
    void edit();
    void rem(); // a function to delete products
    void display();
    void receipt();
};

void shopping::menu()
{
m: // is a level we put for our goto function
    int choice;
    string email;
    string password; // this will be used for the login purpose

    cout << "\t\t\t\t____________________\n";
    cout << "\t\t\t\t                      \n";
    cout << "\t\t\t\t    MAIN MENU         \n";
    cout << "\t\t\t\t____________________\n";
    cout << "\t\t\t\t                      \n";
    cout << "\t\t\t\t 1) Administrator       \n\n";
    cout << "\t\t\t\t 2) Buyer               \n\n";
    cout << "\t\t\t\t 3) Exit                \n\n";
    cout << "\t\t\t\t Please select: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
        cout << "\n\t\t\t\t Login To Continue \n";
        cout << "\t\t\t\t Enter Email: ";
        cin >> email;
        cout << "\t\t\t\t Password: ";
        cin >> password;

        if (email == "123" && password == "123")
        {
            administrator();
        }
        else
        {
            cout << "Invalid email or password";
        }
        break;
    case 2:
        buyer();
        break;
    case 3:
        exit(0);
    default: // default case will execute if a person selects other than 1,2 or 3
        cout << "Please select from the given options";
    }

    goto m; // go to is a jump statement, so it will help us jumping to the main menu again after the completion of the switch case
}

void shopping::administrator()
{
m:
    int choice;
    cout << "\n\n\n\t\t\t Administrator Menu   \n";
    cout << "\n\t\t\t|____1) Add a product_____|";
    cout << "\n\t\t\t|                        |";
    cout << "\n\t\t\t|____2) Modify a product__|";
    cout << "\n\t\t\t|                        |";
    cout << "\n\t\t\t|____3) Delete a product__|";
    cout << "\n\t\t\t|                        |";
    cout << "\n\t\t\t|____4) Back to main menu_| \n";
    cout << "\n\t\t\t Please Enter an option: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
        add();
        break;
    case 2:
        edit();
        break;
    case 3:
        rem();
        break;
    case 4:
        menu();
        break;
    default:
        cout << "Invalid choice!";
    }
    goto m;
}

void shopping::buyer()
{
m:
    int choice;
    cout << "\n\t\t\t\tBuyer \n\n";
    cout << "\t\t\t\t1) Buy product \n\n";
    cout << "\t\t\t\t2) Go back  \n\n";
    cout << "\t\t\t\tEnter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        receipt();
        break;
    case 2:
        menu();
        break;
    default:
        cout << "Invalid choice!";
    }
    goto m;
}

void shopping::add()
{
    int token = 0;
    string line;

    cout << "\n\n\t\t\t Add a product  ";
    cout << "\n\t Product code of the product: ";
    cin >> pcode;
    cout << "\n\t Name of product: ";
    cin >> pname;
    cout << "\n\t Price of product: $ ";
    cin >> price;
    cout << "\n\t Discount on product: % ";
    cin >> discount;
    line = to_string(pcode);

    // SQL query to check if there's another product with the same product code
    string check_query = connStr + " -Q \"SELECT COUNT(*) FROM inventories WHERE product_code = " + to_string(pcode) + "\" -h-1 -W";

    // Execute the SQL query to check the count of products with the same product code
    FILE *pipe = _popen(check_query.c_str(), "r");
    if (!pipe)
    {
        cout << "Failed to run query.\n";
        return;
    }
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        result += buffer;
    }
    _pclose(pipe);

    int count = stoi(result);
    if (count > 0)
    {
        cout << "\n\n\t Duplicate product code. Please try again.";
        return; // Exit the function
    }

    // SQL query to insert the product into the database
    string insert_query = connStr + " -Q \"INSERT INTO inventories (product_code, product_name, product_price, product_discount) VALUES (" + to_string(pcode) + ", '" + pname + "', " + to_string(price) + ", " + to_string(discount) + ")\"";

    // Execute the SQL query to insert the product into the database
    system(insert_query.c_str());

    cout << "\n\n\t Product added successfully.";
}

void shopping::edit()
{
    int pkey;
    int new_code;
    float new_price, new_discount;
    string new_name, code;
    bool found = false;

    cout << "\n\t\t\t Modify the record";
    cout << "\n\t\t\t Product code: ";
    cin >> pkey;
    cin.ignore(); // Ignore leftover newline character

    // SQL query to check if the product exists with the given product code
    string check_query = connStr + " -Q \"SELECT COUNT(*) FROM inventories WHERE product_code = " + to_string(pkey) + "\" -h-1 -W";

    // Execute the SQL query to check if the product exists
    FILE *pipe = _popen(check_query.c_str(), "r");
    if (!pipe)
    {
        cout << "Failed to run query.\n";
        return;
    }
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        result += buffer;
    }
    _pclose(pipe);

    int count = stoi(result);
    if (count == 0)
    {
        cout << "\n\n\t Product not found.";
        return; // Exit the function
    }

    // Product exists, proceed with editing
    found = true;
    cout << "\n\t\t New product code: ";
    cin >> new_code;
    cin.ignore(); // Ignore leftover newline character
    cout << "\n\t\t New name of the product: ";
    getline(cin, new_name); // Use getline to allow spaces
    cout << "\n\t\t New price: $";
    cin >> new_price;
    cout << "\n\t\t New discount: %";
    cin >> new_discount;

    // SQL query to update the row in the database
    string update_query = connStr + " -Q \"UPDATE inventories SET product_code='" + to_string(new_code) + "', product_name='" + new_name + "', product_price=" + to_string(new_price) + ", product_discount=" + to_string(new_discount) + " WHERE product_code='" + to_string(pkey) + "'\"";
    system(update_query.c_str()); // Execute the SQL query

    if (found)
    {
        cout << "\n\t\t Record edited.";
    }
    else
    {
        cout << "\n\n Record not found.";
    }
}

void shopping::rem()
{
    int pkey;
    cout << "\n\n\t Delete product";
    cout << "\n\n\t Product code: ";
    cin >> pkey;

    // SQL query to check if the product exists with the given product code
    string check_query = connStr + " -Q \"SELECT COUNT(*) FROM inventories WHERE product_code = " + to_string(pkey) + "\" -h-1 -W";

    // Execute the SQL query to check if the product exists
    FILE *pipe = _popen(check_query.c_str(), "r");
    if (!pipe)
    {
        cout << "Failed to run query.\n";
        return;
    }
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        result += buffer;
    }
    _pclose(pipe);

    int count = stoi(result);
    if (count == 0)
    {
        cout << "\n\n\t Product not found.";
        return; // Exit the function
    }

    // Product exists, proceed with deletion
    string delete_query = connStr + " -Q \"DELETE FROM inventories WHERE product_code = " + to_string(pkey) + "\"";
    system(delete_query.c_str()); // Execute the SQL query

    cout << "\n\n\t Product deleted successfully.";
}

void shopping::display()
{
    // SQL query to select all products from the database
    string select_query = connStr + " -Q \"SELECT * FROM inventories\"";

    // Execute the SQL query to display the products
    system(select_query.c_str());
}

void shopping::receipt()
{
    vector<int> arrc;   // Stores the product codes
    vector<int> arrq;   // Stores the product quantities
    vector<float> arrp; // Stores the product prices
    vector<float> arrt; // Stores the total amount for each product
    char choice;
    int c = 0;
    float total = 0;

    cout << "\n\n\t\t\t\t Items \n\n";
    display();
    cout << "______________________________________________________________________________________\n";

    do
    {
        cout << "\n\n\t\t\t Enter Product code: ";
        int pcode;
        cin >> pcode;

        // SQL query to select the product details with the given product code
        string select_query = connStr + " -Q \"SELECT product_price FROM inventories WHERE product_code = " + to_string(pcode) + "\" -h-1 -W";

        // Execute the SQL query to get the product price
        FILE *pipe = _popen(select_query.c_str(), "r");
        if (!pipe)
        {
            cout << "Failed to run query.\n";
            return;
        }
        char buffer[128];
        string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != NULL)
        {
            result += buffer;
        }
        _pclose(pipe);

        if (result.empty())
        {
            cout << "\n\n\t Product not found. Please try again.";
            continue;
        }

        float price = stof(result);
        arrc.push_back(pcode); // Save the product code
        arrp.push_back(price); // Save the product price

        cout << "\n\n\t\t\t Enter the quantity: ";
        int quantity;
        cin >> quantity;
        arrq.push_back(quantity); // Save the quantity

        float amount = price * quantity;
        arrt.push_back(amount); // Save the total amount for the product

        c++;
        cout << "\n\n\t\t\t Do you want to buy another product? (y/n): ";
        cin >> choice;
    } while (choice == 'y');

    cout << "\n\n\t\t\t_______________RECEIPT_______________\n";
    cout << "\nProduct No\tProduct Name\tQuantity\tPrice\tAmount\n";

    for (int i = 0; i < c; i++)
    {
        // SQL query to select the product name with the given product code
        string select_query = connStr + " -Q \"SELECT product_name FROM inventories WHERE product_code = " + to_string(arrc[i]) + "\" -h-1 -W";

        // Execute the SQL query to get the product name
        FILE *pipe = _popen(select_query.c_str(), "r");
        if (!pipe)
        {
            cout << "Failed to run query.\n";
            return;
        }
        char buffer[512];
        string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != NULL)
        {
            result += buffer;
        }
        _pclose(pipe);

        string pname = result;
        pname.erase(pname.find_last_not_of(" \n\r\t") + 1); // Trim the trailing whitespace

        float price = arrp[i];
        int quantity = arrq[i];
        float amount = arrt[i];

        cout << arrc[i] << "\t" << pname << "\t" << quantity << "\t" << price << "\t" << amount << "\n";
        total += amount;
    }

    cout << "\n\n\t\t\t Total Amount: $" << total << "\n";
}

int main()
{
    shopping s;
    cout << "  ____  _        \n";
    cout << " | __ ) | | _   _ \n";
    cout << " |  _ \\| || | | |  \n";
    cout << " | |_)  | || |_| |\n";
    cout << " |____/ |_||_\\__,\n";
    cout << "                                                \n";
    cout << "               __  __                         \n";
    cout << "              |  \\/  | __ _ _ __ _ __ _______ \n";
    cout << "              | |\\/| | / _` | | '__   __  __\\ \n";
    cout << "              | |  | |  |(_| | ||        | |\n";
    cout << "              |_|  |_|\\ __,_| ||        | |\n";

    cout << "\n";
    cout << "Welcome to BLU Market, your one-stop destination for fresh produce, quality goods, and daily essentials. \n";
    cout << "Discover a wide range of products sourced directly from local farmers and trusted suppliers. Our market \n";
    cout << "is committed to providing you with the best shopping experience, combining great value with exceptional service. \n";
    cout << "Shop with us today and enjoy the vibrant atmosphere and friendly community that make BLU Market unique.\n";
    s.menu();
}
