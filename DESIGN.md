# CryptoKombat — Design Document (Phase 1)

## Vision
A tongue-in-cheek **2.5D side-view fighter** in the spirit of classic arcade 1v1s: readable silhouettes, juicy hits, best-of-three rounds, and over-the-top finishers. The roster lampoons **crypto-founder archetypes** with **wholly original parody names** — never real trademarks, corporate logos, or photoreal likenesses.

Tone: arcade swagger + memespace absurdity, not defamation. Everyone is a cartoon.

## Pillars
1. **Readable** — locked side camera, big telegraphs, clear block/stun  
2. **Fair local versus** — same-keyboard first; netcode later  
3. **On-brand toys** — Moon Meter, Rugpull / Liquidation / HODL fantasy moves  
4. **Code-first scaffold** — C++ gameplay truths; Blueprints for presentation  

## Camera & space
- Orthographic (default) side view along stage **Y**, camera on **-X**  
- Fighters plane-constrained; jump on **Z**  
- `AFightCamera` keeps both fighters framed with padding  

## Match rules
- Best of 3 (`RoundsToWin = 2`)  
- 99-second round timer; higher health wins on timeout  
- KO → round; match ends when a player reaches 2 rounds  
- Phase 1: no team modes, no online  

## Combat loop
| System | Behavior |
|---|---|
| States | Idle, Walk, Jump, Attack, Hitstun, Block, Special, KO |
| Block | Hold block → 25% chip, small Moon gain |
| Moon Meter | Passive drip + hit/block rewards; spent on specials |
| Traces | Box sweeps on pawn channel; one hit per swing per target |
| Facing | Always toward opponent during fight phase |

## Roster

### 1. Satoshi Shadow — *The Anonymous Genesis*
- **Color:** warm orange  
- **Fantasy:** hooded whitepaper wraith; identity optional, fists mandatory  
- **Normals:** Hash Jab, Block Reward, Nonce Kick, Difficulty Adjustment  
- **Specials:** Genesis Pulse (beam), **HODL Counter** (parry → meter)  
- **Fatality — Rugpull Rift:** yank the floor; rival drops into a mempool abyss  
- **Input hint:** D, D, F, HP (near)

### 2. Vital Spark — *The Smart-Contract Sage*
- **Color:** ethereal blue  
- **Fantasy:** lanky compile-monk; gas-efficient footsies  
- **Normals:** Gas Jab, Opcode Smash, Shard Sweep, Beacon Kick  
- **Specials:** **Liquidation Laser**, Fork Flip (reposition)  
- **Fatality — Reentrancy Ruin:** recursive call stack overflow  
- **Input hint:** B, F, B, Special (near)

### 3. CZ Chain — *The Exchange Enforcer*
- **Color:** vault gold  
- **Fantasy:** vault-door frame; short sentences, long reach  
- **Normals:** Listing Jab, Cold Wallet Crush, Fee Kick, Margin Call  
- **Specials:** Withdrawal Lock (freeze grab), SAFU Shield (armor advance)  
- **Fatality — Total Liquidation:** mark-to-market to zero → candle shatter  
- **Input hint:** F, F, D, HK (near)

### 4. Brian Coin — *The Meme-Market Maverick*
- **Color:** neon green  
- **Fantasy:** volatility surfer; vibes-first rushdown  
- **Normals:** Pump Jab, Volume Punch, Dip Kick, Moon Kick  
- **Specials:** **To The Moon** (launcher), Whale Splash  
- **Fatality — Exit Liquidity:** mid-air dump; red-chart crush  
- **Input hint:** D, F, D, Special (near)

## Art direction
- Stylized low-poly / toon; exaggerated proportions  
- Stage: “Crypto Pit” — neon candles, floating order-book glyphs (original art)  
- No real exchange UIs, coin logos, or celebrity scans  
- Phase 1 placeholders: tinted capsules + primitive props (see `Content/README.md`)  

## Audio (future)
- Heavy hit stops, vinyl-scratch KO stingers, chiptune “Finish Them!” sting  
- Announcer lines: “Liquidity locked!”, “Get rekt!”, “HODL!”  

## Roadmap beyond Phase 1
- AnimBP + montage-driven cancel windows  
- Combo counter UI / WBP_FightHUD  
- Fatality cinematic sequencer  
- Two more roster members + stage select  
- Rollback netcode exploration  

## Legal posture
Parody / satire characters only. If a name drifts too close to a living mark, rename before any public build.
