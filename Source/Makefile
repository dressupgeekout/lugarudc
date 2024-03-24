#
# Building Lugaru without CMake
# Charlotte Koch <dressupgeekout@gmail.com>
#

KOS?=	0

ifeq ($(KOS),1)
CC=	kos-cc
CXX=	kos-c++
endif

SRCS+=	Animation/Animation.cpp
SRCS+=	Animation/Joint.cpp
SRCS+=	Animation/Muscle.cpp
SRCS+=	Animation/Skeleton.cpp
SRCS+=	Audio/Sounds.cpp
SRCS+=	Audio/openal_wrapper.cpp
SRCS+=	Devtools/ConsoleCmds.cpp
SRCS+=	Environment/Lights.cpp
SRCS+=	Environment/Skybox.cpp
SRCS+=	Environment/Terrain.cpp
SRCS+=	Game.cpp
SRCS+=	GameDraw.cpp
SRCS+=	GameInitDispose.cpp
SRCS+=	GameTick.cpp
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
SRCS+=	Menu/Menu.cpp
SRCS+=	Objects/Object.cpp
SRCS+=	Objects/Person.cpp
SRCS+=	Objects/PersonType.cpp
SRCS+=	Objects/Weapons.cpp
SRCS+=	Platform/PlatformUnix.cpp
SRCS+=	Thirdparty/jsoncpp.cpp
SRCS+=	Tutorial.cpp
SRCS+=	User/Account.cpp
SRCS+=	User/Settings.cpp
SRCS+=	Utils/Folders.cpp
SRCS+=	Utils/ImageIO.cpp
SRCS+=	Utils/Input.cpp
SRCS+=	main.cpp

C_SRCS+=	Utils/pack.c
C_SRCS+=	Utils/private.c
C_SRCS+=	Utils/unpack.c

CFLAGS+=	-DPLATFORM_LINUX=1 -DPLATFORM_UNIX=1
CFLAGS+=	-I. -I./Thirdparty

ifeq ($(KOS),1)
# OK
CFLAGS+=	-DDREAMCAST
else
CFLAGS+=	-I/usr/pkg/include
CFLAGS+=	$(shell sdl2-config --cflags)
endif

ifeq ($(KOS),1)
# OK
else
LDFLAGS+=	-L/usr/pkg/lib -L/usr/X11R7/lib
LDFLAGS+=	-lGL -lpng16 -ljpeg -lopenal -lvorbisfile
LDFLAGS+=	$(shell sdl2-config --libs)
endif

.PHONY: all
all: lugaru

lugaru: $(patsubst %.c,%.o,$(C_SRCS)) $(patsubst %.cpp,%.o,$(SRCS))
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $(CFLAGS) $<

%.o: %.c
	$(CC) -o $@ -c $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f *.o */*.o lugaru
