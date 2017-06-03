#include <stdint.h>

/**
 * MIDI_HEADER
 *
 * The MIDI_HEADER structure contains information about the 
 * format, number of tracks, and timing of a MIDI file.
 */
typedef struct
{
    char type[4];
    uint32_t length;
    uint16_t format;
    uint16_t tracks;
    uint16_t division;
} __attribute__((__packed__)) 
MIDI_HEADER;

/**
 * TRACK_EVENT
 *
 * The TRACK_EVENT structure contains information about a
 * single MIDI event, including a note played and its volume.
 */
typedef struct
{
    uint8_t event_type;
    uint8_t param1;
    uint8_t param2;
} __attribute__((__packed__)) 
TRACK_EVENT;

/**
 * The event structure maintains a linked list of notes and
 * rests, and contains information about the duration of each.
 */
typedef struct event
{
    uint8_t duration;
    uint8_t note;
    struct event *next; 
    uint8_t rest;
}
event;

/**
 * The song structure contains information about the notes
 * in a song, the length of the song, and the most recent
 * note or rest in a song.
 */
typedef struct
{
    event *notes;
    event *latest;
    int length;
}
song;

/**
 * create_song
 * 
 * create_song allocates space in memory for a new song which 
 * initially contains no notes or rests.
 */
song* create_song();

/**
 * add_note
 * 
 * add_note adds a note to a song at the end of a song's existing contents
 *
 * Parameters:
 *      s - the song to which the note should be added
 *      key - a character (e.g. 'A', B', 'C') representing the note to add
 *      accidental - either '_' (natural), '#' (sharp), or 'b' (flat)
 *      octave - a number from 0-9 representing the note's octave,
 *               where octave 4 is the middle octave
 *      duration - a positive integer denoting how long (in eighth notes) the
 *                 note should last (assuming 4/4 time)
 */
void add_note(song *s, char key, char accidental, int octave, int duration);

/**
 * add_rest
 *
 * add_rest adds a rest to a song at the end of a song's existing contents
 *
 * Parameters:
 *      s - the song to which the rest should be added
 *      duration - a positive integer denoting how long (in eighth notes) the
 *                 rest should last (assuming 4/4 time)
 */
void add_rest(song *s, int duration);

/**
 * write_song
 *
 * write_song takes a song structure and outputs its contents as a MIDI
 * file to disk. After writing the file, write_song frees all memory 
 * allocated by the song structure.
 *
 * Parameters:
 *      s - the song to convert to a MIDI file 
 *      filename - the relative path of the MIDI file to output
 */
void write_song(song *s, char *filename);
