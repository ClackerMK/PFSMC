#ifndef STATISTICS_H
#define STATISTICS_H

enum Size {
    Fine,
    Diminutive,
    Tiny,
    Small,
    Medium,
    Large,
    Huge,
    Gargantuan,
    Colossal
};

struct Statistics
{
    // Hit Points
    int HP  = 0;

    // Armor
    int AC = 0;
    int AC_touch = 0;
    int AC_ff = 0;

    // Saves
    int Fort = 0;
    int Ref = 0;
    int Will = 0;

    // Combat Maneauver
    int CMD = 0;

    // DCs
    int AbillityDC = 0;
    int SpellDC = 0;

    // Ability Modifier
    int Str = 0;
    int Dex = 0;
    int Con = 0;
    int Wis = 0;
    int Int = 0;
    int Cha = 0;

    // Spellcaster
    bool caster = false;

    // Size
    Size size = Medium;
};


#endif // STATISTICS_H
