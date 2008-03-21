# javac -classpath ../../build/swig CreatePlacemark.java
# TODO: -classpath doesn't work
# TODO: copy CreatePlacemark.java to ../../build/swig and this works fine
javac CreatePlacemark.java
java CreatePlacemark

javac WalkFeatures.java
java WalkFeatures
