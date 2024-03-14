#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>

#define MYIOCTL_MAGIC 'k'
#define MYIOCTL_RESET _IO(MYIOCTL_MAGIC, 0)
#define MYIOCTL_GET_COUNT _IOR(MYIOCTL_MAGIC, 1, int)
#define MYIOCTL_INCREMENT _IOW(MYIOCTL_MAGIC, 2, int)

#include <signal.h>

int* __fd;

void termination_handler (int signum) {
    close(*__fd);
    printf("\nSignal recieved\n");
    exit(-1);
}

int main( ) {

    signal (SIGINT, termination_handler);
    signal (SIGHUP, termination_handler);
    signal (SIGTERM, termination_handler);

    int fd = open("/dev/myioctl", O_RDWR);
    if (fd == -1) {
        perror("Error opening myioctl device");
        return -1;
    }
    __fd = &fd;

    // Example: Get the current counter value
    int counter_value;
    ioctl(fd, MYIOCTL_GET_COUNT, &counter_value) ;
    printf("Current counter value: %d\n", counter_value);

    printf("------------\n");
    printf("Here are the operation optoins:\n");
    printf("1 for increase myioctl module\n");
    printf("2 for reset myioctl module\n");
    printf("3 for show myioctl counter number\n");
    printf("other for exit the app\n");
    printf("------------\n");

    for (;;){
        uint32_t usr_ipt = 0;

        printf("Enter operation you want to do [1,2,3]: ");

        if (scanf("%d", &usr_ipt) != 1) {
            perror("Error reading input");
            close(fd);
            return -1;
        }

        switch(usr_ipt){
            case 1:{
                int increment_value;
                printf("Enter the value to increment the counter: ");
                if (scanf("%d", &increment_value) != 1) {
                    perror("Error reading input");
                    close(fd);
                    return -1;
                }
                ioctl(fd, MYIOCTL_INCREMENT, &increment_value);

                ioctl(fd, MYIOCTL_GET_COUNT, &counter_value);
                printf("Updated counter value: %d\n" , counter_value);
                break;
            }
            case 2:{
                ioctl(fd, MYIOCTL_RESET, -1);
                
                ioctl(fd, MYIOCTL_GET_COUNT, &counter_value);
                printf("Updated counter value: %d\n" , counter_value);
                break;
            }
            case 3:{
                ioctl(fd, MYIOCTL_GET_COUNT, &counter_value);
                printf("Counter value: %d\n" , counter_value);
                break;
            }
            default:{
                printf("Exit the app\n");
                close(fd);
                return 0;
            }
        }
        printf("------------\n");
    }
    return 0;
}
