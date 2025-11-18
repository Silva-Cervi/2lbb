#include "order.h"
#include <iostream>

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

std::istream& operator>>(std::istream& in, ORDER& o) {
    std::cout << "Введите расчетный счет плательщика: ";
    std::getline(in >> std::ws, o.payerAccount);
    std::cout << "Введите расчетный счет получателя: ";
    std::getline(in, o.receiverAccount);
    std::cout << "Введите перечисляемую сумму (в рублях): ";
    in >> o.amount;
    std::string dummy; std::getline(in, dummy);
    return in;
}

std::ostream& operator<<(std::ostream& out, const ORDER& o) {
    out << "Плательщик: " << o.payerAccount << "; Получатель: " << o.receiverAccount << "; Сумма: " << o.amount;
    return out;
}
