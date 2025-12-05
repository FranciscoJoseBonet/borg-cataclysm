#pragma once
#include <string>

class Weapon
{

private:
    std::string name;
    float fireRate;
    float coolDown;

public:
    Weapon(const std::string &name, float fireRate, float cd)
        : name(name), fireRate(fireRate), coolDown(cd) {}
    virtual ~Weapon() = default;

    void Shoot() {}

    std::string getName() const { return name; }
    float getFireRate() const { return fireRate; }
    float getCd() const { return coolDown; }
}