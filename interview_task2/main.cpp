#include <iostream>

/**
*
* Please write a class that let user return from function either value or error code (Status). This means that class either contain value or error.
* The purpose is basically for abandoning patter from C that when error must be returned from
* function, value is most likely returned via setting argument.
*
* Please do not use any stl containers for implementation.
* Language - C++14
*
* Basic version requirements:
* - API required:
* - must be possible to check if error is present (provide nice impl for if usage)
* - creating from Status must indicate error
* - creating from value must indicate success
* - must have value getter
* - must have error getter
*
* - value type can be default constructible
*
* Extended version requirements (if You want to implement):
* - value type does not need to be default constructible (most important feature)
* - constructors shall be compile-time disabled when they do not match
* - require from enum Status to have member OK with value 0
*
* Given:
* enum class Status {
* OK,
* INVALID_ARG,
* ERROR
* };
*
*
* Example usage:
*
* class UserClass {
*
* };
*
* YourClassImpl someFunction() {
*
* if(wrongCondition()) {
* return Status::ERROR;
* }
*
* return UserClass{};
* }
*/

enum class Status {
    OK,
    INVALID_ARG,
    ERROR
};

/**
 * A helper class to represent storage intended to hold either status or value.
 */
template <typename T>
union VariantStorage {
    Status status;
    T value;
    VariantStorage() { /* do nothing on purpose */ }
    ~VariantStorage() { /* do nothing on purpose */ }
};

/**
 * A marker class to enable 'argument forwarding' to cater cases when an instance of Status
 * should be considered as value rather than status.
 */
class Forward{};

/**
 * A class to represent a so-called variant (tribute to C++17) capable of holding status or value of a given type.
 *
 * @note The class supports value semantics
 */
template <typename T>
class Variant {
public:
    static_assert(Status::OK == static_cast<Status>(0), "Status::OK != 0");

    /*
     * No default ctor - it does not make much sense as either status or value has to be provided
     * and value does not have to be default constructible.
     */

    /**
     * Construct a variant instance from status code making it valueless.
     * Implicit conversions are allowed for convenience.
     *
     * @param status Instance of Status
     */
    Variant(Status status): valueless(true) {
        storage.status = status;
    }

    /**
     * Construct a variant instance from value.
     * Implicit conversions are allowed for convenience.
     *
     * @note The ctor works only for types that are not convertible to Status.
     * If one wants to create a variant with Status represented as value the 'forwarding' ctor has to be used.
     *
     * @param val Instance of given value type
     */
    template <typename U = T, std::enable_if_t<!std::is_convertible<U, Status>::value &&
                              std::is_convertible<U, T>::value>* = nullptr>
    Variant(U val) : valueless(false) {
        new (&storage.value) T{std::move(val)};
    }

    /**
     * Construct a variant instance from value.
     * This overload makes it possible to create a variant with Status instance considered as value rather than status.
     *
     * @param val Instance of given value type.
     */
    Variant(Forward, T val): valueless(false) {
        new (&storage.value) T{std::move(val)};
    }

    /**
     * Copy construct a variant instance.
     * @param other Instance of Variant
     */
    Variant(const Variant& other): valueless(other.valueless) {
        if (carriesValue()) {
            new (&storage.value) T{other.storage.value};
        } else {
            storage.status = other.storage.status;
        }
    }

    /**
     * Move construct a variant instance.
     * The moved-from object has a valid but unspecified state.
     * @param other Instance of Variant
     */
    Variant(Variant&& other) noexcept: valueless(other.valueless) {
        if (carriesValue()) {
            new (&storage.value) T{std::move(other.storage.value)};
        } else{
            storage.status = other.storage.status;
        }
    }

    /**
     * Destruct a variant instance.
     */
    ~Variant() {
        if (carriesValue()) {
            storage.value.~T();
        }
        /* no dtor needed for status of integral type */
    }

    /**
     * Replace contents of *this with contents of other
     * @param other Instance of Variant
     * @return *this
     */
    Variant& operator=(const Variant& other) & {
        if (this != &other) {
            valueless = other.valueless;
            if (carriesValue()) {
	        storage.value.~T();
                new(&storage.value) T{other.storage.value};
            } else{
                storage.status = other.storage.status;
            }
        }
        return *this;
    }

    /**
     * Replace contents of *this with contents of temporary other
     * @param other Instance of Variant
     * @return *this
     */
    Variant& operator=(Variant&& other) & noexcept {
        if (this != &other) {
            valueless = other.valueless;
            if (carriesValue()) {
		storage.value.~T();
                new(&storage.value) T{std::move(other.storage.value)};
            } else {
                storage.status = other.storage.status;
            }
        }
        return *this;
    }

    /**
     * Test if *this carries value.
     *
     * @return False if valueless, true otherwise
     */
    bool carriesValue() const noexcept {
        return !valueless;
    }

    explicit operator bool() const noexcept {
        return carriesValue();
    }

    /**
     * Obtain reference to value from const *this.
     * The method throws in case *this is valueless.
     * @return Reference to T
     */
    const T& getValue() const & {
        if (carriesValue()) {
            return storage.value;
        }
        throw std::invalid_argument{"optional is valueless"};
    }

    /**
     * Obtain reference to value from *this.
     * The method throws in case *this is valueless.
     * @return Reference to T
     */
    T& getValue() & {
        if (carriesValue()) {
            return storage.value;
        }
        throw std::invalid_argument{"optional is valueless"};
    }

    /**
     * Obtain value from *this.
     * The method throws in case *this is valueless.
     * @return Reference to T
     */
    T getValue() const && {
        if (carriesValue()) {
            return std::move(storage.value);
        }
        throw std::invalid_argument{"optional is valueless"};
    }

    /**
     * Obtain status code from *this.
     * The method throws in case *this is not valueless.
     * @return Status code
     */
    Status getStatus() const {
        if (!carriesValue()) {
            return storage.status;
        }
        throw std::invalid_argument{"not a valueless optional"};
    }

private:
    bool valueless;
    VariantStorage<T> storage;
};

std::ostream& operator<<(std::ostream &os, Status obj)
{
    os << "Status(" << static_cast<std::underlying_type_t<Status>>(obj) << ")";
    return os;
}


template <typename T>
void printVariant(const Variant<T>& optional)
{
    if (optional.carriesValue()) {
        std::cout << "Value: " << optional.getValue() << std::endl;
    } else {
        std::cout << "Status: " << optional.getStatus() << std::endl;
    }
}

int main()
{
    Variant<std::string> variant1{"value"};
    Variant<std::string> variant2{Status::ERROR};
    Variant<Status> variant3{Forward{}, Status::INVALID_ARG};
    printVariant(variant1);
    printVariant(variant2);
    printVariant(variant3);

    /* copy semantics should work */
    auto variant4{variant1};
    Variant<std::string> variant5{"temp"};
    variant5 = variant1;
    printVariant(variant4);
    printVariant(variant5);

    /* move semantics should work */
    auto variant6{std::move(variant1)};
    variant5 = std::move(variant2);
    printVariant(variant6);
    printVariant(variant5);

    /* other stuff should work */
    Variant<int> variant7{123};
    variant7.getValue() = 456;
    printVariant(variant7);
    auto value = std::move(variant7).getValue();
    std::cout << value << std::endl;

    return 0;
}












