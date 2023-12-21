CREATE TABLE MovieList (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name text not null unique,
    Description text default '',
    Score REAL default NULL
);
CREATE TABLE GameList (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name text not null unique,
    Description text default '',
    Score REAL default NULL
);
CREATE TABLE MusicList (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name text not null unique,
    Description text default '',
    Score REAL default NULL
);

-- SELECT ALL NON RATED THINGS
SELECT * FROM MovieList WHERE Score IS NULL;
