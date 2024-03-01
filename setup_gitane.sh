#Lancer ce script pour config le mot clef gitane pour lancer le fichier GitaneConsole
#!/bin/bash

# Path to your program
PROGRAM_PATH="/home/nathan/Git-Ane/Utils/Console/build/GitAneConsole"

# Desired command name
COMMAND_NAME="gitane"
COMMAND_NAME2="gac"

# Directory where symbolic link will be created

echo "Enter your bin directory"
echo "You can get it by typing echo \$PATH in your terminal"
echo "If you are on ubuntu, then /usr/local/bin works"

read -p "Enter your bin directory: " BIN_DIRECTORY



# Check if the program path exists
if [ ! -f "$PROGRAM_PATH" ]; then
    echo "Error: Program not found at $PROGRAM_PATH"
    exit 1
fi

# Create symbolic link
if ln -s "$PROGRAM_PATH" "$BIN_DIRECTORY/$COMMAND_NAME"; then
    echo "Symbolic link created successfully"
else
    echo "Error: Failed to create symbolic link"
fi

if ln -s "$PROGRAM_PATH" "$BIN_DIRECTORY/$COMMAND_NAME2"; then
    echo "Symbolic link created successfully"
else
    echo "Error: Failed to create symbolic link"
    exit 1
fi
