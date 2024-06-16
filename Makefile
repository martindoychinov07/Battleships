main.exe: main.c game.c replay.c encryption.c
	gcc main.c game.c replay.c encryption.c -o main.exe

startMain: main.exe
	./main.exe

clean:
	del *.exe