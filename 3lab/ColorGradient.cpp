#include "include/ColorGradient.h"

ColorGradient::ColorGradient(std::vector<ColorTuple> colors) {
    this->baseColors = colors;
}

ColorTuple ColorGradient::getInterpolatedColor(float alpha) {
    float scaled_alpha = alpha * this->baseColors.size();
    ColorTuple firstColor, secondsColor;
    if ((int)scaled_alpha == 0){
        firstColor = this->baseColors.at(0);
        secondsColor = this->baseColors.at(0);
    } else if ((int) scaled_alpha == this->baseColors.size()){
        firstColor = this->baseColors.at(this->baseColors.size()-1);
        secondsColor = this->baseColors.at(this->baseColors.size()-1);
    } else {
        firstColor = this->baseColors.at(((int) scaled_alpha));
        secondsColor = this->baseColors.at(((int) scaled_alpha));
    }
    float newInterpolationAlpha = scaled_alpha - (int) scaled_alpha;
    float r = firstColor.r + (secondsColor.r - firstColor.r) * newInterpolationAlpha;
    float g = firstColor.g + (secondsColor.g - firstColor.g) * newInterpolationAlpha;
    float b = firstColor.b + (secondsColor.b - firstColor.b) * newInterpolationAlpha;
    return ColorTuple{r, g, b};
}
