#The sub dir makefile.
#Auther: Tianz
#Copyright: ElegantRobot 22 Aug 2017

#The source files.
SRC = $(wildcard *.cpp)

#The objects files.
OBJS = $(SRC:%.cpp=%.o)

#Compile the all object files.
all:$(OBJS)

$(OBJS):%.o:%.cpp
	$(GXX) -o $(OBJSDIR)/$@ -c $< $(GLOBALINC) $(GLOBALLIBS) $(GXXFLAGS)