from flask import Flask, request, jsonify, send_from_directory
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import os

app = Flask(__name__)

# SQLite DB config
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///dustbin.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

db = SQLAlchemy(app)

# ---------- DATABASE MODEL ----------
class Dustbin(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    dustbin_id = db.Column(db.String(50))
    subdivision = db.Column(db.String(10))  # red/green/blue
    level = db.Column(db.Integer)
    time = db.Column(db.DateTime, default=datetime.utcnow)

# Create DB
with app.app_context():
    db.create_all()

# ---------- HTML Dashboard ----------
@app.route("/")
def dashboard():
    return send_from_directory(".", "index.html")

# ---------- Receive data ----------
@app.route('/data', methods=['POST'])
def receive_data():
    try:
        data = request.json

        dustbin_id = str(data["dustbin_id"])
        subdivision = data["subdivision"].lower()
        level = int(data["fill_level"])

        entry = Dustbin(
            dustbin_id=dustbin_id,
            subdivision=subdivision,
            level=level,
            time=datetime.now()
        )

        db.session.add(entry)
        db.session.commit()

        print(f"[EDGE] Dustbin {dustbin_id} | {subdivision} = {level}%")

        return jsonify({"status": "stored in DB"})

    except Exception as e:
        print("Error:", e)
        return jsonify({"status": "error"}), 500

# ---------- Get ALL data ----------
@app.route('/data', methods=['GET'])
def send_data():
    try:
        entries = Dustbin.query.all()

        result = []
        for e in entries:
            result.append({
                "dustbin_id": e.dustbin_id,
                "subdivision": e.subdivision,
                "level": e.level,
                "time": e.time.strftime("%Y-%m-%d %H:%M:%S")
            })

        return jsonify(result)

    except Exception as e:
        print("Error:", e)
        return jsonify([])

# ---------- Latest per bin ----------
@app.route('/latest', methods=['GET'])
def get_latest():
    try:
        bins = {}

        entries = Dustbin.query.order_by(Dustbin.time.desc()).all()

        for e in entries:
            if e.dustbin_id not in bins:
                bins[e.dustbin_id] = {}

            if e.subdivision not in bins[e.dustbin_id]:
                bins[e.dustbin_id][e.subdivision] = {
                    "level": e.level,
                    "time": e.time.strftime("%Y-%m-%d %H:%M:%S")
                }

        return jsonify(bins)

    except Exception as e:
        print("Error:", e)
        return jsonify({})

# ---------- MAIN ----------
if __name__ == '__main__':
    app.run(host="0.0.0.0", port=3000,
            ssl_context=('cert.pem', 'key.pem'))
