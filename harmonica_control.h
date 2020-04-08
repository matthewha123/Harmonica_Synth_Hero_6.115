/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef __HARMONICA_CONTROL_H_
#define __HARMONICA_CONTROL_H_
#include "button.h"
#include "photo_mux.h"
#include "LCD.h"
#include "wav_tables.h"
#include "audio_isr.h"
#include "ms_counter.h"
#include "Num_Notes_Selector_ADC.h"

// need a function to handle the current mode
    // reads the first button, the first button toggles between blown and sucked note
    
#define ACTIVE_HOLE_ADC_THRESHOLD 40
#define MAX_RECORDING_NOTES 20
#define NUM_HOLES 10
#define MAX_NUM_NOTES_PLAYED 6
    
#define MAX_ECHOS_PLAYED  10
#define SILENCE_TIME_THRESH_BEFORE_ECHO 500
#define MIN_ECHO_NOTE_LENGTH  60
    
typedef enum {
    FALSE = 0,
    TRUE = 1,
} hBool;    
typedef enum {
    BLOWN_NOTES = 0,
    SUCKED_NOTES = 1,
    NO_NOTES = 2,
} NOTE_MODE;

typedef enum {
    NORMAL,
    ECHO,
    RECORDING,
    PLAYBACK,
} OUTPUT_MODE;

typedef enum {
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    NO_HOLE,
} HOLE;

typedef enum {
    C4,
    D4,
    E4,
    G4,
    G5,
    B4,
    C5,
    D5,
    E5,
    F5,
    G6,
    A5,
    C6,
    B5,
    E6,
    D6,
    F6,
    C7,
    A6,
    REST,
} NOTE;


typedef struct {
    NOTE notes[10];
    int8 num_notes_played;
} HARMONICA_OUTPUT;

typedef struct {
    NOTE notes[10];
    int8 num_notes_played;
    uint32 duration;
    uint32 start_time;
} PLAYBACK_NOTE;

typedef struct {
    PLAYBACK_NOTE recordings[MAX_RECORDING_NOTES];
    uint8 recordings_idx;
    uint32 last_note_time;
    hBool started_recording_first_note;
    hBool started_playing_first_note;
    uint8 num_recordings;
} PLAYBACK_NOTES;

typedef struct {
    PLAYBACK_NOTE last_played_note;    // won't actually use the duration, but use PLAYBACK_NOTE to check for the start time, only echo notes that are at least 100 ms long
    hBool waiting_for_note_finish;
    hBool last_played_note_initialized; // false initially
    float echo_volume_multiplier; // decreases with every echo played
    uint32 last_echo_time;       // used to check echo_delay times during silence and during echo
    uint32 echo_delay;          // time between echos
    uint32 echo_duration;      // time during echo
    hBool play_echo;          // whether to play echo or not
    uint8 num_echos_played; // cap this to like 10 or something
} ECHO_DATA;

typedef struct {
    NOTE_MODE current_note_mode;
    HOLE active_hole;
    int8 is_playing; // 0 when not playing, 1 when playing
    HARMONICA_OUTPUT raw_harmonica_output;
    hBool recent_change;
    float volume_multiplier;
    OUTPUT_MODE output_mode;
    PLAYBACK_NOTES playback_notes;
    ECHO_DATA echo_data;
} Harmonica;

extern Harmonica harmonica;

/* 
 this function initializes globals needed for harmonica control, such as buttons
*/
void init_harmonica_control(void);

/*
 sets Harmonica default values
*/
void init_harmonica(Harmonica* h);

/*
    Using Button input, sets the note mode of the Harmonica
    Where Note mode determines whether a the harmonica is being played
    And, if so, whether the playing should produce sucked or blown notes
*/
void set_note_mode(Harmonica* h);

/*
    Returns the note mode of the harmonica
*/
NOTE_MODE get_note_mode(Harmonica* h);

/*
    Gets the active hole of the harmonica (the phototransistor that is being shined on the most
*/
HOLE get_active_hole(Harmonica* h);

/*
    Converts the channel of the phototransistor mux to a hole in the harmonica
    Thresholds channel_val
*/
HOLE channel_to_hole(int8 channel, int16 channel_val);

/*
    Copies all the parameters of proof into copy
*/
void make_harmonica_copy(Harmonica* proof, Harmonica* copy);

/*
    Called in harmonica_tick to get the number of notes that should be played
*/
void set_number_of_notes(Harmonica* h);

/*
    Given the number of notes, and the note_mode of Harmonica, sets the harmonica's active notes
*/
void set_active_notes(Harmonica* h);

/*
    Returns the note at HOLE hole given NOTE_MODE mode
*/
NOTE get_note_from_hole(HOLE hole, NOTE_MODE mode);
/*
    Checks whether a change has occured in the Harmonica, called in harmonica_tick
*/
hBool check_recent_change(Harmonica* after_update, Harmonica* before_update);
/*
    Called from the audio_isr, gets appropriate audio output from the harmonica
    Its behaviour depends on the active hole, the number of notes to be played at once
    Effects such as echo, and playback
*/

uint8 get_raw_dac_output(Harmonica* h); 

int8 get_dac_out(/* some struct*/);


/*
    Called in the main loop to take in user input via buttons/lit phototransistor to
    control the harmonica audio output
*/
void harmonica_tick(Harmonica* h);
    
void process_effects(Harmonica* h);

void copy_notes(NOTE* copy, NOTE* proof, uint8 num_notes);

void reset_playback_notes(Harmonica* h);

uint8 get_recording_output(Harmonica* h);

uint8 get_notes_val(NOTE* notes, uint8 num_notes_played);

void clear_active_notes(Harmonica* h);

void reset_echo_data(Harmonica* h);

hBool is_silent(Harmonica* h);

#endif // HARMONICA_CONTROL_H_
/* [] END OF FILE */
