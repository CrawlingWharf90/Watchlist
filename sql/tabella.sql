CREATE TABLE MovieList (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name text not null unique,
    Description text default '',
    Score REAL default NULL,
    NotWatched INTEGER default 0
);
CREATE TABLE GameList (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name text not null unique,
    Description text default '',
    Score REAL default NULL, 
    NotPlayed INTEGER default 0
);
CREATE TABLE MusicList (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name text not null,
    Artist text not null, 
    Album text default '', 
    Description text default '',
    Score REAL default NULL, 
    NotListened INTEGER default 0
);

-- SELECT ALL NON RATED THINGS
SELECT * FROM MovieList WHERE Score IS NULL;
