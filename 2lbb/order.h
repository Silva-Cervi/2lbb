#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <iostream>

class ORDER {
    std::string payerAccount;    // расчетный счет плательщика
    std::string receiverAccount; // расчетный счет получателя
    double amount;               // сумма в рублях
public:
    ORDER();
    ORDER(const std::string& payer, const std::string& receiver, double amt);
    ORDER(const ORDER& other);
    ~ORDER();

    void setPayer(const std::string& s); std::string getPayer() const;
    void setReceiver(const std::string& s); std::string getReceiver() const;
    void setAmount(double a); double getAmount() const;

    friend std::istream& operator>>(std::istream& in, ORDER& o);
    friend std::ostream& operator<<(std::ostream& out, const ORDER& o);
};

#endif // ORDER_H
