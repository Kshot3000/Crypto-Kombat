# Content Placeholders

This Phase 1 scaffold ships **no binary `.uasset` files** so the repo stays small and code-first.

## What to create in the Editor

| Path (suggested) | Purpose |
|---|---|
| `Content/Maps/Arena_CryptoPit` | Side-view 1v1 stage (planes + capsules) |
| `Content/Blueprints/BP_CryptoFighter` | Child of `ACryptoFighter` — mesh, anim BP, Input |
| `Content/Blueprints/BP_FightGameMode` | Child of `AFightGameMode` |
| `Content/Blueprints/BP_FightCamera` | Child of `AFightCamera` |
| `Content/Input/IMC_Fight` | Enhanced Input Mapping Context (see `CONTROLS.md`) |
| `Content/Input/IA_*` | Actions: Move, Jump, Punch, Kick, Block, Special, Fatality |
| `Content/Data/DT_FighterRoster` | Optional DataTable wrapping `FFighterDefinition` |
| `Content/UI/WBP_FightHUD` | Health bars, Moon Meter, round timer |

## Placeholder art

Until real meshes exist, use:

- **Capsule** or **Cylinder** for fighters (scale ~88uu tall)
- **Box** traces already drive hit detection in C++
- Solid materials tinted per roster color (see `DESIGN.md`)

Blueprints should live under `Content/Blueprints/` and reference the C++ classes in `Source/CryptoKombat/`.
