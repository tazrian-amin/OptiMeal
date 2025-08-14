#include "../types.h"

// ============================================================================
// RESTAURANT OPERATIONS
// ============================================================================

// ===== CREATE =====
int addRestaurantToDatabase(int restaurantId, const char* username, const char* restaurantName) {
    return writeLineToFile(RESTAURANTS_FILE, "a", "%d|%s|%s|1\n", restaurantId, username, restaurantName);
}

// ===== READ =====
void fetchRestaurantNameById(int restaurantId, char* name) {
    FILE* file = fopen("database/restaurants.txt", "r");
    if (file == NULL) {
        strcpy(name, "Unknown");
        return;
    }
    
    char line[256];
    int id;
    char username[50];
    char restaurantName[100];
    int isActive;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%[^|]|%[^|]|%d", &id, username, restaurantName, &isActive) == 4) {
            if (id == restaurantId) {
                strcpy(name, restaurantName);
                fclose(file);
                return;
            }
        }
    }
    
    fclose(file);
    strcpy(name, "Unknown");
}

int fetchRestaurantDetailsForDisplay(int restaurantId, char* name, int* isActive) {
    FILE* file = openRestaurantsFile("r");
    if (file == NULL) return 0;
    
    char line[256];
    int id;
    char username[50];
    char restaurantName[100];
    int restaurantIsActive;
    
    while (fgets(line, sizeof(line), file)) {
        if (!isValidDataLine(line)) continue;
        
        if (sscanf(line, "%d|%[^|]|%[^|]|%d", &id, username, restaurantName, &restaurantIsActive) == 4) {
            if (id == restaurantId) {
                strcpy(name, restaurantName);
                *isActive = restaurantIsActive;
                safeFileClose(file);
                return 1;
            }
        }
    }
    
    safeFileClose(file);
    return 0;
}

int fetchRestaurantIdByOwner(const char* username) {
    // Read from restaurants.txt file using the new format: ID|username|Name|Name|isActive
    FILE* file = openRestaurantsFile("r");
    if (file == NULL) return -1;
    
    char line[256];
    int id;
    char fileUsername[50];
    char restaurantName[100];
    int isActive;
    
    while (fgets(line, sizeof(line), file)) {
        if (!isValidDataLine(line)) continue;
        
        if (sscanf(line, "%d|%[^|]|%[^|]|%d", &id, fileUsername, restaurantName, &isActive) == 4) {
            if (strcmp(username, fileUsername) == 0) {
                safeFileClose(file);
                return id;
            }
        }
    }
    
    safeFileClose(file);
    return -1; // Not found
}

int fetchOwnerRestaurantData(const char* username, Restaurant* restaurant) {
    // Get restaurant ID for this owner
    int restaurantId = fetchRestaurantIdByOwner(username);
    if (restaurantId == -1) return 0;
    
    // Get restaurant details from database
    FILE* file = fopen("database/restaurants.txt", "r");
    if (file == NULL) return 0;
    
    char line[256];
    int id;
    char fileUsername[50];
    char name[100];
    int isActive;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%[^|]|%[^|]|%d", &id, fileUsername, name, &isActive) == 4) {
            if (id == restaurantId) {
                restaurant->id = id;
                strcpy(restaurant->name, name);
                restaurant->isActive = isActive;
                restaurant->menuSize = countMenuItems(restaurantId);
                restaurant->menu = NULL; // For now, we don't load the full menu
                fclose(file);
                return 1;
            }
        }
    }
    
    fclose(file);
    return 0;
}

int fetchAllRestaurantsForAdmin(Restaurant* restaurants, int maxRestaurants) {
    FILE* file = fopen("database/restaurants.txt", "r");
    if (file == NULL) return 0;
    
    char line[256];
    int id;
    char username[50];
    char name[100];
    int isActive;
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < maxRestaurants) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%[^|]|%[^|]|%d", &id, username, name, &isActive) == 4) {
            restaurants[count].id = id;
            strcpy(restaurants[count].name, name);
            restaurants[count].isActive = isActive;
            restaurants[count].menuSize = countMenuItems(id);
            restaurants[count].menu = NULL;
            count++;
        }
    }
    
    fclose(file);
    return count;
}

int fetchCustomersByRestaurant(int restaurantId, Customer* customers, int maxCustomers) {
    FILE* ordersFile = fopen("database/orders.txt", "r");
    if (ordersFile == NULL) return 0;
    
    int customerCount = 0;
    char line[256];
    int orderId, itemId, quantity;
    double totalPrice;
    time_t createdAt;
    char customerPhone[12];
    int orderRestaurantId;
    char uniquePhones[100][12];
    int uniqueCount = 0;
    
    while (fgets(line, sizeof(line), ordersFile) && uniqueCount < 100) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%11s|%d|%d|%d|%lf|%ld", &orderId, customerPhone, &orderRestaurantId, &itemId, &quantity, &totalPrice, &createdAt) == 7) {
            if (orderRestaurantId == restaurantId) {
                int isDuplicate = 0;
                for (int i = 0; i < uniqueCount; i++) {
                    if (strcmp(uniquePhones[i], customerPhone) == 0) {
                        isDuplicate = 1;
                        break;
                    }
                }
                
                if (!isDuplicate) {
                    strcpy(uniquePhones[uniqueCount], customerPhone);
                    uniqueCount++;
                }
            }
        }
    }
    fclose(ordersFile);
    
    for (int i = 0; i < uniqueCount && customerCount < maxCustomers; i++) {
        if (fetchCustomerData(uniquePhones[i], &customers[customerCount])) {
            customerCount++;
        }
    }
    
    return customerCount;
}

// ============================================================================
// MENU ITEM OPERATIONS
// ============================================================================

// ===== CREATE =====
int addMenuItem(int restaurantId, const char* name, double price, const char* category) {
    FILE* file = fopen("database/menu_items.txt", "a");
    if (file == NULL) {
        return 0;
    }
    
    int nextId = generateNextMenuItemId();
    fprintf(file, "%d|%d|%s|%.2f|%s|1\n", nextId, restaurantId, name, price, category);
    fclose(file);
    
    return 1;
}

// ===== READ =====
void fetchMenuItemNameById(int itemId, char* name) {
    FILE* file = fopen("database/menu_items.txt", "r");
    if (file == NULL) {
        strcpy(name, "Unknown");
        return;
    }
    
    char line[256];
    int id;
    int restaurantId;
    char itemName[100];
    double price;
    char category[50];
    int available;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%d|%[^|]|%lf|%[^|]|%d", &id, &restaurantId, itemName, &price, category, &available) == 6) {
            if (id == itemId) {
                strcpy(name, itemName);
                fclose(file);
                return;
            }
        }
    }
    
    fclose(file);
    strcpy(name, "Unknown");
}

double fetchMenuItemPrice(int itemId) {
    FILE* file = fopen("database/menu_items.txt", "r");
    if (file == NULL) return -1;
    
    char fileLine[256];
    int fileItemId;
    int fileRestaurantId;
    char fileItemName[100];
    double fileItemPrice;
    char fileItemCategory[50];
    int fileIsAvailable;
    
    while (fgets(fileLine, sizeof(fileLine), file)) {
        if (fileLine[0] == '\n' || fileLine[0] == '#') continue;
        
        if (sscanf(fileLine, "%d|%d|%[^|]|%lf|%[^|]|%d", &fileItemId, &fileRestaurantId, fileItemName, &fileItemPrice, fileItemCategory, &fileIsAvailable) == 6) {
            if (fileItemId == itemId) {
                fclose(file);
                return fileItemPrice;
            }
        }
    }
    
    fclose(file);
    return -1;
}

int fetchMenuItemRestaurantId(int itemId) {
    FILE* file = fopen("database/menu_items.txt", "r");
    if (file == NULL) return -1;
    
    char line[256];
    int id;
    int restaurantId;
    char name[100];
    double price;
    char category[50];
    int available;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%d|%[^|]|%lf|%[^|]|%d", &id, &restaurantId, name, &price, category, &available) == 6) {
            if (id == itemId) {
                fclose(file);
                return restaurantId;
            }
        }
    }
    
    fclose(file);
    return -1;
}

int fetchMenuItemDetails(int itemId, char* name, double* price, char* category, int* available) {
    FILE* file = fopen("database/menu_items.txt", "r");
    if (file == NULL) return 0;
    
    char line[256];
    int id;
    int restaurantId;
    char itemName[100];
    double itemPrice;
    char itemCategory[50];
    int itemAvailable;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%d|%[^|]|%lf|%[^|]|%d", &id, &restaurantId, itemName, &itemPrice, itemCategory, &itemAvailable) == 6) {
            if (id == itemId) {
                strcpy(name, itemName);
                *price = itemPrice;
                strcpy(category, itemCategory);
                *available = itemAvailable;
                fclose(file);
                return 1;
            }
        }
    }
    
    fclose(file);
    return 0;
}

int fetchMenuItemsByCategory(const char* category, double budget, MenuItem* items, int maxItems) {
    FILE* file = fopen("database/menu_items.txt", "r");
    if (file == NULL) return 0;
    
    int count = 0;
    char line[256];
    int id, restaurantId, available;
    char name[100];
    double price;
    char itemCategory[50];
    
    while (fgets(line, sizeof(line), file) && count < maxItems) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%d|%[^|]|%lf|%[^|]|%d", &id, &restaurantId, name, &price, itemCategory, &available) == 6) {
            if (available && strcmp(itemCategory, category) == 0 && price <= budget) {
                items[count].id = id;
                items[count].restaurantId = restaurantId;
                strcpy(items[count].name, name);
                items[count].price = price;
                strcpy(items[count].category, itemCategory);
                items[count].isAvailable = available;
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int fetchRestaurantMenuItems(int restaurantId, MenuItem* items, int maxItems) {
    FILE* file = fopen("database/menu_items.txt", "r");
    if (file == NULL) return 0;
    
    int count = 0;
    char line[256];
    int id, itemRestaurantId, available;
    char name[100];
    double price;
    char category[50];
    
    while (fgets(line, sizeof(line), file) && count < maxItems) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%d|%[^|]|%lf|%[^|]|%d", &id, &itemRestaurantId, name, &price, category, &available) == 6) {
            if (itemRestaurantId == restaurantId) {
                items[count].id = id;
                items[count].restaurantId = itemRestaurantId;
                strcpy(items[count].name, name);
                items[count].price = price;
                strcpy(items[count].category, category);
                items[count].isAvailable = available;
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int fetchRestaurantMenuItemsForDisplay(int restaurantId, MenuItem* items, int maxItems) {
    FILE* file = fopen("database/menu_items.txt", "r");
    if (file == NULL) return 0;
    
    char line[256];
    int id;
    int itemRestaurantId;
    char name[100];
    double price;
    char category[50];
    int available;
    int count = 0;
    
    while (fgets(line, sizeof(line), file) && count < maxItems) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%d|%[^|]|%lf|%[^|]|%d", &id, &itemRestaurantId, name, &price, category, &available) == 6) {
            if (itemRestaurantId == restaurantId) {
                items[count].id = id;
                items[count].restaurantId = itemRestaurantId;
                strcpy(items[count].name, name);
                items[count].price = price;
                strcpy(items[count].category, category);
                items[count].isAvailable = available;
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

int countMenuItems(int restaurantId) {
    FILE* file = fopen("database/menu_items.txt", "r");
    if (file == NULL) {
        return 0;
    }
    
    int count = 0;
    char line[256];
    int id;
    int itemRestaurantId;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue; // Skip empty lines and comments
        
        if (sscanf(line, "%d|%d|", &id, &itemRestaurantId) == 2) {
            if (itemRestaurantId == restaurantId) {
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}

// ===== UPDATE =====
int updateMenuItemInDatabase(int itemId, const char* name, double price, const char* category, int isAvailable) {
    FILE* originalFile = fopen("database/menu_items.txt", "r");
    FILE* tempFile = fopen("database/menu_items_temp.txt", "w");
    
    if (originalFile == NULL || tempFile == NULL) {
        if (originalFile) fclose(originalFile);
        if (tempFile) fclose(tempFile);
        return 0;
    }
    
    char fileLine[256];
    int id;
    int restaurantId;
    char oldName[100];
    double oldPrice;
    char oldCategory[50];
    int oldAvailable;
    int itemUpdated = 0;
    
    while (fgets(fileLine, sizeof(fileLine), originalFile)) {
        if (fileLine[0] == '\n' || fileLine[0] == '#') {
            fputs(fileLine, tempFile);
            continue;
        }
        
        if (sscanf(fileLine, "%d|%d|%[^|]|%lf|%[^|]|%d", &id, &restaurantId, oldName, &oldPrice, oldCategory, &oldAvailable) == 6) {
            if (id == itemId) {
                fprintf(tempFile, "%d|%d|%s|%.2f|%s|%d\n", id, restaurantId, name, price, category, isAvailable);
                itemUpdated = 1;
            } else {
                fprintf(tempFile, "%d|%d|%s|%.2f|%s|%d\n", id, restaurantId, oldName, oldPrice, oldCategory, oldAvailable);
            }
        } else {
            fputs(fileLine, tempFile);
        }
    }
    
    fclose(originalFile);
    fclose(tempFile);
    
    if (itemUpdated) {
        if (remove("database/menu_items.txt") == 0) {
            if (rename("database/menu_items_temp.txt", "database/menu_items.txt") == 0) {
                return 1;
            }
        }
        rename("database/menu_items_temp.txt", "database/menu_items.txt");
        return 0;
    }
    
    remove("database/menu_items_temp.txt");
    return 0;
}

// ===== DELETE =====
int deleteMenuItemFromDatabase(int itemId) {
    FILE* originalFile = fopen("database/menu_items.txt", "r");
    FILE* tempFile = fopen("database/menu_items_temp.txt", "w");
    
    if (originalFile == NULL || tempFile == NULL) {
        if (originalFile) fclose(originalFile);
        if (tempFile) fclose(tempFile);
        return 0;
    }
    
    char fileLine[256];
    int id;
    int restaurantId;
    char name[100];
    double price;
    char category[50];
    int available;
    int itemDeleted = 0;
    
    while (fgets(fileLine, sizeof(fileLine), originalFile)) {
        if (fileLine[0] == '\n' || fileLine[0] == '#') {
            // Keep empty lines and comments
            fputs(fileLine, tempFile);
            continue;
        }
        
        if (sscanf(fileLine, "%d|%d|%[^|]|%lf|%[^|]|%d", &id, &restaurantId, name, &price, category, &available) == 6) {
            if (id != itemId) {
                // Keep all items except the one to be deleted
                fprintf(tempFile, "%d|%d|%s|%.2f|%s|%d\n", id, restaurantId, name, price, category, available);
            } else {
                itemDeleted = 1;
            }
        } else {
            // Keep lines that don't match expected format
            fputs(fileLine, tempFile);
        }
    }
    
    fclose(originalFile);
    fclose(tempFile);
    
    if (itemDeleted) {
        // Replace original file with temp file
        if (remove("database/menu_items.txt") == 0) {
            if (rename("database/menu_items_temp.txt", "database/menu_items.txt") == 0) {
                return 1; // Successfully deleted
            }
        }
        // If rename failed, try to restore original
        rename("database/menu_items_temp.txt", "database/menu_items.txt");
        return 0;
    }
    
    // Item not found, clean up temp file
    remove("database/menu_items_temp.txt");
    return 0;
}

// ============================================================================
// CUSTOMER OPERATIONS
// ============================================================================

// ===== CREATE =====
int addCustomer(const char* phone, const char* name) {
    FILE* file = fopen("database/customers.txt", "a");
    if (file == NULL) {
        printf("Error: Cannot open customers database file for writing!\n");
        return 0;
    }
    
    // Add new customer with default values (0 points, $0.00 spent)
    fprintf(file, "%s|%s|0|0.00\n", phone, name);
    fclose(file);
    
    return 1; // Successfully added to database
}

// ===== READ =====
int fetchCustomerPoints(const char* phone) {
    FILE* file = fopen("database/customers.txt", "r");
    if (file == NULL) return 0;
    
    char line[256];
    char filePhone[12];
    char name[100];
    int points;
    double totalSpent;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%[^|]|%[^|]|%d|%lf", filePhone, name, &points, &totalSpent) == 4) {
            if (strcmp(filePhone, phone) == 0) {
                fclose(file);
                return points;
            }
        }
    }
    
    fclose(file);
    return 0;
}

int fetchCustomerData(const char* phone, Customer* customer) {
    FILE* file = fopen("database/customers.txt", "r");
    if (file == NULL) return 0;
    
    char line[256];
    char filePhone[12];
    char name[100];
    int points;
    double totalSpent;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%[^|]|%[^|]|%d|%lf", filePhone, name, &points, &totalSpent) == 4) {
            if (strcmp(filePhone, phone) == 0) {
                strcpy(customer->phone, filePhone);
                strcpy(customer->name, name);
                customer->points = points;
                customer->totalSpent = totalSpent;
                fclose(file);
                return 1;
            }
        }
    }
    
    fclose(file);
    return 0;
}

int fetchAllCustomersForAdmin(Customer* customers, int maxCustomers) {
    FILE* file = fopen("database/customers.txt", "r");
    if (file == NULL) return 0;
    
    char fileLine[256];
    char filePhone[12];
    char customerName[100];
    int customerPoints;
    double customerTotalSpent;
    int count = 0;
    
    while (fgets(fileLine, sizeof(fileLine), file) && count < maxCustomers) {
        if (fileLine[0] == '\n' || fileLine[0] == '#') continue;
        
        if (sscanf(fileLine, "%[^|]|%[^|]|%d|%lf", filePhone, customerName, &customerPoints, &customerTotalSpent) == 4) {
            strcpy(customers[count].phone, filePhone);
            strcpy(customers[count].name, customerName);
            customers[count].points = customerPoints;
            customers[count].totalSpent = customerTotalSpent;
            count++;
        }
    }
    
    fclose(file);
    return count;
}

// ===== UPDATE =====
void updateCustomerPoints(const char* phone, int pointsChange) {
    // Read current customer data
    FILE* file = fopen("database/customers.txt", "r");
    if (file == NULL) {
        printf("Error: Cannot open customers database file!\n");
        return;
    }
    
    // Create temporary file for updates
    FILE* tempFile = fopen("database/customers_temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error: Cannot create temporary file!\n");
        fclose(file);
        return;
    }
    
    char line[256];
    char filePhone[12];
    char name[100];
    int points;
    double totalSpent;
    int found = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%[^|]|%[^|]|%d|%lf", filePhone, name, &points, &totalSpent) == 4) {
            if (strcmp(filePhone, phone) == 0) {
                // Update this customer's points
                points += pointsChange;
                if (points < 0) points = 0; // Ensure points don't go negative
                fprintf(tempFile, "%s|%s|%d|%.2f\n", filePhone, name, points, totalSpent);
                found = 1;
            } else {
                // Copy other customers unchanged
                fprintf(tempFile, "%s", line);
            }
        }
    }
    
    fclose(file);
    fclose(tempFile);
    
    if (found) {
        // Replace original file with updated file
        remove("database/customers.txt");
        rename("database/customers_temp.txt", "database/customers.txt");
    } else {
        // Customer not found, remove temp file
        remove("database/customers_temp.txt");
        printf("Warning: Customer not found in database!\n");
    }
}

int updateCustomerStatsInDatabase(const char* phone, double amount, int pointsEarned) {
    // Read current customer data
    FILE* file = fopen("database/customers.txt", "r");
    if (file == NULL) {
        printf("Error: Cannot open customers database file!\n");
        return 0;
    }
    
    // Create temporary file for updates
    FILE* tempFile = fopen("database/customers_temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error: Cannot create temporary file!\n");
        fclose(file);
        return 0;
    }
    
    char line[256];
    char filePhone[12];
    char name[100];
    int points;
    double totalSpent;
    int found = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%[^|]|%[^|]|%d|%lf", filePhone, name, &points, &totalSpent) == 4) {
            if (strcmp(filePhone, phone) == 0) {
                // Update this customer's stats
                points += pointsEarned;
                totalSpent += amount;
                fprintf(tempFile, "%s|%s|%d|%.2f\n", filePhone, name, points, totalSpent);
                found = 1;
            } else {
                // Copy other customers unchanged
                fprintf(tempFile, "%s", line);
            }
        }
    }
    
    fclose(file);
    fclose(tempFile);
    
    if (found) {
        // Replace original file with updated file
        remove("database/customers.txt");
        rename("database/customers_temp.txt", "database/customers.txt");
        return 1;
    } else {
        // Customer not found, remove temp file
        remove("database/customers_temp.txt");
        printf("Warning: Customer not found in database!\n");
        return 0;
    }
}

// ============================================================================
// ORDER OPERATIONS
// ============================================================================

// ===== CREATE =====
int addOrderToDatabase(int orderId, const char* phone, int restaurantId, int itemId, int quantity, double finalAmount, time_t now) {
    FILE* file = fopen("database/orders.txt", "a");
    if (file == NULL) return 0;
    
    fprintf(file, "%d|%s|%d|%d|%d|%.2f|%ld\n", 
            orderId, phone, restaurantId, itemId, quantity, finalAmount, now);
    fclose(file);
    
    return 1;
}

// ===== READ =====
int fetchUserOrders(const char* phone, Order* orders, int maxOrders) {
    FILE* file = fopen("database/orders.txt", "r");
    if (file == NULL) return 0;
    
    int count = 0;
    char line[256];
    int id, restaurantId, itemId, quantity;
    double totalPrice;
    time_t createdAt;
    
    while (fgets(line, sizeof(line), file) && count < maxOrders) {
        if (line[0] == '\n' || line[0] == '#') continue;
        
        if (sscanf(line, "%d|%11s|%d|%d|%d|%lf|%ld", &id, orders[count].customerPhone, &restaurantId, &itemId, &quantity, &totalPrice, &createdAt) == 7) {
            if (strcmp(orders[count].customerPhone, phone) == 0) {
                orders[count].id = id;
                orders[count].restaurantId = restaurantId;
                orders[count].itemId = itemId;
                orders[count].quantity = quantity;
                orders[count].totalPrice = totalPrice;
                orders[count].createdAt = createdAt;
                count++;
            }
        }
    }
    
    fclose(file);
    return count;
}