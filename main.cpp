#include "nelder_mead.h"

using namespace std;


// https://en.wikipedia.org/wiki/Himmelblau%27s_function
// It has one local maximum at M = {0.270845, 0.923039}, func(M) = 181.617
//
// Four identical local minima:
// 1. func({ 3,         2})        = 0
// 2. func({-2.805118,  3.131312}) = 0
// 3. func({-3.779310, -3.283186}) = 0
// 4. func({ 3.584428, -1.848126}) = 0
double himmelblau_function(const Point &p) {
    return sqr(sqr(p[0]) + p[1] - 11) + sqr(p[0] + sqr(p[1]) - 7);
}


int main() {
    auto start = Point::generate_vector(3, 2, -5, 5);
    cout << "Start in " << start << "\n\n";
    auto ret = nelder_mead_method(himmelblau_function, start, 0.01, {true});
    cout << "\nMin point: " << ret;

    cout << "\n\n----------------\nOther starts:\n";
    for (size_t i = 1; i < 10; i++) {
        auto start = Point::generate_vector(3, 2, -5, 5);
        cout << i + 1 << '.' << "\tSTART:\t" << start
             << "\tMIN:\t" << nelder_mead_method(himmelblau_function, start, 0.00000001) << endl;
    }
    return 0;
}
