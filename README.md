About
-----
**A silly tool for reversing strings.**

- Made with Raylib (raysan5).
- rlImGui (raylib-extras)
- With SDL3 as a backend (libsdl-org).
- Design made with ImGui (ocornut).
- CMakeRC/cmrc (vector-of-bool)
- Cousine Nerd Font by Steve Matteson

Updates
--------
- v2.1.0 released
- Raylib updates have allowed for seemless SDL3 integration with no library changes.
- Also, ImGui integration with rlImGui.
- Hell Yes.
- Total Refactor.

# As-Is No Warranties

Current Progress
-----------------
![Looking pretty good so far](ReverseGUI.jpg)

Usage
---------
### There are various keyboard shortcuts. Some provided default by SDL, and others I've added.
### Known Working:
```
<C-c> Copy
<C-p> Paste
<Tab> Next Form
<S-Tab> Prev Form
<Esc> Kill
```
### Semi Working
```
<a> Add Row
<s> Sub Row
<r> Reset
<Enter> Submit
```

TODO
-----
- Fix bug where program crashes when dragging window size smaller than default.

VENDOR LIBRARY CHANGES
---------------
- None at the moment.
- 3rd party libraries seem to be mingling nicely.

BUILDING
--------
Building should be as simple as using two cmake commands:
```shell
cmake -B build -D CMAKE_BUILD_TYPE=[Debug | Release]
cmake --build build --config [Debug | Release]
```

INSTALLATION
------------
Binaries get copied into "install" at the project root.
```shell
cmake --install build --config [Debug | Release]
```

LICENSE
-------
- Raylib is released under zlib license.
- rlImGui is released under zlib license.
- SDL is released under zlib license.
- ImGui is released under MIT license.
- CMakeRC/cmrc is released under MIT license.

UNCERTAIN
----------
- Cousine Nerd Font is release under... I'm not exactly sure.
- If Steve Matteson requests a takedown, I will accept his wishes. (Please Don't)

```
My personal code for this project is public domain (not that it'll be useful anyways).
No fees
No limitations
No warranties
No attribution necessary 
```
