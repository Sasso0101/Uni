#!/bin/bash
# Asks user for directory path and print its size
read -p "Directory path: " DIR
du -sh $DIR
