# Wheeler

## Intro

Most, if not all modern RPG games have some sort of wheel menu for quick actions. GTA5's wheel allows the player to quickly browse their military arsenal, Witcher's wheel allows to switch between magic signs and consumables, and Bethesda's own title, Fallout4, has a wheel for favorited item access. This mod aims to integrate this modern UI paradigm into Skyrim, and hopefully making its players' life easier.

### The wheel


### Controlling
#### Toggling

Wheel menu can be toggled using a hotkey. You can:
  1. Short press the hotkey to toggle it on, until you press it again. 
  2. Press and hold the hotkey to keep it open, until you release.
  
The above 2 toggle methods coexists; if you press the hotkey long enough, it switches from mode 1 to mode 2. 

#### Item usage

Using(equipping/consuming) an item through the wheel is no different from using it in inventory; simply left/right click on the item will equip it to the corresponding hand.

#### Easy wheel editing

Items and actions can be easily stored into the wheel by:
1. Opening the inventory
2. Hovering over the item or action
3. Left click(MKB) / right shoulder(gamepad) on any slot in the wheel

Deletion is just as easy




To-dos:  
Desired hierarchy: Wheeler -> Wheel -> WheelEntry -> WheelItem
- [ ] Input prompts
- [ ] L/R hand equipped indicator
- [ ] Better input handling
  - [x] Only hook inputs that are bound, let other inputs go so user can walk while selecting wheel items
  - [ ] Disable some other inputs too(e.g. opening inventory) even though they're not bound they still affects the wheel.
- [ ] Add other item types
  - [x] Power - they're the same as spell but stored in a different slot
  - [x] Shout - lmb -> equip; rmb -> release (same as power that are not equipped to hand)
  - [ ] Potion 
  - [ ] Arrow
- [ ] Enchanting support
  - [ ] Enchanted items have enchanting bar display
  - [ ] Smart enchant refill
  - [ ] Special icon / text color for enchanted items
  - [x] Enchanted item descriptions
- [ ] Misc
  - [ ] Controller rumble
  - [x] Custom font
  - [x] Asian language(Chinese, Japanese, Korean) with special characters
  - [ ] Custom icons
  - [x] Optimize interpolator manager so that it only manages active interpolators, inactive interpolators are removed from its set, and they add themselves back when they receive a new interpolation target.

- Future plans
- [ ] Fallout-like side menu for consumable access
- [ ] 3D item display
- [ ] Non-trivial optimization
  - [ ] entry data caching
References:

https://gist.github.com/thennequin/64b4b996ec990c6ddc13a48c6a0ba68c
https://www.nexusmods.com/skyrimspecialedition/mods/82545?tab=posts