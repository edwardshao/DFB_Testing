SRC_HOME=/home/laface/source/Volume/JewelryBox/Dev/JewelryBox-2.0/JewelryBox

LIBS = -L${DIRECTFB_DIR}/lib -ldirectfb -ldirect -lfusion -lpthread
INCS = -I${DIRECTFB_DIR}/include/directfb

LFLAGS = -Wl,-O1 -Wl,-rpath-link -Wl,${SRC_HOME}/sdk/cs_rootfs/cross_rootfs/lib -Wl,-rpath-link -Wl,${RUA_DIR}/lib -Wl,-rpath-link -Wl,${DIRECTFB_DIR}/lib \
         -Wl,-rpath-link -Wl,${DCCHD_DIR}/dcchd -Wl,-rpath-link -Wl,${DCCHD_DIR}/core -Wl,-rpath-link -Wl,${DCCHD_DIR}/mono -Wl,-rpath-link -Wl,${DCCHD_DIR}/dvdvr \
         -Wl,-rpath-link -Wl,${DCCHD_DIR}/brd -Wl,-rpath-link -Wl,${DCCHD_DIR}/curacao -Wl,-rpath-link -Wl,${DCCHD_DIR}/curacao/lib -Wl,-rpath-link \
         -Wl,${DCCHD_DIR}/dtv -Wl,-rpath-link -Wl,${DCCHD_DIR}/dtv/network -Wl,-rpath-link -Wl,${DCCHD_DIR}/dtv/capture -Wl,-rpath-link -Wl,${DCCHD_DIR}/dtv/tuner \
         -Wl,-rpath-link -Wl,${DCCHD_DIR}/dtv/acap

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
