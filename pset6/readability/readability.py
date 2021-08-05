from cs50 import get_string


# Counts the number of letters, words, sentences,
# and the Coleman-Liau index from a given string
def main():
    text = get_string("Text: ")
    # Call functions to count letters, words, and sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)
    # Prints letter, word, and sentence counts respectively
    print(f"{letters} letter(s)")
    print(f"{words} word(s)")
    print(f"{sentences} sentence(s)")
    # Calculate the Coleman-Liau index
    index = round(0.0588 * (float(letters) / words * 100)
                  - 0.296 * (float(sentences) / words * 100) - 15.8)
    # Print the Coleman-Liau index based on index number
    if index < 1:
        print("Before Grade 1")
    elif index >= 15:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


# Counts the number of letters in a given string    
def count_letters(s):
    letters = 0
    for c in range(len(s)):
        if s[c].isalpha():
            letters += 1
    return letters


# Counts the number of words in a given string            
def count_words(s):
    words = 0
    for c in range(len(s)):
        if c == len(s) - 1:
            if s[c].isprintable():
                words += 1
        elif s[c].isprintable() and (s[c + 1].isspace()):
            words += 1
    return words
       
         
# Counts the number of sentences in a given string
def count_sentences(s):
    sentences = 0
    for c in range(len(s)):
        if s[c] == '!' or s[c] == '.' or s[c] == '?':
            sentences += 1
    return sentences
           
            
main()