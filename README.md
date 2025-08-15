# OptiMeal - Optimal Meal Ordering & Restaurant Management System

A revolutionary budget-first food ordering system designed for anyone on a budget who struggles with the overwhelming choices and budget constraints of traditional food delivery apps - students, young professionals, job holders, and budget-conscious individuals.

## 🎯 **The Real Problem We're Solving**

As budget-conscious individuals (students, young professionals, job holders), we face a daily struggle that existing food delivery apps fail to address:

**The "Too Many Options" Problem:**
- **Decision Paralysis**: Apps like FoodPanda and Pathao Food overwhelm us with hundreds of menu items
- **Time Wasting**: We spend precious time (that could be used for studying) just deciding what to order
- **Budget Overspending**: Without budget constraints, we often choose expensive items that exceed our financial limits
- **Revenue-First Approach**: Existing platforms prioritize restaurant profits over student affordability

**Why OptiMeal?**
OptiMeal was born from real frustration. Instead of building another app that maximizes restaurant revenue, we created a solution that puts budget-conscious users first - with budget filtering, simplified choices, and time-efficient ordering that respects financial constraints and busy schedules.

---

A comprehensive console-based food ordering and restaurant management system built in C, designed to connect customers, restaurant owners, and administrators in a unified platform while prioritizing user affordability and decision simplicity.

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
- **Budget-First Smart Ordering System**
  - Browse menu items by category (Breakfast, Lunch, Snacks, Dinner)
  - **Automatic budget filtering** - only see items within your price range
  - **Decision simplification** - no more overwhelming choices
  - **Time-efficient ordering** - quick decisions for busy individuals
  - Real-time menu availability checking
  
- **User Management**
  - Phone number-based authentication
  - Automatic account creation for new users
  - Profile management and order history
  
- **Budget-Friendly Loyalty Program**
  - Points earned on every purchase
  - Redeemable rewards and discounts
  - Spending analytics and insights
  - **Budget protection** - helps maintain financial discipline

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

**No More Choice Overwhelm!** Our system supports a comprehensive range of food categories, but presents them in a budget-friendly way:

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

**Current Limitations (We're Working on These!):**
- **Console-Based Interface**: Text-only user interface (GUI coming soon!)
- **Local Storage**: Flat-file database (cloud version in development)
- **Single-User Sessions**: No concurrent user support (multi-user version planned)
- **Limited Scalability**: Designed for small to medium-scale operations (scaling up!)

**Why These Limitations Exist:**
This is our first version built as a CSE115 lab project. We focused on solving the core problem (budget-first ordering) first, and these features will come in future iterations as we learn more advanced programming concepts. The app serves as a proof-of-concept for budget-conscious food ordering.

## 🔮 Future Enhancements

**Our Vision for OptiMeal (Budget-First Development):**

### Phase 1: Enhanced User Experience
- **Graphical User Interface**: Modern GUI implementation
- **Mobile Application**: Cross-platform mobile app development
- **Real-time Updates**: Live order tracking and notifications

### Phase 2: Advanced Features
- **Network Support**: Multi-user and remote access capabilities
- **Payment Integration**: Online payment processing
- **Advanced Analytics**: Machine learning-based recommendations

### Phase 3: Scale & Innovation
- **AI-Powered Recommendations**: Smart meal suggestions based on budget and preferences
- **Social Features**: Student reviews and ratings
- **Campus Integration**: University meal plan integration

**Why This Roadmap?**
We're building this as budget-conscious individuals, for budget-conscious individuals. Each phase represents what we learn in our programming courses, so the app grows with our skills while serving a broader audience!

## 📝 Development

### Project Structure
- **Modular Architecture**: Easy to extend and maintain
- **Clear Dependencies**: Well-defined module relationships
- **Consistent Coding Style**: Uniform code formatting and structure

### Our Development Philosophy
**"Build What You Need, Learn What You Use"**
- We started with the core problem (budget filtering) and built around it
- Each module represents a real need we identified
- The architecture grew organically as we learned more C programming concepts
- We prioritized solving real problems over perfect code structure

### Adding New Features
1. Define data structures in `types.h`
2. Implement business logic in appropriate module
3. Add UI functions in `ui.c`
4. Update database schema if needed
5. Test thoroughly before integration

## 📄 License

This project is developed for educational purposes as part of the CSE115 Lab Project course.

**Our Story:**
OptiMeal started as a simple solution to a daily problem we faced as budget-conscious individuals. What began as a C programming assignment became a passion project when we realized how many people struggle with the same food ordering issues - overwhelming choices and no budget protection. We're sharing this code to help others learn and to inspire solutions to real-world problems that affect millions of people daily.

## 👥 Contributing

**We'd Love Your Help!** This started as an academic project, but it's become something bigger.

**How You Can Contribute:**
- **Bug Reports**: Found an issue? Let us know!
- **Feature Ideas**: Have ideas for budget-friendly features?
- **Code Improvements**: Better ways to implement something?
- **Documentation**: Help make this easier for others to understand

**Our Guidelines:**
- Keep it budget-focused and user-friendly
- Maintain the existing architecture with separation of concerns
- Add comments explaining your changes
- Test thoroughly before submitting a pull request

**Why Contribute?**
Because we're all trying to solve real problems that affect millions of people daily. Your input could help thousands of budget-conscious individuals who face the same daily struggles with food ordering!

## 📞 Support

**Need Help? We're Students Too!**

**For Technical Issues:**
- Review the code comments (we tried to explain everything!)
- Contact the development team (we're friendly!)

**For Feature Requests:**
- Found a bug that affects your ordering? Report it!
- Want to help improve the app? Join us!

**Our Promise:**
We built this to solve real problems, so we're committed to making it work for you. Your feedback helps us build better solutions for all budget-conscious individuals.

---

**OptiMeal** - Built by budget-conscious individuals, for budget-conscious individuals. Because we know what it's like to spend 30 minutes deciding what to order and still go over budget! 🍕💼💰

---

**OptiMeal** - Connecting food lovers with great restaurants through smart technology.
