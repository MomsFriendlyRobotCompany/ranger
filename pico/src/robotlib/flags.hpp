
#pragma once

/*
This is a flag that can be set to indicate something,
but when it is read, resets a true value to false. Thus you
don't have to manually clear the flag.
*/
class BoolFlag {
  bool value{false}; // volatile?

public:
  BoolFlag() {}
  ~BoolFlag() {}

  operator bool() {
    if (value) {
      value = false;
      return true;
    }
    return false;
  }

  inline void set() { value = true; }
};

/*
Another flag, but uses bits instead of a bool, so 32 flags
can be tracked with just one instance. You can only check
this ONCE and then the bit gets cleared using is_set().

Alternatively, you can use check to to see if a flag multiple
times safely. However, you must now use clear() or is_set()
to clear the flag.
*/
class BitFlag {
public:
  BitFlag() {}

  inline void operator+=(const uint32_t v) { value |= v; }  // set
  inline void operator-=(const uint32_t v) { value &= ~v; } // clear
  inline void operator^=(const uint32_t v) { value ^= ~v; } // toggle

  // do I want to clear a true bit on read?
  // - will I check multiple times?
  // - how will I know if I didn't clear somewhere else?
  // bool is_set(const uint32_t v) { return (bool)(value & v); }
  bool is_set(const uint32_t v) {
    // WARN: only check 1 bit at a time, it doesn't know the difference
    // between b001 and b101 ... both are true, but one flag is missing.
    // if (value & v) {
      // value &= ~v; // clear
    if (check(v)) {
      clear(v);
      return true;
    }
    return false;
  }

  inline
  bool check(const uint32_t v) { return value & v; } // check but don't clear

  inline
  void clear(const uint32_t v) { value &= ~v; }

protected:
  uint32_t value{0};
};

// constexpr uint16_t SET_BITS(const uint16_t val, const uint16_t mask) { return
// val | mask; } constexpr uint16_t CLEAR_BITS(const uint16_t val, const
// uint16_t mask) { return val & ~mask; }