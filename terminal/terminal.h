/**
 * Dieser Code ist im Abschlussprojekt des Moduls "Einführung in Embedded Software" entstanden.
 * Hannah Lehnen, Timur Gönül und Gerrit Weiermann
 * Gruppe 18, Slot D
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

/**
 * Disables the raw mode of the terminal.
 * Back to normal.
 */
void disable_raw_mode();

/**
 * Enables the raw mode of the terminal.
 * For keyboard input usages (without confirmation with 'enter' all the time)
 */ 
void enable_raw_mode();
