#ifndef TP2_PLANETENUM_H
#define TP2_PLANETENUM_H

extern const double DISTANCE_DIVIDER;
extern const double SIZE_MULTIPLIER;
extern const double FPS_CONST;

const double SUN_SIZE   = 7;
const double SUN_RADIUS = 1*SUN_SIZE;

// Um ano em 60 segundos
/*const double EARTH_TRANSLATION = 0.017*6;
const double EARTH_ROTATION    = EARTH_TRANSLATION*365;*/

class PlanetEnum {
private:
    const double size, distance, translationAngle, rotationAngle;
    const char* texturePath;
    const bool cullFace;
    PlanetEnum(double size, double distance, double translationAngle, double rotationAngle, const char* texturePath, bool cullFace) : size(size), distance(distance), translationAngle(translationAngle), rotationAngle(rotationAngle), texturePath(texturePath), cullFace(cullFace) {}
public:
    static const PlanetEnum SUN();
    static const PlanetEnum MERCURY();
    static const PlanetEnum VENUS();
    static const PlanetEnum EARTH();
        static const PlanetEnum MOON();
            static const PlanetEnum ROCKET();
        static const PlanetEnum EARTH_ATMOSPHERE();
    static const PlanetEnum MARS();
    static const PlanetEnum JUPITER();
    static const PlanetEnum SATURN();
        static const PlanetEnum SATURN_RING();
    static const PlanetEnum URANUS();
    static const PlanetEnum NEPTUNE();
    static const PlanetEnum PLUTO();
    static const PlanetEnum STARS();

    const bool getCullFace() const;

    static const double EARTH_TRANSLATION() {
        static double ans = 0.017*6*0.05;
        return ans;
    }

    static const double EARTH_ROTATION()    {
        static double ans = EARTH_TRANSLATION()*365;
        return  ans;
    }

    const double getSize() const;
    const double getDistance() const;

    const double getTranslationAngle() const;
    const double getRotationAngle() const;

    const char *getTexturePath() const;
};

#endif //TP2_PLANETENUM_H
