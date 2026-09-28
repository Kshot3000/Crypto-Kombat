/* Crypto Kombat — browser fighter (vanilla JS, no build). */
(function () {
  "use strict";

  const canvas = document.getElementById("game");
  const ctx = canvas.getContext("2d");
  const W = canvas.width;
  const H = canvas.height;

  const GROUND_Y = 580;
  const STAGE_LEFT = 60;
  const STAGE_RIGHT = W - 60;
  const GRAVITY = 1600;
  const CHIP = 0.25;
  const MOON_MAX = 100;
  const ROUND_TIME = 99;
  const ROUNDS_TO_WIN = 2;

  // ---------- Input ----------
  const keys = Object.create(null);
  const justPressed = Object.create(null);

  const BIND = {
    p1: {
      left: "KeyA", right: "KeyD", up: "KeyW",
      block: "ShiftLeft",
      lp: "KeyJ", hp: "KeyU", lk: "KeyK", hk: "KeyI",
      sp1: "KeyL", sp2: "KeyO"
    },
    p2: {
      left: "ArrowLeft", right: "ArrowRight", up: "ArrowUp",
      block: "ControlRight",
      // Mirror CONTROLS.md numpad: 1 LP, 2 LK, 3 SP1, 4 HP, 5 HK, 6 SP2
      lp: "Digit1", lk: "Digit2", sp1: "Digit3",
      hp: "Digit4", hk: "Digit5", sp2: "Digit6"
    }
  };
  // Also accept ControlLeft for P2 block; numpad aliases to Digit*
  const EXTRA_BLOCK_P2 = ["ControlLeft"];
  const NUMPAD_MAP = {
    Numpad1: "Digit1", Numpad2: "Digit2", Numpad3: "Digit3",
    Numpad4: "Digit4", Numpad5: "Digit5", Numpad6: "Digit6"
  };

  window.addEventListener("keydown", (e) => {
    let code = e.code;
    if (NUMPAD_MAP[code]) code = NUMPAD_MAP[code];
    if (!keys[code]) justPressed[code] = true;
    keys[code] = true;
    if (["ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight", "Space"].includes(e.key) ||
        e.code.startsWith("Digit") || e.code.startsWith("Numpad")) {
      e.preventDefault();
    }
  });
  window.addEventListener("keyup", (e) => {
    let code = e.code;
    if (NUMPAD_MAP[code]) code = NUMPAD_MAP[code];
    keys[code] = false;
  });
  canvas.addEventListener("mousedown", () => canvas.focus());
  canvas.focus();

  function pressed(code) { return !!keys[code]; }
  function tap(code) { return !!justPressed[code]; }
  function clearTaps() {
    for (const k in justPressed) delete justPressed[k];
  }
  function bindDown(b, action) {
    if (action === "block" && b === BIND.p2) {
      return pressed(b.block) || EXTRA_BLOCK_P2.some(pressed);
    }
    return pressed(b[action]);
  }
  function bindTap(b, action) { return tap(b[action]); }

  // ---------- Helpers ----------
  function clamp(v, a, b) { return Math.max(a, Math.min(b, v)); }
  function lerp(a, b, t) { return a + (b - a) * t; }
  function rectsOverlap(a, b) {
    return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y;
  }
  function hexRgb(hex) {
    const h = hex.replace("#", "");
    return {
      r: parseInt(h.slice(0, 2), 16),
      g: parseInt(h.slice(2, 4), 16),
      b: parseInt(h.slice(4, 6), 16)
    };
  }
  function rgba(hex, a) {
    const c = hexRgb(hex);
    return `rgba(${c.r},${c.g},${c.b},${a})`;
  }
  function findFighter(id) {
    return window.CK_ROSTER.find((f) => f.id === id) || window.CK_ROSTER[0];
  }

  // ---------- Particles / FX ----------
  const sparks = [];
  const projectiles = [];
  let shake = 0;
  let flash = 0;

  function spawnSparks(x, y, color, n) {
    for (let i = 0; i < n; i++) {
      const a = Math.random() * Math.PI * 2;
      const s = 80 + Math.random() * 220;
      sparks.push({
        x, y,
        vx: Math.cos(a) * s,
        vy: Math.sin(a) * s - 60,
        life: 0.25 + Math.random() * 0.35,
        max: 0.5,
        color,
        size: 2 + Math.random() * 4
      });
    }
  }

  function updateFX(dt) {
    for (let i = sparks.length - 1; i >= 0; i--) {
      const p = sparks[i];
      p.life -= dt;
      p.x += p.vx * dt;
      p.y += p.vy * dt;
      p.vy += 400 * dt;
      if (p.life <= 0) sparks.splice(i, 1);
    }
    for (let i = projectiles.length - 1; i >= 0; i--) {
      const p = projectiles[i];
      p.x += p.vx * dt;
      p.life -= dt;
      if (p.life <= 0 || p.x < -40 || p.x > W + 40) {
        projectiles.splice(i, 1);
        continue;
      }
      const foe = p.owner === game.p1 ? game.p2 : game.p1;
      if (!foe || foe.state === "ko") continue;
      const hb = { x: p.x - 14, y: p.y - 14, w: 28, h: 28 };
      if (rectsOverlap(hb, foe.body())) {
        applyHit(foe, p.owner, p.damage, p.hitstun, p.knock || 180, true);
        spawnSparks(p.x, p.y, p.color, 14);
        projectiles.splice(i, 1);
      }
    }
    shake = Math.max(0, shake - dt * 8);
    flash = Math.max(0, flash - dt * 4);
  }

  // ---------- Fighter ----------
  function Fighter(def, side) {
    this.def = def;
    this.side = side; // 1 or 2
    this.reset(side === 1 ? 280 : W - 280);
  }

  Fighter.prototype.reset = function (x) {
    this.x = x;
    this.y = GROUND_Y;
    this.vx = 0;
    this.vy = 0;
    this.facing = this.side === 1 ? 1 : -1;
    this.health = this.def.maxHealth;
    this.moon = 25;
    this.state = "idle";
    this.stateT = 0;
    this.attack = null;
    this.hitConnected = false;
    this.blocking = false;
    this.armor = 0;
    this.counter = 0;
    this.invuln = 0;
    this.moonFlash = 0;
    this.flurryHits = 0;
    this.animPhase = 0;
    this.flashHit = 0;
  };

  Fighter.prototype.body = function () {
    return {
      x: this.x - this.def.width / 2,
      y: this.y - this.def.height,
      w: this.def.width,
      h: this.def.height
    };
  };

  Fighter.prototype.onGround = function () {
    return this.y >= GROUND_Y - 0.5;
  };

  Fighter.prototype.canAct = function () {
    return this.state === "idle" || this.state === "walk" || this.state === "jump" || this.state === "block";
  };

  Fighter.prototype.startAttack = function (slot) {
    if (!this.canAct() && this.state !== "jump") return;
    if (this.state === "ko" || this.state === "hitstun") return;

    const normals = window.CK_NORMALS;
    let move = null;
    let special = null;

    if (slot === "lp") move = Object.assign({ slot }, normals.lightPunch);
    else if (slot === "hp") move = Object.assign({ slot }, normals.heavyPunch);
    else if (slot === "lk") move = Object.assign({ slot }, normals.lightKick);
    else if (slot === "hk") move = Object.assign({ slot }, normals.heavyKick);
    else if (slot === "sp1" || slot === "sp2") {
      const idx = slot === "sp1" ? 0 : 1;
      special = this.def.specials[idx];
      if (!special) return;
      if (this.moon < special.cost) { this.moonFlash = 0.9; return; }
      this.moon -= special.cost;
      move = {
        slot,
        name: special.name,
        damage: special.damage,
        hitstun: special.hitstun,
        startup: 0.1,
        active: special.kind === "flurry" ? 0.45 : 0.2,
        recovery: 0.28,
        reach: special.reach || 80,
        height: 44,
        yOff: special.kind === "uppercut" ? -90 : -60,
        knock: 160,
        special
      };
    }
    if (!move) return;

    this.attack = move;
    this.hitConnected = false;
    this.flurryHits = 0;
    this.state = move.special ? "special" : "attack";
    this.stateT = 0;
    this.blocking = false;

    if (move.special) {
      const k = move.special.kind;
      if (k === "dash") {
        this.vx = this.facing * 620;
        this.invuln = 0.18;
      } else if (k === "armor") {
        this.armor = move.active + move.startup + 0.2;
        this.vx = this.facing * 180;
      } else if (k === "counter") {
        this.counter = move.startup + move.active + 0.15;
      } else if (k === "projectile") {
        // spawned when active window opens
      } else if (k === "uppercut") {
        this.vy = -420;
        this.y = Math.min(this.y, GROUND_Y - 1);
      }
    }
  };

  Fighter.prototype.hitbox = function () {
    if (!this.attack) return null;
    const t = this.stateT;
    const m = this.attack;
    const start = m.startup;
    const end = m.startup + m.active;
    if (t < start || t > end) return null;
    if (this.hitConnected && !(m.special && m.special.kind === "flurry")) return null;

    const reach = m.reach || 60;
    const h = m.height || 36;
    const yOff = m.yOff || -60;
    const x0 = this.facing > 0 ? this.x + 8 : this.x - 8 - reach;
    return { x: x0, y: this.y + yOff - h / 2, w: reach, h: h };
  };

  Fighter.prototype.update = function (dt, bind, foe) {
    if (this.state === "ko") {
      this.vy += GRAVITY * dt;
      this.y += this.vy * dt;
      if (this.y > GROUND_Y) { this.y = GROUND_Y; this.vy = 0; }
      this.animPhase += dt;
      return;
    }

    this.moon = clamp(this.moon + dt * 4.5, 0, MOON_MAX);
    this.armor = Math.max(0, this.armor - dt);
    this.counter = Math.max(0, this.counter - dt);
    this.invuln = Math.max(0, this.invuln - dt);
    this.moonFlash = Math.max(0, this.moonFlash - dt);
    this.flashHit = Math.max(0, this.flashHit - dt);
    this.animPhase += dt;

    // Facing
    if (foe && this.state !== "attack" && this.state !== "special") {
      this.facing = foe.x >= this.x ? 1 : -1;
    }

    const busy = this.state === "attack" || this.state === "special" || this.state === "hitstun";
    this.blocking = !busy && this.onGround() && bindDown(bind, "block");

    if (this.state === "hitstun") {
      this.stateT -= dt;
      this.vx *= Math.pow(0.05, dt);
      this.vy += GRAVITY * dt;
      this.x += this.vx * dt;
      this.y += this.vy * dt;
      if (this.y > GROUND_Y) { this.y = GROUND_Y; this.vy = 0; }
      if (this.stateT <= 0) {
        this.state = this.onGround() ? "idle" : "jump";
        this.vx = 0;
      }
      this.x = clamp(this.x, STAGE_LEFT, STAGE_RIGHT);
      return;
    }

    if (this.state === "attack" || this.state === "special") {
      this.stateT += dt;
      const m = this.attack;
      const total = m.startup + m.active + m.recovery;

      // Projectile spawn once in active
      if (m.special && m.special.kind === "projectile" &&
          this.stateT >= m.startup && this.stateT - dt < m.startup) {
        projectiles.push({
          x: this.x + this.facing * 40,
          y: this.y - 70,
          vx: this.facing * 520,
          life: 2.0,
          damage: m.damage,
          hitstun: m.hitstun,
          knock: 200,
          color: this.def.color,
          owner: this
        });
      }

      // Splash push
      if (m.special && m.special.kind === "splash" &&
          this.stateT >= m.startup && !this.hitConnected) {
        // handled via large hitbox
      }

      // Flurry multi-hit reset
      if (m.special && m.special.kind === "flurry") {
        const windowT = this.stateT - m.startup;
        if (windowT > 0 && windowT < m.active) {
          const beat = Math.floor(windowT / 0.1);
          if (beat > this.flurryHits) {
            this.hitConnected = false;
            this.flurryHits = beat;
          }
        }
      }

      // Drain special: siphon moon
      if (m.special && m.special.kind === "drain" && this.hitConnected === "siphon") {
        // marker handled in applyHit
      }

      this.vy += GRAVITY * dt;
      if (m.special && m.special.kind === "dash") {
        // keep dash velocity early
        if (this.stateT < m.startup + m.active) {
          this.vx = this.facing * (m.special.kind === "dash" ? 580 : this.vx);
        } else {
          this.vx *= Math.pow(0.02, dt);
        }
      } else if (!(m.special && (m.special.kind === "armor"))) {
        this.vx *= Math.pow(0.01, dt);
      }

      this.x += this.vx * dt;
      this.y += this.vy * dt;
      if (this.y > GROUND_Y) { this.y = GROUND_Y; this.vy = 0; }
      this.x = clamp(this.x, STAGE_LEFT, STAGE_RIGHT);

      if (this.stateT >= total) {
        this.attack = null;
        this.state = this.onGround() ? "idle" : "jump";
        this.stateT = 0;
      }
      return;
    }

    // Movement
    let move = 0;
    if (bindDown(bind, "left")) move -= 1;
    if (bindDown(bind, "right")) move += 1;

    if (this.blocking) {
      this.vx = 0;
      this.state = "block";
    } else if (this.onGround()) {
      this.vx = move * this.def.walkSpeed;
      this.state = move !== 0 ? "walk" : "idle";
      if (bindTap(bind, "up")) {
        this.vy = -this.def.jumpPower;
        this.state = "jump";
      }
    } else {
      this.vx = move * this.def.walkSpeed * 0.85;
      this.state = "jump";
    }

    this.vy += GRAVITY * dt;
    this.x += this.vx * dt;
    this.y += this.vy * dt;
    if (this.y > GROUND_Y) { this.y = GROUND_Y; this.vy = 0; }
    this.x = clamp(this.x, STAGE_LEFT, STAGE_RIGHT);

    // Attacks
    if (!this.blocking) {
      if (bindTap(bind, "lp")) this.startAttack("lp");
      else if (bindTap(bind, "hp")) this.startAttack("hp");
      else if (bindTap(bind, "lk")) this.startAttack("lk");
      else if (bindTap(bind, "hk")) this.startAttack("hk");
      else if (bindTap(bind, "sp1")) this.startAttack("sp1");
      else if (bindTap(bind, "sp2")) this.startAttack("sp2");
    }
  };

  function applyHit(victim, attacker, damage, hitstun, knockback, isProjectile) {
    if (victim.state === "ko" || victim.invuln > 0) return false;

    // Counter window
    if (victim.counter > 0 && !isProjectile) {
      victim.counter = 0;
      victim.moon = clamp(victim.moon + 35, 0, MOON_MAX);
      spawnSparks(victim.x, victim.y - 60, "#ffffff", 18);
      applyHit(attacker, victim, damage * 0.9, hitstun, knockback * 1.1, false);
      shake = Math.max(shake, 0.35);
      return true;
    }

    const blocking = victim.blocking && victim.onGround() &&
      ((attacker.x >= victim.x && victim.facing > 0) || (attacker.x < victim.x && victim.facing < 0) ||
       Math.sign(attacker.x - victim.x) !== victim.facing);

    // Better block check: facing toward attacker
    const toward = Math.sign(attacker.x - victim.x);
    const facingAttacker = victim.facing === toward || victim.facing === 0;
    const isBlock = victim.blocking && victim.onGround() && facingAttacker && victim.armor <= 0;

    if (isBlock && victim.armor <= 0) {
      const chip = damage * CHIP;
      victim.health = Math.max(0, victim.health - chip);
      victim.moon = clamp(victim.moon + 6, 0, MOON_MAX);
      attacker.moon = clamp(attacker.moon + 4, 0, MOON_MAX);
      spawnSparks(victim.x + victim.facing * -20, victim.y - 70, "#88ccff", 8);
      if (victim.health <= 0) koFighter(victim);
      return true;
    }

    let dmg = damage;
    if (victim.armor > 0) dmg *= 0.45;

    victim.health = Math.max(0, victim.health - dmg);
    victim.state = "hitstun";
    victim.stateT = hitstun;
    victim.attack = null;
    victim.blocking = false;
    victim.flashHit = 0.12;
    victim.vx = (attacker.facing || Math.sign(victim.x - attacker.x) || 1) * (knockback || 120);
    if (attacker.attack && attacker.attack.special && attacker.attack.special.kind === "uppercut") {
      victim.vy = -380;
    } else {
      victim.vy = -80;
    }

    attacker.moon = clamp(attacker.moon + 8, 0, MOON_MAX);
    if (attacker.attack && attacker.attack.special && attacker.attack.special.kind === "drain") {
      attacker.moon = clamp(attacker.moon + 20, 0, MOON_MAX);
      victim.moon = clamp(victim.moon - 15, 0, MOON_MAX);
    }

    spawnSparks(victim.x, victim.y - 60, attacker.def.color, 16);
    shake = Math.max(shake, dmg > 100 ? 0.55 : 0.25);
    if (dmg > 90) flash = 0.12;

    if (victim.health <= 0) koFighter(victim);
    return true;
  }

  function koFighter(f) {
    f.state = "ko";
    f.health = 0;
    f.vy = -280;
    f.vx = -f.facing * 120;
    f.attack = null;
    shake = 0.7;
    flash = 0.2;
  }

  function resolveHits() {
    [game.p1, game.p2].forEach((attacker) => {
      if (!attacker || !attacker.attack) return;
      const foe = attacker === game.p1 ? game.p2 : game.p1;
      const hb = attacker.hitbox();
      if (!hb) return;

      // Splash: wider box
      if (attacker.attack.special && attacker.attack.special.kind === "splash") {
        hb.w = 160;
        hb.x = attacker.x - 80;
        hb.y = GROUND_Y - 100;
        hb.h = 100;
      }

      // Grab: must be close
      if (attacker.attack.special && attacker.attack.special.kind === "grab") {
        if (Math.abs(attacker.x - foe.x) > 70) return;
      }

      if (rectsOverlap(hb, foe.body())) {
        if (attacker.hitConnected && !(attacker.attack.special && attacker.attack.special.kind === "flurry")) return;
        const ok = applyHit(
          foe, attacker,
          attacker.attack.damage,
          attacker.attack.hitstun,
          attacker.attack.knock,
          false
        );
        if (ok) attacker.hitConnected = true;
      }
    });
  }

  // ---------- Game state ----------
  const game = {
    mode: "select", // select | fight | roundend | matchend
    p1Pick: 0,
    p2Pick: 1,
    selecting: 1, // whose cursor on select (both can move own)
    p1: null,
    p2: null,
    p1Wins: 0,
    p2Wins: 0,
    round: 1,
    timer: ROUND_TIME,
    banner: "",
    bannerT: 0,
    roundOver: false,
    winner: null
  };

  // defaults
  (function initPicks() {
    const i1 = window.CK_ROSTER.findIndex((f) => f.id === window.CK_DEFAULT_P1);
    const i2 = window.CK_ROSTER.findIndex((f) => f.id === window.CK_DEFAULT_P2);
    game.p1Pick = i1 >= 0 ? i1 : 0;
    game.p2Pick = i2 >= 0 ? i2 : 1;
  })();

  function startMatch() {
    game.p1Wins = 0;
    game.p2Wins = 0;
    game.round = 1;
    game.winner = null;
    startRound();
  }

  function startRound() {
    sparks.length = 0;
    projectiles.length = 0;
    const d1 = window.CK_ROSTER[game.p1Pick];
    const d2 = window.CK_ROSTER[game.p2Pick];
    game.p1 = new Fighter(d1, 1);
    game.p2 = new Fighter(d2, 2);
    game.timer = ROUND_TIME;
    game.roundOver = false;
    game.mode = "fight";
    game.banner = "READY";
    game.bannerT = 1.0;
    game._fightDelay = 1.0;
  }

  function endRound(winnerSide) {
    if (game.roundOver) return;
    game.roundOver = true;
    if (winnerSide === 1) game.p1Wins++;
    else if (winnerSide === 2) game.p2Wins++;
    else {
      // timeout — higher health
      if (game.p1.health > game.p2.health) game.p1Wins++;
      else if (game.p2.health > game.p1.health) game.p2Wins++;
      // draw: both get nothing, still advance
    }

    if (game.p1Wins >= ROUNDS_TO_WIN) {
      game.winner = 1;
      game.mode = "matchend";
      game.banner = "P1 WINS";
      game.bannerT = 99;
    } else if (game.p2Wins >= ROUNDS_TO_WIN) {
      game.winner = 2;
      game.mode = "matchend";
      game.banner = "P2 WINS";
      game.bannerT = 99;
    } else {
      game.mode = "roundend";
      game.banner = winnerSide === 0 ? "TIME UP" : (winnerSide === 1 ? "P1 ROUND" : "P2 ROUND");
      game.bannerT = 2.2;
      game._nextRoundAt = 2.2;
    }
  }

  // ---------- Update ----------
  function update(dt) {
    if (game.mode === "select") {
      updateSelect();
      return;
    }

    if (game.mode === "matchend") {
      if (tap("KeyR")) startMatch();
      if (tap("Escape")) game.mode = "select";
      updateFX(dt);
      if (game.p1) game.p1.update(dt, BIND.p1, game.p2);
      if (game.p2) game.p2.update(dt, BIND.p2, game.p1);
      return;
    }

    if (game.mode === "roundend") {
      game.bannerT -= dt;
      game._nextRoundAt -= dt;
      updateFX(dt);
      if (game.p1) game.p1.update(dt, { left:"",right:"",up:"",block:"",lp:"",hp:"",lk:"",hk:"",sp1:"",sp2:"" }, game.p2);
      if (game.p2) game.p2.update(dt, { left:"",right:"",up:"",block:"",lp:"",hp:"",lk:"",hk:"",sp1:"",sp2:"" }, game.p1);
      if (game._nextRoundAt <= 0) {
        game.round++;
        startRound();
      }
      return;
    }

    // fight
    if (game._fightDelay > 0) {
      game._fightDelay -= dt;
      game.bannerT -= dt;
      if (game._fightDelay <= 0) {
        game.banner = "FIGHT";
        game.bannerT = 0.7;
      }
      updateFX(dt);
      return;
    }

    if (game.bannerT > 0) game.bannerT -= dt;

    if (!game.roundOver) {
      game.timer -= dt;
      if (game.timer <= 0) {
        game.timer = 0;
        endRound(0);
      }
    }

    game.p1.update(dt, BIND.p1, game.p2);
    game.p2.update(dt, BIND.p2, game.p1);

    // Push apart if overlapping
    const b1 = game.p1.body();
    const b2 = game.p2.body();
    if (rectsOverlap(b1, b2) && game.p1.onGround() && game.p2.onGround()) {
      const mid = (game.p1.x + game.p2.x) / 2;
      const sep = (game.p1.def.width + game.p2.def.width) / 2 + 2;
      if (game.p1.x <= game.p2.x) {
        game.p1.x = mid - sep / 2;
        game.p2.x = mid + sep / 2;
      } else {
        game.p2.x = mid - sep / 2;
        game.p1.x = mid + sep / 2;
      }
      game.p1.x = clamp(game.p1.x, STAGE_LEFT, STAGE_RIGHT);
      game.p2.x = clamp(game.p2.x, STAGE_LEFT, STAGE_RIGHT);
    }

    resolveHits();
    updateFX(dt);

    if (!game.roundOver) {
      if (game.p1.state === "ko") endRound(2);
      else if (game.p2.state === "ko") endRound(1);
    }

    if (tap("KeyR")) startMatch();
    if (tap("Escape")) game.mode = "select";
  }

  function updateSelect() {
    const n = window.CK_ROSTER.length;
    if (tap("KeyA") || tap("ArrowLeft")) {
      // P1 left with A, P2 with arrows — both always active
    }
    if (tap("KeyA")) game.p1Pick = (game.p1Pick - 1 + n) % n;
    if (tap("KeyD")) game.p1Pick = (game.p1Pick + 1) % n;
    if (tap("KeyW")) game.p1Pick = (game.p1Pick - 4 + n) % n;
    if (tap("KeyS")) game.p1Pick = (game.p1Pick + 4) % n;

    if (tap("ArrowLeft")) game.p2Pick = (game.p2Pick - 1 + n) % n;
    if (tap("ArrowRight")) game.p2Pick = (game.p2Pick + 1) % n;
    if (tap("ArrowUp")) game.p2Pick = (game.p2Pick - 4 + n) % n;
    if (tap("ArrowDown")) game.p2Pick = (game.p2Pick + 4) % n;

    if (tap("Enter") || tap("Space")) startMatch();
  }

  // ---------- Render ----------
  function drawArena() {
    // Backdrop gradient
    const g = ctx.createLinearGradient(0, 0, 0, H);
    g.addColorStop(0, "#0a0618");
    g.addColorStop(0.55, "#120a28");
    g.addColorStop(1, "#080610");
    ctx.fillStyle = g;
    ctx.fillRect(0, 0, W, H);

    // Back wall panels
    for (let i = 0; i < 8; i++) {
      const x = 40 + i * 155;
      ctx.fillStyle = i % 2 === 0 ? "#14102a" : "#10101f";
      ctx.fillRect(x, 80, 140, 320);
      ctx.strokeStyle = i % 2 === 0 ? "rgba(0,240,255,0.25)" : "rgba(255,43,214,0.25)";
      ctx.lineWidth = 2;
      ctx.strokeRect(x, 80, 140, 320);
      // candle stub
      const up = i % 2 === 0;
      ctx.fillStyle = up ? "rgba(0,240,255,0.55)" : "rgba(255,43,214,0.55)";
      const ch = 40 + (i * 37) % 90;
      ctx.fillRect(x + 55, 360 - ch, 28, ch);
      ctx.fillStyle = up ? "rgba(255,80,120,0.5)" : "rgba(80,255,180,0.45)";
      const ch2 = 20 + (i * 53) % 50;
      ctx.fillRect(x + 90, 360 - ch2, 18, ch2);
    }

    // Neon grid floor
    ctx.fillStyle = "#0c0a18";
    ctx.fillRect(0, GROUND_Y, W, H - GROUND_Y);
    ctx.strokeStyle = "rgba(0,240,255,0.2)";
    ctx.lineWidth = 1;
    for (let x = 0; x < W; x += 40) {
      ctx.beginPath(); ctx.moveTo(x, GROUND_Y); ctx.lineTo(x - 30, H); ctx.stroke();
    }
    for (let y = GROUND_Y; y < H; y += 28) {
      ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(W, y); ctx.stroke();
    }

    // Center strip
    ctx.fillStyle = "rgba(255,43,214,0.15)";
    ctx.fillRect(W / 2 - 40, GROUND_Y, 80, 12);
    ctx.fillStyle = "rgba(0,240,255,0.35)";
    ctx.fillRect(0, GROUND_Y, W, 4);

    // Walls
    ctx.fillStyle = "rgba(0,240,255,0.35)";
    ctx.fillRect(20, 200, 12, GROUND_Y - 200);
    ctx.fillStyle = "rgba(255,43,214,0.35)";
    ctx.fillRect(W - 32, 200, 12, GROUND_Y - 200);

    // Title plate
    ctx.fillStyle = "rgba(0,0,0,0.35)";
    ctx.fillRect(W / 2 - 160, 36, 320, 36);
    ctx.strokeStyle = "rgba(0,240,255,0.4)";
    ctx.strokeRect(W / 2 - 160, 36, 320, 36);
    ctx.fillStyle = "#00f0ff";
    ctx.font = "bold 16px Segoe UI, sans-serif";
    ctx.textAlign = "center";
    ctx.fillText("BLOCKCHAIN COLOSSEUM", W / 2, 60);
  }

  function drawFighter(f) {
    if (!f) return;
    const c = f.def.color;
    const dir = f.facing;
    const bob = f.state === "walk" ? Math.sin(f.animPhase * 12) * 3 : 0;
    const crouch = f.blocking ? 10 : 0;

    const flashWhite = f.flashHit > 0;
    const bodyCol = flashWhite ? "#ffffff" : c;
    const dark = flashWhite ? "#dddddd" : rgba(c, 0.55);

    ctx.save();
    ctx.translate(f.x, f.y + bob);

    // Shadow
    ctx.fillStyle = "rgba(0,0,0,0.45)";
    ctx.beginPath();
    ctx.ellipse(0, 0, f.def.width * 0.55, 10, 0, 0, Math.PI * 2);
    ctx.fill();

    // Legs
    const legSpread = f.state === "walk" ? Math.sin(f.animPhase * 12) * 10 : (f.state === "jump" ? 8 : 4);
    ctx.fillStyle = dark;
    ctx.fillRect(-16 + legSpread, -42 + crouch, 14, 42 - crouch);
    ctx.fillRect(2 - legSpread, -42 + crouch, 14, 42 - crouch);

    // Torso
    ctx.fillStyle = bodyCol;
    ctx.fillRect(-f.def.width / 2 + 4, -f.def.height + 18 + crouch, f.def.width - 8, f.def.height - 60);

    // Head
    ctx.fillStyle = bodyCol;
    ctx.fillRect(-16, -f.def.height + crouch, 32, 28);
    // Visor / eyes
    ctx.fillStyle = "#0a0814";
    ctx.fillRect(-12, -f.def.height + 10 + crouch, 24, 8);
    ctx.fillStyle = "#00f0ff";
    ctx.fillRect(dir > 0 ? 2 : -10, -f.def.height + 12 + crouch, 8, 4);

    // Arms / attack pose
    ctx.fillStyle = dark;
    let armY = -f.def.height + 40 + crouch;
    let armExt = 0;
    if ((f.state === "attack" || f.state === "special") && f.attack) {
      const t = f.stateT;
      if (t > f.attack.startup * 0.5) armExt = f.attack.reach * 0.55;
    }
    if (f.blocking) {
      ctx.fillRect(-8, armY - 10, 16, 36);
      ctx.fillRect(-20, armY, 40, 14);
    } else {
      ctx.fillRect(dir * (8), armY, dir * (18 + armExt), 12);
      ctx.fillRect(-dir * 22, armY + 8, dir * 16, 12);
    }

    // Accent outline
    ctx.strokeStyle = rgba(c, 0.9);
    ctx.lineWidth = 2;
    ctx.strokeRect(-f.def.width / 2 + 4, -f.def.height + 18 + crouch, f.def.width - 8, f.def.height - 60);

    // Counter / armor aura
    if (f.counter > 0) {
      ctx.strokeStyle = "rgba(255,255,255,0.7)";
      ctx.strokeRect(-f.def.width / 2 - 6, -f.def.height - 6, f.def.width + 12, f.def.height + 12);
    }
    if (f.armor > 0) {
      ctx.strokeStyle = "rgba(255,220,50,0.8)";
      ctx.lineWidth = 3;
      ctx.strokeRect(-f.def.width / 2 - 4, -f.def.height - 4, f.def.width + 8, f.def.height + 8);
    }

    // Name plate
    ctx.fillStyle = "rgba(0,0,0,0.5)";
    ctx.fillRect(-40, -f.def.height - 22, 80, 14);
    ctx.fillStyle = c;
    ctx.font = "bold 10px Segoe UI, sans-serif";
    ctx.textAlign = "center";
    ctx.fillText(f.def.name.split(" ")[0].toUpperCase(), 0, -f.def.height - 11);

    // Low-moon feedback when a special is rejected
    if (f.moonFlash > 0) {
      ctx.fillStyle = "#ffd24a";
      ctx.font = "bold 12px Segoe UI, sans-serif";
      ctx.textAlign = "center";
      ctx.fillText("LOW MOON", 0, -f.def.height - 32);
    }

    ctx.restore();

    // Debug hitbox during attack
    const hb = f.hitbox();
    if (hb) {
      ctx.fillStyle = "rgba(255,80,80,0.35)";
      ctx.fillRect(hb.x, hb.y, hb.w, hb.h);
    }
  }

  function drawHUD() {
    if (!game.p1 || !game.p2) return;
    const p1 = game.p1, p2 = game.p2;

    function bar(x, y, w, h, pct, color, flip) {
      ctx.fillStyle = "rgba(0,0,0,0.65)";
      ctx.fillRect(x - 2, y - 2, w + 4, h + 4);
      ctx.strokeStyle = rgba(color, 0.7);
      ctx.strokeRect(x - 2, y - 2, w + 4, h + 4);
      const fw = Math.max(0, w * clamp(pct, 0, 1));
      ctx.fillStyle = color;
      if (flip) ctx.fillRect(x + w - fw, y, fw, h);
      else ctx.fillRect(x, y, fw, h);
    }

    // Names
    ctx.font = "bold 18px Segoe UI, sans-serif";
    ctx.textAlign = "left";
    ctx.fillStyle = p1.def.color;
    ctx.fillText(p1.def.name.toUpperCase(), 40, 36);
    ctx.textAlign = "right";
    ctx.fillStyle = p2.def.color;
    ctx.fillText(p2.def.name.toUpperCase(), W - 40, 36);

    bar(40, 48, 420, 18, p1.health / p1.def.maxHealth, p1.def.color, false);
    bar(W - 460, 48, 420, 18, p2.health / p2.def.maxHealth, p2.def.color, true);

    // Moon meters
    ctx.font = "11px Segoe UI, sans-serif";
    ctx.textAlign = "left";
    ctx.fillStyle = "#c9b6ff";
    ctx.fillText("MOON", 40, 90);
    bar(90, 78, 200, 10, p1.moon / MOON_MAX, "#b388ff", false);
    ctx.textAlign = "right";
    ctx.fillText("MOON", W - 40, 90);
    bar(W - 290, 78, 200, 10, p2.moon / MOON_MAX, "#b388ff", true);

    // Timer
    ctx.textAlign = "center";
    ctx.fillStyle = game.timer <= 10 ? "#ff2bd6" : "#00f0ff";
    ctx.font = "bold 42px Segoe UI, sans-serif";
    ctx.fillText(String(Math.ceil(game.timer)), W / 2, 78);

    // Round pips
    ctx.font = "12px Segoe UI, sans-serif";
    ctx.fillStyle = "#8a86a8";
    ctx.fillText("ROUND " + game.round, W / 2, 98);
    for (let i = 0; i < ROUNDS_TO_WIN; i++) {
      ctx.fillStyle = i < game.p1Wins ? "#00f0ff" : "#333";
      ctx.beginPath(); ctx.arc(W / 2 - 50 - i * 18, 50, 6, 0, Math.PI * 2); ctx.fill();
      ctx.fillStyle = i < game.p2Wins ? "#ff2bd6" : "#333";
      ctx.beginPath(); ctx.arc(W / 2 + 50 + i * 18, 50, 6, 0, Math.PI * 2); ctx.fill();
    }

    // Banner
    if (game.bannerT > 0 && game.banner) {
      const alpha = clamp(game.bannerT * 2, 0, 1);
      ctx.fillStyle = `rgba(0,0,0,${0.45 * alpha})`;
      ctx.fillRect(0, H / 2 - 60, W, 100);
      ctx.textAlign = "center";
      ctx.fillStyle = `rgba(0,240,255,${alpha})`;
      ctx.font = "bold 72px Segoe UI, sans-serif";
      ctx.fillText(game.banner, W / 2, H / 2 + 20);
    }

    if (game.mode === "matchend") {
      ctx.fillStyle = "rgba(0,0,0,0.55)";
      ctx.fillRect(0, 0, W, H);
      ctx.textAlign = "center";
      ctx.fillStyle = game.winner === 1 ? game.p1.def.color : game.p2.def.color;
      ctx.font = "bold 64px Segoe UI, sans-serif";
      const wname = game.winner === 1 ? game.p1.def.name : game.p2.def.name;
      ctx.fillText(wname.toUpperCase() + " WINS", W / 2, H / 2 - 20);
      ctx.fillStyle = "#e8e6ff";
      ctx.font = "22px Segoe UI, sans-serif";
      ctx.fillText("Press R to rematch  ·  Esc for character select", W / 2, H / 2 + 40);
    }
  }

  function drawSelect() {
    drawArena();
    ctx.fillStyle = "rgba(5,4,12,0.72)";
    ctx.fillRect(0, 0, W, H);

    ctx.textAlign = "center";
    ctx.fillStyle = "#00f0ff";
    ctx.font = "bold 40px Segoe UI, sans-serif";
    ctx.fillText("SELECT YOUR FIGHTER", W / 2, 70);
    ctx.fillStyle = "#8a86a8";
    ctx.font = "16px Segoe UI, sans-serif";
    ctx.fillText("P1: WASD to browse  ·  P2: Arrows to browse  ·  Enter to fight", W / 2, 100);

    const roster = window.CK_ROSTER;
    const cols = 4;
    const cardW = 240, cardH = 150;
    const gapX = 30, gapY = 24;
    const startX = (W - (cols * cardW + (cols - 1) * gapX)) / 2;
    const startY = 130;

    roster.forEach((f, i) => {
      const col = i % cols;
      const row = Math.floor(i / cols);
      const x = startX + col * (cardW + gapX);
      const y = startY + row * (cardH + gapY);
      const isP1 = i === game.p1Pick;
      const isP2 = i === game.p2Pick;

      ctx.fillStyle = "#12101f";
      ctx.fillRect(x, y, cardW, cardH);
      ctx.lineWidth = 3;
      if (isP1 && isP2) {
        ctx.strokeStyle = "#ffffff";
      } else if (isP1) {
        ctx.strokeStyle = "#00f0ff";
      } else if (isP2) {
        ctx.strokeStyle = "#ff2bd6";
      } else {
        ctx.strokeStyle = rgba(f.color, 0.35);
        ctx.lineWidth = 1;
      }
      ctx.strokeRect(x, y, cardW, cardH);

      // Mini fighter
      ctx.fillStyle = f.color;
      ctx.fillRect(x + 24, y + 40, 36, 70);
      ctx.fillRect(x + 30, y + 22, 24, 22);
      ctx.fillStyle = "#0a0814";
      ctx.fillRect(x + 34, y + 30, 16, 6);

      ctx.fillStyle = f.color;
      ctx.font = "bold 18px Segoe UI, sans-serif";
      ctx.textAlign = "left";
      ctx.fillText(f.name, x + 78, y + 48);
      ctx.fillStyle = "#8a86a8";
      ctx.font = "12px Segoe UI, sans-serif";
      ctx.fillText(f.tagline, x + 78, y + 68);
      ctx.fillText("HP " + f.maxHealth + "  SPD " + f.walkSpeed, x + 78, y + 90);
      ctx.fillStyle = "#c9b6ff";
      ctx.font = "11px Segoe UI, sans-serif";
      ctx.fillText(f.specials[0].name, x + 78, y + 112);
      ctx.fillText(f.specials[1].name, x + 78, y + 128);

      if (isP1) {
        ctx.fillStyle = "#00f0ff";
        ctx.font = "bold 12px Segoe UI, sans-serif";
        ctx.fillText("P1", x + 8, y + 18);
      }
      if (isP2) {
        ctx.fillStyle = "#ff2bd6";
        ctx.font = "bold 12px Segoe UI, sans-serif";
        ctx.textAlign = "right";
        ctx.fillText("P2", x + cardW - 8, y + 18);
        ctx.textAlign = "left";
      }
    });

    // Matchup preview
    const a = roster[game.p1Pick], b = roster[game.p2Pick];
    ctx.textAlign = "center";
    ctx.fillStyle = "#e8e6ff";
    ctx.font = "bold 22px Segoe UI, sans-serif";
    ctx.fillText(a.name + "  VS  " + b.name, W / 2, H - 48);
    ctx.fillStyle = "#8a86a8";
    ctx.font = "14px Segoe UI, sans-serif";
    ctx.fillText("Default matchup: Vital Spark vs Charles Epoch · Parody names only", W / 2, H - 24);
  }

  function drawFX() {
    sparks.forEach((p) => {
      ctx.globalAlpha = clamp(p.life / p.max, 0, 1);
      ctx.fillStyle = p.color;
      ctx.fillRect(p.x, p.y, p.size, p.size);
    });
    ctx.globalAlpha = 1;
    projectiles.forEach((p) => {
      ctx.fillStyle = p.color;
      ctx.shadowColor = p.color;
      ctx.shadowBlur = 16;
      ctx.fillRect(p.x - 16, p.y - 10, 32, 20);
      ctx.shadowBlur = 0;
      ctx.fillStyle = "#ffffff";
      ctx.fillRect(p.x - 6, p.y - 4, 12, 8);
    });
  }

  function render() {
    ctx.save();
    if (shake > 0) {
      const m = shake * 10;
      ctx.translate((Math.random() - 0.5) * m, (Math.random() - 0.5) * m);
    }

    if (game.mode === "select") {
      drawSelect();
    } else {
      drawArena();
      drawFighter(game.p1);
      drawFighter(game.p2);
      drawFX();
      drawHUD();
    }

    if (flash > 0) {
      ctx.fillStyle = `rgba(255,255,255,${flash * 0.45})`;
      ctx.fillRect(-20, -20, W + 40, H + 40);
    }
    ctx.restore();
  }

  // ---------- Loop ----------
  let last = performance.now();
  function frame(now) {
    let dt = (now - last) / 1000;
    last = now;
    dt = Math.min(dt, 1 / 20);
    update(dt);
    render();
    clearTaps();
    requestAnimationFrame(frame);
  }
  requestAnimationFrame(frame);
})();
