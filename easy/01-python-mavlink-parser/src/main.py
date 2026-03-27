from parser import parse_mavlink_file, extract_gps_data, extract_speed_data, calculate_flight_stats

messages = parse_mavlink_file('../data/mavlink_log.bin')
gps_data = extract_gps_data(messages)
speed_data = extract_speed_data(messages)
stats = calculate_flight_stats(gps_data, speed_data)

for i, point in enumerate(speed_data[:5]):
    print(point['alt'])

print(f"Максимальная высота: {stats['max_altitude']:.1f} м")
print(f"Средняя скорость: {stats['avg_speed']:.1f} м/с")
print(f"Пройденное расстояние: {stats['distance']:.1f} м")