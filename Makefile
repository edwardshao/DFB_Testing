LIBS = -L/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/directfb/lib -ldirectfb -ldirect -lfusion -lpthread

INCS = -I/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/directfb/include/directfb

LFLAGS = -Wl,-O1 -Wl,-rpath-link -Wl,/home/laface/JewelryBox/sdk/cs_rootfs/cross_rootfs/lib -Wl,-rpath-link -Wl,/home/laface/JewelryBox/sdk/mrua/MRUA_src/lib -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/directfb/lib -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/dcchd -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/core -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/mono -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/dvdvr -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/brd -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/curacao -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/curacao/lib -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/dtv -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/dtv/network -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/dtv/capture -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/dtv/tuner -Wl,-rpath-link -Wl,/home/laface/edward_shao_laface-VirtualBox_8922/edward_shao_laface-VirtualBox_8922/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox/sdk/dcchd/dcchd/dtv/acap

CC = mipsel-linux-gcc

all:
	$(CC) -o pss pss.c $(INCS) $(LIBS) $(LFLAGS)
	$(CC) -o image image.c mytime.c $(INCS) $(LIBS) $(LFLAGS)
	$(CC) -o imagev2 imagev2.c mytime.c $(INCS) $(LIBS) $(LFLAGS)
	sudo cp image /var/www
	sudo cp pss /var/www
	sudo cp imagev2 /var/www
clean:
	rm -rf image
