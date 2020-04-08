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
#ifndef _GAME_H_
#define _GAME_H_
#include "harmonica_control.h"
#include "oled_ssd1331.h"
#include "game_random_mode_generator.h"


#define NUM_NOTE_LINES 10
#define GAME_UPDATE_PERIOD_MS 100
    
#define NOTE_WIDTH 5
    
#define SUCK_COLOR 0xA01F
#define BLOWN_COLOR 0x001F
    
#define FAIL_COLOR 0xF100
#define PASS_COLOR 0x07E0
    
#define PLAY_LINE_Y 55
#define NOTE_LINE_SPACING 10
#define NOTE_LINE_START_OFFSET 3
    
#define TICK_MOVEMENT_PERIOD 2
#define NOTE_GROUP_SPAWN_Y_THRESH 15
    
void game_init();
void game_tick(Harmonica* h);

typedef struct {
    uint8 x;
    int y;
} Position;

typedef struct {
    uint8 num_notes;
    uint8 center_hole;
    NOTE_MODE mode;
    hBool active;
    Position pos;
    uint8 speed;
    uint8 holes[MAX_NUM_NOTES_PLAYED];
    uint8 length;
} NOTE_GROUP;

// need a global array of NOTE_GROUP, all initially set to active = false

// need a function to generate a NOTE_GROUP
    // loops through all NOTE_GROUP
        // if all active NOTE_GROUP is > some y coord
        //    and there is an inactive note, take that inactive note and make it active with random paramters

// need a function to generate the random parameters
// need a function to limit num_notes based on center_hole


void handle_note_group_spawn(NOTE_GROUP* note_groups);
void generate_random_note_group(NOTE_GROUP* note_group);
void draw_note_lines(uint16_t color);
void draw_play_line(uint16_t color);
void handle_detect_play(Harmonica* harmonica);
void handle_move_note_groups(NOTE_GROUP* note_groups);

void draw_note_group(NOTE_GROUP* note_group, Harmonica* h);
void draw_all_note_groups(Harmonica* h);
uint8 get_x(uint8 hole);
uint16 get_body_color(NOTE_GROUP* note_group);
hBool is_playing(Harmonica* h, NOTE_GROUP* note_group);

void write_note_group_holes(NOTE_GROUP* note_group);
#endif // _GAME_H_
/* [] END OF FILE */
