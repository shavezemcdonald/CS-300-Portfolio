#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Shaveze McDonald
// Course      : CS 300
// Project     : Project Two
// Description : Advising Assistance Program using a hash table
//============================================================================

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

struct Node {
    Course course;
    Node* next;

    Node() {
        next = nullptr;
    }

    Node(Course aCourse) {
        course = aCourse;
        next = nullptr;
    }
};

const unsigned int DEFAULT_SIZE = 179;

Node* hashTable[DEFAULT_SIZE];

string toUpperCase(string text) {
    for (char& ch : text) {
        ch = toupper(ch);
    }
    return text;
}

unsigned int hashFunction(string key) {
    unsigned int hash = 0;

    for (char ch : key) {
        hash += ch;
    }

    return hash % DEFAULT_SIZE;
}

void initializeTable() {
    for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
        hashTable[i] = nullptr;
    }
}

void insertCourse(Course course) {
    unsigned int index = hashFunction(course.courseNumber);

    if (hashTable[index] == nullptr) {
        hashTable[index] = new Node(course);
    }
    else {
        Node* current = hashTable[index];

        while (current->next != nullptr) {
            current = current->next;
        }

        current->next = new Node(course);
    }
}

Course* searchCourse(string courseNumber) {
    string searchKey = toUpperCase(courseNumber);
    unsigned int index = hashFunction(searchKey);
    Node* current = hashTable[index];

    while (current != nullptr) {
        if (toUpperCase(current->course.courseNumber) == searchKey) {
            return &(current->course);
        }
        current = current->next;
    }

    return nullptr;
}

vector<Course> getAllCourses() {
    vector<Course> allCourses;

    for (unsigned int i = 0; i < DEFAULT_SIZE; i++) {
        Node* current = hashTable[i];

        while (current != nullptr) {
            allCourses.push_back(current->course);
            current = current->next;
        }
    }

    return allCourses;
}

void printCourseList() {
    vector<Course> allCourses = getAllCourses();

    sort(allCourses.begin(), allCourses.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
        });

    cout << "Here is a sample schedule:" << endl << endl;

    for (const Course& course : allCourses) {
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }
}

void printCourseInformation(string courseNumber) {
    Course* course = searchCourse(courseNumber);

    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->courseTitle << endl;

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";

        for (size_t i = 0; i < course->prerequisites.size(); i++) {
            cout << course->prerequisites[i];

            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

void loadData(string fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "File could not be opened." << endl;
        return;
    }

    vector<string> lines;
    vector<string> allCourseNumbers;
    string line;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        lines.push_back(line);

        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 2) {
            allCourseNumbers.push_back(toUpperCase(tokens[0]));
        }
    }

    file.close();

    for (string currentLine : lines) {
        stringstream ss(currentLine);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 2) {
            cout << "Invalid course record found." << endl;
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(tokens[0]);
        course.courseTitle = tokens[1];

        bool validCourse = true;

        for (size_t i = 2; i < tokens.size(); i++) {
            string prereq = toUpperCase(tokens[i]);
            bool found = false;

            for (string existingCourse : allCourseNumbers) {
                if (existingCourse == prereq) {
                    found = true;
                    break;
                }
            }

            if (found) {
                course.prerequisites.push_back(prereq);
            }
            else {
                cout << "Warning: prerequisite " << prereq
                    << " for course " << course.courseNumber
                    << " was not found." << endl;
                validCourse = false;
            }
        }

        if (validCourse) {
            insertCourse(course);
        }
    }

    cout << "Courses loaded successfully." << endl;
}

int main() {
    initializeTable();

    int choice;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl << endl;

    while (true) {
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input." << endl << endl;
            continue;
        }

        switch (choice) {
        case 1:
            loadData("C:\\Users\\deere\\source\\repos\\ProjectTwo\\x64\\Debug\\ABCU.csv.csv");
            cout << endl;
            break;

        case 2:
            printCourseList();
            cout << endl;
            break;

        case 3:
            cout << "What course do you want to know about? ";
            cin >> courseNumber;
            cout << endl;
            printCourseInformation(courseNumber);
            cout << endl;
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            return 0;

        default:
            cout << choice << " is not a valid option." << endl << endl;
            break;
        }
    }
}