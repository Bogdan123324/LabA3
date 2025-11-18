#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>

// Интерфейс налогоплательщика
class ITaxPayment {
public:
    virtual void payTax() = 0;
    virtual ~ITaxPayment() = default;
};

// Базовый класс Магазин
class Shop {
protected:
    std::string name_;
    std::string address_;
    int year_;
    int number_;
    double totalProfit_;
    std::vector<double> monthlyProfits_;

public:
    Shop() : name_("Unknown"), address_("Unknown"), year_(0), number_(0), totalProfit_(0) {}

    Shop(const std::string& name, const std::string& address, int year, int number, double profit)
        : name_(name), address_(address), year_(year), number_(number), totalProfit_(profit) {
        monthlyProfits_.push_back(profit);
    }

    virtual ~Shop() = default;

    void setName(const std::string& name) { name_ = name; }
    void setAddress(const std::string& address) { address_ = address; }
    void setYear(int year) { year_ = year; }
    void setNumber(int number) { number_ = number; }
    void setTotalProfit(double profit) { totalProfit_ = profit; }

    void addMonthlyProfit(double profit) {
        monthlyProfits_.push_back(profit);
        totalProfit_ += profit;
    }

    std::string getName() const { return name_; }
    std::string getAddress() const { return address_; }
    int getYear() const { return year_; }
    int getNumber() const { return number_; }
    double getTotalProfit() const { return totalProfit_; }

    double getAverageProfitGrowth() const {
        if (monthlyProfits_.size() < 2) return 0.0;
        double growth = 0.0;
        for (size_t i = 1; i < monthlyProfits_.size(); ++i) {
            growth += monthlyProfits_[i] - monthlyProfits_[i - 1];
        }
        return growth / (monthlyProfits_.size() - 1);
    }

    virtual void printInfo() const {
        std::cout << "Shop: " << name_ << "\n";
        std::cout << "Address: " << address_ << "\n";
        std::cout << "Year: " << year_ << ", Number: " << number_ << "\n";
        std::cout << "Total Profit: " << totalProfit_ << "\n";
        std::cout << "Average Profit Growth: " << getAverageProfitGrowth() << "\n\n";
    }

    void serialize(const std::string& filename) const {
        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << name_ << "\n" << address_ << "\n" << year_ << "\n" << number_ << "\n";
            outFile << monthlyProfits_.size() << "\n";
            for (double profit : monthlyProfits_) outFile << profit << "\n";
            outFile.close();
        }
        else {
            std::cerr << "Error: Could not open file " << filename << "\n";
        }
    }

    void deserialize(const std::string& filename) {
        std::ifstream inFile(filename);
        if (inFile.is_open()) {
            std::getline(inFile, name_);
            std::getline(inFile, address_);
            inFile >> year_ >> number_;
            monthlyProfits_.clear();
            size_t profitCount;
            inFile >> profitCount;
            inFile.ignore();
            totalProfit_ = 0;
            for (size_t i = 0; i < profitCount; ++i) {
                double profit;
                inFile >> profit;
                monthlyProfits_.push_back(profit);
                totalProfit_ += profit;
            }
            inFile.close();
        }
        else {
            std::cerr << "Error: Could not open file " << filename << "\n";
        }
    }
};

// Класс Аптека
class Pharmacy : public Shop, public ITaxPayment {
private:
    std::string workingHours_;
    int discountCustomers_;
    bool is24_7_;

public:
    Pharmacy() : workingHours_("9:00-21:00"), discountCustomers_(0), is24_7_(false) {}

    Pharmacy(const std::string& name, const std::string& address, int year, int number,
        double profit, const std::string& hours, int discountCustomers, bool is24_7)
        : Shop(name, address, year, number, profit), workingHours_(hours),
        discountCustomers_(discountCustomers), is24_7_(is24_7) {
    }

    void setWorkingHours(const std::string& hours) { workingHours_ = hours; }
    void setDiscountCustomers(int count) { discountCustomers_ = count; }
    void setIs24_7(bool is24_7) { is24_7_ = is24_7; }

    std::string getWorkingHours() const { return workingHours_; }
    int getDiscountCustomers() const { return discountCustomers_; }
    bool getIs24_7() const { return is24_7_; }

    void payTax() override {
        double tax = totalProfit_ * 0.06; // 6% налог для аптеки
        totalProfit_ -= tax;
        std::cout << "Pharmacy '" << name_ << "' paid tax: " << tax << "\n";
    }

    void printInfo() const override {
        std::cout << "=== Pharmacy ===\n";
        std::cout << "Name: " << name_ << "\n";
        std::cout << "Address: " << address_ << "\n";
        std::cout << "Year: " << year_ << ", Number: " << number_ << "\n";
        std::cout << "Working Hours: " << workingHours_ << "\n";
        std::cout << "24/7: " << (is24_7_ ? "Yes" : "No") << "\n";
        std::cout << "Discount Customers: " << discountCustomers_ << "\n";
        std::cout << "Total Profit: " << totalProfit_ << "\n";
        std::cout << "Average Profit Growth: " << getAverageProfitGrowth() << "\n\n";
    }
};

// Класс Бутик
class Boutique : public Shop, public ITaxPayment {
private:
    std::string workingHours_;
    int discountCustomers_;
    bool is24_7_;
    double landTax_;

public:
    Boutique() : workingHours_("10:00-22:00"), discountCustomers_(0), is24_7_(false), landTax_(500) {}

    Boutique(const std::string& name, const std::string& address, int year, int number,
        double profit, const std::string& hours, int discountCustomers, bool is24_7, double landTax)
        : Shop(name, address, year, number, profit), workingHours_(hours),
        discountCustomers_(discountCustomers), is24_7_(is24_7), landTax_(landTax) {
    }

    void setWorkingHours(const std::string& hours) { workingHours_ = hours; }
    void setDiscountCustomers(int count) { discountCustomers_ = count; }
    void setIs24_7(bool is24_7) { is24_7_ = is24_7; }
    void setLandTax(double tax) { landTax_ = tax; }

    std::string getWorkingHours() const { return workingHours_; }
    int getDiscountCustomers() const { return discountCustomers_; }
    bool getIs24_7() const { return is24_7_; }
    double getLandTax() const { return landTax_; }

    void payTax() override {
        double tax = totalProfit_ * 0.15 + landTax_; // 15% налог + земельный налог
        totalProfit_ -= tax;
        std::cout << "Boutique '" << name_ << "' paid tax: " << tax << " (including land tax: " << landTax_ << ")\n";
    }

    void printInfo() const override {
        std::cout << "=== Boutique ===\n";
        std::cout << "Name: " << name_ << "\n";
        std::cout << "Address: " << address_ << "\n";
        std::cout << "Year: " << year_ << ", Number: " << number_ << "\n";
        std::cout << "Working Hours: " << workingHours_ << "\n";
        std::cout << "24/7: " << (is24_7_ ? "Yes" : "No") << "\n";
        std::cout << "Discount Customers: " << discountCustomers_ << "\n";
        std::cout << "Land Tax: " << landTax_ << "\n";
        std::cout << "Total Profit: " << totalProfit_ << "\n";
        std::cout << "Average Profit Growth: " << getAverageProfitGrowth() << "\n\n";
    }
};

int main() {
    std::vector<std::unique_ptr<Shop>> shops;
    std::vector<ITaxPayment*> taxPayers;

    // Создаем 2 аптеки и 1 бутик
    auto pharmacy1 = std::make_unique<Pharmacy>("HealthPlus", "123 Medical St", 2010, 1, 50000,
        "8:00-20:00", 150, false);
    auto pharmacy2 = std::make_unique<Pharmacy>("QuickMed", "456 Pharmacy Ave", 2015, 2, 75000,
        "9:00-21:00", 200, true);
    auto boutique = std::make_unique<Boutique>("FashionElite", "789 Luxury Blvd", 2018, 3, 120000,
        "10:00-22:00", 50, false, 1000);

    // Добавляем месячную прибыль
    pharmacy1->addMonthlyProfit(52000);
    pharmacy1->addMonthlyProfit(48000);
    pharmacy2->addMonthlyProfit(78000);
    pharmacy2->addMonthlyProfit(72000);
    boutique->addMonthlyProfit(125000);
    boutique->addMonthlyProfit(115000);

    // Добавляем в общие контейнеры
    shops.push_back(std::move(pharmacy1));
    shops.push_back(std::move(pharmacy2));
    shops.push_back(std::move(boutique));

    // Демонстрация полиморфизма - вывод информации
    std::cout << "=== Shop Information ===\n\n";
    for (const auto& shop : shops) {
        shop->printInfo();
    }

    // Демонстрация RTTI и интерфейса налогоплательщика
    std::cout << "=== Tax Payment Demonstration ===\n\n";
    for (const auto& shop : shops) {
        // Используем RTTI для приведения к ITaxPayment
        ITaxPayment* taxPayer = dynamic_cast<ITaxPayment*>(shop.get());
        if (taxPayer) {
            std::cout << "Before tax payment - ";
            shop->printInfo();
            taxPayer->payTax();
            std::cout << "After tax payment - ";
            shop->printInfo();
            std::cout << "------------------------\n";
        }
    }

    // Демонстрация разных типов через RTTI
    std::cout << "=== RTTI Type Identification ===\n\n";
    for (const auto& shop : shops) {
        if (dynamic_cast<Pharmacy*>(shop.get())) {
            std::cout << shop->getName() << " is a Pharmacy\n";
        }
        else if (dynamic_cast<Boutique*>(shop.get())) {
            std::cout << shop->getName() << " is a Boutique\n";
        }
        else {
            std::cout << shop->getName() << " is a generic Shop\n";
        }
    }

    // Рейтинг по общей прибыли
    std::cout << "\n=== Profit Ranking ===\n";
    std::sort(shops.begin(), shops.end(), [](const std::unique_ptr<Shop>& a, const std::unique_ptr<Shop>& b) {
        return a->getTotalProfit() > b->getTotalProfit();
        });

    for (size_t i = 0; i < shops.size(); ++i) {
        std::cout << i + 1 << ". " << shops[i]->getName() << " - Profit: " << shops[i]->getTotalProfit() << "\n";
    }

    // Сериализация данных
    std::cout << "\n=== Serialization ===\n";
    for (size_t i = 0; i < shops.size(); ++i) {
        std::string filename = "shop_" + std::to_string(i + 1) + ".txt";
        shops[i]->serialize(filename);
        std::cout << "Data saved to " << filename << "\n";
    }

    return 0;
}