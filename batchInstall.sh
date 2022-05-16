#!/bin/bash
if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under Mac OS X platform        
    SYSTEM="linux"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    # Do something under 64 bits Windows NT platform
    SYSTEM="windows"
else
exit
fi

echo " --- Remove Channels Installer $SYSTEM --- "
echo ''                                                                     
echo 'Remove Channels Build/Install Batch Helper'
echo ''
echo ''
echo ''
echo ''


echo Enter Path to Nuke Folders
read FOLDER

echo Enter Path to Install Nuke Plugins default: install
read INSTALL

if [ -z $INSTALL ]; then
    INSTALL=install
fi

BASEDIR="${PWD}"
if [ ! -d build ]; then
    mkdir build
fi

if [ ! -d release ]; then
    mkdir release
fi 

echo "$FOLDER/Nuke*"
for nukeFolder in "$FOLDER/Nuke"*; do
    if [ -d "$nukeFolder" ]; then
        VERSION=${nukeFolder[@]/"$FOLDER/Nuke"/""}

        mkdir build/$VERSION
        if [ $SYSTEM = "windows" ]; then
            cmake -G "Visual Studio 15 2017" -A x64 -S $BASEDIR -DCMAKE_INSTALL_PREFIX="$INSTALL/$VERSION" -DNuke_ROOT="$nukeFolder" -B "build/$VERSION"
        else
            cmake -S $BASEDIR -D CMAKE_INSTALL_PREFIX="$INSTALL/$VERSION" -D Nuke_ROOT="$nukeFolder" -B "build/$VERSION"
        fi
        
        cmake --build "build/$VERSION" --config Release
        cmake --install "build/$VERSION"
        echo '-------'
        echo '-------'

        # Create zip archivs
	
        if [ $SYSTEM = "windows" ]; then
		powershell Compress-Archive -LiteralPath "$BASEDIR/$INSTALL/$VERSION" -DestinationPath "./release/RemoveChannels-Windows-Nuke$VERSION.zip" -Force
	else
            zip -r ./release/RemoveChannels-Linux-Nuke$VERSION.zip $BASEDIR/$INSTALL/$VERSION
        fi       
    fi
done