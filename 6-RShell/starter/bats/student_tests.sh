#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}


# Test redirection: Output redirection to a file
@test "Redirection: echo test > output.txt" {
    run ./dsh <<EOF
    echo test > output.txt
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [ -f output.txt ]
    run cat output.txt
    [ "$output" = "test" ]
    rm output.txt
}

# Test exit command
@test "Built-in Command: exit terminates the shell" {
    run ./dsh <<EOF
    exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

# Test a simple echo command
@test "Basic Command: echo hello" {
    run ./dsh <<EOF
echo hello
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"hello"* ]]
}

# Test changing directories with cd
@test "Built-in Command: cd changes directory" {
    run ./dsh <<EOF
cd /
pwd
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == */* ]]
}

# Test input with spaces
@test "Input Handling: Command with extra spaces" {
    run ./dsh <<EOF
    echo    "spaces   are   handled"
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"spaces are handled"* ]]
}


# Test complex pipelines
@test "Complex Piping: ps aux | grep root | sort | tail -n 5" {
    run ./dsh <<EOF
ps aux | grep root | sort | tail -n 5
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" != "" ]] # Output should not be empty
}


# Piping and built-in commands together
@test "Piping with Built-in: echo $HOME | grep /" {
    run ./dsh <<EOF
echo $HOME | grep /
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"/"* ]]
}


 # Test for multiple pipes
@test "Piping: echo hello | tr a-z A-Z | rev" {
    run ./dsh <<EOF
echo hello | tr a-z A-Z | rev
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"OLLEH"* ]]
}

# Test piping with multiple redirections
@test "Piping with Redirection: echo test | grep t > output1.txt | cat output1.txt" {
    run ./dsh <<EOF
echo test | grep t > output1.txt
cat output1.txt
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"test"* ]]
    [ -f output1.txt ]
    rm output1.txt
}


 
# Test piping with input redirection
@test "Piping with Input Redirection: cat < input.txt | grep hello" {
    echo "hello world" > input.txt
    run ./dsh <<EOF
cat < input.txt | grep hello
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"hello world"* ]]
    rm input.txt
}

