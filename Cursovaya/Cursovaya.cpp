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
            cout << "Студент успешно добавлен!";
        }

        void change_student() {
            bool flag = true;
            char studentCard[8];
            cout << "Пожалуйста, введите шифр студента, подлежащий изменению (формат 01А2345): ";
            cin >> studentCard;
            databaseIn.open(".\\database.txt");
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
                            return;
                        }
                        else {
                            string semesterName;
                            cout << "Какой предмет вы хотели бы изменить/добавить? ";
                            cin >> semesterName;
                            string strCopy(str);
                            int index = strCopy.find(semesterName);
                            int newMark;
                            if (index != -1) {
                                cout << "Новая оценка: ";
                                cin >> newMark;
                                strCopy[index + semesterName.length() + 1] = to_string(newMark)[0];
                                dataHolder.push_back(strCopy);
                            }
                            else {
                                cout << "Новая оценка: ";
                                cin >> newMark;
                                dataHolder.push_back(strCopy + ";" + semesterName + ";" + to_string(newMark));
                            }
                        }
                    }
                    else if (number == 8) {
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
                    }
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
                cout << "Студент успешно удалён!" << endl;
            }
        }

        void show_students() {
            string s;
            char str[1000];
            string word;
            char* ptr = NULL;
            char* next_ptr = NULL;
            databaseIn.open(".\\database.txt");
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
        }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choiceNumber;
    DatabaseWorker* dbWorker = new DatabaseWorker();
    List options = List({ "Добавить нового студента", "Изменить существующего студента", "Удалить студента", "Показать список студентов"});
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
            case 0: cout << "До скорых встреч!"; break;
        }
    } while (choiceNumber != 0);
    return 0;
}