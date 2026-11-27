#include "i2c-lcd.h"

extern I2C_HandleTypeDef hi2c1;  // Modifie si ton I2C s'appelle hi2c2, etc.

#define SLAVE_ADDRESS_LCD (0x27 << 1)   // Adresse I2C de l'écran (souvent 0x4E ou 0x3F)

void lcd_send_cmd (char cmd) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x0C;  // en=1, rs=0
    data_t[1] = data_u | 0x08;  // en=0, rs=0
    data_t[2] = data_l | 0x0C;  // en=1, rs=0
    data_t[3] = data_l | 0x08;  // en=0, rs=0
    if (HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, data_t, 4, 100) != HAL_OK)
    {
        __NOP();   // mets un point d'arrêt ici
    }
}

void lcd_send_data (char data) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D;  // en=1, rs=1
    data_t[1] = data_u | 0x09;  // en=0, rs=1
    data_t[2] = data_l | 0x0D;  // en=1, rs=1
    data_t[3] = data_l | 0x09;  // en=0, rs=1
    HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD, (uint8_t *) data_t, 4, 100);
}



void lcd_init (void) {
    HAL_Delay(50);
    lcd_send_cmd (0x30);
    HAL_Delay(5);
    lcd_send_cmd (0x30);
    HAL_Delay(1);
    lcd_send_cmd (0x30);
    HAL_Delay(10);
    lcd_send_cmd (0x20);  // Mode 4 bits
    HAL_Delay(10);

    lcd_send_cmd (0x28); // 4-bit, 2 lignes, 5x8
    HAL_Delay(1);
    lcd_send_cmd (0x08); // Display off
    HAL_Delay(1);
    lcd_send_cmd (0x01); // Clear display
    HAL_Delay(2);
    lcd_send_cmd (0x06); // Entry mode set
    HAL_Delay(1);
    lcd_send_cmd (0x0C); // Display on, curseur off
    HAL_Delay(1);
}

void lcd_clear (void) {
    lcd_send_cmd (0x01);
    HAL_Delay(2);
}

void lcd_put_cur(int row, int col) {
    switch (row) {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xc0;
            break;
    }
    lcd_send_cmd (col);
}

void lcd_send_string (char *str) {
    while (*str) lcd_send_data (*str++);
}
