#pragma once

#include <cmath>
#include <cstddef>
#include <algorithm>
#include <type_traits>

template <typename _type>
concept bool floating_point = std::is_floating_point<_type>::value;

template<typename _type, size_t _length>
struct vec {
    _type v[_length] = {{0}};

    using _vec = vec<_type, _length>;

    inline _vec operator+(const _vec& right) {
        _vec v = {{0}};

        for (size_t i = 0; i < _length; i++)
            v.v[i] = this->v[i] + right.v[i];
            
        return v;
    }

    inline _vec operator-(const _vec& right) {
        _vec v = {{0}};

        for (size_t i = 0; i < _length; i++)
            v.v[i] = this->v[i] - right.v[i];

        return v;
    }

    inline _vec operator*(const _vec& right) {
        _vec v = {{0}};

        for (size_t i = 0; i < _length; i++)
            v.v[i] = this->v[i] * right.v[i];

        return v;
    }

    inline _vec operator/(const _vec& right) {
        _vec v = {{0}};

        for (size_t i = 0; i < _length; i++)
            v.v[i] = this->v[i] / right.v[i];

        return v;
    }

    inline _type dot(const _vec& right) {
        _type res = 0;

        for (size_t i = 0; i < _length; i++)
            res += this->v[i] * right.v[i];

        return res;
    }

    inline _vec cross(const _vec& right) requires(_length == 3) {
        _vec v = {
            this->v[1]*right.v[2] - this->v[2]*right.v[1],
            this->v[0]*right.v[2] - this->v[2]*right.v[1],
            this->v[1]*right.v[1] - this->v[0]*right.v[1]
        };
        return v;
    }
};

template <typename _type, size_t _rows, size_t _cols>
struct mat {
    _type m[_rows * _cols];

    using _mat = mat<_type, _rows, _cols>;

    static mat identity() requires (_rows == _cols) {
        _mat m;
        std::fill(m.m, m.m + _rows * _cols, 0);

        for (size_t i = 0; i < _rows * _cols; i += _rows + 1)
            m.m[i] = 1;   

        return m; 
    }

    static mat translate(const vec<_type, _rows>& v) requires (_rows == _cols) {
        _mat m = _mat::identity();
        for (size_t i = 0; i < _rows; i++)
            m.m[i * _rows + _rows - 1] = v.v[i];
        return m;
    }

    static mat scale(const vec<_type, _rows>& v) requires (_rows == _cols) {
        _mat m;
        std::fill(m.m, m.m + _rows * _cols, 0);

        size_t vp = 0;
        for (size_t i = 0; i < _rows * _cols; i += _rows + 1)
        {
            m.m[i] = v.v[vp];
            vp++;
        }

        return m; 
    }

    static mat rotation_z(_type angle) requires (_rows == _cols && _cols >= 2 && floating_point<_type>) {
        _mat m = _mat::identity();

        float csin = sin(angle);
        float ccos = cos(angle);

        m.m[0] = ccos;
        m.m[1] = csin;
        m.m[_rows] = -csin;
        m.m[_rows + 1] = ccos;

        return m;
    }

    static mat rotation_y(_type angle) requires (_rows == _cols && _cols >= 3 && floating_point<_type>) {
        _mat m = _mat::identity();

        float csin = sin(angle);
        float ccos = cos(angle);

        m.m[0] = ccos;
        m.m[2] = csin;
        m.m[_rows * 2] = -csin;
        m.m[_rows * 2 + 1] = ccos;

        return m;
    }

    static mat rotation_x(_type angle) requires (_rows == _cols && _cols >= 3 && floating_point<_type>) {
        _mat m = _mat::identity();

        float csin = sinf(angle);
        float ccos = cosf(angle);

        m.m[_rows + 1] = ccos;
        m.m[_rows + 2] = csin;
        m.m[_rows * 2 + 1] = -csin;
        m.m[_rows * 2 + 2] = ccos;

        return m;
    }
};

template <typename _type, size_t _length>
inline vec<_type, _length> operator*(const vec<_type, _length>& left, const mat<_type, _length, _length>& right)
{
    vec<_type, _length> v = {{0}};
    for (size_t row = 0; row < _length; row++)
        for (size_t col = 0; col < _length; col++)
            v.v[row] += left.v[col]*right.m[row * _length + col];

    return v;
}