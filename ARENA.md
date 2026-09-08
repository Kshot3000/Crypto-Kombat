# Blockchain Colosseum — Arena Guide

## Vision
**Blockchain Colosseum** is CryptoKombat’s starter 1v1 stage: a neon crypto pit framed by soft side walls, a raised **mempool** rim in the center, stacked “block” pillars, and floating candle-chart panels. Accent palette is **neon cyan + magenta** against dark floor / backdrop slabs — arcade swagger, no real exchange UIs or coin logos.

Tone: readable silhouette stage for side-view fights, not a photoreal city.

## Why procedural C++?
This Phase 1 repo ships **no `.umap` binary assets** (Editor may not be available in CI / clone-and-read workflows). `ABlockchainColosseum` builds the stage at **Construction / BeginPlay** from `/Engine/BasicShapes/Cube` so **PIE on any map** still looks like an arena when `AFightGameMode` auto-spawns it.

## How the builder works
| Piece | Implementation |
|---|---|
| Class | `Source/CryptoKombat/Arena/BlockchainColosseum.h/.cpp` — `AActor` |
| Entry | `BuildArena()` clears prior meshes, then spawns `UStaticMeshComponent`s |
| Floor | Dark wide Y / thin Z slab with collision + neon edge strips |
| Soft walls | Left/right cubes near ±`ArenaHalfWidth` (default 600) |
| Backdrop | Dark + cyan/magenta accent panels behind +X |
| Pillars | Short “block” stacks at front corners |
| Mempool rim | Four low collision boxes around center + tinted pit plane |
| Candle charts | Thin floating boxes at mid height |
| Center strip | Bright decal-style plane (logo stand-in) |
| Colors | Brighter `AccentCyan` / `AccentMagenta`, darker floor/walls, overhead light boxes, backdrop ribs, corner pylons |
| Spawns | `GetP1Spawn()` / `GetP2Spawn()` → ±`SpawnHalfSeparation` on Y, `FighterSpawnZ` |

Materials use `CreateDynamicMaterialInstance` on `BasicShapeMaterial` and set `Color` / `BaseColor` vector parameters when available.

### GameMode wiring
`AFightGameMode`:
- `TSubclassOf<ABlockchainColosseum> ArenaClass` (defaults to `ABlockchainColosseum`)
- `bool bAutoSpawnArena = true`
- On match start: if no colosseum exists in the world, spawn one at origin **before** fighters
- Uses arena spawn points for P1 / P2 when an arena is present

Default featured matchup: **Vital Spark** vs **Charles Epoch**.

## Replacing with a real UE map later
When you have the Editor and art pipeline:

1. Create `Content/Maps/Arena_BlockchainColosseum` (side-view stage along **Y**, camera on **-X**).
2. Optionally place a Blueprint child of `ABlockchainColosseum` for spawn markers only, **or** delete procedural meshes and keep empty spawn helpers.
3. Set **Game Mode Override** → `FightGameMode` / `BP_FightGameMode`.
4. Either:
   - Leave `bAutoSpawnArena = true` and place nothing (procedural still works), or
   - Place your authored geometry and set `bAutoSpawnArena = false`, then set `P1SpawnLocation` / `P2SpawnLocation` (or keep a lightweight `ABlockchainColosseum` for spawn queries only).
5. Migrate props (pillars, charts, neon) into static meshes / Nanite as desired; keep soft wall collision volumes at ~Y ±600.

Suggested Content paths:
```
Content/Maps/Arena_BlockchainColosseum
Content/Blueprints/BP_BlockchainColosseum   (optional child for tuning)
```

See also `Content/README.md` and the **Arenas** section in `DESIGN.md`.
