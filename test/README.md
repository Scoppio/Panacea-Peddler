# Lua in FCEUX

Now then, let's get to the emulator specifics!

To load a Lua script in FCEU first load a rom (Lua can only do things after each frame cycle so load a rom first). Go to file, at the bottom choose Run Lua Script and select and load the file.

When Lua starts, the emulator pauses and hands control over to Lua. Lua (that's you!) decides when the next frame is processed. That's why it's very common to write an endless while loop, exiting the main loop of a script will exit the script and hand control back to the emulator. This also happens when a script unexpectingly crashes.

A bare script looks like this:

    ```
        while (true) do
            emu.frameadvance();
        end;
    ```

And is about equal to not running Lua at all. The frameadvance function is the same called internally, so no loss of speed there!

## Bitwise operators

Lua does not have bitwise operators, so we supply some for you. These are common bitwise operators, nothing fancy.

AND(a,b);

OR(a,b);

XOR(a,b);

BIT(n); -- returns a number with only bit n set (1)

The emulator specific Lua is equal to the one of snes9x, with some platform specific changes (few buttons, for instance).

You can find the reference here: <http://dehacked.2y.net/snes9x-lua.html>

The following is a quick reference, you can go to the snes9x reference for more details.

To paint stuff on screen, use the gui table. This contains a few predefined functions to manipulate the main window. For any coordinate, 0,0 is the top-left pixel of the window. You have to prevent out-of-bound errors yourself for now. If a color can be passed on, it is a string. HTML-syntax is supported ("#34053D"), as well as a FEW colors ("red", "green", "blue" ...).

gui.text(x, y, str); -- Print a line to the window, you can use \n for a return but it will only work once

gui.pixel(x, y, color); -- plot a pixel at the given coordinate

gui.line(x1, y1, x2, y2, color); -- plot a line from x1,y1 to x2,y2

gui.box(x1, y1, x2, y2, color); -- draw a square from x1,y1 to x2,y2

gui.popup(str); -- pops up a messagebox informing the user of something. Real handy when debugging!

gui.getpixel(x,y); -- return the values of the pixel at given position. Returns three numbers of the emulator image before paiting is applied.

gui.gdscreenshot(); -- Takes a screen shot of the image and returns it in the form of a string which can be imported by the gd library using the gd.createFromGdStr() function

(for more gd functions see DeHackED's reference: <http://dehacked.2y.net/snes9x-lua.html>)

PAINTING IS ALWAYS ONE FRAME BEHIND! This is because the painting is done at the creation of the next frame, not while Lua is running.

## Emulator control

emu.frameadvance(); -- advances emulation ONE frame

emu.pause(); -- same as pressing the pause button

emu.speedmode(strMode); -- Supported are "normal","turbo","nothrottle","maximum". But know that except for "normal", all other modes will run as "turbo" for now.

emu.wait(); -- skips the emulation of the next frame, in case your script needs to wait for something

## Memory control

memory.readbyte(adr); -- read one byte from given address and return it. Besides decimal values Lua also allows the hex notation 0x00FA. In FCEUX reading is done BEFORE the cheats are applied!

memory.writebyte(adr, value); -- write one byte to the RAM of the NES. writing is done AFTER the hexeditor receives its values, so if you are freezing an address by Lua, it will not show in the hex editor (but it will in the game :)

memory.readbytesigned(adr); -- same as readbyte, except this returns a signed value, rather then an unsigned value.

memory.register(adr, function); -- binds a function to an address. The function will be called when an address changes. NOTE THAT THIS IS EXPENSIVE (eg.: slow)! Only one function allowed per address.

## Input control

You can read and write input by using the joypad table. A input table has the following (case sensitive) keys, where nil denotes they are not to be pressed: up down left right start select A B

The input table must have a true or false value for the keys you want.

joypad.read(playern); -- get the input table for the player who's input you want to read (a number!)

joypad.write(playern, inputtable); -- set the input for player n. Note that this will overwrite any input from the user, and only when this is used.

## Savestates

You can load and save to the predefined savestates 1 ... 9 or create new "anonymous" savestates. You must first create a savestate object, which is your handle to a savestate. Then you can pass this handle on to savestate.load or save to do so.

savestate.create(n); -- n is optional. When supplied, it will create a savestate for slot n, otherwise a new (anonymous) savestate object is created. Note that this does not yet save or load anything!

savestate.load(state); -- load the given savestate

savestate.save(state); -- save the given savestate
