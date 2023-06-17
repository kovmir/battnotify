# battnotify

Show desktop notifications if your battery is low

# INSTALL

```bash
git clone https://github.com/kovmir/battnotify.git
cd battnotify # Optionally adjust config.h to your liking.
make
sudo make install
```

# USAGE

```bash
# Within your ~/.xinitrc
battnotify &
```
# DEPENDENCIES

* [libnotify](https://gitlab.gnome.org/GNOME/libnotify)
* [pkg-config](https://gitlab.freedesktop.org/pkg-config/pkg-config)

# LICENSE

Copyright 2023 Ivan Kovmir

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

# CONTRIBUTING

When submitting PRs, please maintain the
[coding style](https://suckless.org/coding_style/) used for the project.
