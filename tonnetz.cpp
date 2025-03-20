#include <alsa/asoundlib.h>
#include <iostream>
#include <linux/input.h>
#include <unistd.h>
#include <unordered_map>

using namespace std;

const unordered_map<__u16, pair<int, int>> tonnetz = {

    {KEY_ESC, {5, 1}},         {KEY_F1, {5, 2}},
    {KEY_F2, {5, 3}},          {KEY_F3, {5, 4}},
    {KEY_F4, {5, 5}},          {KEY_F5, {5, 6}},
    {KEY_F6, {5, 7}},          {KEY_F7, {5, 8}},
    {KEY_F8, {5, 9}},          {KEY_F9, {5, 10}},
    {KEY_F10, {5, 11}},        {KEY_F11, {5, 12}},
    {KEY_F12, {5, 13}},        {KEY_SYSRQ, {5, 14}},
    {KEY_SCROLLLOCK, {5, 15}}, {KEY_PAUSE, {5, 16}},

    {KEY_GRAVE, {4, 0}},       {KEY_1, {4, 1}},
    {KEY_2, {4, 2}},           {KEY_3, {4, 3}},
    {KEY_4, {4, 4}},           {KEY_5, {4, 5}},
    {KEY_6, {4, 6}},           {KEY_7, {4, 7}},
    {KEY_8, {4, 8}},           {KEY_9, {4, 9}},
    {KEY_0, {4, 10}},          {KEY_MINUS, {4, 11}},
    {KEY_EQUAL, {4, 12}},      {KEY_BACKSPACE, {4, 13}},
    {KEY_INSERT, {4, 14}},     {KEY_HOME, {4, 15}},
    {KEY_PAGEUP, {4, 16}},

    {KEY_TAB, {3, 1}},         {KEY_Q, {3, 2}},
    {KEY_W, {3, 3}},           {KEY_E, {3, 4}},
    {KEY_R, {3, 5}},           {KEY_T, {3, 6}},
    {KEY_Y, {3, 7}},           {KEY_U, {3, 8}},
    {KEY_I, {3, 9}},           {KEY_O, {3, 10}},
    {KEY_P, {3, 11}},          {KEY_LEFTBRACE, {3, 12}},
    {KEY_RIGHTBRACE, {3, 13}}, {KEY_BACKSLASH, {3, 14}},
    {KEY_DELETE, {3, 15}},     {KEY_END, {3, 16}},
    {KEY_PAGEDOWN, {3, 17}},

    {KEY_CAPSLOCK, {2, 2}},    {KEY_A, {2, 3}},
    {KEY_S, {2, 4}},           {KEY_D, {2, 5}},
    {KEY_F, {2, 6}},           {KEY_G, {2, 7}},
    {KEY_H, {2, 8}},           {KEY_J, {2, 9}},
    {KEY_K, {2, 10}},          {KEY_L, {2, 11}},
    {KEY_SEMICOLON, {2, 12}},  {KEY_APOSTROPHE, {2, 13}},
    {KEY_ENTER, {2, 14}},

    {KEY_LEFTSHIFT, {1, 3}},   {KEY_Z, {1, 4}},
    {KEY_X, {1, 5}},           {KEY_C, {1, 6}},
    {KEY_V, {1, 7}},           {KEY_B, {1, 8}},
    {KEY_N, {1, 9}},           {KEY_M, {1, 10}},
    {KEY_COMMA, {1, 11}},      {KEY_DOT, {1, 12}},
    {KEY_SLASH, {1, 13}},      {KEY_RIGHTSHIFT, {1, 14}},
    {KEY_UP, {1, 15}},

    {KEY_LEFTCTRL, {0, 3}},    {KEY_LEFTMETA, {0, 4}},
    {KEY_LEFTALT, {0, 5}},     {KEY_SPACE, {0, 6}},
    {KEY_RIGHTALT, {0, 12}},   {KEY_COMPOSE, {0, 13}},
    {KEY_RIGHTCTRL, {0, 14}},  {KEY_LEFT, {0, 15}},
    {KEY_DOWN, {0, 16}},       {KEY_RIGHT, {0, 17}},
};

int main() {
  int status;
  snd_rawmidi_t *midi_out = nullptr;

  cerr << "starting" << endl;
  status = snd_rawmidi_open(nullptr, &midi_out, "virtual", SND_RAWMIDI_SYNC);
  if (status < 0) {
    cerr << "Error opening MIDI output: " << snd_strerror(status) << endl;
    return 1;
  }
  cerr << "opened port" << endl;

  input_event event;
  while (fread(&event, sizeof(event), 1, stdin) == 1) {
    if (event.type == EV_KEY) {
      unsigned char note[3];

      if (event.value == 1) {
        note[0] = 0x90;
        note[2] = 127;
      } else if (event.value == 0) {
        note[0] = 0x80;
        note[2] = 0;
      } else {
        continue;
      }

      if (tonnetz.find(event.code) != tonnetz.end()) {
        pair<int, int> coords = tonnetz.at(event.code);
        note[1] = coords.first * 4 + coords.second * 7;
      } else {
        continue;
      }

      snd_rawmidi_write(midi_out, note, 3);
    }
  }

  snd_rawmidi_close(midi_out);

  return 0;
}
