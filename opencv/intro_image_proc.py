import cv2
import numpy as np

# Destroy all opened opencv windows
cv2.destroyAllWindows()


# OPENCV setup a BGR format
# Height is the first size element
# Width is the second size element
# Channels is the last size element
# Create a new image and fill of red
size = 240, 320, 3
I = np.zeros(size, dtype=np.uint8)
print(I.shape)
for i in range(0,240):
    for j in range(0,320):
        I[i][j][0] = 0    # Blue
        I[i][j][1] = 0    # Green
        I[i][j][2] = 255  # Red

cv2.imshow('Image',I)
cv2.waitKey()

# ALUMNO ---------------------------------------------
# Draw a center straight line of blue color
width_half = int(320/2)
# Escribir su código aquí

cv2.imshow('Image',I)
cv2.waitKey()
# ---------------------------------------------------------------

# Read an image and get its size
I = cv2.imread('/home/pi/Pictures/logo.png')
print(I.shape)
cv2.imshow('Image',I)
cv2.waitKey()

# ALUMNO ------------------------------------------------------
# Convert to Grayscale
# 1. Create a new image called G of size:
size = I.shape[0],I.shape[1]

# 2. BGR to Grayscale conversion


#cv2.imshow('Grayscale',G)
#cv2.waitKey()
# ---------------------------------------------------------------


# WITH OPENCV FUNCTION
print("OpenCV function")
G = cv2.cvtColor(I,cv2.COLOR_BGR2GRAY)
print("Done")
cv2.imshow('Grayscale',G)
cv2.waitKey()


# Destroy all opened opencv windows
cv2.destroyAllWindows()