# Repository Guidelines

## Project Structure & Module Organization

This is a C++17 interview-practice repository built with CMake. `main.cpp` is the executable entry point and demonstrates selected implementations. Topic-specific headers are grouped in directories such as `ArrayHashTest/`, `BinaryTreeTest/`, `GraphTest/`, `LRUCacheTest/`, `MySortTest/`, `ThreadTest/`, and `DynamicProgrammingTest/`. Shared utilities include `HFrequency.h`; generated binaries and CMake files belong under `build/` and should not be edited by hand. Add new exercises to a descriptive topic directory and keep implementation-focused headers near their related examples.

## Build, Test, and Development Commands

Use an out-of-source build and ensure CMake 3.20+ is installed:

```powershell
cmake --preset windows-vs2022
cmake --build --preset windows-vs2022-debug
.\build\windows-vs2022\Debug\CodeForInterview.exe
```

On macOS, use `cmake --preset mac-clang` followed by `cmake --build --preset mac-clang-debug`. The project currently has no registered CTest targets; validate changes by compiling and running the executable, then exercise the relevant header or add a small focused call in `main.cpp` during development.

## Coding Style & Naming Conventions

Use C++17 language features, four-space indentation, braces on the same line, and readable standard-library types. Use `PascalCase` for classes and topic directories, `camelCase` for local variables and functions, and descriptive header names (for example, `LRUCache.h`). Keep headers self-contained, avoid unnecessary global state, and preserve the existing include style. No formatter or linter is configured, so keep diffs focused and format consistently with nearby code.

## Testing Guidelines

There is no external unit-testing framework or coverage threshold. For algorithm changes, check normal, boundary, empty-input, duplicate-input, and invalid-input cases where applicable. Prefer deterministic examples and document expected behavior in comments or a focused executable path. Always perform a clean configure/build when changing CMake or shared headers.

## Commit & Pull Request Guidelines

Recent commits use short, imperative descriptions such as `add divide test` and `update lru cache`. Follow that style, keep each commit focused, and mention the affected topic. Pull requests should explain the algorithm or behavior changed, list validation commands and results, and call out platform-specific assumptions. Include sample output when runtime behavior changes; screenshots are unnecessary for this command-line project.

## Security & Configuration Tips

Do not commit generated files, IDE caches, binaries, or secrets. Keep local build output in `build/`, and review `git status` before committing.
