#include <utility>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "NumberWithUnits.hpp"
#include <stdexcept>
#include <math.h>
#define EPSILON 0.0001 // Defined Because of Multiple Use || EPSILON - VERY SMALL NUM
#define ONE 1

using namespace std;

namespace ariel
{

    static map<string, map<string, double>> unitTable; // Nested Map With Units and its Converted unit

    // C++ map count function - return 1 if the container contains an element whose key is equivalent to k, or zero otherwise.



    void read_for_units(const string &a, const string &b)
    {
        for (auto const &foreachunit : unitTable[a])
        {
            double convertNumber = unitTable[b][a] * foreachunit.second;

            unitTable[b][foreachunit.first] = convertNumber;
            unitTable[foreachunit.first][b] = 1 / convertNumber;
        }
    }
    void NumberWithUnits::read_units(ifstream &units_file)
    {
        double quantity = 0;
        string funit = ""; // FIRST UNIT
        string sunit = ""; // SECOND UNIT

        while (units_file.good()) // WHILE VALID
        {
            units_file >> funit;
            if (funit == "1")
            {
                if ((units_file >> funit >> sunit >> quantity >> sunit))
                {
                    unitTable[funit][sunit] = quantity;
                    unitTable[sunit][funit] = ONE / quantity;
                read_for_units(funit,sunit);
                read_for_units(sunit,funit);
                }
            }
        }
    }


    ostream &operator<<(ostream &out, const NumberWithUnits &b)
    {
        out << b.number << "[" << b.type << ']';
        return out;
    }

    double transfer(const string &a, const string &b, double value)
    {
        // Function made To check and Transfer\Convert The Second Obj in order to make Valid Calculation

        //Obviously if Same Unit - Don't do anything but return because valid
        double convertedunit = unitTable.at(b).at(a); // At End if all good , convert by multiply this with value
        if (a == b)
        {
            return value;
        }
        //Not Connected - Throw...
        if (unitTable[a].find(b) == unitTable[a].end() || unitTable[b].find(a) == unitTable[b].end())
        {
            string ans = "Error - Can't be Transformed";
            throw std::out_of_range{ans};
        }
        //Main Purpose - All is VALID
        return value * convertedunit;
    }
    NumberWithUnits::NumberWithUnits(double number, const string &type) // Baic Constructor(number and type of unit), Take Exception into Considertation
    {

        if (unitTable.count(type) != ONE) // Trying to Create a NWU with no Valid Unit...
        {
            throw invalid_argument{"Invalid Unit Entry "};
        }
        this->type = type;
        this->number = number;
    }
    NumberWithUnits operator+(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        double b_converted_a = transfer(a.type, b.type, b.number);
        return NumberWithUnits(a.number + b_converted_a, a.type);
    }

    const NumberWithUnits &operator+(const NumberWithUnits &a)
    {
        return a;
    }
    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &n)
    {
        // Same OBJ + addition of valid obj
        double n_converted = transfer(this->type, n.type, n.number);
        this->number = this->number + n_converted;
        return *this;
    }
    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &n)
    {
        // Same OBJ - subtraction of valid obj
        double n_converted = transfer(this->type, n.type, n.number);
        this->number = this->number - n_converted;
        return *this;
    }
    NumberWithUnits operator-(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        // Valid subtraction

        return a + (b * -ONE);
    }
    NumberWithUnits operator-(const NumberWithUnits &n)
    {
        // Return Same Obj with - before 
        NumberWithUnits returnobj(-n.number, n.type);
        return returnobj;
    }
    // Comparison Between Two NWU OBJ || If Equal return 0, A bigger than B return 1, Else return -1
    int compare(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        // Convert Obviuously Needed in order to compare two valid objs of same Type.
        // Convert B to Type of A
        NumberWithUnits b_converted_a(transfer(a.type, b.type, b.number), a.type);
        int compareIndex = 0;
        double a_help = a.number;
        double b_help = b_converted_a.number;
        double difference = (a_help - b_help);
        double difference_ABS = fabs(difference);

        if (difference_ABS < EPSILON) // This Means they are Equal - RAMAT DIUK EPSILON
        {
            compareIndex = 0;
        }
        else if (difference > 0) // This Means a was bigger
        {
            compareIndex = ONE;
        }
        else // NO OTHER OPTION THAN a SMALLER THAN b
        {
            compareIndex = -1;
        }

        return compareIndex; // Return Based on Comparisons...
    }
    bool operator>(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return compare(a, b) > 0; // A Bigger then B hence Return 1 (Bigger than 0)
    }

    bool operator>=(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return compare(a, b) >= 0; // May Be Equal to Zero Because We are Checking Bigger or Equals
    }

    bool operator<(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return compare(a, b) < 0; // B bigger than A hence return -1 tht is smaller than 0
    }

    bool operator<=(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return compare(a, b) <= 0; // May be Equal hence also can return 0
        ;
    }

    bool operator==(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return compare(a, b) == 0; // Equals that is close as Epsilon -- Hence return 0
    }

    bool operator!=(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return !(a == b);
    }

    NumberWithUnits &operator++(NumberWithUnits &n)
    {
        // Same n Num + 1
        n.number = n.number + ONE;
        return n;
    }
    NumberWithUnits operator++(NumberWithUnits &n, int a)
    {
        // Will Return Copy without update like required (++ before)
        NumberWithUnits copy = NumberWithUnits(n);
        ++n;
        return copy;
    }
    NumberWithUnits &operator--(NumberWithUnits &n)
    {
        // Same n Num -1 Updated
        n.number = n.number - ONE;
        return n;
    }
    NumberWithUnits operator--(NumberWithUnits &n, int a)
    {
        // Will Return Copy without update like required (-- before)
        NumberWithUnits copy = NumberWithUnits(n);
        --n;
        return copy;
    }
    NumberWithUnits operator*(const NumberWithUnits &n, const long double &num)
    {
        NumberWithUnits b = NumberWithUnits(n);
        b.number = b.number * (double)num; // Update B Num Paramater....
        return b;
    }
    NumberWithUnits operator*(const long double &num23, const NumberWithUnits &n)
    {
        return n * num23;
    }
    istream &operator>>(istream &in, NumberWithUnits &a)
    {
        double sizeoff = 0;
        string unit = "";
        char tav = 0;

        in >> sizeoff >> tav >> unit >> tav;

        if (unit[unit.length() - ONE] == ']')
        {
            unit.pop_back();
        }
        if (tav == '-')
        {
            in.putback('-');
        }
        if (unitTable.find(unit) == unitTable.end())
        {
            throw std::invalid_argument("Unvalid Unit");
        }
        a.type = unit;
        a.number = sizeoff;

        return in;
    }
}
