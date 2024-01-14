<?php
// Connect to SQLite database
$db = new SQLite3('/database.db');

// Retrieve form data
$name = $_POST['Name'];
$description = $_POST['Description'];
$score = $_POST['Score'];
$addToWatchlist = $_POST['AddToWatchlist'];

// Insert data into MovieList table
$query = "INSERT INTO MovieList (Name, Description, Score, AddToWatchlist) VALUES ('$name', '$description', $score, $addToWatchlist)";
$db->exec($query);

// Close database connection
$db->close();
?>
