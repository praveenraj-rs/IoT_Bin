from flask import Flask, request, jsonify, send_from_directory
import os
import json
from datetime import datetime

app = Flask(__name__)

DATA_FILE = "dustbin_data.json"

# Create JSON file if not present
if not os.path.exists(DATA_FILE):
    with open(DATA_FILE, "w") as f:
        json.dump({}, f)

# ---------- HTML Dashboard ----------
@app.route("/")
def dashboard():
    return send_from_directory(".", "index.html")

# ---------- Receive data from EDGE ----------
@app.route('/data', methods=['POST'])
def receive_data():
    try:
        data = request.json

        dustbin_id = str(data["dustbin_id"])
        subdivision = data["subdivision"].lower()   # red/green/blue
        level = int(data["fill_level"])
        time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        # Load existing data
        with open(DATA_FILE, "r") as f:
            db = json.load(f)

        # Initialize if new dustbin
        if dustbin_id not in db:
            db[dustbin_id] = {
                "red": {"level": 0, "time": ""},
                "green": {"level": 0, "time": ""},
                "blue": {"level": 0, "time": ""}
            }

        # Update ONLY that subdivision
        db[dustbin_id][subdivision] = {
            "level": level,
            "time": time
        }

        # Save back
        with open(DATA_FILE, "w") as f:
            json.dump(db, f, indent=4)

        print(f"[EDGE] Dustbin {dustbin_id} | {subdivision} = {level}%")

        return jsonify({"status": "updated"})

    except Exception as e:
        print("Error:", e)
        return jsonify({"status": "error"}), 500


# ---------- Send ALL data ----------
@app.route('/data', methods=['GET'])
def send_data():
    try:
        with open(DATA_FILE, "r") as f:
            db = json.load(f)
        return jsonify(db)
    except Exception as e:
        print("Error:", e)
        return jsonify({})


# ---------- Get latest (dashboard friendly) ----------
@app.route('/latest', methods=['GET'])
def get_latest():
    try:
        with open(DATA_FILE, "r") as f:
            db = json.load(f)

        # Format for dashboard (optional)
        formatted = {}

        for dustbin_id, bins in db.items():
            formatted[f"bin{dustbin_id}"] = bins

        return jsonify(formatted)

    except Exception as e:
        print("Error:", e)
        return jsonify({})


# ---------- MAIN ----------
if __name__ == '__main__':
    #app.run(host="0.0.0.0",port=3000, debug=True)
    app.run(host="0.0.0.0",port=3000)
