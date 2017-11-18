# FreeBSD Support

This is just the initial port to FreeBSD, only basic test has been
done so far.

## Driver Programs - Directory "driver-common"

* gpio_test - compiles and run on RPi 2B
* epd_test - compiles and run on RPi 2B
* epd_fuse - basically working on RPi 2B (2.0 V231 + ClockDemo)

Problems
* needs `kldload fuse`
* need to fix mountpoint (devfs?)
* need to create rc.d script

## OS Versions Tried

* RPi: `FreeBSD-12.0-CURRENT-arm-armv6-RPI2-20170925-r323985.img.xz`


## packages installed

~~~
mg zsh lynx git getopt rsync sudo
fusefs-libs pkgconf
openntpd
py36-pillow python3
dejavu
~~~

## initial run

Note: need to substitute /dev/epd -> /tmp/epd in the clock demo since
devfs does not allow simple mkdir

as user:
~~~
% make rpi PANEL_VERSION=V231_G2
% sudo make rpi-install PANEL_VERSION=V231_G2
~~~


as root (or use sudo)
~~~
# kldload fuse
# mkdir /tmp/epd
# ~repaper/gratis.hxw/PlatformWithOS/FreeBSD/epd_fuse --panel=2.0 /tmp/epd
# python3 ~repaper/gratis.hxw/PlatformWithOS/demo/ClockDemo.py
~~~
