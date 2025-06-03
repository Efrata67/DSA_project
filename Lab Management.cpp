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

void addBooks() {
    int count;
    if (!getSafeInt(count, "Enter the number of books to add (1-100): ", 1, 100)) return;

    for (int i = 0; i < count; i++) {
        cout << "\n--- Adding Book #" << i+1 << " ---\n";

        string category = selectCategory();
        if (category.empty()) {
            cout << "Returning to main menu.\n";
            return;
        }

        string title, author;
        int year, totalCopies;

        do {
            cout << "Enter book title (minimum 4 letters, letters and spaces only): ";
            getline(cin, title);
        } while (!isValidInput(title, "^[a-zA-Z ]+$", 4));

        do {
            cout << "Enter author name (minimum 4 letters, letters and spaces only): ";
            getline(cin, author);
        } while (!isValidInput(author, "^[a-zA-Z ]+$", 4));

        if (bookExists(title, author)) {
            Book* temp = head;
            while (temp) {
                if (caseInsensitiveCompare(temp->title, title) &&
                    caseInsensitiveCompare(temp->author, author)) {
                    cout << "Enter additional copies to add (1-1000): ";
                    if (!getSafeInt(totalCopies, "", 1, 1000)) return;

                    int newTotal = temp->totalCopies + totalCopies;
                    int newAvailable = temp->availableCopies + totalCopies;
                    temp->totalCopies = newTotal;
                    temp->availableCopies = newAvailable;
                    cout << "Book already exists. Added " << totalCopies << " more copies.\n";
                    cout << " New total: " << newTotal << " copies (" << newAvailable << " available)\n";
                    saveToFile();
                    break;
                }
                temp = temp->next;
            }
        } else {
            if (!getSafeInt(year, "Enter year of publication (1800-2025): ", 1800, 2025)) return;
            if (!getSafeInt(totalCopies, "Enter total number of copies (1-1000): ", 1, 1000)) return;

            string dt = getCurrentDateTime();
            addBookToList(title, author, year, category, dt, totalCopies, totalCopies);
            cout << "Book added successfully with " << totalCopies << " copies.\n";
        }
    }
    saveToFile();
}

void searchBooks() {
    string category = selectCategory();
    if (category.empty()) {
        cout << "Returning to main menu.\n";
        return;
    }

    string title;
    cout << "Enter title to search in category '" << category << "': ";
    getline(cin, title);

    Book* temp = head;
    bool found = false;

    while (temp) {
        if (caseInsensitiveCompare(temp->category, category) &&
            caseInsensitiveCompare(temp->title, title)) {
            displayBookDetails(temp);
            found = true;
            break;
        }
        temp = temp->next;
    }

    if (!found) {
        cout << " Book not found in category '" << category << "'.\n";
    }
}

void deleteBook() {
    string category = selectCategory();
    if (category.empty()) {
        cout << "Returning to main menu.\n";
        return;
    }

    string title;
    cout << "Enter title to delete from category '" << category << "': ";
    getline(cin, title);

    Book* temp = head;
    while (temp) {
        if (caseInsensitiveCompare(temp->category, category) &&
            caseInsensitiveCompare(temp->title, title)) {
            displayBookDetails(temp);

            int copiesToDelete;
            if (!getSafeInt(copiesToDelete, "Enter number of copies to delete (1-" + to_string(temp->totalCopies) + "): ", 1, temp->totalCopies)) {
                return;
            }

            if (copiesToDelete > temp->availableCopies) {
                cout << " Warning: You're trying to delete more copies than are currently available.\n";
                cout << " Only " << temp->availableCopies << " copies are available to delete.\n";

                char confirm;
                cout << " Do you want to proceed? (y/n): ";
                cin >> confirm;
                cin.ignore();

                if (tolower(confirm) != 'y') {
                    cout << " Delete operation cancelled.\n";
                    return;
                }
            }

            string deletionTime = getCurrentDateTime();
            string deletionDetails = "Deleted " + to_string(copiesToDelete) +
                                   " copies of '" + title +
                                   "' from category '" + category +
                                   "' on " + deletionTime;

            if (copiesToDelete == temp->totalCopies) {
                if (temp->prev)
                    temp->prev->next = temp->next;
                else
                    head = temp->next;

                if (temp->next)
                    temp->next->prev = temp->prev;

                delete temp;
                cout << " Book record completely deleted.\n";
            } else {
                temp->totalCopies -= copiesToDelete;
                temp->availableCopies = max(0, temp->availableCopies - copiesToDelete);
                cout << " " << copiesToDelete << " copies removed from inventory.\n";
                displayBookDetails(temp);
            }

            ofstream logfile("library_deletions.log", ios::app);
            if (logfile.is_open()) {
                logfile << deletionDetails << "\n";
                logfile.close();
            }

            saveToFile();
            cout << " Deletion logged: " << deletionDetails << "\n";
            return;
        }
        temp = temp->next;
    }
    cout << " Book not found in category '" << category << "'.\n";
}

void updateBook() {
    cout << "\n===== Update Book =====\n";

    string category = selectCategory();
    if (category.empty()) {
        cout << "Update cancelled.\n";
        return;
    }

    string title;
    cout << "Enter title of the book to update in category '" << category << "': ";
    getline(cin, title);

    Book* temp = head;
    while (temp) {
        if (caseInsensitiveCompare(temp->category, category) &&
            caseInsensitiveCompare(temp->title, title)) {
            cout << "\n--- Current Book Information ---";
            displayBookDetails(temp);

            cout << "\nEnter new details for the book (leave blank to keep current value):\n";

            string newTitle;
            cout << "Enter new title [" << temp->title << "]: ";
            getline(cin, newTitle);
            if (!newTitle.empty()) {
                while (!isValidInput(newTitle, "^[a-zA-Z ]+$", 4)) {
                    cout << "Enter new title [" << temp->title << "]: ";
                    getline(cin, newTitle);
                    if (newTitle.empty()) break;
                }
                if (!newTitle.empty()) {
                    if (bookExists(newTitle, temp->author)) {
                        cout << "A book with this title and author already exists.\n";
                        return;
                    }
                    temp->title = newTitle;
                }
            }

            string newAuthor;
            cout << "Enter new author [" << temp->author << "]: ";
            getline(cin, newAuthor);
            if (!newAuthor.empty()) {
                while (!isValidInput(newAuthor, "^[a-zA-Z ]+$", 4)) {
                    cout << "Enter new author [" << temp->author << "]: ";
                    getline(cin, newAuthor);
                    if (newAuthor.empty()) break;
                }
                if (!newAuthor.empty()) {
                    if (bookExists(temp->title, newAuthor)) {
                        cout << "A book with this title and author already exists.\n";
                        return;
                    }
                    temp->author = newAuthor;
                }
            }

            cout << "\nCurrent category: " << temp->category << endl;
            cout << "Select new category:\n";
            string newCategory = selectCategory();
            if (!newCategory.empty()) {
                temp->category = newCategory;
            }

            string yearInput;
            int newYear;
            cout << "Enter new year [" << temp->year << "]: ";
            getline(cin, yearInput);
            if (!yearInput.empty()) {
                while (true) {
                    bool valid = true;
                    for (char c : yearInput) {
                        if (!isDigit(c)) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid) {
                        newYear = stoi(yearInput);
                        if (newYear >= 1800 && newYear <= 2025) {
                            temp->year = newYear;
                            break;
                        }
                    }
                    cout << "Please enter a valid year between 1800 and 2025 [" << temp->year << "]: ";
                    getline(cin, yearInput);
                    if (yearInput.empty()) break;
                }
            }

            string copiesInput;
            int newTotalCopies;
            cout << "Enter new total copies [" << temp->totalCopies << "]: ";
            getline(cin, copiesInput);
            if (!copiesInput.empty()) {
                while (true) {
                    bool valid = true;
                    for (char c : copiesInput) {
                        if (!isDigit(c)) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid) {
                        newTotalCopies = stoi(copiesInput);
                        if (newTotalCopies >= 1 && newTotalCopies <= 1000) {
                            int difference = newTotalCopies - temp->totalCopies;
                            temp->availableCopies += difference;
                            if (temp->availableCopies < 0) temp->availableCopies = 0;
                            if (temp->availableCopies > newTotalCopies) temp->availableCopies = newTotalCopies;
                            temp->totalCopies = newTotalCopies;
                            break;
                        }
                    }
                    cout << "Please enter a valid number between 1 and 1000 [" << temp->totalCopies << "]: ";
                    getline(cin, copiesInput);
                    if (copiesInput.empty()) break;
                }
            }

            saveToFile();
            cout << "\nBook updated successfully!\n";
            cout << "--- Updated Book Information ---";
            displayBookDetails(temp);
            return;
        }
        temp = temp->next;
    }
    cout << "Book not found in category '" << category << "'.\n";
}

void saveBorrowRecords() {
ofstream file("borrow_records.txt");
BorrowRecord* temp = borrowHead;
while (temp) {
    file << temp->bookTitle << "|" << temp->bookCategory << "|"
         << temp->borrowerName << "|" << temp->borrowerId << "|"
         << temp->borrowedCopies << "|" << temp->borrowDate << "|"
         << temp->returnDate << "|" << (temp->returned ? "1" : "0") << "\n";
    temp = temp->next;
}
file.close();
}

void loadBorrowRecords() {
ifstream file("borrow_records.txt");
string line;

while (getline(file, line)) {
    string tokens[8];
    size_t pos = 0;
    int i = 0;
    while ((pos = line.find('|')) != string::npos && i < 7) {
        tokens[i++] = line.substr(0, pos);
        line.erase(0, pos + 1);
    }
    tokens[7] = line;

    if (i >= 7) {
        BorrowRecord* newRecord = new BorrowRecord{
            tokens[0], tokens[1], tokens[2], tokens[3],
            stoi(tokens[4]), tokens[5], tokens[6],
            tokens[7] == "1", NULL
        };

        if (!borrowHead) {
            borrowHead = newRecord;
        } else {
            BorrowRecord* temp = borrowHead;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newRecord;
        }
    }
}
file.close();
}

void addBorrowRecord(const string& title, const string& category, const string& name,
                const string& id, int copies, const string& borrowDate,
                const string& returnDate) {
BorrowRecord* newRecord = new BorrowRecord{
    title, category, name, id, copies, borrowDate, returnDate, false, NULL
};

if (!borrowHead) {
    borrowHead = newRecord;
} else {
    BorrowRecord* temp = borrowHead;
    while (temp->next) {
        temp = temp->next;
    }
    temp->next = newRecord;
}
saveBorrowRecords();
}

void displayBorrowRules() {
cout << "\n--------- Borrowing Rules -----------\n";
cout << "1. You can borrow one copy of any book, but not multiple copies of the same book.\n";
cout << "2. The standard borrowing period is 14 days.\n";
cout << "3. Late returns will incur a fine of 5 birr per day.\n";
cout << "4. Books must be returned in the same condition as borrowed.\n";
cout << "5. Lost or damaged books must be replaced or paid for.\n";
cout << "-----------------------------------------------\n";
}

bool hasBorrowedSpecificBook(const string& borrowerId, const string& title, const string& category) {
BorrowRecord* temp = borrowHead;
while (temp) {
    if (caseInsensitiveCompare(temp->borrowerId, borrowerId) &&
        caseInsensitiveCompare(temp->bookTitle, title) &&
        caseInsensitiveCompare(temp->bookCategory, category) &&
        !temp->returned) {
        return true;
    }
    temp = temp->next;
}
return false;
}

void borrowBook() {
    string category = selectCategory();
    if (category.empty()) {
        cout << "Borrowing cancelled.\n";
        return;
    }

    displayBooksByCategory(category);

    string title;
    cout << "Enter the title of the book you want to borrow: ";
    getline(cin, title);

    Book* temp = head;
    while (temp) {
        if (caseInsensitiveCompare(temp->category, category) &&
            caseInsensitiveCompare(temp->title, title)) {
            displayBookDetails(temp);

            if (temp->availableCopies <= 0) {
                cout << "Sorry, no copies of this book are currently available.\n";
                return;
            }

            string borrowerName, borrowerId;
            do {
                cout << "Enter your full name (minimum 3 letters, no numbers): ";
                getline(cin, borrowerName);
            } while (!isValidInput(borrowerName, "^[a-zA-Z ]+$", 3));

            do {
                cout << "Enter your ID (minimum 3 characters, letters or numbers): ";
                getline(cin, borrowerId);
                if (borrowerId.length() < 3) {
                    cout << "ID must be at least 3 characters long.\n";
                }
            } while (borrowerId.length() < 3);

            if (hasBorrowedSpecificBook(borrowerId, title, category)) {
                cout << "Sorry, you have already borrowed a copy of this book.\n";
                cout << "Please return it before borrowing another copy.\n";
                return;
            }

            displayBorrowRules();

            char confirm;
            cout << "Do you want to proceed with borrowing 1 copy of '" << title << "'? (y/n): ";
            cin >> confirm;
            cin.ignore();

            if (tolower(confirm) == 'y') {
                temp->availableCopies -= 1;
                saveToFile();

                string borrowDate = getCurrentDateTime();
                string returnDate = calculateReturnDate(14);

                addBorrowRecord(title, category, borrowerName, borrowerId,
                              1, borrowDate, returnDate);

                cout << "\n--------- Borrowing Confirmation -----------\n";
                cout << " Book Title: " << title << "\n";
                cout << " Category: " << category << "\n";
                cout << " Borrower Name: " << borrowerName << "\n";
                cout << " Borrower ID: " << borrowerId << "\n";
                cout << " Copies Borrowed: 1\n";
                cout << " Borrow Date: " << borrowDate << "\n";
                cout << " Due Date: " << returnDate << "\n";
                cout << "-------------------------------------\n";
                cout << "Thank you for borrowing from our library!\n";
            } else {
                cout << "Borrowing cancelled.\n";
            }
            return;
        }
        temp = temp->next;
    }
    cout << "Book not found in category '" << category << "'.\n";
}

void borrowMultipleBooks() {
    cout << "\n----------- Borrow Multiple Books ------------\n";

    string borrowerName, borrowerId;
    do {
        cout << "Enter your full name (minimum 3 letters, no numbers): ";
        getline(cin, borrowerName);
    } while (!isValidInput(borrowerName, "^[a-zA-Z ]+$", 3));

    do {
        cout << "Enter your ID (minimum 3 characters, letters or numbers): ";
        getline(cin, borrowerId);
        if (borrowerId.length() < 3) {
            cout << "ID must be at least 3 characters long.\n";
        }
    } while (borrowerId.length() < 3);

    int numBooks;
    if (!getSafeInt(numBooks, "Enter the number of books to borrow (2-5): ", 2, 5)) return;

    struct BorrowInfo {
        string category;
        string title;
        Book* book;
    };

    BorrowInfo borrows[5];
    int borrowCount = 0;

    for (int i = 0; i < numBooks; i++) {
        cout << "\n--- Selecting Book #" << i+1 << " ---\n";
        string category = selectCategory();
        if (category.empty()) {
            cout << "Borrowing cancelled.\n";
            return;
        }

        displayBooksByCategory(category);

        string title;
        cout << "Enter the title of the book to borrow: ";
        getline(cin, title);

        Book* temp = head;
        bool found = false;
        while (temp) {
            if (caseInsensitiveCompare(temp->category, category) &&
                caseInsensitiveCompare(temp->title, title)) {
                found = true;
                if (temp->availableCopies <= 0) {
                    cout << "Sorry, no copies of this book are currently available.\n";
                    return;
                }
                if (hasBorrowedSpecificBook(borrowerId, title, category)) {
                    cout << "Sorry, you have already borrowed a copy of this book.\n";
                    return;
                }
                // Check if book is already selected in this session
                for (int j = 0; j < borrowCount; j++) {
                    if (caseInsensitiveCompare(borrows[j].title, title) &&
                        caseInsensitiveCompare(borrows[j].category, category)) {
                        cout << "You have already selected this book in this borrowing session.\n";
                        return;
                    }
                }
                borrows[borrowCount] = {category, title, temp};
                borrowCount++;
                break;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "Book not found in category '" << category << "'. Borrowing cancelled.\n";
            return;
        }
    }

    displayBorrowRules();
    cout << "\nYou are about to borrow the following books:\n";
    for (int i = 0; i < borrowCount; i++) {
        cout << i+1 << ". " << borrows[i].title << " (" << borrows[i].category << ")\n";
    }

    char confirm;
    cout << "Do you want to proceed with borrowing these " << borrowCount << " books? (y/n): ";
    cin >> confirm;
    cin.ignore();

    if (tolower(confirm) != 'y') {
        cout << "Borrowing cancelled.\n";
        return;
    }

    string borrowDate = getCurrentDateTime();
    string returnDate = calculateReturnDate(14);

    cout << "\n--------- Borrowing Confirmation -----------\n";
    cout << " Borrower Name: " << borrowerName << "\n";
    cout << " Borrower ID: " << borrowerId << "\n";
    cout << " Borrow Date: " << borrowDate << "\n";
    cout << " Due Date: " << returnDate << "\n";
    cout << " Books Borrowed:\n";

    for (int i = 0; i < borrowCount; i++) {
        borrows[i].book->availableCopies -= 1;
        addBorrowRecord(borrows[i].title, borrows[i].category, borrowerName,
                       borrowerId, 1, borrowDate, returnDate);
        cout << " - " << borrows[i].title << " (" << borrows[i].category << ")\n";
    }

    saveToFile();
    cout << "------------------------------------------\n";
    cout << "Thank you for borrowing from our library!\n";
}

void returnBook() {
    string category = selectCategory();
    if (category.empty()) {
        cout << "Returning cancelled.\n";
        return;
    }

    displayBooksByCategory(category);

    string title;
    cout << "Enter the title of the book you want to return: ";
    getline(cin, title);

    string borrowerName, borrowerId;
    do {
        cout << "Enter your full name (minimum 3 letters, no numbers): ";
        getline(cin, borrowerName);
    } while (!isValidInput(borrowerName, "^[a-zA-Z ]+$", 3));

    do {
        cout << "Enter your ID (minimum 3 characters, letters or numbers): ";
        getline(cin, borrowerId);
        if (borrowerId.length() < 3) {
            cout << "ID must be at least 3 characters long.\n";
        }
    } while (borrowerId.length() < 3);

    Book* book = head;
    while (book) {
        if (caseInsensitiveCompare(book->category, category) &&
            caseInsensitiveCompare(book->title, title)) {
            break;
        }
        book = book->next;
    }

    if (!book) {
        cout << "Book not found in category '" << category << "'.\n";
        return;
    }

    BorrowRecord* currentRecord = borrowHead;
    BorrowRecord* selectedRecord = NULL;

    while (currentRecord) {
        if (caseInsensitiveCompare(currentRecord->bookTitle, title) &&
            caseInsensitiveCompare(currentRecord->bookCategory, category) &&
            caseInsensitiveCompare(currentRecord->borrowerName, borrowerName) &&
            caseInsensitiveCompare(currentRecord->borrowerId, borrowerId) &&
            !currentRecord->returned) {
            selectedRecord = currentRecord;
            break;
        }
        currentRecord = currentRecord->next;
    }

    if (!selectedRecord) {
        cout << "No matching active borrow record found.\n";
        return;
    }

    cout << "\nFound borrow record:\n";
    cout << "Borrowed 1 copy on " << selectedRecord->borrowDate
         << " (Due: " << selectedRecord->returnDate << ")\n";

    char confirm;
    cout << "Confirm return of 1 copy of '" << title << "'? (y/n): ";
    cin >> confirm;
    cin.ignore();

    if (tolower(confirm) != 'y') {
        cout << "Return cancelled.\n";
        return;
    }

    string returnDate = getCurrentDateTime();
    int daysLate = daysBetweenDates(selectedRecord->returnDate, returnDate);

    book->availableCopies += 1;
    saveToFile();

    selectedRecord->borrowedCopies = 0;
    selectedRecord->returned = true;
    saveBorrowRecords();

    cout << "\n>>>>>>> Return Confirmation <<<<<<<<\n";
    cout << " Book Title: " << title << "\n";
    cout << " Category: " << category << "\n";
    cout << " Borrower Name: " << borrowerName << "\n";
    cout << " Borrower ID: " << borrowerId << "\n";
    cout << " Copies Returned: 1\n";
    cout << " Return Date: " << returnDate << "\n";

    if (daysLate > 0) {
        int fine = daysLate * 5;
        cout << " WARNING: This return is " << daysLate << " days late!\n";
        cout << " Fine imposed: " << fine << " birr\n";
        cout << " Please pay the fine at the library desk.\n";
    } else {
        cout << " Book returned on time. Thank you!\n";
    }
 cout << "------------------------------------\n";
}


void deleteAllBooks() {
    int choice;
    cout << "\n-------- Delete Options --------\n";
    cout << "1. Delete All Books in a Specific Category\n";
    cout << "2. Delete All Books in the Library\n";
    cout << "3. Cancel and Return to Main Menu\n";

    if (!getSafeInt(choice, "Enter your choice (1-3): ", 1, 3)) return;

    if (choice == 3) {
        cout << "Delete operation cancelled.\n";
        return;
    }

    string category;
    if (choice == 1) {
        category = selectCategory();
        if (category.empty()) {
            cout << "Delete operation cancelled.\n";
            return;
        }
    }

    cout << "\nWARNING: This action cannot be undone!\n";
    if (choice == 1) {
        cout << "You are about to delete ALL books in category: " << category << "\n";
    } else {
        cout << "You are about to delete ALL books in the library!\n";
    }

    char confirm;
    cout << "Are you sure you want to proceed? (y/n): ";
    cin >> confirm;
    cin.ignore();

    if (tolower(confirm) != 'y') {
        cout << "Delete operation cancelled.\n";
        return;
    }

    string deletionTime = getCurrentDateTime();
    int deletedCount = 0;
    Book* current = head;
    Book* nextNode;

    while (current) {
        nextNode = current->next;
        if (choice == 2 || (choice == 1 && caseInsensitiveCompare(current->category, category))) {
            if (current->prev)
                current->prev->next = current->next;
            else
                head = current->next;

            if (current->next)
                current->next->prev = current->prev;

            delete current;
            deletedCount++;
        }
        current = nextNode;
    }

    ofstream logfile("library_deletions.log", ios::app);
    if (logfile.is_open()) {
        if (choice == 1) {
            logfile << "Deleted ALL books in category '" << category
                   << "' (" << deletedCount << " books) on " << deletionTime << "\n";
        } else {
            logfile << "Deleted ALL books in library (" << deletedCount
                   << " books) on " << deletionTime << "\n";
        }
        logfile.close();
    }

    saveToFile();
    cout << "\n------ Deletion Summary ------\n";
    cout << "Time of deletion: " << deletionTime << "\n";
    if (choice == 1) {
        cout << "Category deleted: " << category << "\n";
    }
    cout << "Number of books deleted: " << deletedCount << "\n";
    cout << "Operation completed successfully.\n";
}

void sortBooks() {
    sortBooksMenu();
}

void cleanup() {
    Book* tempBook;
    while (head) {
        tempBook = head;
        head = head->next;
        delete tempBook;
    }

    BorrowRecord* tempRecord;
    while (borrowHead) {
        tempRecord = borrowHead;
        borrowHead = borrowHead->next;
        delete tempRecord;
    }
}

int getMenuChoice() {
    int choice;
    while (true) {
        cout << "\n>>>>>>>>>>  DILLA UNIVERSITY LIBRARY MANAGEMENT SYSTEM  <<<<<<<<<<<\n";
         cout << "\n----------  Library Menu ---------\n";
        cout << "1.  Add Books\n";
        cout << "2.  Display Books\n";
        cout << "3.  Search Book (by category)\n";
        cout << "4.  Delete Book Copies\n";
        cout << "5.  Count Books\n";
        cout << "6.  Sort Books\n";
        cout << "7.  Delete All Books\n";
        cout << "8.  Update Book\n";
        cout << "9.  Borrow One Book\n";
        cout << "10. Borrow Multiple Books\n";
        cout << "11. Return Book\n";
        cout << "12. Exit\n";
        if (getSafeInt(choice, "Enter your choice (1-12): ", 1, 12)) {
            return choice;
        }
    }
}

int main() {
    loadFromFile();
    loadBorrowRecords();
    int choice;

    do {
        choice = getMenuChoice();
        switch (choice) {
            case 1: addBooks(); break;
            case 2: displayBooksMenu(); break;
            case 3: searchBooks(); break;
            case 4: deleteBook(); break;
            case 5: countBooksMenu(); break;
            case 6: sortBooks(); break;
            case 7: deleteAllBooks(); break;
            case 8:  updateBook(); break;
            case 9:  borrowBook(); break;
            case 10: borrowMultipleBooks(); break;
            case 11: returnBook(); break;
            case 12: cout << "Exiting Library System.\n"; break;
        }
    } while (choice != 12);

    cleanup();
    return 0;
}
