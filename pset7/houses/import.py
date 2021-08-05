from cs50 import SQL
from sys import argv
import csv

# Check number of command line arguments
if len(argv) != 2:
    print("Usage: python import.py [dbName].csv")
    exit(1)

# Open students.db
db = SQL("sqlite:///students.db")

# Open the input csv file
with open(argv[1], "r") as csvfile:
    
    # Create the DictReader
    reader = csv.DictReader(csvfile)
    
    for row in reader:
        # Parse name for each row
        name = row['name'].split(' ')
        first_name = name[0]
        if len(name) == 2:
            middle_name = None
            last_name = name[1]
        else:
            middle_name = name[1]
            last_name = name[2]
        
        # Insert each student's information int othe students table of students.db
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                   first_name, middle_name, last_name, row['house'], row['birth'])