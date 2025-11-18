#ifndef KEEPER_ORDER_H
#define KEEPER_ORDER_H

#include "order.h"
#include "exceptions.h"
#include <string>

class KeeperOrder {
    ORDER** items;
    int size;
    int capacity;

    void resize(int newCap);

public:
    KeeperOrder();
    KeeperOrder(const KeeperOrder& other);
    KeeperOrder& operator=(const KeeperOrder& other);
    ~KeeperOrder();

    // Вставка в алфавитном порядке по расчетному счету плательщика
    void add(ORDER* o);               // владеет o
    void insertAt(ORDER* o, int index);
    void removeAt(int index);
    int getSize() const { return size; }
    ORDER* getAt(int index) const;

    void clear();
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    // сумма по заданному плательщику
    bool sumByPayer(const std::string& payer, double& sumOut) const;
};

#endif // KEEPER_ORDER_H
