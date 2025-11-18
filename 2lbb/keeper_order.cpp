#include "keeper_order.h"
#include <iostream>
#include <fstream>

void KeeperOrder::resize(int newCap) {
    if (newCap <= capacity) return;
    ORDER** newItems = new ORDER * [newCap];
    for (int i = 0; i < size; ++i) newItems[i] = items[i];
    for (int i = size; i < newCap; ++i) newItems[i] = nullptr;
    delete[] items;
    items = newItems;
    capacity = newCap;
}

KeeperOrder::KeeperOrder() : items(nullptr), size(0), capacity(0) {
    resize(4);
    std::cout << "KeeperOrder: конструктор по умолчанию\n";
}

KeeperOrder::KeeperOrder(const KeeperOrder& other) : items(nullptr), size(0), capacity(0) {
    resize(other.capacity);
    for (int i = 0; i < other.size; ++i) items[i] = new ORDER(*other.items[i]);
    size = other.size;
    std::cout << "KeeperOrder: конструктор копирования\n";
}

KeeperOrder& KeeperOrder::operator=(const KeeperOrder& other) {
    if (this == &other) return *this;
    clear();
    resize(other.capacity);
    for (int i = 0; i < other.size; ++i) items[i] = new ORDER(*other.items[i]);
    size = other.size;
    std::cout << "KeeperOrder: оператор присваивания\n";
    return *this;
}

KeeperOrder::~KeeperOrder() {
    clear();
    delete[] items;
    std::cout << "KeeperOrder: деструктор\n";
}

void KeeperOrder::insertAt(ORDER* o, int index) {
    if (index < 0 || index > size) throw LabException("Индекс вставки вне диапазона");
    if (size >= capacity) resize(capacity * 2);
    for (int i = size; i > index; --i) items[i] = items[i - 1];
    items[index] = o;
    ++size;
}

void KeeperOrder::add(ORDER* o) {
    if (!o) throw LabException("Попытка добавить нулевой ORDER");
    int pos = 0;
    while (pos < size && items[pos]->getPayer().compare(o->getPayer()) < 0) ++pos;
    insertAt(o, pos);
}

void KeeperOrder::removeAt(int index) {
    if (index < 0 || index >= size) throw LabException("Индекс удаления вне диапазона");
    delete items[index];
    for (int i = index; i < size - 1; ++i) items[i] = items[i + 1];
    items[--size] = nullptr;
}

ORDER* KeeperOrder::getAt(int index) const {
    if (index < 0 || index >= size) throw LabException("Индекс getAt вне диапазона");
    return items[index];
}

void KeeperOrder::clear() {
    for (int i = 0; i < size; ++i) { delete items[i]; items[i] = nullptr; }
    size = 0;
}

void KeeperOrder::saveToFile(const std::string& filename) const {
    std::ofstream out(filename.c_str());
    if (!out) throw LabException("Не удалось открыть файл для записи: " + filename);
    out << size << '\n';
    for (int i = 0; i < size; ++i) {
        out << items[i]->getPayer() << '\n';
        out << items[i]->getReceiver() << '\n';
        out << items[i]->getAmount() << '\n';
    }
    out.close();
    std::cout << "Данные сохранены в файл: " << filename << '\n';
}

void KeeperOrder::loadFromFile(const std::string& filename) {
    std::ifstream in(filename.c_str());
    if (!in) throw LabException("Не удалось открыть файл для чтения: " + filename);
    clear();
    int n = 0; in >> n;
    std::string dummy; std::getline(in, dummy);
    resize(n > 4 ? n : 4);
    for (int i = 0; i < n; ++i) {
        std::string payer, receiver;
        double amt;
        std::getline(in, payer);
        std::getline(in, receiver);
        in >> amt; std::getline(in, dummy);
        ORDER* o = new ORDER(payer, receiver, amt);
        add(o);
    }
    in.close();
    std::cout << "Данные загружены из файла: " << filename << '\n';
}

bool KeeperOrder::sumByPayer(const std::string& payer, double& sumOut) const {
    bool found = false;
    double sum = 0.0;
    for (int i = 0; i < size; ++i) {
        if (items[i]->getPayer() == payer) {
            found = true;
            sum += items[i]->getAmount();
        }
    }
    sumOut = sum;
    return found;
}
