# The relative path to the directory that holds the build shared libraries.
BUILT_LIB_DIR=../../build/src/swig/.libs
echo "=== setting shared library path to built libraries dir: $BUILT_LIB_DIR"
export LD_LIBRARY_PATH=$BUILT_LIB_DIR  # For Linux.
export DYLD_LIBRARY_PATH=$BUILT_LIB_DIR  # For Mac OS X.

rm -f *.class

echo "... compiling CreateFolder"
javac -classpath .:../../build/src/swig CreateFolder.java
echo "... running CreateFolder"
java -classpath .:../../build/src/swig CreateFolder

echo "... compiling CreatePlacemark"
javac -classpath .:../../build/src/swig CreatePlacemark.java
echo "... running CreatePlacemark"
java -classpath .:../../build/src/swig CreatePlacemark

echo "... compiling ParseKml"
javac -classpath .:../../build/src/swig ParseKml.java
echo "... running ParseKml"
java -classpath .:../../build/src/swig ParseKml

echo "... compiling WalkFeatures"
javac -classpath .:../../build/src/swig WalkFeatures.java
echo "... running WalkFeatures"
java -classpath .:../../build/src/swig WalkFeatures
