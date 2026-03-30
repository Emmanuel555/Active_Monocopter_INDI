# ESP32 Setup for Outdoor SAM
Installation instructions:

**Teensy 4.1 w Udev rules:

sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
sudo reboot

cd /tmp
wget https://www.pjrc.com/teensy/00-teensy.rules
sudo cp 00-teensy.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger

sudo reboot

**PlatformIO only works with VScode

rmb to run sudo apt install python3-venv
