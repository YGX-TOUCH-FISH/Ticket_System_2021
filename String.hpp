//
// Created by 18163 on 2021/4/24.
//

#ifndef BPT_STRING_HPP
#define BPT_STRING_HPP
#include <cstring>
#include <iostream>
#include "exception.hpp"
namespace RA {
    template<int N = 50>
    class String {
        char str[N+1]{};
    public:
        int hash_value = 0;
    public:
        String() { str[0] = '\0'; }
        explicit String(const char *ec) {
            if (ec) {
                int i = 0;
                for ( ; i < N && ec[i] != '\0' ; ++i) str[i] = ec[i];
                str[i] = '\0';
            }
            else {
                str[0] = '\0';
            }
            size_t len = strlen(ec);
            hash_value = 0;
            for (int i = 0; i < len; ++i) {
                hash_value = (1ll * hash_value * 13331 + str[i]) % 998244353 ;
            }
            hash_value = (hash_value + 998244353) % 998244353 ;
        }
        explicit String(const std::string &s){
            int i = 0;
            for ( ; i < N && s[i] != '\0' ; ++i) str[i] = s[i];
            str[i] = '\0';
            size_t len = s.length();
            hash_value = 0;
            for (i = 0; i < len; ++i) {
                hash_value = (1ll * hash_value * 13331 + str[i]) % 998244353 ;
            }
            hash_value = (hash_value + 998244353) % 998244353 ;
        }
        String (const String &ec) {
            int i = 0;
            for ( ; i < N && ec.str[i] != '\0' ; ++i) str[i] = ec.str[i];
            str[i] = '\0';
            size_t len = strlen(ec.str);
            hash_value = 0;
            for (i = 0; i < len; ++i) {
                hash_value = (1ll * hash_value * 13331 + str[i]) % 998244353 ;
            }
            hash_value = (hash_value + 998244353) % 998244353 ;
        }
        String &operator=(const String &ec) {
            if (this == &ec) return *this;
            int i = 0;
            for ( ; i < N && ec.str[i] != '\0' ; ++i) str[i] = ec.str[i];
            str[i] = '\0';
            hash_value = ec.hash_value;
            return *this;
        }
        String &operator=(const char *ec) {
            if (!ec) throw invalid_visit();
            int i = 0;
            for ( ; i < N && ec[i] != '\0' ; ++i) str[i] = ec[i];
            str[i] = '\0';
            size_t len = strlen(ec);
            hash_value = 0;
            for (i = 0; i < len; ++i) {
                hash_value = (1ll * hash_value * 13331 + str[i]) % 998244353 ;
            }
            hash_value = (hash_value + 998244353) % 998244353 ;
            return *this;
        }
        /*  compare operator                          */
        bool operator==(const String &ec) const {
            return hash_value == ec.hash_value;
        }
        bool operator <(const String &ec) const{
            if (!ec.str) throw invalid_visit();
            size_t l1 = strlen(str);
            size_t l2 = strlen(ec.str);
            size_t l = (l1 < l2)? l1 : l2;
            for (int i = 0 ; i < l ; ++i)
                if (str[i] != ec.str[i])
                    return str[i] < ec.str[i];
            return (l1 < l2);
        }
        bool operator >(const String &ec) const {
            return ec < (*this);
        }
        bool operator >=(const String &ec) const {
            return ((*this) > ec || (*this) == ec );
        }
        bool operator <=(const String &ec) const {
            return ((*this) < ec || (*this) == ec );
        }
        bool operator !=(const String &ec) const {
            return !((*this) == ec);
        }
        bool operator ==(const char *ec) const {
            size_t l1 = strlen(str);
            size_t l2 = strlen(ec);
            if (l1 != l2) return false;
            for (int i = 0 ; i < l1 ; ++i)
                if (str[i] != ec[i])
                    return false;
            return true;
        }
        char &operator[](size_t index) {
            if (index > N) throw index_out_of_bound();
            return str[index];
        }
        const char &operator[](size_t index) const {
            if (index > N) throw index_out_of_bound();
            return str[index];
        }

        friend std::istream &operator >>(std::istream &is, String &s) {
            is >> s.str;
            return is;
        }
        friend std::ostream &operator <<(std::ostream &os, const String &s) {
            os << s.str;
            return os;
        }
    };
}
#endif //BPT_STRING_HPP
