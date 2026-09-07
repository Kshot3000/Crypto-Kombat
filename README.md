# CryptoKombat

**Mortal Kombat–style 2.5D / side-view 1v1 fighter** themed around stylized crypto-founder archetypes — with **original parody names only** (no real trademarks, logos, or photoreal likenesses).

| | |
|---|---|
| Engine | **Unreal Engine 5.4+** (`EngineAssociation: 5.4`) |
| Language | C++ (Phase 1 scaffold) + Blueprints for content |
| Mode | Local same-keyboard versus (P1 / P2) |
| Repo | https://github.com/Kshot3000/Crypto-Kombat |

> Not affiliated with NetherRealm, Midway, or any cryptocurrency project/person. Pure parody arcade vibes.

## Quick start

### 1. Prerequisites
- Unreal Engine **5.4** or newer
- Visual Studio 2022 (Windows) / Xcode (Mac) / suitable C++ toolchain (Linux)
- Git

### 2. Generate project files
```bash
# Windows example — adjust Engine path
"C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="X:\Crypto-Kombat\CryptoKombat.uproject" -game -engine

# Or: right-click CryptoKombat.uproject → Generate Visual Studio project files
```

### 3. Build
Open the generated `.sln` / compile from Rider / build editor target:
```text
CryptoKombatEditor (Development Editor)
```

### 4. Open & play
1. Launch `CryptoKombat.uproject` in the UE5.4 Editor  
2. Create a simple side-view map (or use the default OpenWorld stub)  
3. Place floor collision; set Game Mode Override to `FightGameMode` / `BP_FightGameMode`  
4. PIE (Play In Editor) — GameMode spawns two `ACryptoFighter`s + `AFightCamera`  
5. See **CONTROLS.md** for the keyboard layout  

Until Blueprints exist, C++ classes run with capsule characters and debug hit boxes.

## Project layout

```
CryptoKombat.uproject
Config/                 DefaultEngine / DefaultGame / DefaultInput
Content/README.md       Where Blueprints & placeholders go
Source/CryptoKombat/
  Characters/           ACryptoFighter
  Game/                 AFightGameMode, AFightGameState
  Camera/               AFightCamera
  Data/                 FighterTypes + roster factory (10 fighters)
  Input/                Enhanced Input documentation stub
DESIGN.md               Vision, roster, moves, art direction
CONTROLS.md             P1 / P2 keyboard layout
```

## Roster (10 fighters)

| ID | Name | Tagline |
|---|---|---|
| `SatoshiShadow` | Satoshi Shadow | The Anonymous Genesis |
| `VitalSpark` | Vital Spark | The Smart-Contract Sage |
| `CZChain` | CZ Chain | The Exchange Enforcer |
| `BrianCoin` | Brian Coin | The Meme-Market Maverick |
| `SolFlash` | Sol Flash | The Parallel Striker |
| `DotWeaver` | Dot Weaver | The Interchain Architect |
| `HaydenSwap` | Hayden Swap | The Liquidity Ghost |
| `TronBlaze` | Tron Blaze | The Showman Chain |
| `ArthurPerp` | Arthur Perp | The Leverage King |
| `AdaScholar` | Ada Scholar | The Peer-Reviewed Pugilist |

Full bios, normals, specials, and fatality concepts → **DESIGN.md**.

## Features in this scaffold
- Health, block (chip), hitstun, KO  
- Moon Meter (builds over time / on hit; spends on specials)  
- Box-sweep attack traces  
- Best-of-3 rounds + 99s timer  
- Locked orthographic side-view camera framing both fighters  
- Enhanced Input hooks + classic `DefaultInput.ini` fallback for 2-player keyboard  

## License / parody note
Original code and names © project contributors. Do not ship real logos, trademarks, or lookalike celebrity meshes.
