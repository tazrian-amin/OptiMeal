#include "../types.h"

// ============================================================================
// USER AUTHENTICATION
// ============================================================================

int userLogin(char* phone) {
    char name[100];
    char existingUserName[100];
    
    printf("\n=== USER LOGIN ===\n");
    
    do {
        printf("Enter phone number (01XXXXXXXXX): ");
        scanf("%s", phone);
        clearInputBuffer();
        
        if (!isValidPhoneNumber(phone)) {
            printf("Invalid phone number format! Please enter exactly 11 digits starting with '01'.\n");
            printf("Example: 01712345678\n\n");
        }
    } while (!isValidPhoneNumber(phone));
    
    if (checkIfUserExists(phone, existingUserName)) {
        printf("\nWelcome back, %s!\n", existingUserName);
        printf("Login successful!\n");
        return 1;
    } else {
        printf("\nNew user detected!\n");
        printf("Please enter your name to complete registration:\n");
        printf("Name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        
        if (createNewUserAccount(phone, name)) {
            if (addCustomer(phone, name)) {
                printf("\nRegistration successful!\n");
                printf("Welcome to OptiMeal, %s!\n", name);
                printf("Your account has been created successfully!\n");
                printf("You are now logged in!\n");
                return 1;
            } else {
                printf("\nWarning: Account created but database entry failed!\n");
                printf("Please contact support.\n");
                return 0;
            }
        } else {
            printf("\nRegistration failed!\n");
            printf("Sorry, we couldn't create your account at this time.\n");
            printf("Please try again later.\n");
            return 0;
        }
    }
}

// ============================================================================
// USER MENU MANAGEMENT
// ============================================================================

void userMenu(const char* phone) {
    int menuChoice;
    
    do {
        printf("\n=== USER DASHBOARD ===\n");
        printf("1. Place Order\n");
        printf("2. View My Orders\n");
        printf("3. View My Profile\n");
        printf("0. Back to Main Menu\n");
        printf("======================\n");
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
                placeOrder(phone);
                break;
            case 2:
                viewUserOrders(phone);
                break;
            case 3:
                viewUserProfile(phone);
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
// ORDER OPERATIONS
// ============================================================================

void placeOrder(const char* phone) {
    double budget;
    int selectedCategoryChoice;
    int selectedItemId;
    int quantity;
    char selectedCategory[50];
    double discount = 0.0;
    int pointsRedeemed = 0;
    int willRedeemPoints = 0;
    
    printf("\n=== PLACE ORDER ===\n");
    
    double monthlySpending = calculateUserMonthlySpending(phone);
    int monthlyOrderCount = calculateUserMonthlyOrderCount(phone);
    if (monthlySpending > 0) {
        printf("⚠️  You have spent $%.2f so far this month (%d orders).\n", monthlySpending, monthlyOrderCount);
    }

    // Step 1: Get user's budget
    printf("Enter your budget for this meal: $");
    while (scanf("%lf", &budget) != 1 || budget <= 0) {
        printf("Invalid input! Please enter a positive number for your budget: $");
        clearInputBuffer();
    }
    clearInputBuffer();
    
    // Step 2: Show available categories and get choice
    printf("\n=== AVAILABLE CATEGORIES ===\n");
    displayAvailableCategories();
    
    printf("Enter your choice (1-4): ");
    while (scanf("%d", &selectedCategoryChoice) != 1 || selectedCategoryChoice < 1 || selectedCategoryChoice > 4) {
        printf("Invalid input! Please enter a number between 1 and 4: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    
    switch (selectedCategoryChoice) {
        case 1: strcpy(selectedCategory, "Breakfast"); break;
        case 2: strcpy(selectedCategory, "Lunch"); break;
        case 3: strcpy(selectedCategory, "Snacks"); break;
        case 4: strcpy(selectedCategory, "Dinner"); break;
        default: strcpy(selectedCategory, "Unknown"); break;
    }
    
    // Step 3: Show available menu items within budget and category
    printf("\n=== AVAILABLE MENU ITEMS (Within Budget: $%.2f, Category: %s) ===\n", budget, selectedCategory);
    
    MenuItem items[100];
    int availableItems = fetchMenuItemsByCategory(selectedCategory, budget, items, 100);
    
    if (availableItems == 0) {
        printf("\nNo items available. Cannot proceed with order.\n");
        return;
    }
    
    printf("\n%-5s %-30s %-10s %-15s\n", "ID", "Name", "Price", "Restaurant");
    printf("-------------------------------------------------------------\n");
    
    for (int i = 0; i < availableItems; i++) {
        char restaurantName[100];
        fetchRestaurantNameById(items[i].restaurantId, restaurantName);
        printf("%-5d %-30s $%-9.2f %-15s\n", 
               items[i].id, items[i].name, items[i].price, restaurantName);
    }
    
    char option;
    int continueOptions = 1;
    
    while (continueOptions) {
        // Show options menu
        printf("\n=== OPTIONS ===\n");
        printf("Press 's' for sorting by price (low → high)\n");
        printf("Press 'f' to filter by name or restaurant\n");
        printf("Press 'r' to reset to original list\n");
        printf("Press 'c' to continue with selection\n");
        printf("Enter your choice: ");
        
        scanf(" %c", &option);
        clearInputBuffer();
        
        if (option == 's' || option == 'S') {
            sortMenuItemsByPrice(items, availableItems);
            printf("\n=== SORTED BY PRICE (Low → High) ===\n");
            printf("%-5s %-30s %-10s %-15s\n", "ID", "Name", "Price", "Restaurant");
            printf("-------------------------------------------------------------\n");
            
            for (int i = 0; i < availableItems; i++) {
                char restaurantName[100];
                fetchRestaurantNameById(items[i].restaurantId, restaurantName);
                printf("%-5d %-30s $%-9.2f %-15s\n", 
                       items[i].id, items[i].name, items[i].price, restaurantName);
            }
        } else if (option == 'f' || option == 'F') {
            printf("Enter search term (item name or restaurant): ");
            char searchTerm[100];
            fgets(searchTerm, sizeof(searchTerm), stdin);
            searchTerm[strcspn(searchTerm, "\n")] = 0;
            
            MenuItem filteredItems[100];
            int filteredCount = filterMenuItemsByName(items, availableItems, searchTerm, filteredItems);
            
            if (filteredCount == 0) {
                printf("No items found matching '%s'. Showing all items.\n", searchTerm);
                printf("\n%-5s %-30s %-10s %-15s\n", "ID", "Name", "Price", "Restaurant");
                printf("-------------------------------------------------------------\n");
                
                for (int i = 0; i < availableItems; i++) {
                    char restaurantName[100];
                    fetchRestaurantNameById(items[i].restaurantId, restaurantName);
                    printf("%-5d %-30s $%-9.2f %-15s\n", 
                           items[i].id, items[i].name, items[i].price, restaurantName);
                }
            } else {
                printf("\n=== FILTERED RESULTS ===\n");
                printf("%-5s %-30s %-10s %-15s\n", "ID", "Name", "Price", "Restaurant");
                printf("-------------------------------------------------------------\n");
                
                for (int i = 0; i < filteredCount; i++) {
                    char restaurantName[100];
                    fetchRestaurantNameById(filteredItems[i].restaurantId, restaurantName);
                    printf("%-5d %-30s $%-9.2f %-15s\n", 
                           filteredItems[i].id, filteredItems[i].name, filteredItems[i].price, restaurantName);
                }
                
                memcpy(items, filteredItems, filteredCount * sizeof(MenuItem));
                availableItems = filteredCount;
            }
        } else if (option == 'r' || option == 'R') {
            availableItems = fetchMenuItemsByCategory(selectedCategory, budget, items, 100);
            printf("\n=== ORIGINAL LIST RESTORED ===\n");
            printf("%-5s %-30s %-10s %-15s\n", "ID", "Name", "Price", "Restaurant");
            printf("-------------------------------------------------------------\n");
            
            for (int i = 0; i < availableItems; i++) {
                char restaurantName[100];
                fetchRestaurantNameById(items[i].restaurantId, restaurantName);
                printf("%-5d %-30s $%-9.2f %-15s\n", 
                       items[i].id, items[i].name, items[i].price, restaurantName);
            }
        } else if (option == 'c' || option == 'C') {
            continueOptions = 0;
        } else {
            printf("Invalid option! Press 'Enter' to try again...");
            getchar();
            clearScreen();
            
            printf("\n=== AVAILABLE MENU ITEMS (Within Budget: $%.2f, Category: %s) ===\n", budget, selectedCategory);
            printf("\n%-5s %-30s %-10s %-15s\n", "ID", "Name", "Price", "Restaurant");
            printf("-------------------------------------------------------------\n");
            
            for (int i = 0; i < availableItems; i++) {
                char restaurantName[100];
                fetchRestaurantNameById(items[i].restaurantId, restaurantName);
                printf("%-5d %-30s $%-9.2f %-15s\n", 
                       items[i].id, items[i].name, items[i].price, restaurantName);
            }
        }
    }
    
    // Step 4: Select menu item
    printf("\nEnter the Menu Item ID to order (from the list above): ");
    while (scanf("%d", &selectedItemId) != 1) {
        printf("Invalid input! Please enter a valid Menu Item ID: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    
    int itemFound = 0;
    for (int i = 0; i < availableItems; i++) {
        if (items[i].id == selectedItemId) {
            itemFound = 1;
            break;
        }
    }
    
    if (!itemFound) {
        printf("Invalid Menu Item ID. Please select an item from the list above.\n");
        return;
    }
    
    if (!validateMenuItemForOrder(selectedItemId, budget, selectedCategory)) {
        printf("Invalid Menu Item ID or item not available within your budget/category.\n");
        return;
    }
    
    // Step 5: Get quantity
    printf("Enter quantity: ");
    while (scanf("%d", &quantity) != 1 || quantity <= 0) {
        printf("Invalid input! Please enter a positive number for quantity: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    
    // Step 6: Calculate initial total
    double initialTotal = calculateOrderTotalAmount(selectedItemId, quantity);
    double finalTotal = initialTotal;
    int pointsEarned = calculatePointsEarnedForOrder(selectedItemId, quantity);
    
    // Step 7: Point redemption and discount
    printf("\n=== POINT REDEMPTION ===\n");
    
    int currentPoints = fetchCustomerPoints(phone);
    double redeemableAmount = currentPoints / 100.0;
    
    printf("Your current points: %d\n", currentPoints);
    printf("Redeemable amount: $%.2f\n", redeemableAmount);
    printf("Order total: $%.2f\n", initialTotal);
    
    if (currentPoints >= 100 && redeemableAmount > 0) {
        printf("\nDo you want to redeem points for a discount? (y/n): ");
        char choice;
        scanf(" %c", &choice);
        clearInputBuffer();
        
        if (choice == 'y' || choice == 'Y') {
            willRedeemPoints = 1;
            
            if (redeemableAmount >= initialTotal) {
                discount = initialTotal;
                pointsRedeemed = (int)(discount * 100);
                finalTotal = 0.0;
                printf("Points redeemed: %d ($%.2f)\n", pointsRedeemed, discount);
                printf("Order is FREE! 🎉\n");
            } else {
                discount = redeemableAmount;
                pointsRedeemed = currentPoints;
                finalTotal = initialTotal - discount;
                printf("Points redeemed: %d ($%.2f)\n", pointsRedeemed, discount);
                printf("New total: $%.2f\n", finalTotal);
            }
        }
    } else {
        printf("You need at least 100 points to redeem for discounts.\n");
    }
    
    // Step 8: Final confirmation and place order
    printf("\n=== ORDER SUMMARY ===\n");
    
    char itemName[100];
    fetchMenuItemNameById(selectedItemId, itemName);
    printf("Item: %s\n", itemName);
    
    printf("Quantity: %d\n", quantity);
    printf("Initial Total: $%.2f\n", initialTotal);
    
    if (discount > 0.0) {
        printf("Discount: -$%.2f\n", discount);
    }
    
    printf("Final Total: $%.2f\n", finalTotal);
    printf("Points to be earned: %d\n", pointsEarned);
    
    printf("\nConfirm order? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (processCustomerOrder(phone, selectedItemId, quantity, finalTotal)) {
            if (willRedeemPoints && pointsRedeemed > 0) {
                if (updateCustomerStatsInDatabase(phone, 0.0, -pointsRedeemed)) {
                    printf("Points redeemed: %d points deducted from your account.\n", pointsRedeemed);
                } else {
                    printf("Warning: Order placed but points deduction failed.\n");
                }
            }
            
            printf("\nOrder placed successfully! 🎉\n");
            printf("Order ID: %d\n", generateNextOrderId() - 1);
            printf("Payable Amount: $%.2f\n", finalTotal);
            printf("Points earned: %d\n", pointsEarned);
        } else {
            printf("\nFailed to place order. Please try again.\n");
            printf("No points were deducted since the order failed.\n");
        }
    } else {
        printf("Order cancelled. No points were deducted.\n");
    }
}

// ============================================================================
// ORDER HISTORY
// ============================================================================

void viewUserOrders(const char* phone) {
    printf("\n=== MY ORDERS ===\n");
    
    printf("%-5s %-20s %-20s %-12s %-20s\n", "ID", "Restaurant", "Item", "Total", "Date & Time");
    printf("-----------------------------------------------------------------------------\n");
    
    Order orders[100];
    int orderCount = fetchUserOrders(phone, orders, 100);
    
    if (orderCount == 0) {
        printf("No orders found.\n");
        printf("-----------------------------------------------------------------------------\n");
        return;
    }
    
    for (int i = 0; i < orderCount; i++) {
        char restaurantName[100];
        fetchRestaurantNameById(orders[i].restaurantId, restaurantName);
        
        char itemName[100];
        fetchMenuItemNameById(orders[i].itemId, itemName);
        
        char dateStr[20];
        struct tm* timeinfo = localtime(&orders[i].createdAt);
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M", timeinfo);
        
        char displayRestaurant[21], displayItem[21];
        strncpy(displayRestaurant, restaurantName, 20);
        strncpy(displayItem, itemName, 20);
        displayRestaurant[20] = '\0';
        displayItem[20] = '\0';
        
        printf("%-5d %-20s %-20s $%-11.2f %-20s\n", 
        orders[i].id, displayRestaurant, displayItem, orders[i].totalPrice, dateStr);
    }
    
    printf("-----------------------------------------------------------------------------\n");
    printf("Total Orders: %d\n", orderCount);
}

// ============================================================================
// USER PROFILE
// ============================================================================

void viewUserProfile(const char* phone) {
    printf("\n=== MY PROFILE ===\n");
    
    Customer customer;
    if (!fetchCustomerData(phone, &customer)) {
        printf("Error: Cannot retrieve customer data!\n");
        return;
    }
    
    printf("Phone: %s\n", customer.phone);
    printf("Name: %s\n", customer.name);
    printf("Current Points: %d\n", customer.points);
    printf("Redeemable Amount: $%.2f\n", customer.points / 100.0);
    
    Order orders[100];
    int totalOrders = fetchUserOrders(phone, orders, 100);
    double grandTotal = 0.0;
    
    for (int i = 0; i < totalOrders; i++) {
        grandTotal += orders[i].totalPrice;
    }
    
    double monthlySpending = calculateUserMonthlySpending(phone);
    int monthlyOrderCount = calculateUserMonthlyOrderCount(phone);
    
    printf("\n=== SPENDING SUMMARY ===\n");
    printf("Total Spent (All Time): $%.2f (%d orders)\n", grandTotal, totalOrders);
    printf("Current Month: $%.2f (%d orders)\n", monthlySpending, monthlyOrderCount);
    
    if (monthlySpending > 0) {
        printf("\n=== SPENDING INSIGHTS ===\n");
        if (monthlySpending > 200.0) {
            printf("⚠️  High spending this month! Consider a smaller budget.\n");
        } else if (monthlySpending > 100.0) {
            printf("📊 Moderate spending this month.\n");
        } else {
            printf("✅ Good spending control this month!\n");
        }
        
        double avgOrderSpending = monthlySpending / monthlyOrderCount;
        printf("Average Order Spending: $%.2f\n", avgOrderSpending);
    }
}