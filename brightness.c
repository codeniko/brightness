/* Brightness.c
Modify the brightness on laptop running a linux distro. You need root privileges to run as it read/writes to /sys/class/backlight/intel_backlight/brightness. 
To avoid the password prompt for something as trivial as this, the setuid permission bit wouldn't hurt.

chown root:user and chmod 4750 suggested
---- or if you have multiple users
chown root:staff and chmod 4755
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define BUFSIZE 5
#define BRIGHTNESS_HIGH 4882
#define BRIGHTNESS_LOW 500
#define BRIGHTNESS_INTERVAL 500
static const char brightness_path[] = "/sys/class/backlight/intel_backlight/brightness";
static char buf[BUFSIZE];

static void printError(const char msg[]) {
	const int bPathLen = strlen(brightness_path);
	char errstr[150];
	memcpy(errstr, brightness_path, bPathLen);
	memcpy(errstr+bPathLen, msg, strlen(msg) + 1);
	perror(errstr);
}

static int readBrightness() {
	FILE *file = 0;
	memset(buf, 0, BUFSIZE);

	file = fopen(brightness_path, "r");
	if (!file) {
		printError("Error opening file to read.");
		exit(1);
	}
	fgets(buf, BUFSIZE, file);
	fclose(file);

	errno = 0;
	char *endptr = 0;
	long val = strtol(buf, &endptr, 10);
	/* Check for various possible errors */
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0) || (*endptr != '\0' && *endptr != '\n') ) {
		printError(" file content is not valid.");
		exit(1);
	}

	return (int)val;
}

static int convertBrightness(int bval) {
	int brightness = 0;
	/* -1 and 0 values for sight-damaging dim... */
	switch(bval) {
		case -1: brightness = 80;
					break;
		case 0: brightness = 120;
				  break;
		case 10: brightness = BRIGHTNESS_HIGH;
					break;
		default: brightness = bval * 500;
	}

	if (brightness > BRIGHTNESS_HIGH || brightness < 80) {
		fprintf(stderr, "Error: Invalid brightness value.\n");
		exit(1);
	}

	return brightness;
}

static void setBrightness(int brightness) {
	FILE *file = 0;
	memset(buf, 0, BUFSIZE);

	file = fopen(brightness_path, "w");
	if (!file) {
		printError("Error opening file for write.");
		exit(1);
	}
	fprintf(file, "%d", brightness);
	fclose(file);
}

int main(int argc, char **argv) {
	errno = 0;
	char *endptr = 0;

	if (argc == 1) { /*arg not given, set brightness to opposite extreme or highest*/
		int brightness = readBrightness();
		if (brightness == BRIGHTNESS_HIGH) {
			setBrightness(BRIGHTNESS_LOW);
		} else {
			setBrightness(BRIGHTNESS_HIGH);
		}
	} else {
		/*argument given, set brightness to value or decrement/increment*/
		if (strcmp(argv[1], "+") == 0) {
			int brightness = readBrightness() + BRIGHTNESS_INTERVAL;
			if (brightness < BRIGHTNESS_HIGH) {
				setBrightness(brightness);
			} else {
				setBrightness(BRIGHTNESS_HIGH);
			}
		} else if (strcmp(argv[1], "-") == 0) {
			int brightness = readBrightness() - BRIGHTNESS_INTERVAL;
			if (brightness > BRIGHTNESS_LOW) {
				setBrightness(brightness);
			} else {
				setBrightness(BRIGHTNESS_LOW);
			}
		} else {
			int bval = (int) strtol(argv[1], &endptr, 10);
			if (*endptr != '\0') {
				printError("Error: Invalid brightness value.\n");
				return 1;
			}
			setBrightness(convertBrightness(bval));
		}
	}
	return 0;
}
