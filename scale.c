/*
 * scale.c
 *
 * Creates a MIDI file that plays the C scale.
 */
#include <stdio.h>
#include "midi.h"

int main(void)
{
    song *s = create_song();
    add_note(s, 'C', '_', 4, 1);
    add_note(s, 'D', '_', 4, 1);
    add_note(s, 'E', '_', 4, 1);
    add_note(s, 'F', '_', 4, 1);
    add_note(s, 'G', '_', 4, 1);
    add_note(s, 'A', '_', 4, 1);
    add_note(s, 'B', '_', 4, 1);
    add_note(s, 'C', '_', 5, 1);
    add_note(s, 'B', '_', 4, 1);
    add_note(s, 'A', '_', 4, 1);
    add_note(s, 'G', '_', 4, 1);
    add_note(s, 'F', '_', 4, 1);
    add_note(s, 'E', '_', 4, 1);
    add_note(s, 'D', '_', 4, 1);
    add_note(s, 'C', '_', 4, 1);
    write_song(s, "scale.mid");
}
