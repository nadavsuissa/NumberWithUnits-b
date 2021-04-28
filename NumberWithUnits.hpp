
#include <iostream>
#include <string>
using namespace std;

namespace ariel
{
    class NumberWithUnits
    {

    private:
        double number;
        string type;

    public:
        NumberWithUnits(double value, const string &unit);

        static void read_units(ifstream &units_file);

        friend NumberWithUnits operator+(const NumberWithUnits &a, const NumberWithUnits &b);

        friend const NumberWithUnits &operator+(const NumberWithUnits &a);
        NumberWithUnits &operator+=(const NumberWithUnits &n);

        friend NumberWithUnits operator-(const NumberWithUnits &a, const NumberWithUnits &b);
        friend NumberWithUnits operator-(const NumberWithUnits &n);
        NumberWithUnits &operator-=(const NumberWithUnits &n);

        friend NumberWithUnits &operator++(NumberWithUnits &n);
        friend NumberWithUnits operator++(NumberWithUnits &n, int a);
        friend NumberWithUnits &operator--(NumberWithUnits &n);

        friend int compare(const NumberWithUnits &a, const NumberWithUnits &b);

        friend NumberWithUnits operator--(NumberWithUnits &n, int a);
        friend NumberWithUnits operator*(const NumberWithUnits &n, const long double &num);
        friend NumberWithUnits operator*(const long double &num, const NumberWithUnits &n);
        friend bool operator>(const NumberWithUnits &a, const NumberWithUnits &b);
        friend bool operator>=(const NumberWithUnits &a, const NumberWithUnits &b);

        friend bool operator<(const NumberWithUnits &a, const NumberWithUnits &b);
        friend bool operator<=(const NumberWithUnits &a, const NumberWithUnits &b);
        
        friend bool operator==(const NumberWithUnits &a, const NumberWithUnits &b);
        friend bool operator!=(const NumberWithUnits &a, const NumberWithUnits &b);
        friend ostream &operator<<(ostream &out, const NumberWithUnits &a);
        friend istream &operator>>(istream &s, NumberWithUnits &n);
    };
}