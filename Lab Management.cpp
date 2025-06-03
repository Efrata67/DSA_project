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