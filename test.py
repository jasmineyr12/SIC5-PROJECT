from flask import Flask, request, jsonify

app = Flask(_name_)

# Define the base variable globally
base = []

@app.route('/tampilsuhu', methods=['POST'])
def receive_suhu():
    global base
    ambil = request.get_json()
    terimasuhu = ambil['Suhu']
    terimahum = ambil['kelembapan']
    kumpul = [terimasuhu, terimahum]

    # Update the global base variable with received data
    base = kumpul


    # Log the received data
    print("Sampai di lokasi")
    print("- Suhu:", terimasuhu, "°C, humidity:", terimahum, "%")
    
    # Send response
    response = {'message': 'Data diterima'}
    return jsonify(response), 200

@app.route('/tampilsuhu', methods=['GET'])
def dapat_data():
    global base
    # Return the base data as a JSON response
    return jsonify({'Temperature': base[0], 'Humidity': base[1]}) if base else jsonify({'message': 'No data available'})

if _name_ == '_main_':
    app.run(debug=True, host='0.0.0.0', port=5000)