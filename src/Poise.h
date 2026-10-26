#pragma once

// Elden Combat - poise ("unbalance") system, TrueHUD poise bar and stamina add-on.
//
// Balancing follows Elden Rim's unbalance rules, rebuilt as a numeric meter so
// TrueHUD can show it:
//   - 1H weapons break a 100-poise target in 3 quick hits, 2H weapons in 2.
//   - Power attacks deal more poise damage.
//   - Poise refills after a short delay without hits (Elden Rim's 2-3 s windows).
//   - A blocking target is guard-broken when its poise breaks, or when a blocked
//     hit lands while its stamina is (nearly) empty.
//   - Enemies break the player's poise with reduced damage.
// Breaking poise triggers one stagger; the Elden Rim stagger OAR pack picks the
// animation and sound (block-broken while blocking, unbalance otherwise).

class PoiseSettings
{
public:
    static PoiseSettings* GetSingleton();
    void Load();
    void Save();

    // [Poise]
    bool  bEnablePoise{ true };
    float fBasePoise{ 100.0f };          // NPC humanoid poise
    float fPlayerPoise{ 100.0f };        // player poise
    float fCreatureSizeScaling{ 1.0f };  // 0 = creatures use base poise; 1 = full height scaling
    float fMinSizeMult{ 0.5f };
    float fMaxSizeMult{ 5.0f };
    float fArmorPoisePerWeight{ 0.0f };  // extra poise per unit of worn armor weight
    float fRegenDelay{ 2.5f };           // seconds without poise damage before refilling
    float fRegenPercentPerSec{ 40.0f };  // % of max poise refilled per second
    float fBreakImmunity{ 2.0f };        // seconds of no poise damage after a break
    bool  bSuppressVanillaStagger{ true }; // melee hits only stagger through poise (needs Precision)

    // [Damage] base poise damage per hit
    float fDmgUnarmed{ 20.0f };
    float fDmgDagger{ 25.0f };
    float fDmgSword{ 34.0f };
    float fDmgWarAxe{ 34.0f };
    float fDmgMace{ 40.0f };
    float fDmgGreatsword{ 50.0f };
    float fDmgBattleaxe{ 50.0f };        // battleaxes and warhammers
    float fDmgBow{ 20.0f };
    float fDmgCrossbow{ 34.0f };
    float fDmgStaff{ 15.0f };
    float fDmgCreature{ 34.0f };         // creature natural attacks (scaled by attacker size)
    float fDmgBash{ 40.0f };
    float fDmgSpell{ 0.0f };             // per spell hit (0 = spells deal no poise damage)
    float fPowerAttackMult{ 2.0f };
    float fBlockedHitMult{ 0.5f };       // poise damage on a blocked hit
    float fPlayerToNPCMult{ 1.0f };
    float fNPCToPlayerMult{ 0.6f };
    float fNPCToNPCMult{ 1.0f };

    // [Break] stagger magnitude used when poise breaks (MSL tiers: ~0.25 small .. 1.0+ largest)
    float fBreakMagLight{ 0.75f };       // unarmed, 1H, ranged, spell
    float fBreakMagHeavy{ 1.25f };       // 2H, creatures
    float fBreakMagPowerBonus{ 0.25f };
    float fBreakMagOnPlayer{ 0.5f };
    bool  bGuardBreak{ true };
    float fGuardBreakStaminaPct{ 5.0f }; // blocked hit at or below this stamina % = guard break
    float fGuardBreakMag{ 1.0f };
    bool  bPerfectBlockBreaksPoise{ true }; // perfect block / ward parry staggers empty the attacker's poise

    // [HyperArmor] reduced poise damage while winding up / swinging a power attack
    bool  bHyperArmor{ true };
    float fHyperArmorMult{ 0.4f };       // poise damage multiplier during a power attack
    bool  bHyperArmorPlayer{ true };
    bool  bHyperArmorNPC{ true };

    // [TrueHUD]
    bool  bTrueHUDBar{ true };
    bool  bTrueHUDPlayerBar{ true };     // needs a restart to change
    bool  bFlashOnBreak{ true };
    bool  bBarColors{ true };            // recolor the poise bar by state
    float fDangerPct{ 35.0f };           // at or below this % the bar turns "danger"
    std::uint32_t uNormalColor{ 0xE0C068 };  // RGB
    std::uint32_t uDangerColor{ 0xFF8C1A };
    std::uint32_t uBrokenColor{ 0xE53935 };
    bool  bNormalColorDefault{ true };   // keep TrueHUD's own color while poise is healthy

    // [Stamina] Valhalla-style stamina add-on
    bool  bStaminaAddon{ true };
    float fCombatRegenMult{ 1.5f };      // multiplies the game's combat stamina regen
    float fRegenDelayMult{ 1.0f };       // multiplies the game's stamina regen delay (lower = sooner)
    float fLightHitRestorePct{ 5.0f };   // % of max stamina restored when a light melee attack lands
    bool  bLightHitRestorePlayer{ true };
    bool  bLightHitRestoreNPC{ false };

    // [Debug]
    bool  bDebug{ false };

    // [RaceOverrides] "Plugin.esp|0xFormID" = poise multiplier
    std::vector<std::pair<std::string, float>> raceOverrides;

    static constexpr const char* INI_PATH = R"(.\Data\SKSE\Plugins\EldenCombat_Poise.ini)";

private:
    PoiseSettings() = default;
};

namespace Poise
{
    // lifecycle
    void OnDataLoaded();          // register hit sink, resolve race overrides, apply stamina GMSTs
    void OnPostLoad();            // request TrueHUD API
    void OnGameLoaded();          // clear per-actor state
    void Update(float a_delta);   // called every player animation update

    // integration points used by the timed block / dodge code
    void OnExternalStagger(RE::Actor* a_actor);  // a perfect block / ward parry stagger hit this actor
    void NotePerfectBlock(RE::Actor* a_defender); // defender just perfect-blocked: ignore that hit for poise
    bool ShouldSuppressVanillaStagger(RE::Actor* a_target);

    // queries (TrueHUD + menu)
    float GetCurrent(RE::Actor* a_actor);
    float GetMax(RE::Actor* a_actor);

    // stamina add-on
    void ApplyStaminaSettings();

    // menu
    void __stdcall RenderMenu();
}
