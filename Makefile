CC = gcc

img.png : plot.py draw_blue.txt draw_orange.txt draw_white.txt
	python3 plot.py

draw_white.txt : generate_drawdata white.raw
	./generate_drawdata white.raw $@

draw_orange.txt : generate_drawdata orange.raw
	./generate_drawdata orange.raw $@

draw_blue.txt : generate_drawdata blue.raw
	./generate_drawdata blue.raw $@

generate_drawdata : generate_drawdata.c
	$(CC) $^ -o $@ -lm

white.raw : white_filter dk_trim_double.raw
	./white_filter dk_trim_double.raw $@

orange.raw : orange_filter dk_trim_double.raw
	./orange_filter dk_trim_double.raw $@

blue.raw : blue_filter dk_trim_double.raw
	./blue_filter dk_trim_double.raw $@

white_filter : white_filter.c
	$(CC) iir.c butterworth.c $^ -o $@ -lm

orange_filter : orange_filter.c
	$(CC) iir.c butterworth.c $^ -o $@ -lm

blue_filter : blue_filter.c
	$(CC) iir.c butterworth.c $^ -o $@ -lm

dk_trim_double.raw : dk_trim.raw short_to_double
	./short_to_double dk_trim.raw dk_trim_double.raw

short_to_double : short_to_double.c
	$(CC) $^ -o $@

dk_trim.raw : dk_trim.wav
	sox $^ $@

dk_trim.wav : dk.wav
	sox $^ $@ trim 249 10

clean :
	$(RM) img.png
	$(RM) draw_blue.txt draw_orange.txt draw_white.txt
	$(RM) generate_drawdata
	$(RM) blue.raw orange.raw white.raw
	$(RM) blue_filter orange_filter white_filter
	$(RM) dk_trim_double.raw
	$(RM) short_to_double
	$(RM) dk_trim.raw
	$(RM) dk_trim.wav
