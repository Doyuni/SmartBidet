# SmartBidet with Raspberry Pi

### Command to run the program

1. **make**  
2. **sudo insmod button_dev.ko**  
3. **sudo mknod -m 666 /dev/button_dev c 240 0**  
4. **sudo insmod touch_dev.ko**  
5. **sudo mknod -m 666 /dev/touch_dev c 220 0**  
6. **ls -al/dev**  
 *check device files  
7. **echo "test" > /dev/button_dev**  
8. **dmesg**  
 *check device driver (_same way to check touch_dev_)  
9. **gcc -o app (all .c file) -lpthread -lwiringPi**  
 *compile  
10. **./app**  
 *execute  

