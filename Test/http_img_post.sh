#!/bin/bash

# ==============================
# Configuration Variables
# ==============================

IMAGE_FILE="test.jpg"
SERVER_URL="http://localhost:5000/upload"
CONTENT_TYPE="image/jpeg"

# ==============================
# Check if image exists
# ==============================

if [ ! -f "$IMAGE_FILE" ]; then
    echo "Error: Image file '$IMAGE_FILE' not found!"
    exit 1
fi

# ==============================
# Send POST Request
# ==============================

echo "Sending image: $IMAGE_FILE"
echo "Server: $SERVER_URL"
echo "--------------------------------"

curl -X POST "$SERVER_URL" \
     -H "Content-Type: $CONTENT_TYPE" \
     --data-binary @"$IMAGE_FILE"

echo ""
echo "--------------------------------"
echo "Request completed"
