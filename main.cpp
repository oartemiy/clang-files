#include <__iterator/bounded_iter.h>
#include <_stdio.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <forward_list>
#include <functional>
#include <ios>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// 1024 bytes
class BigData
{
public:
    unsigned char* bytes = nullptr;

    BigData()
    {
        if (bytes == nullptr)
        {
            bytes = new unsigned char[1024]();
        }
    }

    ~BigData()
    {
        if (bytes != nullptr)
        {
            delete[] bytes;
            bytes = nullptr;
        }
    }

    BigData(const BigData& rhs)
    {
        if (bytes == nullptr)
        {
            bytes = new unsigned char[1024]();
            for (size_t i = 0; i < 1024; ++i)
            {
                bytes[i] = rhs.bytes[i];
            }
        }
    }

    BigData& operator=(const BigData& rhs)
    {
        if (bytes == nullptr)
        {
            bytes = new unsigned char[1024]();
            for (size_t i = 0; i < 1024; ++i)
            {
                bytes[i] = rhs.bytes[i];
            }
        }
        return *this;
    }

    BigData(BigData&& rhs) noexcept
    {
        if (this != &rhs)
        {
            bytes = rhs.bytes;
            rhs.bytes = nullptr;
        }
    }

    BigData& operator=(BigData&& rhs) noexcept
    {
        if (this != &rhs)
        {
            delete[] bytes;

            bytes = rhs.bytes;
            rhs.bytes = nullptr;
        }
        return *this;
    }
};

template<typename T>
class Vector
{
private:
    T* _data = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;

    void reallocation()
    {
        _capacity = _capacity * 2 + 1;
        T* newData = new T[_capacity]();
        for (size_t i = 0; i < _size; ++i)
        {
            newData[i] = _data[i];
        }
        delete[] _data;
        _data = newData;
    }

public:
    Vector(size_t size) : _size(size), _capacity(size)
    {
        _data = new T[size]();
    }

    Vector() = default;

    // 2 Конструктор копирования
    Vector(const Vector& other) : _size(other._size), _capacity(other._capacity)
    {
        delete[] _data;

        _data = new T[other._size];
        for (size_t i = 0; i < _size; ++i)
        {
            _data[i] = other._data[i];
        }
    }

    // 3 Конструктор перемещения
    Vector(Vector&& other) noexcept : _data(other._data), _size(other._size), _capacity(other._capacity)
    {
        other._size = 0;
        other._capacity = 0;
        other._data = nullptr;
    }

    // 1 Деструктор
    ~Vector()
    {
        delete[] _data;
    }

    // 4 Оператор копирования присваиванием
    Vector& operator=(const Vector& other)
    {
        delete[] _data;

        _data = new T[other._size];
        for (size_t i = 0; i < _size; ++i)
        {
            _data[i] = other._data[i];
        }
        return *this;
    }

    // 5 Оператор Конструктор перемещения
    Vector& operator=(Vector&& other) noexcept
    {
        if (this != &other)
        {
            delete[] _data;

            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;
            other._size = 0;
            other._capacity = 0;
            other._data = nullptr;
        }
        return *this;
    }

    [[nodiscard]] size_t size() const
    {
        return _size;
    }

    [[nodiscard]] size_t capacity() const
    {
        return _capacity;
    }

    // // все коментарии, если бы здесь было бы копирование

    void push_back(T&& rValue)
    {
        if (_size == _capacity)
        {
            reallocation();
        }
        new (_data + _size++) T(std::move(rValue));  // placement new
                                                     // Да, здесь вызывается конструктор T, что плохо
                                                     // для больших данных

        // data_[size_++] = value;
    }

    void push_back(const T& constRefValue)
    {
        if (_size == _capacity)
        {
            reallocation();
        }
        new (_data + _size++) T(constRefValue);
    }

    T& operator[](size_t index)
    {
        return _data[index];
    }
};

template<typename T>
class Matrix
{
private:
    T** _arr = nullptr;
    size_t _n = 0, _m = 0;

    void free_matrix()
    {
        if (_arr != nullptr)
        {
            std::cout << "Info in addres: " << static_cast<void*>(_arr) << ' ' << "was deleted" << std::endl;
            for (size_t i = 0; i < _n; ++i)
            {
                delete[] _arr[i];
            }
            delete[] _arr;
        }
    }

public:
    ~Matrix()
    {
        if (_arr != nullptr)
        {
            free_matrix();
        }
    }

    Matrix(const size_t N, const size_t M, T elem = 0) : _n{N}, _m{M}
    {
        _arr = new T*[N];
        std::cout << "created Info in addres: " << static_cast<void*>(_arr) << std::endl;
        for (size_t i = 0; i < N; ++i)
        {
            _arr[i] = new T[M];
            for (size_t j = 0; j < M; ++j)
            {
                new (*(_arr + i) + j) T(elem);
                // arr[i][j] = elem;
            }
        }
    }

    Matrix(const Matrix& rhs) : _n{rhs._n}, _m{rhs._m}
    {
        if (_arr != nullptr)
        {
            free_matrix();
        }
        _arr = new T*[_n];
        std::cout << "created Info in addres: " << static_cast<void*>(_arr) << std::endl;
        for (size_t i = 0; i < _n; ++i)
        {
            _arr[i] = new T[_m];
            for (size_t j = 0; j < _m; ++j)
            {
                new (*(_arr + i) + j) T(rhs[i][j]);
            }
        }
    }

    Matrix(Matrix&& rValue) noexcept : _n{rValue._n}, _m{rValue._m}
    {
        if (_arr != nullptr)
        {
            free_matrix();
        }
        _arr = rValue._arr;
        std::cout << "Info in addres " << static_cast<void*>(_arr) << " Was moved" << std::endl;
        rValue._n = 0;
        rValue._m = 0;
        rValue._arr = nullptr;
    }

    Matrix& operator=(const Matrix& rhs)
    {
        if (_arr != nullptr)
        {
            free_matrix();
        }
        _n = rhs._n;
        _m = rhs._m;
        _arr = new T*[_n];
        for (size_t i = 0; i < _n; ++i)
        {
            _arr[i] = new T[_m];
            for (size_t j = 0; j < _m; ++j)
            {
                new (*(_arr + i) + j) T(rhs[i][j]);
            }
        }
        return *this;
    }

    Matrix& operator=(Matrix&& rValue) noexcept
    {
        if (_arr != nullptr)
        {
            free_matrix();
        }
        _n = rValue._n;
        _m = rValue._m;
        _arr = rValue._arr;
        std::cout << "Info in addres " << static_cast<void*>(_arr) << " Was moved" << std::endl;
        rValue._n = 0;
        rValue._m = 0;
        rValue._arr = nullptr;
        return *this;
    }

    T*& operator[](size_t i)
    {
        return _arr[i];
    }

    const T* operator[](size_t i) const
    {
        return _arr[i];
    }

    // T* operator[](size_t i) const {return arr_[i]; }

    bool is_alive()
    {
        return _arr != nullptr;
    }

    std::pair<size_t, size_t> shape()
    {
        return {_n, _m};
    }
};

class CTextBlock
{
private:
    char* _pText;

public:
    // char &operator[](size_t pos)
    // {
    //     return pText[pos];
    // }

    const char& operator[](std::size_t pos) const
    {
        return _pText[pos];
    }

    char& operator[](std::size_t pos)
    {
        return const_cast<char&>(const_cast<const CTextBlock&>(*this)[pos]);
    }

    ~CTextBlock()
    {
        delete[] _pText;
    }

    CTextBlock(const CTextBlock& rhs)
    {
        size_t size = std::strlen(rhs._pText);
        _pText = new char[size + 1]();
        for (size_t i = 0; i < size; ++i)
        {
            new (_pText + i) char(rhs._pText[i]);
        }
        _pText[size] = '\n';
    }

    CTextBlock(CTextBlock&& rhs) noexcept
    {
        _pText = rhs._pText;
        rhs._pText = nullptr;
    }

    CTextBlock& operator=(const CTextBlock& rhs)
    {
        size_t size = std::strlen(rhs._pText);
        _pText = new char[size + 1]();
        for (size_t i = 0; i < size; ++i)
        {
            new (_pText + i) char(rhs._pText[i]);
            // pText[i] = rhs.pText[i];
        }
        _pText[size] = '\n';
        return *this;
    }

    CTextBlock& operator=(CTextBlock&& rhs) noexcept
    {
        _pText = rhs._pText;
        rhs._pText = nullptr;
        return *this;
    }

    CTextBlock(const char* rhs)
    {
        size_t size = std::strlen(rhs);
        _pText = new char[size + 1]();
        std::copy(rhs, rhs + size, _pText);
        // for (size_t i = 0; i < size; ++i)
        // {
        //     pText[i] = rhs[i];
        // }
        _pText[size] = '\n';
    }
};

int main()
{
    int arr[10];

    new (arr) int(1);

    new (arr + 1) int(45);

    std::cout << arr[1] << std::endl;

    const CTextBlock CCTB("Hello");

    const char* pc = &CCTB[0];

    // *pc = 'j';

    for (size_t i = 0; i < 5; ++i)
    {
        std::cout << CCTB[i] << ' ';
    }

    std::cout << std::endl;
    // std::string str = "asaidsfijdfsjdsfoijjiofijohfgjhkgfdjkls";
    // std::vector<std::string> vec;
    // unsigned char* ptr = reinterpret_cast<unsigned char*>(&str);
    // for (size_t i = 0; i < 24; ++i) {
    //     std::cout << std::hex << static_cast<int>(*(ptr + i)) << ' ';
    // }
    // std::cout << std::endl;
    // vec.push_back(std::move(str));
    // std::cout << vec[0] << std::endl;
    // std::cout << str << std::endl;
    // for (size_t i = 0; i < 24; ++i) {
    //     std::cout << std::hex << static_cast<int>(*(ptr + i)) << ' ';
    // }
    // std::cout << std::endl;

    // std::cout << sizeof(std::string) << std::endl;
    // unsigned char buffer[sizeof(std::string)];

    // new (buffer) std::string("ABCDEFGIABCDEFABCDEFABCEDE");

    // for (size_t i = 0; i < 24; ++i) {
    //     std::cout << std::hex << static_cast<int>(buffer[i]) << ' ';
    // }
    // unsigned long long addres = 0;
    // for (size_t i = 0; i < 8; ++i) {
    //     addres |= static_cast<unsigned long long>(buffer[i]) << (i * 8);
    // }
    // std::cout << std::endl;
    // std::cout << addres << std::endl;
    // char* ptr = reinterpret_cast<char*>(addres);
    // std::cout << (void*)ptr << ' ' << ptr << std::endl;

    // int buffer[10] = {0};

    // std::fill(reinterpret_cast<unsigned char*>(buffer),
    // reinterpret_cast<unsigned char*>(buffer + 1), 0x01);

    // std::cout << std::hex << buffer[0] << std::endl;

    // int arr[10]{1, 5, 3, 4, 5, 6, 2};
    // for (size_t i = 0; i < 10; ++i) {
    //     std::cout << arr[i] <<  ' ';
    // }
    // std::cout << std::endl;
    // int some_param = 3;
    // std::sort(arr, arr + 10, [some_param](auto lhs, auto rhs) { return lhs +
    // some_param > rhs; }); for (size_t i = 0; i < 10; ++i) {
    //     std::cout << arr[i] <<  ' ';
    // }
    // std::cout << std::endl;

    // Matrix<int> mat{3, 3, 4};
    // mat[0][0] = 2;
    // std::cout << mat[0][0] << std::endl;

    // Matrix<int> mat_to_move = Matrix<int>{2, 2, 10};
    // mat = mat_to_move;
    // std::cout << mat[0][0] << std::endl;

    // Vector<BigData> vector;
    // BigData bg1 = BigData{};
    // bg1.bytes[0] = 1;
    // bg1.bytes[1] = 2;
    // std::cout << static_cast<void*>(bg1.bytes) << std::endl;
    // vector.push_back(std::move(bg1));
    // std::cout << static_cast<void*>(vector[0].bytes) << std::endl;
    // // std::cout << vector[0].bytes << std::endl;
    // BigData bg2 = BigData{};
    // bg2.bytes[0] = 3;
    // bg2.bytes[1] = 4;
    // vector.push_back(bg2);
    // // std::cout << vector[1].bytes << std::endl;
}

# .clang-tidy
---
Checks: '-*,
bugprone-argument-comment,
bugprone-assert-side-effect,
bugprone-bad-signal-to-kill-thread,
bugprone-branch-clone,
bugprone-copy-constructor-init,
bugprone-dangling-handle,
bugprone-dynamic-static-initializers,
bugprone-fold-init-type,
bugprone-forward-declaration-namespace,
bugprone-forwarding-reference-overload,
bugprone-inaccurate-erase,
bugprone-incorrect-roundings,
bugprone-integer-division,
bugprone-lambda-function-name,
bugprone-macro-parentheses,
bugprone-macro-repeated-side-effects,
bugprone-misplaced-operator-in-strlen-in-alloc,
bugprone-misplaced-pointer-arithmetic-in-alloc,
bugprone-misplaced-widening-cast,
bugprone-move-forwarding-reference,
bugprone-multiple-statement-macro,
bugprone-no-escape,
bugprone-parent-virtual-call,
bugprone-posix-return,
bugprone-reserved-identifier,
bugprone-sizeof-container,
bugprone-sizeof-expression,
bugprone-spuriously-wake-up-functions,
bugprone-string-constructor,
bugprone-string-integer-assignment,
bugprone-string-literal-with-embedded-nul,
bugprone-suspicious-enum-usage,
bugprone-suspicious-include,
bugprone-suspicious-memset-usage,
bugprone-suspicious-missing-comma,
bugprone-suspicious-semicolon,
bugprone-suspicious-string-compare,
bugprone-suspicious-memory-comparison,
bugprone-suspicious-realloc-usage,
bugprone-swapped-arguments,
bugprone-terminating-continue,
bugprone-throw-keyword-missing,
bugprone-too-small-loop-variable,
bugprone-undefined-memory-manipulation,
bugprone-undelegated-constructor,
bugprone-unhandled-self-assignment,
bugprone-unused-raii,
bugprone-unused-return-value,
bugprone-use-after-move,
bugprone-virtual-near-miss,
cert-dcl21-cpp,
cert-dcl58-cpp,
cert-err34-c,
cert-err52-cpp,
cert-err60-cpp,
cert-flp30-c,
cert-msc50-cpp,
cert-msc51-cpp,
cert-str34-c,
cppcoreguidelines-interfaces-global-init,
cppcoreguidelines-narrowing-conversions,
cppcoreguidelines-pro-type-member-init,
cppcoreguidelines-pro-type-static-cast-downcast,
cppcoreguidelines-slicing,
google-default-arguments,
google-runtime-operator,
hicpp-exception-baseclass,
hicpp-multiway-paths-covered,
misc-misplaced-const,
misc-new-delete-overloads,
misc-non-copyable-objects,
misc-throw-by-value-catch-by-reference,
misc-unconventional-assign-operator,
misc-uniqueptr-reset-release,
modernize-avoid-bind,
modernize-concat-nested-namespaces,
modernize-deprecated-headers,
modernize-deprecated-ios-base-aliases,
modernize-loop-convert,
modernize-make-shared,
modernize-make-unique,
modernize-pass-by-value,
modernize-raw-string-literal,
modernize-redundant-void-arg,
modernize-replace-auto-ptr,
modernize-replace-disallow-copy-and-assign-macro,
modernize-replace-random-shuffle,
modernize-return-braced-init-list,
modernize-shrink-to-fit,
modernize-unary-static-assert,
modernize-use-auto,
modernize-use-bool-literals,
modernize-use-emplace,
modernize-use-equals-default,
modernize-use-equals-delete,
modernize-use-nodiscard,
modernize-use-noexcept,
modernize-use-nullptr,
modernize-use-override,
modernize-use-transparent-functors,
modernize-use-uncaught-exceptions,
mpi-buffer-deref,
mpi-type-mismatch,
openmp-use-default-none,
performance-faster-string-find,
performance-for-range-copy,
performance-implicit-conversion-in-loop,
performance-inefficient-algorithm,
performance-inefficient-string-concatenation,
performance-inefficient-vector-operation,
performance-move-const-arg,
performance-move-constructor-init,
performance-no-automatic-move,
performance-noexcept-move-constructor,
performance-trivially-destructible,
performance-type-promotion-in-math-fn,
performance-unnecessary-copy-initialization,
performance-unnecessary-value-param,
portability-simd-intrinsics,
readability-avoid-const-params-in-decls,
readability-const-return-type,
readability-container-size-empty,
readability-convert-member-functions-to-static,
readability-delete-null-pointer,
readability-deleted-default,
readability-inconsistent-declaration-parameter-name,
readability-make-member-function-const,
readability-misleading-indentation,
readability-misplaced-array-index,
readability-non-const-parameter,
readability-redundant-control-flow,
readability-redundant-declaration,
readability-redundant-function-ptr-dereference,
readability-redundant-smartptr-get,
readability-redundant-string-cstr,
readability-redundant-string-init,
readability-simplify-subscript-expr,
readability-static-accessed-through-instance,
readability-static-definition-in-anonymous-namespace,
readability-string-compare,
readability-uniqueptr-delete-release,
readability-use-anyofallof,
readability-identifier-naming'
CheckOptions:
  - key: readability-identifier-naming.VariableCase
    value: camelBack
  - key: readability-identifier-naming.VariablePrefix
    value: ''
  - key: readability-identifier-naming.VariableSuffix
    value: ''

  - key: readability-identifier-naming.LocalVariableCase
    value: camelBack
  
  - key: readability-identifier-naming.GlobalVariableCase
    value: lower_case

  - key: readability-identifier-naming.ConstantCase
    value: UPPER_CASE
  - key: readability-identifier-naming.StaticConstantCase
    value: UPPER_CASE
  - key: readability-identifier-naming.EnumConstantCase
    value: UPPER_CASE

  - key: readability-identifier-naming.ParameterCase
    value: camelBack

  - key: readability-identifier-naming.PrivateMemberCase
    value: camelBack
  - key: readability-identifier-naming.PrivateMemberPrefix
    value: _

  - key: readability-identifier-naming.ProtectedMemberCase
    value: camelBack
  - key: readability-identifier-naming.ProtectedMemberPrefix
    value: _

  - key: readability-identifier-naming.FunctionCase
    value: camelCase
  - key: readability-identifier-naming.MethodCase
    value: camelCase

  - key: readability-identifier-naming.ClassCase
    value: CamelCase
  - key: readability-identifier-naming.StructCase
    value: CamelCase
  - key: readability-identifier-naming.EnumCase
    value: CamelCase

  - key: readability-identifier-naming.TemplateParameterCase
    value: PascalCase
  - key: readability-identifier-naming.TemplateParameterPrefix
    value: "T"

  - key: readability-identifier-naming.HungarianNotation
    value: false
