#include <alsa/asoundlib.h>
#include <iostream>
#include <linux/input.h>
#include <unistd.h>
#include <unordered_map>

const std::unordered_map<__u16, unsigned char> tonnetz = {

    {KEY_ESC, 109},       {KEY_F1, 104},       {KEY_F2, 99},
    {KEY_F3, 94},         {KEY_F4, 89},        {KEY_F5, 84},
    {KEY_F6, 79},         {KEY_F7, 74},        {KEY_F8, 69},
    {KEY_F9, 64},         {KEY_F10, 59},       {KEY_F11, 54},
    {KEY_F12, 49},        {KEY_SYSRQ, 44},     {KEY_SCROLLLOCK, 39},
    {KEY_PAUSE, 34},

    {KEY_GRAVE, 110},     {KEY_1, 105},        {KEY_2, 100},
    {KEY_3, 95},          {KEY_4, 90},         {KEY_5, 85},
    {KEY_6, 80},          {KEY_7, 75},         {KEY_8, 70},
    {KEY_9, 65},          {KEY_0, 60},         {KEY_MINUS, 55},
    {KEY_EQUAL, 50},      {KEY_BACKSPACE, 45}, {KEY_INSERT, 40},
    {KEY_HOME, 35},       {KEY_PAGEUP, 30},

    {KEY_TAB, 101},       {KEY_Q, 96},         {KEY_W, 91},
    {KEY_E, 86},          {KEY_R, 81},         {KEY_T, 76},
    {KEY_Y, 71},          {KEY_U, 66},         {KEY_I, 61},
    {KEY_O, 56},          {KEY_P, 51},         {KEY_LEFTBRACE, 46},
    {KEY_RIGHTBRACE, 41}, {KEY_BACKSLASH, 36}, {KEY_DELETE, 31},
    {KEY_END, 26},        {KEY_PAGEDOWN, 21},

    {KEY_CAPSLOCK, 92},   {KEY_A, 87},         {KEY_S, 82},
    {KEY_D, 77},          {KEY_F, 72},         {KEY_G, 67},
    {KEY_H, 62},          {KEY_J, 57},         {KEY_K, 52},
    {KEY_L, 47},          {KEY_SEMICOLON, 42}, {KEY_APOSTROPHE, 37},
    {KEY_ENTER, 32},

    {KEY_LEFTSHIFT, 83},  {KEY_Z, 78},         {KEY_X, 73},
    {KEY_C, 68},          {KEY_V, 63},         {KEY_B, 58},
    {KEY_N, 53},          {KEY_M, 48},         {KEY_COMMA, 43},
    {KEY_DOT, 38},        {KEY_SLASH, 33},     {KEY_RIGHTSHIFT, 28},
    {KEY_UP, 23},

    {KEY_LEFTCTRL, 79},   {KEY_LEFTMETA, 74},  {KEY_LEFTALT, 69},
    {KEY_SPACE, 0},       {KEY_RIGHTALT, 34},  {KEY_COMPOSE, 29},
    {KEY_RIGHTCTRL, 24},  {KEY_LEFT, 19},      {KEY_DOWN, 14},
    {KEY_RIGHT, 9},
};

int main() {
  int status;
  snd_rawmidi_t *midi_out = nullptr;

  status = snd_rawmidi_open(nullptr, &midi_out, "virtual", SND_RAWMIDI_SYNC);
  if (status < 0) {
    std::cerr << "Error opening MIDI output: " << snd_strerror(status)
              << std::endl;
    return 1;
  }

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
        note[1] = tonnetz.at(event.code);
      } else {
        continue;
      }

      snd_rawmidi_write(midi_out, note, 3);
    }
  }

  snd_rawmidi_close(midi_out);

  return 0;
}
