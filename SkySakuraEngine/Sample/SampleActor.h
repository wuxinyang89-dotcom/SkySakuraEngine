#pragma once
#include "ReflectionMacros.h"
#include "SampleActor.generated.h"

// ============================================================
// 示例：用户编写的原始类
// ReflectionParser 会扫描此文件，识别 SCLASS/SPROPERTY/SFUNCTION 宏
// 并生成对应的 SampleActor.generated.h
// ============================================================

SCLASS()
class SampleActor
{
    GENERATED_BODY()

public:
    SampleActor() : health_(100.0f), name_("DefaultActor"), isActive_(true) {}

    SFUNCTION()
    void TakeDamage(float damage)
    {
        health_ -= damage;
        if (health_ < 0.0f) health_ = 0.0f;
    }

    SFUNCTION()
    float GetHealth() const { return health_; }

    SFUNCTION()
    void SetName(const char* name) { name_ = name; }

private:
    SPROPERTY()
    float health_;

    SPROPERTY()
    const char* name_;

    SPROPERTY()
    bool isActive_;
};
