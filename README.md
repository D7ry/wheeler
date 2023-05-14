# Wheeler
WIP design of a wheel menu....
Pipeline:


`WheelData`:
manages items, sends them to renderer, reacts to user changes to data(adding&deleting stuff), flush changes to `WheelSerializer`.
- `WheelSerializer`: gets saved items, init `WheelItems`(vector of vector of stuff...), flush changes made by `WheelItems`
- `WheelItem`:
Metadata of an item.
```cpp
class WheelItem // base class
{
	public:
	virtual void activate(int aux);
	void render();
}

//example of another class
class WheelItemWeapon
{
	public:
	void activate(int aux) override
	{
		player->equipWeapon(this->_weapon);
	}
	private:
	Weapon _weapon;
}
class WheelItemSubMenu
{
	public:
	void activate(int aux) override
	{
		
	}
}
```

```cpp
class WheelData
{
	class WheelSerializer;

	std::vector<WheelItem> _items;
}

```

`WheelRender`: gets data from `WheelData`, render them
- UI(oof), animations...
- Sound(low priority)
- make callbacks to Data on user interactions(deleting items)

To-dos:

- MVP: 
	- [ ] data structure for wheelData
		- [ ] design
		- [ ] temporary test data structure
	- [ ] minimally working renderer


References:

https://gist.github.com/thennequin/64b4b996ec990c6ddc13a48c6a0ba68c
https://www.nexusmods.com/skyrimspecialedition/mods/82545?tab=posts