FTPNotes
========
A simple graphical Full Tilt Poker notes editor.

.. image:: http://i.imgur.com/nqG66.png

Compiling
^^^^^^^^^
FTPNotes depends on Qt 4.6 and above. You can download the Qt SDK at
http://qt.nokia.com/downloads/. After Qt is installed, ensure that ``qmake`` is
in your PATH, then execute the following commands::

  $ qmake
  $ make

*If you are compiling on Windows with the VC++ compiler, replace* ``make``
*with* ``nmake``.

After compiling, you can launch FTPNotes with the following command::

  $ ./bin/ftpnotes

Usage
^^^^^
1. Launch ``ftpnotes``.

2. Click ``File -> Open`` (``CTRL+O``) to choose a notes file to open.

   Full Tilt Poker saves player notes into an XML file named
   ``ftpusername.xml``, where ``ftpusername`` is your Full Tilt Poker username. 
   
   Notes files typically reside in your Full Tilt Poker installation directory.
   On Windows this defaults to ``C:\Program Files\Full Tilt Poker`` or
   ``C:\Program Files (x86)\Full Tilt Poker``. On Mac, the default location is
   ``/Users/mac-username/Library/Application Support/FullTiltPoker``.

3. After the notes file is opened, a list of players will be populated. To edit
   the notes for a specific player, double click on the player's name. You can
   search for a player by typing into the ``Find player:`` textbox. To add notes
   for a new player, click the ``Add Player`` button.
