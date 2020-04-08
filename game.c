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
#include "game.h"

uint32 last_game_update = 0;
uint32 num_ticks = 0;
NOTE_GROUP test_group;

const uint8 num_note_groups = 3;
NOTE_GROUP note_groups[3];

void game_init() {
    
    for(int idx = 0; idx < num_note_groups; idx++) {
        note_groups[idx].active = FALSE;
        
    }
    
    test_group.num_notes = 3;
    test_group.center_hole = 4; // 0 indexed
    test_group.mode = SUCKED_NOTES;
    test_group.active = TRUE;
    test_group.pos = (Position){.x = get_x(test_group.center_hole), .y = 0};
    test_group.speed = 2;
    test_group.holes[0] = 3;
    test_group.holes[1] = 4;
    test_group.holes[2] = 5;
    test_group.length = 25;
    note_groups[0] = test_group;
}

uint8 get_x(uint8 hole) {
    if(hole > 9) {
        return 255;
    } else {
        return NOTE_LINE_START_OFFSET + 
                (NOTE_LINE_SPACING * hole);
    }
}

void game_tick(Harmonica* h) {
    if(ms_counter_ReadCounter() - last_game_update > GAME_UPDATE_PERIOD_MS) {
        
        clear_whole_display();
        CyDelay(1);
        draw_note_lines(0xFFFF);
        CyDelay(1);
        draw_play_line(0xFFFF);
        CyDelay(1);
        handle_move_note_groups(note_groups);
        handle_note_group_spawn(note_groups);
        draw_all_note_groups(h);
        num_ticks++;
        last_game_update = ms_counter_ReadCounter();
    }
}

void draw_note_lines(uint16_t color) {
    uint8 start_x = NOTE_LINE_START_OFFSET;
    uint8 x_delta = NOTE_LINE_SPACING;
    for(int line = 0; line < NUM_NOTE_LINES; line++) {
        draw_line(start_x,0, start_x, RGB_OLED_HEIGHT, color);
        start_x += x_delta;
    }
}

void draw_play_line(uint16_t color) {
    uint8 y = PLAY_LINE_Y;
    draw_line(0, y, RGB_OLED_WIDTH, y, color);
}

void draw_note_group(NOTE_GROUP* note_group, Harmonica* h) {
    if(note_group->active == FALSE) {
        return;
    }
    hBool isNegative = (note_group->pos.y < 0) ? TRUE : FALSE;
    uint8 start_y = (isNegative) ? 0 : note_group->pos.y;
    int8 end_y_signed = (isNegative) ? (note_group->pos.y + note_group->length): start_y + note_group->length;
    uint8 end_y = (end_y_signed < 0) ? 0 : (uint8) end_y_signed;
    hBool cross_play_line = ((PLAY_LINE_Y >= start_y) && (PLAY_LINE_Y < end_y)) ?
                                TRUE : FALSE;
    uint16 outline_color;
    uint16 body_color = get_body_color(note_group);
    if(cross_play_line == FALSE) {
        outline_color = body_color;
    } else {
        outline_color = (is_playing(h, note_group) == TRUE) ? PASS_COLOR : FAIL_COLOR;
    }
    
    for(int idx = 0; idx < note_group->num_notes; idx++) {
        uint8 start_x = get_x(note_group->holes[idx]) - (NOTE_WIDTH / 2);
        uint8 end_x = start_x + NOTE_WIDTH - 1;
        draw_rect(start_x, start_y, end_x, end_y, outline_color, body_color);
    }

}

void handle_move_note_groups(NOTE_GROUP* note_groups) {
    if(num_ticks > TICK_MOVEMENT_PERIOD) {
        for(int idx = 0; idx < num_note_groups; idx++) {
            NOTE_GROUP* note_group = &note_groups[idx];
            
            if(note_group->active == TRUE) {
                note_group->pos.y += note_group->speed;
                
                if(note_group->pos.y > RGB_OLED_HEIGHT) {
                    note_group->active = FALSE;
                }
            }
        }
        
        num_ticks = 0;
    }
    
}

uint16 get_body_color(NOTE_GROUP* note_group) {
    return (note_group->mode == BLOWN_NOTES) ? BLOWN_COLOR : SUCK_COLOR;
}

hBool is_playing(Harmonica* h, NOTE_GROUP* note_group) {
    uint8 harmonica_num_notes = h->raw_harmonica_output.num_notes_played;
    HOLE center_hole = h->active_hole;
    
    return harmonica_num_notes == note_group->num_notes &&
            center_hole == note_group->center_hole 
            && h->output_mode == NORMAL &&
            h->current_note_mode == note_group->mode;
}

void generate_random_note_group(NOTE_GROUP* note_group) {
    
    uint8 rand_val = (ms_counter_ReadCounter() % 10);
    note_group->center_hole = rand_val; // 0 indexed
    
    // calculating largest possible num_notes without bleeding outside of range
    uint8 upper_dist = 10 - note_group->center_hole + 1;
    uint8 lower_dist = note_group->center_hole + 1;
    
    uint8 max_num_notes = (upper_dist < lower_dist) ? upper_dist : lower_dist;
    
    note_group->num_notes = (ms_counter_ReadCounter() % max_num_notes) + 1;
    note_group->mode = ((game_random_mode_generator_ReadCounter() % 2) == 0) ? SUCKED_NOTES : BLOWN_NOTES;
    note_group->active = TRUE;
    note_group->pos = (Position){.x = get_x(note_group->center_hole), .y = -25};
    note_group->speed = 2;
    
    write_note_group_holes(note_group);
    
    
    note_group->length = ((ms_counter_ReadCounter() * game_random_mode_generator_ReadCounter()) % 16) + 15;
    
    CyDelay(note_group->length);
}

void write_note_group_holes(NOTE_GROUP* note_group) {
    // need to figure out lower hole
   uint8 hole_offset = note_group->num_notes / 2;
   int8 center_hole_int8 = (int8) note_group->center_hole;
   int8 lowest_hole_raw = (center_hole_int8 - hole_offset);
   int8 lowest_hole = (lowest_hole_raw < 0) ? 0 : lowest_hole_raw;
    note_group->num_notes += lowest_hole - lowest_hole_raw;
//        int8 hole_offset = (num_notes_played / 2);
//    int8 lowest_hole_raw = ((int)active_hole - hole_offset);
//    HOLE lowest_hole =  (lowest_hole_raw < ZERO) ? ZERO : (HOLE) (lowest_hole_raw);
    
    for(uint8 idx = 0; idx <= note_group->num_notes; idx++) {
        note_group->holes[idx] = idx + (uint8) lowest_hole;
    }
}

void handle_note_group_spawn(NOTE_GROUP* note_groups) {
    hBool all_note_groups_past_threshold = TRUE;
    
    for(int idx = 0; idx < num_note_groups; idx++) {
        NOTE_GROUP* ng = &note_groups[idx];
        if(ng->pos.y < NOTE_GROUP_SPAWN_Y_THRESH && ng->active == TRUE) {
            all_note_groups_past_threshold = FALSE;
        }
    }
    for(int idx = 0; idx < num_note_groups; idx++) {
        NOTE_GROUP* ng = &note_groups[idx];
        if(ng->active == FALSE && all_note_groups_past_threshold) {
            ng->active = TRUE;
            generate_random_note_group(ng);
            return;
        } 
    }
    
}

void draw_all_note_groups(Harmonica* h) {
    for(int idx = 0; idx < num_note_groups; idx++) {
        draw_note_group(&note_groups[idx], h);
    }
}

// Strategy

// have a maximum number of blocks on screen at once

// being maybe like 3 or 4

// it must be possible to play them, so far example, if two blocks come at the same time, they must be adjacent

// these blocks that are adjacent will count as one block

// basic idea: spawn one block- whether its a single or its a an adjacent group

// it has an x and y coordinate that's off the screen, when they are eventually reach like x coord = 10 or so
    // spawn another, if and only if the numbe rof blocks on the screen is less than a certain number

// Have a horizontal line at the bottom, and notify when the blocks are at that line (when y coords are in range)

// when the blocks are past the bottom of the screen, decrement the total number of blocks on the screen, allowing more to come

// keep track of the number of ticks that the blocks are over the line (basically the height, but have a way to keep track of the number of ticks a note was "correct")


// Data Structure:
    // Note_Group
        //x, y
        // speed
        // length
        // center_hole
        // num_notes
            // need a number to limit the number of notes based on center_hole
        // mode
        // active bool ??



/* [] END OF FILE */
