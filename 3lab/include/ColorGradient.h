#ifndef INC_3LAB_COLORGRADIENT_H
#define INC_3LAB_COLORGRADIENT_H
#include <vector>
#include "ColorTuple.h"

class ColorGradient {
    /*
     * N-color linear gradient.
     * */
public:
    std::vector<ColorTuple> baseColors;
    ColorGradient(std::vector<ColorTuple> colors);
    ColorTuple getInterpolatedColor(float alpha);

};


#endif //INC_3LAB_COLORGRADIENT_H
