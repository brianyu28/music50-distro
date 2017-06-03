#include <arpa/inet.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "midi.h"

#define NOTE_START 0x90
#define NOTE_END 0x80
#define NOTE_VELOCITY 0x7f

uint8_t note_repr(char key, char accidental, int octave);
void put_vlv(unsigned long value, FILE *f);

song* create_song()
{
    song *s = calloc(sizeof(song), 1);
    return s;
}

void add_note(song *s, char key, char accidental, int octave, int duration)
{
    // allocate memory for note event
    event *e =  calloc(sizeof(event), 1);
    e->duration = duration;
    e->note = note_repr(key, accidental, octave);
    e->rest = 0;

    // append note to end of list of events
    if (s->latest == NULL)
        s->notes = e;
    else
        s->latest->next = e;
    s->latest = e;
    s->length += 2;
}

void add_rest(song *s, int duration)
{
    // allocate memory for rest event
    event *e = calloc(sizeof(event), 1);
    e->duration = duration;
    e->rest = 1;
    
    // append rest to end of list of events
    if (s->latest == NULL)
        s->notes = e;
    else
        s->latest->next = e;
    s->latest = e;
}

void write_song(song *s, char *filename)
{
    FILE *f = fopen(filename, "w");

    // write MIDI header
    MIDI_HEADER header;
    char *header_type = "MThd";
    strncpy(header.type, header_type, strlen(header_type));
    header.length = htonl(6);   // MIDI header always 6 bytes
    header.format = htons(0);   // Format 0, MIDI file with 1 track
    header.tracks = htons(1);   // Specifies 1 track MIDI file
    header.division = htons(2); // Quarter notes divided into 2 units
    fwrite(&header, sizeof(MIDI_HEADER), 1, f);
    
    // write track header 
    char *track_type = "MTrk";
    fwrite(track_type, strlen(track_type) * sizeof(char), 1, f);

    // write track size, dependent upon length of song
    uint32_t track_size = htonl(4 * (s->length + 1));
    fwrite(&track_size, sizeof(track_size), 1, f);

    // add notes
    event* e = s->notes;
    int prev_duration = 1;
    while (e)
    {
        if (e->rest)
        {
            // extend delay before next event
            prev_duration += e->duration;
        }
        else
        {
            // add event for the current note
            TRACK_EVENT te;
            put_vlv(prev_duration, f);
            te.event_type = NOTE_START;
            te.param1 = e->note;
            te.param2 = NOTE_VELOCITY;
            fwrite(&te, sizeof(TRACK_EVENT), 1, f);
    
            // stop the note after its duration
            put_vlv(e->duration, f);
            te.event_type = NOTE_END;
            fwrite(&te, sizeof(TRACK_EVENT), 1, f);
            prev_duration = 0;
        }
        
        // progress to next note in song
        e = e->next;
    }
    
    // end the MIDI file with ending code 0xff2f00
    TRACK_EVENT end;
    put_vlv(1, f);
    end.event_type = 0xff;
    end.param1 = 0x2f;
    end.param2 = 0;
    fwrite(&end, sizeof(TRACK_EVENT), 1, f);

    // free memory
    event* ptr = s->notes;
    while (ptr != NULL) {
        event *tmp = ptr->next;
        free(ptr);
        ptr = tmp;
    }
    free(s);
}

// note to MIDI representation
uint8_t note_repr(char key, char accidental, int octave)
{
    // determine the key's number
    key = toupper(key);
    if (key <= 'B')
        key += 7;
    key = 2 * (key - 'C');
    if (key > 5)
        key--;

    // adjust for octave
    key += octave * 12;

    // adjust for accidental
    if (accidental == '#')
        key++;
    else if (accidental == 'b')
        key--;
    return (uint8_t) key;
}

// writes variable length value to file
// adapated from http://midi.teragonaudio.com/tech/midifile/vari.htm
void put_vlv(unsigned long value, FILE *f)
{
   unsigned long buffer = value & 0x7f;
   while (value >>= 7)
   {
     buffer <<= 8;
     buffer |= ((value & 0x7f) | 0x80);
   }
   while(1)
   {
      putc(buffer, f);
      if (buffer & 0x80)
          buffer >>= 8;
      else
          break;
   }
}

