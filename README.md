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
	// the callee is responsible for openening a new wheel menu.
	void Render();
	private:
	virtual void Activate(int aux);

}

//example of another class
class WheelItemWeapon : WheelItem
{
	public:
	void Render() override;
	private:
	void Activate(int aux) override
	{
		player->equipWeapon(this->_weapon);
	}
	Weapon _weapon;
}
class WheelItemSubMenu : WheelItem
{
	public:
	void Activate(int aux) override
	{

	}
	void Render() override;
}
```

```cpp
class Wheel
{
	std::vector<WheelItem> _items;
}

```
To-dos:

- MVP: 
	- [ ] data structure
		- [ ] design
		- [ ] temporary test data structure
	- [ ] minimally working renderer


References:

https://gist.github.com/thennequin/64b4b996ec990c6ddc13a48c6a0ba68c
https://www.nexusmods.com/skyrimspecialedition/mods/82545?tab=posts