# HTTP Web Server in Windows using C++

An easy-to-understand HTTP Web Server for all the Windows users that want to work with C++ and learn about how to build an HTTP Web Server from scratch.


# Contents

- [Why?](#why)
- [Installation](#installation)
  <!-- - [Method 1: <a href="https://pypi.org/project/shallow-backup/" rel="nofollow">pipx</a>](#method-1-pipx)
  - [Method 2: Install From Source](#method-2-install-from-source) -->
- [TODO](#todo)
<!-- - [Usage](#usage)
- [Output Structure](#output-structure) -->

### Why?

I wanted to build an HTTP Web server from scratch because:

- Wanted to learn how to build one from scratch.
- Didn't find a good reference project that did it for `Windows`.
- Implement socket programming in `C++`.


### Installation

---

#### Step 1: Clone the Repo

```bash
$ git clone https://github.com/rathink4/http-server-cpp.git

```

### TODO

---

- [ ] Document the README.md file.
- [x] Implement TCP Server-Client connection.
- [x] Implement HTTP Server-Client connection.
- [ ] Add GET, POST, PUT, DELETE functionality in the web server.
- [ ] Handle multiple client connections requests.

<!-- ### Usage

---

- To start the interactive program, run `$ shallow-backup`.
- To backup your dotfiles, run `$ shallow-backup --backup-dots`.

`shallow-backup` was built with scripting in mind. Every feature that's supported in the interactive program is supported with command line arguments.

```shell
Usage: shallow-backup [OPTIONS]

  Easily back up installed packages, dotfiles, and more.
  You can edit which files are backed up in ~/.shallow-backup.

  Written by Aaron Lichtman (@alichtman).

Options:

  --add-dot TEXT               Add a dotfile or dotfolder to config by path.
  --backup-all                 Full back up.
  --backup-configs             Back up app config files.
  --backup-dots                Back up dotfiles.
  --backup-fonts               Back up installed fonts.
  --backup-packages            Back up package libraries.
  --delete-config              Delete config file.
  --destroy-backup             Delete backup directory.
  --dry-run                    Don't backup or reinstall any files, just give
                               verbose output.

  --new-path TEXT              Input a new back up directory path.
  --no-new-backup-path-prompt  Skip setting new back up directory path prompt.
  --no-splash                  Don't display splash screen.
  --reinstall-all              Full reinstallation.
  --reinstall-configs          Reinstall configs.
  --reinstall-dots             Reinstall dotfiles and dotfolders.
  --reinstall-fonts            Reinstall fonts.
  --reinstall-packages         Reinstall packages.
  --remote TEXT                Set remote URL for the git repo.

  --edit                       Open config file in $EDITOR.
  -v, --version                Display version and author info.
  -h, -help, --help            Show this message and exit.
``` -->


<!-- ### Output Structure

---

```shell
shallow_backup/
├── configs
│   ├── plist
│   │   └── com.apple.Terminal.plist
│   ├── sublime_2
│   │   └── ...
│   └── sublime_3
│       └── ...
├── dotfiles
│   ├── .bash_profile
│   ├── .bashrc
│   ├── .gitconfig
│   ├── .pypirc
│   ├── ...
│   ├── shallow-backup.json
│   ├── .ssh/
│   │   └── known_hosts
│   ├── .vim/
│   └── .zshrc
├── fonts
│   ├── AllerDisplay.ttf
│   ├── Aller_Bd.ttf
│   ├── ...
│   ├── Ubuntu Mono derivative Powerline Italic.ttf
│   └── Ubuntu Mono derivative Powerline.ttf
└── packages
    ├── brew-cask_list.txt
    ├── brew_list.txt
    ├── cargo_list.txt
    ├── gem_list.txt
    ├── installed_apps_list.txt
    ├── npm_list.txt
    ├── macports_list.txt
    ├── pip_list.txt
    └── sublime3_list.txt
``` -->