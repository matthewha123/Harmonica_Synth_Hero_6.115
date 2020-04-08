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
#include "oled_ssd1331.h"
#include "common.h"

uint8 screen_buffer[255];

void send_cmd(uint8 cmd) {
    OLED_DC_Write(0);
    OLED_CS_Write(0);
    SPI_OLED_WriteTxData(cmd);
    while(spi_done == 0) {
    }
    spi_done = 0;
    // OLED_CS_Write(1);
}

void send_data(uint8 data) {
    OLED_DC_Write(1);
    OLED_CS_Write(0);
    SPI_OLED_WriteTxData(data);
    while(spi_done == 0) {
    }
    spi_done = 0;
    // OLED_CS_Write(1);
}

void init_oled() {
    OLED_CS_Write(0);
    send_cmd(SSD1331_CMD_DISPLAYOFF);  	// 0xAE
    send_cmd(SSD1331_CMD_SETREMAP); 	// 0xA0
    send_cmd(0x72);				// RGB Color
    send_cmd(SSD1331_CMD_STARTLINE); 	// 0xA1
    send_cmd(0x0);
    send_cmd(SSD1331_CMD_DISPLAYOFFSET); 	// 0xA2
    send_cmd(0x0);
    send_cmd(SSD1331_CMD_NORMALDISPLAY);  	// 0xA4
    send_cmd(SSD1331_CMD_SETMULTIPLEX); 	// 0xA8
    send_cmd(0x3F);  			// 0x3F 1/64 duty
    send_cmd(SSD1331_CMD_SETMASTER);  	// 0xAD
    send_cmd(0x8E);
    send_cmd(SSD1331_CMD_POWERMODE);  	// 0xB0
    send_cmd(0x0B);
    send_cmd(SSD1331_CMD_PHASEPERIOD);  	// 0xB1
    send_cmd(0x11);
    send_cmd(SSD1331_CMD_CLOCKDIV);  	// 0xB3
    send_cmd(0xF0);  // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    send_cmd(SSD1331_CMD_PRECHARGEA);  	// 0x8A
    send_cmd(0x64);
    send_cmd(SSD1331_CMD_PRECHARGEB);  	// 0x8B
    send_cmd(0x78);
    send_cmd(SSD1331_CMD_PRECHARGEC);  	// 0x8C
    send_cmd(0x64);
    send_cmd(SSD1331_CMD_PRECHARGELEVEL);  	// 0xBB
    send_cmd(0x3A);
    send_cmd(SSD1331_CMD_VCOMH);  		// 0xBE
    send_cmd(0x3E);
    send_cmd(SSD1331_CMD_MASTERCURRENT);  	// 0x87
    send_cmd(0x06);
    send_cmd(SSD1331_CMD_CONTRASTA);  	// 0x81
    send_cmd(0xFF);
    send_cmd(SSD1331_CMD_CONTRASTB);  	// 0x82
    send_cmd(0xFF);
    send_cmd(SSD1331_CMD_CONTRASTC);  	// 0x83
    send_cmd(0xFF);
    send_cmd(CMD_DEACTIVE_SCROLLING);
    send_cmd(SSD1331_CMD_DISPLAYON);	//--turn on oled panel
    clear_whole_display();
    
    for(int i = 0; i < 255; i++) {
        screen_buffer[i] = 0x00;
    }
}

void draw_pixel(uint8 x, uint8 y, uint16 color) {
    if(x >= RGB_OLED_WIDTH || y >= RGB_OLED_HEIGHT) {
        return;
    }
    OLED_CS_Write(0);
    send_cmd(CMD_SET_COLUMN_ADDRESS);
    send_cmd(x);
    send_cmd(RGB_OLED_WIDTH-1);
    //set row point
    send_cmd(CMD_SET_ROW_ADDRESS);
    send_cmd(y);
    send_cmd(RGB_OLED_HEIGHT-1);
    send_data(color >> 8);
    send_data(color);
    
}

void draw_line(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint16 color) {
    if((x0 < 0) || (y0 < 0) || (x1 < 0) || (y1 < 0))
        return;
    OLED_CS_Write(0);
    if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_WIDTH - 1;
    if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_HEIGHT - 1;
    if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_WIDTH - 1;
    if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_HEIGHT - 1;

    send_cmd(CMD_DRAW_LINE);//draw line
    send_cmd(x0);//start column
    send_cmd(y0);//start row
    send_cmd(x1);//end column
    send_cmd(y1);//end row
    send_cmd((uint8_t)((color>>11)&0x1F));//R
    send_cmd((uint8_t)((color>>5)&0x3F));//G
    send_cmd((uint8_t)(color&0x1F));//B
}

void draw_rect(uint8 x0, uint8 y0, uint8 x1, uint8 y1, uint16 out_color, uint16 fill_color) {
    if((x0 < 0) || (y0 < 0) || (x1 < 0) || (y1 < 0))
        return;
    OLED_CS_Write(0);
    if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_WIDTH - 1;
    if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_HEIGHT - 1;
    if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_WIDTH - 1;
    if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_HEIGHT - 1;

    send_cmd(CMD_FILL_WINDOW);//fill window
    send_cmd(ENABLE_FILL);
    send_cmd(CMD_DRAW_RECTANGLE);//draw rectangle
    send_cmd(x0);//start column
    send_cmd(y0);//start row
    send_cmd(x1);//end column
    send_cmd(y1);//end row
    send_cmd((uint8_t)((out_color>>11)&0x1F));//R
    send_cmd((uint8_t)((out_color>>5)&0x3F));//G
    send_cmd((uint8_t)(out_color&0x1F));//B
    send_cmd((uint8_t)((fill_color>>11)&0x1F));//R
    send_cmd((uint8_t)((fill_color>>5)&0x3F));//G
    send_cmd((uint8_t)(fill_color&0x1F));//B
}

void clear_whole_display() {
    OLED_CS_Write(0);
    draw_rect(0,0,RGB_OLED_WIDTH, RGB_OLED_HEIGHT, 0x0000, 0x0000);
}

void clear_whole_display_by_pixel() {
    
    send_cmd(CMD_SET_COLUMN_ADDRESS);
    send_cmd(0);
    send_cmd(RGB_OLED_WIDTH-1);
    //set row point
    send_cmd(CMD_SET_ROW_ADDRESS);
    send_cmd(0);
    
    for(int i = 0; i < 49; i++) {
        SPI_OLED_PutArray(screen_buffer, 255);
    }
}

/* [] END OF FILE */
