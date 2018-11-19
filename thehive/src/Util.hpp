#ifndef _UTIL_H
#define _UTIL_H

#include <cstdint>
#include <iostream>

namespace gg{

    /*          UNSIGNED INT - 8 BITS       */
    //struct Vector3u8{
    //    Vector3u8(uint8_t _X = 0, uint8_t _Y = 0, uint8_t _Z = 0);
    //    Vector3u8(const Vector3u8 &orig);

    //    uint8_t X, Y, Z;
    //};

    //struct Vector2u8{
    //    Vector2u8(uint8_t _X = 0, uint8_t _Y = 0);
    //    Vector2u8(const Vector2u8 &orig);

    //    uint8_t X, Y;
    //};



    ///*          UNSIGNED INT - 16 BITS       */
    //struct Vector3u16{
    //    Vector3u16(uint16_t _X = 0, uint16_t _Y = 0, uint16_t _Z = 0);
    //    Vector3u16(const Vector3u16 &orig);

    //    uint16_t X, Y, Z;
    //};

    //struct Vector2u16{
    //    Vector2u16(uint16_t _X = 0, uint16_t _Y = 0);
    //    Vector2u16(const Vector2u16 &orig);

    //    uint16_t X, Y;
    //};



    ///*          INT - 8 BITS       */
    //struct Vector3i8{
    //    Vector3i8(int8_t _X = 0, int8_t _Y = 0, int8_t _Z = 0);
    //    Vector3i8(const Vector3i8 &orig);

    //    int8_t X, Y, Z;
    //};

    //struct Vector2i8{
    //    Vector2i8(int8_t _X = 0, int8_t _Y = 0);
    //    Vector2i8(const Vector2i8 &orig);

    //    int8_t X, Y;
    //};



    /*          INT - 16 BITS       */
    //struct Vector3i16{
    //    Vector3i16(int16_t _X = 0, int16_t _Y = 0, int16_t _Z = 0);
    //    Vector3i16(const Vector3i16 &orig);

    //    int16_t X, Y, Z;
    //};

    //struct Vector2i16{
    //    Vector2i16(int16_t _X = 0, int16_t _Y = 0);
    //    Vector2i16(const Vector2i16 &orig);

    //    int16_t X, Y;
    //};



    /*          FLOAT       */
    struct Vector3f{
        Vector3f(float _X = 0, float _Y = 0, float _Z = 0);
        Vector3f(const Vector3f &orig);

        float X, Y, Z;
        Vector3f operator+ (const Vector3f &v1);
        Vector3f operator- (const Vector3f &v1);
        Vector3f operator* (const float &multiplier);
        Vector3f operator* (const Vector3f &v1);
        Vector3f operator/ (const float &divider);

        void operator+= (const Vector3f &v1);
        void operator-= (const Vector3f &v1);
        void operator*= (const float &v1);
        void operator*= (const Vector3f &v1);
        void operator/= (const float &divider);
    };



    //struct Vector2f{
    //    Vector2f(float _X = 0, float _Y = 0);
    //    Vector2f(const Vector2f &orig);

    //    float X, Y;
    //};

    template <typename T>
    float DIST(const T &v1, const T &v2);

    template <typename T>
    float Modulo(const T &v1);

    template <typename T>
    T Normalice(T v1);

    template <typename T>
    T operator+=(const T &v1, const T &v2);

    float genFloatRandom(float min, float max);
    uint16_t genIntRandom(uint16_t min, uint16_t max);

};

std::ostream& operator<<(std::ostream& os, const gg::Vector3f &v1);
//template <typename T>
//T gg::T::operator+ (T v1);

#endif
