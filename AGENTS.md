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

Run checks appropriate to the change. Non-functional-only edits that cannot affect app behavior or appearance—such as README/documentation or comment edits—do not require a build or emulator testing.

For functional app changes, build and validate on the `emery` emulator; do not stop at a successful build. Inspect a screenshot for changes that affect the watchface's appearance, and review logs for changes that affect runtime behavior.

```bash
pebble build
pebble install --emulator emery --logs   # for runtime changes; streams APP_LOG output
pebble screenshot --scale 6 --no-open screenshot.png   # for visual changes; then view screenshot.png
```

`pebble logs --emulator emery` streams until Ctrl-C, so run it in a second terminal or in the background while you interact with the app, and stop it before moving on.

Only keep a connection open (`pebble logs`, `--logs`) while you actually need its output. On a physical watch (`--phone` / `--cloudpebble`), an open log stream keeps Bluetooth active and drains the battery of the watch and phone. Leave out `--logs` when you only need to install, and stop any log stream as soon as you have what you need.

By default, validate only `emery`; test another platform or all `targetPlatforms` only when explicitly instructed. When requested, round displays (`gabbro`) and smaller screens (`aplite`, `diorite`) need their own checks.

Before installing a new build for emulator validation, run `pebble kill` and `pebble wipe` to stop the emulator and clear cached app/settings state; otherwise stale settings can make the new build appear unchanged. If issues persist, `pebble clean` clears the build cache; then install again.

After automated emulator validation, run `pebble kill` and `pebble wipe` to stop the emulator and clear its state. Leave the emulator running only when explicitly asked to run it.

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

- For changes that affect the watchface's appearance, run `pebble screenshot --scale 6 --no-open screenshot.png` and inspect it against the user's request. No screenshot is needed for non-functional-only edits such as README or comment changes.
- Use `./debug.sh enable` to preview optional fields with mock step/heart-rate values; weather and sun still use the configured location/network, and heart-rate display must be enabled separately. Run `./debug.sh disable` after testing and inspect `git diff`: it edits `src/c/main.c` and `src/pkjs/index.js` in place, resetting hard-coded defaults rather than restoring prior values.

## AI Code Review Guidelines

- For visual changes, check the screenshot for issues or differences from the reference image or user's description, and fix them before finishing.
