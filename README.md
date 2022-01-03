# LaSalle_GTAS2_ASO_Practice_1
A Linux Kernel Module (LKM) for Raspberry Pi 3B+ to control LEDs connected to it.

# HOW TO USE
Use "sudo make" to recompile the module.

Use "sudo make install" to install the module.

Use "sudo make uninstall" to uninstall the module.

Every time a change must be applied:
  1. Make sure to recompile the module.
  2. Make sure that the module has been uninstalled previuously.
  3. Install the newest version of the module.
  
# OUTPUT & MESSAGES
Messages informing that a certain button has been pressed can be visualized on "/var/log/messages" file.

# BUTTONS MAP
Button 1: Red LED ON

Button 2: Red LED OFF

Button 3: Blue LED ON

Button 4: Blue LED OFF
