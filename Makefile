#The control makefile.
#Copyright 2017 CallingBaby, Inc. All rights reserved.
#Auther: Tianz

#The name of target you want.
TARGRT = image

#The path of stoll target file.
TARGETDIR = $(shell pwd)/target

#The file of stool temporary object files.
export OBJSDIR = $(shell pwd)/objs

#The global include files.
export GLOBALINC = \
	-I$(shell pwd)/controller/include \
	-I$(shell pwd)/sender/include \
	-I$(shell pwd)/receiver/include\
	
#The global libs.
export GLOBALLIBS = 

#The path of sub dir makefiles.
SUBMAKEFILES += controller/application
SUBMAKEFILES += controller/src
SUBMAKEFILES += sender/src
SUBMAKEFILES += receiver/src

#The compiler.
export GXX = g++

#The g++ compile flags.
export GXXFLAGS = -O2

#Start to compile the targrt.
$(TARGRT):$(OBJSDIR) $(TARGETDIR)

#To run all of sub dir makefile.
	$(foreach temMake,$(SUBMAKEFILES),$(MAKE) -C $(temMake);)

#Generate the targrt.
	$(GXX) -o $(TARGETDIR)/$@ $(OBJSDIR)/*.o $(GLOBALINC) $(GLOBALLIBS)

#Then remove the all of temporary object files.
	$(RM) -r $(OBJSDIR)
	@echo "\n\nCompile seccussfully!\n\n"

#Make the objs file.
$(OBJSDIR):
	mkdir -p $@

#Make the file to stoll target.
$(TARGETDIR):
	mkdir -p $@

.PHONY:clean
#The clean work.
clean:
	$(RM) -r $(TARGETDIR)

