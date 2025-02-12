1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice because it reads a line of input from the user until either a newline character or the end of the file (EOF) is reached. Since a shell environment expects one command or string of commands at a time, it works very well.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  We use malloc() to dynamically allocate memory for cmd_buff because the amount of input from the user can vary, and the program's requirements may change in the future.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming leading and trailing spaces is necessary to avoid the program failing due to not recognizing the commands. If the parsed command has a different character like a trailing space, it won't be parsed as the correct command.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  
        1- Standard Output Redirection (>): Redirects the output of a command to a file. Challenge: We must ensure the file is created or overwritten, and handle permissions properly. 
        2- Standard Input Redirection (<): Redirects the input for a command from a file. Challenge: Handling the EOF and ensuring that the file is read properly.
        3-Standard Error Redirection (2>): Redirects error output to a file. Challenge: This requires distinguishing between STDOUT and STDERR and redirecting them accordingly.


- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection changes the source or destination of input/output, while piping connects the output of one command to the input of another. Redirection deals with files, while piping transfers the data between the commands.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Keeping STDERR and STDOUT separate allows for easier error handling and debugging. If both were merged, it would be difficult to distinguish between regular output and error output.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  The shell should capture both STDOUT and STDERR, while ideally keeping them separate for the afformentioned reasons. 