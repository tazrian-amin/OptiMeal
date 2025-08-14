#include "../types.h"

// ============================================================================
// MAIN INTERFACE
// ============================================================================

void displayMainMenu() {
    printf("\n=== OPTIMEAL LOGIN SYSTEM ===\n");
    printf("1. User Login\n");
    printf("2. Restaurant Owner Login\n");
    printf("3. Admin Login\n");
    printf("0. Exit\n");
    printf("==============================\n");
}

// ============================================================================
// CATEGORY DISPLAY
// ============================================================================

void displayAvailableCategories() {
    printf("1. Breakfast\n");
    printf("2. Lunch\n");
    printf("3. Snacks\n");
    printf("4. Dinner\n");
}

// ============================================================================
// ITEM DISPLAY
// ============================================================================

int displayAvailableItemsInBudget(double budget, const char* category) {
    MenuItem items[100];
    int count = fetchMenuItemsByCategory(category, budget, items, 100);
    
    if (count == 0) {
        printf("No items found within your budget and category.\n");
        return 0;
    }
    
    printf("\n%-5s %-30s %-10s %-15s\n", "ID", "Name", "Price", "Restaurant");
    printf("-------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        char restaurantName[100];
        fetchRestaurantNameById(items[i].restaurantId, restaurantName);
        printf("%-5d %-30s $%-9.2f %-15s\n", 
               items[i].id, items[i].name, items[i].price, restaurantName);
    }
    
    return count;
}

// ============================================================================
// ORDER DISPLAY
// ============================================================================

void displayOrderSummaryDetails(const char* itemName, int quantity, double total, int pointsEarned) {
    printf("\n=== ORDER SUMMARY ===\n");
    printf("Item: %s\n", itemName);
    printf("Quantity: %d\n", quantity);
    printf("Total: $%.2f\n", total);
    printf("Points earned: %d\n", pointsEarned);
    printf("====================\n");
}

// ============================================================================
// PROFILE DISPLAY
// ============================================================================

void displayUserProfileDetails(const char* phone) {
    int points = fetchCustomerPoints(phone);
    
    printf("\n=== USER PROFILE ===\n");
    printf("Phone: %s\n", phone);
    printf("Current Points: %d\n", points);
    printf("Redeemable Amount: $%.2f\n", points / 100.0);
    printf("==================\n");
}

// ============================================================================
// RESTAURANT DISPLAY
// ============================================================================

void displayRestaurantInformation(int restaurantId) {
    char restaurantName[100];
    fetchRestaurantNameById(restaurantId, restaurantName);
    
    printf("\n=== RESTAURANT INFO ===\n");
    printf("ID: %d\n", restaurantId);
    printf("Name: %s\n", restaurantName);
    printf("======================\n");
}

void displayRestaurantMenuItems(int restaurantId) {
    MenuItem items[100];
    int count = fetchRestaurantMenuItems(restaurantId, items, 100);
    
    printf("\n=== RESTAURANT MENU ===\n");
    printf("%-5s %-30s %-10s %-15s %-10s\n", "ID", "Name", "Price", "Category", "Status");
    printf("--------------------------------------------------------------------\n");
    
    if (count == 0) {
        printf("No menu items found for this restaurant.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("%-5d %-30s $%-9.2f %-15s %-10s\n", 
            items[i].id, items[i].name, items[i].price, items[i].category, 
            items[i].isAvailable ? "Available" : "Unavailable");
        }
    }
    
    printf("========================\n");
}

// ============================================================================
// MENU ITEM FILTERING
// ============================================================================

int filterMenuItemsByName(MenuItem* items, int count, const char* searchTerm, MenuItem* filteredItems) {
    int filteredCount = 0;
    
    for (int i = 0; i < count; i++) {
        char restaurantName[100];
        fetchRestaurantNameById(items[i].restaurantId, restaurantName);
        
        // Check if search term matches item name or restaurant name (case-insensitive)
        if (strContainsIgnoreCase(items[i].name, searchTerm) || 
            strContainsIgnoreCase(restaurantName, searchTerm)) {
            filteredItems[filteredCount] = items[i];
            filteredCount++;
        }
    }
    
    return filteredCount;
}