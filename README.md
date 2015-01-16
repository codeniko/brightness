## Laptop-Brightness
Quickly modify the brightness of laptop screens running Linux distributions. Developed for an HP Pavilion Dv7 laptop running Debian.

## Installation
It's installed in /usr/local/bin. Because the program modifies the "/sys/class/backlight/intel_backlight/brightness" file and thus requires requires root privileges, the setuid permission bit is set upon installation to avoid having to sudo everytime you change the brightness.

    git clone https://github.com/codeniko/laptop-brightness.git
    cd laptop-brightness
    make && sudo make install
  
## Usage

    brightness [1..10]  # Change to selected brightness level, 10=brightest, 1=dimmest.
    brightness 0,-1     # Sight-damaging brightness levels if laptop MUST survive.
    brightness +        # Increment brightness (+500 in the brightness file until maxed)
    brightness -        # Decrement brightness (-500 in the brightness file until low 500)
    brightness          # Toggle between levels 10 and 1. 
