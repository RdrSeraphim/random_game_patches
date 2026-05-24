# random_game_patches
Random patches / utils I use to fix quirks in games I play.

Note that while I'll explain what each does, whether it Just Works™ after compiling and installing is up for debate. These use hardcoded memory addresses, which may not be the same on your system. If the respective game has an update, hardcoded addresses can (and probably will) break.

I offer no guarantee of portability. Do your research before blindly applying, and by that I mean use a decompiler and verify what's going on makes sense.

## Installation

No binaries here, compile from source with `make`. No special libraries required, everything's statically linked, just move the relevant `*.so` to your game directory and adjust your Steam launch options for the game to include it: `LD_PRELOAD="./name_here.so:$LD_PRELOAD" %command%`

I have the Makefile set to use `clang`. `gcc` will work just fine, just edit the CC line in the `Makefile`.

## Games

### Metro 2033 Redux

#### Gamepad Lobotomizer

**Working?** Yes, as of 20260524.   
**Why?** Despite no real controller being plugged in, my Keychron K2 HE keyboard reports an input device with joystick events (prolly because hall effect switches). Unlike every other game I've played with this keyboard, this game insists that I must have an Xbox controller. Instead of manually `chmod`'ing the specific device to `000` or fiddling with `udev` rules, this makes `cinput_manager_core::gamepad_mode(void)` return `0` to ensure that whatever calls it understands "nope, no gamepad here, boss." I did attempt to have `cinput_manager_core::gamepad_connected(void)` do this, but it appears that the function doesn't get called sufficiently to do the trick.

