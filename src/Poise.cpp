#include "Poise.h"
#include "SKSEMenuFramework.h"
#include "TrueHUDAPI.h"

// ============================================================================
// Settings
// ============================================================================
PoiseSettings* PoiseSettings::GetSingleton()
{
    static PoiseSettings singleton;
    return &singleton;
}

namespace
{
    float GetF(CSimpleIniA& ini, const char* sec, const char* key, float def)
    {
        return static_cast<float>(ini.GetDoubleValue(sec, key, def));
    }
}

void PoiseSettings::Load()
{
    CSimpleIniA ini;
    ini.SetUnicode();
    if (ini.LoadFile(INI_PATH) < 0) {
        logger::warn("[Poise] {} not found, writing defaults", INI_PATH);
        Save();
        return;
    }

    bEnablePoise = ini.GetBoolValue("Poise", "bEnablePoise", bEnablePoise);
    fBasePoise = GetF(ini, "Poise", "fBasePoise", fBasePoise);
    fPlayerPoise = GetF(ini, "Poise", "fPlayerPoise", fPlayerPoise);
    fCreatureSizeScaling = GetF(ini, "Poise", "fCreatureSizeScaling", fCreatureSizeScaling);
    fMinSizeMult = GetF(ini, "Poise", "fMinSizeMult", fMinSizeMult);
    fMaxSizeMult = GetF(ini, "Poise", "fMaxSizeMult", fMaxSizeMult);
    fArmorPoisePerWeight = GetF(ini, "Poise", "fArmorPoisePerWeight", fArmorPoisePerWeight);
    fRegenDelay = GetF(ini, "Poise", "fRegenDelay", fRegenDelay);
    fRegenPercentPerSec = GetF(ini, "Poise", "fRegenPercentPerSec", fRegenPercentPerSec);
    fBreakImmunity = GetF(ini, "Poise", "fBreakImmunity", fBreakImmunity);
    bSuppressVanillaStagger = ini.GetBoolValue("Poise", "bSuppressVanillaStagger", bSuppressVanillaStagger);

    fDmgUnarmed = GetF(ini, "Damage", "fDmgUnarmed", fDmgUnarmed);
    fDmgDagger = GetF(ini, "Damage", "fDmgDagger", fDmgDagger);
    fDmgSword = GetF(ini, "Damage", "fDmgSword", fDmgSword);
    fDmgWarAxe = GetF(ini, "Damage", "fDmgWarAxe", fDmgWarAxe);
    fDmgMace = GetF(ini, "Damage", "fDmgMace", fDmgMace);
    fDmgGreatsword = GetF(ini, "Damage", "fDmgGreatsword", fDmgGreatsword);
    fDmgBattleaxe = GetF(ini, "Damage", "fDmgBattleaxe", fDmgBattleaxe);
    fDmgBow = GetF(ini, "Damage", "fDmgBow", fDmgBow);
    fDmgCrossbow = GetF(ini, "Damage", "fDmgCrossbow", fDmgCrossbow);
    fDmgStaff = GetF(ini, "Damage", "fDmgStaff", fDmgStaff);
    fDmgCreature = GetF(ini, "Damage", "fDmgCreature", fDmgCreature);
    fDmgBash = GetF(ini, "Damage", "fDmgBash", fDmgBash);
    fDmgSpell = GetF(ini, "Damage", "fDmgSpell", fDmgSpell);
    fPowerAttackMult = GetF(ini, "Damage", "fPowerAttackMult", fPowerAttackMult);
    fBlockedHitMult = GetF(ini, "Damage", "fBlockedHitMult", fBlockedHitMult);
    fPlayerToNPCMult = GetF(ini, "Damage", "fPlayerToNPCMult", fPlayerToNPCMult);
    fNPCToPlayerMult = GetF(ini, "Damage", "fNPCToPlayerMult", fNPCToPlayerMult);
    fNPCToNPCMult = GetF(ini, "Damage", "fNPCToNPCMult", fNPCToNPCMult);

    fBreakMagLight = GetF(ini, "Break", "fBreakMagLight", fBreakMagLight);
    fBreakMagHeavy = GetF(ini, "Break", "fBreakMagHeavy", fBreakMagHeavy);
    fBreakMagPowerBonus = GetF(ini, "Break", "fBreakMagPowerBonus", fBreakMagPowerBonus);
    fBreakMagOnPlayer = GetF(ini, "Break", "fBreakMagOnPlayer", fBreakMagOnPlayer);
    bGuardBreak = ini.GetBoolValue("Break", "bGuardBreak", bGuardBreak);
    fGuardBreakStaminaPct = GetF(ini, "Break", "fGuardBreakStaminaPct", fGuardBreakStaminaPct);
    fGuardBreakMag = GetF(ini, "Break", "fGuardBreakMag", fGuardBreakMag);
    bPerfectBlockBreaksPoise = ini.GetBoolValue("Break", "bPerfectBlockBreaksPoise", bPerfectBlockBreaksPoise);

    bTrueHUDBar = ini.GetBoolValue("TrueHUD", "bTrueHUDBar", bTrueHUDBar);
    bTrueHUDPlayerBar = ini.GetBoolValue("TrueHUD", "bTrueHUDPlayerBar", bTrueHUDPlayerBar);
    bFlashOnBreak = ini.GetBoolValue("TrueHUD", "bFlashOnBreak", bFlashOnBreak);

    bStaminaAddon = ini.GetBoolValue("Stamina", "bStaminaAddon", bStaminaAddon);
    fCombatRegenMult = GetF(ini, "Stamina", "fCombatRegenMult", fCombatRegenMult);
    fRegenDelayMult = GetF(ini, "Stamina", "fRegenDelayMult", fRegenDelayMult);
    fLightHitRestorePct = GetF(ini, "Stamina", "fLightHitRestorePct", fLightHitRestorePct);
    bLightHitRestorePlayer = ini.GetBoolValue("Stamina", "bLightHitRestorePlayer", bLightHitRestorePlayer);
    bLightHitRestoreNPC = ini.GetBoolValue("Stamina", "bLightHitRestoreNPC", bLightHitRestoreNPC);

    bDebug = ini.GetBoolValue("Debug", "bDebug", bDebug);

    raceOverrides.clear();
    CSimpleIniA::TNamesDepend keys;
    ini.GetAllKeys("RaceOverrides", keys);
    for (const auto& k : keys) {
        raceOverrides.emplace_back(k.pItem, GetF(ini, "RaceOverrides", k.pItem, 1.0f));
    }

    logger::info("[Poise] settings loaded: poise={} base={} player={} trueHUD={} stamina={}",
        bEnablePoise, fBasePoise, fPlayerPoise, bTrueHUDBar, bStaminaAddon);
}

void PoiseSettings::Save()
{
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(INI_PATH);  // keep RaceOverrides and comments that are already there

    ini.SetBoolValue("Poise", "bEnablePoise", bEnablePoise);
    ini.SetDoubleValue("Poise", "fBasePoise", fBasePoise);
    ini.SetDoubleValue("Poise", "fPlayerPoise", fPlayerPoise);
    ini.SetDoubleValue("Poise", "fCreatureSizeScaling", fCreatureSizeScaling);
    ini.SetDoubleValue("Poise", "fMinSizeMult", fMinSizeMult);
    ini.SetDoubleValue("Poise", "fMaxSizeMult", fMaxSizeMult);
    ini.SetDoubleValue("Poise", "fArmorPoisePerWeight", fArmorPoisePerWeight);
    ini.SetDoubleValue("Poise", "fRegenDelay", fRegenDelay);
    ini.SetDoubleValue("Poise", "fRegenPercentPerSec", fRegenPercentPerSec);
    ini.SetDoubleValue("Poise", "fBreakImmunity", fBreakImmunity);
    ini.SetBoolValue("Poise", "bSuppressVanillaStagger", bSuppressVanillaStagger);

    ini.SetDoubleValue("Damage", "fDmgUnarmed", fDmgUnarmed);
    ini.SetDoubleValue("Damage", "fDmgDagger", fDmgDagger);
    ini.SetDoubleValue("Damage", "fDmgSword", fDmgSword);
    ini.SetDoubleValue("Damage", "fDmgWarAxe", fDmgWarAxe);
    ini.SetDoubleValue("Damage", "fDmgMace", fDmgMace);
    ini.SetDoubleValue("Damage", "fDmgGreatsword", fDmgGreatsword);
    ini.SetDoubleValue("Damage", "fDmgBattleaxe", fDmgBattleaxe);
    ini.SetDoubleValue("Damage", "fDmgBow", fDmgBow);
    ini.SetDoubleValue("Damage", "fDmgCrossbow", fDmgCrossbow);
    ini.SetDoubleValue("Damage", "fDmgStaff", fDmgStaff);
    ini.SetDoubleValue("Damage", "fDmgCreature", fDmgCreature);
    ini.SetDoubleValue("Damage", "fDmgBash", fDmgBash);
    ini.SetDoubleValue("Damage", "fDmgSpell", fDmgSpell);
    ini.SetDoubleValue("Damage", "fPowerAttackMult", fPowerAttackMult);
    ini.SetDoubleValue("Damage", "fBlockedHitMult", fBlockedHitMult);
    ini.SetDoubleValue("Damage", "fPlayerToNPCMult", fPlayerToNPCMult);
    ini.SetDoubleValue("Damage", "fNPCToPlayerMult", fNPCToPlayerMult);
    ini.SetDoubleValue("Damage", "fNPCToNPCMult", fNPCToNPCMult);

    ini.SetDoubleValue("Break", "fBreakMagLight", fBreakMagLight);
    ini.SetDoubleValue("Break", "fBreakMagHeavy", fBreakMagHeavy);
    ini.SetDoubleValue("Break", "fBreakMagPowerBonus", fBreakMagPowerBonus);
    ini.SetDoubleValue("Break", "fBreakMagOnPlayer", fBreakMagOnPlayer);
    ini.SetBoolValue("Break", "bGuardBreak", bGuardBreak);
    ini.SetDoubleValue("Break", "fGuardBreakStaminaPct", fGuardBreakStaminaPct);
    ini.SetDoubleValue("Break", "fGuardBreakMag", fGuardBreakMag);
    ini.SetBoolValue("Break", "bPerfectBlockBreaksPoise", bPerfectBlockBreaksPoise);

    ini.SetBoolValue("TrueHUD", "bTrueHUDBar", bTrueHUDBar);
    ini.SetBoolValue("TrueHUD", "bTrueHUDPlayerBar", bTrueHUDPlayerBar);
    ini.SetBoolValue("TrueHUD", "bFlashOnBreak", bFlashOnBreak);

    ini.SetBoolValue("Stamina", "bStaminaAddon", bStaminaAddon);
    ini.SetDoubleValue("Stamina", "fCombatRegenMult", fCombatRegenMult);
    ini.SetDoubleValue("Stamina", "fRegenDelayMult", fRegenDelayMult);
    ini.SetDoubleValue("Stamina", "fLightHitRestorePct", fLightHitRestorePct);
    ini.SetBoolValue("Stamina", "bLightHitRestorePlayer", bLightHitRestorePlayer);
    ini.SetBoolValue("Stamina", "bLightHitRestoreNPC", bLightHitRestoreNPC);

    ini.SetBoolValue("Debug", "bDebug", bDebug);

    for (const auto& [key, mult] : raceOverrides) {
        ini.SetDoubleValue("RaceOverrides", key.c_str(), mult);
    }

    if (ini.SaveFile(INI_PATH) < 0) {
        logger::error("[Poise] failed to save {}", INI_PATH);
    }
}

// ============================================================================
// Poise state
// ============================================================================
namespace
{
    struct PoiseState
    {
        float damage{ 0.0f };          // poise lost, settled at lastHit
        float lastHit{ -1000.0f };     // clock time of the last poise damage
        float brokenUntil{ -1000.0f }; // bar shows empty + immune until this time
        float max{ 0.0f };             // cached max poise
        float maxStamp{ -1000.0f };
        float lastTouched{ 0.0f };
    };

    std::recursive_mutex g_lock;
    std::unordered_map<RE::FormID, PoiseState> g_states;
    std::unordered_map<RE::FormID, float> g_ignoreHitsUntil;  // defenders who just perfect-blocked
    struct LastHit { RE::FormID attacker{ 0 }; float time{ -1000.0f }; };
    std::unordered_map<RE::FormID, LastHit> g_lastHit;
    std::unordered_map<RE::FormID, float> g_raceMult;
    std::atomic<float> g_clock{ 0.0f };
    float g_pruneTimer{ 0.0f };

    TRUEHUD_API::IVTrueHUD3* g_trueHUD{ nullptr };
    bool g_ownsSpecialBar{ false };

    float g_baseCombatRegen{ -1.0f };
    float g_baseRegenDelay{ -1.0f };

    float Now() { return g_clock.load(); }

    void PoiseLog(const std::string& msg)
    {
        if (PoiseSettings::GetSingleton()->bDebug) {
            logger::info("[Poise] {}", msg);
            RE::DebugNotification(msg.c_str());
        }
    }

    float SizeMult(RE::Actor* a)
    {
        auto* s = PoiseSettings::GetSingleton();
        const auto mx = a->GetBoundMax();
        const auto mn = a->GetBoundMin();
        const float h = (mx.z - mn.z) * a->GetScale();
        if (h <= 1.0f) {
            return 1.0f;
        }
        const float raw = h / 128.0f;
        const float m = 1.0f + (raw - 1.0f) * s->fCreatureSizeScaling;
        return std::clamp(m, s->fMinSizeMult, s->fMaxSizeMult);
    }

    bool IsHumanoid(RE::Actor* a)
    {
        return a->HasKeywordString("ActorTypeNPC");
    }

    float ArmorBonus(RE::Actor* a)
    {
        auto* s = PoiseSettings::GetSingleton();
        if (s->fArmorPoisePerWeight <= 0.0f) {
            return 0.0f;
        }
        float weight = 0.0f;
        auto inv = a->GetInventory([](RE::TESBoundObject& o) { return o.IsArmor(); });
        for (auto& [obj, data] : inv) {
            const auto& entry = data.second;
            if (entry && entry->IsWorn()) {
                if (auto* armo = obj->As<RE::TESObjectARMO>()) {
                    weight += armo->weight;
                }
            }
        }
        return weight * s->fArmorPoisePerWeight;
    }

    // main thread only
    float ComputeMax(RE::Actor* a)
    {
        auto* s = PoiseSettings::GetSingleton();
        if (a->IsPlayerRef()) {
            return (std::max)(1.0f, s->fPlayerPoise + ArmorBonus(a));
        }
        float mult = 1.0f;
        if (auto* race = a->GetRace()) {
            if (auto it = g_raceMult.find(race->GetFormID()); it != g_raceMult.end()) {
                mult = it->second;
            } else if (!IsHumanoid(a)) {
                mult = SizeMult(a);
            }
        }
        return (std::max)(1.0f, s->fBasePoise * mult + ArmorBonus(a));
    }

    // caller holds g_lock
    float CurrentDamage(const PoiseState& st, float max)
    {
        auto* s = PoiseSettings::GetSingleton();
        const float now = Now();
        const float regenStart = st.lastHit + s->fRegenDelay;
        float dmg = st.damage;
        if (now > regenStart) {
            dmg -= (now - regenStart) * (s->fRegenPercentPerSec / 100.0f) * max;
        }
        return (std::max)(0.0f, dmg);
    }

    // main thread only; caller holds g_lock
    PoiseState& Touch(RE::Actor* a)
    {
        auto& st = g_states[a->GetFormID()];
        const float now = Now();
        if (st.max <= 0.0f || now - st.maxStamp > 5.0f) {
            st.max = ComputeMax(a);
            st.maxStamp = now;
        }
        st.lastTouched = now;
        return st;
    }

    void SendStagger(RE::Actor* a_target, RE::Actor* a_source, float a_magnitude)
    {
        if (!a_target || a_target->IsDead() || !a_target->Is3DLoaded()) {
            return;
        }
        if (a_target->AsActorState()->IsSwimming() || a_target->IsInKillMove()) {
            return;
        }
        float direction = 0.0f;
        if (a_source) {
            const float heading = a_target->GetHeadingAngle(a_source->GetPosition(), false);
            direction = (heading >= 0.0f) ? heading / 360.0f : (360.0f + heading) / 360.0f;
        }
        a_target->SetGraphVariableFloat("staggerDirection", direction);
        a_target->SetGraphVariableFloat("StaggerMagnitude", a_magnitude);
        a_target->NotifyAnimationGraph("staggerStart");
    }

    // caller holds g_lock
    void MarkBroken(RE::Actor* a, PoiseState& st)
    {
        st.damage = 0.0f;  // refills to full once the break window ends
        st.lastHit = -1000.0f;
        st.brokenUntil = Now() + PoiseSettings::GetSingleton()->fBreakImmunity;
        if (g_trueHUD && g_ownsSpecialBar && PoiseSettings::GetSingleton()->bFlashOnBreak) {
            g_trueHUD->FlashActorSpecialBar(SKSE::GetPluginHandle(), a->GetHandle(), true);
        }
    }

    void Break(RE::Actor* a_target, RE::Actor* a_attacker, float a_mag, PoiseState& st, const char* why)
    {
        MarkBroken(a_target, st);
        SendStagger(a_target, a_attacker, a_mag);
        PoiseLog(fmt::format("{} poise broken ({}) mag {:.2f}", a_target->GetName(), why, a_mag));
    }

    enum class HitClass { kLight, kHeavy, kRanged, kSpell, kNone };

    struct HitInfo
    {
        float damage{ 0.0f };
        HitClass cls{ HitClass::kNone };
        bool melee{ false };
    };

    HitInfo Classify(RE::Actor* attacker, RE::FormID source, bool bash)
    {
        auto* s = PoiseSettings::GetSingleton();
        HitInfo h;
        auto* form = source ? RE::TESForm::LookupByID(source) : nullptr;

        if (bash) {
            h = { s->fDmgBash, HitClass::kLight, true };
            return h;
        }
        if (form && form->Is(RE::FormType::Enchantment)) {
            return h;  // the weapon hit is counted separately
        }
        if (form && form->Is(RE::FormType::Weapon)) {
            auto* weap = form->As<RE::TESObjectWEAP>();
            using WT = RE::WEAPON_TYPE;
            switch (weap->GetWeaponType()) {
            case WT::kHandToHandMelee:
                if (!IsHumanoid(attacker)) {
                    h = { s->fDmgCreature * SizeMult(attacker), HitClass::kHeavy, true };
                } else {
                    h = { s->fDmgUnarmed, HitClass::kLight, true };
                }
                break;
            case WT::kOneHandDagger: h = { s->fDmgDagger, HitClass::kLight, true }; break;
            case WT::kOneHandSword:  h = { s->fDmgSword, HitClass::kLight, true }; break;
            case WT::kOneHandAxe:    h = { s->fDmgWarAxe, HitClass::kLight, true }; break;
            case WT::kOneHandMace:   h = { s->fDmgMace, HitClass::kLight, true }; break;
            case WT::kTwoHandSword:  h = { s->fDmgGreatsword, HitClass::kHeavy, true }; break;
            case WT::kTwoHandAxe:    h = { s->fDmgBattleaxe, HitClass::kHeavy, true }; break;
            case WT::kBow:           h = { s->fDmgBow, HitClass::kRanged, false }; break;
            case WT::kCrossbow:      h = { s->fDmgCrossbow, HitClass::kRanged, false }; break;
            case WT::kStaff:         h = { s->fDmgStaff, HitClass::kSpell, false }; break;
            default: break;
            }
            return h;
        }
        if (form && (form->Is(RE::FormType::Spell) || form->Is(RE::FormType::Scroll))) {
            h = { s->fDmgSpell, HitClass::kSpell, false };
            return h;
        }
        // no source form: creature natural attack
        if (!IsHumanoid(attacker)) {
            h = { s->fDmgCreature * SizeMult(attacker), HitClass::kHeavy, true };
        }
        return h;
    }

    void RestoreStaminaOnLightHit(RE::Actor* attacker)
    {
        auto* s = PoiseSettings::GetSingleton();
        if (!s->bStaminaAddon || s->fLightHitRestorePct <= 0.0f) {
            return;
        }
        const bool isPlayer = attacker->IsPlayerRef();
        if ((isPlayer && !s->bLightHitRestorePlayer) || (!isPlayer && !s->bLightHitRestoreNPC)) {
            return;
        }
        auto* av = attacker->AsActorValueOwner();
        const float maxSt = av->GetPermanentActorValue(RE::ActorValue::kStamina);
        const float cur = av->GetActorValue(RE::ActorValue::kStamina);
        const float amount = (std::min)(maxSt * s->fLightHitRestorePct / 100.0f, maxSt - cur);
        if (amount > 0.0f) {
            av->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, amount);
        }
    }

    void ProcessHit(RE::ActorHandle a_attacker, RE::ActorHandle a_target, RE::FormID a_source,
        bool a_power, bool a_bash, bool a_blocked)
    {
        auto* s = PoiseSettings::GetSingleton();
        auto attackerPtr = a_attacker.get();
        auto targetPtr = a_target.get();
        auto* attacker = attackerPtr.get();
        auto* target = targetPtr.get();
        if (!attacker || !target || attacker == target || target->IsDead()) {
            return;
        }

        const HitInfo hit = Classify(attacker, a_source, a_bash);

        // stamina add-on: landed light melee hit
        if (hit.melee && !a_power && !a_bash && !a_blocked) {
            RestoreStaminaOnLightHit(attacker);
        }

        if (!s->bEnablePoise || hit.cls == HitClass::kNone) {
            return;
        }

        std::lock_guard lock(g_lock);
        const float now = Now();

        if (auto it = g_ignoreHitsUntil.find(target->GetFormID()); it != g_ignoreHitsUntil.end() && now < it->second) {
            return;  // the target perfect-blocked this hit
        }
        // one poise hit per attacker swing (enchantments and multi-hit events)
        auto& last = g_lastHit[target->GetFormID()];
        if (last.attacker == attacker->GetFormID() && now - last.time < 0.08f) {
            return;
        }
        last = { attacker->GetFormID(), now };

        auto& st = Touch(target);
        if (now < st.brokenUntil) {
            return;  // still staggered from the last break
        }

        // guard break: blocked hit with (almost) no stamina left
        if (a_blocked && s->bGuardBreak) {
            auto* av = target->AsActorValueOwner();
            const float maxSt = av->GetPermanentActorValue(RE::ActorValue::kStamina);
            const float pct = maxSt > 0.0f ? av->GetActorValue(RE::ActorValue::kStamina) / maxSt * 100.0f : 100.0f;
            if (pct <= s->fGuardBreakStaminaPct) {
                Break(target, attacker, s->fGuardBreakMag, st, "guard break, no stamina");
                return;
            }
        }

        float dmg = hit.damage;
        if (a_power) dmg *= s->fPowerAttackMult;
        if (a_blocked) dmg *= s->fBlockedHitMult;
        if (target->IsPlayerRef()) dmg *= s->fNPCToPlayerMult;
        else if (attacker->IsPlayerRef()) dmg *= s->fPlayerToNPCMult;
        else dmg *= s->fNPCToNPCMult;
        if (dmg <= 0.0f) {
            return;
        }

        st.damage = CurrentDamage(st, st.max) + dmg;
        st.lastHit = now;

        if (st.damage >= st.max) {
            float mag;
            if (target->IsPlayerRef()) {
                mag = s->fBreakMagOnPlayer;
            } else {
                mag = (hit.cls == HitClass::kHeavy) ? s->fBreakMagHeavy : s->fBreakMagLight;
                if (a_power) mag += s->fBreakMagPowerBonus;
            }
            if (a_blocked && s->bGuardBreak) {
                mag = (std::max)(mag, s->fGuardBreakMag);
            }
            Break(target, attacker, mag, st, a_blocked ? "guard break" : "unbalance");
        } else if (s->bDebug) {
            PoiseLog(fmt::format("{} poise {:.0f}/{:.0f} (-{:.0f})", target->GetName(), st.max - st.damage, st.max, dmg));
        }
    }

    class HitSink : public RE::BSTEventSink<RE::TESHitEvent>
    {
    public:
        static HitSink* GetSingleton()
        {
            static HitSink s;
            return &s;
        }

        RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*) override
        {
            if (!a_event || !a_event->cause || !a_event->target) {
                return RE::BSEventNotifyControl::kContinue;
            }
            auto* attacker = a_event->cause->As<RE::Actor>();
            auto* target = a_event->target->As<RE::Actor>();
            if (!attacker || !target) {
                return RE::BSEventNotifyControl::kContinue;
            }
            using F = RE::TESHitEvent::Flag;
            const bool power = a_event->flags.any(F::kPowerAttack);
            const bool bash = a_event->flags.any(F::kBashAttack);
            const bool blocked = a_event->flags.any(F::kHitBlocked);
            const auto source = a_event->source;
            // process next frame so the timed block code has marked perfect blocks first
            SKSE::GetTaskInterface()->AddTask([ah = attacker->GetHandle(), th = target->GetHandle(), source, power, bash, blocked]() {
                ProcessHit(ah, th, source, power, bash, blocked);
            });
            return RE::BSEventNotifyControl::kContinue;
        }
    };

    void RegisterTrueHUD()
    {
        auto* s = PoiseSettings::GetSingleton();
        if (!g_trueHUD || !s->bTrueHUDBar || g_ownsSpecialBar) {
            return;
        }
        const auto handle = SKSE::GetPluginHandle();
        if (g_trueHUD->RequestSpecialResourceBarsControl(handle) != TRUEHUD_API::APIResult::OK) {
            logger::warn("[Poise] TrueHUD special bar is owned by another plugin; poise bar disabled");
            return;
        }
        g_trueHUD->RegisterSpecialResourceFunctions(handle, Poise::GetCurrent, Poise::GetMax, true, s->bTrueHUDPlayerBar);
        g_ownsSpecialBar = true;
        logger::info("[Poise] TrueHUD poise bar registered (player bar: {})", s->bTrueHUDPlayerBar);
    }

    void ReleaseTrueHUD()
    {
        if (g_trueHUD && g_ownsSpecialBar) {
            g_trueHUD->ReleaseSpecialResourceBarControl(SKSE::GetPluginHandle());
            g_ownsSpecialBar = false;
        }
    }

    RE::Setting* GameSetting(const char* name)
    {
        auto* gsc = RE::GameSettingCollection::GetSingleton();
        return gsc ? gsc->GetSetting(name) : nullptr;
    }
}

namespace Poise
{
    void OnDataLoaded()
    {
        auto* s = PoiseSettings::GetSingleton();
        s->Load();

        g_raceMult.clear();
        auto* dh = RE::TESDataHandler::GetSingleton();
        for (const auto& [key, mult] : s->raceOverrides) {
            const auto bar = key.find('|');
            if (bar == std::string::npos || !dh) {
                logger::warn("[Poise] race override '{}' must look like Skyrim.esm|0x00012345", key);
                continue;
            }
            const std::string plugin = key.substr(0, bar);
            RE::FormID id = 0;
            try {
                id = static_cast<RE::FormID>(std::stoul(key.substr(bar + 1), nullptr, 16));
            } catch (...) {
                logger::warn("[Poise] bad form id in race override '{}'", key);
                continue;
            }
            if (auto* race = dh->LookupForm<RE::TESRace>(id & 0x00FFFFFF, plugin)) {
                g_raceMult[race->GetFormID()] = mult;
            } else {
                logger::warn("[Poise] race override '{}' not found", key);
            }
        }
        logger::info("[Poise] {} race overrides active", g_raceMult.size());

        if (auto* holder = RE::ScriptEventSourceHolder::GetSingleton()) {
            holder->AddEventSink<RE::TESHitEvent>(HitSink::GetSingleton());
        }

        if (auto* set = GameSetting("fCombatStaminaRegenRateMult")) {
            g_baseCombatRegen = set->data.f;
        }
        if (auto* set = GameSetting("fStaminaRegenDelay")) {
            g_baseRegenDelay = set->data.f;
        }
        logger::info("[Poise] base stamina GMSTs: combat regen {} / regen delay {}", g_baseCombatRegen, g_baseRegenDelay);
        ApplyStaminaSettings();
        RegisterTrueHUD();
    }

    void OnPostLoad()
    {
        g_trueHUD = reinterpret_cast<TRUEHUD_API::IVTrueHUD3*>(TRUEHUD_API::RequestPluginAPI(TRUEHUD_API::InterfaceVersion::V3));
        logger::info("[Poise] TrueHUD API {}", g_trueHUD ? "found" : "not found");
    }

    void OnGameLoaded()
    {
        std::lock_guard lock(g_lock);
        g_states.clear();
        g_ignoreHitsUntil.clear();
        g_lastHit.clear();
        ApplyStaminaSettings();
    }

    void Update(float a_delta)
    {
        if (a_delta <= 0.0f || a_delta > 1.0f) {
            return;
        }
        g_clock = g_clock.load() + a_delta;
        g_pruneTimer += a_delta;
        if (g_pruneTimer < 10.0f) {
            return;
        }
        g_pruneTimer = 0.0f;
        std::lock_guard lock(g_lock);
        const float now = Now();
        std::erase_if(g_states, [now](const auto& kv) { return now - kv.second.lastTouched > 60.0f && now > kv.second.brokenUntil; });
        std::erase_if(g_ignoreHitsUntil, [now](const auto& kv) { return now > kv.second; });
        std::erase_if(g_lastHit, [now](const auto& kv) { return now - kv.second.time > 5.0f; });
    }

    void OnExternalStagger(RE::Actor* a_actor)
    {
        auto* s = PoiseSettings::GetSingleton();
        if (!a_actor || !s->bEnablePoise || !s->bPerfectBlockBreaksPoise) {
            return;
        }
        std::lock_guard lock(g_lock);
        auto& st = Touch(a_actor);
        MarkBroken(a_actor, st);
        PoiseLog(fmt::format("{} poise broken (perfect block)", a_actor->GetName()));
    }

    void NotePerfectBlock(RE::Actor* a_defender)
    {
        if (!a_defender) {
            return;
        }
        std::lock_guard lock(g_lock);
        g_ignoreHitsUntil[a_defender->GetFormID()] = Now() + 0.4f;
    }

    bool ShouldSuppressVanillaStagger(RE::Actor* a_target)
    {
        auto* s = PoiseSettings::GetSingleton();
        return a_target && s->bEnablePoise && s->bSuppressVanillaStagger;
    }

    float GetMax(RE::Actor* a_actor)
    {
        if (!a_actor) {
            return 1.0f;
        }
        std::lock_guard lock(g_lock);
        if (auto it = g_states.find(a_actor->GetFormID()); it != g_states.end() && it->second.max > 0.0f) {
            return it->second.max;
        }
        return a_actor->IsPlayerRef() ? PoiseSettings::GetSingleton()->fPlayerPoise : PoiseSettings::GetSingleton()->fBasePoise;
    }

    float GetCurrent(RE::Actor* a_actor)
    {
        if (!a_actor) {
            return 0.0f;
        }
        std::lock_guard lock(g_lock);
        auto* s = PoiseSettings::GetSingleton();
        auto it = g_states.find(a_actor->GetFormID());
        if (it == g_states.end() || it->second.max <= 0.0f) {
            return a_actor->IsPlayerRef() ? s->fPlayerPoise : s->fBasePoise;
        }
        const auto& st = it->second;
        if (Now() < st.brokenUntil) {
            return 0.0f;
        }
        return (std::max)(0.0f, st.max - CurrentDamage(st, st.max));
    }

    void ApplyStaminaSettings()
    {
        auto* s = PoiseSettings::GetSingleton();
        if (g_baseCombatRegen >= 0.0f) {
            if (auto* set = GameSetting("fCombatStaminaRegenRateMult")) {
                set->data.f = s->bStaminaAddon ? g_baseCombatRegen * s->fCombatRegenMult : g_baseCombatRegen;
            }
        }
        if (g_baseRegenDelay >= 0.0f) {
            if (auto* set = GameSetting("fStaminaRegenDelay")) {
                set->data.f = s->bStaminaAddon ? g_baseRegenDelay * s->fRegenDelayMult : g_baseRegenDelay;
            }
        }
    }

    // ========================================================================
    // Menu (SKSE Menu Framework)
    // ========================================================================
    namespace
    {
        bool g_dirty{ false };

        void Tip(const char* text)
        {
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", text);
            }
        }
        void Check(const char* label, bool* v, const char* tip)
        {
            if (ImGui::Checkbox(label, v)) g_dirty = true;
            Tip(tip);
        }
        void Slider(const char* label, float* v, float lo, float hi, const char* fmtStr, const char* tip)
        {
            if (ImGui::SliderFloat(label, v, lo, hi, fmtStr)) g_dirty = true;
            Tip(tip);
        }
    }

    void __stdcall RenderMenu()
    {
        auto* s = PoiseSettings::GetSingleton();

        ImGui::Text("Elden Combat - Poise & Stamina");
        if (g_dirty) {
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "(Unsaved to INI)");
        }
        if (ImGui::Button("Save to INI##poise")) {
            s->Save();
            g_dirty = false;
            RE::DebugNotification("Elden Combat: poise settings saved");
        }
        ImGui::SameLine();
        if (ImGui::Button("Reload INI##poise")) {
            s->Load();
            ApplyStaminaSettings();
            g_dirty = false;
        }
        ImGui::Separator();

        if (ImGui::BeginTabBar("PoisePages")) {
            if (ImGui::BeginTabItem("Poise")) {
                Check("Enable Poise##p", &s->bEnablePoise, "Master toggle for the poise (unbalance) system.");
                Slider("NPC Poise##p", &s->fBasePoise, 10.0f, 500.0f, "%.0f", "Poise of a human-sized NPC.\nDefault 100: 1H weapons break it in 3 hits, 2H in 2.");
                Slider("Player Poise##p", &s->fPlayerPoise, 10.0f, 500.0f, "%.0f", "Your own poise.");
                Slider("Creature Size Scaling##p", &s->fCreatureSizeScaling, 0.0f, 2.0f, "%.2f", "How much bigger creatures gain extra poise from their height.\n0 = every creature uses NPC poise, 1 = a creature twice as tall has twice the poise.");
                Slider("Min Size Mult##p", &s->fMinSizeMult, 0.1f, 1.0f, "%.2f", "Lowest poise multiplier for small creatures.");
                Slider("Max Size Mult##p", &s->fMaxSizeMult, 1.0f, 10.0f, "%.1f", "Highest poise multiplier for giants, dragons etc.");
                Slider("Poise per Armor Weight##p", &s->fArmorPoisePerWeight, 0.0f, 2.0f, "%.2f", "Extra poise per unit of worn armor weight.\n0 = armor doesn't matter (Elden Rim behaviour).");
                Slider("Refill Delay (s)##p", &s->fRegenDelay, 0.0f, 10.0f, "%.1f s", "Seconds without taking poise damage before poise starts to refill.");
                Slider("Refill Speed (%/s)##p", &s->fRegenPercentPerSec, 1.0f, 200.0f, "%.0f %%", "Percent of max poise refilled per second.");
                Slider("Break Immunity (s)##p", &s->fBreakImmunity, 0.0f, 5.0f, "%.1f s", "After a break, poise can't be damaged for this long, then refills to full.");
                Check("Only Stagger Through Poise##p", &s->bSuppressVanillaStagger, "Cancel the game's own melee hit staggers so only poise breaks stagger.\nNeeds Precision.");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Damage")) {
                ImGui::TextDisabled("Poise damage per hit");
                Slider("Unarmed##d", &s->fDmgUnarmed, 0.0f, 150.0f, "%.0f", "Fists.");
                Slider("Dagger##d", &s->fDmgDagger, 0.0f, 150.0f, "%.0f", "Daggers.");
                Slider("Sword##d", &s->fDmgSword, 0.0f, 150.0f, "%.0f", "One-handed swords.");
                Slider("War Axe##d", &s->fDmgWarAxe, 0.0f, 150.0f, "%.0f", "War axes.");
                Slider("Mace##d", &s->fDmgMace, 0.0f, 150.0f, "%.0f", "Maces.");
                Slider("Greatsword##d", &s->fDmgGreatsword, 0.0f, 150.0f, "%.0f", "Greatswords.");
                Slider("Battleaxe / Warhammer##d", &s->fDmgBattleaxe, 0.0f, 150.0f, "%.0f", "Battleaxes and warhammers.");
                Slider("Bow##d", &s->fDmgBow, 0.0f, 150.0f, "%.0f", "Arrows.");
                Slider("Crossbow##d", &s->fDmgCrossbow, 0.0f, 150.0f, "%.0f", "Bolts.");
                Slider("Staff##d", &s->fDmgStaff, 0.0f, 150.0f, "%.0f", "Staff hits.");
                Slider("Creature##d", &s->fDmgCreature, 0.0f, 150.0f, "%.0f", "Creature claws and bites, scaled by the creature's size.");
                Slider("Shield Bash##d", &s->fDmgBash, 0.0f, 150.0f, "%.0f", "Shield and weapon bashes.");
                Slider("Spell##d", &s->fDmgSpell, 0.0f, 150.0f, "%.0f", "Per spell hit. 0 = spells don't damage poise.");
                ImGui::Separator();
                Slider("Power Attack Mult##d", &s->fPowerAttackMult, 1.0f, 5.0f, "x%.2f", "Poise damage multiplier for power attacks.");
                Slider("Blocked Hit Mult##d", &s->fBlockedHitMult, 0.0f, 1.0f, "x%.2f", "Poise damage multiplier when the target blocks the hit.");
                Slider("Player -> NPC##d", &s->fPlayerToNPCMult, 0.0f, 3.0f, "x%.2f", "Your hits on enemies.");
                Slider("NPC -> Player##d", &s->fNPCToPlayerMult, 0.0f, 3.0f, "x%.2f", "Enemy hits on you (Elden Rim uses about 0.3 - 0.75).");
                Slider("NPC -> NPC##d", &s->fNPCToNPCMult, 0.0f, 3.0f, "x%.2f", "Followers vs enemies.");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Break")) {
                ImGui::TextDisabled("Stagger size when poise breaks (~0.25 small, 0.5 medium, 0.75 large, 1.0+ largest)");
                Slider("Light Break##b", &s->fBreakMagLight, 0.0f, 2.0f, "%.2f", "Break by unarmed, 1H, ranged or spell.");
                Slider("Heavy Break##b", &s->fBreakMagHeavy, 0.0f, 2.0f, "%.2f", "Break by 2H weapons or creatures.");
                Slider("Power Attack Bonus##b", &s->fBreakMagPowerBonus, 0.0f, 1.0f, "+%.2f", "Added when a power attack breaks poise.");
                Slider("Break on Player##b", &s->fBreakMagOnPlayer, 0.0f, 2.0f, "%.2f", "Stagger size when enemies break your poise.");
                ImGui::Separator();
                Check("Guard Break##b", &s->bGuardBreak, "A blocking target whose poise breaks is guard-broken\n(block-broken animation and sound from the Elden Rim stagger pack).");
                Slider("Guard Break Stamina (%)##b", &s->fGuardBreakStaminaPct, 0.0f, 50.0f, "%.0f %%", "A blocked hit at or below this stamina instantly guard-breaks.");
                Slider("Guard Break Size##b", &s->fGuardBreakMag, 0.0f, 2.0f, "%.2f", "Minimum stagger size for guard breaks.");
                Check("Perfect Block Breaks Poise##b", &s->bPerfectBlockBreaksPoise, "Perfect block and ward parry staggers also empty the attacker's poise,\nso there is only one stagger and one sound.");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("TrueHUD")) {
                bool bar = s->bTrueHUDBar;
                if (ImGui::Checkbox("Poise Bar##t", &bar)) {
                    s->bTrueHUDBar = bar;
                    g_dirty = true;
                    if (bar) RegisterTrueHUD(); else ReleaseTrueHUD();
                }
                Tip("Show poise as TrueHUD's special bar on enemy info bars and boss bars.\nTrueHUD's special bar must be enabled in TrueHUD's own settings.");
                Check("Player Poise Bar##t", &s->bTrueHUDPlayerBar, "Also show your own poise under your health/stamina bars.\nTakes effect after a game restart.");
                Check("Flash on Break##t", &s->bFlashOnBreak, "Flash the poise bar when it breaks.");
                ImGui::Text("TrueHUD: %s", g_trueHUD ? (g_ownsSpecialBar ? "poise bar active" : "found, bar not active") : "not installed");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Stamina")) {
                bool on = s->bStaminaAddon;
                if (ImGui::Checkbox("Stamina Add-on##s", &on)) {
                    s->bStaminaAddon = on;
                    g_dirty = true;
                    ApplyStaminaSettings();
                }
                Tip("Valhalla-style stamina boost. Off = the game's own stamina rules.");
                if (ImGui::SliderFloat("Combat Regen Mult##s", &s->fCombatRegenMult, 0.5f, 5.0f, "x%.2f")) {
                    g_dirty = true;
                    ApplyStaminaSettings();
                }
                Tip("Multiplies how fast stamina refills during combat.");
                if (ImGui::SliderFloat("Regen Delay Mult##s", &s->fRegenDelayMult, 0.1f, 2.0f, "x%.2f")) {
                    g_dirty = true;
                    ApplyStaminaSettings();
                }
                Tip("Multiplies the pause before stamina starts refilling after you use it.\nLower = refills sooner.");
                Slider("Light Hit Restore (%)##s", &s->fLightHitRestorePct, 0.0f, 30.0f, "%.1f %%", "Stamina restored (percent of max) when a light melee attack lands.");
                Check("For Player##s", &s->bLightHitRestorePlayer, "Light-hit stamina restore for you.");
                Check("For NPCs##s", &s->bLightHitRestoreNPC, "Light-hit stamina restore for NPCs.");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Debug")) {
                Check("Debug Notifications##dbg", &s->bDebug, "Show poise damage and breaks on screen and in EldenCombat.log.");
                if (auto* player = RE::PlayerCharacter::GetSingleton()) {
                    ImGui::Text("Player poise: %.0f / %.0f", GetCurrent(player), GetMax(player));
                }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
}
