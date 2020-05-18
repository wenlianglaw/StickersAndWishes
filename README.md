# StickersAndWishes
A program to manage the stickers and wishes, bringing a joylife to ur gf (or bf).

## How to use ##
### Install ###
```make```

### Run ###
```./main```

## Overview ##
This is a program to manage stickers and wishes.   Users can add or remove any amount stickers, and exchange stickers with wishes,  for example, exchange 10 stickers with 1 wish.

Initial view.
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

Stickers:   How many stickers do you have.

Section 1:  Display all the current wishes.

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
