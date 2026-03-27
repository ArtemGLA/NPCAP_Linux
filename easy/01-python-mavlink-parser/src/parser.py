"""
MAVLink Parser - парсер бинарных MAVLink лог-файлов.

Задание: реализуйте функции для парсинга MAVLink сообщений
и извлечения телеметрических данных.
"""

import math
import struct
from typing import Any


# MAVLink v1 константы
MAVLINK_STX_V1 = 0xFE
MAVLINK_STX_V2 = 0xFD

# Message IDs
MSG_HEARTBEAT = 0
MSG_GPS_RAW_INT = 24
MSG_GLOBAL_POSITION_INT = 33
MSG_VFR_HUD = 74


def parse_mavlink_file(filepath: str) -> list[dict]:
    """
    Парсит бинарный MAVLink файл и возвращает список сообщений.
    
    Каждый MAVLink v1 пакет имеет структуру:
    - STX (1 байт): 0xFE
    - LEN (1 байт): длина payload
    - SEQ (1 байт): sequence number
    - SYS (1 байт): system ID
    - COMP (1 байт): component ID
    - MSG (1 байт): message ID
    - PAYLOAD (LEN байт): данные
    - CRC (2 байта): контрольная сумма
    
    Args:
        filepath: путь к бинарному файлу
        
    Returns:
        Список словарей с полями:
        - msg_id: ID сообщения
        - system_id: ID системы
        - component_id: ID компонента
        - sequence: sequence number
        - payload: байты payload
    """
    # TODO: Реализуйте парсинг MAVLink файла
    # 
    # Подсказка:
    # 1. Откройте файл в бинарном режиме
    # 2. Ищите стартовый байт 0xFE
    # 3. Читайте заголовок (5 байт после STX)
    # 4. Читайте payload согласно полю LEN
    # 5. Пропускайте CRC (2 байта)
    # 6. Добавляйте сообщение в список
    
    messages = []

    with open(filepath, 'rb') as f:
        data = f.read()
    
    i = 0
    data_len = len(data)
    
    while i < data_len:
        # Ищем стартовый байт 0xFE (MAVLink v1)
        if data[i] != 0xFE:
            i += 1
            continue
        
        # Читаем заголовок (5 байт после STX)
        # LEN, SEQ, SYS, COMP, MSG
        header = data[i+1:i+6]  # 5 байт
        payload_len = header[0]
        
        # Извлекаем поля
        seq = header[1]
        sys_id = header[2]
        comp_id = header[3]
        msg_id = header[4]
        
        # Payload
        payload_start = i + 6
        payload = data[payload_start:payload_start + payload_len]
        
        # CRC (два байта, little-endian)
        crc_start = payload_start + payload_len
        crc = struct.unpack('<H', data[crc_start:crc_start+2])[0]
        
        # Сохраняем сообщение
        messages.append({
            'msg_id': msg_id,
            'system_id': sys_id,
            'component_id': comp_id,
            'sequence': seq,
            'payload': payload,
            'crc': crc 
        })
        
        # Переходим к следующему байту после окончания текущего пакета

        # Вычисляем полный размер пакета: STX(1) + заголовок(5) + payload + CRC(2)
        packet_end = i + 6 + payload_len + 2
        i = packet_end

    return messages


def extract_gps_data(messages: list[dict]) -> list[dict]:
    """
    Извлекает GPS данные из GLOBAL_POSITION_INT сообщений.
    
    Структура GLOBAL_POSITION_INT payload (28 байт):
    - time_boot_ms (uint32): время в мс
    - lat (int32): широта * 1e7
    - lon (int32): долгота * 1e7
    - alt (int32): абсолютная высота в мм
    - relative_alt (int32): относительная высота в мм
    - vx (int16): скорость X в см/с
    - vy (int16): скорость Y в см/с
    - vz (int16): скорость Z в см/с
    - hdg (uint16): курс в сотых долях градуса
    
    Args:
        messages: список сообщений от parse_mavlink_file()
        
    Returns:
        Список словарей с полями:
        - timestamp: время в секундах
        - lat: широта в градусах
        - lon: долгота в градусах
        - alt: высота в метрах
        - relative_alt: относительная высота в метрах
    """
    # TODO: Реализуйте извлечение GPS данных
    #
    # Подсказка:
    # 1. Фильтруйте сообщения по msg_id == MSG_GLOBAL_POSITION_INT
    # 2. Используйте struct.unpack('<IiiiihhhH', payload) для распаковки
    # 3. Конвертируйте: lat/lon делите на 1e7, alt/relative_alt делите на 1000
    
    gps_data = []
    
    # Формат распаковки: < (little-endian), I (uint32), i (int32) x4, h (int16) x3, H (uint16)
    fmt = '<IiiiihhhH'
    expected_size = struct.calcsize(fmt)  # должно быть 28

    for msg in messages:
        if msg['msg_id'] != MSG_GLOBAL_POSITION_INT:  # Только GLOBAL_POSITION_INT
            continue

        payload = msg['payload']

        # Распаковываем кортеж из 9 элементов
        (time_boot_ms, lat, lon, alt, relative_alt, vx, vy, vz, hdg) = struct.unpack(fmt, payload)

        # Преобразуем в физические величины
        gps_entry = {
            'timestamp': time_boot_ms / 1000.0,        # секунды
            'lat': lat / 1e7,                           # градусы
            'lon': lon / 1e7,                           # градусы
            'alt': alt / 1000.0,                         # метры
            'relative_alt': relative_alt / 1000.0        # метры
        }
        gps_data.append(gps_entry)
    
    return gps_data


def extract_speed_data(messages: list[dict]) -> list[dict]:
    """
    Извлекает данные о скорости из VFR_HUD сообщений.
    
    Структура VFR_HUD payload (20 байт):
    - airspeed (float): воздушная скорость м/с
    - groundspeed (float): наземная скорость м/с
    - alt (float): высота м
    - climb (float): вертикальная скорость м/с
    - heading (int16): курс в градусах
    - throttle (uint16): газ в %
    
    Args:
        messages: список сообщений от parse_mavlink_file()
        
    Returns:
        Список словарей с полями:
        - airspeed: воздушная скорость м/с
        - groundspeed: наземная скорость м/с
        - climb: вертикальная скорость м/с
        - heading: курс в градусах
    """
    # TODO: Реализуйте извлечение данных о скорости
    #
    # Подсказка:
    # 1. Фильтруйте сообщения по msg_id == MSG_VFR_HUD
    # 2. Используйте struct.unpack('<ffffhH', payload) для распаковки
    
    speed_data = []
    
    for msg in messages:
        if(msg['msg_id'] != MSG_VFR_HUD):
            continue
    
        payload = msg['payload']
        fmt = '<ffffhH'
        (airspeed, groundspeed, alt, climb, heading, throttle) = struct.unpack(fmt, payload)

        stat = {
            'airspeed' : airspeed,
            'groundspeed' : groundspeed,
            'alt' : alt,
            'climb' : climb,
            'heading' : heading,
            'throttle' : throttle,
        }
        
        speed_data.append(stat)
    
    return speed_data


def haversine_distance(lat1: float, lon1: float, lat2: float, lon2: float) -> float:
    """
    Вычисляет расстояние между двумя GPS точками по формуле Хаверсина.
    
    Args:
        lat1, lon1: координаты первой точки в градусах
        lat2, lon2: координаты второй точки в градусах
        
    Returns:
        Расстояние в метрах
    """
    R = 6371000  # Радиус Земли в метрах
    
    phi1 = math.radians(lat1)
    phi2 = math.radians(lat2)
    delta_phi = math.radians(lat2 - lat1)
    delta_lambda = math.radians(lon2 - lon1)
    
    a = math.sin(delta_phi / 2) ** 2 + \
        math.cos(phi1) * math.cos(phi2) * math.sin(delta_lambda / 2) ** 2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    
    return R * c


def calculate_flight_stats(gps_data: list[dict], speed_data: list[dict]) -> dict:
    """
    Вычисляет статистику полёта.
    
    Args:
        gps_data: данные GPS от extract_gps_data()
        speed_data: данные скорости от extract_speed_data()
        
    Returns:
        Словарь с полями:
        - max_altitude: максимальная относительная высота (м)
        - min_altitude: минимальная относительная высота (м)
        - avg_speed: средняя скорость (м/с)
        - max_speed: максимальная скорость (м/с)
        - distance: общее пройденное расстояние (м)
        - duration: длительность полёта (с)
    """
    # TODO: Реализуйте расчёт статистики
    #
    # Подсказка:
    # 1. Для высоты используйте relative_alt из gps_data
    # 2. Для скорости используйте groundspeed из speed_data
    # 3. Для расстояния суммируйте haversine_distance между последовательными точками
    # 4. Для длительности используйте разницу timestamp первой и последней точки
    
    stats = {
        'max_altitude': 0.0,
        'min_altitude': 0.0,
        'avg_speed': 0.0,
        'max_speed': 0.0,
        'distance': 0.0,
        'duration': 0.0,
    }

    max_altitude = 0.0
    for gps in gps_data:
        if max_altitude < gps['relative_alt']:
            max_altitude = gps['relative_alt']
    stats['max_altitude'] = max_altitude
    
    min_altitude = max_altitude
    for gps in gps_data:
        if min_altitude < gps['relative_alt']:
            min_altitude = gps['relative_alt']
    stats['min_altitude'] = min_altitude

    speed_sum = 0
    if len(speed_data) != 0:
        for speed in speed_data:
            speed_sum += speed['groundspeed']
        avg_speed = speed_sum / len(speed_data)
        stats['avg_speed'] = avg_speed

    max_speed = 0.0
    for speed in speed_data:
        if max_speed < speed['groundspeed']:
            max_speed = speed['groundspeed']
    stats['max_speed'] = max_speed

    size_gps = len(gps_data)
    distance = 0.0
    for i in range (1, size_gps):
        gps_start = gps_data[i - 1]
        gps_end = gps_data[i]
        distance = distance + haversine_distance(gps_start['lat'], gps_start['lon'], gps_end['lat'], gps_end['lon'])
    stats['distance'] = distance

    if len(gps_data) > 1:
        timestamp = gps_data[len(gps_data) - 1]['timestamp'] - gps_data[0]['timestamp']

    return stats

def main():
    """Пример использования парсера."""
    import os
    
    # Путь к тестовому файлу
    data_dir = os.path.join(os.path.dirname(__file__), '..', 'data')
    log_file = os.path.join(data_dir, 'mavlink_log.bin')
    
    if not os.path.exists(log_file):
        print(f"Файл {log_file} не найден")
        print("Скопируйте mavlink_log.bin из common/test-data/")
        return
    
    # Парсинг файла
    messages = parse_mavlink_file(log_file)
    print(f"Прочитано сообщений: {len(messages)}")
    
    # Извлечение данных
    gps_data = extract_gps_data(messages)
    speed_data = extract_speed_data(messages)
    print(f"GPS точек: {len(gps_data)}")
    print(f"Данных скорости: {len(speed_data)}")
    
    # Статистика
    stats = calculate_flight_stats(gps_data, speed_data)
    print("\nСтатистика полёта:")
    print(f"  Макс. высота: {stats['max_altitude']:.1f} м")
    print(f"  Средняя скорость: {stats['avg_speed']:.1f} м/с")
    print(f"  Макс. скорость: {stats['max_speed']:.1f} м/с")
    print(f"  Пройдено: {stats['distance']:.1f} м")


if __name__ == '__main__':
    main()
