# AttDef

## If you just want to run the server, you're looking for the [latest release](https://github.com/ZantetsukenGT/AttDef/releases/latest), you may also want to read the [Runtime prerrequisites](#runtime-prerrequisites) and [Actually running the server](#actually-running-the-server) sections

# Table of Contents

- [Build prerrequisites](#build-prerrequisites)
- [Runtime prerrequisites](#runtime-prerrequisites)
  - [Linux](#linux)
  - [Windows](#windows)
- [How to build](#how-to-build)
- [Just before running the server](#just-before-running-the-server)
  - [Delete server.cfg](#delete-servercfg)
  - [Plugins and components](#plugins-and-components)
  - [Scriptfiles](#scriptfiles)
  - [config.json](#configjson)
- [Actually running the server](#actually-running-the-server)
  - [Setting up the server to automatically start](#setting-up-the-server-to-automatically-start)
- [Collaboration and Pull Requests](#collaboration-and-pull-requests)


# Build prerrequisites
Install the following in order to compile the gamemode:
- Git v2.39.x, because sampctl requires it
- [sampctl version 1.16.0](https://github.com/Southclaws/sampctl/releases/tag/1.16.0)
  - Following [sampctl's config wiki](https://github.com/Southclaws/sampctl/wiki/Configuration), configure `git_username` and `github_token` within `config.json` or you may hit github's rate limits.
    - [`C:\Users\<User>\AppData\Roaming\sampctl`] on Windows
    - Possibly [`~/.sampctl`] on linux

# Runtime prerrequisites
## Linux
If you're planning to run the server on a Linux machine, keep in mind that samp-server is still a 32bit process as of now.

You may need to install the i386 architecture and install the 32bit version of the C++ Standard Library runtime, the installation procedure may vary depending on your distro:

If your package manager is APT (Debian, Ubuntu, etc), you can achieve this by running:

```bash
dpkg --add-architecture i386 && apt-get update && apt-get install libstdc++6:i386
```

If your package manager is pacman (Arch Linux), you can achieve this by running:

```bash
pacman -Syu lib32-gcc-libs
```

If you're unsure, research how to do this on your machine!

## Windows
If you're planning to run the server on a Windows machine, you **may or may not** need to install the [MVSC runtimes.](https://www.techpowerup.com/download/visual-c-redistributable-runtime-package-all-in-one/)

# How to build

Clone the project or download the project in a zip archive:

```bash
git clone https://github.com/ZantetsukenGT/AttDef attdef
#linux
cd ./attdef
#windows
cd .\attdef
```

Fetch the compatibility include and needed libraries/dependencies; the following commands will fetch plugins as well:

```bash
powershell -ExecutionPolicy Bypass -File .\scripts\fetch-gbugfix.ps1
sampctl ensure
```

Before building, copy `modules/iphub-key.local.example.inc` to `modules/iphub-key.local.inc` and set your IPHub API key there. The local file is git-ignored; exclude it from source archives. If a key has been shared, revoke it with IPHub and replace it with a new one.

The open.mp compiler is pinned to version `3.10.11`. `pawn.lock` pins direct and transitive dependency commits. `gBugFix` is fetched separately at a fixed commit because its package metadata requests standard-library branches that conflict with the open.mp compiler dependencies. The lock intentionally omits optional worktree-integrity hashes: sampctl 1.16 can report false mismatches for valid Windows checkouts, while the commit pins remain enforced. To refresh dependency revisions, run `powershell -ExecutionPolicy Bypass -File .\scripts\refresh-lockfile.ps1`.

The local Weapon Config project patch (HUD refresh and safe paused-player sync using the current server position) targets source revision `c7512b7a4e2a8412fe9ff14337ef8924e8ff74cb`. After fetching dependencies, prepare that dependency with:

```bash
powershell -ExecutionPolicy Bypass -File .\scripts\prepare-dependencies.ps1
```

The script refuses to replace a dependency checkout with unrelated local changes. It is safe to rerun after `sampctl ensure`.

Compile the gamemode to generate the attdef.amx file needed to run the server:

```bash
#Will build with debug symbols -d3 flag (debug symbols included, development and for crashdetect to output useful info)
sampctl build
#or
sampctl build dev
```

```bash
#Will build with optimizations and no debug symbols, mainly for release builds
sampctl build release
```

GitHub Actions runs both builds on pushes and pull requests using the pinned sampctl and open.mp compiler versions, validates the dependency lock, and verifies the pinned compatibility include and Weapon Config patch before compiling.

# Just before running the server

## Server configuration and Discord Bot

open.mp uses `config.json` for general server configuration, while the `discord-connector` plugin reads the bot token from `server.cfg`:

1. Copy `config.json.example` to `config.json` and adjust server port, name, and RCON password.
2. If using Discord integration, copy `server.cfg.example` to `server.cfg` and set your `discord_bot_token`.
3. Copy `scriptfiles/discord.cfg.example` to `scriptfiles/discord.cfg` and set your Discord channel IDs, role IDs, and server IP (loaded dynamically at runtime — no recompilation required).
4. Copy `modules/iphub-key.local.example.inc` to `modules/iphub-key.local.inc` and configure your API key.

## Plugins and components

Ensure all required plugins are present in the `plugins/` (or `components/`) directory as declared in `config.json`:
- `crashdetect`
- `samp_bcrypt`
- `sscanf`
- `streamer`
- `ColAndreas`
- `discord-connector`

## Scriptfiles

In the `scriptfiles/` folder:

1. Make a copy of `AttDefDatabase.db.example` and rename it to `AttDefDatabase.db`.
2. Back up `AttDefDatabase.db` before upgrading the gamemode. On startup, the gamemode applies pending SQLite schema migrations transactionally and records them in `SchemaMigrations`.
3. For ColAndreas map collision support (team pings, sniper raycasting), place `ColAndreas.cadb` under `scriptfiles/colandreas/`. 

# Actually running the server
Use your preferred way to start the server:

```bash
#On Windows
.\samp-server.exe

#On Linux
./samp-server
```

This works fine until you need to log off, on Windows, the process will stay alive as long as the machine is turned on and in Linux the process will be terminated.

## Setting up the server to automatically start

So, on Linux, using [`pm2`](https://pm2.io/) is recommended, it will help you in restarting the server upon crashes and start it automatically when you reboot your machine.

Just casually follow these steps:

1. Install [nvm](https://github.com/nvm-sh/nvm)
```bash
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.3/install.sh | bash
```

2. Log off and log in again to complete the nvm installation

3. Install the latest version of NodeJS

```bash
nvm install node # "node" is an alias for the latest version
nvm use node
```

4. Install pm2
```bash
npm install pm2 -g
```

5. Tell pm2 to run on boot
```bash
#Run as root or use sudo
pm2 startup
```

6. Start your server with pm2
```bash
# very important to be inside the correct folder
cd path/to/server
pm2 start "./samp-server" --name myAttdef
pm2 save
```

You're done!

Other useful pm2 commands:

```bash
pm2 list
pm2 log myAttdef
pm2 stop myAttdef
pm2 restart myAttdef
pm2 delete myAttdef
pm2 describe myAttdef
```

## Collaboration

This project is open for collaboration, create a fork, start from [How to build](#how-to-build) and make a pull request!

## Credits & Authors

- **062_, Whitetiger, Khalid & Beijind:** Original Attack-Defend gamemode creators.
- **Volk, Chillax, Excision, cYs & NTL:** UGMP bases, arenas, maps and legacy contributions.
- **ZantetsukenGT:** open.mp port, weapon-config integration, fixes and baseline modernization (up to v1.38).
- **jizzy b:** Lead developer for v1.42.0 - Full modular architecture, event-driven observer pattern, AC Black Diamond anti-cheat integration, team ping system with ColAndreas raycasting, complete textdraw overhaul & HUD styles (/style), modern Discord bot integration, SQLite migrations, and UTF-8 standardization.
