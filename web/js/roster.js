/** Crypto Kombat roster — parody names only. Accent colors as CSS hex. */
window.CK_ROSTER = [
  {
    id: "VitalSpark",
    name: "Vital Spark",
    tagline: "The Ethereum Architect",
    color: "#8c59f2",
    maxHealth: 950,
    walkSpeed: 280,
    jumpPower: 520,
    width: 48,
    height: 110,
    specials: [
      { name: "Proof-of-Stake Slam", cost: 30, kind: "uppercut", damage: 125, hitstun: 0.52, reach: 70 },
      { name: "Danksharding Dash", cost: 22, kind: "dash", damage: 55, hitstun: 0.28, reach: 160 }
    ]
  },
  {
    id: "CharlesEpoch",
    name: "Charles Epoch",
    tagline: "The Formal Methods Firebrand",
    color: "#2673d9",
    maxHealth: 1100,
    walkSpeed: 220,
    jumpPower: 480,
    width: 56,
    height: 118,
    specials: [
      { name: "Formal Proof Guard", cost: 20, kind: "counter", damage: 90, hitstun: 0.45, reach: 50 },
      { name: "Ouroboros Orbit", cost: 25, kind: "sweep", damage: 95, hitstun: 0.5, reach: 110 }
    ]
  },
  {
    id: "SatoshiShadow",
    name: "Satoshi Shadow",
    tagline: "The Anonymous Genesis",
    color: "#f28c1a",
    maxHealth: 1000,
    walkSpeed: 250,
    jumpPower: 500,
    width: 50,
    height: 112,
    specials: [
      { name: "Genesis Pulse", cost: 25, kind: "projectile", damage: 120, hitstun: 0.55, reach: 0 },
      { name: "HODL Counter", cost: 15, kind: "counter", damage: 70, hitstun: 0.35, reach: 45 }
    ]
  },
  {
    id: "CZChain",
    name: "CZ Chain",
    tagline: "The Exchange Enforcer",
    color: "#f2d933",
    maxHealth: 1100,
    walkSpeed: 210,
    jumpPower: 460,
    width: 58,
    height: 120,
    specials: [
      { name: "Withdrawal Lock", cost: 25, kind: "grab", damage: 100, hitstun: 0.7, reach: 55 },
      { name: "SAFU Shield", cost: 20, kind: "armor", damage: 50, hitstun: 0.35, reach: 70 }
    ]
  },
  {
    id: "SolFlash",
    name: "Sol Flash",
    tagline: "The Parallel Striker",
    color: "#8c40f2",
    maxHealth: 900,
    walkSpeed: 320,
    jumpPower: 540,
    width: 46,
    height: 106,
    specials: [
      { name: "Slot Storm", cost: 20, kind: "flurry", damage: 95, hitstun: 0.4, reach: 90 },
      { name: "Priority Fee Rush", cost: 25, kind: "dash", damage: 70, hitstun: 0.28, reach: 150 }
    ]
  },
  {
    id: "BrianCoin",
    name: "Brian Coin",
    tagline: "The Meme-Market Maverick",
    color: "#33e68c",
    maxHealth: 980,
    walkSpeed: 300,
    jumpPower: 560,
    width: 50,
    height: 110,
    specials: [
      { name: "To The Moon", cost: 25, kind: "uppercut", damage: 110, hitstun: 0.6, reach: 65 },
      { name: "Whale Splash", cost: 35, kind: "splash", damage: 90, hitstun: 0.45, reach: 140 }
    ]
  },
  {
    id: "TronBlaze",
    name: "Tron Blaze",
    tagline: "The Showman Chain",
    color: "#f22633",
    maxHealth: 1050,
    walkSpeed: 260,
    jumpPower: 500,
    width: 52,
    height: 114,
    specials: [
      { name: "Sunbeam Spear", cost: 25, kind: "projectile", damage: 115, hitstun: 0.5, reach: 0 },
      { name: "Arena Drop", cost: 30, kind: "grab", damage: 105, hitstun: 0.65, reach: 50 }
    ]
  },
  {
    id: "ArthurPerp",
    name: "Arthur Perp",
    tagline: "The Leverage King",
    color: "#26d9e6",
    maxHealth: 920,
    walkSpeed: 300,
    jumpPower: 520,
    width: 48,
    height: 110,
    specials: [
      { name: "100x Long", cost: 35, kind: "dash", damage: 150, hitstun: 0.55, reach: 140 },
      { name: "Funding Rate Drain", cost: 20, kind: "drain", damage: 45, hitstun: 0.4, reach: 95 }
    ]
  }
];

window.CK_DEFAULT_P1 = "VitalSpark";
window.CK_DEFAULT_P2 = "CharlesEpoch";

window.CK_NORMALS = {
  lightPunch:  { name: "Light Punch", damage: 30, hitstun: 0.22, startup: 0.06, active: 0.12, recovery: 0.16, reach: 58, height: 36, yOff: -70, knock: 80 },
  heavyPunch:  { name: "Heavy Punch", damage: 68, hitstun: 0.42, startup: 0.12, active: 0.16, recovery: 0.28, reach: 72, height: 40, yOff: -75, knock: 160 },
  lightKick:   { name: "Light Kick",  damage: 34, hitstun: 0.26, startup: 0.08, active: 0.12, recovery: 0.18, reach: 66, height: 28, yOff: -30, knock: 90 },
  heavyKick:   { name: "Heavy Kick",  damage: 78, hitstun: 0.48, startup: 0.14, active: 0.18, recovery: 0.32, reach: 86, height: 34, yOff: -50, knock: 200 }
};
