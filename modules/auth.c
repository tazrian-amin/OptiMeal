#include "../types.h"

// ============================================================================
// USER AUTHENTICATION
// ============================================================================

// ===== AUTHENTICATION =====
int authenticateUserCredentials(const char* phone, const char* name) {
    FILE* file = openUserAuthFile("r");
    if (file == NULL) return 0;
    
    char line[256];
    char filePhone[12];
    char fileName[100];
    
    while (fgets(line, sizeof(line), file)) {
        if (!isValidDataLine(line)) continue;
        
        if (sscanf(line, "%[^|]|%[^\n]", filePhone, fileName) == 2) {
            fileName[strcspn(fileName, "\n")] = 0;
            
            if (strcmp(phone, filePhone) == 0 && strcmp(name, fileName) == 0) {
                safeFileClose(file);
                return 1;
            }
        }
    }
    
    safeFileClose(file);
    return 0;
}

// ===== CHECK EXISTENCE =====
int checkIfUserExists(const char* phone, char* userName) {
    FILE* file = openUserAuthFile("r");
    if (file == NULL) return 0;
    
    char line[256];
    char filePhone[12];
    char fileName[100];
    
    while (fgets(line, sizeof(line), file)) {
        if (!isValidDataLine(line)) continue;
        
        if (sscanf(line, "%[^|]|%[^\n]", filePhone, fileName) == 2) {
            fileName[strcspn(fileName, "\n")] = 0;
            
            if (strcmp(filePhone, phone) == 0) {
                strcpy(userName, fileName);
                safeFileClose(file);
                return 1;
            }
        }
    }
    
    safeFileClose(file);
    return 0;
}

// ===== CREATE ACCOUNT =====
int createNewUserAccount(const char* phone, const char* name) {
    return writeLineToFile(USER_AUTH_FILE, "a", "%s|%s\n", phone, name);
}

// ============================================================================
// RESTAURANT OWNER AUTHENTICATION
// ============================================================================

// ===== AUTHENTICATION =====
int authenticateOwnerCredentials(const char* username, const char* password) {
    FILE* file = openOwnerAuthFile("r");
    if (file == NULL) return 0;
    
    char line[256];
    char fileUsername[50];
    char filePassword[50];
    
    while (fgets(line, sizeof(line), file)) {
        if (!isValidDataLine(line)) continue;
        
        if (sscanf(line, "%[^|]|%[^\n]", fileUsername, filePassword) == 2) {
            filePassword[strcspn(filePassword, "\n")] = 0;
            
            if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
                safeFileClose(file);
                return 1;
            }
        }
    }
    
    safeFileClose(file);
    return 0;
}

// ===== CHECK EXISTENCE =====
int checkIfOwnerExists(const char* username) {
    FILE* file = openOwnerAuthFile("r");
    if (file == NULL) return 0;
    
    char line[256];
    char fileUsername[50];
    char filePassword[50];
    
    while (fgets(line, sizeof(line), file)) {
        if (!isValidDataLine(line)) continue;
        
        if (sscanf(line, "%[^|]|%[^\n]", fileUsername, filePassword) == 2) {
            if (strcmp(username, fileUsername) == 0) {
                safeFileClose(file);
                return 1;
            }
        }
    }
    
    safeFileClose(file);
    return 0;
}

// ===== CREATE ACCOUNT =====
int createNewOwnerAccount(const char* username, const char* password) {
    return writeLineToFile(OWNER_AUTH_FILE, "a", "%s|%s\n", username, password);
}

// ============================================================================
// ADMIN AUTHENTICATION
// ============================================================================

// ===== AUTHENTICATION =====
int authenticateAdminCredentials(const char* username, const char* password) {
    FILE* file = openAdminAuthFile("r");
    if (file == NULL) return 0;
    
    char line[256];
    char fileUsername[50];
    char filePassword[50];
    
    while (fgets(line, sizeof(line), file)) {
        if (!isValidDataLine(line)) continue;
        
        if (sscanf(line, "%[^|]|%[^\n]", fileUsername, filePassword) == 2) {
            filePassword[strcspn(filePassword, "\n")] = 0;
            
            if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
                safeFileClose(file);
                return 1;
            }
        }
    }
    
    safeFileClose(file);
    return 0;
}