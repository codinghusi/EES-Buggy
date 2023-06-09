#include "buggy_controller.h"
#include "../terminal/terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

void Buggy_Controller::keyboard_control()
{
    enableRawMode();
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            printf("%d\n", c);
        }
        else
        {
            printf("%d ('%c')\n", c, c);
        }
        if(prevent_forward && c != 's') c='b';
        switch (c)
        {
        case 'A':
        case 'w':
            motors.forwards();
            motors.drive();
            break;
        case 's':
            motors.backwards();
            motors.drive();
            break;
        case 'a':
            motors.forwards();
            motors.driveRelative(90);
            break;
        case 'd':
            motors.forwards();
            motors.driveRelative(-90);
            break;
        case 'q':
            motors.rotateRelative(-90);
            break;
        case 'e':
            motors.rotateRelative(90);
            break;
        case 'c':
            circumnavigate();
            break;
        case 'r':
            rectangle();
            break;
        case 'o':
            slalom();
            break;
        case 27:
        continue;
        case '[':
        continue;

        default:
            motors.brake();
            break;
        }
    }
    disableRawMode();
}
