/*
 * command.h
 *
 *  Created on: Nov 23, 2016
 *      Author: RAVINDRA
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#define CMDLINE_MAX_ARGS 3

extern int CMD_help (int argc, char **argv);
extern int CMD_on (int argc, char **argv);
extern int CMD_off (int argc, char **argv);
extern int CMD_anti (int argc, char **argv);
extern int CMD_clock (int argc, char **argv);
extern int CMD_half (int argc, char **argv);
extern int CMD_full (int argc, char **argv);
extern int CMD_speed (int argc, char **argv);
extern int CMD_angle (int argc, char **argv);
extern int CMD_90 (int argc, char **argv);
extern int CMD_control(int argc, char **argv);

#endif /* COMMAND_H_ */
