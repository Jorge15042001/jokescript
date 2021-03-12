CC:=g++
OUTPUT=build/JokeScript
SRC_DIR :=src
OBJ_DIR :=build/obj
SRC_FILES :=$(wildcard $(SRC_DIR)/*.cpp)
HDRS_FILES :=$(wildcard $(SRC_DIR)/*.h)
OBJ_FILES :=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS :=
CPPFLAGS :=
CXXFLAGS :=-std=c++20 
CXXDebugFlags := -Wall -Wextra -Wpedantic -Waddress -Waggressive-loop-optimizations -Wdeprecated -Weffc++ -Wformat  -Wignored-attributes -Wignored-qualifiers -Wliteral-suffix -Wnoexcept -Wreorder -Wreturn-type -Wunused
OPTIMIZACION := -O3

 
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	mkdir -p build
	mkdir -p build/obj
	$(CC) $(OPTIMIZACION) $(CXXFLAGS)    -c -o $@ $^ $(CXXDebugFlags) -fdata-sections -ffunction-sections

$(OUTPUT): $(OBJ_FILES)
	$(CC) $(OPTIMIZACION) $(CXXFLAGS)   -o $@ $^ $(CXXDebugFlags) -Wl,--gc-sections

clean :
	rm -rf build

