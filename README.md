# simple_hand_digit_reader
simplest trained neural network + script for testing by tourself

AI.save - saved weights for NN, so if you dont want to spend time on training, you are able to use it.

b.cpp - the NN itself. You can import weiths in this script, train nn for better results, test it or just have fun

a.py - simple script woth interface for drawing digits by mouse, touchpad or something else.
  Hotkeys:
    c - clear the canvas
    p - shows interpolated version (you should not draw in  this mode)
    b - to back in normal mode after pressing p
    everything else saves current state of canvas to file that could be read by NN in b.cpp
    
Suggested protocol: 
  - run b.exe after compiling b.cpp
  - run a.py
  - do whatever you want (train nn / import settings, test weights / not)
  - after three questions you are able to try out your own digits painted and saved in a.py
  - do it while you want it
  - end
 
 
 thx ^v^ )
