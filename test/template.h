#include <Arduino.h>

template<typename T, size_t N>
struct NormalGroup {
    T data[N];
    constexpr size_t size() const { return N; }
    T& operator[](size_t i) { return data[i]; }
     // 반복문을 위한 begin/end (포인터 반환)
    T* begin() { return &data[0]; }
    T* end() { return &data[N]; }
    const T* begin() const { return &data[0]; }
    const T* end() const { return &data[N]; }
};

class Button {
public:
    template<size_t N>
    using Group = NormalGroup<Button, N>;

};

