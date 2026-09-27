# Crypto Kombat — Browser version

Self-contained **HTML5 Canvas** local 1v1 fighter. No build step, no Unreal dependency.

Parody names only. Not affiliated with NetherRealm, Midway, or any cryptocurrency project or person.

## Play

**Option A — open the file**

1. Clone the repo
2. Open `web/index.html` in a modern desktop browser (Chrome / Firefox / Edge / Safari)

**Option B — tiny static server**

```bash
# from repo root
npx --yes serve web

# or
cd web && python3 -m http.server 8080
```

Then visit the printed URL (e.g. `http://localhost:8080`).

Desktop keyboard is required for v1 (no on-screen buttons).

## Flow

1. **Character select** — P1 uses WASD to highlight a fighter; P2 uses arrow keys. Press **Enter** to start. Default picks: Vital Spark vs Charles Epoch.
2. **Fight** — best of 3, 99s rounds, Moon Meter specials.
3. **Match over** — **R** rematch · **Esc** back to select.

## Controls

### Player 1

| Action | Key |
|---|---|
| Move | **A** / **D** |
| Jump | **W** |
| Block | **Left Shift** |
| Light Punch | **J** |
| Heavy Punch | **U** |
| Light Kick | **K** |
| Heavy Kick | **I** |
| Special 1 | **L** |
| Special 2 | **O** |

### Player 2 (browser-friendly; numpad optional)

| Action | Key |
|---|---|
| Move | **←** / **→** |
| Jump | **↑** |
| Block | **Ctrl** |
| Light Punch | **1** (or Numpad 1) |
| Light Kick | **2** (or Numpad 2) |
| Special 1 | **3** (or Numpad 3) |
| Heavy Punch | **4** (or Numpad 4) |
| Heavy Kick | **5** (or Numpad 5) |
| Special 2 | **6** (or Numpad 6) |

## Files

```
web/
  index.html    shell + canvas
  style.css     neon page chrome
  js/roster.js  fighter definitions + normals
  js/game.js    input, combat, render, match flow
  README.md     this file
```

## Roster (browser)

Vital Spark, Charles Epoch, Satoshi Shadow, CZ Chain, Sol Flash, Brian Coin, Tron Blaze, Arthur Perp.

Each has two Moon Meter specials (projectile, dash, uppercut, counter, armor, flurry, grab, splash, or drain).
