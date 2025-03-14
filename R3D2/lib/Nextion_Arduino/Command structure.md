Nextion command structure
-------------------------

Created JimmyTheBook 21-Feb-2025

These are notes about the command structure in Nextion (which is a little more advanced that I am).


Basic command structure
-----------------------
Commands are string based lines.
Object names need to be exact.


Example... getting the text from an object
"get <objname>.txt"

The command is sent via Serial as characters.
The command needs to end with three "0xFF" characters.
