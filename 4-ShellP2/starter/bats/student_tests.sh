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

@test "Run external command: ls" {
    run ./dsh <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Handle empty input" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
}

@test "cd to a valid directory" {
    mkdir -p /tmp/test-dir
    run ./dsh <<EOF
cd /tmp/test-dir
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"/tmp/test-dir"* ]]
    rmdir /tmp/test-dir
}


@test "cd with no arguments does not change directory" {
    original_dir=$(pwd)
    run ./dsh <<EOF
cd
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"$original_dir"* ]]
}

@test "Execute a command with arguments" {
    run ./dsh <<EOF
echo Hello, World!
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello, World!"* ]]
}

@test "Quoted strings with spaces are preserved" {
    run ./dsh <<EOF
echo "This is a test"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"This is a test"* ]]
}



@test "Execute a command with special characters" {
    run ./dsh <<EOF
echo !@#$%^&*()
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"!@#$%^&*()"* ]]
}

@test "Exit command terminates shell" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

