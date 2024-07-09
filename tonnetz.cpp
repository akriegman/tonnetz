#include <alsa/asoundlib.h>
#include <iostream>
#include <linux/input.h>
#include <unistd.h>
#include <unordered_map>

const std::unordered_map<__u16, unsigned char> tonnetz = {

    {KEY_ESC, 1},         {KEY_F1, 59},        {KEY_F2, 60},
    {KEY_F3, 61},         {KEY_F4, 62},        {KEY_F5, 63},
    {KEY_F6, 64},         {KEY_F7, 65},        {KEY_F8, 66},
    {KEY_F9, 67},         {KEY_F10, 68},       {KEY_F11, 68},
    {KEY_F12, 68},        {KEY_SYSRQ, 60},     {KEY_SCROLLLOCK, 70},
    {KEY_PAUSE, 119},

    {KEY_GRAVE, 41},      {KEY_1, 2},          {KEY_2, 3},
    {KEY_3, 4},           {KEY_4, 5},          {KEY_5, 6},
    {KEY_6, 7},           {KEY_7, 8},          {KEY_8, 9},
    {KEY_9, 10},          {KEY_0, 11},         {KEY_MINUS, 12},
    {KEY_EQUAL, 13},      {KEY_BACKSPACE, 14}, {KEY_INSERT, 110},
    {KEY_HOME, 102},      {KEY_PAGEUP, 104},

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

    {KEY_LEFTSHIFT, 42},  {KEY_Z, 44},         {KEY_X, 45},
    {KEY_C, 46},          {KEY_V, 47},         {KEY_B, 48},
    {KEY_N, 49},          {KEY_M, 50},         {KEY_COMMA, 51},
    {KEY_DOT, 52},        {KEY_SLASH, 53},     {KEY_RIGHTSHIFT, 54},
    {KEY_UP, 103},

    {KEY_LEFTCTRL, 29},   {KEY_LEFTMETA, 125}, {KEY_LEFTALT, 56},
    {KEY_SPACE, 57},      {KEY_RIGHTALT, 68},  {KEY_COMPOSE, 60},
    {KEY_RIGHTCTRL, 68},  {KEY_LEFT, 105},     {KEY_DOWN, 108},
    {KEY_RIGHT, 106},
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
