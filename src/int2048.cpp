#include "int2048.h"

namespace sjtu {

    int2048::int2048() {
        sign = 1;
        num.push_back(0);
    }

    int2048::int2048(long long a) {
        if (a < 0) {
            sign = -1;
            a *= -1;
        } else {
            sign = 1;
        }
        if (a == 0) {
            num.push_back(0);
        } else {
            while (a > 0) {
                num.push_back(a % base);
                a /= base;
            }
        }
    }

    int2048::int2048(const std::string & num0) {
        read(num0);
    }

    int2048::int2048(const int2048 & num0) {
        *this = num0;
    }

    void int2048::read(const std::string & num0) {
        int len0 = num0.size(), len = num.size();
        if (num0[0] == '-') {
            sign = -1;
            for (int i = 0; i < len0 - 1; i++) {
                if (i < len) {
                    num[i] = num0[len0 - i - 1] - '0';
                } else {
                    num.push_back(num0[len0 - i - 1] - '0');
                }
            }
            if (len > len0 - 1) {
                for (int i = len0 - 1; i < len; i++) {
                    num.pop_back();
                }
            }
        } else {
            sign = 1;
            for (int i = 0; i < len0; i++) {
                if (i < len) {
                    num[i] = num0[len0 - i - 1] - '0';
                } else {
                    num.push_back(num0[len0 - i - 1] - '0');
                }
            }
            if (len > len0) {
                for (int i = len0; i < len; i++) {
                    num.pop_back();
                }
            }
        }
        len = num.size();
        int len_trans = (len + digit - 1) / digit;
        for (int i = 0, j = 0; i < len_trans; i++) {
            num[i] = num[j++];
            for (int k = 1, m = 10; k < digit && j < len; k++, m *= 10) {
                num[i] += num[j++] * m;
            }
        }
        for (int i = len - 1; i >= len_trans; i--) {
            num.pop_back();
        }
        for (int i = len_trans - 1; i >= 1; i--) {
            if (num[i] == 0) {
                num.pop_back();
            } else {
                break;
            }
        }
        if (num.size() == 1 && num[0] == 0) {
            sign = 1;
        }
    }

    void int2048::print() const {
        std::cout << *this;
    }

    int2048 & int2048::add(const int2048 & num0) {
        if (num0 == 0) {
            return *this;
        }
        if (sign == num0.sign) {
            int len = num.size(), len0 = num0.num.size();
            int progression = 0;
            for (int i = 0; i < std::min(len, len0); i++) {
                num[i] += num0.num[i] + progression;
                progression = 0;
                if (num[i] >= base) {
                    progression = num[i] / base;
                    num[i] %= base;
                }
            }
            if (progression != 0) {
                if (len == len0) {
                    num.push_back(progression);
                } else if (len > len0) {
                    for (int i = len0; i < len; i++) {
                        num[i] += progression;
                        progression = 0;
                        if (num[i] >= base) {
                            progression = num[i] / base;
                            num[i] %= base;
                        } else {
                            break;
                        }
                    }
                    if (progression != 0) {
                        num.push_back(progression);
                    }
                }
            }
            if (len < len0) {
                long long tmp_num;
                for (int i = len; i < len0; i++) {
                    if (progression == 0) {
                        num.push_back(num0.num[i]);
                    } else {
                        tmp_num = num0.num[i] + progression;
                        progression = 0;
                        if (tmp_num >= base) {
                            progression = tmp_num / base;
                            tmp_num %= base;
                        }
                        num.push_back(tmp_num);
                    }
                }
                if (progression != 0) {
                    num.push_back(progression);
                }
            }
            return *this;
        } else {
            return minus(-num0);
        }
    }

    int2048 & int2048::minus(const int2048 & num0) {
        if (num0 == 0) {
            return *this;
        }
        if (sign == num0.sign) {
            int len = num.size(), len0 = num0.num.size();
            int sign0 = abs_cmp(*this, num0);
            if (sign0 == 1 || sign0 == 0) {
                bool if_borrowing = false;
                for (int i = 0; i < len0; i++) {
                    if (if_borrowing) {
                        if_borrowing = false;
                        num[i]--;
                    }
                    num[i] -= num0.num[i];
                    if (num[i] < 0) {
                        num[i] += base;
                        if_borrowing = true;
                    }
                }
                if (if_borrowing) {
                    for (int i = len0; i < len; i++) {
                        num[i]--;
                        if (num[i] >= 0) {
                            break;
                        } else {
                            num[i] += base;
                        }
                    }
                }
            } else {
                bool if_borrowing = false;
                for (int i = 0; i < len; i++) {
                    if (if_borrowing) {
                        num[i]++;
                        if_borrowing = false;
                    }
                    num[i] = num0.num[i] - num[i];
                    if (num[i] < 0) {
                        num[i] += base;
                        if_borrowing = true;
                    }
                }
                long long tmp_num;
                for (int i = len; i < len0; i++) {
                    if (if_borrowing) {
                        tmp_num = num0.num[i] - 1;
                        if_borrowing = false;
                        if (tmp_num < 0) {
                            tmp_num += base;
                            if_borrowing = true;
                        }
                        num.push_back(tmp_num);
                    } else {
                        num.push_back(num0.num[i]);
                    }
                }
                sign *= -1;
            }
            for (int i = num.size() - 1; i >= 1; i--) {
                if (num[i] == 0) {
                    num.pop_back();
                } else {
                    break;
                }
            }
            if (num.size() == 1 && num[0] == 0) {
                sign = 1;
            }
            return *this;
        } else {
            return add(-num0);
        }
    }

    int2048 add(int2048 a, const int2048 & b) {
        return a.add(b);
    }

    int2048 minus(int2048 a, const int2048 & b) {
        return a.minus(b);
    }

    int2048 &int2048::operator=(const int2048 & num0) {
        sign = num0.sign;
        int len = num.size(), len0 = num0.num.size();
        if (len >= len0) {
            for (int i = 0; i < len0; i++) {
                num[i] = num0.num[i];
            }
            for (int i = len0; i < len; i++) {
                num.pop_back();
            }
        } else {
            for (int i = 0; i < len; i++) {
                num[i] = num0.num[i];
            }
            for (int i = len; i < len0; i++) {
                num.push_back(num0.num[i]);
            }
        }
        return *this;
    }

    int2048 operator+(const int2048 & a, const int2048 & b) {
        return add(a, b);
    }

    int2048 operator-(const int2048 & a, const int2048 & b) {
        return minus(a, b);
    }

    int2048 int2048::operator+() const {
        return *this;
    }

    int2048 int2048::operator-() const {
        if (*this == 0) {
            return *this;
        }
        int2048 num0(*this);
        num0.sign *= -1;
        return num0;
    }

    int2048 &int2048::operator+=(const int2048 & num0) {
        return add(num0);
    }

    int2048 &int2048::operator-=(const int2048 & num0) {
        return minus(num0);
    }

    bool operator==(const int2048 & a, const int2048 & b) {
        if (a.sign != b.sign || a.num.size() != b.num.size()) {
            return false;
        }
        return !abs_cmp(a, b);
    }

    bool operator!=(const int2048 & a, const int2048 & b) {
        return !(a == b);
    }

    bool operator>=(const int2048 & a, const int2048 & b) {
        if (a.sign == b.sign) {
            int sign0 = abs_cmp(a, b);
            if (a.sign * sign0 == 1 || a.sign * sign0 == 0) {
                return true;
            } else {
                return false;
            }
        } else {
            if (a.sign == 1) {
                return true;
            } else {
                return false;
            }
        }
    }

    bool operator>(const int2048 & a, const int2048 & b) {
        if (a.sign == b.sign) {
            int sign0 = abs_cmp(a, b);
            if (a.sign * sign0 == 1) {
                return true;
            } else {
                return false;
            }
        } else {
            if (a.sign == 1) {
                return true;
            } else {
                return false;
            }
        }
    }

    bool operator<=(const int2048 & a, const int2048 & b) {
        return !(a > b);
    }

    bool operator<(const int2048 & a, const int2048 & b) {
        return !(a >= b);
    }

    int abs_cmp(const int2048 & a, const int2048 & b) {
        int len = a.num.size(), len0 = b.num.size();
        int sign0 = 0;
        if (len > len0) {
            sign0 = 1;
        } else if (len < len0) {
            sign0 = -1;
        } else {
            for (int i = len - 1; i >= 0; i--) {
                if (a.num[i] > b.num[i]) {
                    sign0 = 1;
                    break;
                }
                if (a.num[i] < b.num[i]) {
                    sign0 = -1;
                    break;
                }
            }
        }
        return sign0;
    }

    std::istream &operator>>(std::istream & in, int2048 & num0) {
        std::string num00;
        in >> num00;
        num0.read(num00);
        return in;
    }

    std::ostream &operator<<(std::ostream & out, const int2048 & num0) {
        if (num0.sign == -1) {
            out << '-';
        }
        int len = num0.num.size();
        long long a = num0.num[len - 1];
        if (a == 0 && len == 1) {
            out << 0;
            return out;
        }
        bool flag = false;
        for (int m = int2048::base / 10; m >= 1; m /= 10) {
            if (a / m != 0 || flag) {
                out << (a / m);
                flag = true;
            }
            a %= m;
        }
        for (int i = len - 2; i >= 0; i--) {
            a = num0.num[i];
            for (int m = int2048::base / 10; m >= 1; m /= 10) {
                out << (a / m);
                a %= m;
            }
        }
        return out;
    }

    int2048 &int2048::operator*=(const int2048 & num0) {
        if (num0 == 0) {
            *this = 0;
            return *this;
        }
        int len = num.size(), len0 = num0.num.size();
        long long progression_multiply = 0;
        int2048 result;
        for (int i = 0; i < len + len0; i++) {
            result.num.push_back(0);
        }
        long long tmp;
        for (int j = 0; j < len0; j++) {
            for (int i = 0; i < len; i++) {
                tmp = num[i] * num0.num[j] + progression_multiply;
                progression_multiply = 0;
                if (tmp >= base) {
                    progression_multiply = tmp / base;
                    tmp %= base;
                }
                result.num[i + j] += tmp;
                if (result.num[i + j] >= base) {
                    result.num[i + j + 1]++;
                    result.num[i + j] %= base;
                }
            }
            if (progression_multiply != 0) {
                result.num[len + j] += progression_multiply;
                progression_multiply = 0;
                if (result.num[len + j] >= base) {
                    result.num[len + j + 1]++;
                    result.num[len + j] %= base;
                }
            }
        }
        result.sign = sign * num0.sign;
        for (int i = result.num.size() - 1; i >= 1; i--) {
            if (result.num[i] == 0) {
                result.num.pop_back();
            } else {
                break;
            }
        }
        if (result.num.size() == 1 && result.num[0] == 0) {
            sign = 1;
        }
        *this = result;
        return *this;
    }

    int2048 operator*(int2048 a, const int2048 & b) {
        return (a *= b);
    }

    int2048 &int2048::operator/=(const int2048 & num0) {
        if (*this == 0) {
            *this = 0;
            return *this;
        }
        int len = num.size(), len0 = num0.num.size(), sign0 = abs_cmp(*this, num0);
        int2048 result;
        if (sign0 == -1) {
            if (sign != num0.sign) {
                result.sign = -1;
                result.num[0] = 1;
            }
            *this = result;
            return *this;
        }
        if (sign0 == 0) {
            result.num[0] = 1;
            result.sign = sign * num0.sign;
            *this = result;
            return *this;
        }
        int2048 num00(num0), this0(*this);
        sign = 1;
        num00.sign = 1;
        for (int i = 1; i <= len - len0; i++) {
            result.num.push_back(0);
        }
        double div0 = num00.get(len0 - 2) + (num00.get(len0 - 3) + 1.0) / base;
        double div = 1.0 / (num00.num.back() + div0 / base);
        long long dividend, quotient;
        for (int i = len - 1, j = result.num.size() - 1; j >= 0 && j <= this0.num.size();) {
            dividend = this->get(i + 1) * base + this->get(i);
            quotient = (long long)(dividend * div);
            int2048 a = (int2048(quotient) * num00).offset(j * digit);
            *this -= (int2048(quotient) * num00).offset(j * digit);
            result.num[j] += quotient;
            if (this->get(i + 1) == 0) {
                i--;
                j--;
            }
        }
        long long progression = 0;
        while (abs_cmp(*this, num00) != -1) {
            *this -= num00;
            progression++;
        }
        for (int i = 0; i < result.num.size(); i++) {
            result.num[i] += progression;
            progression = 0;
            if (result.num[i] >= base) {
                progression = result.num[i] / base;
                result.num[i] %= base;
            }
        }
        for (int i = result.num.size() - 1; i >= 1; i--) {
            if (result.num[i] == 0) {
                result.num.pop_back();
            } else {
                break;
            }
        }
        *this = result;
        sign = this0.sign * num0.sign;
        if (sign == -1) {
            if (this0 != (*this) * num0) {
                *this -= 1;
            }
        }
        return *this;
    }

    int2048 operator/(int2048 a, const int2048 & b) {
        return (a /= b);
    }

    int2048 &int2048::operator%=(const int2048 & num0) {
        return (*this = *this % num0);
    }

    int2048 operator%(int2048 a, const int2048 & b) {
        return a - a / b * b;
    }

    int2048 &int2048::offset(const int &_offset) {
        int len = num.size();
        if (_offset < digit) {
            for (int i = len - 1; i >= 0; i--) {
                for (int j = _offset; j >= 1; j--) {
                    num[i] *= 10;
                }
                if (i == len - 1) {
                    if (num[i] >= base) {
                        num.push_back(num[i] / base);
                        num[i] %= base;
                    }
                } else {
                    num[i + 1] += num[i] / base;
                    num[i] %= base;
                }
            }
            return *this;
        }
        int offset0 = 0;
        while (offset0 < _offset + 1 - digit) {
            num.push_back(0);
            offset0 += digit;
        }
        int offset1 = _offset - offset0;
        for (int i = num.size() - 1; len > 0; len--, i--) {
            for (int j = offset1; j >= 1; j--) {
                num[len - 1] *= 10;
            }
            if (i == num.size() - 1) {
                if (num[len - 1] >= base) {
                    num.push_back(num[len - 1] / base);
                }
                num[i] += num[len - 1] % base;
                num[len - 1] = 0;
            } else {
                num[i + 1] += num[len - 1] / base;
                num[i] += num[len - 1] % base;
                num[len - 1] = 0;
            }
        }
        return *this;
    }

    long long int2048::get(const int &index) {
        if (index >= num.size()) {
            return 0;
        }
        return num[index];
    }

} // namespace sjtu
