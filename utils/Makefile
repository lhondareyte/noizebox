PRG=AnalyzeMIDI
SRCS=readMIDI.c midi.c
OBJS=readMIDI.o midi.o

.c.o:
	@printf "Compiling $< ..." 
	@$(CC) -c  $< -o $@
	@echo "done."

$(PRG): $(OBJS)
	@printf "Linking..."
	@$(CC) $(OBJS) -o $(PRG)
	@echo "done."
clean:
	@printf "Cleaning..."
	@rm -f *.o $(PRG) *~
	@echo "done."
