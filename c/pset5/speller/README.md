# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

>"pneumonoultramicroscopicsilicovolcanoconiosis" is a form of pneumoconiosis caused
>by the inhalation of a fine silica dust found in most valcanoes.

## According to its man page, what does `getrusage` do?

>`getrusage` returns the resource usage of the calling process, children of the calling process, or
>the calling thread.

## Per that same man page, how many members are in a variable of type `struct rusage`?

>an `rusage` structure has 16 members.

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

>passing the two structures by reference, as opposed to by value, saves memory due to not having
>to create another structure of the same size with the same values. In some cases, passing by
>reference may also increase the speed of the program for the same reason as saving memory.
>When calculating benchmarks, saving CPU cycles while calculating speed is highly desired to keep
>results from being swayed.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

>`main()` first initializes the variable 'c' by using `fgetc()` to get the first character of the file
>as an integer. `main()` repeats as long as the next character is not EOF and increments to the next
>character in the file after every iteration. Next, `main()` checks the character to see if it is a
>letter or a single apostrophe that does not start a word (such as in contractions). If the current
>character passes both of the previous checks, the character is loaded into the `word[]` array and the
>index of the array is incremented. After the index is incremented, it is compared to the length of the
>longest word in our dictionary to see if we are capable of checking the word. If the length exceeds
>the longest word in our dictionary, the rest of the characters in the word are iterated over and the
>index is reset to zero (effectively skipping the word). If the current character is a number instead
>of a letter, the same process is used to skip the rest of the word. If the character is not a letter
>or number and the index is greater than zero (meaning that at least one letter was loaded), it is
>assumed to be a space. `main()` will then terminate the string with '\0' and increment the `words`
>counter. The system resource usage is then recorded for before passing the word to the `check()`
>function, which will return a boolean value indicating if the word is misspelled. The system resource
>usage is then checked again and the benchmark time is updated by passing the before and after usage
>to `calculate()`. `main()` will then check the result of the `check()` function. If the 'misspelled'
>variable is 'true', the word will be printed to `stdout` and the `misspellings` counter will be
>incremented. The index is then reset to zero to begin the process of loading the next word.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

>`fgetc()` is used when loading words, due to the input being non-standardized. `fgetc()` allows the
>program to check each character as it is loaded, as opposed to using `fscanf()` to load the entire
>word, then going back to make sure the word is the correct length and does not contain any illegal
>characters. Checking each character as it is loaded also reduces memory usage and CPU cycles, as a
>word that does not meet the requirements can be discarded as soon as the requirement breaking
>condition is met. Another performance benefit results from not having to iterate over an entire word
>to check the requirements, which would be a time-inefficient process.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

>constant pointers (such as in bool check(const char *word)) declared in this way mean that the
>variable is a pointer to constant data. This type of pointer allows you to change the pointer;
>however, the data that is being pointed to cannot be changed. In this instance, the constant
>pointers allow you to increment through the word, but you are not able to change the value at
>each address, preventing the word from being changed as it is checked. This same logic applies
>to loading the dictionary, preventing the dictionary words from being modified as they are loaded.
