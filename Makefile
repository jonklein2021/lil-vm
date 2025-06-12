all:
	@echo Compiling...
	@gcc src/lc3.c -I src/include -o lc3.exe
	@echo Done!
clean:
	@echo Cleaning...
	@rm -f *.exe
	@echo Done!
