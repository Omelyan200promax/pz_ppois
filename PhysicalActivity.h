#pragma once
#include <string>
#include <fstream>

// Интенсивность тренировки. enum class — значения не "протекают" в глобальную область.
enum class Intensity { Low = 0, Medium = 1, High = 2 };

double intensityFactor(Intensity i);        // множитель к MET: 0.8 / 1.0 / 1.2
std::string intensityName(Intensity i);     // "низкая" / "средняя" / "высокая"

// Один ВИД активности (бег, плавание...). Не конкретная тренировка, а справочная запись.
class PhysicalActivity {
public:
    PhysicalActivity();                                      // нужен для чтения из файла
    PhysicalActivity(const std::string& name, double met);   // с проверкой данных

    const std::string& getName() const;
    double getMet() const;

    // ккал = MET * множитель интенсивности * вес(кг) * время(ч)
    double calcKcal(double weightKg, int minutes, Intensity intensity) const;

    void write(std::ofstream& out) const;
    void read(std::ifstream& in);

private:
    std::string name_;
    double met_;
};
//для того, чтобы не было ошибок двойного определения, класс в одном файле, а его методы в cpp/ Так клод подсказал, это уточнить