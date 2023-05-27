# Wheeler
WIP design of a wheel menu....
Pipeline:

- `Wheeler`: manages items
- `Serializer`: gets saved items, init `WheelItems`(vector of vector of stuff...), flush changes made by `Wheeler`
- `WheelItem`: parent class of all wheel items, contains `Draw()` function called by `Wheeler`
- `Renderer`: ImGui hook and entry
- `Texture`: interface for getting ptrs to textures, responsible for reading textures
- `Config`: all settings, styling etc...
- `Controls`: input mapping

To-dos:
Desired hierarchy: Wheeler -> Wheel -> WheelEntry -> WheelItem
- [x] Refactor project structure
  - [x] Move `WheelItem` to `Wheeler`
  - [x] Manage `WheelItemMutable` automatically in `WheelItemMutableManager` through automatic constructor insertion and destructor removal
  - [x] Move `Wheel` to a separate class
  - [x] `Wheeler` only manages `Wheel` instead of `WheelEntry` that is now `Wheel`'s responsibility to manage
  - [x] `Wheel` manages `WheelEntry`
  - [x] `WheelEntry` manages all subclasses of `WheelItem`
  - [x] Refactor functions so they reach correct objects
  - [x] Change all storage to `std::unique_ptr` instead of raw pointer, except for `WheelItem` which is stored as a `shared_ptr` in the entry vector, and a `weak_ptr` for `WheelItemMutable` in `WheelItemMutableManager`
  - [x] Refactor serialization for this hierarchy
  - [x] Move render function into wheelentry, wheel, and wheelitem; add locking
- [ ] Establish rendering pipeline: each level in the hierarchy(Wheeler, Wheel, WheelEntry, WheelItem) call the lower hierarchy's Draw() function, passing its own alpha, scale, rotation, and translation mult
  - [ ] Change existing rendering functions to work with alpha, scale, rotation, and translation
- [ ] Input prompts
- [ ] L/R hand equipped indicator
- [ ] Steal Ersh's input hook once OAR releases
  - [ ] Only hook inputs that are bound, let other inputs go so user can walk while selecting wheel items
- [ ] Add other item types
  - [ ] Power - they're the same as spell but stored in a different slot
  - [ ] Shout - lmb -> equip; rmb -> release (same as power that are not equipped to hand)
  - [ ] Potion 
  - [ ] Arrow
- [ ] Enchanting support
  - [ ] Enchanted items have enchanting bar display
  - [ ] Smart enchant refill
  - [ ] Special icon / text color for enchanted items
- [ ] Misc
  - [ ] Controller rumble
  - [ ] Custom font
  - [ ] Asian language(Chinese, Japanese, Korean) with special characters

- Future plans
- [ ] Fallout-like side menu for consumable access
- [ ] 3D item display
References:

https://gist.github.com/thennequin/64b4b996ec990c6ddc13a48c6a0ba68c
https://www.nexusmods.com/skyrimspecialedition/mods/82545?tab=posts