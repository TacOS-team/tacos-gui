#!/bin/bash
tacos_gui_appdir=emu/applications
tacos_appdir=../applications/gui

tacos_gui_libdir=emu/libs
tacos_libdir=../libs

applications="pron guacamole pron_cube.cpp pron_cube_mouse.cpp pron_cube_tuning.cpp jpegviewer.cpp"
libs="pronlib libcolor"

if [ -f $tacos_gui_appdir/Makefile.tacos ]; then
  echo "Configuring Makefile for GUI applications..."
  cp $tacos_gui_appdir/Makefile.tacos $tacos_appdir/Makefile
else
  echo "No TacOS Makefile for GUI applications found."
fi

for app in $applications; do
  if [ -d $tacos_gui_appdir/$app ]; then
    if [ -d $tacos_appdir/$app ]; then
      echo "Deleting $tacos_appdir/$app..."
      rm -rf $tacos_appdir/$app
    fi

    echo "Copying $tacos_gui_appdir/$app to $tacos_appdir/$app..."
    cp -r $tacos_gui_appdir/$app $tacos_appdir/$app

    if [ -f $tacos_appdir/$app/Makefile.tacos ]; then
      echo "Configuring Makefile..."
      mv $tacos_appdir/$app/Makefile.tacos $tacos_appdir/$app/Makefile
    else
      echo "No TacOS Makefile found for $app."
    fi

    if [ $app = "pron" ]; then
      cp -r $tacos_gui_appdir/../fonts $tacos_appdir/$app/
    fi
  elif [ -f $tacos_gui_appdir/$app ]; then
    echo "Copying $tacos_gui_appdir/$app to $tacos_appdir/$app..."
    cp $tacos_gui_appdir/$app $tacos_appdir/$app

    if [ $app = "jpegviewer" ]; then
      cp $tacos_gui_appdir/../tacos.jpg $tacos_appdir/
    fi
  else
    echo "Application $app does not exist."
  fi
done

for lib in $libs; do
  if [ -d $tacos_gui_libdir/$lib ]; then
    if [ -d $tacos_libdir/$lib ]; then
      echo "Deleting $tacos_libdir/$lib..."
      rm -rf $tacos_libdir/$lib
    fi

    echo "Copying $tacos_gui_libdir/$lib to $tacos_libdir/$lib..."
    cp -r $tacos_gui_libdir/$lib $tacos_libdir/$lib

    if [ -f $tacos_libdir/$lib/Makefile.tacos ]; then
      echo "Configuring Makefile..."
      mv $tacos_libdir/$lib/Makefile.tacos $tacos_libdir/$lib/Makefile
    else
      echo "No TacOS Makefile found."
    fi

    echo '*' > $tacos_libdir/$lib/.gitignore
  else
    echo "Library $lib does not exist."
  fi
done
