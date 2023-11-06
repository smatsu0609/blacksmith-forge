#include "LargeDecimal.h"

LargeDecimal::LargeDecimal(uint64_t val) {
  do {
    this->digits_.push_back(val % 10);
    val = val / 10;
  } while(val);
}

LargeDecimal::LargeDecimal(uint64_t hi, uint64_t lo) {
  char buf[32 + 1];
  sprintf(buf, "%016lx%016lx", hi, lo);
  this->SetHex(buf);
}

bool LargeDecimal::SetDec(const char* dec_str) {
  std::string tmp(dec_str);
  std::string str(tmp.rbegin(), tmp.rend());  // reversed string for the input string
  for (auto c : str) {
    if (c == ',' || c == '_') {
      continue;
    } else if (std::isdigit(c)) {
      this->digits_.push_back(c - '0');
    } else {
      return false;
    }
  }
  return true;
}

bool LargeDecimal::SetHex(const char* hex_str) {
  std::string tmp(hex_str);
  std::string str(tmp.rbegin(), tmp.rend());  // reversed string for the input string
  LargeDecimal base(1);

  for (auto c : str) {
    if (c == ',' || c == '_') {
      continue;
    } else if (std::isxdigit(c)) {
      char nibble;
      if (std::isdigit(c)) {
        nibble = c - '0';
      } else {
        nibble = std::toupper(c) - 'A' + 10;
      }
      *this = *this + base.Mult(nibble);
      base = base.Mult(16);
    } else {
      return false;
    }
  }
  return true;
}

LargeDecimal LargeDecimal::Add(const LargeDecimal& x, const LargeDecimal& y)
{
  LargeDecimal result;
  uint8_t d, roll;
  int8_t a, b;
  roll = 0;
  for (std::size_t i = 0; i < std::max<std::size_t>(x.digits_.size(), y.digits_.size()); i++) {
    a = (i < x.digits_.size())? x.digits_[i]: 0;
    b = (i < y.digits_.size())? y.digits_[i]: 0;
    d = a + b + roll;
    result.digits_.push_back(d % 10);
    roll = d / 10;
  }
  while (roll) {
    result.digits_.push_back(roll % 10);
    roll /= 10;
  }
  return result;
}

LargeDecimal LargeDecimal::Sub(const LargeDecimal& x, const LargeDecimal& y)
{
  LargeDecimal result;
  bool sign = (LargeDecimal::Compare(x, y) < 0);
  uint8_t d, borrow;
  int8_t a, b, tmp;

  borrow = 0;
  for (std::size_t i = 0; i < std::max<std::size_t>(x.digits_.size(), y.digits_.size()); i++) {
    a = (i < x.digits_.size())? x.digits_[i]: 0;
    b = (i < y.digits_.size())? y.digits_[i]: 0;
    if (sign) {
      // swap a, b
      tmp = a; a = b; b = tmp;
    }
    if (a < b + borrow) {
      d = a + 10 - b - borrow;
      borrow = 1;
    } else {
      d = a - b - borrow;
      borrow = 0;
    }
    result.digits_.push_back(d);
  }

  while (result.digits_.back() == 0 && result.digits_.size() > 1) {
    result.digits_.pop_back();
  }

  result.sign_ = sign;

  return result;
}

int LargeDecimal::Compare(const LargeDecimal& x, const LargeDecimal& y) {
  int ret = 0;
  if (x.digits_.size() == y.digits_.size()) {
    for (std::size_t i = 0; i < x.digits_.size(); i++) {
      if (x.digits_[i] == y.digits_[i]) {
        continue;
      }
      ret = (x.digits_[i] > y.digits_[i]) ? 1: -1;
      break;
    }
  } else {
    ret = (x.digits_.size() > y.digits_.size())? 1: -1;
  }
  return ret;
}

LargeDecimal LargeDecimal::Mult(int8_t n) {
  uint8_t roll, d;
  LargeDecimal result;

  roll = 0;
  for (auto digit : this->digits_) {
    d = digit * n + roll;
    result.digits_.push_back(d % 10);
    roll = d / 10;
  }
  while (roll) {
    result.digits_.push_back(roll % 10);
    roll /= 10;
  }
  return result;
}

LargeDecimal LargeDecimal::Mult(const LargeDecimal& x, const LargeDecimal& y) {
  LargeDecimal result;
  LargeDecimal base(x);

  for (auto d : y.digits_) {
    result = result + base.Mult(d);
    base = base.Mult(10);
  }
  result.sign_ = x.sign_ xor y.sign_;
  return result;
}

std::string LargeDecimal::GetString(void) {
  std::string result;
  for (auto d : this->digits_) {
    char c = d + '0';
    result = c + result;
  }
  if (this->sign_) {
    result = '-' + result;
  }
  return result;
}
