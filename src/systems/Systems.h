//
// Created by Jorge Jimenez on 7/11/25.
//

#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "../components/Components.h"
#include <cmath>

class Systems
{
};

class Interpolate : Systems
{
public:
    template<typename T>
    T interpolate(T t, InterpolationType const type)
    {
        switch(type)
        {
            case EASEOUT_SINE:
                return easeoutSine(t);
            case EASEIN_SINE:
                return easeinSine(t);
            case EASEOUT_ELASTIC:
                return easeoutElastic(t);
            case EASEIN_ELASTIC:
                return easeinElastic(t);
            case EASEINOUT_SINE:
                return easeinoutSine(t);
            case EASEINOUT_ELASTIC:
                return easeinoutElastic(t);
            case EASEINOUT_EXPO:
                return easeinoutExpo(t);
            case EASEIN_EXPO:
                return easeinExpo(t);
            default:
                return easeinElastic(t);
        }

    }

// EASEOU_SINE,
private:
    template<typename T>
    T easeoutSine(T t)
    {
        return sin((t * M_PI) / 2);
    }

    template<typename T>
    T easeoutElastic(T t)
    {
        constexpr auto c4 = (2 * M_PI) / 3;

        return t == 0
          ? 0
          : t == 1
          ? 1
          : pow(2, -10 * t) * sin((t * 10 - 0.75) * c4) + 1;
    }
    template<typename T>
    T easeinoutElastic(T t)
    {
        constexpr auto c5 = (2 * M_PI) / 4.5;

        return t == 0
          ? 0
          : t == 1
          ? 1
          : t < 0.5
          ? -(pow(2, 20 * t - 10) * sin((20 * t - 11.125) * c5)) / 2
          : (pow(2, -20 * t + 10) * sin((20 * t - 11.125) * c5)) / 2 + 1;
    }

    template<typename T>
    T easeinoutSine(T t)
    {
        return -(cos(M_PI * t) - 1) / 2;
    }

    template<typename T>
    T easeinSine(T t)
    {
        return 1 - cos((t * M_PI) / 2);
    }

    template<typename T>
    T easeinElastic(T t)
    {
        constexpr auto c4 = (2 * M_PI) / 3;

        return t == 0
          ? 0
          : t == 1
          ? 1
          : -pow(2, 10 * t - 10) * sinf((t * 10 - 10.75) * c4) + 1;
    }
    template<typename T>
    T easeinoutExpo(T t)
    {
        return t == 0
          ? 0
          : t == 1
          ? 1
          : t < 0.5 ? pow(2, 20 * t - 10) / 2
          : (2 - pow(2, -20 * t + 10)) / 2;
    }

    template<typename T>
    T easeinExpo(T t)
    {
        return t == 0 ? 0 : pow(2, 10 * t - 10) + 1;
    }
};



#endif //SYSTEMS_H
