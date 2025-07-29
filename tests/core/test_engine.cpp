#include <gtest/gtest.h>
#include "core/Engine.hpp"
#include "core/ServiceLocator.hpp"
#include "modules/IRenderer.hpp"

using namespace GameEngine;

class EngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        engine = std::make_unique<EngineCore>();
    }
    
    void TearDown() override {
        if (engine) {
            engine->Shutdown();
        }
    }
    
    std::unique_ptr<EngineCore> engine;
};

TEST_F(EngineTest, Creation) {
    EXPECT_NE(engine, nullptr);
    EXPECT_FALSE(engine->IsRunning());
}

TEST_F(EngineTest, Initialization) {
    bool result = engine->Initialize();
    EXPECT_TRUE(result);
}

TEST_F(EngineTest, ServiceLocator) {
    // Test that service locator works correctly
    EXPECT_FALSE(ServiceLocator<IRenderer>::IsAvailable());
    
    // This should throw an exception
    EXPECT_THROW(ServiceLocator<IRenderer>::Get(), std::runtime_error);
}

TEST_F(EngineTest, Configuration) {
    // Test configuration loading
    engine->LoadConfiguration("config.json");
    
    // Check that configuration was loaded
    const auto& config = engine->GetConfig();
    EXPECT_FALSE(config.empty());
    EXPECT_TRUE(config.contains("engine"));
}

TEST_F(EngineTest, DeltaTime) {
    engine->Initialize();
    
    // Initially delta time should be 0
    EXPECT_FLOAT_EQ(engine->GetDeltaTime(), 0.0f);
    EXPECT_FLOAT_EQ(engine->GetTotalTime(), 0.0f);
}

TEST_F(EngineTest, ModuleManagement) {
    engine->Initialize();
    
    // Test loading a module (this will fail initially since we don't have dynamic loading)
    bool result = engine->LoadModule("nonexistent_module");
    EXPECT_FALSE(result);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}