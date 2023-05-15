#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <string>
#include <vector>
#include <stdio.h>
using namespace std;

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
                length++;
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
            int counter = 0;
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
        List stateOrder = List({ "Full Name", "Date of Birth", "Sex", "Student Card", "Institute", "Department", "Course" });
        List addingOrder = List({ "Введите ФИО студента: ", "День рождения в формате ДД.ММ.ГГГГ: ", "Пол(М - мужской, Ж - женский): ", "Шифр студента (формат: 01Б2345): ", "Институт студента: ", "Шифр кафедры студента: ", "Курс в формате (Б - бакалавриат, М - магистратура, А - аспирантура)(цифра курса): " });
    public:
        void add_student() {
            char data[100];
            databaseOut.open(databasePath);
            for (int i = 0; i < 7; i++) {
                cout << addingOrder[i]->val;
                cin.getline(data, 100);
                databaseOut << data << ';';
            }
            databaseOut << endl;
            databaseOut.close();
            cout << "Студент успешно добавлен!";
        }

        void change_student() {
            bool flag = true;
            char studentCard[8];
            cout << "Пожалуйста, введите шифр студента, подлежащий изменению (формат 01А2345): ";
            cin >> studentCard;
            databaseIn.open(".\\database.txt");
            List dataHolder = List();
            char str[100];
            for (databaseIn.getline(str, 100, 10); !(databaseIn.eof()); databaseIn.getline(str, 100, 10)) {
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
                    int number;
                    cout << "Ваша цифра: ";
                    cin >> number;
                    string newData;
                    cout << addingOrder[number - 1]->val;
                    getline(cin >> ws, newData);
                    changedStudent.all_params[number - 1]->val = newData;
                    dataHolder.push_back(changedStudent.return_string_data());
                    flag = false;
                }
            }
            databaseIn.close();
            databaseOut.open(".\\database.txt");
            for (int i = 0; i < dataHolder.length + 1; i++) {
                databaseOut << dataHolder[i]->val << endl;
            }
            databaseOut.close();
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
            databaseIn.open(".\\database.txt");
            char str[100];
            bool flag = true;
            List dataHolder = List();
            for (databaseIn.getline(str, 100, 10); !(databaseIn.eof()); databaseIn.getline(str, 100, 10)) {
                if (!(strstr(str, studentCard))) dataHolder.push_back(str);
                else flag = false;
            }
            databaseIn.close();
            databaseOut.open(".\\database.txt");
            for (int i = 0; i < dataHolder.length + 1; i++) {
                databaseOut << dataHolder[i]->val << endl;
            }
            databaseOut.close();
            if (flag) {
                cout << "Такой студент не был найден." << endl;
            }
            else {
                cout << "Студент успешно удалён!" << endl;
            }
        }

        void show_students() {
            string s;
            char str[100];
            string word;
            char* ptr = NULL;
            char* next_ptr = NULL;
            databaseIn.open(".\\database.txt");
            int counter;
            for (databaseIn.getline(str, 100, 10); !(databaseIn.eof()); databaseIn.getline(str, 100, 10)) {
                stringstream ss(str);
                counter = 0;
                while (getline(ss, word, ';')) {
                    cout << stateOrder[counter]->val << ": " << word << endl;
                    counter++;
                }
                cout << "-----" << endl;
            }
            databaseIn.close();
        }
};

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    DatabaseWorker* dbWorker = new DatabaseWorker();
    dbWorker->show_students();
    dbWorker->change_student();
    dbWorker->show_students();
    return 0;
}