# The path to the directory that holds the build shared libraries.
BUILT_LIB_DIR=../../build/src/swig/.libs

# The path to the built LibKML.jar.
LIBKML_JAR_PATH=../../build/src/swig/LibKML.jar

#The path to your junit jar file.
JUNIT_JAR_PATH=$JUNIT_HOME/junit-4.6.jar

echo "=== setting shared library path to built libraries dir: $BUILT_LIB_DIR"
export LD_LIBRARY_PATH=$BUILT_LIB_DIR  # For Linux.
export DYLD_LIBRARY_PATH=$BUILT_LIB_DIR  # For Mac OS X.

# Cleanup previous test runs.
rm -f *.class

echo "=== Running KmlBaseTest"
javac -classpath .:$LIBKML_JAR_PATH:$JUNIT_JAR_PATH KmlBaseTest.java 
java -classpath .:$LIBKML_JAR_PATH:$JUNIT_JAR_PATH org.junit.runner.JUnitCore KmlBaseTest
