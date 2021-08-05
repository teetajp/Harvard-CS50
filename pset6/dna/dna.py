from sys import argv
import csv


# Check if arguments are passed to the command line
if len(argv) != 3:
    print("Usage: python dna.py [csv in database] [txt of sequence]")
    exit(1)

# Open the csv file for reading and map to a dictionary
csvfile = open(argv[1], "r", newline='')
csvlist = list(csv.reader(csvfile))
strfields = {}
for row in range(len(csvlist)):
    if row > 0:
        break
    else:
        for col in range(len(csvlist[row])):
            if col > 0:
                strfields[csvlist[row][col]] = 0

# Open the sequence text file for reading
with open(argv[2], "r", newline='') as sequencefile:
    seqreader = csv.reader(sequencefile)
    
    # for each pos, compute how many times the STR repeats starting tat that pos
    # for each pos, keep checking successive substrings until the STR repeats no longer
    for row in seqreader:
        s = row[0]
        # The minimum STR length is 4
        for char in range(len(s) - 3):
            for genome in strfields:
                counter = 0  # Reset the counter
                # Count the max consecutive repeats of 'str' in s
                while s[char:char + len(genome)] == genome:
                    counter += 1
                    char += len(genome)
                # Update max number of STR repeats
                strfields[genome] = counter if counter > strfields[genome] else strfields[genome]

# Check if max STRs in provided sequence match any person's max STR provided in the csv
for row in range(1, len(csvlist)):
    matching = False
    gen_counter = 1
    for genome in strfields:
        if (strfields[genome] == int(csvlist[row][gen_counter])):
            matching = True
        else:
            matching = False
            break
        gen_counter += 1
    if matching == True:
        print(csvlist[row][0])
        csvfile.close()
        exit(0)
                    
csvfile.close()
print("No match")
exit(1)