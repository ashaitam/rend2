# 26.03.2026 20:58 Abdurrahman : CRÉER LE Makefile pour compiler.
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

OFILES = project.o tools.o message.o brick.o ball.o paddle.o game.o

project: $(OFILES)
	$(CXX) $(CXXFLAGS) $(OFILES) -o project

project.o: project.cc tools.h message.h constants.h brick.h ball.h paddle.h game.h
	$(CXX) $(CXXFLAGS) -c project.cc

tools.o: tools.cc tools.h
	$(CXX) $(CXXFLAGS) -c tools.cc

message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) -c message.cc

brick.o: brick.cc brick.h tools.h constants.h message.h
	$(CXX) $(CXXFLAGS) -c brick.cc

ball.o: ball.cc ball.h tools.h constants.h message.h
	$(CXX) $(CXXFLAGS) -c ball.cc

paddle.o: paddle.cc paddle.h tools.h constants.h message.h
	$(CXX) $(CXXFLAGS) -c paddle.cc

game.o: game.cc game.h brick.h ball.h paddle.h tools.h message.h constants.h
	$(CXX) $(CXXFLAGS) -c game.cc

clean:
	rm -f *.o project
