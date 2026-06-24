Gamelists
=========

The `gamelist.xml` file for a system defines metadata for games and folders.

ES checks the following locations in order and uses the first `gamelist.xml` it finds:

* `[SYSTEM_PATH]/gamelist.xml`
* `~/.emulationstation/gamelists/[SYSTEM_NAME]/gamelist.xml`
* `/etc/emulationstation/gamelists/[SYSTEM_NAME]/gamelist.xml`

Example:

```xml
<gameList>
	<game>
		<path>/home/pi/ROMs/nes/mm2.nes</path>
		<name>Mega Man 2</name>
		<desc>Mega Man 2 is a classic NES game.</desc>
		<image>~/.emulationstation/downloaded_images/nes/mm2-image.png</image>
		<thumbnail>~/.emulationstation/downloaded_images/nes/mm2-thumb.png</thumbnail>
		<marquee>~/.emulationstation/downloaded_images/nes/mm2-marquee.png</marquee>
		<favorite>true</favorite>
	</game>
</gameList>
```

Everything is enclosed in a `<gameList>` tag. Each entry is stored inside either a `<game>` or `<folder>` tag. All metadata values are stored as strings in XML, even when they represent numbers, booleans or dates.

Reference
=========

The source of truth for supported metadata is `es-app/src/MetaData.cpp`.

Metadata types
--------------

* `string` - plain text.
* `multiline string` - plain text intended for longer descriptions.
* `path` - a file path. Paths may be absolute, relative to the system folder when prefixed with `./`, or relative to the user's home directory when prefixed with `~/`.
* `rating` - a floating-point value between `0` and `1`.
* `integer` - an integer stored as a string.
* `boolean` - stored as `true` or `false`.
* `date/time` - stored as an ISO-like compact string such as `19950311T000000`.

Path metadata is automatically resolved when read and written back as a relative path when possible, to keep installations portable.

Some metadata is marked internally as a statistic. These fields are maintained by ES and may be hidden from metadata editing UIs depending on context.

#### `<game>`

Supported metadata fields for `<game>` entries:

* `name` - displayed name.
* `sortname` - alternate name used for sorting.
* `franchise` - franchise or series name.
* `desc` - long description.
* `image` - main image path.
* `video` - video path.
* `marquee` - marquee/logo image path.
* `thumbnail` - smaller image path, typically used by grid-style views.
* `rating` - rating from `0` to `1`.
* `releasedate` - release date.
* `developer` - developer name.
* `publisher` - publisher name.
* `genre` - genre name.
* `subsystem` - subsystem or variant name.
* `favorite` - `true` or `false`.
* `hidden` - `true` or `false`.
* `kidgame` - `true` or `false`.
* `playcount` - number of times played.
* `lastplayed` - last played date/time.
* `players` - supported player count.
* `region` - region string.
* `language` - language string.
* `rate` - age rating string.
* `input` - input type string.
* `aspect` - aspect ratio string.

#### `<folder>`

Supported metadata fields for `<folder>` entries:

* `name` - displayed name.
* `sortname` - alternate name used for sorting.
* `franchise` - franchise or series name.
* `desc` - description.
* `image` - main image path.
* `thumbnail` - smaller image path.
* `video` - video path.
* `marquee` - marquee/logo image path.
* `rating` - rating from `0` to `1`.
* `releasedate` - release date.
* `developer` - developer name.
* `publisher` - publisher name.
* `genre` - genre name.
* `subsystem` - subsystem or variant name.
* `players` - supported player count.
* `region` - region string.
* `language` - language string.
* `rate` - age rating string.
* `input` - input type string.
* `aspect` - aspect ratio string.

Notes
=====

* ES can scrape much of this metadata automatically.
* If a value matches the default for a field, ES may omit it when writing `gamelist.xml`.
* A `<game>` entry can point to a directory if that directory matches a configured extension.
* Folder metadata is only useful when that folder is actually present in the displayed hierarchy.
* ES keeps entries even if the referenced files are missing.
* `--gamelist-only` skips filesystem scanning and only displays entries defined in `gamelist.xml`.
* `--ignore-gamelist` ignores `gamelist.xml` and forces ES to build the list from the filesystem.
* If at least one game in a system has suitable media metadata, ES may switch to a richer gamelist view depending on the active view style and theme.
* Themes can display metadata beyond the classic fields, including `marquee`, `thumbnail`, `favorite`, `franchise`, `subsystem`, `region`, `language`, `rate`, `input` and `aspect`.
