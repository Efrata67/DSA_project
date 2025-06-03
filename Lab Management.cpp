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