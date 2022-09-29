#!/bin/sh
# Select DAC CH1
ssh root@192.168.1.22 /mmEye/i2c_reg 3c 14 1
# HV ON (currently using the digital out CH5)
ssh root@192.168.1.22 /mmEye/i2c_reg 3c e 10
i=0
while [ $i -lt 256 ]; do
    hex1=`printf "%x" $i`
    # Set the higher bits
    ssh root@192.168.1.22 /mmEye/i2c_reg 3c 15 $hex1
    j=0
    while [ $j -lt 256 ]; do
	hex2=`printf "%x" $j`
	echo "$hex1 $hex2" 
	# Set the lower bits
#	ssh root@192.168.1.22 /mmEye/i2c_reg 3c 16 $hex2
#	./dmm1 $hex1 $hex2 >> testdmm.txt
	j=`expr $j + 16`
    done
    i=`expr $i + 1`
done

#HV OFF
ssh root@192.168.1.22 /mmEye/i2c_reg 3c e 10

# DAC off
ssh root@192.168.1.22 /mmEye/i2c_reg 3c 15 00
ssh root@192.168.1.22 /mmEye/i2c_reg 3c 16 00
ssh root@192.168.1.22 /mmEye/i2c_reg 3c 14 0
