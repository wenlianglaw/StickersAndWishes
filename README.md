# StickersAndWishes
StickersAndWishes is a console program to manage the stickers and wishes, bringing a joylife to ur gf (or bf).  It's a small program that doesn't rely on any dependencies and easy to build and play with.

## How to use ##
### Install ###
```make```

### Run ###
```./main```

## Overview ##
Users can add or remove any amount stickers, and exchange stickers for wishes,  e.g, exchange 10 stickers for 1 wish.

```
----------------------------------------------------------------------
| Stickers: x                                                          |
|_______________________________                                       |
|                               |                                      |
|                1              |                                      |
|_______________________________|                                      |
|                                                                      |
| Menu                                                                 |
|1...                                                                  |
|2...                                                                  |
|3...                                                                  |
|                                                                      |
| Command:_                                                            |
|                                                                      |
|                                                                      |
|----------------------------------------------------------------------|
```

Stickers:   How many stickers you currently have.

Section 1:  Display all the wishes.

Command:  Command from keyboard.  

## Features ##

- Add/Remove stickers
- Add/Use a wish.
- Search wishes.
- Mofidy a wish.
- Save to / load from disk.



## Menu ##

0: Refresh wishes.

1: Lookup wishes.

2: Use a wish.

3: Convert stickers to wish.

4: Add a new wish.

5: Modify a wish.

+: Add stickers.

-: Remove stickers.

q: Exit.

## Some Notes ##

### Save and Load ###

json is a good choice but it brings extra 3rd party libraries to this project.
I try to use as less libraries as possible to keep the project lite and
make the code comipilable among different OS.

Moreover, the data structure is quite simple.  Based on all of these I decided
to write bytes into the file.

### rlutil library ###

I used the rlutil::cls() and rlutil::getch() funtion from this library in this project.
