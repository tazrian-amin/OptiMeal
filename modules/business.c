#include "../types.h"

// ============================================================================
// ORDER CALCULATIONS
// ============================================================================

double calculateOrderTotalAmount(int itemId, int quantity) {
    double price = fetchMenuItemPrice(itemId);
    return price * quantity;
}

int calculatePointsEarnedForOrder(int itemId, int quantity) {
    double total = calculateOrderTotalAmount(itemId, quantity);
    return (int)(total * 10); // 10 points per dollar spent
}

// ============================================================================
// ORDER PROCESSING
// ============================================================================

int processCustomerOrder(const char* phone, int itemId, int quantity, double finalAmount) {
    double price = fetchMenuItemPrice(itemId);
    if (price == -1) return 0;
    
    int orderId = generateNextOrderId();
    int restaurantId = fetchMenuItemRestaurantId(itemId);
    time_t now = time(NULL);
    
    if (!addOrderToDatabase(orderId, phone, restaurantId, itemId, quantity, finalAmount, now)) {
        return 0;
    }
    
    int pointsEarned = (int)(finalAmount * 10);
    if (!updateCustomerStatsInDatabase(phone, finalAmount, pointsEarned)) {
        return 0;
    }
    
    return 1;
}

// ============================================================================
// BUSINESS VALIDATION
// ============================================================================

int validateMenuItemForOrder(int itemId, double budget, const char* category) {
    double price = fetchMenuItemPrice(itemId);
    if (price == -1) return 0;
    
    if (price > budget) return 0;
    
    char name[100];
    double itemPrice;
    char itemCategory[50];
    int available;
    
    if (fetchMenuItemDetails(itemId, name, &itemPrice, itemCategory, &available)) {
        return (available && strcmp(itemCategory, category) == 0);
    }
    
    return 0;
}

// ============================================================================
// SPENDING ANALYSIS
// ============================================================================

double calculateUserMonthlySpending(const char* phone) {
    Order orders[100];
    int totalOrders = fetchUserOrders(phone, orders, 100);
    double monthlyTotal = 0.0;
    
    // Get current time
    time_t now = time(NULL);
    struct tm* currentTime = localtime(&now);
    int currentMonth = currentTime->tm_mon;  // 0-11 (Jan=0, Dec=11)
    int currentYear = currentTime->tm_year + 1900;
    
    for (int i = 0; i < totalOrders; i++) {
        // Check if this order is from the current month
        struct tm* orderTime = localtime(&orders[i].createdAt);
        if (orderTime->tm_mon == currentMonth && orderTime->tm_year + 1900 == currentYear) {
            monthlyTotal += orders[i].totalPrice;
        }
    }
    
    return monthlyTotal;
}

int calculateUserMonthlyOrderCount(const char* phone) {
    Order orders[100];
    int totalOrders = fetchUserOrders(phone, orders, 100);
    int monthlyOrderCount = 0;
    
    // Get current time
    time_t now = time(NULL);
    struct tm* currentTime = localtime(&now);
    int currentMonth = currentTime->tm_mon;  // 0-11 (Jan=0, Dec=11)
    int currentYear = currentTime->tm_year + 1900;
    
    for (int i = 0; i < totalOrders; i++) {
        // Check if this order is from the current month
        struct tm* orderTime = localtime(&orders[i].createdAt);
        if (orderTime->tm_mon == currentMonth && orderTime->tm_year + 1900 == currentYear) {
            monthlyOrderCount++;
        }
    }
    
    return monthlyOrderCount;
}