# impsbru

A impossible shapes logic game for Amiga.

![screenshot 1](res/screenshots/1.png)
![screenshot 2](res/screenshots/2.png)

## Getting Started

Follow the edges of the shapes with red cube and finish the level by walking on the red rectangle. It's tricky because shapes are impossible to exist in 3 dimensions. Squeeze your brain!

### Pre-requirements

This game is made for specific hardware. To properly run this game you will need:

* Amiga OCS/ECS (500 or better)
* Kickstart 1.3 or better

or compatible Amiga emulator like [WinUAE](http://www.winuae.net/).

### Installing

Copy the [latest game release](https://github.com/approxit/impsbru/releases) archive and extract to directory that Amiga can load. The `bin` and `data` directories must coexist as given. 

### Running the game

Game must be stared from main executable file located in `bin/impsbru`.
In AmigaDOS CLI you can use following commands:

```
1> cd impsbru/bin
1> impsbru
``` 

## Controls

### Map Editor
* **<kbd>q</kbd> / <kbd>w</kbd> / <kbd>e</kbd>** - toggles element at `north west` / `north` / `north east` direction of the cursor, if can.
* **<kbd>z</kbd> / <kbd>x</kbd> / <kbd>c</kbd>** - toggles element at `south west` / `south` / `south east` direction of the cursor, if can.

* **<kbd>u</kbd> / <kbd>i</kbd> / <kbd>o</kbd>** - moves cursor at `north west` / `north` / `north east` direction, if can.
* **<kbd>m</kbd> / <kbd>,</kbd> / <kbd>.</kbd>** - moves cursor at `south west` / `south` / `south east` direction, if can.

* **<kbd>escape</kbd> / <kbd>backspace</kbd>** - cancels action or if can't, exists the game.
* **<kbd>return</kbd> / <kbd>space</kbd>** - accepts action

* **<kbd>1</kbd> to <kbd>0</kbd>** - loads example map from one of 10 slots
* **<kbd>shift</kbd> + <kbd>1</kbd> to <kbd>shift</kbd> + <kbd>0</kbd>** - saves current map to one of 10 slots 

## Built With

* [Amiga C Engine](https://github.com/approxit/ACE) - Game engine
* [Amiga C Engine CLI](https://github.com/approxit/ace-cli) - Build tool
* [amiga-dev](https://github.com/kusma/amiga-dev) - VBCC+VASM+VLINK development environment
* [Photoshop](http://www.adobe.com/products/photoshop.html) - Graphics editor

## Versioning

This project use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/approxit/impsbru/tags). 

## Authors

* **[approxit](https://github.com/approxit)** - initial work

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.