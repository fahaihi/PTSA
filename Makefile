ifeq ($(OS), Windows_NT)
	RM = @del /q /f 2>nul
	TARGET = main.exe
	TMP = /
	SEP = $(subst /,\,$(TMP))
	SCRIPT = dir
else
	RM = @rm -f 2>/dev/null
	TARGET = main
	SEP = /
	SCRIPT = chmod +x
endif

.PHONY: clean run

CPPFLAGS = -fopenmp
VPATH = editor shouji ptsa
OBJS = main.o editor.o shouji.o ptsa.o
OBJSDIR = build

$(TARGET): $(OBJSDIR) $(OBJS)
	$(CXX) $(wildcard $(OBJSDIR)/*.o) -o $@ $(CPPFLAGS)
	$(SCRIPT) $(TARGET)

$(OBJS): %.o:%.cpp
	$(CXX) -c $< -o $(OBJSDIR)/$@ $(CPPFLAGS)

$(OBJSDIR):
	-mkdir $@

clean:
	$(RM) $(OBJSDIR)$(SEP)*.o $(TARGET)
