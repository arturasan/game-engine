#pragma once

#include <stdexcept>
#include <type_traits>

namespace GameEngine {

template<typename T>
class ServiceLocator {
    static T* service;
    static std::function<T*()> factory;
    
public:
    static void Provide(T* s) { 
        service = s; 
    }
    
    static void Provide(std::function<T*()> f) { 
        factory = f; 
    }
    
    static T& Get() { 
        if (!service) {
            if (factory) {
                service = factory();
            } else {
                throw std::runtime_error("Service not provided and no factory available");
            }
        }
        return *service; 
    }
    
    static bool IsAvailable() { 
        return service != nullptr; 
    }
    
    static void Reset() { 
        if (service) {
            delete service;
            service = nullptr;
        }
        factory = nullptr;
    }
};

// Initialize static members
template<typename T>
T* ServiceLocator<T>::service = nullptr;

template<typename T>
std::function<T*()> ServiceLocator<T>::factory = nullptr;

} // namespace GameEngine