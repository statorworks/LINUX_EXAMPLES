This is a setup to have a more futuristic 'neon' theme on the Raspberry Pi.

![alt text](https://github.com/statorworks/LINUX_EXAMPLES/blob/main/THEME/Screenshot_cyan.png)

Please see the other snapshots.

Copy the folder and run the theme_change script as root, you will get the color option:

1 Orangegutan

2 PunkBerry

3 Greenix

4 Cyanaid

5 YellowMellow

6 Random

Setup:

The <theme><name> entry on etc/xdg/openbox/lxde-pi-rc.xml must be 'Zerobot'

The Themename and IconThemeName entries on etc/xdg/lxsession/desktop.conf are 'Zerobot'

The dark GTK theme is selected on /~/.config/gtk-3.0/settings.ini with:

[settings]

gtk-application-prefer-dark-theme=1


This script modifies the following, ensure they have proper permission for 'Pi' user:

/home/pi/.themes 

/usr/share/icons 

/usr/share/lxpanel/images


On the Zerobot OS image all this will be ready to go.

Todo: Some of the folder icons that still look yellow need to be matche on next release.

Please see the screenshots below. Enjoy!!
