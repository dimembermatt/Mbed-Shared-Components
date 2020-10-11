BUILD_ROOT="BUILD/"
SRC_ROOT="../src/"
MBED_ROOT="../mbed-os/mbed.h"

# Delete everything in the BUILD directory.
rm BUILD/* -rf

# Auto generate test executables

# This find command checks for all files called test_*
# in the TESTS folder, recursively. For each file that's
# found, make a new directory for it in BUILD, if it doesn't
# exist, and then compile the executable there and run it.
find . -name "test_*.cpp" | while read file;
do
    echo "Generating test for $file.";

    # Extracting the directory and file name metadata.
    DIR=${file%/*}
    FILE=${file%.*}

    # Build the directory if it exists (-p flag).
    mkdir -p ${BUILD_ROOT}${DIR}

    # Make the executable and place it in the new directory.
    # We do a couple of things here:
    #   -Wall : Turns on a bunch of warnings.
    #   -Wextra : Even more warnings.
    #   -o : Specifies the name and location of our executable.
    #   -I : Specifies a directory to be added to be searched for header files.
    #   The last two arguments are the file to compile with the main, as well
    #   as all the files in the library added.
    g++ -Wall -Wextra -o ${BUILD_ROOT}${FILE} -I ${SRC_ROOT} ${file};
    # TODO: 'I {MBED_ROOT}'' figure out how to include mbed.h dependency
    # TODO: '${SRC_ROOT}${DIR}/*.cpp' figure  out how to include option for building libraries

    # While we're at it, let's execute it as well.
    if [ -f ${BUILD_ROOT}${FILE} ]
    then
        echo "Executing testbench:";
        # Executing the executable we generated.
        ./${BUILD_ROOT}${FILE};
    else
        echo "Didn't find an executable.";
    fi

    echo "$file test Generated.";
    echo "\n\n";
done