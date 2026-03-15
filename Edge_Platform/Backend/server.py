from flask import Flask, request
import numpy as np
import cv2
import time

app = Flask(__name__)

@app.route('/upload', methods=['POST'])
def upload():

    print("\n---- Image Received ----")

    image_data = request.data

    if not image_data:
        print("No image received")
        return "ERROR", 400

    # Convert bytes to numpy array
    nparr = np.frombuffer(image_data, np.uint8)

    # Decode image
    img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)

    if img is None:
        print("Image decode failed")
        return "ERROR", 400

    # Save image for debugging
    filename = "received"+ ".jpg"
    cv2.imwrite(filename, img)

    print("Saved:", filename)

    # -------------------------
    # Dummy Classification
    # Replace with your AI model
    # -------------------------

    # Example logic
    height, width, _ = img.shape

    if width % 3 == 0:
        result = "RED"

    elif width % 3 == 1:
        result = "BLUE"

    else:
        result = "GREEN"

    print("Detected bin:", result)

    # Send response to ESP32
    return result, 200


if __name__ == '__main__':

    print("Starting Edge Server...")
    app.run(
        host="0.0.0.0",
        port=5000,
        debug=True
    )
