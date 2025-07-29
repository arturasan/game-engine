#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include <filesystem>

namespace GameEngine {

// Forward declarations
class IAssetLoader;
class IAsset;

// Asset handle type
using AssetHandle = uint32_t;

// Asset metadata
struct AssetMetadata {
    std::string path;
    std::string type;
    uint64_t size = 0;
    uint64_t lastModified = 0;
    std::unordered_map<std::string, std::string> customData;
};

// Asset loading options
struct AssetLoadOptions {
    bool async = false;
    bool cache = true;
    bool hotReload = false;
    std::unordered_map<std::string, std::string> parameters;
};

// Asset interface
class IAsset {
public:
    virtual ~IAsset() = default;
    virtual const std::string& GetType() const = 0;
    virtual const std::string& GetPath() const = 0;
    virtual bool IsLoaded() const = 0;
    virtual void Unload() = 0;
    virtual uint64_t GetMemoryUsage() const = 0;
    
    template<typename T>
    T* As() {
        return dynamic_cast<T*>(this);
    }
};

// Asset loader interface
class IAssetLoader {
public:
    virtual ~IAssetLoader() = default;
    virtual std::unique_ptr<IAsset> LoadAsset(const std::string& path, const AssetLoadOptions& options = {}) = 0;
    virtual bool CanLoadAsset(const std::string& path) const = 0;
    virtual const std::string& GetSupportedExtension() const = 0;
    virtual const std::string& GetAssetType() const = 0;
};

// Asset manager interface
class IAssetManager {
public:
    virtual ~IAssetManager() = default;
    
    // Asset lifecycle
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual void Update() = 0;
    
    // Asset loading
    virtual AssetHandle LoadAsset(const std::string& path, const AssetLoadOptions& options = {}) = 0;
    virtual void UnloadAsset(AssetHandle handle) = 0;
    virtual void ReloadAsset(AssetHandle handle) = 0;
    
    // Asset access
    virtual IAsset* GetAsset(AssetHandle handle) = 0;
    virtual const IAsset* GetAsset(AssetHandle handle) const = 0;
    virtual const AssetMetadata* GetAssetMetadata(AssetHandle handle) const = 0;
    
    // Asset querying
    virtual std::vector<AssetHandle> FindAssetsByType(const std::string& type) const = 0;
    virtual std::vector<AssetHandle> FindAssetsByPath(const std::string& pathPattern) const = 0;
    virtual bool AssetExists(const std::string& path) const = 0;
    
    // Asset registration
    virtual void RegisterLoader(std::unique_ptr<IAssetLoader> loader) = 0;
    virtual void UnregisterLoader(const std::string& extension) = 0;
    virtual bool IsLoaderRegistered(const std::string& extension) const = 0;
    
    // Asset watching (hot-reload)
    virtual void WatchDirectory(const std::string& path, bool recursive = true) = 0;
    virtual void UnwatchDirectory(const std::string& path) = 0;
    virtual void SetHotReloadEnabled(bool enabled) = 0;
    virtual bool IsHotReloadEnabled() const = 0;
    
    // Asset cooking/conversion
    virtual bool CookAsset(const std::string& inputPath, const std::string& outputPath) = 0;
    virtual bool CookDirectory(const std::string& inputPath, const std::string& outputPath, bool recursive = true) = 0;
    
    // Memory management
    virtual void GarbageCollect() = 0;
    virtual uint64_t GetTotalMemoryUsage() const = 0;
    virtual uint32_t GetLoadedAssetCount() const = 0;
    
    // Asset events
    using AssetEventCallback = std::function<void(AssetHandle, const std::string&)>;
    virtual void SetAssetLoadedCallback(AssetEventCallback callback) = 0;
    virtual void SetAssetUnloadedCallback(AssetEventCallback callback) = 0;
    virtual void SetAssetReloadedCallback(AssetEventCallback callback) = 0;
    
    // Asset paths
    virtual void AddAssetPath(const std::string& path, int priority = 0) = 0;
    virtual void RemoveAssetPath(const std::string& path) = 0;
    virtual std::string ResolveAssetPath(const std::string& relativePath) const = 0;
    
    // Statistics
    virtual void GetStatistics(uint32_t& loadedAssets, uint64_t& memoryUsage, uint32_t& cacheHits, uint32_t& cacheMisses) const = 0;
    virtual void ResetStatistics() = 0;
};

} // namespace GameEngine