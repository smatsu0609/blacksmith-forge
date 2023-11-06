#pragma once

#include <cstdint>
#include <vector>
#include <string>

class LargeDecimal {
 protected:
  bool sign_ = false;
  std::vector<uint8_t> digits_; 
 public:
  // constructor
  LargeDecimal() { }
  LargeDecimal(const LargeDecimal& obj) {this->digits_ = obj.digits_; this->sign_ = obj.sign_;}
  LargeDecimal(uint64_t val);
  LargeDecimal(uint64_t hi, uint64_t lo);
  LargeDecimal(uint64_t *p_data, int num);
  virtual ~LargeDecimal() { }
  LargeDecimal& operator = (const LargeDecimal& x) {this->digits_ = x.digits_; this->sign_ = x.sign_; return *this;}
  LargeDecimal operator + (const LargeDecimal& x) {return Add(*this, x);}
  LargeDecimal operator - (const LargeDecimal& x) {return Sub(*this, x);}
  LargeDecimal operator * (const LargeDecimal& x) {return Mult(*this, x);}
//  bool operator > (const LargeDecimal& x) {return Compare(*this, x) > 0;}
//  bool operator >= (const LargeDecimal& x) {return Compare(*this, x) >= 0;}
//  bool operator < (const LargeDecimal& x) {return Compare(*this, x) < 0;}
//  bool operator <= (const LargeDecimal& x) {return Compare(*this, x) <= 0;}
  static LargeDecimal Add(const LargeDecimal& x, const LargeDecimal& y);
  static LargeDecimal Sub(const LargeDecimal& x, const LargeDecimal& y);
  static LargeDecimal Mult(const LargeDecimal& x, const LargeDecimal& y);
  LargeDecimal Mult(int8_t n);
  static int Compare(const LargeDecimal& x, const LargeDecimal& y);

  bool SetHex(const char* hex_str);
  bool SetDec(const char* dec_str);
  std::string GetString(void);
};

