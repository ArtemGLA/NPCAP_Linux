from analyzer import *

df = load_flight_log('data/flight_log.csv')

anomalies = {
    'altitude': detect_altitude_anomalies(df),
    'gps': detect_gps_anomalies(df),
    'battery': detect_battery_anomalies(df),
    'speed': detect_speed_anomalies(df),
    'attitude': detect_attitude_anomalies(df),
}

report = generate_report(anomalies)
print(report)