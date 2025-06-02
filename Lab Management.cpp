#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <ctime>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <cctype>
using namespace std;

struct Book {
    string title;
    string author;
    int year;
    int totalCopies;
    int availableCopies;
    string category;
    string addedDate;
    Book* prev;
    Book* next;
};


struct BorrowRecord {
    string bookTitle;
    string bookCategory;
    string borrowerName;
    string borrowerId;
    int borrowedCopies;
    string borrowDate;
    string returnDate;
    bool returned;
    BorrowRecord* next;
};

bool caseInsensitiveCompare(const string& str1, const string& str2) {
    if (str1.length() != str2.length()) {
        return false;
    }
    for (size_t i = 0; i < str1.length(); ++i) {
        if (tolower(str1[i]) != tolower(str2[i])) {
            return false;
        }
    }
    return true;
}

Book* head = NULL;
BorrowRecord* borrowHead = NULL;


bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isValidInput(const string& input, const string& pattern, int minLength) {
    if (input.length() < minLength) {
        cout << " Input must be at least " << minLength << " characters long.\n";
        return false;
    }
    regex reg(pattern);
    if (!regex_match(input, reg)) {
        cout << " Invalid input format. Only letters and spaces allowed.\n";
        return false;
    }
    return true;
}

bool getSafeInt(int& result, const string& prompt, int min, int max) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        bool isValid = !input.empty();
        for (char c : input) {
            if (!isDigit(c)) {
                isValid = false;
                break;
            }
        }

        if (!isValid) {
            cout << " Invalid input. Please enter a valid number.\n";
        } else {
            result = stoi(input);
            if (result >= min && result <= max) {
                return true;
            } else {
                cout << " Please enter a number between " << min << " and " << max << ".\n";
            }
        }
    }
}

string getCurrentDateTime() {
    time_t now = time(0);
    string dt = ctime(&now);
    return dt.substr(0, dt.length()-1);
}

string calculateReturnDate(int days = 14) {
    time_t now = time(0);
    now += days * 24 * 60 * 60;
    string dt = ctime(&now);
    return dt.substr(0, dt.length()-1);
}

time_t stringToTime(const string& dateStr) {
    struct tm tm = {0};
    istringstream iss(dateStr);
    string day, month, timeStr, yearStr;
    int dayNum, year;

    iss >> day >> month >> dayNum >> timeStr >> yearStr;

    size_t colon1 = timeStr.find(':');
    size_t colon2 = timeStr.rfind(':');
    int hour = stoi(timeStr.substr(0, colon1));
    int minute = stoi(timeStr.substr(colon1+1, colon2-colon1-1));
    int second = stoi(timeStr.substr(colon2+1));

    const char* months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    int monthNum = 0;
    for (int i = 0; i < 12; i++) {
        if (month == months[i]) {
            monthNum = i;
            break;
        }
    }

    year = stoi(yearStr);

    tm.tm_year = year - 1900;
    tm.tm_mon = monthNum;
    tm.tm_mday = dayNum;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1;

    return mktime(&tm);
}

int daysBetweenDates(const string& date1, const string& date2) {
    time_t t1 = stringToTime(date1);
    time_t t2 = stringToTime(date2);
    return difftime(t2, t1) / (60 * 60 * 24);
}

void displayBookDetails(Book* book) {
    cout << "\n--- Book Details ---\n";
    cout << " Title: " << book->title << "\n";
    cout << " Author: " << book->author << "\n";
    cout << " Year: " << book->year << "\n";
    cout << " Category: " << book->category << "\n";
    cout << " Added on: " << book->addedDate << "\n";
    cout << " Total copies: " << book->totalCopies << "\n";
    cout << " Available copies: " << book->availableCopies << "\n";
    float percentage = (book->totalCopies > 0) ? ((float)book->availableCopies / book->totalCopies * 100) : 0;
    cout << " Status: " << percentage << "% available\n";
    cout << "----------------------------------\n";
}

void displayBooksByCategory(const string& category) {
    if (!head) {
        cout << "No books in the library.\n";
        return;
    }

    Book* temp = head;
    bool found = false;

    cout << "\n--- Books in Category: " << category << " ---\n";
    while (temp) {
        if (caseInsensitiveCompare(temp->category, category)) {
            found = true;
            cout << " Title: " << temp->title << "\n";
            cout << " Author: " << temp->author << "\n";
            cout << " Year: " << temp->year << "\n";
            cout << " Added on: " << temp->addedDate << "\n";
            cout << " Copies: " << temp->availableCopies << "/" << temp->totalCopies << " available\n";
            cout << "-------------------------\n";
        }
        temp = temp->next;
    }

    if (!found) {
        cout << " No books found in this category.\n";
    }
}

void displayAllBooks() {
    if (!head) {
        cout << "No books in the library.\n";
        return;
    }

    Book* temp = head;
    Book* checker;

    while (temp) {
        bool shown = false;
        checker = head;
        while (checker != temp) {
            if (caseInsensitiveCompare(checker->category, temp->category)) {
                shown = true;
                break;
            }
            checker = checker->next;
        }

        if (!shown) {
            displayBooksByCategory(temp->category);
        }
        temp = temp->next;
    }
}

int countCategories() {
    int count = 0;
    Book* temp = head;
    Book* checker;

    while (temp) {
        bool found = false;
        checker = head;
        while (checker != temp) {
            if (caseInsensitiveCompare(checker->category, temp->category)) {
                found = true;
                break;
            }
            checker = checker->next;
        }
        if (!found) count++;
        temp = temp->next;
    }
    return count;
}

void showCategoryMenu() {
    cout << "\n--------- Book Categories ----------\n";
    Book* temp = head;
    Book* checker;
    int index = 1;

    while (temp) {
        bool shown = false;
        checker = head;
        while (checker != temp) {
            if (caseInsensitiveCompare(checker->category, temp->category)) {
                shown = true;
                break;
            }
            checker = checker->next;
        }

        if (!shown) {
            cout << index++ << ". " << temp->category << "\n";
        }
        temp = temp->next;
    }

    cout << index << ". Back to Main Menu\n";
}

string getCategoryByIndex(int index) {
    Book* temp = head;
    Book* checker;
    int currentIndex = 1;

    while (temp) {
        bool foundEarlier = false;
        checker = head;
        while (checker != temp) {
            if (caseInsensitiveCompare(checker->category, temp->category)) {
                foundEarlier = true;
                break;
            }
            checker = checker->next;
        }

        if (!foundEarlier) {
            if (currentIndex == index) {
                return temp->category;
            }
            currentIndex++;
        }
        temp = temp->next;
    }
    return "";
}

string selectCategory() {
    int totalCategories = countCategories();
    int choice;
    int totalOptions = totalCategories + 1;

    while (true) {
        showCategoryMenu();
        if (!getSafeInt(choice, "Select category (or " + to_string(totalCategories+1) + " to exit): ", 1, totalOptions)) continue;

        if (choice <= totalCategories) {
            return getCategoryByIndex(choice);
        } else {
            return "";
        }
    }
}

void swapBooks(Book* a, Book* b) {
    string tempTitle = a->title;
    string tempAuthor = a->author;
    int tempYear = a->year;
    int tempTotal = a->totalCopies;
    int tempAvailable = a->availableCopies;
    string tempCategory = a->category;
    string tempDate = a->addedDate;

    a->title = b->title;
    a->author = b->author;
    a->year = b->year;
    a->totalCopies = b->totalCopies;
    a->availableCopies = b->availableCopies;
    a->category = b->category;
    a->addedDate = b->addedDate;

    b->title = tempTitle;
    b->author = tempAuthor;
    b->year = tempYear;
    b->totalCopies = tempTotal;
    b->availableCopies = tempAvailable;
    b->category = tempCategory;
    b->addedDate = tempDate;
}

bool bookExists(const string& title, const string& author) {
    Book* temp = head;
    while (temp) {
        if (caseInsensitiveCompare(temp->title, title) &&
            caseInsensitiveCompare(temp->author, author)) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

void addBookToList(const string& title, const string& author, int year,
                  const string& category, const string& addedDate,
                  int totalCopies, int availableCopies) {
    Book* newBook = new Book{title, author, year, totalCopies, availableCopies,
                            category, addedDate, NULL, NULL};
    if (!head) {
        head = newBook;
    } else {
        Book* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = newBook;
        newBook->prev = temp;
    }
}

void sortBooksByTitle(const string& category) {
    if (!head || !head->next) {
        cout << " Not enough books to sort.\n";
        return;
    }

    bool swapped;
    Book *start = head;
    Book *end = NULL;

    do {
        swapped = false;
        Book *current = start;

        while (current->next != end) {
            bool shouldSwap = false;

            if (caseInsensitiveCompare(current->category, category) &&
                caseInsensitiveCompare(current->next->category, category)) {
                string title1 = current->title;
                string title2 = current->next->title;
                transform(title1.begin(), title1.end(), title1.begin(), ::tolower);
                transform(title2.begin(), title2.end(), title2.begin(), ::tolower);
                shouldSwap = title1 > title2;
            }

            if (shouldSwap) {
                swapBooks(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
        end = current;
    } while (swapped);

    saveToFile();
    cout << "\nBooks in category '" << category << "' sorted by title:\n";
    displayBooksByCategory(category);
}

void sortBooksMenu() {
    string category = selectCategory();
    if (category.empty()) {
        cout << "Sorting cancelled.\n";
        return;
    }

    sortBooksByTitle(category);
}

void displayBooksMenu() {
    int choice;
    do {
        cout << "\n---------- Display Options -----------\n";
        cout << "1. Display All Books\n";
        cout << "2. Display Books by Category\n";
        cout << "3. Back to Main Menu\n";

        if (!getSafeInt(choice, "Enter your choice (1-3): ", 1, 3)) continue;

        switch (choice) {
            case 1:
                displayAllBooks();
                break;
            case 2: {
                string category = selectCategory();
                if (!category.empty()) {
                    displayBooksByCategory(category);
                }
                break;
            }
            case 3:
                return;
        }
    } while (true);
}

void saveToFile() {
    ofstream file("library_data.txt");
    Book* temp = head;
    while (temp) {
        file << temp->title << "|" << temp->author << "|" << temp->year << "|"
             << temp->totalCopies << "|" << temp->availableCopies << "|"
             << temp->category << "|" << temp->addedDate << "\n";
        temp = temp->next;
    }
    file.close();
}

void loadFromFile() {
    ifstream file("library_data.txt");
    string line;
    bool isEmpty = true;

    while (getline(file, line)) {
        isEmpty = false;
        string tokens[7];
        size_t pos = 0;
        int i = 0;
        while ((pos = line.find('|')) != string::npos && i < 6) {
            tokens[i++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        tokens[6] = line;

        if (i >= 6) {
            string title = tokens[0];
            string author = tokens[1];
            int year = stoi(tokens[2]);
            int totalCopies = stoi(tokens[3]);
            int availableCopies = stoi(tokens[4]);
            string category = tokens[5];
            string addedDate = tokens[6];

            addBookToList(title, author, year, category, addedDate, totalCopies, availableCopies);
        }
    }
    file.close();

    if (isEmpty) {
        string dt = getCurrentDateTime();
        addBookToList("Fikir Ena Desita", "Hana Gebreegziabher", 2005, "Fiction", dt, 3, 3);
        addBookToList("Sew LeSew", "Aregawi Desalegn", 1998, "History", dt, 2, 2);
        addBookToList("Yetintawi Tarik Tarik", "Molla Abebe", 2010, "Fiction", dt, 1, 1);
        addBookToList("Introduction to Algorithms", "Thomas H. Cormen", 2009, "Computer Science", dt, 5, 5);
        addBookToList("Clean Code", "Robert C. Martin", 2008, "Computer Science", dt, 4, 4);
        saveToFile();
    }
}

void countBooksInCategory(const string& category) {
    if (!head) {
        cout << "No books in the library.\n";
        return;
    }

    int bookCount = 0;
    int totalCopies = 0;
    int availableCopies = 0;
    Book* temp = head;

    while (temp) {
        if (caseInsensitiveCompare(temp->category, category)) {
            bookCount++;
            totalCopies += temp->totalCopies;
            availableCopies += temp->availableCopies;
        }
        temp = temp->next;
    }

    cout << "\n--- Book Count for Category: " << category << " ---\n";
    cout << " Number of unique titles: " << bookCount << "\n";
    cout << " Total copies: " << totalCopies << "\n";
    cout << " Available copies: " << availableCopies << "\n";
    cout << " Checked out copies: " << (totalCopies - availableCopies) << "\n";
    cout << "-------------------------\n";
}

void countAllBooks() {
    if (!head) {
        cout << "No books in the library.\n";
        return;
    }

    int bookCount = 0;
    int totalCopies = 0;
    int availableCopies = 0;
    Book* temp = head;

    while (temp) {
        bookCount++;
        totalCopies += temp->totalCopies;
        availableCopies += temp->availableCopies;
        temp = temp->next;
    }

    cout << "\n--- Total Library Statistics ---\n";
    cout << " Number of unique titles: " << bookCount << "\n";
    cout << " Total copies: " << totalCopies << "\n";
    cout << " Available copies: " << availableCopies << "\n";
    cout << " Checked out copies: " << (totalCopies - availableCopies) << "\n";
    cout << "-------------------------\n";
}