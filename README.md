# Sagrada

Sagrada is the virtual version of the board game of the same name. A quick review of the game can be found e.g. [here](https://boardgamegeek.com/boardgame/199561/sagrada).
It implements all the features that are listed in the hard copy version. The players can choose the amount of computer players they want to play against(1-3).

A more detailed description of the project can be found in **doc/documentation.pdf** and **doc/user_documentation.pdf**.

## Installing the required packages

### Linux

#### gtkmm-3.0

Run the following command on Ubuntu distributions

```sh
apt-get install libgtkmm-3.0-dev
```

On other platforms, use the packaging tool for concrete distribution. Further information can be found on the [official download page](https://gtkmm.org/en/download.html).

#### Meson and Ninja

There are multiple options to install Meson. All the options are described on the [official installation page](https://mesonbuild.com/Getting-meson.html).

One example using `apt` (that installs `ninja` as well) on Ubuntu distributions:

```sh
sudo apt install meson
```

### Windows

#### gtkmm-3.0

To install gtkmm-3.0 on a Windows machine, please follow the instructions provided on the [official Windows installation page](https://wiki.gnome.org/Projects/gtkmm/MSWindows).

#### Ninja

- Download the `ninja-win.zip` file from the latest release from the [official github release page](https://github.com/ninja-build/ninja/releases).
- Extract the zip file
- Add the path to the extracted directory to the `PATH` environment variable

#### Meson

The standard way for installing software on Windows machines is by using installers. Meson provides an MSI installer that is available
on the [official github releases page](https://github.com/mesonbuild/meson/releases).

Download the latest stable version and run the installer.

## Build and compile

Building the project requires a `g++12` compiler.

To build the solution, execute the following commands:

```sh
$ meson build
$ ninja -C build
$ build/sagrada
```

This build by default does not use any compiler flags for code optimization. To produce an optimized build, add the `--buildtype=debugoptimized` flag to the `meson build` command.

## Usage

### GUI

To run the game with a GUI, simply run the `build_dir/sagrada` executable without any command-line arguments.

A detailed description of using the GUI can be found in **doc/user_documentation.pdf**.

### Tournament

Once the project is built, you can easily run tournaments with the `build_dir/tournament` executable. This executable
runs a tournament between the defined players playing a given number of games and prints the results to the console.

Example run:

```sh
build/tournament -n 100 -p random minimax-depth=3,worlds=4 -v
```

Invoking this command runs a 100-game tournament between the `random` and the `minimax` agents.

For a more detailed description of the tournament's CLI, run `build/tournament --help`.

## Other bits

To generate a [Doxygen](https://www.doxygen.nl/) documentation, use the following commands

```sh
cd doc
doxygen Doxyfile
```

Generating this documentation requires Doxygen and [graphviz](https://graphviz.org/) installed in the system.
