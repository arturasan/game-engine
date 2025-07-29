#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "=== Game Engine Test Suite ===" << std::endl;
    
    ::testing::InitGoogleTest(&argc, argv);
    
    // Run all tests
    int result = RUN_ALL_TESTS();
    
    if (result == 0) {
        std::cout << "\nAll tests passed!" << std::endl;
    } else {
        std::cout << "\nSome tests failed!" << std::endl;
    }
    
    return result;
}