#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

const int MAX_COURSES = 1000;
const int MAX_USERS = 1000;
const string USERS_FILE = "users.txt";
const string COURSES_FILE = "courses.txt";

// Parallel arrays for courses
string courseIds[MAX_COURSES];
string titles[MAX_COURSES];
int creditHours[MAX_COURSES];
int courseCount = 0;

// Parallel arrays for users
string usernames[MAX_USERS];
string passwords[MAX_USERS];
int userCount = 0;

// ----------------- Utility functions -----------------
string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// Split a line by a delimiter (manual, without vectors)
int splitLine(const string &line, char delim, string parts[], int maxParts) {
    int count = 0;
    string cur = "";
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == delim) {
            if (count < maxParts) parts[count++] = cur;
            cur = "";
        } else {
            cur += line[i];
        }
    }
    if (count < maxParts) parts[count++] = cur;
    return count;
}

// ----------------- File I/O for users -----------------
void loadUsers() {
    userCount = 0;
    ifstream fin(USERS_FILE.c_str());
    if (!fin.is_open()) return;
    string line;
    while (getline(fin, line) && userCount < MAX_USERS) {
        line = trim(line);
        if (line.empty()) continue;
        string parts[2];
        int n = splitLine(line, ',', parts, 2);
        if (n == 2) {
            usernames[userCount] = trim(parts[0]);
            passwords[userCount] = trim(parts[1]);
            userCount++;
        }
    }
    fin.close();
}

bool saveUserToFile(const string &username, const string &password) {
    ofstream fout(USERS_FILE.c_str(), ios::app);
    if (!fout.is_open()) return false;
    fout << username << "," << password << "\n";
    fout.close();
    return true;
}

bool usernameExists(const string &username) {
    for (int i = 0; i < userCount; i++) {
        if (usernames[i] == username) return true;
    }
    return false;
}

bool registerUser() {
    string username, password, password2;
    cout << "=== Register ===\n";
    cout << "Enter username: ";
    getline(cin, username);
    username = trim(username);
    if (username.empty()) { cout << "Username cannot be empty.\n"; return false; }
    if (usernameExists(username)) { cout << "Username already exists.\n"; return false; }
    cout << "Enter password: ";
    getline(cin, password);
    cout << "Confirm password: ";
    getline(cin, password2);
    if (password != password2) { cout << "Passwords do not match.\n"; return false; }
    if (userCount >= MAX_USERS) { cout << "User storage full.\n"; return false; }
    usernames[userCount] = username;
    passwords[userCount] = password;
    userCount++;
    if (!saveUserToFile(username, password)) {
        cout << "Warning: failed to write to " << USERS_FILE << "\n";
    }
    cout << "Registration successful.\n";
    return true;
}

int loginUser() {
    string username, password;
    cout << "=== Login ===\n";
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);
    for (int i = 0; i < userCount; i++) {
        if (usernames[i] == username && passwords[i] == password) {
            cout << "Login successful. Welcome " << username << "!\n";
            return i;
        }
    }
    cout << "Invalid credentials.\n";
    return -1;
}

// ----------------- File I/O for courses -----------------
void loadCourses() {
    courseCount = 0;
    ifstream fin(COURSES_FILE.c_str());
    if (!fin.is_open()) return;
    string line;
    while (getline(fin, line) && courseCount < MAX_COURSES) {
        line = trim(line);
        if (line.empty()) continue;
        string parts[3];
        int n = splitLine(line, ',', parts, 3);
        if (n == 3) {
            courseIds[courseCount] = trim(parts[0]);
            titles[courseCount] = trim(parts[1]);
            creditHours[courseCount] = atoi(parts[2].c_str());
            courseCount++;
        }
    }
    fin.close();
}

bool rewriteCoursesFile() {
    ofstream fout(COURSES_FILE.c_str(), ios::trunc);
    if (!fout.is_open()) return false;
    for (int i = 0; i < courseCount; i++) {
        fout << courseIds[i] << "," << titles[i] << "," << creditHours[i] << "\n";
    }
    fout.close();
    return true;
}

bool courseIdExists(const string &cid, int &indexOut) {
    for (int i = 0; i < courseCount; i++) {
        if (courseIds[i] == cid) {
            indexOut = i;
            return true;
        }
    }
    return false;
}

// ----------------- Course operations -----------------
void listAllCourses() {
    cout << "\n=== All Courses (" << courseCount << ") ===\n";
    if (courseCount == 0) { cout << "No courses available.\n"; return; }
    for (int i = 0; i < courseCount; i++) {
        cout << i+1 << ". " << courseIds[i] << " | " << titles[i] << " | Credits: " << creditHours[i] << "\n";
    }
}

void addCourse() {
    if (courseCount >= MAX_COURSES) { cout << "Course storage full.\n"; return; }
    string cid, ttl, credStr;
    int cred;
    cout << "Enter courseId: ";
    getline(cin, cid);
    cid = trim(cid);
    int idx;
    if (cid.empty()) { cout << "courseId cannot be empty.\n"; return; }
    if (courseIdExists(cid, idx)) { cout << "courseId already exists.\n"; return; }
    cout << "Enter title: ";
    getline(cin, ttl);
    ttl = trim(ttl);
    cout << "Enter credit hours (1-6): ";
    getline(cin, credStr);
    cred = atoi(credStr.c_str());
    if (cred < 1 || cred > 6) { cout << "Invalid credit hours.\n"; return; }
    courseIds[courseCount] = cid;
    titles[courseCount] = ttl;
    creditHours[courseCount] = cred;
    courseCount++;
    rewriteCoursesFile();
    cout << "Course added successfully.\n";
}

void deleteCourse() {
    string cid;
    cout << "Enter courseId to delete: ";
    getline(cin, cid);
    cid = trim(cid);
    int idx;
    if (!courseIdExists(cid, idx)) { cout << "Course not found.\n"; return; }
    for (int i = idx; i < courseCount-1; i++) {
        courseIds[i] = courseIds[i+1];
        titles[i] = titles[i+1];
        creditHours[i] = creditHours[i+1];
    }
    courseCount--;
    rewriteCoursesFile();
    cout << "Course deleted.\n";
}

void updateCourse() {
    string cid;
    cout << "Enter courseId to update: ";
    getline(cin, cid);
    int idx;
    if (!courseIdExists(cid, idx)) { cout << "Course not found.\n"; return; }
    cout << "Found: " << courseIds[idx] << " | " << titles[idx] << " | " << creditHours[idx] << "\n";
    cout << "Enter new title (leave blank to keep): ";
    string newTitle; getline(cin, newTitle);
    newTitle = trim(newTitle);
    if (!newTitle.empty()) titles[idx] = newTitle;
    cout << "Enter new credit hours (leave blank to keep): ";
    string credStr; getline(cin, credStr);
    credStr = trim(credStr);
    if (!credStr.empty()) {
        int cred = atoi(credStr.c_str());
        if (cred >= 1 && cred <= 6) creditHours[idx] = cred;
        else { cout << "Invalid credit hours.\n"; return; }
    }
    rewriteCoursesFile();
    cout << "Course updated.\n";
}

void searchCourse() {
    cout << "Search by (1) courseId or (2) title substring: ";
    string choice; getline(cin, choice);
    if (choice == "1") {
        string cid; cout << "Enter courseId: "; getline(cin, cid);
        int idx;
        if (courseIdExists(cid, idx)) {
            cout << "Found: " << courseIds[idx] << " | " << titles[idx] << " | " << creditHours[idx] << "\n";
        } else cout << "Not found.\n";
    } else if (choice == "2") {
        string sub; cout << "Enter title substring: "; getline(cin, sub);
        bool any = false;
        for (int i = 0; i < courseCount; i++) {
            string t = titles[i];
            string tl = t, sl = sub;
            for (size_t j = 0; j < tl.size(); j++) tl[j] = tolower(tl[j]);
            for (size_t j = 0; j < sl.size(); j++) sl[j] = tolower(sl[j]);
            if (tl.find(sl) != string::npos) {
                cout << courseIds[i] << " | " << titles[i] << " | " << creditHours[i] << "\n";
                any = true;
            }
        }
        if (!any) cout << "No matches.\n";
    }
}

// ----------------- Menu -----------------
void showMenu() {
    cout << "\n=== Course Management Menu ===\n";
    cout << "1. Add Course\n";
    cout << "2. Delete Course\n";
    cout << "3. Search Course\n";
    cout << "4. Update Course\n";
    cout << "5. List All Courses\n";
    cout << "6. Logout\n";
    cout << "7. Exit Program\n";
    cout << "Enter choice: ";
}

int main() {
    loadUsers();
    loadCourses();

    cout << "Welcome to DIT400 Course Management System\n";

    while (true) {
        cout << "\nChoose: (1) Login (2) Register (3) Exit\n> ";
        string opt; getline(cin, opt);
        if (opt == "1") {
            int userIdx = loginUser();
            if (userIdx >= 0) {
                bool loggedIn = true;
                while (loggedIn) {
                    showMenu();
                    string ch; getline(cin, ch);
                    if (ch == "1") addCourse();
                    else if (ch == "2") deleteCourse();
                    else if (ch == "3") searchCourse();
                    else if (ch == "4") updateCourse();
                    else if (ch == "5") listAllCourses();
                    else if (ch == "6") { cout << "Logged out.\n"; loggedIn = false; }
                    else if (ch == "7") { cout << "Goodbye!\n"; return 0; }
                    else cout << "Invalid choice.\n";
                }
            }
        } else if (opt == "2") {
            registerUser();
        } else if (opt == "3") {
            cout << "Goodbye.\n";
            break;
        } else {
            cout << "Invalid option.\n";
        }
    }
    return 0;
}
