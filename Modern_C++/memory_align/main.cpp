#include <iostream>
#include <bitset>

bool isPowerOfTwo(int n) {
    if (n <= 0) {
        return false;
    }
    return (n & (n - 1)) == 0;
}

static inline int aligned(int v, int byteAlign) {
    return (v + byteAlign - 1) & ~(byteAlign - 1);
}

int main() {
    int byteAlign = 1 << 3;

    try {
        if (!isPowerOfTwo(byteAlign)) {
            throw std::runtime_error("byteAlign is not power Of 2");
        }
    } catch (const std::exception& e) {
        puts(e.what());
        throw std::runtime_error("error");
    }

    std::cout << "byteAlign: " << std::bitset<sizeof(int) * 8>(byteAlign).to_string() << std::endl;
    std::cout << "byteAlign - 1: " << std::bitset<sizeof(int) * 8>(byteAlign - 1).to_string() << std::endl;
    std::cout << "~(byteAlign - 1): " << std::bitset<sizeof(int) * 8>(~(byteAlign - 1)).to_string() << std::endl;

    for (int i = 0; i < 10; ++i) {
        auto r = aligned(i, byteAlign);
        std::cout << i << " align to " << byteAlign << ": " << r << std::endl;
    }

    return 0;
}
