### Requirements

You should download the Arduino IDE to compile the src to your board.

This project uses an Arduino Nano ATMega168 board, or clone. Much of the work on this project was completed using a Mac computer, and if you're using a clone, you will need to [download drivers](https://github.com/adrianmihalko/ch340g-ch34g-ch34x-mac-os-x-driver).

If using a Debian box of some kind, don't use apt to install, as [this](https://playground.arduino.cc/Linux/Debian) site warns. Download the binary from their site.

The LinkedList project is in the Sketch, partiall because there is an unmerged pull request for a bug fix (fixed in this project).

### Tests

```
g++ -std=c++14 tests.cpp -o tests && ./tests
```

### Designs

These parts in `3d_designs/reused_parts/` were incorporated from other projects:

    main_wheel.stl -> https://github.com/davidhrbaty/ArduBot
    small_wheel.stl -> https://github.com/davidhrbaty/ArduBot
    small_wheel_holder.stl -> https://github.com/davidhrbaty/ArduBot
    SG90-A5.stl -> https://www.thingiverse.com/thing:2282898

### Reading

Naming things is hard, so I've reused common names for components by reading [this](http://www-anw.cs.umass.edu/rlr/terms.html).
