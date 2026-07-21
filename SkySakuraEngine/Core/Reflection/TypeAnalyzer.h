#pragma once

template <typename T, typename U>
struct is_same_type;

template <typename T, typename U>
struct is_same_type<T, U> {
    static const bool value = false;
};

template <typename T>
struct is_same_type<T, T> {
    static const bool value = true;
};

template <typename T>
struct is_pointer {
    static const bool value = false;
};

template <typename T>
struct is_pointer<T*> {
    static const bool value = true;
};