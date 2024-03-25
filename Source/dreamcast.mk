#
# lugarudc - Makefile for KallistiOS
# Charlotte Koch <dressupgeekout@gmail.com>
#

export KOS_WRAPPERS_VERBOSE=	1

#KOS_ROMDISK_DIR = romdisk

SRCS+=	Animation/Animation.cpp
SRCS+=	Animation/Joint.cpp
SRCS+=	Animation/Muscle.cpp
SRCS+=	Animation/Skeleton.cpp
SRCS+=	Audio/Sounds.cpp
SRCS+=	Audio/openal_wrapper.cpp
SRCS+=	Environment/Lights.cpp
SRCS+=	Environment/Skybox.cpp
SRCS+=	Environment/Terrain.cpp
#SRCS+=	Game.cpp
SRCS+=	GameDraw.cpp
SRCS+=	GameInitDispose.cpp
#SRCS+=	GameTick.cpp
SRCS+=	Globals.cpp
SRCS+=	Graphic/Decal.cpp
SRCS+=	Graphic/Models.cpp
SRCS+=	Graphic/Sprite.cpp
SRCS+=	Graphic/Stereo.cpp
SRCS+=	Graphic/Text.cpp
SRCS+=	Graphic/Texture.cpp
SRCS+=	Level/Awards.cpp
SRCS+=	Level/Campaign.cpp
SRCS+=	Level/Dialog.cpp
SRCS+=	Level/Hotspot.cpp
SRCS+=	Math/Frustum.cpp
SRCS+=	Math/XYZ.cpp
#SRCS+=	Menu/Menu.cpp
SRCS+=	Objects/Object.cpp
SRCS+=	Objects/Person.cpp
SRCS+=	Objects/PersonType.cpp
SRCS+=	Objects/Weapons.cpp
SRCS+=	Platform/PlatformUnix.cpp
SRCS+=	Thirdparty/jsoncpp.cpp
SRCS+=	Tutorial.cpp
SRCS+=	User/Account.cpp
#SRCS+=	User/Settings.cpp
SRCS+=	Utils/Folders.cpp
SRCS+=	Utils/ImageIO.cpp
#SRCS+=	Utils/Input.cpp
SRCS+=	Utils/pack.c
SRCS+=	Utils/private.c
SRCS+=	Utils/unpack.c
SRCS+=	dreamcast_main.cpp

# Just in case:
#SRCS+=	romdisk.o

OBJS+=	Animation/Animation.o
OBJS+=	Animation/Joint.o
OBJS+=	Animation/Muscle.o
OBJS+=	Animation/Skeleton.o
OBJS+=	Audio/Sounds.o
OBJS+=	Audio/openal_wrapper.o
OBJS+=	Environment/Lights.o
OBJS+=	Environment/Skybox.o
OBJS+=	Environment/Terrain.o
#OBJS+=	Game.o
OBJS+=	GameDraw.o
OBJS+=	GameInitDispose.o
#OBJS+=	GameTick.o
OBJS+=	Globals.o
OBJS+=	Graphic/Decal.o
OBJS+=	Graphic/Models.o
OBJS+=	Graphic/Sprite.o
OBJS+=	Graphic/Stereo.o
OBJS+=	Graphic/Text.o
OBJS+=	Graphic/Texture.o
OBJS+=	Level/Awards.o
OBJS+=	Level/Campaign.o
OBJS+=	Level/Dialog.o
OBJS+=	Level/Hotspot.o
OBJS+=	Math/Frustum.o
OBJS+=	Math/XYZ.o
#OBJS+=	Menu/Menu.o
OBJS+=	Objects/Object.o
OBJS+=	Objects/Person.o
OBJS+=	Objects/PersonType.o
OBJS+=	Objects/Weapons.o
OBJS+=	Platform/PlatformUnix.o
OBJS+=	Thirdparty/jsoncpp.o
OBJS+=	Tutorial.o
OBJS+=	User/Account.o
#OBJS+=	User/Settings.o
OBJS+=	Utils/Folders.o
OBJS+=	Utils/ImageIO.o
#OBJS+=	Utils/Input.o
OBJS+=	Utils/pack.o
OBJS+=	Utils/private.o
OBJS+=	Utils/unpack.o
OBJS+=	dreamcast_main.o

KOS_CFLAGS+=	-Wno-unused-variable -Wno-parentheses
KOS_CFLAGS+=	-DPLATFORM_LINUX=1 -DPLATFORM_UNIX=1 -DDREAMCAST

KOS_CFLAGS+=	-I. -I./Thirdparty
KOS_CFLAGS+=	-I$(KOS_PORTS)/include
KOS_CFLAGS+=	-I$(KOS_PORTS)/include/jpeg
KOS_CFLAGS+=	-I$(KOS_PORTS)/include/png
KOS_CFLAGS+=	-I$(KOS_PORTS)/include/SDL

LDLIBS+=	-lAL
LDLIBS+=	-lGL
LDLIBS+=	-lstb_image
LDLIBS+=	-lkosutils
LDLIBS+=	-lm

TARGET= lugaru.elf

.PHONY: all
all: $(TARGET)

.PHONY: cdi
cdi: lugaru.cdi

lugaru.cdi: $(TARGET)

MKDCDISC?=	mkdcdisc

lugaru.cdi: $(PROG)
	$(MKDCDISC) -v 2 --no-padding --name LUGARU -d ../Data -e $(TARGET) -o $@

include $(KOS_BASE)/Makefile.rules

.PHONY: clean
clean: rm-elf
	-rm -rf $(OBJS) romdisk.*

$(TARGET): $(OBJS)
	kos-c++ -o $(TARGET) $(OBJS) $(LDLIBS)

.PHONY: dist
dist: $(TARGET)
	-rm -f $(OBJS)
	$(KOS_STRIP) $(TARGET)

.PHONY: rm-elf
rm-elf:
	-rm -f $(TARGET)
