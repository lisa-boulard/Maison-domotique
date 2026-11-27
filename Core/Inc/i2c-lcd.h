#ifndef INC_I2C_LCD_H_
#define INC_I2C_LCD_H_

#include "stm32f4xx_hal.h"

void lcd_init (void);   // Initialisation de l'écran LCD
void lcd_send_cmd (char cmd);  // Envoyer une commande
void lcd_send_data (char data);  // Envoyer un caractère/donnée
void lcd_send_string (char *str);  // Envoyer une chaîne de caractères
void lcd_clear (void); // Effacer l'écran
void lcd_put_cur(int row, int col); // Positionner le curseur (ligne, colonne)

#endif /* INC_I2C_LCD_H_ */
