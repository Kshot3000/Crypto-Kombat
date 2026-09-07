# CryptoKombat — Controls (Local Versus)

Same keyboard, two players. Bindings live in `Config/DefaultInput.ini` (classic Action/Axis).  
When you author Enhanced Input assets (`IMC_Fight`, `IA_*`), mirror this layout and assign per-player Mapping Contexts.

## Player 1 (left side)

| Action | Key |
|---|---|
| Move left / right | **A** / **D** |
| Jump | **W** |
| Block (hold) | **Left Shift** |
| Light Punch | **J** |
| Heavy Punch | **U** |
| Light Kick | **K** |
| Heavy Kick | **I** |
| Special 1 | **L** |
| Special 2 | **O** |

## Player 2 (right side)

| Action | Key |
|---|---|
| Move left / right | **←** / **→** |
| Jump | **↑** |
| Block (hold) | **Right Ctrl** |
| Light Punch | **Numpad 1** |
| Heavy Punch | **Numpad 4** |
| Light Kick | **Numpad 2** |
| Heavy Kick | **Numpad 5** |
| Special 1 | **Numpad 3** |
| Special 2 | **Numpad 6** |

## Tips
- Face direction auto-updates toward the opponent (`AFightGameMode::UpdateFacing`).
- Specials require **Moon Meter** (fills slowly + on hit / block).
- Fatality input strings are documented per fighter in **DESIGN.md** (presentation-only in Phase 1).
- Controllers: map Gamepad face buttons to the same logical actions in EI when ready.

## Enhanced Input asset checklist
Create under `Content/Input/`:

1. `IMC_Fight_P1` / `IMC_Fight_P2`  
2. `IA_Move` (Axis1D), `IA_Jump`, `IA_PunchLight`, `IA_PunchHeavy`, `IA_KickLight`, `IA_KickHeavy`, `IA_Special1`, `IA_Special2`, `IA_Block`  
3. Assign on `BP_CryptoFighter` → `MappingContext` + action properties  
