#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>

class ILoggable {
public:
    virtual void logToScreen() const = 0;
    virtual void logToFile(const std::string& filename) const = 0;
    virtual ~ILoggable() = default;
};

class IShuffle {
public:
    virtual void shuffle() = 0;
    virtual void shuffle(size_t i, size_t j) = 0;
    virtual ~IShuffle() = default;
};

class ExpressionEvaluator : public ILoggable {
protected:
    double* operands;
    size_t count;

public:
    ExpressionEvaluator() : count(20) {
        operands = new double[count]();
    }

    ExpressionEvaluator(size_t n) : count(n) {
        operands = new double[count]();
    }

    virtual ~ExpressionEvaluator() {
        delete[] operands;
    }

    void setOperand(size_t pos, double value) {
        if (pos < count) {
            operands[pos] = value;
        }
    }

    void setOperands(double ops[], size_t n) {
        size_t minCount = (n < count) ? n : count;
        for (size_t i = 0; i < minCount; ++i) {
            operands[i] = ops[i];
        }
    }

    virtual double calculate() const = 0;
    virtual std::string getExpression() const = 0;
    virtual std::string getName() const = 0;

    void logToScreen() const override {
        std::cout << "[" << count << "]" << std::endl;
        std::cout << getExpression() << std::endl;
        std::cout << calculate() << std::endl;
    }

    void logToFile(const std::string& filename) const override {
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << "[" << count << "]" << std::endl;
            file << getExpression() << std::endl;
            file << calculate() << std::endl;
            file.close();
        }
    }

    size_t getOperandCount() const { return count; }
    double getOperand(size_t index) const {
        return (index < count) ? operands[index] : 0.0;
    }
};

std::string formatNumber(double value) {
    std::stringstream ss;
    if (value == static_cast<int>(value)) {
        ss << static_cast<int>(value);
    }
    else {
        ss << std::fixed << std::setprecision(1) << value;
    }
    return ss.str();
}

class Summator : public ExpressionEvaluator {
public:
    Summator(size_t n = 20) : ExpressionEvaluator(n) {}

    double calculate() const override {
        double result = 0.0;
        for (size_t i = 0; i < count; ++i) {
            result += operands[i];
        }
        return result;
    }

    std::string getExpression() const override {
        std::string expr = "";
        for (size_t i = 0; i < count; ++i) {
            if (operands[i] < 0) {
                expr += "(" + formatNumber(operands[i]) + ")";
            }
            else {
                expr += formatNumber(operands[i]);
            }
            if (i < count - 1) expr += " + ";
        }
        return expr;
    }

    std::string getName() const override { return "Summator"; }
};

class Divisor : public ExpressionEvaluator, public IShuffle {
public:
    Divisor(size_t n = 20) : ExpressionEvaluator(n) {}

    double calculate() const override {
        double result = operands[0];
        for (size_t i = 1; i < count; ++i) {
            if (operands[i] == 0) return 0.0;
            result /= operands[i];
        }
        return result;
    }

    std::string getExpression() const override {
        std::string expr = "";
        for (size_t i = 0; i < count; ++i) {
            if (operands[i] < 0) {
                expr += "(" + formatNumber(operands[i]) + ")";
            }
            else {
                expr += formatNumber(operands[i]);
            }
            if (i < count - 1) expr += " / ";
        }
        return expr;
    }

    std::string getName() const override { return "Divisor"; }

    void shuffle() override {
        for (size_t i = 0; i < count - 1; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (operands[i] >= 0 && operands[j] < 0) {
                    std::swap(operands[i], operands[j]);
                }
            }
        }
    }

    void shuffle(size_t i, size_t j) override {
        if (i < count && j < count) {
            if (operands[i] < 0 && operands[j] >= 0) {
                std::swap(operands[i], operands[j]);
            }
        }
    }
};

class CustomExpressionEvaluator : public ExpressionEvaluator, public IShuffle {
public:
    CustomExpressionEvaluator(size_t n = 20) : ExpressionEvaluator(n) {}

    double calculate() const override {
        double result = operands[0];
        for (size_t i = 1; i < count; ++i) {
            if (i % 2 == 1) {
                result -= operands[i];
            }
            else {
                result += operands[i];
            }
        }
        return result;
    }

    std::string getExpression() const override {
        std::string expr = "";
        expr += formatNumber(operands[0]);

        for (size_t i = 1; i < count; ++i) {
            if (i % 2 == 1) {
                expr += " - ";
            }
            else {
                expr += " + ";
            }

            if (operands[i] < 0) {
                expr += "(" + formatNumber(operands[i]) + ")";
            }
            else {
                expr += formatNumber(operands[i]);
            }
        }
        return expr;
    }

    std::string getName() const override { return "CustomExpression"; }

    void shuffle() override {
        for (size_t i = 0; i < count - 1; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (operands[i] >= 0 && operands[j] < 0) {
                    std::swap(operands[i], operands[j]);
                }
            }
        }
    }

    void shuffle(size_t i, size_t j) override {
        if (i < count && j < count) {
            if (operands[i] < 0 && operands[j] >= 0) {
                std::swap(operands[i], operands[j]);
            }
        }
    }
};

int main() {
    std::ofstream clearLog("Lab3.log");
    clearLog.close();

    ExpressionEvaluator* evaluators[3];

    double summatorOps[] = { 5, 12.5, 9, -1.5, -9.5, 0, 11 };
    evaluators[0] = new Summator(7);
    evaluators[0]->setOperands(summatorOps, 7);

    evaluators[1] = new Divisor(4);
    evaluators[1]->setOperand(0, 100);
    evaluators[1]->setOperand(1, -4);
    evaluators[1]->setOperand(2, 2.5);
    evaluators[1]->setOperand(3, -4);

    double customOps[] = { 5, 4, -2, 9, 3 };
    evaluators[2] = new CustomExpressionEvaluator(5);
    evaluators[2]->setOperands(customOps, 5);

    for (size_t i = 0; i < 3; ++i) {
        evaluators[i]->logToFile("Lab3.log");
        evaluators[i]->logToScreen();
        std::cout << std::endl;
    }

    for (size_t i = 0; i < 3; ++i) {
        IShuffle* shuffleable = dynamic_cast<IShuffle*>(evaluators[i]);
        if (shuffleable) {
            shuffleable->shuffle();
            evaluators[i]->logToScreen();
            std::cout << std::endl;

            if (i == 2) {
                double temp[5];
                for (size_t j = 0; j < 5; ++j) {
                    temp[j] = evaluators[2]->getOperand(j);
                }

                evaluators[2]->setOperands(customOps, 5);
                shuffleable->shuffle(0, 4);
                evaluators[2]->logToScreen();
            }
        }
    }

    for (size_t i = 0; i < 3; ++i) {
        delete evaluators[i];
    }

    return 0;
}