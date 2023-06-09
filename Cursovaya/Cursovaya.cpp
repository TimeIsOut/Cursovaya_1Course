﻿#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;

void Crypt()
{
    srand(time(NULL));
    char* pass = new char[64];
    for (int i = 0; i < 64; ++i) {
        switch (rand() % 3) {
        case 0:
            pass[i] = rand() % 10 + '0';
            break;
        case 1:
            pass[i] = rand() % 26 + 'A';
            break;
        case 2:
            pass[i] = rand() % 26 + 'a';
        }
    }
    string command = "openssl\\bin\\openssl.exe enc -aes-256-cbc -salt -in database.txt -out database.txt.enc -pass pass:";
    command += pass;
    system(command.c_str());
    if (remove("database.txt") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
    ofstream file;
    file.open("key.txt", ios::binary);
    file.write(pass, 65);
    file.close();
    command = "openssl\\bin\\openssl.exe rsautl -encrypt -inkey rsa.public -pubin -in key.txt -out key.txt.enc";
        system(command.c_str());
    if (remove("key.txt") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
}


void Decrypt()
{
    string command = "openssl\\bin\\openssl.exe rsautl -decrypt -inkey rsa.private -in key.txt.enc -out key.txt";
    system(command.c_str());
    if (remove("key.txt.enc") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
    char* pass = new char[64];
    ifstream file;
    file.open("key.txt", ios::binary);
    file.read(pass, 65);
    file.close();
    if (remove("key.txt") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
    command = "openssl\\bin\\openssl.exe enc -aes-256-cbc -d -in database.txt.enc -out database.txt -pass pass:";
    command += pass;
    system(command.c_str());
    if (remove("database.txt.enc") != 0) {
        cout << "[ERROR] - deleting file" << endl;
    }
}

struct Node {
    string val;
    Node* next;
    Node(string _val) : val(_val), next(nullptr){}
};

class List {
    public:
        Node* first = nullptr;
        Node* last = nullptr;
        int length = 0;
        List() : first(), last(){}
        List(vector<string> array) {
            for (string line : array) {
                push_back(line);
            }
        }

        bool is_empty() {
            return first == nullptr;
        }

        void push_back(string _val) {
            Node* p = new Node(_val);
            if (is_empty()) {
                first = p;
                last = p;
                return;
            }
            last->next = p;
            last = p;
            length++;
        }

        void print() {
            if (is_empty()) return;
            Node* p = first;
            while (p) {
                cout << p->val << ' ';
                p = p->next;
            }
            cout << endl;
        }

        void remove_first() {
            if (is_empty()) return;
            Node* p = first;
            first = p->next;
            delete p;
            length--;
        }

        string pop() {
            if (is_empty()) return "";
            if (first == last) {
                string val = first->val;
                remove_first();
                length--;
                return val;
            }
            string val = last->val;
            Node* p = first;
            while (p->next != last) p = p->next;
            p->next = nullptr;
            delete last;
            last = p;
            length--;
            return val;
        }

        Node* operator[] (const int index) {
            if (is_empty()) return nullptr;
            Node* p = first;
            for (int i = 0; i < index; i++) {
                p = p->next;
                if (!p) return nullptr;
            }
            return p;
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
                    day = stoi(buf); break;
                case 1:
                    month = stoi(buf); break;
                case 2:
                    year = stoi(buf); break;
            }
            counter++;
        }
    }
    string return_string_data() {
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

    bool operator== (Date rhs) {
        return rhs.day == this->day && rhs.month == this->month && rhs.year == this->year;
    }

    bool operator!= (Date rhs) {
        return rhs.day != this->day || rhs.month != this->month || rhs.year != this->year;
    }

    bool operator< (Date rhs) {
        if (rhs.year != this->year) {
            return this->year < rhs.year;
        }
        if (rhs.month != this->month) {
            return this->month < rhs.month;
        }
        if (rhs.day != this->day) {
            return this->day < rhs.day;
        }
        return false;
    }

    bool operator> (Date rhs) {
        if (rhs.year != this->year) {
            return this->year > rhs.year;
        }
        if (rhs.month != this->month) {
            return this->month > rhs.month;
        }
        if (rhs.day != this->day) {
            return this->day > rhs.day;
        }
        return false;
    }

    bool operator<= (Date rhs) {
        if (rhs.year != this->year) {
            return this->year < rhs.year;
        }
        if (rhs.month != this->month) {
            return this->month < rhs.month;
        }
        if (rhs.day != this->day) {
            return this->day < rhs.day;
        }
        return true;
    }

    bool operator>= (Date rhs) {
        if (rhs.year != this->year) {
            return this->year > rhs.year;
        }
        if (rhs.month != this->month) {
            return this->month > rhs.month;
        }
        if (rhs.day != this->day) {
            return this->day > rhs.day;
        }
        return true;
    }
};

class Student {
    public:
        string fullName;
        Date dateOfBirth;
        string sex;
        string studentCard;
        string institute;
        string department;
        string course;
        List all_params;
        Student() : fullName("UNDEFINED"), dateOfBirth(Date()), sex("?"), studentCard("???????"), institute("?????"), department("??-??"), course("??") {};
        Student(string databaseLine) {
            stringstream ss(databaseLine);
            string buf;
            int counter = -1;
            while (getline(ss, buf, ';')) {
                switch (counter) {
                    case 0: fullName = buf; break;
                    case 1: dateOfBirth = Date(buf); break;
                    case 2: sex = buf; break;
                    case 3: studentCard = buf; break;
                    case 4: institute = buf; break;
                    case 5: department = buf; break;
                    case 6: course = buf; break;
                }
                counter++;
            }
            all_params = List({ fullName, dateOfBirth.return_string_data(), sex, studentCard, institute, department, course });
        }
        string return_string_data() {
            return all_params[0]->val + ';' + all_params[1]->val + ';' + all_params[2]->val + ';' + all_params[3]->val + ';' + all_params[4]->val + ';' + all_params[5]->val + ';' + all_params[6]->val + ';';
        }
};

class DatabaseWorker {
    protected:
        string databasePath = ".\\database.txt";
        ifstream databaseIn;
        ofstream databaseOut;
        List stateOrder = List({ "Full Name", "Date of Birth", "Sex", "Student Card", "Institute", "Department", "Course"});
        List addingOrder = List({ "Введите ФИО студента: ", "День рождения в формате ДД.ММ.ГГГГ: ", "Пол(М - мужской, Ж - женский): ", "Шифр студента (формат: 01Б2345): ", "Институт студента: ", "Шифр кафедры студента: ", "Курс в формате (Б - бакалавриат, М - магистратура, А - аспирантура)(цифра курса): " });
    public:
        void add_student() {
            string data;
            Decrypt();
            databaseOut.open(databasePath, ios_base::app);
            databaseOut << "0;";
            for (int i = 0; i < 7; i++) {
                cout << addingOrder[i]->val;
                getline(cin >> ws, data);
                databaseOut << data << ';';
            }
            databaseOut << endl;
            int courseCount;
            cout << "Сколько семестров прошёл студент? ";
            cin >> courseCount;
            for (int counter = 1; counter <= courseCount; counter++) {
                cout << counter << " семестр -----" << endl;
                databaseOut << counter << ';';
                string courseName;
                int courseMark;
                do {
                    cout << "Введите название предмета или 0, если все предметы были указаны и Вы хотите перейти к следующему семестру: ";
                    getline(cin >> ws, courseName);
                    if (courseName != "0") {
                        cout << "Введите оценку: ";
                        cin >> courseMark;
                        databaseOut << courseName << ";" << courseMark << ";";
                    }
                } while (courseName != "0");
                databaseOut << endl;
            }
            databaseOut.close();
            Crypt();
            cout << "Студент успешно добавлен!";
        }

        void change_student() {
            bool flag = true;
            char studentCard[8];
            cout << "Пожалуйста, введите шифр студента, подлежащий изменению (формат 01А2345): ";
            cin >> studentCard;
            Decrypt();
            databaseIn.open(databasePath);
            List dataHolder = List();
            char str[1000];
            for (databaseIn.getline(str, 1000, 10); !(databaseIn.eof()); databaseIn.getline(str, 1000, 10)) {
                if (!(strstr(str, studentCard))) { 
                    dataHolder.push_back(str);
                }
                else {
                    string line(str);
                    Student changedStudent(line);
                    int counter = 1;
                    cout << "Что бы вы хотели поменять? Напишите цифру Вашего варианта:" << endl;
                    for (Node* node = stateOrder.first; node != nullptr; node = node->next, counter++) {
                        cout << counter << ": " << node->val << endl;
                    }
                    cout << "8: Добавить новый семестр(ы)" << endl;
                    cout << "9: Поменять оценку в пройденном семестре" << endl;
                    int number;
                    cout << "Ваша цифра: ";
                    cin >> number;
                    string newData;
                    if (number != 8 && number != 9) {
                        cout << addingOrder[number - 1]->val;
                        getline(cin >> ws, newData);
                        changedStudent.all_params[number - 1]->val = newData;
                        dataHolder.push_back(changedStudent.return_string_data());
                        flag = false;
                    }
                    else if (number == 9) {
                        int semesterNumber;
                        cout << "В каком семестре вы хотели бы изменить/добавить оценку? ";
                        cin >> semesterNumber;
                        do {
                            string line(str);
                            dataHolder.push_back(line);
                            databaseIn.getline(str, 1000, 10);

                        } while (str[0] - '0' != semesterNumber && str[0] != '0' && !(databaseIn.eof()));
                        if (str[0] - '0' != semesterNumber) {
                            cout << "Такого семестра у студента не было." << endl;
                            flag = false;
                            continue;
                        }
                        else {
                            string semesterName;
                            cout << "Какой предмет вы хотели бы изменить/добавить? ";
                            getline(cin >> ws, semesterName);
                            string strCopy(str);
                            int index = strCopy.find(semesterName);
                            int newMark;
                            if (index != string::npos) {
                                cout << "Новая оценка: ";
                                cin >> newMark;
                                strCopy[index + semesterName.length() + 1] = to_string(newMark)[0];
                                dataHolder.push_back(strCopy);
                                flag = false;
                            }
                            else {
                                cout << "Новая оценка: ";
                                cin >> newMark;
                                dataHolder.push_back(strCopy + semesterName + ";" + to_string(newMark) + ";");
                                flag = false;
                            }
                        }
                    }
                    else if (number == 8) {
                        int counter = 0;
                        do {
                            string line(str);
                            dataHolder.push_back(line);
                            databaseIn.getline(str, 1000, 10);
                            counter++;
                        } while (str[0] != '0' && !(databaseIn.eof()));
                        string newLine = to_string(counter) + ';';
                        cout << counter << " семестр -----" << endl;
                        string adding;
                        while (true) {
                            cout << "Введите название предмета, который хотели бы добавить в семестр или 0, чтобы закончить заполнение: ";
                            getline(cin >> ws, adding);
                            if (adding == "0") break;
                            newLine = newLine + adding + ';';
                            cout << "Оценка: ";
                            getline(cin >> ws, adding);
                            newLine = newLine + adding + ';';
                        }
                        dataHolder.push_back(newLine);
                        dataHolder.push_back(str);
                        flag = false;
                        continue;
                    }
                }
            }
            databaseIn.close();
            databaseOut.open(databasePath);
            for (int i = 0; i <= dataHolder.length; i++) {
                databaseOut << dataHolder[i]->val << endl;
            }
            databaseOut.close();
            Crypt();
            if (flag) {
                cout << "Такой студент не был найден." << endl;
            }
            else {
                cout << "Студент успешно изменён!" << endl;
            }
        }

        void delete_student() {
            char studentCard[8];
            cout << "Пожалуйста, введите шифр студента, подлежащий удалению (формат 01Б2345): ";
            cin >> studentCard;
            Decrypt();
            databaseIn.open(databasePath);
            char str[1000];
            bool flag = true;
            List dataHolder = List();
            for (databaseIn.getline(str, 1000, 10); !(databaseIn.eof()); databaseIn.getline(str, 1000, 10)) {
                if (!(strstr(str, studentCard))) dataHolder.push_back(str);
                else {
                    flag = false;
                    do {
                        databaseIn.getline(str, 1000, 10);
                    } while (str[0] != '0' && !(databaseIn.eof()));
                    dataHolder.push_back(str);
                }
            }
            databaseIn.close();
            databaseOut.open(".\\database.txt");
            for (int i = 0; i < dataHolder.length + 1; i++) {
                databaseOut << dataHolder[i]->val << endl;
            }
            databaseOut.close();
            Crypt();
            if (flag) {
                cout << "Такой студент не был найден." << endl;
            }
            else {
                cout << "Студент успешно удалён!" << endl;
            }
        }

        void show_students() {
            string s;
            char str[1000];
            string word;
            char* ptr = NULL;
            char* next_ptr = NULL;
            Decrypt();
            databaseIn.open(databasePath);
            int counter;
            for (databaseIn.getline(str, 1000, 10); !(databaseIn.eof()); databaseIn.getline(str, 1000, 10)) {
                stringstream ss(str);
                string buf;
                char firstSymbol = str[0];
                if (firstSymbol != '0') {
                    getline(ss, buf, ';');
                    cout << buf << " семестр -----" << endl;
                    while (getline(ss, buf, ';')) {
                        string name = buf;
                        getline(ss, buf, ';');
                        cout << name << ": " << buf << endl;
                    }
                    cout << "-----" << endl << endl;
                }
                else {
                    counter = 0;
                    getline(ss, word, ';');
                    while (getline(ss, word, ';')) {
                        cout << stateOrder[counter]->val << ": " << word << endl;
                        counter++;
                    }
                    cout << "-----" << endl;
                }
            }
            databaseIn.close();
            Crypt();
        }

        List filter_students_by_date(bool getData = false) {
            char str[1000];
            Decrypt();
            databaseIn.open(databasePath);
            string start, end;
            cout << "С какой даты вы бы хотели вывести студентов? (формат ДД.ММ.ГГГГ): ";
            getline(cin >> ws, start);
            cout << "На какой дате вы бы хотели закончить вывод студентов? (формат ДД.ММ.ГГГГ): ";
            getline(cin >> ws, end);
            Date startDate(start), endDate(end);
            List dataHolder = List();
            bool flag = true;
            for (databaseIn.getline(str, 1000, 10); !(databaseIn.eof()); databaseIn.getline(str, 1000, 10)) {
                string dbLine(str);
                if (str[0] == '0') {
                    Student gotStudent(dbLine);
                    if (startDate <= gotStudent.dateOfBirth && gotStudent.dateOfBirth <= endDate) {
                        flag = true;
                        dataHolder.push_back(dbLine);
                    }
                    else {
                        flag = false;
                    }
                }
                else if (flag) {
                    dataHolder.push_back(dbLine);
                }
            }
            if (getData) {
                databaseIn.close();
                Crypt();
                return dataHolder;
            }
            else {
                string word;
                cout << endl;
                for (int i = 0; i <= dataHolder.length; i++) {
                    string str = dataHolder[i]->val;
                    stringstream ss(str);
                    string buf;
                    char firstSymbol = str[0];
                    if (firstSymbol != '0') {
                        getline(ss, buf, ';');
                        cout << buf << " семестр -----" << endl;
                        while (getline(ss, buf, ';')) {
                            string name = buf;
                            getline(ss, buf, ';');
                            cout << name << ": " << buf << endl;
                        }
                        cout << "-----" << endl << endl;
                    }
                    else {
                        int counter = 0;
                        getline(ss, word, ';');
                        while (getline(ss, word, ';')) {
                            cout << stateOrder[counter]->val << ": " << word << endl;
                            counter++;
                        }
                        cout << "-----" << endl;
                    }
                }
            }
            databaseIn.close();
            Crypt();
        }

        void my_task() {
            List filteredStudents = filter_students_by_date(true);
            List dataHolder = List();
            List semestersDataHolder = List();
            bool flag = true;
            string studentLine = "";
            for (int counter = 0; counter <= filteredStudents.length; counter++) {
                string str = filteredStudents[counter]->val;
                if (str[0] != '0' && flag) {
                    string pureData = str.substr(2);
                    if (pureData.find("3") == string::npos) {
                        semestersDataHolder.push_back(str);
                    }
                    else {
                        flag = false;
                        semestersDataHolder = List();
                    }
                }
                else if (str[0] == '0') {
                    if (semestersDataHolder.length != 0) {
                        dataHolder.push_back(studentLine);
                        for (int i = 0; i <= semestersDataHolder.length; i++) {
                            dataHolder.push_back(semestersDataHolder[i]->val);
                        }
                        semestersDataHolder = List();
                        studentLine = str;
                    }
                    else {
                        studentLine = str;
                    }
                }
            }
            for (int counter = 0; counter <= dataHolder.length; counter++) {
                string str = dataHolder[counter]->val;
                stringstream ss(str);
                string word;
                string buf;
                char firstSymbol = str[0];
                cout << endl;
                if (firstSymbol != '0') {
                    getline(ss, buf, ';');
                    cout << buf << " семестр -----" << endl;
                    while (getline(ss, buf, ';')) {
                        string name = buf;
                        getline(ss, buf, ';');
                        cout << name << ": " << buf << endl;
                    }
                    cout << "-----" << endl << endl;
                }
                else {
                    int i = 0;
                    getline(ss, word, ';');
                    while (getline(ss, word, ';')) {
                        cout << stateOrder[i]->val << ": " << word << endl;
                        i++;
                    }
                    cout << "-----" << endl;
                }
            }

        }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choiceNumber;
    DatabaseWorker* dbWorker = new DatabaseWorker();
    Crypt();
    List options = List({ "Добавить нового студента", "Изменить существующего студента", "Удалить студента", "Показать список студентов", "Вывести студентов определённого года рождения", "Показать выполнение задания" });
    cout << "Добро пожаловать в меню обработки базы данных университета!" << endl;
    do {
        cout << endl << "Пожалуйста, выберите пункт меню, с которым вы хотите работать." << endl;
        for (int counter = 0; counter <= options.length; counter++) {
            cout << counter + 1 << ": " << options[counter]->val << ";" << endl;
        }
        cout << "0: Выход" << endl;
        cout << "Ваша цифра: ";
        cin >> choiceNumber;
        switch (choiceNumber) {
            case 1: dbWorker->add_student(); break;
            case 2: dbWorker->change_student(); break;
            case 3: dbWorker->delete_student(); break;
            case 4: dbWorker->show_students(); break;
            case 5: dbWorker->filter_students_by_date(); break;
            case 6: dbWorker->my_task(); break;
            case 0: cout << "До скорых встреч!"; break;
        }
    } while (choiceNumber != 0);
    return 0;
}