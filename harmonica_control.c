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
#include "harmonica_control.h"
Button button1;
Button button2;
Button button3;
Button button4;
Button button5;
Harmonica harmonica;

void init_harmonica_control(void) {
    init_button(&button1);
    init_button(&button2);
    init_button(&button3);
    init_button(&button4);
}
void init_harmonica(Harmonica* h) {
    h->current_note_mode = NO_NOTES;  
    h->is_playing = 0;
    for(int i = 0; i < 10; i++) {
        h->raw_harmonica_output.notes[i] = REST;
        h->raw_harmonica_output.num_notes_played = 1;
    }
    h->active_hole = NO_HOLE; 
    h->recent_change = FALSE;
    h->volume_multiplier = 1.0;
    h->output_mode = NORMAL;
    
    reset_playback_notes(h);
    reset_echo_data(h);

}

void reset_playback_notes(Harmonica* h) {
    for(int rec_idx = 0; rec_idx < MAX_RECORDING_NOTES; rec_idx ++) {
        for(int note_idx = 0; note_idx < NUM_HOLES; note_idx++) {
            h->playback_notes.recordings[rec_idx].notes[note_idx] = REST;
        }
        h->playback_notes.recordings[rec_idx].num_notes_played = 1;
        h->playback_notes.recordings[rec_idx].num_notes_played = 100;
    }
    h->playback_notes.recordings_idx = 0;
    h->playback_notes.last_note_time = 0;
    h->playback_notes.started_recording_first_note = FALSE;
    h->playback_notes.started_playing_first_note = FALSE;
    h->playback_notes.num_recordings = 0;
}

void reset_echo_data(Harmonica* h) {
    ECHO_DATA* echo_data = &(h->echo_data);
    echo_data->last_played_note.num_notes_played = 1;
    echo_data->last_played_note.duration = 0;
    echo_data->last_played_note.start_time = 0;
    
    for(int noteIDX = 0; noteIDX < 10; noteIDX++) {
        echo_data->last_played_note.notes[noteIDX] = REST;
    }
    echo_data->last_played_note_initialized = FALSE;
    echo_data->echo_volume_multiplier = 0.9;
    echo_data->last_echo_time = 0;
    echo_data->echo_delay = 1000;
    echo_data->echo_duration = 600;
    echo_data->play_echo = FALSE;
    echo_data->num_echos_played = 0;
    echo_data->waiting_for_note_finish = FALSE;
}
void set_note_mode(Harmonica* h) {
    Button_State b1_state = poll_button(&button1, Button1_Read());
    Button_State b2_state = poll_button(&button2, Button2_Read());
    
    if(b1_state == HELD) {
        h->current_note_mode = BLOWN_NOTES;
    }
    else if(b2_state == HELD) {
        h->current_note_mode = SUCKED_NOTES;
    }
    else {
        h->current_note_mode = NO_NOTES;
    }
}

void set_output_mode(Harmonica* h) {
    Button_State b3_state = poll_button(&button3, Button3_Read());
    Button_State b4_state = poll_button(&button4, Button4_Read());
    Button_State b5_state = poll_button(&button5, Button5_Read());
    
    if(b3_state == FALLING_EDGE) {
        reset_playback_notes(h);
        h->output_mode = (h->output_mode == RECORDING) ? NORMAL : RECORDING;
    }
    else if(b4_state == FALLING_EDGE) {
        if(h->output_mode == RECORDING) {
            h->output_mode = PLAYBACK;
        } else {
            h->output_mode = NORMAL;
        }
    }
    else if(b5_state == FALLING_EDGE) {
        if(h->output_mode == ECHO) {
            h->output_mode = NORMAL;
        } else {
            reset_echo_data(h);
            h->output_mode = ECHO;
        }
    }
}

HOLE channel_to_hole(int8 channel, int16 channel_val) {
    
    if(channel_val < ACTIVE_HOLE_ADC_THRESHOLD) {
        return NO_HOLE;
    }
    switch(channel) {
        case 0:
            return ZERO;
        break;
        case 1:
            return ONE;
        break;
        case 2:
            return TWO;
        break;
        case 3:
            return THREE;
        break;
        case 4:
            return FOUR;
        break;
        case 5:
            return FIVE;
        break;
        case 6:
            return SIX;
        break;
        case 7:
            return SEVEN;
        break;
        case 8:
            return EIGHT;
        break;
        case 9:
            return NINE;
        break;
        default:
            return NO_HOLE;
        break;
    }
}

void set_active_hole(Harmonica* h) {
    
    if(h->current_note_mode == NO_NOTES) {
        h->active_hole = NO_HOLE;
        return;
    }
 
    int16 max_channel_val = -10;
    int8 max_channel = -1;
      
    for(int ch = 0; ch < MAX_CHANNELS; ch++) {
        int16 channel_val = photo_read_mux(ch);
        if(channel_val > max_channel_val) {
            max_channel = ch;
            max_channel_val = channel_val;
        }
    }
    
    if(max_channel_val > 100) {
        h->active_hole = channel_to_hole(max_channel, max_channel_val);
    }
    
}

int8 get_dac_output() {
    // right now this just returns the 
}

NOTE_MODE get_note_mode(Harmonica* h) {
    return h->current_note_mode;
}

HOLE get_active_hole(Harmonica* h) {
    return h->active_hole;
}

void make_harmonica_copy(Harmonica* proof, Harmonica* copy) {
    copy->active_hole = proof->active_hole;
    copy->current_note_mode = proof->current_note_mode;
    copy->is_playing = proof->is_playing;
    
    for(int i = 0; i < 10; i++) {
        copy->raw_harmonica_output.notes[i] = proof->raw_harmonica_output.notes[i];
    }
    copy->raw_harmonica_output.num_notes_played = proof->raw_harmonica_output.num_notes_played;
    copy->recent_change = proof->recent_change;
    copy->volume_multiplier = proof->volume_multiplier;
}

hBool check_recent_change(Harmonica* after_update, Harmonica* before_update) {
    
    if(after_update->active_hole != before_update->active_hole) {
        return TRUE;
    }
    if(after_update->current_note_mode != before_update->current_note_mode) {
        return TRUE;
    }
    
    if(after_update->is_playing != before_update->is_playing) {
        return TRUE;
    }
    
    for(int i = 0; i < 10; i++) {
        if(after_update->raw_harmonica_output.notes[i] != before_update->raw_harmonica_output.notes[i]) {
            return TRUE;
        }
    }
    
    if(after_update->raw_harmonica_output.num_notes_played != before_update->raw_harmonica_output.num_notes_played) {
        return TRUE;
    }
    
    if(after_update->volume_multiplier != before_update->volume_multiplier ) {
        return TRUE;
    }
    return FALSE;
}

void set_number_of_notes(Harmonica* h) {
    
    Num_Notes_Selector_ADC_StartConvert();
    Num_Notes_Selector_ADC_IsEndConversion(Num_Notes_Selector_ADC_WAIT_FOR_RESULT);
    uint8 pot_result = (uint8) Num_Notes_Selector_ADC_GetResult8();
    Num_Notes_Selector_ADC_StopConvert();
    uint8 num_notes_played = ((uint8) pot_result / 42) + 1;
    num_notes_played = (num_notes_played > MAX_NUM_NOTES_PLAYED) ? MAX_NUM_NOTES_PLAYED : num_notes_played;
    h->raw_harmonica_output.num_notes_played = num_notes_played;
}

void set_active_notes(Harmonica* h) {
    
    if(h->current_note_mode == NO_NOTES || h->active_hole == NO_HOLE) {
        clear_active_notes(h);
        return;
    }
    
    uint8 note_idx = 0;
    hBool get_note_from_left = TRUE;
    uint8 num_notes_played = h->raw_harmonica_output.num_notes_played;
    HOLE active_hole = h->active_hole;
    
    int8 hole_offset = (num_notes_played / 2);
    int8 lowest_hole_raw = ((int)active_hole - hole_offset);
    HOLE lowest_hole =  (lowest_hole_raw < ZERO) ? ZERO : (HOLE) (lowest_hole_raw);
    int8 highest_hole_raw = (int)active_hole + (hole_offset);
    
    highest_hole_raw = (num_notes_played % 2 == 0) ? highest_hole_raw - 1 : highest_hole_raw;
    
    HOLE highest_hole =  (highest_hole_raw > NINE)? NINE : (HOLE)(highest_hole_raw);
    char   hole_char[6u];
    // write to the screen what the holes are
    for(uint8 hole = (uint8) lowest_hole; hole < ((uint8) highest_hole) + 1; hole++) {
        h->raw_harmonica_output.notes[note_idx] = get_note_from_hole((HOLE) hole, h->current_note_mode);
        
        LCD_Position(1u, note_idx + 2);
        sprintf(hole_char, "%d", hole);
        LCD_PrintString(hole_char);
        note_idx++;
        
    }
    return;
}

NOTE get_note_from_hole(HOLE hole, NOTE_MODE mode) {
    
    if(mode == NO_NOTES) {
        return REST;
    }    
    switch(hole) {
        case ZERO:
            return (mode == BLOWN_NOTES) ? C4 : D4;
        break;
        case ONE:
            return (mode == BLOWN_NOTES) ? E4 : G4;
        break;
        case TWO:
            return (mode == BLOWN_NOTES) ? G4 : B4;
        break;
        case THREE:
            return (mode == BLOWN_NOTES) ? C5 : D5;
        break;
        case FOUR:
            return (mode == BLOWN_NOTES) ? E5 : F5;
        break;
        case FIVE:
            return (mode == BLOWN_NOTES) ? G5 : A5;
        break;
        case SIX:
            return (mode == BLOWN_NOTES) ? C6 : B5;
        break;
        case SEVEN:
            return (mode == BLOWN_NOTES) ? E6 : D6;
        break;
        case EIGHT:
            return (mode == BLOWN_NOTES) ? G6 : F6;
        break;
        case NINE:
            return (mode == BLOWN_NOTES) ? C7 : A6;
        break;
        default:
            return REST;
        break;
    }
}

void clear_active_notes(Harmonica* h) {
    for(int note_idx = 0; note_idx < 10; note_idx++) {
        h->raw_harmonica_output.notes[note_idx] = REST;
    }
}


void harmonica_tick(Harmonica* h) {
    
    h->recent_change = FALSE;
    Harmonica old_h;
    make_harmonica_copy(h, &old_h);
    set_note_mode(h);
    set_output_mode(h);
    set_active_hole(h);
    set_number_of_notes(h);
    
    set_active_notes(h);
    h->recent_change = check_recent_change(h, &old_h);
    if(h->recent_change) {
        reset_wav_tables_counter();
    }
    process_effects(h);
}

void process_effects(Harmonica* h) {
    switch(h->output_mode) {
    case RECORDING:
        if(h->recent_change) {
            if(h->playback_notes.started_recording_first_note == TRUE) {
                uint32 start_time = h->playback_notes.recordings[h->playback_notes.recordings_idx].start_time;
                h->playback_notes.recordings[h->playback_notes.recordings_idx].duration = ms_counter_ReadCounter() - start_time;
                h->playback_notes.recordings_idx = (h->playback_notes.recordings_idx + 1) % MAX_RECORDING_NOTES;
            }
            uint8 recordings_idx = h->playback_notes.recordings_idx;
            copy_notes(h->playback_notes.recordings[recordings_idx].notes, h->raw_harmonica_output.notes, h->raw_harmonica_output.num_notes_played);
            h->playback_notes.recordings[recordings_idx].num_notes_played = h->raw_harmonica_output.num_notes_played;
            h->playback_notes.recordings[recordings_idx].start_time = ms_counter_ReadCounter();
            h->playback_notes.started_recording_first_note = TRUE;
        }
        break;
    case ECHO:
        if(h->recent_change) {
            ECHO_DATA* echo_data = &(h->echo_data);
            if(is_silent(h) == FALSE) {
                copy_notes(echo_data->last_played_note.notes, h->raw_harmonica_output.notes, h->raw_harmonica_output.num_notes_played);
                echo_data->last_played_note.num_notes_played = h->raw_harmonica_output.num_notes_played;
                echo_data->last_played_note.start_time = ms_counter_ReadCounter();
                echo_data->waiting_for_note_finish = TRUE;
                echo_data->last_played_note_initialized = FALSE;
            } else {
                if(echo_data->waiting_for_note_finish == TRUE) {
                    echo_data->waiting_for_note_finish = FALSE;
                    if((ms_counter_ReadCounter() - echo_data->last_played_note.start_time) > MIN_ECHO_NOTE_LENGTH) {
                        echo_data->last_played_note_initialized = TRUE;
                        echo_data->last_echo_time = ms_counter_ReadCounter();
                        echo_data->play_echo = FALSE;
                        echo_data->num_echos_played = 0;
                        echo_data->echo_volume_multiplier = 0.9;
                    }
                }
            }
        }
        break;
    default:
        break;
    }
}

void copy_notes(NOTE* copy, NOTE* proof, uint8 num_notes) {
    for(int i = 0; i < num_notes; i++) {
        copy[i] = proof[i];
    }
    return;
}

uint8_t get_sound_output(Harmonica* h) {
    uint8 rec_out;
    ECHO_DATA* echo_data = &(h->echo_data);
    switch(h->output_mode) {
    case NORMAL:
        return get_raw_dac_output(h);
        break;
    case ECHO:
        if(echo_data->last_played_note_initialized == FALSE) {
            return get_raw_dac_output(h);
        }
        else {
            if(echo_data->play_echo == FALSE) {
                if((ms_counter_ReadCounter() - echo_data->last_echo_time > echo_data->echo_delay)) {
                    echo_data->play_echo = TRUE;
                    echo_data->last_echo_time = ms_counter_ReadCounter();
                }
                // should return the same note, but rapidly decreases in volume at the end
//                float echo_delay = (float) echo_data->echo_delay;
//                float proportion_time_finished = (echo_delay - ((float)ms_counter_ReadCounter() - (float)echo_data->last_echo_time)) / echo_delay;
//                return (uint8) (proportion_time_finished *
//                            echo_data->echo_volume_multiplier * 
//                                    get_notes_val(echo_data->last_played_note.notes, echo_data->last_played_note.num_notes_played));
                return 0;
            }
            else if(echo_data->play_echo == TRUE) {
                
                 if((ms_counter_ReadCounter() - echo_data->last_echo_time > echo_data->echo_duration)) {
                    echo_data->last_echo_time = ms_counter_ReadCounter();
                    echo_data->play_echo = FALSE;
                    echo_data->num_echos_played++;
                    echo_data->echo_volume_multiplier = echo_data->echo_volume_multiplier / 1.5;
                 }
                
                if(echo_data->num_echos_played < MAX_ECHOS_PLAYED) {
                    return (uint8) (echo_data->echo_volume_multiplier * get_notes_val(echo_data->last_played_note.notes, echo_data->last_played_note.num_notes_played));
                }
                    echo_data->last_played_note_initialized = FALSE;
                    echo_data->waiting_for_note_finish = FALSE;
                    echo_data->num_echos_played = 0;
                    echo_data->echo_volume_multiplier = 0.9;
                    return 0;
            } 
        }
        break;
    case RECORDING:
        return get_raw_dac_output(h);
        break;
    case PLAYBACK:
        rec_out = get_recording_output(h);
        return rec_out;
    }
    return 0;
}

uint8 get_recording_output(Harmonica* h) {
    PLAYBACK_NOTES* playback_notes = &(h->playback_notes);
    if(playback_notes->started_playing_first_note == FALSE) {
        playback_notes->started_playing_first_note = TRUE;
        playback_notes->last_note_time = ms_counter_ReadCounter();
        playback_notes->num_recordings = playback_notes->recordings_idx +2;
        playback_notes->recordings_idx = 0;
        reset_wav_tables_counter();
    }
    uint32 last_note_time = playback_notes->last_note_time;
    uint32 duration = playback_notes->recordings[playback_notes->recordings_idx].duration;
    uint32 current_time = ms_counter_ReadCounter();
    if(current_time - last_note_time > duration) {
        playback_notes->recordings_idx++;
        if(playback_notes->recordings_idx >= playback_notes->num_recordings) {
            h->output_mode = NORMAL;
            return 0;
        }
        reset_wav_tables_counter();
        playback_notes->last_note_time = ms_counter_ReadCounter();
    }
    
    uint8 recordings_idx = playback_notes->recordings_idx;
    NOTE* notes = playback_notes->recordings[recordings_idx].notes;
    uint8 num_notes_played = playback_notes->recordings[recordings_idx].num_notes_played;
    return get_notes_val(notes, num_notes_played);
}

uint8 get_notes_val(NOTE* notes, uint8 num_notes_played) {
    uint16 summed_value = 0;
    for(uint8 note_index = 0; note_index < num_notes_played; note_index++) {
        uint8 added_value = 0;
        switch(notes[note_index]) {
            case C4:
                added_value = get_wav_val_and_inc(&c4_sus);
            break;
            case D4:
                added_value = get_wav_val_and_inc(&d4_sus);
            break;
            case G4:
                added_value = get_wav_val_and_inc(&g4_sus);
            break;
            case E4:
                added_value = get_wav_val_and_inc(&e4_sus);
            break;
            case B4:
                added_value = get_wav_val_and_inc(&b4_sus);
            break;
            case C5:
                added_value = get_wav_val_and_inc(&c5_sus);
            break;
            case D5:
                added_value = get_wav_val_and_inc(&d5_sus);
            break;
            case E5:
                added_value = get_wav_val_and_inc(&e5_sus);
            break;
            case F5:
                added_value = get_wav_val_and_inc(&f5_sus);
            break;
            case G6:
                added_value = get_wav_val_and_inc(&g6_sus);
            break;
            case A5:
                added_value = get_wav_val_and_inc(&a5_sus);
            break;
            case C6:
                added_value = get_wav_val_and_inc(&c6_sus);
            break;
            case E6:
                added_value = get_wav_val_and_inc(&e6_sus);
            break;
            case D6:
                added_value = get_wav_val_and_inc(&d6_sus);
            break;  
            case F6:
                added_value = get_wav_val_and_inc(&f6_sus);
            break;
            case C7:
                added_value = get_wav_val_and_inc(&c7_sus);
            break;
            case A6:
                added_value = get_wav_val_and_inc(&a6_sus);
            break;
            case G5:
                added_value = get_wav_val_and_inc(&g5_sus);
            break;
            case B5:
                added_value = get_wav_val_and_inc(&b5_sus);
            break;
            default:
            break;
        }
        summed_value = summed_value + added_value;
    }
    return summed_value / num_notes_played;
}

hBool is_silent(Harmonica* h) {
    for(int noteIDX = 0; noteIDX < 10; noteIDX++) {
        if(h->raw_harmonica_output.notes[noteIDX] != REST) {
            return FALSE;
        }
    }
    return TRUE;
}

uint8 get_raw_dac_output(Harmonica* h) {
    // using a counter specific to every wav table
    // indexes into the wav tables corresponding to the each note in h->raw_harmonica_output->notes[0...num_notes_played - 1]
    // sums all these values
    // divides by num_notes_played
    // increments every counter
    
        // the wav tables:
            // one named for every note : 
            // struct storing pointer to the wav table
            //        storing pointer to a global counter for each table
            //        storing the length of one period, given sample rate of 44100
            // okay now to generate the wav tables
    // return
    NOTE* notes = h->raw_harmonica_output.notes;
    uint8 num_notes_played = h->raw_harmonica_output.num_notes_played;
    
    
    if(h->current_note_mode == NO_NOTES) {
        return 0;
    }
    return get_notes_val(notes, num_notes_played);
}

/* [] END OF FILE */
