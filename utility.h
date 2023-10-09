//
// Created by Mikhail Kiselev on 09.10.2023.
//

#ifndef NEDLER_MEAD_UTILITY_H
#define NEDLER_MEAD_UTILITY_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <functional>
#include <random>

using namespace std;


class Point;

using Function = function<double(const Point &)>;

auto sqr(const auto &x) {
    return x * x;
}

class Point : public vector<double> {
    void assert_match_sizes(const Point &other) const {
        if (this->size() != other.size())
            throw invalid_argument("sizes dont match");
    }

public:
    [[nodiscard]] double distance(const Point &other) const {
        assert_match_sizes(other);

        double ret = 0;
        for (size_t i = 0; i < size(); i++)
            ret += sqrt(sqr(at(i) - other.at(i)));
        return ret;
    }

    Point operator+(const Point &other) const {
        assert_match_sizes(other);

        auto ret = Point{};
        ret.resize(size());

        for (size_t i = 0; i < size(); i++) {
            ret[i] = at(i) + other.at(i);
        }

        return ret;
    }

    Point operator-(const Point &other) const {
        return *this + other * (-1);
    }

    Point operator*(double x) const {
        auto ret = Point{};
        ret.resize(size());
        for (size_t i = 0; i < size(); i++) {
            ret[i] = at(i) * x;
        }
        return ret;
    }

    Point operator/(double x) const {
        return *this * (1.0 / x);
    }

    [[nodiscard]] Point extended(double x) const {
        auto copy = *this;
        copy.push_back(x);
        return copy;
    }

    static Point random(size_t dimension, double min, double max) {
        uniform_real_distribution<double> unif(min, max);
        static default_random_engine re;

        auto ret = Point{};
        ret.resize(dimension);
        for (double &i: ret) {
            i = unif(re);
        }
        return ret;
    }

    static vector<Point> generate_vector(size_t size, size_t dimensions, double min, double max) {
        auto ret = vector<Point>(size);
        for (size_t i = 0; i < size; i++) {
            ret[i] = Point::random(dimensions, min, max);
        }
        return ret;
    }

    ostream &operator<<(ostream &out) const {
        out << "{";
        for (size_t i = 0; i < size(); i++) {
            if (i != 0) {
                cout << ", ";
            }
            cout << at(i);
        }
        return out << "}";
    }
};

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &vec) {
    out << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        if (i != 0) {
            cout << ", ";
        }
        cout << vec.at(i);
    }
    return out << "]";
}

double MSE_with_func_as_extra_coordinate(const Function &func, const vector<Point> &lhs, const vector<Point> &rhs) {
    assert(lhs.size() == rhs.size());

    double ret = 0;
    for (size_t i = 0; i < lhs.size(); i++) {
        ret += sqr(lhs[i].extended(func(lhs[i])).distance(rhs[i].extended(func(rhs[i])))) / lhs.size();
    }
    return ret;
}

auto sub_vector(const auto &vec, size_t begin, size_t end) {
    return vector(vec.begin() + begin, vec.end() - (vec.size() - end));
}

Point centroid(const vector<Point> &polygon) {
    auto ret = polygon[0];
    for (size_t i = 1; i < polygon.size(); i++) {
        ret = ret + polygon[i];
    }
    return ret / polygon.size();
}

#endif //NEDLER_MEAD_UTILITY_H
