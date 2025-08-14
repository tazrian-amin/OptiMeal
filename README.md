# OptiMeal - Optimal Meal Ordering & Restaurant Management System

A comprehensive console-based food ordering and restaurant management system built in C, designed to connect customers, restaurant owners, and administrators in a unified platform.

## 🏗️ System Architecture

OptiMeal follows a modular, layered architecture pattern for maintainability and scalability:

```
OptiMeal/
├── main.c               # Program entry point & main menu navigation
├── types.h              # Data structures & function prototypes
├── modules/             # Core functionality modules
│   ├── auth.c           # Authentication & user management
│   ├── user.c           # Customer operations
│   ├── owner.c          # Restaurant owner operations
│   ├── admin.c          # System administration
│   ├── database.c       # Data persistence layer
│   ├── business.c       # Business logic & calculations
│   ├── ui.c             # User interface & display
│   └── utils.c          # Utility functions & helpers
├── database/            # Data storage (flat files)
│   ├── restaurants.txt  # Restaurant information
│   ├── menu_items.txt   # Food menu items
│   ├── customers.txt    # Customer profiles
│   └── orders.txt       # Order transactions
└── auth/                # Authentication credentials
    ├── admin.txt        # Admin credentials
    ├── owner.txt        # Restaurant owner credentials
    └── user.txt         # User credentials
```

### Architecture Layers

1. **Presentation Layer** (`ui.c`, `main.c`)
   - User interface and menu navigation
   - Input/output handling and display formatting

2. **Business Logic Layer** (`business.c`, `user.c`, `owner.c`, `admin.c`)
   - Core application logic
   - Order processing and calculations
   - User role-specific operations

3. **Data Access Layer** (`database.c`)
   - File I/O operations
   - Data persistence and retrieval
   - Database operations abstraction

4. **Authentication Layer** (`auth.c`)
   - User verification and account management
   - Security and access control

5. **Utility Layer** (`utils.c`)
   - Helper functions and common operations
   - Input validation and sanitization

## 🚀 Features

### Customer Features
- **Smart Ordering System**
  - Browse menu items by category (Breakfast, Lunch, Snacks, Dinner)
  - Budget-aware filtering and recommendations
  - Real-time menu availability checking
  
- **User Management**
  - Phone number-based authentication
  - Automatic account creation for new users
  - Profile management and order history
  
- **Loyalty Program**
  - Points earned on every purchase
  - Redeemable rewards and discounts
  - Spending analytics and insights

### Restaurant Owner Features
- **Menu Management**
  - Add, view, edit, and delete menu items
  - Set pricing and categorize food items
  - Manage item availability status
  
- **Business Analytics**
  - View customer base and spending patterns
  - Monitor restaurant performance
  - Track order volume and revenue

### Administrator Features
- **System Management**
  - Create and manage restaurant owner accounts
  - Register new restaurants
  - Monitor system-wide activity
  
- **Oversight & Analytics**
  - View all restaurants and their status
  - Customer data analysis
  - System performance monitoring

## 🍽️ Food Categories & Menu System

The system supports a comprehensive range of food categories:

| Category | Examples |
|----------|----------|
| **Breakfast** | Pancakes, Egg Sandwiches, French Toast, Hot Tea |
| **Lunch** | Pizzas, Noodles, Salads, Curries, Sushi |
| **Dinner** | Burgers, Tacos, Biryani, Quesadillas |
| **Snacks** | Fries, Wings, Appetizers, Beverages |
| **Appetizers** | Spring Rolls, Garlic Bread, Edamame |

## 💾 Data Structure

### Core Data Models

```c
// Menu Item Structure
typedef struct {
    int id;
    int restaurantId;
    char name[100];
    double price;
    char category[50];
    int isAvailable;
} MenuItem;

// Restaurant Structure
typedef struct {
    int id;
    char name[100];
    MenuItem* menu;
    int menuSize;
    int isActive;
} Restaurant;

// Customer Structure
typedef struct {
    char phone[12];
    char name[100];
    int points;
    double totalSpent;
} Customer;

// Order Structure
typedef struct {
    int id;
    char customerPhone[12];
    int restaurantId;
    int itemId;
    int quantity;
    double totalPrice;
    time_t createdAt;
} Order;
```

### Database Schema

The application uses flat-file storage with the following format:

- **restaurants.txt**: `restaurant_id|username|restaurant_name|is_active`
- **menu_items.txt**: `item_id|restaurant_id|name|price|category|is_available`
- **customers.txt**: `phone|name|points|total_spent`
- **orders.txt**: `order_id|customer_phone|restaurant_id|item_id|quantity|total_price|timestamp`

## 🛠️ Installation & Setup

### Prerequisites
- GCC compiler
- Unix-like operating system (Linux, macOS)
- Make utility

### Building the Application

1. **Clone or download the project**
   ```bash
   cd OptiMeal
   ```

2. **Compile the application**
   ```bash
   make
   ```

3. **Run the application**
   ```bash
   ./optimeal
   ```

### Clean Build
```bash
make clean
make
```

## 📱 Usage Guide

### Starting the Application
```bash
./optimeal
```

### Main Menu Options
```
=== OPTIMEAL LOGIN SYSTEM ===
1. User Login
2. Restaurant Owner Login
3. Admin Login
0. Exit
=============================
```

### Customer Workflow
1. **Login/Register**: Enter phone number (automatic registration for new users)
2. **Browse Menu**: Select food category and view available items
3. **Place Order**: Choose items, specify quantity, and confirm order
4. **Track Orders**: View order history and current status
5. **Manage Profile**: Check loyalty points and spending analytics

### Restaurant Owner Workflow
1. **Login**: Enter username and password
2. **Manage Menu**: Add, edit, or remove menu items
3. **Monitor Business**: View customer data and restaurant performance
4. **Update Availability**: Mark items as available/unavailable

### Administrator Workflow
1. **Login**: Enter admin credentials
2. **Manage Restaurants**: Create new restaurant owner accounts
3. **System Monitoring**: View all restaurants and customer data
4. **Oversight**: Monitor system performance and user activity

## 🔐 Authentication & Security

- **User Authentication**: Phone number-based login with automatic registration
- **Owner Authentication**: Username/password combination
- **Admin Authentication**: Secure admin credentials
- **Input Validation**: Comprehensive input sanitization and validation
- **Password Hiding**: Secure password input with hidden characters

## 📊 Business Logic Features

### Order Processing
- **Price Calculation**: Automatic total calculation with quantity
- **Points System**: Loyalty points earned based on purchase amount
- **Budget Filtering**: Smart menu filtering based on customer budget
- **Category Matching**: Efficient food categorization and search

### Analytics & Reporting
- **Customer Insights**: Spending patterns and order history
- **Restaurant Performance**: Order volume and revenue tracking
- **System Metrics**: Overall platform usage and statistics

## 🧪 Technical Features

### Code Quality
- **Modular Design**: Clean separation of concerns
- **Error Handling**: Comprehensive error checking and user feedback
- **Memory Management**: Safe file operations and data handling
- **Cross-Platform**: Unix/Linux/macOS compatibility

### Performance
- **Efficient File I/O**: Optimized database operations
- **Smart Filtering**: Fast menu item search and filtering
- **Data Structures**: Optimized data models for performance

## 🚧 Limitations & Considerations

- **Console-Based Interface**: Text-only user interface
- **Local Storage**: Flat-file database (no network capabilities)
- **Single-User Sessions**: No concurrent user support
- **Limited Scalability**: Designed for small to medium-scale operations

## 🔮 Future Enhancements

Potential areas for improvement and expansion:
- **Graphical User Interface**: Modern GUI implementation
- **Network Support**: Multi-user and remote access capabilities
- **Advanced Analytics**: Machine learning-based recommendations
- **Payment Integration**: Online payment processing
- **Mobile Application**: Cross-platform mobile app development
- **Real-time Updates**: Live order tracking and notifications

## 📝 Development

### Project Structure
- **Modular Architecture**: Easy to extend and maintain
- **Clear Dependencies**: Well-defined module relationships
- **Consistent Coding Style**: Uniform code formatting and structure

### Adding New Features
1. Define data structures in `types.h`
2. Implement business logic in appropriate module
3. Add UI functions in `ui.c`
4. Update database schema if needed
5. Test thoroughly before integration

## 📄 License

This project is developed for educational purposes as part of the CSE115 Lab Project course.

## 👥 Contributing

This is an academic project, but suggestions and improvements are welcome. Please ensure any modifications maintain the existing architecture and coding standards.

## 📞 Support

For technical support or questions about the OptiMeal system, please refer to the course materials or contact the development team.

---

**OptiMeal** - Connecting food lovers with great restaurants through smart technology.
