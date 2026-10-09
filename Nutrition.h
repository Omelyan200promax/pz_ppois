#pragma once
// КБЖУ: калории, белки, жиры, углеводы.
#include <string>
#include <sstream>
#include <iomanip>

struct Nutrition {
    double kcal = 0, protein = 0, fat = 0, carbs = 0;

    Nutrition& operator+=(const Nutrition& o) {
        kcal += o.kcal; protein += o.protein; fat += o.fat; carbs += o.carbs;
        return *this;
    }
};

inline Nutrition operator+(Nutrition a, const Nutrition& b) { a += b; return a; }

inline std::string toString(const Nutrition& n) {
    std::ostringstream s;
    s << std::fixed << std::setprecision(1)
      << n.kcal << " ккал | Б " << n.protein << " | Ж " << n.fat << " | У " << n.carbs;
    return s.str();
}
//inline для того, чтобы использовать функции в нескольких cpp
