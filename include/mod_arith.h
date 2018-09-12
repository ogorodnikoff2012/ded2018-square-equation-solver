#pragma once

#include <cassert>

class Residue {
    public:
        explicit Residue(unsigned int value, unsigned int mod) : value_(value % mod), mod_(mod) {}
        Residue(const Residue& other) : value_(other.value_), mod_(other.mod_) {}

        Residue& operator =(const Residue& other) {
            value_ = other.value_;
            mod_ = other.mod_;
            return *this;
        }

        Residue& operator +=(const Residue& other) {
            assert(mod_ == other.mod_);
            value_ += other.value_;
            if (value_ >= mod_) {
                value_ -= mod_;
            }
            return *this;
        }

        Residue operator +(const Residue& other) const {
            Residue ans = *this;
            return ans += other;
        }

        Residue operator -() const {
            return Residue((value_ == 0) ? 0 : (mod_ - value_), mod_);
        }

        Residue& operator -=(const Residue& other) {
            return operator +=(-other);
        }

        Residue operator -(const Residue& other) const {
            return *this + (-other);
        }

        Residue& operator *=(const Residue& other) {
            assert(mod_ == other.mod_);
            unsigned long long result = value_;
            result *= other.value_;
            value_ = result % mod_;
            return *this;
        }

        Residue operator *(const Residue& other) const {
            Residue result = *this;
            return result *= other;
        }

        Residue pow(unsigned int n) const {
            if (n == 0) {
                return Residue(1, mod_);
            }

            if (n == 1) {
                return *this;
            }

            if (n % 2 == 0) {
                return (*this * *this).pow(n / 2);
            }

            return *this * pow(n - 1);
        }

        Residue inverse() const {
            assert(value_ != 0);
            return pow(mod_ - 2);
        }

        Residue &operator /=(const Residue& other) {
            return (*this) *= other.inverse();
        }

        Residue operator /(const Residue& other) const {
            Residue result = *this;
            return result /= other;
        }

        operator unsigned int() const {
            return value_;
        }
    private:
        unsigned int value_;
        unsigned int mod_;
};
