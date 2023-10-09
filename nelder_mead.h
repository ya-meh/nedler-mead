//
// Created by Mikhail Kiselev on 09.10.2023.
//

#ifndef NEDLER_MEAD_NELDER_MEAD_H
#define NEDLER_MEAD_NELDER_MEAD_H

#include "utility.h"


struct NelderMeadDebugInfo {
    bool debug;
    size_t call;

    void log(const vector<Point> &curr, const vector<Point> &prev, double mse) {
        if (debug) {
            call += 1;
            cout << "#" << call << "\t" << prev << "\t->\t" << curr << "\tMSE: " << mse << endl;
        }
    }
};

// https://en.wikipedia.org/wiki/Nelder–Mead_method
// alpha > 0
// gamma > 1
// 0 < rho <= 0.5
Point nelder_mead_method(const Function &func, vector<Point> &x, double tolerance, NelderMeadDebugInfo debug = {},
                         double alpha = 1, double gamma = 2, double rho = 0.5, double sigma = 0.5) {
    auto return_or_go_deeper = [&, prev_x = x]() -> Point {
        auto mse = MSE_with_func_as_extra_coordinate(func, x, prev_x);
        if (mse < tolerance) {
            return x[0];
        }
        debug.log(x, prev_x, mse);
        return nelder_mead_method(func, x, tolerance, debug, alpha, gamma, rho, sigma);
    };

    // 1. Order
    sort(x.begin(), x.end(), [&](const auto &lhs, const auto &rhs) -> bool {
        return func(lhs) < func(rhs);
    });

    // 2. Calculate x_o, the centroid of all points except x_n+1
    auto x_o = centroid(sub_vector(x, 0, x.size() - 1));

    // 3. Reflection
    auto x_r = x_o + (x_o - x.back()) * alpha;
    if (func(x[0]) <= func(x_r) && func(x_r) < func(x[x.size() - 2])) {
        x.back() = x_r;
        return return_or_go_deeper();
    }

    // 4. Expansion
    if (func(x_r) < func(x[0])) {
        auto x_e = x_o + (x_r - x_o) * gamma;
        if (func(x_e) < func(x_r)) {
            x.back() = x_e;
            return return_or_go_deeper();;
        } else {
            x.back() = x_r;
            return return_or_go_deeper();
        }
    }

    // 5. Contraction
    if (func(x_r) >= func(x[x.size() - 2])) {
        if (func(x_r) < func(x.back())) {
            if (auto x_c = x_o + (x_r - x_o) * rho; func(x_c) < func(x_r)) {
                x.back() = x_c;
                return return_or_go_deeper();
            }
        } else {
            if (auto x_c = x_o + (x.back() - x_o) * rho; func(x_c) < func(x_r)) {
                x.back() = x_c;
                return return_or_go_deeper();
            }
        }
    }

    // 6. Shrink
    for (size_t i = 1; i < x.size(); i++) {
        x[i] = x[0] + (x[i] - x[0]) * sigma;
    }
    return return_or_go_deeper();
}

#endif //NEDLER_MEAD_NELDER_MEAD_H
