# Elden Combat (private)

Personal SKSE plugin for my LoreRim setup. **Private - do not publish or redistribute.**
Based on [Timed Block Dodge and Counter](https://github.com/DCCStudios/TimedBlockDodgeCounter) by DCCStudios
(no license = all rights reserved by its author), with:

- Elden Rim-style poise ("unbalance") as a meter, shown on TrueHUD's special bar (enemies, bosses, player)
- guard break on blocking targets, perfect block empties the attacker's poise (one stagger, one sound)
- slow-motion toggles for perfect block and perfect dodge, adjustable windows
- Valhalla-style stamina add-on
- all settings in SKSE Menu Framework: Elden Combat > "Perfect Block & Dodge" / "Poise & Stamina"

Sounds in `package/` come from SUKI's SFX replacer (perfect block) and Elden Rim (perfect dodge).

Requires: SKSE, Address Library, SKSE Menu Framework, Precision, TrueHUD, TK Dodge RE,
Open Animation Replacer + Modern Stagger Lock (+ the "EldenRim Stagger Sounds NPC" OAR pack for animations/sounds).

Build: GitHub Actions (`.github/workflows/build.yml`) -> artifact `EldenCombat` is the ready MO2 mod folder.
