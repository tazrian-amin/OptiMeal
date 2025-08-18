#include "../types.h"

// ============================================================================
// CROSS-PLATFORM UTILITIES
// ============================================================================

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// ============================================================================
// INPUT HANDLING
// ============================================================================

void clearInputBuffer() {
    int inputChar;
    while ((inputChar = getchar()) != '\n' && inputChar != EOF);
}

// ============================================================================
// SECURITY
// ============================================================================

void hidePassword(char* password, int maxLength) {
#ifdef _WIN32
    int charIndex = 0;
    char inputChar;

    printf("Enter password: ");
    fflush(stdout);

    while (charIndex < maxLength - 1) {
        inputChar = _getch();

        if (inputChar == '\r') { // Enter key
            break;
        } else if (inputChar == '\b') { // Backspace
            if (charIndex > 0) {
                charIndex--;
                printf("\b \b");
                fflush(stdout);
            }
        } else {
            password[charIndex++] = inputChar;
            printf("*");
            fflush(stdout);
        }
    }

    password[charIndex] = '\0';
    printf("\n");
#else
    struct termios oldTerm, newTerm;
    int charIndex = 0;
    char inputChar;

    // Get current terminal settings
    tcgetattr(STDIN_FILENO, &oldTerm);
    newTerm = oldTerm;
    
    // Disable echo and canonical mode
    newTerm.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerm);
    
    printf("Enter password: ");
    fflush(stdout);
    
    while (charIndex < maxLength - 1) {
        inputChar = getchar();
        
        if (inputChar == '\n') {
            break;
        } else if (inputChar == '\b' || inputChar == 127) { // Backspace or Delete
            if (charIndex > 0) {
                charIndex--;
                printf("\b \b"); // Move back, clear character, move back again
                fflush(stdout);
            }
        } else {
            password[charIndex] = inputChar;
            printf("*");
            fflush(stdout);
            charIndex++;
        }
    }
    
    password[charIndex] = '\0';
    printf("\n");
    
    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerm);
#endif
}

// ============================================================================
// STRING UTILITIES
// ============================================================================

void toLowerCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int strContainsIgnoreCase(const char* haystack, const char* needle) {
    if (strlen(needle) == 0) return 1;
    
    // Create temporary copies for case-insensitive comparison
    char haystackLower[100], needleLower[100];
    strcpy(haystackLower, haystack);
    strcpy(needleLower, needle);
    
    toLowerCase(haystackLower);
    toLowerCase(needleLower);
    
    return strstr(haystackLower, needleLower) != NULL;
}

// phone number validation
int isValidPhoneNumber(const char* phone) {
    if (phone == NULL || strlen(phone) == 0) {
        return 0;
    }
    
    if (strlen(phone) != 11) {
        return 0;
    }
    
    if (phone[0] != '0' || phone[1] != '1') {
        return 0;
    }
    
    for (int i = 0; phone[i]; i++) {
        if (!isdigit(phone[i])) {
            return 0;
        }
    }
    
    return 1; 
}

// ============================================================================
// SORTING UTILITIES
// ============================================================================

void sortMenuItemsByPrice(MenuItem* items, int count) {
    for (int i = 1; i < count; i++) {
        MenuItem key = items[i];
        int j = i - 1;
        
        while (j >= 0 && items[j].price > key.price) {
            items[j + 1] = items[j];
            j--;
        }
        items[j + 1] = key;
    }
}

void sortCustomersBySpending(Customer* customers, int count) {
    for (int i = 1; i < count; i++) {
        Customer key = customers[i];
        int j = i - 1;
        
        while (j >= 0 && customers[j].totalSpent < key.totalSpent) {
            customers[j + 1] = customers[j];
            j--;
        }
        customers[j + 1] = key;
    }
}

// ============================================================================
// FILE I/O UTILITIES
// ============================================================================

// ===== SAFE FILE OPENING =====
FILE* safeFileOpen(const char* filepath, const char* mode) {
    FILE* file = fopen(filepath, mode);
    if (file == NULL) {
        printf("Error: Cannot open file '%s' in mode '%s'!\n", filepath, mode);
    }
    return file;
}

// ===== SAFE FILE CLOSING =====
void safeFileClose(FILE* file) {
    if (file != NULL) {
        fclose(file);
    }
}

// ===== LINE VALIDATION =====
int isValidDataLine(const char* line) {
    return line != NULL && line[0] != '\n' && line[0] != '#';
}

// ===== ID GENERATION UTILITY =====
int generateNextIdFromFile(const char* filepath) {
    FILE* file = safeFileOpen(filepath, "r");
    if (file == NULL) return 1;
    
    int maxId = 0;
    char line[256];
    int id;
    
    while (fgets(line, sizeof(line), file)) {
        if (!isValidDataLine(line)) continue;
        
        if (sscanf(line, "%d|", &id) == 1) {
            if (id > maxId) maxId = id;
        }
    }
    
    safeFileClose(file);
    return maxId + 1;
}

// ===== FILE WRITE UTILITY =====
int writeLineToFile(const char* filepath, const char* mode, const char* format, ...) {
    FILE* file = safeFileOpen(filepath, mode);
    if (file == NULL) return 0;
    
    va_list args;
    va_start(args, format);
    int result = vfprintf(file, format, args);
    va_end(args);
    
    safeFileClose(file);
    return result > 0;
}

// ===== SPECIFIC FILE OPERATIONS =====

// Orders file operations
FILE* openOrdersFile(const char* mode) {
    return safeFileOpen("database/orders.txt", mode);
}

FILE* openRestaurantsFile(const char* mode) {
    return safeFileOpen("database/restaurants.txt", mode);
}

FILE* openMenuItemsFile(const char* mode) {
    return safeFileOpen("database/menu_items.txt", mode);
}

FILE* openCustomersFile(const char* mode) {
    return safeFileOpen("database/customers.txt", mode);
}

FILE* openUserAuthFile(const char* mode) {
    return safeFileOpen("auth/user.txt", mode);
}

FILE* openOwnerAuthFile(const char* mode) {
    return safeFileOpen("auth/owner.txt", mode);
}

FILE* openAdminAuthFile(const char* mode) {
    return safeFileOpen("auth/admin.txt", mode);
}

// ===== ID GENERATION FUNCTIONS =====
int generateNextOrderId() {
    return generateNextIdFromFile("database/orders.txt");
}

int generateNextRestaurantId() {
    return generateNextIdFromFile("database/restaurants.txt");
}

int generateNextMenuItemId() {
    return generateNextIdFromFile("database/menu_items.txt");
}