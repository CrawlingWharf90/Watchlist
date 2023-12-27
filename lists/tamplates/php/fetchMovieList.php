<?php
// Connect to SQLite database
$db = new SQLite3('/database.db');

// Fetch data from MovieList table
$query = "SELECT * FROM MovieList";
$result = $db->query($query);

// Display the data
while ($row = $result->fetchArray()) {
    echo "Name: " . $row['Name'] . "<br>";
    echo "Description: " . $row['Description'] . "<br>";
    echo "Score: " . $row['Score'] . "<br>";
    echo "Add To Watchlist: " . $row['AddToWatchlist'] . "<br><br>";
}

// Close database connection
$db->close();
?>
