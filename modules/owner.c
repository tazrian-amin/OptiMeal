#include "../types.h"

// ============================================================================
// OWNER AUTHENTICATION
// ============================================================================

int ownerLogin(char* username) {
    char password[50];
    
    printf("\n=== RESTAURANT OWNER LOGIN ===\n");
    printf("Enter username: ");
    scanf("%s", username);
    clearInputBuffer();
    
    hidePassword(password, sizeof(password));
    
    return authenticateOwnerCredentials(username, password);
}

// ============================================================================
// OWNER MENU MANAGEMENT
// ============================================================================

void ownerMenu(const char* username) {
    int menuChoice;
    
    int restaurantId = fetchRestaurantIdByOwner(username);
    if (restaurantId == -1) {
        printf("Error: Could not find restaurant for this owner!\n");
        return;
    }
    
    printf("Restaurant ID: %d\n", restaurantId);
    
    do {
        printf("\n=== RESTAURANT OWNER DASHBOARD ===\n");
        printf("1. Add Menu Item\n");
        printf("2. View My Menu Items\n");
        printf("3. View Restaurant Info\n");
        printf("4. Edit Menu Item\n");
        printf("5. Delete Menu Item\n");
        printf("6. My Customers\n");
        printf("0. Back to Main Menu\n");
        printf("==================================\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &menuChoice) != 1) {
            printf("Invalid input! Please enter a number (0-4).\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        if (menuChoice < 0 || menuChoice > 6) {
            printf("Invalid choice! Please enter a number between 0 and 6.\n");
            continue;
        }
        
        switch (menuChoice) {
            case 1:
                addMenuItemToRestaurant(restaurantId);
                break;
            case 2:
                viewRestaurantMenu(restaurantId);
                break;
            case 3:
                viewRestaurantInfo(restaurantId);
                break;
            case 4:
                editMenuItemInRestaurant(restaurantId);
                break;
            case 5:
                deleteMenuItemFromRestaurant(restaurantId);
                break;
            case 6:
                viewRestaurantCustomers(restaurantId);
                break;
            case 0:
                printf("Returning to main menu...\n");
                break;
        }
        
        if (menuChoice != 0) {
            printf("\nPress Enter to continue...");
            getchar();
            system("clear");
        }
        
    } while (menuChoice != 0);
}

// ============================================================================
// MENU ITEM OPERATIONS
// ============================================================================

// ===== CREATE =====
void addMenuItemToRestaurant(int restaurantId) {
    char name[100];
    double price;
    char category[50];
    int categoryChoice;
    
    printf("\n=== ADD MENU ITEM ===\n");
    
    printf("Enter item name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    
    printf("Enter item price: ");
    scanf("%lf", &price);
    clearInputBuffer();
    
    printf("\nChoose Category:\n");
    displayAvailableCategories();
    printf("Enter your choice (1-4): ");
    
    if (scanf("%d", &categoryChoice) != 1) {
        printf("Invalid input! Please enter a number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    if (categoryChoice < 1 || categoryChoice > 4) {
        printf("Invalid category choice! Please select 1-4.\n");
        return;
    }
    
    switch (categoryChoice) {
        case 1: strcpy(category, "Breakfast"); break;
        case 2: strcpy(category, "Lunch"); break;
        case 3: strcpy(category, "Snacks"); break;
        case 4: strcpy(category, "Dinner"); break;
        default: strcpy(category, "Unknown"); break;
    }
    
    if (addMenuItem(restaurantId, name, price, category)) {
        printf("\nMenu item added successfully!\n");
        printf("Item: %s\n", name);
        printf("Price: $%.2f\n", price);
        printf("Category: %s\n", category);
    } else {
        printf("\nFailed to add menu item. Please try again.\n");
    }
}

// ===== READ =====
void viewRestaurantMenu(int restaurantId) {
    printf("\n=== RESTAURANT MENU ===\n");
    printf("%-5s %-25s %-10s %-15s %-10s\n", "ID", "Name", "Price", "Category", "Available");
    printf("--------------------------------------------------------------------\n");
    
    MenuItem items[100];
    int count = fetchRestaurantMenuItemsForDisplay(restaurantId, items, 100);
    
    for (int i = 0; i < count; i++) {
        printf("%-5d %-25s $%-9.2f %-15s %-10s\n", 
               items[i].id, items[i].name, items[i].price, items[i].category, 
               items[i].isAvailable ? "Yes" : "No");
    }
    
    printf("--------------------------------------------------------------------\n");
}

// ===== UPDATE =====
void editMenuItemInRestaurant(int restaurantId) {
    int itemIdToEdit;
    char confirmation;
    
    printf("\n=== EDIT MENU ITEM ===\n");
    
    printf("\nCurrent Restaurant Menu:\n");
    viewRestaurantMenu(restaurantId);
    
    printf("\nEnter the ID of the item you want to edit: ");
    if (scanf("%d", &itemIdToEdit) != 1) {
        printf("Invalid input! Please enter a valid item ID.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    MenuItem items[100];
    int itemCount = fetchRestaurantMenuItemsForDisplay(restaurantId, items, 100);
    int itemFound = 0;
    MenuItem itemToEdit;
    
    for (int i = 0; i < itemCount; i++) {
        if (items[i].id == itemIdToEdit) {
            itemFound = 1;
            itemToEdit = items[i];
            break;
        }
    }
    
    if (!itemFound) {
        printf("Error: Item ID %d not found in your restaurant menu!\n", itemIdToEdit);
        return;
    }
    
    printf("\n=== EDITING: %s (ID: %d) ===\n", itemToEdit.name, itemToEdit.id);
    printf("Current values:\n");
    printf("Name: %s\n", itemToEdit.name);
    printf("Price: $%.2f\n", itemToEdit.price);
    printf("Category: %s\n", itemToEdit.category);
    printf("Available: %s\n", itemToEdit.isAvailable ? "Yes" : "No");
    
    char newName[100];
    double newPrice;
    char newCategory[50];
    int newAvailable;
    int categoryChoice;
    
    printf("\nEnter new values (press Enter to keep current value):\n");
    
    printf("New name [%s]: ", itemToEdit.name);
    fgets(newName, sizeof(newName), stdin);
    newName[strcspn(newName, "\n")] = 0;
    if (strlen(newName) == 0) {
        strcpy(newName, itemToEdit.name);
    }
    
    printf("New price [$%.2f]: ", itemToEdit.price);
    char priceInput[20];
    fgets(priceInput, sizeof(priceInput), stdin);
    if (strlen(priceInput) > 1) {
        newPrice = atof(priceInput);
        if (newPrice <= 0) {
            printf("Invalid price! Keeping current price.\n");
            newPrice = itemToEdit.price;
        }
    } else {
        newPrice = itemToEdit.price;
    }
    
    printf("\nChoose new category:\n");
    displayAvailableCategories();
    printf("Enter your choice (1-4) [current: %s]: ", itemToEdit.category);
    char categoryInput[20];
    fgets(categoryInput, sizeof(categoryInput), stdin);
    if (strlen(categoryInput) > 1) {
        categoryChoice = atoi(categoryInput);
        if (categoryChoice >= 1 && categoryChoice <= 4) {
            switch (categoryChoice) {
                case 1: strcpy(newCategory, "Breakfast"); break;
                case 2: strcpy(newCategory, "Lunch"); break;
                case 3: strcpy(newCategory, "Snacks"); break;
                case 4: strcpy(newCategory, "Dinner"); break;
                default: strcpy(newCategory, itemToEdit.category); break;
            }
        } else {
            printf("Invalid category choice! Keeping current category.\n");
            strcpy(newCategory, itemToEdit.category);
        }
    } else {
        strcpy(newCategory, itemToEdit.category);
    }
    
    printf("Available? (1=Yes, 0=No) [current: %s]: ", itemToEdit.isAvailable ? "Yes" : "No");
    char availableInput[20];
    fgets(availableInput, sizeof(availableInput), stdin);
    if (strlen(availableInput) > 1) {
        newAvailable = atoi(availableInput);
        if (newAvailable != 0 && newAvailable != 1) {
            printf("Invalid choice! Keeping current availability.\n");
            newAvailable = itemToEdit.isAvailable;
        }
    } else {
        newAvailable = itemToEdit.isAvailable;
    }
    
    printf("\n=== EDIT SUMMARY ===\n");
    printf("Item: %s (ID: %d)\n", itemToEdit.name, itemToEdit.id);
    printf("Name: %s → %s\n", itemToEdit.name, newName);
    printf("Price: $%.2f → $%.2f\n", itemToEdit.price, newPrice);
    printf("Category: %s → %s\n", itemToEdit.category, newCategory);
    printf("Available: %s → %s\n", 
           itemToEdit.isAvailable ? "Yes" : "No", 
           newAvailable ? "Yes" : "No");
    
    printf("\nConfirm these changes? (y/N): ");
    if (scanf("%c", &confirmation) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    if (confirmation == 'y' || confirmation == 'Y') {
        if (updateMenuItemInDatabase(itemIdToEdit, newName, newPrice, newCategory, newAvailable)) {
            printf("\nSuccessfully updated '%s'!\n", itemToEdit.name);
        } else {
            printf("\nError: Failed to update the menu item. Please try again.\n");
        }
    } else {
        printf("\nEdit cancelled. No changes were made.\n");
    }
}

// ===== DELETE =====
void deleteMenuItemFromRestaurant(int restaurantId) {
    int itemIdToDelete;
    char confirmation;
    
    printf("\n=== DELETE MENU ITEM ===\n");
    
    printf("Current Restaurant Menu:\n");
    viewRestaurantMenu(restaurantId);
    
    printf("\nEnter the ID of the item you want to delete: ");
    if (scanf("%d", &itemIdToDelete) != 1) {
        printf("Invalid input! Please enter a valid item ID.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    MenuItem items[100];
    int itemCount = fetchRestaurantMenuItemsForDisplay(restaurantId, items, 100);
    int itemFound = 0;
    char itemName[100];
    
    for (int i = 0; i < itemCount; i++) {
        if (items[i].id == itemIdToDelete) {
            itemFound = 1;
            strcpy(itemName, items[i].name);
            break;
        }
    }
    
    if (!itemFound) {
        printf("Error: Item ID %d not found in your restaurant menu!\n", itemIdToDelete);
        return;
    }
    
    printf("\nAre you sure you want to delete '%s' (ID: %d)?\n", itemName, itemIdToDelete);
    printf("This action cannot be undone! (y/N): ");
    
    if (scanf("%c", &confirmation) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();
    
    if (confirmation == 'y' || confirmation == 'Y') {
        if (deleteMenuItemFromDatabase(itemIdToDelete)) {
            printf("\nSuccessfully deleted '%s' from your menu!\n", itemName);
        } else {
            printf("\nError: Failed to delete the menu item. Please try again.\n");
        }
    } else {
        printf("\nDeletion cancelled. No changes were made.\n");
    }
}

// ============================================================================
// RESTAURANT INFORMATION
// ============================================================================

void viewRestaurantInfo(int restaurantId) {
    printf("\n=== RESTAURANT INFORMATION ===\n");
    
    char name[100];
    int isActive;
    
    if (fetchRestaurantDetailsForDisplay(restaurantId, name, &isActive)) {
        printf("Restaurant ID: %d\n", restaurantId);
        printf("Name: %s\n", name);
        printf("Status: %s\n", isActive ? "Active" : "Inactive");
        
        int menuCount = countMenuItems(restaurantId);
        printf("Total Menu Items: %d\n", menuCount);
    } else {
        printf("Error: Could not retrieve restaurant information!\n");
    }
}

// ============================================================================
// CUSTOMER MANAGEMENT
// ============================================================================

void viewRestaurantCustomers(int restaurantId) {
    printf("\n=== MY CUSTOMERS ===\n");
    
    Customer customers[100];
    int customerCount = fetchCustomersByRestaurant(restaurantId, customers, 100);
    
    if (customerCount == 0) {
        printf("No customers have ordered from your restaurant yet.\n");
        return;
    }
    
    printf("Total Customers: %d\n\n", customerCount);
    printf("%-15s %-25s %-10s %-15s\n", "Phone", "Name", "Points", "Total Spent");
    printf("--------------------------------------------------------------------\n");
    
    for (int i = 0; i < customerCount; i++) {
        printf("%-15s %-25s %-10d $%-14.2f\n", 
               customers[i].phone, 
               customers[i].name, 
               customers[i].points, 
               customers[i].totalSpent);
    }
    
    printf("--------------------------------------------------------------------\n");
    
    double totalRevenue = 0.0;
    for (int i = 0; i < customerCount; i++) {
        totalRevenue += customers[i].totalSpent;
    }
    
    printf("\n=== CUSTOMER INSIGHTS ===\n");
    printf("Total Revenue from Customers: $%.2f\n", totalRevenue);
    printf("Average Customer Value: $%.2f\n", totalRevenue / customerCount);
    
    if (customerCount > 0) {
        printf("\n=== TOP 5 CUSTOMERS BY SPENDING ===\n");
        printf("%-3s %-15s %-25s %-15s\n", "Rank", "Phone", "Name", "Total Spent");
        printf("--------------------------------------------------------------------\n");
        
        Customer sortedCustomers[100];
        memcpy(sortedCustomers, customers, customerCount * sizeof(Customer));
        
        sortCustomersBySpending(sortedCustomers, customerCount);
        
        int topCount = (customerCount < 5) ? customerCount : 5;
        for (int i = 0; i < topCount; i++) {
            printf("%-3d %-15s %-25s $%-14.2f\n", 
                   i + 1, 
                   sortedCustomers[i].phone, 
                   sortedCustomers[i].name, 
                   sortedCustomers[i].totalSpent);
        }
        printf("--------------------------------------------------------------------\n");
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}