# qt-paint
Example to show how to use mouse events to draw to a widget

This code is not optimized but may serve as a starting point for someone who is interested.

What I do here is: 
- Create a QImage the size of the widget
- Set pixels on the QImage when events come in (mouse press & mouse move)
- tell the widget to repaint itself after pixels are manipulated
- handle paintEvent to draw the QImage as a pixmap, starting at teh top left corner of the widget

What I didn't do,what can be fixed/improved if I wanted to keep making a paint app:
- Not optimized
- Resizing the widget will clear the image, this is not nice
