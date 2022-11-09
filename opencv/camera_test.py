# Import OpenCV library
import cv2

# Open camera
cam = cv2.VideoCapture(0)

while True:
    # Read a frame from camera
    hasFrame,frame = cam.read()
    width = int(cam.get(3))
    height = int(cam.get(4))
    
    # By default, opencv use
    cv2.imshow('frame',frame)
    #cv2.imwrite(filename, frame)
    
    if cv2.waitKey(1)==ord('q'):
        break

cam.release()
cv2.destroyAllWindows()