# battnotify

Show desktop notifications if your battery is low

# INSTALL

```
git clone https://github.com/kovmir/battnotify.git
cd battnotify # Optionally adjust config.h to your liking.
make
sudo make install
```

# USAGE

```
# Within your ~/.xinitrc
battnotify &
```
# DEPENDENCIES

* [libnotify](https://gitlab.gnome.org/GNOME/libnotify)
* [pkg-config](https://gitlab.freedesktop.org/pkg-config/pkg-config)

# CONTRIBUTING

When submitting PRs, please maintain the
[coding style](https://suckless.org/coding_style/) used for the project.
