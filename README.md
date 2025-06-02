# Dilla University Library Management System

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [File Structure](#file-structure)
- [Dependencies](#dependencies)
- [Data Structures](#data-structures)
- [Key Functions](#key-functions)
- [Usage](#usage)
- [Input Validation](#input-validation)
- [Error Handling](#error-handling)
- [Limitations](#limitations)
- [Future Improvements](#future-improvements)
- [Compilation Requirements](#compilation-requirements)
- [Sample Data](#sample-data)
- [Notes](#notes)
- [License](#license)

## Overview
The **Dilla University Library Management System** is a C++ console-based application designed to manage a library's book inventory and borrowing records. It provides a comprehensive set of functionalities for librarians to add, display, search, update, delete, sort, and count books, as well as manage book borrowing and returning processes. The system uses a doubly linked list to store book records and a singly linked list for borrow records, with data persistence through file storage.

## Features
- **Book Management**:
  - Add single or multiple books with details such as title, author, publication year, category, and number of copies.
  - Display books by category or all books in the library.
  - Search for books by title within a specific category.
  - Update book details (title, author, category, year, total copies).
  - Delete specific copies of a book or all books in a category/library.
  - Sort books by title within a category.
  - Count books by category or across the entire library.
- **Borrowing and Returning**:
  - Borrow one or multiple books (up to 5 at a time) with validation to prevent borrowing the same book twice.
  - Return books with automatic fine calculation for late returns (5 birr per day).
  - Display borrowing rules to users.
- **Data Persistence**:
  - Books are saved to `library_data.txt`.
  - Borrow records are saved to `borrow_records.txt`.
  - Deletions are logged in `library_deletions.log`.
- **Input Validation**:
  - Ensures valid input for book titles, author names, years (1800–2025), and copy counts (1–1000).
  - Validates borrower names and IDs.
- **Date Handling**:
  - Tracks book addition, borrowing, and return dates.
  - Calculates due dates (14 days from borrowing) and fines for late returns.

## File Structure
- **DSA_LAST.cpp**: The main source code file containing the entire library management system.
- **library_data.txt**: Stores book records in the format `title|author|year|totalCopies|availableCopies|category|addedDate`.
- **borrow_records.txt**: Stores borrow records in the format `bookTitle|bookCategory|borrowerName|borrowerId|borrowedCopies|borrowDate|returnDate|returned`.
- **library_deletions.log**: Logs deletion events with timestamps and details.

## Dependencies
- **C++ Standard Library**:
  - `<iostream>`, `<fstream>`, `<string>`, `<regex>`, `<ctime>`, `<cstring>`, `<algorithm>`, `<sstream>`, `<cctype>`
- **C++ Compiler**: Compatible with C++11 or later (e.g., g++, MSVC).
- No external libraries are required.

## Data Structures
- **Book**: A structure representing a book with fields:
  - `title`, `author`, `category`, `addedDate` (strings)
  - `year`, Stationary`totalCopies`, `availableCopies` (integers)
  - `prev`, `next` (pointers for doubly linked list)
- **BorrowRecord**: A structure representing a borrow record with fields:
  - `bookTitle`, `bookCategory`, `borrowerName`, `borrowerId`, `borrowDate`, `returnDate` (strings)
  - `borrowedCopies` (integer)
  - `returned` (boolean)
  - `next` (pointer for singly linked list)
- **Linked Lists**:
  - Doubly linked list for books (`head` as global pointer).
  - Singly linked list for borrow records (`borrowHead` as global pointer).

## Key Functions
- **Input Validation**:
  - `isValidInput()`: Validates string inputs using regex for letters and spaces.
  - `getSafeInt()`: Ensures integer inputs are within specified ranges.
- **Book Management**:
  - `addBookToList()`: Adds a book to the doubly linked list.
  - `displayBooksByCategory()`: Displays books in a specific category.
  - `displayAllBooks()`: Displays all books, grouped by category.
  - `sortBooksByTitle()`: Sorts books by title within a category using bubble sort.
  - `updateBook()`: Updates book details with input validation.
  - `deleteBook()`: Removes specific copies or an entire book record.
  - `deleteAllBooks()`: Deletes all books in a category or the entire library.
- **Borrowing and Returning**:
  - `borrowBook()`: Handles borrowing a single book with validation.
  - `borrowMultipleBooks()`: Allows borrowing multiple books in one session.
  - `returnBook()`: Processes book returns and calculates fines if late.
  - `addBorrowRecord()`: Adds a borrow record to the singly linked list.
- **File Operations**:
  - `saveToFile()`, `loadFromFile()`: Manage book data persistence.
  - `saveBorrowRecords()`, `loadBorrowRecords()`: Manage borrow record persistence.
- **Date Handling**:
  - `getCurrentDateTime()`: Returns the current date and time.
  - `calculateReturnDate()`: Computes the due date (14 days from borrowing).
  - `daysBetweenDates()`: Calculates the number of days between two dates for fine computation.
- **Utility**:
  - `caseInsensitiveCompare()`: Performs case-insensitive string comparison.
  - `countCategories()`: Counts unique book categories.
  - `selectCategory()`: Provides a menu for selecting categories.

## Usage
1. **Compile and Run**:
   ```bash
   g++ -o library DSA_LAST.cpp
   ./library
   ```
2. **Main Menu Options**:
   - **1. Add Books**: Add one or more books with validated inputs.
   - **2. Display Books**: View all books or books by category.
   - **3. Search Book**: Search for a book by title within a category.
   - **4. Delete Book Copies**: Remove specific copies of a book.
   - **5. Count Books**: Count books by category or across the library.
   - **6. Sort Books**: Sort books by title within a category.
   - **7. Delete All Books**: Delete all books in a category or the entire library.
   - **8. Update Book**: Modify book details.
   - **9. Borrow One Book**: Borrow a single book.
   - **10. Borrow Multiple Books**: Borrow up to 5 books in one session.
   - **11. Return Book**: Return a borrowed book with fine calculation.
   - **12. Exit**: Clean up memory and exit the program.
3. **Default Data**: If `library_data.txt` is empty, the system initializes with sample books in Fiction, History, and Computer Science categories.

## Input Validation
- **Book Title/Author**: Must be at least 4 characters, containing only letters and spaces.
- **Borrower Name**: Must be at least 3 characters, containing only letters and spaces.
- **Borrower ID**: Must be at least 3 characters, allowing letters and numbers.
- **Year**: Must be between 1800 and 2025.
- **Copies**: Must be between 1 and 1000.
- **Borrowing Limits**: Users cannot borrow multiple copies of the same book or exceed 5 books in a multiple-borrow session.

## Error Handling
- Validates all user inputs to prevent invalid data entry.
- Prevents borrowing unavailable books or books already borrowed by the same user.
- Logs deletions to ensure traceability.
- Handles file I/O errors by initializing default data if files are empty or corrupted.

## Limitations
- Case-insensitive searches may lead to unexpected matches if titles/authors differ only by case.
- Date parsing assumes a specific format (`ctime` output), which may not be robust across all systems.
- No support for partial title searches or advanced filtering.
- Fine calculation is simplistic (5 birr per day) and does not account for holidays or library-specific policies.

## Future Improvements
- Add support for partial title/author searches using regex.
- Implement user authentication for borrowers.
- Enhance date parsing to handle multiple formats.
- Add a graphical user interface (GUI) for better user experience.
- Include support for tracking book conditions or reservations.

## Compilation Requirements
- **Compiler**: Any C++11-compliant compiler (e.g., g++ 4.8 or later).
- **Operating System**: Platform-independent, tested on Unix-like systems and Windows.
- **Standard Library**: Uses standard C++ libraries; no external dependencies required.

## Sample Data
Upon first run (if `library_data.txt` is empty), the system initializes with:
- "Fikir Ena Desita" (Fiction, 3 copies)
- "Sew LeSew" (History, 2 copies)
- "Yetintawi Tarik Tarik" (Fiction, 1 copy)
- "Introduction to Algorithms" (Computer Science, 5 copies)
- "Clean Code" (Computer Science, 4 copies)

## Notes
- Ensure write permissions for the directory containing `library_data.txt`, `borrow_records.txt`, and `library_deletions.log`.
- The system assumes the system clock is set correctly for accurate date calculations.
- Memory is cleaned up on program exit to prevent leaks.

## License
This project is for educational purposes and is not licensed for commercial use. Use and modify at your own discretion.