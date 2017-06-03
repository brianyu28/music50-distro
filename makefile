scale: scale.c midi.c midi.h
	clang -o scale scale.c midi.c

music: music.c midi.c midi.h
	clang -o music music.c midi.c -lcs50

clean: 
	rm -f music scale *.mid
