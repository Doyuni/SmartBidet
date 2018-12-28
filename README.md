# SmartBidet with Raspberry Pi

### Command to run the program
~~~
make
sudo insmod button_dev.ko
sudo mknod -m 666 /dev/button_dev c 240 0
sudo insmod touch_dev.ko
sudo mknod -m 666 /dev/touch_dev c 220 0
ls -al/dev
->check device files
echo "test" > /dev/button_dev
dmesg
->check device driver ( same way to check touch_dev)
gcc -o app (all .c file) -lpthread -lwiringPi
->comile
./app
->execute
~~~
