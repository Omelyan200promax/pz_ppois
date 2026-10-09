#include "PhysicalActivity.h"

#include <stdexcept>

double intensityFactor(Intensity i) {
    switch (i) {
        case Intensity::Low:    return 0.8;
        case Intensity::Medium: return 1.0;
        case Intensity::High:   return 1.2;
    }
    return 1.0;
}

std::string intensityName(Intensity i) {
    switch (i) {
        case Intensity::Low:    return "низкая";
        case Intensity::Medium: return "средняя";
        case Intensity::High:   return "высокая";
    }
    return "?";
}
//не доделано