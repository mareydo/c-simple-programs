#variables
CXX = g++
LD = g++
CXXFLAGS = -Wall -pedantic
LIBS= 
PROGRAM=marecja9
DOXYFILE=Doxyfile

BUILD_DIR =bin
SRC_DIR =src

OBJ= $(BUILD_DIR)/main.o $(BUILD_DIR)/CommandFactory.o $(BUILD_DIR)/DenseMatrix.o $(BUILD_DIR)/Matrix.o $(BUILD_DIR)/MatrixCalculator.o $(BUILD_DIR)/MatrixFactory.o $(BUILD_DIR)/SparseMatrix.o

#all, "" = vytvori vsetky vystupy
all: $(OBJ)
	make compile
	
#compile = binarna podoba v ./login
compile: $(PROGRAM)

#run 	 = spusti binarnu podobu
run: $(PROGRAM)
	./$(PROGRAM)
#doc  	 = vygeneruje dokumentaciu
doc:
	cd src
	doxygen $(DOXYFILE)

clean-doc:
	rm -rf doc
	
clean-exe:
	rm -rf $(PROGRAM)

clean-build:
	rm -rf $(BUILD_DIR)

#clean   = vymaze vsetky vytvorene subory
clean: clean-exe clean-build clean-doc

#uclean bez suboru
.PHONY: clean
	
$(PROGRAM): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LIBS) -o $@
	
#zavislosti g++ -MM *.cpp
$(BUILD_DIR)/CommandFactory.o: $(SRC_DIR)/CommandFactory.cpp $(SRC_DIR)/CommandFactory.h $(SRC_DIR)/ECommand.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)
$(BUILD_DIR)/DenseMatrix.o: $(SRC_DIR)/DenseMatrix.cpp $(SRC_DIR)/DenseMatrix.h $(SRC_DIR)/Matrix.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)
$(BUILD_DIR)/Matrix.o: $(SRC_DIR)/Matrix.cpp $(SRC_DIR)/Matrix.h $(SRC_DIR)/MatrixFactory.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)
$(BUILD_DIR)/MatrixCalculator.o: $(SRC_DIR)/MatrixCalculator.cpp $(SRC_DIR)/MatrixCalculator.h $(SRC_DIR)/Matrix.h $(SRC_DIR)/CommandFactory.h $(SRC_DIR)/ECommand.h $(SRC_DIR)/MatrixFactory.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)
$(BUILD_DIR)/MatrixFactory.o: $(SRC_DIR)/MatrixFactory.cpp $(SRC_DIR)/MatrixFactory.h $(SRC_DIR)/Matrix.h $(SRC_DIR)/SparseMatrix.h $(SRC_DIR)/DenseMatrix.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)
$(BUILD_DIR)/SparseMatrix.o: $(SRC_DIR)/SparseMatrix.cpp $(SRC_DIR)/SparseMatrix.h $(SRC_DIR)/Matrix.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/MatrixCalculator.h $(SRC_DIR)/Matrix.h $(SRC_DIR)/CommandFactory.h $(SRC_DIR)/ECommand.h $(SRC_DIR)/MatrixFactory.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LIBS)
	