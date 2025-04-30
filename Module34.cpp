#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
struct HasValidHash {
private:
    template <typename U>
    static auto test(int) -> decltype(
        std::declval<U>().hash(),
        std::is_convertible<decltype(std::declval<U>().hash()), long>(),
        std::true_type()
    );
    
    template <typename>
    static std::false_type test(...);
    
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

template <typename T>
struct HasValidToString {
private:
    template <typename U>
    static auto test(int) -> decltype(
        std::declval<U>().toString(),
        std::is_same<decltype(std::declval<U>().toString()), std::string>(),
        std::true_type()
    );
    
    template <typename>
    static std::false_type test(...);
    
public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

template <typename T>
constexpr bool has_no_virtual_destructor = !std::has_virtual_destructor<T>::value;

template <typename T>
constexpr bool ComplexConcept = 
    HasValidHash<T>::value && 
    HasValidToString<T>::value && 
    has_no_virtual_destructor<T>;

class ValidType {
public:
    long hash() const { return 42; }
    std::string toString() const { return "ValidType"; }
    ~ValidType() = default;
};

template <typename T>
typename std::enable_if<ComplexConcept<T>>::type
process(const T& obj) {
    std::cout << "Processing: " << obj.toString() 
              << ", hash: " << obj.hash() << std::endl;
}

int main() {
    ValidType valid;
    process(valid);  
    
    return 0;
}