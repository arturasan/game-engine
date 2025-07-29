#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace GameEngine {

// Physics handle types
using PhysicsBodyHandle = uint32_t;
using PhysicsShapeHandle = uint32_t;
using PhysicsJointHandle = uint32_t;
using PhysicsMaterialHandle = uint32_t;

// Physics body types
enum class PhysicsBodyType {
    Static,
    Dynamic,
    Kinematic
};

// Physics shape types
enum class PhysicsShapeType {
    Box,
    Sphere,
    Capsule,
    Cylinder,
    ConvexHull,
    TriangleMesh,
    HeightField
};

// Physics material properties
struct PhysicsMaterialDesc {
    float density = 1.0f;
    float friction = 0.5f;
    float restitution = 0.5f;
    float rollingFriction = 0.0f;
    float spinningFriction = 0.0f;
};

// Shape descriptions
struct BoxShapeDesc {
    glm::vec3 halfExtents{1.0f, 1.0f, 1.0f};
};

struct SphereShapeDesc {
    float radius = 1.0f;
};

struct CapsuleShapeDesc {
    float radius = 0.5f;
    float height = 2.0f;
};

struct CylinderShapeDesc {
    float radius = 0.5f;
    float height = 2.0f;
};

struct ConvexHullDesc {
    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;
};

struct TriangleMeshDesc {
    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;
};

struct HeightFieldDesc {
    int width = 0;
    int height = 0;
    std::vector<float> heights;
    float minHeight = 0.0f;
    float maxHeight = 1.0f;
    bool flipQuadEdges = false;
};

// Body description
struct PhysicsBodyDesc {
    PhysicsBodyType type = PhysicsBodyType::Dynamic;
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 linearVelocity{0.0f, 0.0f, 0.0f};
    glm::vec3 angularVelocity{0.0f, 0.0f, 0.0f};
    float mass = 1.0f;
    float linearDamping = 0.0f;
    float angularDamping = 0.0f;
    bool gravityEnabled = true;
    bool CCD = false; // Continuous Collision Detection
    PhysicsMaterialHandle material = 0;
};

// Joint types
enum class PhysicsJointType {
    Fixed,
    Hinge,
    Slider,
    Spherical,
    Point2Point,
    Generic6DOF
};

// Joint description
struct PhysicsJointDesc {
    PhysicsJointType type;
    PhysicsBodyHandle bodyA;
    PhysicsBodyHandle bodyB;
    glm::vec3 pivotA{0.0f, 0.0f, 0.0f};
    glm::vec3 pivotB{0.0f, 0.0f, 0.0f};
    glm::quat frameA{1.0f, 0.0f, 0.0f, 0.0f};
    glm::quat frameB{1.0f, 0.0f, 0.0f, 0.0f};
    
    // Joint limits
    bool useLimits = false;
    glm::vec3 lowerLimit{0.0f, 0.0f, 0.0f};
    glm::vec3 upperLimit{0.0f, 0.0f, 0.0f};
    
    // Joint motor
    bool useMotor = false;
    float motorTargetVelocity = 0.0f;
    float motorMaxForce = 0.0f;
};

// Raycast result
struct RaycastResult {
    bool hit = false;
    glm::vec3 hitPoint{0.0f, 0.0f, 0.0f};
    glm::vec3 hitNormal{0.0f, 0.0f, 0.0f};
    float hitDistance = 0.0f;
    PhysicsBodyHandle body = 0;
    void* userData = nullptr;
};

// Physics query filter
struct PhysicsQueryFilter {
    uint32_t collisionGroup = 0xFFFFFFFF;
    uint32_t collisionMask = 0xFFFFFFFF;
    void* userData = nullptr;
    std::function<bool(PhysicsBodyHandle)> shouldHitBody;
};

// Physics interface
class IPhysics {
public:
    virtual ~IPhysics() = default;
    
    // Physics lifecycle
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    
    // Simulation
    virtual void Update(float deltaTime) = 0;
    virtual void SetGravity(const glm::vec3& gravity) = 0;
    virtual const glm::vec3& GetGravity() const = 0;
    virtual void SetSubSteps(int subSteps) = 0;
    virtual int GetSubSteps() const = 0;
    
    // Materials
    virtual PhysicsMaterialHandle CreateMaterial(const PhysicsMaterialDesc& desc) = 0;
    virtual void DestroyMaterial(PhysicsMaterialHandle handle) = 0;
    virtual void SetMaterialProperties(PhysicsMaterialHandle handle, const PhysicsMaterialDesc& desc) = 0;
    virtual void GetMaterialProperties(PhysicsMaterialHandle handle, PhysicsMaterialDesc& desc) const = 0;
    
    // Shapes
    virtual PhysicsShapeHandle CreateBoxShape(const BoxShapeDesc& desc) = 0;
    virtual PhysicsShapeHandle CreateSphereShape(const SphereShapeDesc& desc) = 0;
    virtual PhysicsShapeHandle CreateCapsuleShape(const CapsuleShapeDesc& desc) = 0;
    virtual PhysicsShapeHandle CreateCylinderShape(const CylinderShapeDesc& desc) = 0;
    virtual PhysicsShapeHandle CreateConvexHullShape(const ConvexHullDesc& desc) = 0;
    virtual PhysicsShapeHandle CreateTriangleMeshShape(const TriangleMeshDesc& desc) = 0;
    virtual PhysicsShapeHandle CreateHeightFieldShape(const HeightFieldDesc& desc) = 0;
    virtual void DestroyShape(PhysicsShapeHandle handle) = 0;
    
    // Bodies
    virtual PhysicsBodyHandle CreateBody(const PhysicsBodyDesc& desc) = 0;
    virtual void DestroyBody(PhysicsBodyHandle handle) = 0;
    virtual void AddShapeToBody(PhysicsBodyHandle body, PhysicsShapeHandle shape, const glm::vec3& position, const glm::quat& rotation) = 0;
    virtual void RemoveShapeFromBody(PhysicsBodyHandle body, PhysicsShapeHandle shape) = 0;
    
    // Body properties
    virtual void SetBodyType(PhysicsBodyHandle handle, PhysicsBodyType type) = 0;
    virtual PhysicsBodyType GetBodyType(PhysicsBodyHandle handle) const = 0;
    virtual void SetBodyPosition(PhysicsBodyHandle handle, const glm::vec3& position) = 0;
    virtual glm::vec3 GetBodyPosition(PhysicsBodyHandle handle) const = 0;
    virtual void SetBodyRotation(PhysicsBodyHandle handle, const glm::quat& rotation) = 0;
    virtual glm::quat GetBodyRotation(PhysicsBodyHandle handle) const = 0;
    virtual void SetBodyLinearVelocity(PhysicsBodyHandle handle, const glm::vec3& velocity) = 0;
    virtual glm::vec3 GetBodyLinearVelocity(PhysicsBodyHandle handle) const = 0;
    virtual void SetBodyAngularVelocity(PhysicsBodyHandle handle, const glm::vec3& velocity) = 0;
    virtual glm::vec3 GetBodyAngularVelocity(PhysicsBodyHandle handle) const = 0;
    virtual void SetBodyMass(PhysicsBodyHandle handle, float mass) = 0;
    virtual float GetBodyMass(PhysicsBodyHandle handle) const = 0;
    virtual void SetBodyDamping(PhysicsBodyHandle handle, float linearDamping, float angularDamping) = 0;
    virtual void GetBodyDamping(PhysicsBodyHandle handle, float& linearDamping, float& angularDamping) const = 0;
    virtual void SetBodyGravityEnabled(PhysicsBodyHandle handle, bool enabled) = 0;
    virtual bool IsBodyGravityEnabled(PhysicsBodyHandle handle) const = 0;
    virtual void SetBodyCCDEnabled(PhysicsBodyHandle handle, bool enabled) = 0;
    virtual bool IsBodyCCDEnabled(PhysicsBodyHandle handle) const = 0;
    virtual void SetBodyUserData(PhysicsBodyHandle handle, void* userData) = 0;
    virtual void* GetBodyUserData(PhysicsBodyHandle handle) const = 0;
    
    // Forces and impulses
    virtual void ApplyForce(PhysicsBodyHandle handle, const glm::vec3& force) = 0;
    virtual void ApplyForceAtPoint(PhysicsBodyHandle handle, const glm::vec3& force, const glm::vec3& point) = 0;
    virtual void ApplyImpulse(PhysicsBodyHandle handle, const glm::vec3& impulse) = 0;
    virtual void ApplyImpulseAtPoint(PhysicsBodyHandle handle, const glm::vec3& impulse, const glm::vec3& point) = 0;
    virtual void ApplyTorque(PhysicsBodyHandle handle, const glm::vec3& torque) = 0;
    virtual void ApplyTorqueImpulse(PhysicsBodyHandle handle, const glm::vec3& impulse) = 0;
    
    // Joints
    virtual PhysicsJointHandle CreateJoint(const PhysicsJointDesc& desc) = 0;
    virtual void DestroyJoint(PhysicsJointHandle handle) = 0;
    virtual void SetJointMotorEnabled(PhysicsJointHandle handle, bool enabled) = 0;
    virtual bool IsJointMotorEnabled(PhysicsJointHandle handle) const = 0;
    virtual void SetJointMotorTarget(PhysicsJointHandle handle, float target) = 0;
    virtual float GetJointMotorTarget(PhysicsJointHandle handle) const = 0;
    virtual void SetJointMotorMaxForce(PhysicsJointHandle handle, float force) = 0;
    virtual float GetJointMotorMaxForce(PhysicsJointHandle handle) const = 0;
    
    // Queries
    virtual RaycastResult Raycast(const glm::vec3& start, const glm::vec3& end, const PhysicsQueryFilter& filter = {}) = 0;
    virtual std::vector<RaycastResult> RaycastAll(const glm::vec3& start, const glm::vec3& end, const PhysicsQueryFilter& filter = {}) = 0;
    virtual bool OverlapBox(const glm::vec3& center, const glm::vec3& halfExtents, const PhysicsQueryFilter& filter = {}) = 0;
    virtual bool OverlapSphere(const glm::vec3& center, float radius, const PhysicsQueryFilter& filter = {}) = 0;
    virtual std::vector<PhysicsBodyHandle> OverlapBoxAll(const glm::vec3& center, const glm::vec3& halfExtents, const PhysicsQueryFilter& filter = {}) = 0;
    virtual std::vector<PhysicsBodyHandle> OverlapSphereAll(const glm::vec3& center, float radius, const PhysicsQueryFilter& filter = {}) = 0;
    
    // Collision detection
    virtual void SetCollisionCallback(std::function<void(PhysicsBodyHandle, PhysicsBodyHandle)> callback) = 0;
    virtual void SetSeparationCallback(std::function<void(PhysicsBodyHandle, PhysicsBodyHandle)> callback) = 0;
    
    // Debug rendering
    virtual void DrawDebug() = 0;
    virtual void SetDebugDrawEnabled(bool enabled) = 0;
    virtual bool IsDebugDrawEnabled() const = 0;
    
    // Statistics
    virtual void GetStatistics(uint32_t& bodyCount, uint32_t& shapeCount, uint32_t& jointCount) const = 0;
    virtual void ResetStatistics() = 0;
};

} // namespace GameEngine