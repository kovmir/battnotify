# REPOSITORY MIGRATION NOTICE

Migrated to [SourceHut](https://git.sr.ht/~kovmir/battnotify).

# battnotify

[![builds.sr.ht status](https://builds.sr.ht/~kovmir/battnotify/commits/master/.build.yml.svg)](https://builds.sr.ht/~kovmir/battnotify/commits/master/.build.yml?)

Display desktop notifications if your battery is low.

# PREVIEW

![screenshot](screenshot.png)

*The look of the notification on your computer depends on your notification
daemon and its configuration.*

# INSTALL

```bash
git clone https://github.com/kovmir/battnotify.git
cd battnotify # Optionally adjust config.h to your liking.
make
sudo make install
```

# USAGE

```bash
battnotify # Put 'battnotify &' in your '~/.xinitrc'.
```

*A notification daemon, like [dunst][1], must be started prior to `battnotify`.*

# DEPENDENCIES

* [GNU Make][5]
* [pkg-config][4]
* [GCC][6] or [Clang][7]
* [libnotify][3]

# SUPPORTED OPERATING SYSTEMS

* Linux

# FAQ

**Q: Does it support Wayland?**

A: It does, as it is independent of the display server due to [Desktop
Notifications Specification][8].

# CONTRIBUTING

When submitting PRs, please maintain the [coding style][2] used for the project.

[1]: https://dunst-project.org/
[2]: https://suckless.org/coding_style/
[3]: https://gitlab.gnome.org/GNOME/libnotify
[4]: https://gitlab.freedesktop.org/pkg-config/pkg-config
[5]: https://www.gnu.org/software/make/
[6]: https://gcc.gnu.org/
[7]: https://clang.llvm.org/
[8]: https://specifications.freedesktop.org/notification-spec/notification-spec-latest.html
