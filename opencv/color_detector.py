import cv2
import numpy as np

# Destroy all opened opencv windows
cv2.destroyAllWindows()


# FILTER IMAGE BY COLOR
I = cv2.imread('/home/pi/Pictures/image.png')
# 1. Convert to HSV (Hue, Saturation, Value) colorspace
Ihsv = cv2.cvtColor(I,cv2.COLOR_BGR2HSV)

# 2. Setup a range of color values to filter
lower_value = np.array([110,50,50])
upper_value = np.array([130,255,255])

# 3. Generate a mask of such colors
mask = cv2.inRange(Ihsv,lower_value, upper_value)

# 4. Filter
Ifilt = cv2.bitwise_and(I,I,mask=mask)

# 5. Show image
cv2.imshow("Mask", mask)
cv2.imshow("Filtered image", Ifilt)
cv2.waitKey()

# Save the image
#cv2.imwrite(filename, frame)

# Where do the lower and upper values come from?
HSV_color = np.array([[[110,50,50]]],dtype=np.uint8)
BGR_color = cv2.cvtColor(HSV_color,cv2.COLOR_HSV2BGR)
print("BGR Lower Value")
print(BGR_color)
HSV_color = np.array([[[130,255,255]]],dtype=np.uint8)
BGR_color = cv2.cvtColor(HSV_color,cv2.COLOR_HSV2BGR)
print("BGR Upper Value")
print(BGR_color)

# ALUMNO ------------------------------------------------------
# Detect red objects
BGR_upper_value = np.array([[[0,0,255]]])
HSV_upper_value = cv2.cvtColor(BGR_upper_value,cv2.COLOR_BGR2HSV)
# Do the same for the lower value but choose a right BGR_lower_value value, e.g., in Microsoft Paint

# Apply color detection algorithm

# Save image using
#cv2.imwrite(filename, THE_PROCESSED_IMAGE_VARIABLE_NAME)

# ---------------------------------------------------------------



# Destroy all opened opencv windows
cv2.destroyAllWindows()