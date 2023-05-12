#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <string>
#include <stdio.h>
using namespace std;

string STATE_ORDER[7] = { "Full Name", "Date of Birth", "Sex", "Student Card", "Institute", "Department", "Level"};

struct Node {
    string val;
    Node* next;
    Node(string _val) : val(_val), next(nullptr){}
};

struct List {
    Node* first;
    Node* last;
    List() : first(nullptr), last(nullptr){}

    bool is_empty() {
        return first == nullptr;
    }
};

struct Date {
    int day, month, year;
    Date() : day(1), month(1), year(1901) {};
    Date(int _day, int _month, int _year) : day(_day), month(_month), year(_year){}
    Date(string data) {
        stringstream ss(data);
        string buf;
        int counter = 0;
        while (getline(ss, buf, '.')) {
            switch (counter) {
                case 0:
                    day = stoi(buf);
                case 1:
                    month = stoi(buf);
                case 2:
                    year = stoi(buf);
            }
            counter++;
        }
    }
    string returnStringData() {
        string sDay, sMonth, sYear = to_string(year);
        if (to_string(day).length() == 1) {
            sDay = '0' + to_string(day);
        }
        else {
            sDay = to_string(day);
        }
        if (to_string(month).length() == 1) {
            sMonth = '0' + to_string(month);
        }
        else {
            sMonth = to_string(month);
        }
        return sDay + '.' + sMonth + '.' + sYear;
    }
};

class Student {
    protected:
        string fullName;
        Date dateOfBirth;
        char sex;
        char cardNumber[8];
        string institute;
        string department;
        char level[3];
        Student(string _fullName = "UNDEFINED", string dob = "01.01.1901", char _sex = '?', char _cardNumber = *"???????", string _institute = "UNDEFINED", string _department = "UNDEFINED", char _level = *"??") {
            fullName = _fullName;
            dateOfBirth = Date(dob);
            sex = _sex;
            *cardNumber = _cardNumber;
            institute = _institute;
            department = _department;
            *level = _level;
        }
    public:
        string showDatabaseData() {
            return fullName + ";" + dateOfBirth.returnStringData() + ";" + sex + ";" + cardNumber + ";" + institute + ";" + department + ";" + level;
        }
};


void add_student() {
    string order[7] = { "Введите ФИО студента: ", "День рождения в формате ДД.ММ.ГГГГ: ", "Пол(М - мужской, Ж - женский): ", "Шифр студента (формат: 01Б2345): ", "Институт студента: ", "Шифр кафедры студента: ", "Курс в формате (Б - бакалавриат, М - магистратура, А - аспирантура)(цифра курса): " };
    ofstream database;
    char data[100];
    database.open(".\\database.txt");
    for (int i = 0; i < 7; i++) {
        cout << order[i];
        cin.getline(data, 100);
        database << data << ';';
    }
    database << '\n';
    cout << "Студент успешно добавлен!";
}

void show_students() {
    ifstream database;
    string s;
    char str[100];
    string word;
    char* ptr = NULL;
    char* next_ptr = NULL;
    database.open(".\\database.txt");
    int counter;
    for (database.getline(str, 100, 10); !(database.eof()); database.getline(str, 100, 10)) {
        stringstream ss(str);
        counter = 0;
        while (getline(ss, word, ';')) {
            cout << STATE_ORDER[counter] << ": " << word << endl;
            counter++;
        }
        cout << "-----" << endl;
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    return 0;
}