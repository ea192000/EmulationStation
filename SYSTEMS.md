# Systems

This document explains how to add systems to EmulationStation through `es_systems.cfg`.

It is intended as a practical reference for the current codebase, not as a complete list of every emulator command you may want to use.

Overview
========

A system is defined inside a `<system>` tag within `<systemList>`.

Minimal example:

```xml
<systemList>
	<system>
		<name>snes</name>
		<fullname>Super Nintendo Entertainment System</fullname>
		<path>~/roms/snes</path>
		<extension>.smc .sfc .zip</extension>
		<command>retroarch -L /path/to/core %ROM%</command>
		<platform>snes</platform>
		<theme>snes</theme>
	</system>
</systemList>
```

Required fields
===============

The current loader requires the following fields to be present and non-empty:

* `<name>`
* `<path>`
* `<extension>`
* `<command>`

`<fullname>` is strongly recommended, but the loader primarily validates the fields above.

Field reference
===============

### `<name>`
Short internal name for the system.

Example:
```xml
<name>psx</name>
```

### `<fullname>`
Human-readable system name shown in the UI.

Example:
```xml
<fullname>PlayStation</fullname>
```

### `<path>`
Base folder where EmulationStation starts scanning for games.

Notes:
* `~` is expanded to the user's home directory.
* Paths are normalized internally.
* By default, subfolders are scanned recursively unless the `ShowSubFolders` setting is disabled.

Example:
```xml
<path>~/roms/psx</path>
```

### `<extension>`
Whitespace-separated list of file extensions to include.

Notes:
* Extensions are case-sensitive.
* The leading `.` is required.
* Duplicate extensions are ignored.

Example:
```xml
<extension>.cue .bin .img .pbp .chd</extension>
```

### `<command>`
Command line used to launch the selected game.

Supported substitutions:

* `%ROM%` - absolute path to the selected ROM, escaped for shell usage.
* `%ROM_RAW%` - absolute path to the selected ROM without escaping.
* `%BASENAME%` - base filename without directory or extension.

Example:
```xml
<command>duckstation-qt -batch "%ROM_RAW%"</command>
```

### `<platform>`
Whitespace- or comma-separated list of scraping platform identifiers.

This field is optional, but recommended. Unknown platform values generate a warning and are ignored.

If the special platform `ignore` is used, scraping is disabled for that system.

Example:
```xml
<platform>genesis megadrive</platform>
```

### `<theme>`
Theme folder name to use for the system.

If omitted, EmulationStation uses the value of `<name>`.

Example:
```xml
<theme>megadrive</theme>
```

Current platform values
=======================

The accepted platform names come from `es-app/src/PlatformId.cpp`.

Currently supported values are:

`3do`, `amiga`, `amstradcpc`, `apple2`, `arcade`, `atari800`, `atari2600`, `atari5200`, `atari7800`, `atarilynx`, `atarist`, `atarijaguar`, `atarijaguarcd`, `atarixe`, `bbcmicro`, `colecovision`, `c64`, `daphne`, `intellivision`, `macintosh`, `xbox`, `xbox360`, `msx`, `neogeo`, `neogeocd`, `ngp`, `ngpc`, `n3ds`, `n64`, `nds`, `fds`, `nes`, `pokemini`, `channelf`, `gb`, `gba`, `gbc`, `gc`, `wii`, `wiiu`, `virtualboy`, `gameandwatch`, `switch`, `openbor`, `pc`, `sega32x`, `segacd`, `dreamcast`, `gamegear`, `genesis`, `mastersystem`, `megadrive`, `saturn`, `sg-1000`, `samcoupe`, `psx`, `ps2`, `ps3`, `ps4`, `psvita`, `psp`, `snes`, `scummvm`, `x1`, `x68000`, `solarus`, `pico8`, `tic80`, `moto`, `pc88`, `pc98`, `pcengine`, `pcenginecd`, `pcfx`, `wonderswan`, `wonderswancolor`, `zxspectrum`, `zx81`, `videopac`, `vectrex`, `trs-80`, `coco`, `ti99`, `dragon32`, `zmachine`, `fmtowns`, `gamecom`, `phone`, `odyssey`, `scv`, `cv`, `vcs`, `cdi`, `vmu`, `fantasy`, `ignore`

Behavior notes
==============

* A system with no valid games is ignored.
* Hidden files are skipped unless `ShowHiddenFiles` is enabled.
* Directories can be added as games if their names match a configured extension.
* Directories that do not match an extension are added as folders when subfolder scanning is enabled.
* Systems are shown in the same order they appear in `es_systems.cfg`.
* If `ParseGamelistOnly` is enabled, filesystem scanning is skipped and only gamelist entries are used.
* If `IgnoreGamelist` is enabled, `gamelist.xml` metadata is ignored.

Examples
========

## Nintendo Wii / GameCube with Dolphin

```xml
<system>
	<name>wii</name>
	<fullname>Nintendo Wii</fullname>
	<path>~/roms/wii</path>
	<extension>.elf .dol .gcm .iso .wbfs .ciso .gcz .wad</extension>
	<command>dolphin-emu -b -e %ROM%</command>
	<platform>wii</platform>
	<theme>wii</theme>
</system>
```

```xml
<system>
	<name>gc</name>
	<fullname>Nintendo GameCube</fullname>
	<path>~/roms/gc</path>
	<extension>.elf .dol .gcm .iso .wbfs .ciso .gcz .wad</extension>
	<command>dolphin-emu -b -e %ROM%</command>
	<platform>gc</platform>
	<theme>gc</theme>
</system>
```

## Nintendo 64 with RetroArch

```xml
<system>
	<name>n64</name>
	<fullname>Nintendo 64</fullname>
	<path>~/roms/n64</path>
	<extension>.z64 .n64 .zip</extension>
	<command>retroarch --fullscreen -L /usr/lib/libretro/mupen64plus_libretro.so %ROM%</command>
	<platform>n64</platform>
	<theme>n64</theme>
</system>
```

## Sony PlayStation Portable with PPSSPP

```xml
<system>
	<name>psp</name>
	<fullname>PlayStation Portable</fullname>
	<path>~/roms/psp</path>
	<extension>.iso .cso</extension>
	<command>ppsspp --fullscreen --escape-exit "%ROM_RAW%"</command>
	<platform>psp</platform>
	<theme>psp</theme>
</system>
```

## Steam shortcut system

```xml
<system>
	<name>steam</name>
	<fullname>Steam</fullname>
	<path>~/.emulationstation/steam</path>
	<extension>.txt</extension>
	<command>steam steam://rungameid/$(tail %ROM%)</command>
	<platform>pc</platform>
	<theme>steam</theme>
</system>
```

For the Steam example, each `.txt` file represents one game and contains the Steam application ID.
