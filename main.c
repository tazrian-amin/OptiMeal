#include "types.h"

// ============================================================================
// PROGRAM ENTRY POINT
// ============================================================================

int main() {
    int menuChoice;
    char ownerUsername[50];
    char userPhone[12];
    
    // ============================================================================
    // MAIN PROGRAM LOOP
    // ============================================================================
    
    do {
        displayMainMenu();
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
        
        // ============================================================================
        // USER AUTHENTICATION FLOW
        // ============================================================================
        
        switch (menuChoice) {
            case 1:
                if (userLogin(userPhone)) {
                    userMenu(userPhone);
                } else {
                    printf("\nUser login failed!\n");
                    printf("Please try again.\n");
                }
                break;
                
            // ============================================================================
            // RESTAURANT OWNER AUTHENTICATION FLOW
            // ============================================================================
                
            case 2:
                if (ownerLogin(ownerUsername)) {
                    printf("\nRestaurant owner login successful!\n");
                    printf("Welcome, Restaurant Owner!\n");
                    ownerMenu(ownerUsername);
                } else {
                    printf("\nRestaurant owner login failed!\n");
                    printf("Contact the admin to register your restaurant to OptiMeal system.\n");
                }
                break;
                
            // ============================================================================
            // ADMIN AUTHENTICATION FLOW
            // ============================================================================
                
            case 3:
                if (adminLogin()) {
                    printf("\nAdmin login successful!\n");
                    printf("Welcome, Administrator!\n");
                    adminMenu();
                } else {
                    printf("\nAdmin login failed!\n");
                    printf("Please check your username and password.\n");
                }
                break;
                
            // ============================================================================
            // PROGRAM EXIT
            // ============================================================================
                
            case 0:
                printf("Thank you for using OptiMeal!\n");
                break;
        }
        
        if (menuChoice != 0) {
            printf("\nPress Enter to continue...");
            getchar();
            clearScreen();
        }
        
    } while (menuChoice != 0);
    
    return 0;
}