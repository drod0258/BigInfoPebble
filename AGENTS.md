## Project Overview

This is a Pebble watchface written in C using the Pebble SDK.

## Supported Platforms

The app targets multiple Pebble watch models:
- aplite (Pebble classic)
- basalt (Pebble Time)
- diorite (Pebble 2)
- emery (Pebble Time 2)
- flint (Pebble 2 Duo)
- gabbro (Pebble Round 2)

## Commands

```bash
# Build the app for all platforms
pebble build

# Clean build artifacts
pebble clean

# Install the app on specific emulator
pebble install --emulator emery

# Screenshot the running emulator
pebble screenshot --scale 6 --no-open screenshot.png
```

If you need more information on the `pebble` command or a sub-command, append `--help`.

## Verify your work

After every change, build, install, look at the result and read the logs. Do not stop at a successful build.

```bash
pebble build
pebble install --emulator emery --logs   # streams APP_LOG output; Ctrl-C once the app is up
pebble screenshot --scale 6 --no-open screenshot.png   # then view screenshot.png
```

`pebble logs --emulator emery` streams until Ctrl-C, so run it in a second terminal or in the background while you interact with the app, and stop it before moving on.

Only keep a connection open (`pebble logs`, `--logs`) while you actually need its output. On a physical watch (`--phone` / `--cloudpebble`), an open log stream keeps Bluetooth active and drains the battery of the watch and phone. Leave out `--logs` when you only need to install, and stop any log stream as soon as you have what you need.

Repeat with `--emulator <platform>` for each platform in `targetPlatforms` in package.json. Round displays (gabbro) and the smaller screens (aplite, diorite) need their own check.

If the emulator gets into a strange state, such as a screenshot that shows a different app or an install that times out, `pebble kill` stops it and `pebble wipe` resets its storage, and `pebble clean` clears the build cache; then install again.

### Headless Environments

If you're running in an environment without a window server (e.g., headless Linux, Docker, CI), you must add `--vnc` to **all commands that interact with the emulator**. This includes app installs, screenshots, button presses, and any `emu-*` commands:

```bash
pebble install --emulator basalt --vnc
pebble screenshot --vnc --scale 6 --no-open screenshot.png
pebble emu-button --emulator basalt --vnc click select
```

The `--vnc` flag enables a VNC-based display backend that doesn't require X11.

## Project Structure

```
src/c/           - C source files for the watchface
src/pkjs/        - PebbleKitJS files
resources/       - Images, fonts, and other resources
```

## Architecture

BigInfo is a native C watchface with a PebbleKit JS companion for phone-side data and configuration:

1. **Watchface and rendering**: `src/c/main.c` creates one full-screen window and composes the face from text layers and custom-drawn battery meters for time, date, weather, health, sun/moon, and connection status. Layout adapts to display shape, size, and unobstructed area.
2. **Updates and state**: The C layer owns display state and persisted settings. Minute ticks, battery and Bluetooth changes, and watch health data update the face; settings and data requests/responses are exchanged with JS over AppMessage.
3. **Phone companion and configuration**: `src/pkjs/` uses Clay settings and phone APIs to handle configuration, location, weather, sun/moon calculations, and phone battery data. `package.json` defines the watchface target, platforms, resources, capabilities, and shared message keys.

## SDK Documentation

The full Pebble SDK documentation is available at https://developer.repebble.com.

An index of every page is at https://developer.repebble.com/llms.txt. Use it to discover what's available. Every page also has a Markdown version: append `.md` to any documentation URL to fetch plain Markdown instead of HTML (e.g. `https://developer.repebble.com/guides/events-and-services/buttons.md`). Prefer the `.md` form when reading docs.

Main Categories:
- Tutorials - Step-by-step learning (C watchface tutorial in 5 parts, advanced topics)
- Developer Guides - Comprehensive reference organized by topic

Key Sections:
- App Resources - Images, fonts, vector graphics, 256 resource limit
- User Interfaces - Layer hierarchy, TextLayer, MenuLayer, round vs rectangular displays
- Events & Services - Buttons, accelerometer, compass, health data, background workers
- Communication - Bluetooth AppMessage, PebbleKit JS/Android/iOS integration
- Graphics & Animations - Drawing APIs, property animations, vector graphics
- Debugging - App logs, GDB, common errors and solutions
- Best Practices - Multi-platform support, battery conservation, modular architecture
- Design & Interaction - Glance-first design, one-click actions, platform guidelines
- App Store Publishing - Submission requirements, assets, analytics

Key Entry Points:
- https://developer.repebble.com/tutorials/watchface-tutorial/part1 - C development start
- https://developer.repebble.com/guides/events-and-services/buttons - Button handling
- https://developer.repebble.com/guides/user-interfaces/layers - UI foundations

## Development Best Practices

- Whenever making changes, run `pebble screenshot --scale 6 --no-open screenshot.png` and view the screenshot to make sure it's what the user requested. If not, make more changes until it does what it's supposed to.

## AI Code Review Guidelines

- Once you think you've fulfilled the user's request, ask yourself if you see any issues with the current screenshot, and if there are any differences between the screenshot and the reference image or the user's description. If so, fix them.
