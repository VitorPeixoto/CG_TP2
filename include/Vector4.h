#ifndef VECTOR4_H
#define VECTOR4_H

#include <math.h>

template <class T>
class Vector4 {
    public:
        static const int X = 0;
        static const int Y = 1;
        static const int Z = 2;
        static const int W = 3;
        Vector4() {}
        Vector4(T _x, T _y, T _z, T _w) {
            components[X] = _x;
            components[Y] = _y;
            components[Z] = _z;
            components[W] = _w;
        }
        virtual ~Vector4() {}

           T getX() { return components[X]; }
        void setX(T val) { components[X] = val; }
           T getY() { return components[Y]; }
        void setY(T val) { components[Y] = val; }
           T getZ() { return components[Z]; }
        void setZ(T val) { components[Z] = val; }
           T getW() { return components[Z]; }
        void setW(T val) { components[W] = val; }
        void set(Vector4<T> v) {
            components[X] = v[X];
            components[Y] = v[Y];
            components[Z] = v[Z];
            components[W] = v[W];
        }
        T* getComponents() {
            return components;
        }

        T dotProduct(Vector4<T> axis) { return (components[X] * axis[X]) + (components[Y] * axis[Y]) + (components[Z] * axis[Z]); }
        static Vector4<T> crossProduct(Vector4<T> v, Vector4<T> u) {
            return Vector4((v[Y]*u[Z]) - (v[Z]*u[Y]),
                           (v[Z]*u[X]) - (v[X]*u[Z]),
                           (v[X]*u[Y]) - (v[Y]*u[X]),
                           v[W]);
        }

        T getNorm() {
            return sqrt(components[X] * components[X] +
                        components[Y] * components[Y] +
                        components[Z] * components[Z]);
        }

        static Vector4<T> normalized(Vector4<T> v) {
            return v / v.getNorm();
        }
        //Vector4<T> tripleProduct(Vector4<T> v1, Vector4<T> v2) { return (v1*this->dotProduct(v2)) - (v2*this->dotProduct(v1)); }
        Vector4<T> tripleProduct(Vector4<T> v1, Vector4<T> v2) { return (v1*this->dotProduct(v2)) - (*this*v1.dotProduct(v2)); }
        T max();

        T& operator[] (int index);
        const T& operator[] (int index) const;
        Vector4<T>  operator+(const Vector4<T>& vector) const;
        Vector4<T>  operator-(const Vector4<T>& vector) const;
        Vector4<T>  operator-() const;
        Vector4<T>  operator*(T num) const;
        Vector4<T>  operator/(T num) const;

        Vector4<T>& operator+=(const Vector4<T>& vector);
        Vector4<T>& operator-=(const Vector4<T>& vector);
        Vector4<T>& operator*=(T num);
        Vector4<T>& operator/=(T num);

        bool operator==(const Vector4<T>& vector) const;
        bool operator!=(const Vector4<T>& vector) const;
    protected:

    private:
        static const int DIMENSIONS = 4;
        T components[DIMENSIONS];
};

template <class T>
T& Vector4<T>::operator[] (int index) {
   if(index >= 0 && index < DIMENSIONS)
      return components[index];
}

template <class T>
const T& Vector4<T>::operator[] (int index) const {
   if(index >= 0 && index < DIMENSIONS)
      return components[index];
}

template <class T>
inline bool Vector4<T>::operator==(const Vector4<T>& vector) const {
    return components[X] == vector[X] && components[Y] == vector[Y] && components[Z] == vector[Z] && components[W] == vector[W];
}

template <class T>
inline bool Vector4<T>::operator!=(const Vector4<T>& vector) const {
        return components[X] != vector[X] || components[Y] != vector[Y] || components[Z] != vector[Z] || components[W] != vector[W];
}

template <class T>
inline Vector4<T> Vector4<T>::operator+(const Vector4<T>& vector) const {
        return Vector4<T>(components[X] + vector[X], components[Y] + vector[Y], components[Z] + vector[Z]);
}

template <class T>
inline Vector4<T> Vector4<T>::operator-(const Vector4<T>& vector) const {
        return Vector4<T>(components[X] - vector[X], components[Y] - vector[Y], components[Z] - vector[Z], components[W]);
}

template <class T>
inline Vector4<T> Vector4<T>::operator-() const {
        return Vector4<T>(-components[X], -components[Y], -components[Z]);
}

template <class T>
inline Vector4<T> Vector4<T>::operator*(T num) const {
        return Vector4<T>(components[X] * num, components[Y] * num, components[Z] * num);
}

template <class T>
inline Vector4<T> Vector4<T>::operator/(T num) const {
        return Vector4<T>(components[X] / num, components[Y] / num, components[Z] / num, components[W]);
}

template <class T>
Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& vector) {
        components[X] += vector[X];
        components[Y] += vector[Y];
        components[Z] += vector[Z];
        return *this;
}

template <class T>
Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& vector) {
        components[X] -= vector[X];
        components[Y] -= vector[Y];
        components[Z] -= vector[Z];
        return *this;
}

template <class T>
Vector4<T>& Vector4<T>::operator*=(T num) {
        components[X] *= num;
        components[Y] *= num;
        components[Z] *= num;
        return *this;
}

template <class T>
Vector4<T>& Vector4<T>::operator/=(T num) {
        components[X] /= num;
        components[Y] /= num;
        components[Z] /= num;
        return *this;
}
typedef Vector4<int>    Vector4i;
typedef Vector4<float>  Vector4f;
typedef Vector4<double> Vector4d;

#endif // VECTOR4_H
