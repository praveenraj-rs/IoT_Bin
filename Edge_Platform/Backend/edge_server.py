from flask import Flask, request, jsonify
import numpy as np
import cv2
import json
import os
import requests
from datetime import datetime
from ultralytics import YOLO

# ─────────────────────────────────────────────
# CONFIG  ← edit these
# ─────────────────────────────────────────────
REMOTE_SERVER_URL = "http://192.168.75.95:3000"   # dashboard server
DUSTBIN_ID        = "1"                             # this edge's dustbin ID
DEFAULT_BIN       = "RED"

# ─────────────────────────────────────────────
# INIT
# ─────────────────────────────────────────────
app   = Flask(__name__)
model = YOLO("yolov8n.pt")

JSON_FILE = "bin_fill_status.json"
if not os.path.exists(JSON_FILE):
    with open(JSON_FILE, "w") as f:
        json.dump({}, f)

# ─────────────────────────────────────────────
# COCO → BIN MAPPING
# ─────────────────────────────────────────────
RED_BIN_SET = {
    "bottle", "cup", "wine glass", "plastic bag", "handbag",
    "backpack", "toothbrush", "hair drier", "remote",
    "keyboard", "mouse"
}
BLUE_BIN_SET = {
    "cell phone", "laptop", "tv", "microwave",
    "oven", "toaster", "refrigerator",
    "book", "clock", "scissors", "teddy bear"
}
GREEN_BIN_SET = {
    "banana", "apple", "orange", "broccoli",
    "carrot", "sandwich", "pizza", "hot dog",
    "cake", "donut"
}

def map_to_bin(label: str) -> str:
    label = label.lower().strip()
    if label in RED_BIN_SET:
        return "RED"
    elif label in BLUE_BIN_SET:
        return "BLUE"
    elif label in GREEN_BIN_SET:
        return "GREEN"
    return "UNKNOWN"

# ─────────────────────────────────────────────
# HELPERS
# ─────────────────────────────────────────────
def load_db() -> dict:
    with open(JSON_FILE, "r") as f:
        try:
            return json.load(f)
        except Exception:
            return {}

def save_db(db: dict):
    with open(JSON_FILE, "w") as f:
        json.dump(db, f, indent=4)

def forward_to_dashboard(dustbin_id: str, subdivision: str, fill_level: int):
    """
    Send the latest fill level to the remote dashboard server.
    Dashboard /data POST expects: { dustbin_id, subdivision, fill_level }
    """
    payload = {
        "dustbin_id": dustbin_id,
        "subdivision": subdivision.lower(),   # red / green / blue
        "fill_level":  fill_level
    }
    try:
        resp = requests.post(
            f"{REMOTE_SERVER_URL}/data",
            json=payload,
            timeout=5,
            verify=False   # set True if you have a valid cert
        )
        print(f"[FORWARD] → {REMOTE_SERVER_URL}/data | {payload} | status={resp.status_code}")
    except requests.exceptions.RequestException as e:
        print(f"[FORWARD ERROR] Could not reach dashboard server: {e}")

# ─────────────────────────────────────────────
# ROUTE: Image Upload + YOLO classification
# ─────────────────────────────────────────────
@app.route("/upload", methods=["POST"])
def upload_image():
    """
    End node POSTs a raw image here.
    Returns bin color string: RED / GREEN / BLUE
    """
    try:
        file_bytes = np.frombuffer(request.data, np.uint8)
        img = cv2.imdecode(file_bytes, cv2.IMREAD_COLOR)

        if img is None:
            print("[UPLOAD] Could not decode image")
            return "ERROR", 400

        print(f"[UPLOAD] Image shape: {img.shape}")
        results = model(img, imgsz=320)

        detected_labels = []
        for r in results:
            for box in r.boxes:
                cls_id = int(box.cls[0])
                detected_labels.append(model.names[cls_id])

        print(f"[YOLO] Detected: {detected_labels}")

        if not detected_labels:
            print(f"[DECISION] Nothing detected → {DEFAULT_BIN}")
            return DEFAULT_BIN

        for label in detected_labels:
            bin_color = map_to_bin(label)
            if bin_color != "UNKNOWN":
                print(f"[DECISION] {label} → {bin_color}")
                return bin_color

        print(f"[DECISION] All unknown → {DEFAULT_BIN}")
        return DEFAULT_BIN

    except Exception as e:
        print(f"[UPLOAD ERROR] {e}")
        return "ERROR", 500

# ─────────────────────────────────────────────
# ROUTE: Bin Fill from End Node
#
# End node sends:
#   POST /bin_fill
#   { "dustbin_id": "1", "subdivision": "red", "fill": 72 }
#
# Edge will:
#   1. Store fill level in local JSON
#   2. Forward it to the remote dashboard server
# ─────────────────────────────────────────────
@app.route("/bin_fill", methods=["POST"])
def bin_fill():
    try:
        data       = request.get_json(force=True)
        dustbin_id = str(data.get("dustbin_id", DUSTBIN_ID))
        bin_color  = str(data.get("subdivision", "red")).lower()  # red/green/blue 
        fill       = int(data.get("fill_level", 0))
        timestamp  = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        print(f"\n[FILL] Dustbin {dustbin_id} | {bin_color} = {fill}%  @ {timestamp}")

        # ── 1. Store locally ──────────────────────────────────
        db = load_db()

        if dustbin_id not in db:
            db[dustbin_id] = {
                "red":   {"level": 0, "time": ""},
                "green": {"level": 0, "time": ""},
                "blue":  {"level": 0, "time": ""}
            }

        sub_key = bin_color.lower()   # "red" / "green" / "blue"

        if sub_key in db[dustbin_id]:
            db[dustbin_id][sub_key] = {
                "level": fill,
                "time":  timestamp
            }
        else:
            print(f"[WARN] Unknown bin color '{bin_color}' — skipping local save")

        save_db(db)
        print(f"[LOCAL] Saved → {JSON_FILE}")

        # ── 2. Forward to remote dashboard server ─────────────
        forward_to_dashboard(dustbin_id, sub_key, fill)

        return jsonify({"status": "ok", "dustbin": dustbin_id, "bin": bin_color, "fill": fill})

    except Exception as e:
        print(f"[BIN_FILL ERROR] {e}")
        return jsonify({"status": "error", "detail": str(e)}), 500

# ─────────────────────────────────────────────
# ROUTE: Local status (optional debug endpoint)
# ─────────────────────────────────────────────
@app.route("/status", methods=["GET"])
def status():
    """Returns the locally stored fill levels for quick inspection."""
    return jsonify(load_db())

# ─────────────────────────────────────────────
# MAIN
# ─────────────────────────────────────────────
if __name__ == "__main__":
    print(f"[EDGE] Starting — Dustbin ID={DUSTBIN_ID}")
    print(f"[EDGE] Remote dashboard → {REMOTE_SERVER_URL}")
    app.run(host="0.0.0.0", port=5000, ssl_context="adhoc")
    #app.run(host="0.0.0.0", port=5000)   # ← use this if no SSL needed
