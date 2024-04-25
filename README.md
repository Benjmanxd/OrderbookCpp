# Orderbook written in c++

**This is to record my learning journey in finance application, orders. Please comeby and give me some advise.**

### features roadmap

- [ ] support overfill checking
- [ ] multithreading
- [ ] gui interface
- [ ] database

### Supported order types

- [x] Fill and Kill
- [x] Good till Cancel
- [ ] Fill or Kill

### Build procedure

**Please make sure to install cmake 3.27 in your system**

- NixOS

  ```
  # cmake -B . build -DBUILD_NIX=ON && cmake --build build -j
  ```

- Other Linux Distro

  ```
  # cmake -B . build && cmake --build build -j
  ```
