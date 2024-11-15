#include <string_view>

namespace OrderbookCore {
inline constexpr auto hash_strlit(const std::string_view sv) {
    unsigned long hash{ 5381 };
    for (unsigned char c : sv) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}

inline constexpr auto operator ""_hash (const char* str, size_t len) {
  return hash_strlit(std::string_view{ str, len });
}
}
