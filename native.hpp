#pragma once

#include <cstdint>

typedef int Entity;
typedef int Ped;
typedef uint32_t Hash;

namespace PLAYER {
    inline Ped PLAYER_PED_ID() { return 0; }
}

namespace WEAPON {
    inline Hash GET_SELECTED_PED_WEAPON(Ped ped) { (void)ped; return 0; }
    inline int GET_WEAPON_DAMAGE(Hash weapon, int p1) { (void)weapon; (void)p1; return 50; }
    inline void GIVE_WEAPON_TO_PED(Ped ped, Hash weapon, int ammo, bool b1, bool b2) { (void)ped; (void)weapon; (void)ammo; (void)b1; (void)b2; }
}

namespace PED {
    inline bool IS_PED_SHOOTING(Ped ped) { (void)ped; return false; }
}

namespace ENTITY {
    struct Vector3 { float x, y, z; };
    inline Vector3 GET_ENTITY_COORDS(Entity e, bool alive) { (void)e; (void)alive; return {0,0,0}; }
}

namespace MISC {
    inline void SHOOT_SINGLE_BULLET_BETWEEN_COORDS(float x1, float y1, float z1, float x2, float y2, float z2, int damage, bool p7, Hash weapon, Ped owner, bool p10, bool p11, float speed) {
        (void)x1;(void)y1;(void)z1;(void)x2;(void)y2;(void)z2;(void)damage;(void)p7;(void)weapon;(void)owner;(void)p10;(void)p11;(void)speed;
    }
}
