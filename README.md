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
- [ ] multi-wheel
	- [ ] data structure
	- [ ] UI
- [ ] in-game wheel editing
- [ ] add config in DMenu
- [ ] framework texture styling
- [ ] Scaling
	- [ ] Support magic
	- [ ] Support shout
	- [ ] Support power
- [ ] animated armoury support
Nice-to-haves:
- [ ] block only used inputs(input hooking)
- [ ] multi-lingual and custom font
- [ ] theme


References:

https://gist.github.com/thennequin/64b4b996ec990c6ddc13a48c6a0ba68c
https://www.nexusmods.com/skyrimspecialedition/mods/82545?tab=posts