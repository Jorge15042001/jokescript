CC:=g++
OUTPUT=build/JokeScript
SRC_DIR :=src
OBJ_DIR :=build/obj
SRC_FILES :=$(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES :=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS :=
CPPFLAGS :=
CXXFLAGS :=-std=c++20 
CXXDebugFlags := -Wall -Wextra -Wpedantic -Waddress -Waggregate-return -Waggressive-loop-optimizations -Wdeprecated -Weffc++ -Wformat  -Wignored-attributes -Wignored-qualifiers -Wimplicit -Wincompatible-pointer-types -Wliteral-suffix -Wnoexcept -Wreorder -Wreturn-type -Wunused

 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p build
	mkdir -p build/obj
	$(CC)  $(CXXFLAGS)    -c -o $@ $^ $(CXXDebugFlags)

$(OUTPUT): $(OBJ_FILES)
	$(CC)  $(CXXFLAGS)   -o $@ $^ $(CXXDebugFlags)
clean :
	rm -rf build

