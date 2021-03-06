#include "Color.h"
#include "Arduino.h"
#include <math.h>

#define MIN3(x,y,z)  ((y) <= (z) ? \
                         ((x) <= (y) ? (x) : (y)) \
                     : \
                         ((x) <= (z) ? (x) : (z)))

#define MAX3(x,y,z)  ((y) >= (z) ? \
                         ((x) >= (y) ? (x) : (y)) \
                     : \
                         ((x) >= (z) ? (x) : (z)))

Color::Color(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
    updateHSV();
}

int Color::boundedRGB(int val) const {
    return boundedVal(val, 0, 255);
}

double Color::boundedH(double val) const {
    return boundedVal(val, 0.0, 360.0);
}

double Color::boundedSV(double val) const {
    return boundedVal(val, 0.0, 1.0);
}


void Color::updateRGB() {
    double h = this->h;
    double s = this->s;
    double v = this->v;

    double c = v * s;
    double x = c * (1.0 - abs(fmod(h/60,2.0) - 1.0));

    double m = v - c;

    double r = 0;
    double g = 0;
    double b = 0;

    if(h < 60) {
        r = c;
        g = x;
        b = 0;
    }
    else if(h < 120) {
        r = x;
        g = c;
        b = 0;
    }
    else if(h < 180) {
        r = 0;
        g = c;
        b = x;
    }
    else if(h < 240) {
        r = 0;
        g = x;
        b = c;
    }
    else if(h < 300) {
        r = x;
        g = 0;
        b = c;
    }
    else {
        r = c;
        g = 0;
        b = x;
    }

    this->r = (int) round((r + m)*255);
    this->g = (int) round((g + m)*255);
    this->b = (int) round((b + m)*255);

}

void Color::updateHSV() {
    double r = ((double)this->r) / 255.0;
    double g = ((double)this->g) / 255.0;
    double b = ((double)this->b) / 255.0;

    double cMin = MIN3(r,g,b);
    double cMax = MAX3(r,g,b);

    double delta = cMax - cMin;

    this->v = cMax;

    if(delta == 0) {
        this->h = 0;
        this->s = 0;
        return;
    }
    else {
        this->s = delta/cMax;
    }

    double hVal = 0;

    if(cMax == r) {
        hVal = 60 * (g-b)/delta;
    }
    else if(cMax == g) {
        hVal = 60 * ((b - r)/delta + 2);
    }
    else {
        hVal = 60 * ((r - g)/delta + 4);
    }

    if(hVal < 0) {
        hVal += 360;
    }

    this->h = hVal;
}

int Color::getR() const {
    return r;
}

int Color::getG() const {
    return g;
}

int Color::getB() const {
    return b;
}

void Color::setR(int r) {
    this->r = boundedRGB(r);
    updateHSV();
}

void Color::setG(int g) {
    this->g = boundedRGB(g);
    updateHSV();
}

void Color::setB(int b) {
    this->b = boundedRGB(b);
    updateHSV();
}

void Color::setRGB(int r, int g, int b) {
    this->r = boundedRGB(r);
    this->g = boundedRGB(g);
    this->b = boundedRGB(b);
    updateHSV();
}

void Color::setColor(Color& other) {
    this->r = other.getR();
    this->g = other.getG();
    this->b = other.getB();
    
    this->h = other.getH();
    this->s = other.getS();
    this->v = other.getV();
}

void Color::incR(int r) {
    this->r = boundedRGB(this->r + r);
    updateHSV();
}

void Color::incG(int g) {
    this->g = boundedRGB(this->g + g);
    updateHSV();
}

void Color::incB(int b) {
    this->b = boundedRGB(this->b + b);
    updateHSV();
}

double Color::getH() const {
    return h;
}

double Color::getS() const {
    return s;
}

double Color::getV() const {
    return v;
}

void Color::setH(double h) {
    this->h = boundedH(h);
    updateRGB();
}

void Color::setS(double s) {
    this->s = boundedSV(s);
    updateRGB();
}

void Color::setV(double v) {
    this->v = boundedSV(v);
    updateRGB();
}

void Color::setHSV(double h, double s, double v) {
    this->h = boundedH(h);
    this->s = boundedSV(s);
    this->v = boundedSV(v);
    updateRGB();
}

void Color::incH(double h) {
    this->setH((this->getH() + h));
}

void Color::incS(double s) {
    this->setS(this->getS() + s);
}

void Color::incV(double v) {
    this->setV(this->getV() + v);
}

void Color::cycleH(double h) {
    this->setH(fmod(this->h + h, 360.0));
}

bool Color::operator==(const Color &other) {
    return (this->r == other.r &&
            this->g == other.g &&
            this->b == other.b);
}

bool Color::operator!=(const Color &other) {
    return !(*this == other);
}
