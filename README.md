# Tonnetz

This is an interception tools plugin to turn a keyboard into a midi controller
with the layout based on the Tonnetz.

If anyone is interested in getting similar functionality on Windows, I believe
it is possible but you have to use both the RawInput API and the Hooks API,
because RawInput can't cancel keypresses but Hooks can, and Hooks can't identify
which device the event is coming from but RawInput can. I gave up because it's
easier on Linux and it wasn't worth figuring out.

You should also note that most computer keyboards can only press two keys at a
time, so you'll need a mechanical/gaming keyboard if you want to play any
chords.

## Installation

You'll need to have
[Interception Toolds](https://gitlab.com/interception/linux/tools) installed,
and if you have to recompile then you'll need the libasound2 headers:

```bash
sudo apt install libasound2-dev
```

Now clone this repository. Go into `0tonnetz.yaml` and replace the path to the
`tonnetz` executable with the correct path on your computer. Then replace the
path to the keyboard with the path to the keyboard which you want this to apply
to. It should be somewhere in `/dev/input`

Then if you have [run](github.com/akriegman/run) installed somewhere in the root
users path you can install Tonnetz with:

```bash
sudo run install
```

Otherwise, you can do

```bash
$ sudo bash
# . runfile
# install
```

Now your keyboard will be treated as a midi controller until you uninstall (same
as either above method but replace `install` with `uninstall`).
