game: multiplayer.c
	gcc multiplayer.c -o game.exe

play: game.exe
	./game.exe

clean:
	del *.exe