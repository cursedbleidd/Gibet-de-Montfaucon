#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <time.h>

using namespace std;

string OutPut;
string usedLetters = "";

const int RED = 4;
const int GREEN = 2;
const int BLUE = 1;
const int WHITE = 15;

void colorChange(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void deleteSymbols(int count)
{
    for (int i = 0; i < count; i++)
    {
        cout << "\b \b";
    }
}

void blinkingRes(bool result, string word) //вывод результата игры
{
    string msg;
    msg = result ? "ВЫ ПОБЕДИЛИ!" : "Вы проиграли!";
    int color[] = { WHITE, RED, GREEN, BLUE };
    for (int i = 0; i < 4; i++)
    {
        colorChange(color[i]); Sleep(10);
        cout << msg; Sleep(1000);
        deleteSymbols(size(msg));
    }
    colorChange(WHITE);
    if (!result) cout << word << endl << " " << msg;
    else cout << msg;
}

void loadGame()
{
    cout << " Виселица"; Sleep(1000);
    for (int i = 0; i < 3; i++)
    {
        cout << "."; Sleep(250);
        cout << "."; Sleep(250);
        cout << "."; Sleep(250);
        deleteSymbols(3); Sleep(150);
    }
    deleteSymbols(9);
}

string chooseWord() //выбор случайного слова
{
    string word; int n, cnt = 0;
    ifstream dict("dict.txt");
    srand(time(0));
    dict >> n;
    string *words = new string[n];
    n = rand() % n;
    while (dict >> *(words + cnt)) cnt++;
    dict.close();
    return *(words + n);
}

bool checkInput(string str, char userInput) //проверка слова на наличие введенной буквы
{
    int i = 0;
    bool chk = true;
    while (chk && size(str) > i)
    {
        chk = (str[i] != userInput);
        i++;
    }
    return chk;
}

void paintField(string key, ifstream& spray, bool status) //отрисовка поля
{
    string sprite;
    system("cls");
    if (status)
    {
        OutPut = "";
        for (int i = 1; i <= 7; i++)
        {
            getline(spray, sprite);
            OutPut += " " + sprite + "\n";
        }
    }
    cout << OutPut + "\n " + "Использованные буквы:" + usedLetters + "\n " + key + "\n ";
}

void Game(string word, ifstream &spray)
{
    int letters = size(word), mistakes = 0, cnt;
    bool status = true;
    string key(letters, '_');
    char userInput;
    while (mistakes != 5 && letters != 0) //проверка победы или поражения после каждого ввода
    {
        paintField(key, spray, status);
        cin >> userInput;
        cnt = 0;
        while (!checkInput(key, userInput) || !checkInput(usedLetters, userInput) || (int)userInput < -64 || (int)userInput > -33) //защита от неправильного ввода
        {
            paintField(key, spray, false);
            cout << "Буква уже была названа или использован неверный формат символа.(Используйте заглавные буквы Кириллицы)\n Введите букву:\n "; cin >> userInput;
        }
        for (int i = 0; i < size(word); i++)
        {
            if (word[i] == userInput)
            {
                key[i] = word[i];
                cnt++;
            }
        }
        Sleep(1000);
        status = cnt == 0;
        if (status)
        {
            usedLetters += " " + string(1, userInput);
            mistakes++;
        }
        else letters -= cnt;
    }
    paintField(key, spray, status);
    blinkingRes(letters == 0 && mistakes != 5, word);
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("title Gibet de Montfaucon");
    ifstream sprays("spray.txt");
    string word = chooseWord();
    setlocale(LC_ALL, "Russian");
    loadGame();
    Game(word, sprays);
    sprays.close();
    cout << endl << " ";
    system("pause");
}