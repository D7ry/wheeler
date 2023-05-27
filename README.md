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
- [ ] Refactor project structure
  - [x] Move `WheelItem` to `Wheeler`
  - [x] Manage `WheelItemMutable` automatically in `WheelItemMutableManager` through automatic constructor insertion and destructor removal
  - [x] Move `Wheel` to a separate class
  - [ ] `Wheeler` only manages `Wheel` instead of `WheelEntry` that is now `Wheel`'s responsibility to manage
  - [ ] `Wheel` manages `WheelEntry`
  - [ ] `WheelEntry` manages all subclasses of `WheelItem`
  - [ ] Refactor functions so they reach correct objects
  - [ ] Change all storage to `std::unique_ptr` instead of raw pointer, except for `WheelItem` which is stored as a `shared_ptr` in the entry vector, and a `weak_ptr` for `WheelItemMutable` in `WheelItemMutableManager`
  - [ ] Refactor serialization for this hierarchy

References:

https://gist.github.com/thennequin/64b4b996ec990c6ddc13a48c6a0ba68c
https://www.nexusmods.com/skyrimspecialedition/mods/82545?tab=posts