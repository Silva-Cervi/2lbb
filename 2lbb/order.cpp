#include "order.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <stdexcept>

ORDER::ORDER() : payerAccount(""), receiverAccount(""), amount(0.0) {
    std::cout << "ORDER: конструктор по умолчанию\n";
}
ORDER::ORDER(const std::string& payer, const std::string& receiver, double amt)
    : payerAccount(payer), receiverAccount(receiver), amount(amt) {
    std::cout << "ORDER: параметрический конструктор\n";
}
ORDER::ORDER(const ORDER& other)
    : payerAccount(other.payerAccount), receiverAccount(other.receiverAccount), amount(other.amount) {
    std::cout << "ORDER: конструктор копирования\n";
}
ORDER::~ORDER() {
    std::cout << "ORDER: деструктор\n";
}

void ORDER::setPayer(const std::string& s) { payerAccount = s; }
std::string ORDER::getPayer() const { return payerAccount; }
void ORDER::setReceiver(const std::string& s) { receiverAccount = s; }
std::string ORDER::getReceiver() const { return receiverAccount; }
void ORDER::setAmount(double a) { amount = a; }
double ORDER::getAmount() const { return amount; }

static std::string trim_copy(const std::string& s) {
    size_t l = 0;
    while (l < s.size() && std::isspace(static_cast<unsigned char>(s[l]))) ++l;
    size_t r = s.size();
    while (r > l && std::isspace(static_cast<unsigned char>(s[r - 1]))) --r;
    return s.substr(l, r - l);
}

std::istream& operator>>(std::istream& in, ORDER& o) {
    std::cout << "Введите расчетный счет плательщика: ";
    std::getline(in >> std::ws, o.payerAccount);
    o.payerAccount = trim_copy(o.payerAccount);

    std::cout << "Введите расчетный счет получателя: ";
    std::getline(in, o.receiverAccount);
    o.receiverAccount = trim_copy(o.receiverAccount);

    // читаем сумму как строку, парсим через stod, проверяем >= 0 и отсутствие лишних символов
    while (true) {
        std::cout << "Введите перечисляемую сумму (в рублях, >= 0): ";
        std::string line;
        if (!std::getline(in, line)) {
            // EOF или ошибка — оставляем предыдущую сумму и выходим
            break;
        }
        std::string s = trim_copy(line);
        if (s.empty()) {
            std::cout << "Ошибка: пустая строка. Попробуйте ещё раз.\n";
            continue;
        }
        try {
            size_t idx = 0;
            double val = std::stod(s, &idx);
            // проверяем нет ли лишних символов после числа (за исключением пробелов)
            size_t j = idx;
            while (j < s.size() && std::isspace(static_cast<unsigned char>(s[j]))) ++j;
            if (j != s.size()) {
                std::cout << "Ошибка: недопустимые символы в вводе. Введите только число.\n";
                continue;
            }
            if (val < 0.0) {
                std::cout << "Ошибка: сумма не может быть отрицательной. Попробуйте ещё раз.\n";
                continue;
            }
            o.amount = val;
            break;
        }
        catch (const std::invalid_argument&) {
            std::cout << "Ошибка: неверный формат числа. Попробуйте ещё раз.\n";
        }
        catch (const std::out_of_range&) {
            std::cout << "Ошибка: число вне допустимого диапазона. Попробуйте ещё раз.\n";
        }
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const ORDER& o) {
    // Форматируем сумму с двумя знаками после запятой
    out << "Плательщик: " << o.payerAccount << "; Получатель: " << o.receiverAccount
        << "; Сумма: " << std::fixed << std::setprecision(2) << o.amount;
    // аккуратно сбрасываем форматирование дробного вывода (чтобы внешние вызовы не удивились)
    out.unsetf(std::ios_base::floatfield);
    return out;
}
