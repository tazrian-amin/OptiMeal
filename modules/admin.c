#include "../types.h"

// ============================================================================
// ADMIN AUTHENTICATION
// ============================================================================

int adminLogin() {
    char username[50];
    char password[50];
    
    printf("\n=== ADMIN LOGIN ===\n");
    printf("Enter username: ");
    scanf("%s", username);
    clearInputBuffer();
    
    hidePassword(password, sizeof(password));
    
    return authenticateAdminCredentials(username, password);
}

// ============================================================================
// ADMIN MENU MANAGEMENT
// ============================================================================

void adminMenu() {
    int menuChoice;
    
    do {
        printf("\n=== ADMIN DASHBOARD ===\n");
        printf("1. Create Restaurant Owner\n");
        printf("2. View All Restaurants\n");
        printf("3. View All Customers\n");
        printf("0. Back to Main Menu\n");
        printf("========================\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &menuChoice) != 1) {
            printf("Invalid input! Please enter a number (0-3).\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        if (menuChoice < 0 || menuChoice > 3) {
            printf("Invalid choice! Please enter a number between 0 and 3.\n");
            continue;
        }
        
        switch (menuChoice) {
            case 1:
                createRestaurantOwner();
                break;
            case 2:
                viewAllRestaurantsAdmin();
                break;
            case 3:
                viewAllCustomersAdmin();
                break;
            case 0:
                printf("Returning to main menu...\n");
                break;
        }
        
        if (menuChoice != 0) {
            printf("\nPress Enter to continue...");
            getchar();
            clearScreen();
        }
        
    } while (menuChoice != 0);
}

// ============================================================================
// RESTAURANT MANAGEMENT
// ============================================================================

void createRestaurantOwner() {
    char username[50];
    char password[50];
    char restaurantName[100];
    int restaurantId;
    
    printf("\n=== CREATE RESTAURANT OWNER ===\n");
    
    printf("Enter username for restaurant owner: ");
    scanf("%s", username);
    clearInputBuffer();
    
    hidePassword(password, sizeof(password));
    
    printf("Enter restaurant name: ");
    fgets(restaurantName, sizeof(restaurantName), stdin);
    restaurantName[strcspn(restaurantName, "\n")] = 0;
    
    if (checkIfOwnerExists(username)) {
        printf("\nError: Username '%s' already exists!\n", username);
        printf("Please choose a different username.\n");
        return;
    }
    
    restaurantId = generateNextRestaurantId();
    
    if (!createNewOwnerAccount(username, password)) {
        printf("\nError: Cannot add owner credentials!\n");
        return;
    }
    
    if (!addRestaurantToDatabase(restaurantId, username, restaurantName)) {
        printf("\nError: Cannot add restaurant to database!\n");
        return;
    }
    
    printf("\nRestaurant owner created successfully!\n");
    printf("Username: %s\n", username);
    printf("Restaurant: %s (ID: %d)\n", restaurantName, restaurantId);
    printf("Status: Active (0 menu items)\n");
    printf("\nNote: The owner can now login and add menu items to their restaurant.\n");
}

void viewAllRestaurantsAdmin() {
    printf("\n=== ALL RESTAURANTS ===\n");
    printf("%-5s %-20s %-10s %-15s\n", "ID", "Name", "Status", "Menu Items");
    printf("------------------------------------------------\n");
    
    Restaurant restaurants[100];
    int count = fetchAllRestaurantsForAdmin(restaurants, 100);
    
    int totalRestaurants = 0;
    int totalMenuItems = 0;
    
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %-10s %-15d\n", 
               restaurants[i].id, 
               restaurants[i].name, 
               restaurants[i].isActive ? "Active" : "Inactive", 
               restaurants[i].menuSize);
        
        totalRestaurants++;
        totalMenuItems += restaurants[i].menuSize;
    }
    
    printf("------------------------------------------------\n");
    printf("Total Restaurants: %d\n", totalRestaurants);
    printf("Total Menu Items: %d\n", totalMenuItems);
}

// ============================================================================
// CUSTOMER MANAGEMENT
// ============================================================================

void viewAllCustomersAdmin() {
    printf("\n=== ALL CUSTOMERS ===\n");
    printf("%-15s %-20s %-15s %-15s\n", "Phone", "Name", "Current Points", "Total Spent");
    printf("----------------------------------------------------------------\n");
    
    Customer customers[100];
    int count = fetchAllCustomersForAdmin(customers, 100);
    
    int totalCustomers = 0;
    
    for (int i = 0; i < count; i++) {
        printf("%-15s %-20s %-15d $%-14.2f\n", 
            customers[i].phone, 
            customers[i].name, 
            customers[i].points, 
            customers[i].totalSpent);
        
        totalCustomers++;
    }
    
    printf("----------------------------------------------------------------\n");
    printf("Total Customers: %d\n", totalCustomers);
}