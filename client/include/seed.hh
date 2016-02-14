
#include <cstdint>
#include <string>

namespace blocks {
  template <uint16_t SIZE> class seed_block {
  public:
    static seed_block<SIZE> from_file(const std::string &path);
    bool save(const std::string &path);

    uint16_t size() { return SIZE; }

    uint8_t get_8(uint16_t idx) const {
      return _data[idx % SIZE];
    }

    uint16_t get_16(uint16_t idx) const {
      return get_8(idx) << 8 + get_8(idx + 1);
    }

    uint32_t get_32(uint16_t idx) const {
      return get_16(idx) << 16 + get_16(idx + 2);
    }

    uint64_t get_64(uint16_t idx) const {
      return get_32(idx) << 32 + get_32(idx + 4);
    }
  protected:
    uint8_t _data[SIZE];
  };

  typedef seed_block<64> seed;
}
