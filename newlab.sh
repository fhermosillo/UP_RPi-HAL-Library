#!/bin/bash

echo "Project name?"
read projname

if [ -d "$(pwd)/$projname" ]
then
    echo "Error: Project $projname already exists." 
    exit
else
    echo "Creating new HAL project at $(pwd)/$projname..."
    # Create project directory
    mkdir $(pwd)/$projname
    # copy the contents of hal folder
    cp -R $(pwd)/hal/. $(pwd)/$projname
    sudo chmod 777 $(pwd)/$projname
    sudo geany $(pwd)/$projname/main.c &
fi

