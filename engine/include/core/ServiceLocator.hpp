#pragma once

template<typename T>
class ServiceLocator {
    static T* service;
public:
    static void Provide(T* s) { service = s; }
    static T& Get() { return *service; }
};

template<typename T>
T* ServiceLocator<T>::service = nullptr;
