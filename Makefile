IN=watchlist.c
OUT=watchlist.exe
CC=gcc

all: $(OUT)

$(OUT): $(IN)
	$(CC) $(IN) -o $(OUT)

clean:
	rm -f $(OUT)

run: all
	./$(OUT)