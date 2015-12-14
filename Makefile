PROGRAM=udiff
OBJECTS=diff.o getucdscript.o main.o optlong.o slurp.o token.o unified.o

CXX=clang++ -std=c++11
CXXFLAGS=-Wall -O2

$(PROGRAM) : $(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS)

diff.o : diff.cpp diff.hpp
	$(CXX) $(CXXFLAGS) -c -o diff.o diff.cpp

getucdscript.o : getucdscript.cpp getucdscript.hpp
	$(CXX) $(CXXFLAGS) -c -o getucdscript.o getucdscript.cpp

main.o : main.cpp optlong.hpp diff.hpp slurp.hpp unified.hpp
	$(CXX) $(CXXFLAGS) -c -o main.o main.cpp

optlong.o : optlong.cpp optlong.hpp
	$(CXX) $(CXXFLAGS) -c -o optlong.o optlong.cpp

slurp.o : slurp.cpp slurp.hpp diff.hpp
	$(CXX) $(CXXFLAGS) -c -o slurp.o slurp.cpp

token.o : token.cpp diff.hpp getucdscript.hpp
	$(CXX) $(CXXFLAGS) -c -o token.o token.cpp

unified.o : unified.cpp unified.hpp diff.hpp
	$(CXX) $(CXXFLAGS) -c -o unified.o unified.cpp

clean :
	rm -fr $(PROGRAM) $(OBJECTS)
