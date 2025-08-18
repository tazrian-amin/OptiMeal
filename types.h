#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

// ============================================================================
// FILE PATH CONSTANTS
// ============================================================================

#define DATABASE_PATH "database/"
#define AUTH_PATH "auth/"

#define ORDERS_FILE DATABASE_PATH "orders.txt"
#define RESTAURANTS_FILE DATABASE_PATH "restaurants.txt"
#define MENU_ITEMS_FILE DATABASE_PATH "menu_items.txt"
#define CUSTOMERS_FILE DATABASE_PATH "customers.txt"
#define MENU_ITEMS_TEMP_FILE DATABASE_PATH "menu_items_temp.txt"
#define CUSTOMERS_TEMP_FILE DATABASE_PATH "customers_temp.txt"

#define USER_AUTH_FILE AUTH_PATH "user.txt"
#define OWNER_AUTH_FILE AUTH_PATH "owner.txt"
#define ADMIN_AUTH_FILE AUTH_PATH "admin.txt"

// ============================================================================
// DATA STRUCTURES
// ============================================================================

// MenuItem struct
typedef struct {
    int id;
    int restaurantId;
    char name[100];
    double price;
    char category[50];
    int isAvailable;
} MenuItem;

// Restaurant struct
typedef struct {
    int id;
    char name[100];
    MenuItem* menu;
    int menuSize;
    int isActive;
} Restaurant;

// Customer struct
typedef struct {
    char phone[12];
    char name[100];
    int points;
    double totalSpent;
} Customer;

// Order struct
typedef struct {
    int id;
    char customerPhone[12];
    int restaurantId;
    int itemId;
    int quantity;
    double totalPrice;
    time_t createdAt;
} Order;

// ============================================================================
// MAIN PROGRAM
// ============================================================================

// Function prototypes for main.c
int main();

// ============================================================================
// AUTHENTICATION LAYER (auth.c)
// ============================================================================

// User Authentication
int authenticateUserCredentials(const char* phone, const char* name);
int checkIfUserExists(const char* phone, char* userName);
int createNewUserAccount(const char* phone, const char* name);

// Owner Authentication
int authenticateOwnerCredentials(const char* username, const char* password);
int checkIfOwnerExists(const char* username);
int createNewOwnerAccount(const char* username, const char* password);

// Admin Authentication
int authenticateAdminCredentials(const char* username, const char* password);

// ============================================================================
// USER OPERATIONS (user.c)
// ============================================================================

// User Session Management
int userLogin(char* phone);
void userMenu(const char* phone);

// User Order Operations
void placeOrder(const char* phone);
void viewUserOrders(const char* phone);

// User Profile Operations
void viewUserProfile(const char* phone);

// ============================================================================
// RESTAURANT OWNER OPERATIONS (owner.c)
// ============================================================================

// Owner Session Management
int ownerLogin(char* username);
void ownerMenu(const char* username);

// Restaurant Menu Management
void addMenuItemToRestaurant(int restaurantId);
void viewRestaurantMenu(int restaurantId);
void editMenuItemInRestaurant(int restaurantId);
void deleteMenuItemFromRestaurant(int restaurantId);

// Restaurant Information
void viewRestaurantInfo(int restaurantId);
void viewRestaurantCustomers(int restaurantId);

// ============================================================================
// ADMIN OPERATIONS (admin.c)
// ============================================================================

// Admin Session Management
int adminLogin();
void adminMenu();

// Restaurant Owner Management
void createRestaurantOwner();

// System Monitoring
void viewAllRestaurantsAdmin();
void viewAllCustomersAdmin();

// ============================================================================
// DATA ACCESS LAYER (database.c)
// ============================================================================

// ===== RESTAURANT OPERATIONS =====
// Create
int addRestaurantToDatabase(int restaurantId, const char* username, const char* restaurantName);

// Read
void fetchRestaurantNameById(int restaurantId, char* name);
int fetchRestaurantDetailsForDisplay(int restaurantId, char* name, int* isActive);
int fetchRestaurantIdByOwner(const char* username);
int fetchAllRestaurantsForAdmin(Restaurant* restaurants, int maxRestaurants);
int fetchOwnerRestaurantData(const char* username, Restaurant* restaurant);

// ===== MENU ITEM OPERATIONS =====
// Create
int addMenuItem(int restaurantId, const char* name, double price, const char* category);

// Read
void fetchMenuItemNameById(int itemId, char* name);
double fetchMenuItemPrice(int itemId);
int fetchMenuItemRestaurantId(int itemId);
int fetchMenuItemDetails(int itemId, char* name, double* price, char* category, int* available);
int fetchMenuItemsByCategory(const char* category, double budget, MenuItem* items, int maxItems);
int fetchRestaurantMenuItems(int restaurantId, MenuItem* items, int maxItems);
int fetchRestaurantMenuItemsForDisplay(int restaurantId, MenuItem* items, int maxItems);
int countMenuItems(int restaurantId);

// Update
int updateMenuItemInDatabase(int itemId, const char* name, double price, const char* category, int isAvailable);

// Delete
int deleteMenuItemFromDatabase(int itemId);

// ===== CUSTOMER OPERATIONS =====
// Create
int addCustomer(const char* phone, const char* name);

// Read
int fetchCustomerPoints(const char* phone);
int fetchCustomerData(const char* phone, Customer* customer);
int fetchAllCustomersForAdmin(Customer* customers, int maxCustomers);

// Update
int updateCustomerStatsInDatabase(const char* phone, double amount, int pointsEarned);

// ===== ORDER OPERATIONS =====
// Create
int addOrderToDatabase(int orderId, const char* phone, int restaurantId, int itemId, int quantity, double finalAmount, time_t now);

// Read
int fetchUserOrders(const char* phone, Order* orders, int maxOrders);
int fetchCustomersByRestaurant(int restaurantId, Customer* customers, int maxCustomers);

// ===== ID GENERATION =====
int generateNextOrderId();
int generateNextRestaurantId();
int generateNextMenuItemId();

// ============================================================================
// BUSINESS LOGIC LAYER (business.c)
// ============================================================================

// Order Calculations
double calculateOrderTotalAmount(int itemId, int quantity);
int calculatePointsEarnedForOrder(int itemId, int quantity);

// Order Processing
int processCustomerOrder(const char* phone, int itemId, int quantity, double finalAmount);

// Validation
int validateMenuItemForOrder(int itemId, double budget, const char* category);

// Spending Analysis
double calculateUserMonthlySpending(const char* phone);
int calculateUserMonthlyOrderCount(const char* phone);

// ============================================================================
// USER INTERFACE LAYER (ui.c)
// ============================================================================

// Main Interface
void displayMainMenu();

// Category Display
void displayAvailableCategories();

// Item Display
int filterMenuItemsByName(MenuItem* items, int count, const char* searchTerm, MenuItem* filteredItems);
int displayAvailableItemsInBudget(double budget, const char* category);

// Order Display
void displayOrderSummaryDetails(const char* itemName, int quantity, double total, int pointsEarned);

// Profile Display
void displayUserProfileDetails(const char* phone);

// Restaurant Display
void displayRestaurantInformation(int restaurantId);
void displayRestaurantMenuItems(int restaurantId);



// ============================================================================
// UTILITY LAYER (utils.c)
// ============================================================================

// Cross-Platform Utilities
void clearScreen();

// Input Handling
void clearInputBuffer();

// String Utilities
void toLowerCase(char* str);
int strContainsIgnoreCase(const char* haystack, const char* needle);
int isValidPhoneNumber(const char* phone);

// Sorting Utilities
void sortMenuItemsByPrice(MenuItem* items, int count);
void sortCustomersBySpending(Customer* customers, int count);

// Security
void hidePassword(char* password, int maxLength);

// File I/O Utilities
FILE* safeFileOpen(const char* filepath, const char* mode);
void safeFileClose(FILE* file);
int isValidDataLine(const char* line);
int generateNextIdFromFile(const char* filepath);
int writeLineToFile(const char* filepath, const char* mode, const char* format, ...);

// Specific File Operations
FILE* openOrdersFile(const char* mode);
FILE* openRestaurantsFile(const char* mode);
FILE* openMenuItemsFile(const char* mode);
FILE* openCustomersFile(const char* mode);
FILE* openUserAuthFile(const char* mode);
FILE* openOwnerAuthFile(const char* mode);
FILE* openAdminAuthFile(const char* mode);

// ID Generation Functions
int generateNextOrderId();
int generateNextRestaurantId();
int generateNextMenuItemId();

#endif // TYPES_H