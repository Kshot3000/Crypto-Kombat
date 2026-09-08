# Content Placeholders

This Phase 1 scaffold ships **no binary `.uasset` files** so the repo stays small and code-first.

## Procedural visuals (no assets required)

PIE already looks like a stylish prototype via C++:

| System | Class | What you see |
|---|---|---|
| Fighters | `UFighterVisuals` on `ACryptoFighter` | Accent-tinted BasicShapes body (torso/head/arms/legs); attack arm squash |
| Hits | `AHitSpark` | Short colored sphere burst at impact |
| HUD | `AFightHUD` | Canvas neon health / Moon / timer / score |
| Arena | `ABlockchainColosseum` | Dark floor, cyan/magenta neon, overhead light boxes |

Default skeletal mesh is hidden; collision stays on the Character capsule. See **ART.md** for the mesh / VFX roadmap.

## What to create in the Editor

| Path (suggested) | Purpose |
|---|---|
| `Content/Maps/Arena_BlockchainColosseum` | Authored side-view stage (future; replace procedural builder) |
| `Content/Blueprints/BP_BlockchainColosseum` | Optional BP child for arena tuning |
| `Content/Blueprints/BP_CryptoFighter` | Child of `ACryptoFighter` — mesh, anim BP, Input |
| `Content/Blueprints/BP_FightGameMode` | Child of `AFightGameMode` |
| `Content/Blueprints/BP_FightCamera` | Child of `AFightCamera` |
| `Content/Input/IMC_Fight` | Enhanced Input Mapping Context (see `CONTROLS.md`) |
| `Content/Input/IA_*` | Actions: Move, Jump, Punch, Kick, Block, Special, Fatality |
| `Content/Data/DT_FighterRoster` | Optional DataTable wrapping `FFighterDefinition` |
| `Content/UI/WBP_FightHUD` | Optional UMG mirror of `AFightHUD` |

## Placeholder art (upgrade path)

Until Mixamo / MetaHuman meshes exist:

- Procedural **block fighters** stand in for characters
- **Box** traces already drive hit detection in C++
- Solid / BasicShape materials tinted per roster `AccentColor`

Blueprints should live under `Content/Blueprints/` and reference the C++ classes in `Source/CryptoKombat/`.

## Procedural arena (no .umap yet)

`ABlockchainColosseum` builds floor, soft walls, backdrop, pillars, mempool rim, neon strips, overhead lights, and candle panels from engine cubes at Construction / BeginPlay. `AFightGameMode` auto-spawns it when none exists in the world (`bAutoSpawnArena`).

When you author a real map, prefer `Content/Maps/Arena_BlockchainColosseum` and follow the swap steps in **ARENA.md**. Until then, PIE on any map still looks like the Colosseum.
