// Enhanced Library Tracker with additional DSA concepts
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <tuple>
#include <queue>
#include <set>
using namespace std;
// ------------------- DS -------------------

struct Book {
    int id;
    string title;
    string author;
    bool isAvailable;
    Book() : isAvailable(true) {}
    Book(int i, string t, string a) : id(i), title(t), author(a), isAvailable(true) {}
};

struct Member {
    int id;
    string name;
    vector<int> borrowedBooks;  // Track books currently borrowed
    
    Member() {}
    Member(int i, string n) : id(i), name(n) {}
};

// Issue record with timestamp simulation
struct IssueRecord {
    int bookId;
    int memberId;
    int issueTime;  // Simple counter for chronological order
    
    IssueRecord(int b, int m, int t) : bookId(b), memberId(m), issueTime(t) {}
};

// ------------------- Global Containers{GC} -------------------

unordered_map<int, Book> books;
unordered_map<int, Member> members;
vector<IssueRecord> issuedRecords;
unordered_set<int> availableBooks;  // Set for quick availability check
priority_queue<int, vector<int>, greater<int>> recycledBookIds;  // Min-heap for ID reuse
priority_queue<int, vector<int>, greater<int>> recycledMemberIds;

int nextBookId = 1;
int nextMemberId = 1;
int issueCounter = 0;


int getNextBookId() {
    if (!recycledBookIds.empty()) {
        int id = recycledBookIds.top();
        recycledBookIds.pop();
        return id;
    }
    return nextBookId++;
}

int getNextMemberId() {
    if (!recycledMemberIds.empty()) {
        int id = recycledMemberIds.top();
        recycledMemberIds.pop();
        return id;
    }
    return nextMemberId++;
}

void addBook() {
    int id = getNextBookId();
    string title, author;
    cout << "Enter title: "; getline(cin, title);
    cout << "Enter author: "; getline(cin, author);
    
    books[id] = Book(id, title, author);
    availableBooks.insert(id);
    cout << "Book added with ID " << id << "\n";
}

void removeBook() {
    int id;
    cout << "Enter book ID to remove: "; cin >> id; cin.ignore();
    
    if (books.find(id) == books.end()) {
        cout << "Book not found.\n";
        return;
    }
    
    if (!books[id].isAvailable) {
        cout << "Cannot remove: Book is currently issued.\n";
        return;
    }
    
    books.erase(id);
    availableBooks.erase(id);
    recycledBookIds.push(id);
    cout << "Book removed.\n";
}

void searchBooks() {
    cout << "Search by: 1.Title 2.Author 3.Availability\nChoice: ";
    int choice; cin >> choice; cin.ignore();
    
    vector<Book> results;
    
    if (choice == 3) {
        cout << "Available books:\n";
        for (int id : availableBooks) {
            results.push_back(books[id]);
        }
    } else {
        string key;
        if (choice == 1) {
            cout << "Enter title: "; getline(cin, key);
        } else {
            cout << "Enter author: "; getline(cin, key);
        }
        
        for (auto& [id, book] : books) {
            bool matches = (choice == 1 && book.title.find(key) != string::npos) ||
                          (choice == 2 && book.author.find(key) != string::npos);
            if (matches) results.push_back(book);
        }
    }
    
    sort(results.begin(), results.end(), 
         [](const Book& a, const Book& b) { return a.title < b.title; });
    
    if (results.empty()) {
        cout << "No books match.\n";
    } else {
        for (const Book& book : results) {
            cout << "ID: " << book.id << ", Title: " << book.title 
                 << ", Author: " << book.author 
                 << " [" << (book.isAvailable ? "Available" : "Issued") << "]\n";
        }
    }
}

void addMember() {
    int id = getNextMemberId();
    string name;
    cout << "Enter member name: "; getline(cin, name);
    members[id] = Member(id, name);
    cout << "Member added with ID " << id << "\n";
}

void removeMember() {
    int id;
    cout << "Enter member ID to remove: "; cin >> id; cin.ignore();
    
    if (members.find(id) == members.end()) {
        cout << "Member not found.\n";
        return;
    }
    
    if (!members[id].borrowedBooks.empty()) {
        cout << "Cannot remove: Member has borrowed books.\n";
        return;
    }
    
    members.erase(id);
    recycledMemberIds.push(id);
    cout << "Member removed.\n";
}

void issueBook() {
    int bid, mid;
    cout << "Enter book ID: "; cin >> bid;
    cout << "Enter member ID: "; cin >> mid; cin.ignore();
    
    if (books.find(bid) == books.end()) {
        cout << "Book not found.\n";
        return;
    }
    if (members.find(mid) == members.end()) {
        cout << "Member not found.\n";
        return;
    }
    if (!books[bid].isAvailable) {
        cout << "Book is already issued.\n";
        return;
    }
    
    // Issue the book
    books[bid].isAvailable = false;
    availableBooks.erase(bid);
    members[mid].borrowedBooks.push_back(bid);
    issuedRecords.emplace_back(bid, mid, ++issueCounter);
    
    cout << "Book '" << books[bid].title << "' issued to " << members[mid].name << "\n";
}

void returnBook() {
    int bid, mid;
    cout << "Enter book ID: "; cin >> bid;
    cout << "Enter member ID: "; cin >> mid; cin.ignore();
    
    // the issue record
    auto recordIt = find_if(issuedRecords.begin(), issuedRecords.end(),
        [bid, mid](const IssueRecord& r) { return r.bookId == bid && r.memberId == mid; });
    
    if (recordIt == issuedRecords.end()) {
        cout << "No such issue record found.\n";
        return;
    }
    
    // Return the book
    books[bid].isAvailable = true;
    availableBooks.insert(bid);
    
    auto& borrowedBooks = members[mid].borrowedBooks;
    borrowedBooks.erase(find(borrowedBooks.begin(), borrowedBooks.end(), bid));
    
    issuedRecords.erase(recordIt);
    
    cout << "Book returned successfully.\n";
}

void showMemberHistory() {
    int mid;
    cout << "Enter member ID: "; cin >> mid; cin.ignore();
    
    if (members.find(mid) == members.end()) {
        cout << "Member not found.\n";
        return;
    }
    
    cout << "Member: " << members[mid].name << "\n";
    cout << "Currently borrowed books:\n";
    for (int bookId : members[mid].borrowedBooks) {
        cout << "  - " << books[bookId].title << " (ID: " << bookId << ")\n";
    }
    
    cout << "Issue history:\n";
    vector<IssueRecord> memberRecords;
    for (const auto& record : issuedRecords) {
        if (record.memberId == mid) {
            memberRecords.push_back(record);
        }
    }
    
    sort(memberRecords.begin(), memberRecords.end(),
         [](const IssueRecord& a, const IssueRecord& b) { return a.issueTime > b.issueTime; });
    
    for (const auto& record : memberRecords) {
        cout << "  - " << books[record.bookId].title << " (Issue #" << record.issueTime << ")\n";
    }
}

void showStatistics() {
    cout << "\n=== Library Statistics ===\n";
    cout << "Total books: " << books.size() << "\n";
    cout << "Available books: " << availableBooks.size() << "\n";
    cout << "Issued books: " << (books.size() - availableBooks.size()) << "\n";
    cout << "Total members: " << members.size() << "\n";
    cout << "Total issues: " << issueCounter << "\n";
    
    // Most active member
    if (!members.empty()) {
        auto maxMember = max_element(members.begin(), members.end(),
            [](const pair<int, Member>& a, const pair<int, Member>& b) {
                return a.second.borrowedBooks.size() < b.second.borrowedBooks.size();
            });
        cout << "Most active member: " << maxMember->second.name 
             << " (" << maxMember->second.borrowedBooks.size() << " books)\n";
    }
}

void showAll() {
    cout << "\n=== All Books ===\n";
    vector<Book> sortedBooks;
    for (auto& [id, book] : books) sortedBooks.push_back(book);
    sort(sortedBooks.begin(), sortedBooks.end(),
         [](const Book& a, const Book& b) { return a.title < b.title; });
    
    for (const Book& book : sortedBooks) {
        cout << book.id << ": " << book.title << " by " << book.author
             << " [" << (book.isAvailable ? "Available" : "Issued") << "]\n";
    }
    
    cout << "\n=== All Members ===\n";
    for (auto& [id, member] : members) {
        cout << id << ": " << member.name << " (" << member.borrowedBooks.size() << " books)\n";
    }
    cout << "\n=== Current Issues ===\n";
    for (const auto& record : issuedRecords) {
        cout << "'" << books[record.bookId].title << "' -> " 
             << members[record.memberId].name << " (Issue #" << record.issueTime << ")\n";
    }
}

// ---------------Main Menu-------------------
int main() {
    while (true) {
        cout << "\n=== Enhanced Library Tracker ===\n"
             << "1. Add Book          2. Remove Book\n"
             << "3. Search Books      4. Add Member\n"
             << "5. Remove Member     6. Issue Book\n"
             << "7. Return Book       8. Show All\n"
             << "9. Member History    10. Statistics\n"
             << "11. Exit\n"
             << "Choice: ";
        
        int choice;
        cin >> choice; 
        cin.ignore();
        
        switch (choice) {
            case 1: addBook(); break;
            case 2: removeBook(); break;
            case 3: searchBooks(); break;
            case 4: addMember(); break;
            case 5: removeMember(); break;
            case 6: issueBook(); break;
            case 7: returnBook(); break;
            case 8: showAll(); break;
            case 9: showMemberHistory(); break;
            case 10: showStatistics(); break;
            case 11: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }
    return 0;
}
