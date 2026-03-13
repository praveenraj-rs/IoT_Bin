from flask import Flask, request
import paho.mqtt.client as mqtt
import cv2
import numpy as np

app = Flask(__name__)

mqtt_client = mqtt.Client()
mqtt_client.connect("localhost",1883,60)

@app.route('/upload', methods=['POST'])
def upload():

    image_data = request.data

    nparr = np.frombuffer(image_data, np.uint8)
    img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)

    cv2.imwrite("received.jpg", img)

    # Dummy detection logic
    result = "BLUE"

    mqtt_client.publish("smartbin/bin_action", result)

    return "Image received", 200


app.run(host='0.0.0.0', port=5000)
