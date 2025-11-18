#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include "keeper_order.h"
#include "exceptions.h"

// Разбиение текста на предложения (оканчивается . ! ?). Возвращаем с терминатором.
static std::vector<std::string> splitIntoSentences(const std::string& text) {
    std::vector<std::string> out;
    std::string cur;
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        cur.push_back(c);
        if (c == '.' || c == '!' || c == '?') {
            size_t j = i + 1;
            while (j < text.size() && (text[j] == '"' || text[j] == '\'' || text[j] == ')' ||
                text[j] == ' ' || text[j] == '\n' || text[j] == '\r' || text[j] == '\t')) {
                cur.push_back(text[j]);
                ++j;
            }
            out.push_back(cur);
            cur.clear();
            i = j - 1;
        }
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}

// подсчёт знаков пунктуации в строке
static int countPunct(const std::string& s) {
    int cnt = 0;
    for (unsigned char ch : s) if (std::ispunct(ch)) ++cnt;
    return cnt;
}

void part1_interactive() {
    KeeperOrder ko;
    int choice = -1;
    while (choice != 0) {
        try {
            std::cout << "\n--- Меню (ORDER) ---\n";
            std::cout << "1. Добавить ORDER\n";
            std::cout << "2. Показать все\n";
            std::cout << "3. Удалить по индексу\n";
            std::cout << "4. Сохранить в файл\n";
            std::cout << "5. Загрузить из файла\n";
            std::cout << "6. Показать сумму снятую с введённого расчётного счёта плательщика\n";
            std::cout << "0. Выйти в главное меню\n";
            std::cout << "Выберите пункт: ";
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Неверный ввод\n"; continue;
            }
            std::cin.ignore(10000, '\n');
            if (choice == 1) {
                ORDER* o = new ORDER();
                std::cin >> *o;
                ko.add(o);
                std::cout << "ORDER добавлен.\n";
            }
            else if (choice == 2) {
                int n = ko.getSize();
                std::cout << "Всего записей: " << n << '\n';
                for (int i = 0; i < n; ++i) std::cout << i << ": " << *ko.getAt(i) << '\n';
            }
            else if (choice == 3) {
                int idx; std::cout << "Индекс для удаления: "; std::cin >> idx; std::cin.ignore(10000, '\n');
                ko.removeAt(idx);
                std::cout << "Удалено.\n";
            }
            else if (choice == 4) {
                std::string fname; std::cout << "Имя файла для сохранения: "; std::getline(std::cin, fname);
                ko.saveToFile(fname);
            }
            else if (choice == 5) {
                std::string fname; std::cout << "Имя файла для загрузки: "; std::getline(std::cin, fname);
                ko.loadFromFile(fname);
            }
            else if (choice == 6) {
                std::string payer; std::cout << "Введите расчетный счет плательщика: "; std::getline(std::cin, payer);
                double sum = 0.0;
                if (ko.sumByPayer(payer, sum)) {
                    std::cout << "Сумма списаний с " << payer << " = " << sum << " руб.\n";
                }
                else {
                    std::cout << "Расчетного счета " << payer << " не найдено.\n";
                }
            }
            else if (choice == 0) break;
            else std::cout << "Неизвестный пункт\n";
        }
        catch (const LabException& ex) {
            std::cout << "Ошибка: " << ex.what() << '\n';
        }
        catch (const std::exception& ex) {
            std::cout << "std::exception: " << ex.what() << '\n';
        }
    }
}

void part2_sentences_max_punct() {
    std::cout << "Введите имя входного файла: ";
    std::string fname; std::getline(std::cin, fname);
    std::ifstream in(fname.c_str());
    if (!in) { std::cout << "Не удалось открыть файл " << fname << '\n'; return; }
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    auto sentences = splitIntoSentences(content);
    if (sentences.empty()) { std::cout << "В файле нет предложений.\n"; return; }
    int maxP = -1;
    for (auto& s : sentences) {
        int c = countPunct(s);
        if (c > maxP) maxP = c;
    }
    std::cout << "\nПредложения с максимальным количеством знаков пунктуации (" << maxP << "):\n\n";
    for (auto& s : sentences) {
        if (countPunct(s) == maxP) std::cout << s << "\n";
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "=== Лабораторная №2 — Вариант 20: ORDER ===\n";
    std::cout << "Версия программы: 1.0\n";
    int top = -1;
    while (top != 0) {
        std::cout << "\n=== Главное меню ===\n";
        std::cout << "1. Часть 1 (ORDER и стандартные потоки)\n";
        std::cout << "2. Часть 2 (файловые/строковые потоки — предложения с макс. пунктуацией)\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите пункт: ";
        if (!(std::cin >> top)) { std::cin.clear(); std::cin.ignore(10000, '\n'); std::cout << "Неверный ввод\n"; continue; }
        std::cin.ignore(10000, '\n');
        if (top == 1) part1_interactive();
        else if (top == 2) part2_sentences_max_punct();
        else if (top == 0) break;
        else std::cout << "Неизвестный пункт\n";
    }
    return 0;
}
