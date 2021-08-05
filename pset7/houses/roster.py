from cs50 import SQL
from sys import argv

# Check number of command line arguments
if len(argv) != 2:
    print("Usage: python roster.py [House Name]")
    exit(1)

# Open students.db
db = SQL("sqlite:///students.db")

students = db.execute(f"SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])

for student in students:
    # Print first and last name and birth year of student who does not have a middle name
    if student['middle'] == None:
        print(f"{student['first']} {student['last']}, born {student['birth']}")
    # Print first, middle, last name and birth year of student who has a middle name
    else:
        print(f"{student['first']} {student['middle']} {student['last']}, born {student['birth']}")