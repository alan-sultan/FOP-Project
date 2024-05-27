# BLU Market Shopping System

## Overview

This is a simple console-based shopping system for BLU Market, a fictional marketplace. The system allows administrators to manage products and buyers to purchase products. The application interacts with a SQL Server database to store and retrieve product information.

## Features

### Administrator Functions
1. **Add a product**: Add new products to the inventory.
2. **Modify a product**: Edit the details of existing products.
3. **Delete a product**: Remove products from the inventory.

### Buyer Functions
1. **Buy product**: Purchase products and generate a receipt.
2. **View products**: Display all available products.

### Main Menu
- Allows users to choose between administrator and buyer functionalities.
- Requires administrators to log in with predefined credentials.

## Prerequisites

1. **SQL Server**: Ensure you have SQL Server installed and running. The application uses `sqlcmd` to interact with the SQL Server.
2. **Database**: Create a database named `project` and a table `inventories` with the following structure:
   ```sql
   CREATE TABLE inventories (
       product_code INT PRIMARY KEY,
       product_name VARCHAR(100),
       product_price FLOAT,
       product_discount FLOAT
   );
