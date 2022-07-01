# simple_hand_digit_reader
simplest trained neural network + script for testing by yourself

AI.save - saved weights for NN. So if you don't want to spend time on training, you are able to use it.

nn.cpp - the NN itself. You can import weiths in this script, train NN for better results, test it or just have fun

a.py - simple script with interface for drawing digits by mouse, touchpad or something else.
  Hotkeys:
  - c - clear the canvas
  - p - shows interpolated version (you should not draw in  this mode) 
  - b - to back in normal mode after pressing p
  everything else saves current state of canvas to file that could be read by NN in nn.cpp
    
Suggested protocol: 
  - run b.exe after compiling b.cpp
  - run a.py
  - do whatever you want (train nn / import settings, test weights / not)
  - after three questions you are able to try out your own digits painted and saved in a.py
  - do it while you want it
  - end
 
*samples has been taken from MNIST datasets   http://yann.lecun.com/exdb/mnist/*

They contain half of the samples from MNIST (30000) and you shouldn't set epsize > 30000
Pretrained weights was get with epsize 50000. 
 
 thx ^v^ )
