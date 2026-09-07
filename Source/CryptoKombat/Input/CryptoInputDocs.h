// Copyright CryptoKombat. Phase 1 scaffold.
//
// Enhanced Input — create these assets in the Editor under Content/Input/
// and assign them on BP_CryptoFighter (or defaults in the Mapping Context).
//
// Mapping Context: IMC_Fight
//
// Actions (Value type):
//   IA_Move          Axis1D (Digital)   — horizontal walk
//   IA_Jump          Digital            — jump
//   IA_PunchLight    Digital
//   IA_PunchHeavy    Digital
//   IA_KickLight     Digital
//   IA_KickHeavy     Digital
//   IA_Special1      Digital
//   IA_Special2      Digital
//   IA_Block         Digital (hold)
//
// Suggested keyboard mappings are authored in Config/DefaultInput.ini for the
// classic Action/Axis fallback used by ACryptoFighter when EI assets are unset.
// See CONTROLS.md at project root for the full P1/P2 layout.
//
// Dual Local Players: AFightGameMode calls CreatePlayer(1) so P2 receives a
// second PlayerController. Give each BP a distinct IMC with P1 vs P2 keys,
// or rely on DefaultInput.ini axis/action names P1_* / P2_*.

#pragma once

#include "CoreMinimal.h"
