#!/sbin/sh
for i in `seq 1 1000`
do
	echo $i
	./client "192.168.5.102" &
done
