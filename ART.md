# Art Direction — CryptoKombat

**Tone:** Mortal Kombat arcade energy × crypto neon — parody archetypes, never photoreal celebrity likenesses or real trademarks.

## Phase 1 (now) — stylized procedural graybox

| Layer | Approach |
|---|---|
| Fighters | `UFighterVisuals` — BasicShapes torso / head / arms / legs under the capsule; MID tint from `AccentColor`; light proportion variance by `FighterId` |
| Hit FX | `AHitSpark` — short-lived colored sphere burst (~0.25s) on successful traces |
| Arena | `ABlockchainColosseum` — dark floor, bright cyan/magenta neon strips, overhead light boxes, candle panels |
| HUD | `AFightHUD` — Canvas neon bars (health, Moon Meter, timer, score) — no UMG assets required |

Goal of this pass: **PIE reads as a stylish prototype**, not empty capsules on a bare floor. Collision and combat stay on the Character capsule / box traces.

## Near-term polish

- Attack pose squash → simple idle bob / block guard pose
- Niagara or sprite sheets for sparks, dust, Moon Meter flare
- Arena emissive materials (custom M_Neon) once Content is allowed
- Optional `WBP_FightHUD` mirroring Canvas layout for designer iteration

## Mid-term — readable fighters

1. **Mixamo / Epic mannequin** retarget into a shared AnimBP (idle, walk, punch, kick, hit, KO)
2. Stylized materials (toon rim + accent glow) — still parody silhouettes, not lookalikes
3. Per-fighter accessories as mesh children (hoodie, cape, shades) keyed off `FighterId`

## Later — MetaHuman / hero art

- MetaHumans or custom sculpts only with **clear parody styling** (exaggerated proportions, neon warpaint, crypto motif props)
- No real logos, ticket symbols, or celebrity face scans
- Fatality cinematics as Level Sequences once finishers are playable

## VFX roadmap

| Priority | Effect |
|---|---|
| P0 | Hit sparks (done), block chip flash |
| P1 | Special projectile trails, Moon Meter full pulse |
| P2 | Stage ambience (mempool particles, chart flicker) |
| P3 | Fatality gore-adjacent comedy FX (blocks dissolving, rugpull floor) |

## HUD goals

- Always-on: names, health, Moon Meter, timer, round pips
- Banners: READY / FIGHT! / ROUND OVER / MATCH OVER / FINISH THEM
- Accessibility: high-contrast fills, outlined text, colorblind-safe secondary markers later

## Do / Don't

- **Do** keep silhouettes readable in side-view orthographic framing  
- **Do** push neon vs dark floor contrast  
- **Don't** ship real brand marks or photoreal founder faces  
- **Don't** break capsule collision when swapping visuals  
