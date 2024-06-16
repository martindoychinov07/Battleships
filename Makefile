game: game.c
	gcc game.c -o game.exe

playGame: game.exe
	./game.exe

replay: replay.c
	gcc replay.c -o replay.exe

playReplay: replay.exe
	./replay.exe

clean:
	del *.exe