#include <iostream>
#include<fstream>
#include <thread>
#include<string>
#include<regex>
#include<map>


using namespace std;


static regex reg1("([A-Z]{3})([0-9]{1,4})");

static regex reg2("([A-Z]{2})(([\\s]{1})([0-9]{1,4}))");

static regex reg3("([A-Z]{2})([0-9]{1,5})");

static regex reg4("([A-Z][A-Z0-9]|[0-9][A-Z])(([\\s]{1})([0-9]{1,4}))");

static regex reg5("([A-Z][A-Z0-9]|[0-9][A-Z])([0-9]{1,5})");

static regex reg6("([0-9]{1,5})");




/* Функция приведения строки к нормальному/стандартному виду
    - возвращает строку вида: AFL1234, AF12345, 12345,
    без пробелов и ведущих нулей */
string getFullStringForEquality(const string& str)
{
    cmatch result;
    string strNumOfFlight;
    string strAviaCode;


    // Получение номера рейса и кода авиакомпании
    if (regex_match(str.c_str(), result, reg1) ||
        regex_match(str.c_str(), result, reg2) ||
        regex_match(str.c_str(), result, reg3) ||
        regex_match(str.c_str(), result, reg4) ||
        regex_match(str.c_str(), result, reg5))
    {
        strAviaCode = result[1].str();
        strNumOfFlight = result[2].str();
    }
    else if (regex_match(str.c_str(), result, reg6))
    {
        strAviaCode = "";
        strNumOfFlight = result[1].str();
    }

    // Очистка от пробелов и ведущих нулей
    strNumOfFlight.erase(remove(strNumOfFlight.begin(), strNumOfFlight.end(), ' '), strNumOfFlight.end());

    size_t pos = strNumOfFlight.find_first_not_of('0');
    if (pos != string::npos)
    {
        strNumOfFlight = strNumOfFlight.substr(pos);
    }

    return strAviaCode + strNumOfFlight;
}


void readStrFromFile(const string& fileNameIn, const string& fileNameOut)
/*Считываем данные из файла, приводим к "нормальному виду",
если есть нормальный ключ, то затираем строчку, иначе добавляем,
добавляем в файл непустые строчки,
на вход - название файлов
 входных и выходных данных*/
{
    //"/home/user/task_2/1_in.txt"

    ifstream fin(fileNameIn);
    ofstream fout(fileNameOut);

    map<string, string>dataFromFile;

    string str;
    string normalStr;

    if (fin.is_open() && fout.is_open())
    {
        cout << "File is open\n";
        while (!fin.eof())
        {
            str = "";
            getline(fin, str);
            normalStr = getFullStringForEquality(str);

            if (dataFromFile.find(normalStr) != dataFromFile.end())
            {
                dataFromFile[normalStr] = "";
            }
            else
            {
                dataFromFile[normalStr] = str;
            }
        }

        for (const auto& [normalStr, str] : dataFromFile)
        {
            if (str.empty() == false)
            {
                fout << str << '\n';
            }
        }

        cout << "Data in file\n";
    }
    else
    {
        cout << "File is not open\n";
    }
    fin.close();
    fout.close();

}

int main()
{
    //чтение из файла

    string fileNameInFirst = "/home/user/task_2/1_in.txt";
    string fileNameOutFirst = "/home/user/task_2/1_out.txt";

    string fileNameInSecond = "/home/user/task_2/2_in.txt";
    string fileNameOutSecond = "/home/user/task_2/2_out.txt";


    thread firstFile(readStrFromFile, fileNameInFirst, fileNameOutFirst);
    thread secondFile(readStrFromFile, fileNameInSecond, fileNameOutSecond);
    firstFile.join();
    secondFile.join();

    return 0;
}

