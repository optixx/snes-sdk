Snes9x: The Portable Super Nintendo Entertainment System Emulator
-----------------------------------------------------------------

 Contents:
  1. Introduction
  2. Requirements
  3. Starting Snes9X
  4. Available command line options
  5. Keyboard controls
  6. Joystick support
  7. Game saving
  8. Emulation speed
  9. Building Snes9X from source
  A. Release notes
  B. Contacting the Snes9X team
  C. Contacting the Unix maintainer
  D. Credits and Thanks


1. Introduction

      The Unix port of Snes9X is part of the main Snes9X development
      tree. For more information about Snes9X in general see
      README.TXT. For Unix specific problems see the release notes
      further down in this document.


2. Requirements

      · C PU
      A 100MHz 486DX4 minimum when using 8-bit graphics and minimal or
      no sound, Pentium 166 or higher for transparency effects and
      Pentium 200 or higher for Super FX and SA-1 games.

      · Memory
      16Mb or more for Linux. Sun workstations shouldn't have a problem.

      · Software
      You will need access to SNES ROM images in *.smc, *.sfc, *.fig
      or *.1, *.2, or sf32xxxa, sf32xxxb, etc., format otherwise you
      will have nothing to run! Some home-brewed ROM images can be
      downloaded from http://www.snes9x.com. To play commercial games
      you need to use a cartridge dumper to extract the content of
      your original cartridges. Please note, it is illegal in most
      countries to have commercial ROM images without also owning the
      actual SNES ROM cartridge, and even then you may be liable under
      various EULAs.

      · Using an X11 display target
      X Window System ports need an 8, 15, 16, 24 or 32 bit X server
      running; transparency effects are available at all depths, but
      don't look good with only an 8-bit display. For maximum
      emulation speed, have the X server switched to 8-bit and don't
      enable transparency effects, or 15 or 16-bit with transparency
      effects enabled.

      · Using the Linux SVGA display target
      The SVGA target is untested and considered deprecated. It will
      probably be removed in some later release.

      · Using the 3dfx glide display target
      If you want to use the 3dfx bi-linear and scaling features of
      Snes9x, you will need a Voodoo 1, 2 or 3 graphics card and the
      glide library installed. The glide target is not maintained or
      tested.

      · Using the OpenGL display target
      If you want to make use of OpenGL support, you'll need to have a
      suitable OpenGL implementation installed. Most newer Linux
      distributions come with Mesa, which will do fine. You'll need to
      make sure that you have hardware OpenGL render drivers linked
      into your X server. Nvidia chipset users will need to download
      the latest Linux drivers from http://www.nvidia.com as the
      Nvidia drivers shipped with the X server don't support all
      Nvidia chipsets or all their accelerated functions.


 3. Starting Snes9X

      From a shell just type:
      snes9x [OPTIONS] <ROM filename>

      ROM images are normally loaded from the directory ./roms. This
      can be changed by specifying a pathname with the image name or
      setting the environment variable SNES9X_ROM_DIR to point to a
      different directory.

      Freeze game files and S-RAM save files are normally read from
      and written to the directory $HOME/.snes96_snapshots.  This can
      be changed by setting the environment variable
      SNES96_SNAPSHOT_DIR to point to a different directory.

      To enable full-screen mode on the Linux X Window System and SVGA
      ports, or to use the Voodoo 3D card without the 3dfx Linux
      kernel driver installed, Snes9x needs special system access
      permissions to allow it to write directly to video RAM and alter
      video chipset register values. THIS IS AN OUTDATED AND DANGEROUS
      METHOD OF GETTING FULL SCREEN. AVOID IT IF POSSIBLE.

      As an alternative for the voodoo port, download and install the
      3dfx kernel driver.


 4. Available Snes9X command line options
      
      · Graphics options:

      -tr or transparency (default: off)
       Enable transparency effects, also enables 16-bit screen mode
       selection.  Transparency effects are automatically enabled if
       the depth of your X server is 15-bit or greater.

      -hires or -hi (default: lo-res.)
       Enable support for SNES hi-res. and interlace modes. USE ONLY
       IF GAME REQUIRES IT (FEW DO) BECAUSE IT REALLY SLOWS DOWN THE
       EMULATOR.

      -y or -interpolate (default: off)
       Enables 'TV mode', hires support, 16-bit internal rendering and
       transparency effects. TV mode scales the SNES image by x2 by
       inserting an extra blended pixel between each SNES pixel and
       80% brightness 'scan-lines' between each horizontal line. The
       result looks very nice but needs a fast machine. Use with the
       full-screen X mode and a 15 or 16 depth X server, or the SVGA
       port for fastest operation.

      -y2 or -interpolate2
       Enable Kreed's Super 2xSaI image processing mode.

      -y3 or -interpolate3
       Enable Kreed's Super Eagle image processing mode.

      -y4 or -interpolate4
       Enable Kreed's 2xSaI image processing mode.

      -y5 or -interpolate5
       Enable Kreed's software bi-linear filtering and image scaling mode.

      -nms or -nomodeswitch (default: switch modes)
       The Linux X Windows System port can change the screen
       resolution when switching to full-screen mode so the SNES
       display fills the whole screen.  Specify this option to stop it
       if causes you problems.

      -fullscreen or -fs
       Start the emulator in full screen mode, rather than a window on
       the X11 port if supported by your X server, a suitable screen
       mode is defined in your XF86Config file and the snes9x binary
       has root permissions (see above).  

      -scale or -sc (default: off)
       Stretch the SNES display to fit the whole of the computer
       display.  Linux X Window System full-screen mode or SVGA port
       only. Use only if you have a very fast machine.

      -displayframerate or -dfr 
       Display a frame rate counter superimposed over the bottom,
       left-hand corner of the SNES display. The value before the
       slash (/) indicates how many frames per second are being
       rendered compared to a real SNES, the value of which is
       displayed after the slash.

      -sdd1-pack <directory>
       Use SDD1 graphics package from this directory. Normally not
       needed since Snes9X can decompress the SDD1 graphics on the fly
       as of Snes9X v1.42.


      · Sound options:

      -ns or -nosound
       Disable sound CPU emulation and sound output, useful for the
       Starfox 2 beta floating around that has corrupt sound CPU code,
       causing the game to hang with sound emulation turned
       on. Disabling sound also speeds up emulation speed, at the
       expense of no music or sound effects during the game and
       reduced game compatibility.

      -sk 0-3 or -soundskip 0-3 (default: 0)
       ONLY USED IF SOUND IS DISABLED. The algorithm to use when
       trying to fool a ROM into thinking that the SPC700 sound CPU is
       there when actual emulation of it is turned off (for speed
       reasons).

      -stereo or -st (default: stereo)
       Enable stereo sound output.

      -mono (default: stereo)
       Enable mono sound output. Slightly faster, but doesn't sound as nice.

      -r 0-7 or -soundquality or -sq 0-7 (default: 4)
       Sound playback rate/quality:
         0 - disable sound, 1 - 8192, 2 - 11025, 3 - 16500, 
         4 - 22050 (default), 5 - 29300, 6 - 36600, 7 - 44000.

      -envx or -ex (default: off)
       Enable volume envelope height reading by the sound CPU. Can
       cure sound repeat problems with some games (e.g. Mortal Kombat
       series), while causing others to lock if enabled (eg. Bomberman
       series). Use with -sy option below to fix the problems this
       option can otherwise create.

      -ts or -threadsound
       Starts to feed data to the sound card using a separate thread
       rather than using timers, signals and polling. Use to work
       around a bug in the beat Sound Blaster Live Linux driver that
       keeps telling Snes9x that the sound card can accept more sound
       data, when it actually cannot and Snes9x slows down or locks
       up. A multi-CPU system could benefit from using this option -
       sound processing could take place on another CPU for speed.

      -sy or -soundsync
       Turns on sound-sync code where sound data is generated in sync
       with the emulation of the SNES sound CPU instructions. Slightly
       slower emulation speed but helps reduce note and sound effects
       timing problems and one or two games actually require this
       mode, notably Williams Arcade Classics.  Also turns on
       interpolated sound and envelope height reading.  Pressing the
       ']' key during gameplay toggles sound-sync only on and off.
       Not very effective/useless when thread sound is enabled due to
       mutex data locking timing problems.

      -is or -interpolatedsound (default: off)
       Turns on interpolated sound. Any samples played at a lower
       frequency than the current playback rate (-r option) have
       additional sample points computed and played where previously
       the same sample data was played repeatedly. Lower frequency
       sounds have improved bass and sound much less harsh - I suspect
       the real SNES does this.  Pressing '[' during gameplay toggles
       the feature on and off.

      -b size or -buffersize size or -bs size (default: auto-select)
       Sound playback buffer size in bytes 128-4096.

      -nosamplecaching or -nsc or -nc (default: on)
       Disable decompressed sound sample caching. Decompressing
       samples takes time, slowing down the emulator. Normally the
       decompressed samples are saved just in case they need to be
       played again, but the way samples are stored and played on the
       SNES, it can result in a click sound or distortion when caching
       samples with loops in them.

      -alt or -altsampledecode (defalt: off)
       Switch to using Anti Resonance's sample decoding method. Some
       games, notably Squaresoft RPG games, make use of a SNES hardware
       sample decoding bug/ feature to generate wind and swish-type
       sound effects; selecting this option enables code that tries to
       simulate the feature. The disadvantage is that it can decode
       some 'normal' samples incorrectly.

      -noecho or -ne (default: on)
       Turn off sound echo and FIR filter effects. Processing these
       effects can really slow down a non-MMX Pentium machine due to
       the number of calculations required to implement these
       features.

      -ratio 1+ or -ra 1+ (default: 2) (OBSOLETE)
       Ratio of 65c816 to SPC700 instructions. The value is no longer
       used because SPC700 cycle counting is now implemented giving
       much more accurate timing.

      -nomastervolume or -nmv (default: on)
       Disable emulation of the sound DSP master volume control. Some
       ROMs set the volume level very low requiring you to turn up the
       volume level of your speakers introducing more background
       noise. Use this option to always have the master volume set on
       full and to by-pass a bug which prevents the music and sound
       effects being heard on Turrican.


      · Cheat options:

      -gg <code> or -gamegenie <code>
       Supply a Game Genie code for the current ROM. Up to 10 codes
       can be in affect at once. Game Genie codes for many SNES games
       are available from:
       http://vgstrategies.miningco.com/games/vgstrategies/library/ggn/bl_ggnsnes.htm

      -ar <code> or -actionreplay <code>
       Supply a Pro-Action Reply code for the current ROM. Up to 10
       codes can be in affect at once. At the moment, codes which
       alter RAM do not work.

      -gf <code> or -goldfinger <code>
       Supply a Gold Finger code for the current ROM. Up to 10 codes
       can be active at once.

      -nocheat                  
       Disable the cheat system.

      -cheat                    
       Enables the cheat system. (Default)


      · Speed up/slow down options: (See "Speeding Up The Emulation")

      -f <frame skip count> or -frameskip <frame skip count> (default: auto-adjust)
       Set this value to deliberately fix the frame skip rate and
       disable auto- speed regulation. Use a larger value faster
       emulation but more jerky movement and a smaller value for
       smooth but slower screen updates.  Use '+' and '-' keys to
       modify the value during a game.  Ideal for some Super FX games
       that confuse the auto-adjust code or for games that
       deliberately flash the screen every alternate frame.

      -frametime <time in milliseconds> or -ft <time in milliseconds>
       (default: 16.6ms NTSC games and 20ms for PAL games) If
       auto-adjust frame skip option is in effect, then the emulator
       will try to maintain a constant game and music speed locked to
       this value by skipping the rendering of some frames or waiting
       until the required time is reached.  Increase the value to slow
       down games, decrease it to speed up games.  During a game the
       value can be adjusted in millisecond steps by pressing Shift
       '-' or Shift '+'.

      -h <0-200> or -cycles <0-200>(default: 100)
       Percentage of CPU cycles to execute per scan line, decrease
       value to increase emulation frame rate. Most ROMs work with a
       value of 85 or above.

      -j or -nojoy
       Turn off joystick, SideWinder and GrIP detection (joystick
       polling on the PC slows the emulator down).


      · ROM image format options:

      -i or -interleaved (default: auto-detect)
       Force interleaved ROM image format.

      -i2 or -interleaved (default: can't be auto-detected)
       Force alternate interleaved format (i.e. most Super FX games).

      -hirom or -fh or -hr (default: auto-detect)
       Force Hi-ROM memory map for ROMs where the Hi-ROM header test fails.

      -lorom or -fl or -lr (default: auto-detect)
       Force Lo-ROM memory map for ROMs where the Hi-ROM header test fails)

      -header or -hd (default: auto-detect)
       Force the detection of a ROM image header. Some ROM images have
       been hand-edited to remove unused space from the end of the
       file; if the resultant image size is not a multiple of 32k then
       Snes9x can't auto-detect the presence of a 512 byte ROM image
       header.

      -noheader or -nhd (default: auto-detect)
       Force Snes9x into thinking no ROM image header is present. See
       -header above.

      -p or -pal (default: auto-detect)
       Fool ROM into thinking this is a PAL SNES system and adjust
       frame time to 20ms (50 frames per second)

      -n or -ntsc (default: auto-detect)
       Fool ROM into thinking this is a NTSC SNES system and adjust
       frame time to 16.7ms (60 frames per second)

      -l or -layering (default: off)
       Swap background layer priorities from background involved in
       sub-screen addition/subtraction. Can improve some games
       play-ability - no need to constantly toggle background layers
       on and off to read text/see maps, etc.  Toggle feature on and
       off during game by pressing '8'.  Not used if transparency
       effects are enabled.

      -l <freeze game file name> or -loadsnapshot <freeze game file name>
       Load snapshot file and restart game from saved position.

      -nh or -nohdma (default: H-DMA enabled)
       Turn off the H-DMA emulation. Pressing '0' during a game
       toggles H-DMA on and off.

      -n or -nospeedhacks (default: speed hacks)
       Turn off a couple of speed hacks. The hacks boost the speed of
       many ROMs but cause problems a few ROMs.

      -nw or -nowindows (default: graphics windows emulated)
       Disable graphics windows emulation. Use 'backspace' key during
       a game to toggle the emulation on and off.

      -nopatch
       Disable IPS autopatching.

      -bsxbootup
       Boot up BS games from the BS-X.


      · Joystick options:

      -joymap[1|2|3|4] followed by 8 numbers
       Specify the SNES joypad buttons to Linux joystick driver
       buttons mapping for each of the four supported joypads.
       Specify the Linux joystick button number for the corresponding
       SNES button in the following 
       order: A, B, X, Y, TL, TR, Start and Select
       The default map is: 1 0 4 3 6 7 8 9 
       which is suitable for Sidewinder gamepads.

      -s or -swap
       Swap emulated joy-pad 1 and 2 around, pressing '6' during a
       game does the same thing.

      -j or -nojoy
       Turn off joystick, SideWinder and GrIP detection (joystick
       polling on the PC slows the emulator down).

      -version
       Print the Snes9X version and the exit.


      For example, to start a game called "mario", with sound, and
      transparency effects, type:

      snes9x -tr mario.smc


 5. Keyboard controls

    While the emulator is running:
    'Escape'                  Quit the emulator
    'Pause' or 'Scroll Lock'  Pause the emulator
    'Alt'+'Return'            Toggle the Linux X11 port full-screen mode 
                              on and off.

    Joy-pad 1:
    'up' or 'u'               Up direction
    'down', 'j' or 'n'        Down direction
    'left' or 'h'             Left direction
    'right' or 'k'            Right direction
    'a', 'v' or 'q'           TL button
    'z', 'b' or 'w'           TR button
    's', 'm' or 'e'           X button
    'x', ',' or 'r'           Y button
    'd', '.' or 't'           A button
    'c', or 'y'               B button
    'return'                  Start button
    'space'                   Select button

    'Mouse left'              Mouse left button or SuperScope fire button.
    'Mouse right'             Mouse right button or SuperScope cursor button.

    'tab'                     Emulation turbo speed button for skipping past 
                              long ROM intros.

    '`', '~' or '#'           SuperScope turbo toggle switch.
    '/'                       SuperScope pause button.

    '0'                       Toggle H-DMA emulation on/off.
    '1'                       Toggle background 1 on/off.
    '2'                       Toggle background 2 on/off.
    '3'                       Toggle background 3 on/off.
    '4'                       Toggle background 4 on/off.
    '5'                       Toggle sprites (sprites) on/off
    '6'                       Toggle swapping of joy-pad one and two around
    '7'                       Rotate between Multi-player 5, mouse on port 1,
                              mouse on port 2 and SuperScope emulation.
    '8'                       Toggle background layer priorities for 
                              backgrounds
                              involved in sub-screen addition/subtraction.
    '9'                       Toggle transparency effects on and off - only if
                              16-bit screen mode selected.
    Shift+'9'                 Toggle SNES mode 7 bi-linear filtering on/off.
    'Backspace'               Toggle emulation of graphics window effects 
                              on/off.

    '-'                       Decrease frame redraw skip rate
    '+'                       Increase frame redraw skip rate
                              The sequence is auto-frame rate adjust, render every
                              frame, render 1 frame in two, render 1 frame in three,
                              render 1 frame in four, etc.

    Shift+'-'                 Decrease frame time in 1ms steps.
    Shift+'+'                 Increase frame time in 1ms steps.
                              The frame time value is the average length of time a
                              single frame should take to emulate - works only if
                              the auto-frame rate adjust is operational.
                              The code will skip the rendering of some
                              frames in order to try and reach the required value.
    Shift+'F1-F10'            Quick save a freeze game file.
    'F1-F10'                  Quick load a freeze game file, restoring a game to an
                              exact position.
    Alt or Ctrl+'F1'          Save the state of the SPC700 sound CPU into a file
                              called <romname>.spc. Separate programs are available
                              that can load and then 'play' these state files. Ideal
                              for 'ripping' music from a game and playing it at a
                              later date.
    Alt/Ctrl+'F2' or 'F11'    Load a game's saved position.
    Alt/Ctrl+'F3' or 'F12'    Save a game's position.

    Alt/Ctrl+'F4' -> 'F11'    Toggle sound channels on/off.
    Alt/Ctrl+'F12'            Turn on all sound channels.

    [Alt|Ctrl]+'Print Screen' Takes a screenshot.  Screenshots will
                              be saved as separate PNG files on the form
                              $HOME/.snes96_snapshots/<romname>###.png
                              Note that when not running in full
                              screen mode some window managers will
                              catch 'Print Screen' for their own
                              usage. In such cases press one or more
                              extra modifiers to get the signal to Snes9X.


 6. Joystick Support

      The Linux port makes use of the v1.x joystick kernel drivers
      written by Vojtech Pavlik (vojtech@ucw.cz) to allow the use of a
      wide variety of different joystick/joypad types to control SNES
      games.

      If you are still using the older versions of the drivers, you can
      download the latest driver version from
      http://atrey.karlin.mff.cuni.cz/~vojtech/joystick/

      Refer to the documentation that comes with the drivers to enable
      support for your type of joystick/pad in Snes9x.


 7. Game Saving

      Many SNES games could take a very long time to complete from
      start to finish so they allowed your progressed to be saved into
      RAM fitted inside the game pack; the RAM contents were backed up
      by a battery when the SNES was switched off or the game
      removed. Snes9x simulates this by saving the contents of the
      emulated battery-backed RAM into a file when you load a new game
      or exit Snes9x. The file is then automatically re-loaded the
      next time you play the game.

      Snes9x also provides freeze-files; these are files that saves a
      game's position at any point in the game, not just at predefined
      places chosen by the game's designers - ideal for saving your
      game just before a tricky bit!  Snes9x provides 9 save slots;
      during a game, press Shift + F1 to F9 to save a game, and just
      F1 to F9 to load it again later.

      Freeze game files and Save-RAM (S-RAM) save files are normally
      read from and written to the directory
      $HOME/.snes96_snapshots. This can be changed by setting the
      environment variable SNES96_SNAPSHOT_DIR to point to a different
      directory.

      Snes9x uses its own unique format for freeze-files, as does
      ZSNES, but Snes9x can also load ZSNES format freeze-files. Just
      copy the ZSNES freeze files into your save directory and, if the
      native format Snes9x freeze file doesn't exist (<rom image
      name>.00X where X is a digit), Snes9x will try to load the
      corresponding ZSNES freeze file instead (<rom image name>.zsX
      where X is a 't' or a digit). Note the filename extension must
      be in lower-case. When you freeze a game position after loading
      a ZSNES format freeze file, Snes9x will save it in native Snes9x
      format.


 8. Emulation speed

      Emulating an SNES is very compute intensive, with its two or
      three CPUs, an 8 channel digital sound processor with real-time
      sound sample decompression and stereo sound, two custom graphics
      processors, etc.

      If you only have a 486 machine, you will need to stick to using
      only 8-bit graphics and minimal or no sound:

      With sound:
      snes9x -ne -r 1 -mono <rom filename>

      Without sound:
      snes9x -ns <ROM filename>.

      Disabling the joystick support will also help (-j).

      For maximum speed, if you're using the X Window System port,
      make sure your X server is set to depth 8 and transparency
      effects are not enabled, or your X server is set to depth 15 or
      16 if you want transparency effects.

      If you want to use the TV mode (-y), switching to a full-screen
      display is usually fastest with the X server set to depth 15 or
      16.

      Don't enable the scale option and don't resize the window on the
      X Windows port.

      Users with slower Pentium machines might want to turn off echo
      and digital FIR filter effects, due to the number of multiply
      operations needed to implement them. Use -ne option.

      Got a big throbbing beast of a CPU under the cover of your
      computer? These options will sort out the men from the boys:
      snes9x -y -sc -r 7 -sy <ROM filename>


 9. How to play BS games 

      0) Trash all BS games' .srm files created by Snes9x 1.43 and earlier.
      1) Prepare the BS-X ROM image.
         Make sure the file is uncompressed and doesn't have a header.
      2) Rename the BS-X ROM to 'BS-X.bios'.
      3) Put BS-X.bios in the proper directory:
         the same directory as .srm files.
      4) Launch BS-X.bios as you usually play SNES games.
      5) Set your name and gender.
      6) Launch a BS game.
         The game is launched for the first time and its .srm doesn't exist,
         so BS-X.srm is loaded instead.
      7) Once you launched the game, its .srm is created and loaded
         from the next time. 

      If you launch a BS game for the first time without BS-X.srm,
      maybe the game doesn't work properly. 
      To boot up BS games from BS-X town, use -bsxbootup option.


 10. Configuring settings and key mapping

      The Unix port of Snes9x reads the preferences file 'snes9x.conf'
      when launching, if it exists in ~/.snes96_snapshots or ~/.roms.
      To make your snes9x.conf,
      copy the template file 'snes9x.conf.default' from snes9x/doc,
      rename it to snes9x.conf, put it in the proper directory, and
      edit it as you like. The document 'control-inputs.txt' will help
      you to edit the keyboard/joystick mapping.


 11. Building Snes9X from source

      The source release of Snes9X comes with a configure file that
      will try to make a Makefile suitable for your system. To invoke
      it go into the snes9x directory inside the distribution:

        cd snes9x-1.??-src/snes9x

      Then run configure and finish with running make if it went well.

        ./configure && make

      configure takes several arguments. Further documentation about
      this is available by running ./configure --help


A. Release notes:

v1.5  - 200606

      Unix specific changes:
      · Added -bsxbootup option.
      · Config file and key remapping feature.

v1.43 - 20050101

      Unix specific changes:
      · Removed bundled zlib in favour of detecting the system zlib.
      · Fixed mouse offset in resized X11 window.
      · Fixed compilation on AMD64.
      · Rewrote keyboard setup code
      · Unmap all joystick buttons before applying -joymapX
      · Loading a zip file on binaries without zip support will give
        an appropriate error message
      · Added install target with proper --prefix handling.
      · Added VPATH handling.


v1.42 - 20031204

      Unix specific changes:
      · Added fallback to C-core if configure can't find nasm.
      · Changed the default controller setting to use just the plain
        SNES controller pad. Mouse, Superscope and multiplayer adapters
        are now turned off by default. This reflects how the Windows
        port has been doing it for some time.
      · Added a new command line option: -sdd1-pack
      · Fixed the x86 SuperFx assembler core.
      · Added possibility to enable the debugger via configure.
      · Moved full screen button from 'Print Screen' to 'Alt'+'Return'.
      · Added screenshot capability. Available by pressing 'Print Screen'.

      Known Unix specific problems:
      · Doom hangs when any barrel is shot if the C++ SuperFx core is
        used.

      Known misfeatures:
      · Fullscreen uses an old DGA hack that requires root access. I
        do not recommend its usage. Hopefully this can be replaced
        with modern code in a later release. Code submissions welcome.

v1.41-1 - 20030812

      No Unix specific changes.

v1.41 - 20030812

      Unix specific changes:
      · Fixed OpenGL target.
      · Fixed sound on some big endian Linux systems.

      Known Unix specific problems and missfeatures:
        Same as in 1.40.

v1.40 - 20030725

      Known Unix specific problems:
      · Doom crashes after splash screen if the assembler core is used.
      · Doom hangs when any barrel is shot if the C++ core is used.

      Known misfeatures:
      · Fullscreen uses an old DGA hack that requires root access. I
        do not recommend its usage. Hopefully this can be replaced
        with modern code in a later release. Code submissions welcome.


B. Contacting the Snes9X team

      The Snes9X team can be reached at http://www.snes9x.com/


C. Contacting the Unix maintainer

      The current Unix maintainer is Peter Bortas <peter@bortas.org>. 
      If you have patches, feel free to mail them to him.


D. Credits and Thanks

      Thanks to ernstp and entonne for patches and testing on Linux PPC.
