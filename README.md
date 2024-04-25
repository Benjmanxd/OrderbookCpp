# Orderbook written in c++

**This is to record my learning journey in finance application, orders. Please comeby and give me some advise.**

### features roadmap

- [ ] support overfill checking
- [ ] multithreading
- [ ] gui interface
- [ ] database (MySQL, Redis)

### Supported order types

- [x] Fill and Kill (fill whatever quantity it can and kill the order) (IOC)
- [ ] Fill or Kill (fill all quantity or none, then kill immediately) (AON, IOC)
- [x] Good till Cancel (remain active until filled or cancelled)
- [ ] Good for Day (fill until the end of the trading day)
- [x] Market (fill at best available price)
- [ ] Limit (fill at specific price)

### Build procedure

**Please make sure to install cmake 3.27 in your system**

- Linux

  ```
  # cmake -B . build && cmake --build build -j
  ```
