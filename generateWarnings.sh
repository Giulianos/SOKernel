#!/bin/bash

./docker-build.sh >> /dev/null
result=$?
if [ $result -eq 0 ]
then
echo "Generating warnings..."
./docker-build.sh | grep warning > warnings.txt
./docker-build.sh | grep note > warnings.txt
elif
echo "DONE!"
